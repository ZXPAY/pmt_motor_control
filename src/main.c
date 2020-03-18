/**
 * @file main.c
 * @author Xiang-Guan Deng
 * @brief Main code for motor control board
 * @date 2020.xx.xx
 *
 * Compiler: arm-none-eabi-gcc (8.3.1)
 * TODO: overflow problem (0~360)
 */

/* Include C standard library */
#include <stdbool.h>
#include <stdio.h>

/* Include hardware library */
#include "uart.h"
#include "system.h"
#include "cortex_m4.h"
#include "fpu.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup

/* Define parameters */
#define EXC_KI        0.01            /* 激磁角Ki回饋 */
#define I_SVPWM_KP    1/360.0         /* 電流Kp回饋 */
#define I_SVPWM_KI    0.1/360.0       /* 電流Ki回饋 */
#define I_SVPWM_LOW   0.4             /* 電流下限 */
#define I_SVPWM_HIGH  0.4             /* 電流上限 */
#define STEP_C_THETA_LENGTH   1.0     /* 命令微步累加器角度轉換成長度增益 */
#define STEP_S_THETA_LENGTH   1.0     /* 感測微步累加器角度轉換成長度增益 */

/* Include other library */
#include "hal_tick.h"

/* Include board support package */
#include "control_board_v2.h"
#include "hal_as5047d.h"
#include "hal_drv8847.h"
#include "rs485.h"

extern drv8847_t drv8847;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

/* Include control library */
#include "sin_cos_val_table.h"
#include "ele_angle.h"
#include "adj_velocity.h"
#include "pi_current.h"
#include "i_excite_angle.h"
#include "svpwm.h"
#include "step_accumulator.h"

/* Define stepper motor control object */
sangle_t sangle;                        /* 感測角 */
cangle_t cangle;                        /* 命令角 */
adj_v_t adj_v;                          /* 相位調整限速器 */
fb_exc_angle_t fb_exc_angle;            /* 激磁角回饋 */
fb_current_t fb_current;                /* 電流回饋 */
pwmAB_t pwm12;                          /* 1A1B 2A2B PWM */
extern step_caccumulator_t c_accum;     /* 命令微步累加器, 在ele_angle.c內初始化 */
extern step_saccumulator_t s_accum;     /* 感測微步累加器, 在ele_angle.c內初始化 */

/* Define default N-step */
#define N_STEP 1

/* Main code */
int main (void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    /* Initilize hardware */
    board_init();
    enable_fpu();

    RS485_trm("initialize \n");
    as5047d.init();
    drv8847.init();

    /* Initialize sin, cos table, call get_sin() and get_cos() to get current value  */
    init_sin_cos_table(PERIOD_COUNT, N_STEP);

    /* Initialize add adjust calculator */
    init_cangle_inc(&adj_v);
    set_cangle_wback(&adj_v, 1);

    /* Initialize excited angle feedback */
    init_exc_ang_para(&fb_exc_angle, EXC_KI);

    /* Initialize current feedback */
    init_current_para(&fb_current, I_SVPWM_KP, I_SVPWM_KI, I_SVPWM_LOW, I_SVPWM_HIGH);

    /* Initialize step accumulator */
    set_caccum_k(&c_accum, STEP_C_THETA_LENGTH);
    set_saccum_k(&s_accum, STEP_S_THETA_LENGTH);

    // Initialize SysTick, setting slot 1 ms
    SYST_RVR = 24000;
    SYST_CSR = 0x07;
    hal_delay(500);

    /* Initialize machanical angle */
    for(int i=0;i<16;i++) {
        as5047d.update();    // update encoder angle to be first machanical angle
    }
    init_sangle(&sangle, as5047d.angle);
    init_cangle(&cangle, N_STEP, 0);

    RS485_trm("start\n");


    // Enable interrupt
    __enable_irqn(FTM0_IRQn);
    __enable_irqn(FTM1_IRQn);
    __enable_irqn(UART1_RX_TX_IRQn);
    uint32_t aa = 1000;
    uint32_t cnt = 0;
    while (true) {
        as5047d.update();

        update_sangle(&sangle, as5047d.angle);
        cal_exc_ang_correct(&fb_exc_angle, sangle.ele_dangle, cangle.ele_dangle);
        cal_current_correct(&fb_exc_angle, &fb_current);
        cal_pwmAB(&pwm12, &fb_exc_angle, &fb_current);

        if(cnt % 1000 == 0) {
            update_cangle(&cangle, get_cangle_inc(&adj_v));
        }

        RS485_trm(", %.3f, %.3f, %d, %.3f, %.3f, %.2f, %.2f, %.2f, %.2f, %ld, %ld, \n", drv8847.i1, drv8847.i2, as5047d.angle, sangle.ele_dangle, cangle.ele_dangle,
                                                    fb_exc_angle.th_esvpwm, fb_current.i_svpwm, fb_exc_angle.th_er, fb_exc_angle.th_cum, pwm12.pwma, pwm12.pwmb);
        // RS485_trm("%x, %x, %x, %x\n", drv8847.drv->i2c_read(0), drv8847.drv->i2c_read(1), drv8847.drv->i2c_read(2), drv8847.drv->i2c_read(3));
        hal_delay(100);
        aa += 1000;
        cnt++;
    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}
