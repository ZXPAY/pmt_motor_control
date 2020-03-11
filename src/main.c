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
#define EXC_KI        0.1         /* 激磁角Ki回饋 */
#define I_SVPWM_KP    1.5         /* 電流Kp回饋 */
#define I_SVPWM_KI    0.1         /* 電流Ki回饋 */
#define I_SVPWM_LOW   -200.0      /* 電流下限 */
#define I_SVPWM_HIGH  200.0       /* 電流上限 */

/* RS485 transmit macro */
#define RS485_trm(format, args...) printf("\r[RS485] "format, ##args)

/* Include other library */
#include "hal_tick.h"

/* Include board support package */
#include "control_board_v2.h"
#include "hal_as5047d.h"
#include "hal_drv8847.h"

extern drv8847_t drv8847;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

/* Include control library */
#include "sin_cos_val_table.h"
#include "ele_angle.h"
#include "adj_velocity.h"
#include "pi_current.h"
#include "i_excite_angle.h"
#include "svpwm.h"

/* Define stepper motor control object */
sangle_t sangle;                 /* 感測角 */
cangle_t cangle;                 /* 命令角 */
adj_v_t adj_v;                   /* 相位調整限速器 */
fb_exc_angle_t fb_exc_angle;     /* 激磁角回饋 */
fb_current_t fb_current;         /* 電流回饋 */
pwmAB_t AB1;                     /* 1A 1B PWM */
pwmAB_t AB2;                     /* 2A 2B PWM */

/* Define default N-step */
#define N_STEP 1

/* Main code */
int main (void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    /* Initilize hardware */
    uart_init();
    init_hw_as5047d();
    init_hw_drv8847();
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
    __enable_irqn(ADC0_IRQn);

    uint32_t cnt = 0;
    while (true) {
        as5047d.update();

        drv8847.trigger();
        drv8847.update_current();

        update_sangle(&sangle, as5047d.angle);

        if(cnt % 1000 == 0) {
            cal_exc_ang_correct(&fb_exc_angle, sangle.ele_dangle, cangle.ele_dangle);
            cal_current_correct(&fb_exc_angle, &fb_current);
            update_cangle(&cangle, get_cangle_inc(&adj_v));
            // set_cangle_inc(&adj_v, 1);
            // RS485_trm("%.2f, %.2f\n", sin(fb_exc_angle.th_esvpwm), sin(fb_exc_angle.th_esvpwm)*drv8847.drv->period1);
            // drv8847.drv->set_duty1((uint16_t) abs_float(sin(fb_exc_angle.th_esvpwm)*drv8847.drv->period1));
            // drv8847.drv->set_duty2((uint16_t) abs_float(cos(fb_exc_angle.th_esvpwm)*drv8847.drv->period2));
        }
        if(cnt % 10000 == 0) {
            RS485_trm("%.3f, %.3f, %d, %.3f, %.3f, %.2f, %.2f, %.2f, %.2f\n", drv8847.current_Rsense1, drv8847.current_Rsense2, as5047d.angle, sangle.ele_dangle, cangle.ele_dangle,
                                                    fb_exc_angle.th_esvpwm, fb_current.i_svpwm, fb_exc_angle.th_er, fb_exc_angle.th_cum);
        }
        // if(cnt % 2000 == 0) {
        //     if(fg) drv8847.clockwise();
        //     else drv8847.counterclockwise();
        //     fg ^= 1;
        // }
        cnt++;
    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}
