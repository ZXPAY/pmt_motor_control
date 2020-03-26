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
#include <string.h>

/* Include hardware library */
#include "uart.h"
#include "system.h"
#include "cortex_m4.h"
#include "fpu.h"
#include "drv8847_s.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "control_config.h"

/* Include other library */
#include "hal_tick.h"

/* Include board support package */
#include "control_board.h"
#include "hal_as5047d.h"
#include "hal_drv8847.h"
#include "tick.h"
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
extern step_caccumulator_t c_accum;     /* 命令微步累加器, 在control/ele_angle.c內初始化 */
extern step_saccumulator_t s_accum;     /* 感測微步累加器, 在control/ele_angle.c內初始化 */

volatile uint32_t presc_cnt = 0;

/* Main code */
int main (void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    /* Initilize hardware */
    board_init();
    enable_fpu();
    enable_tick();

    RS485_trm("initialize hardware ...\r\n");
    as5047d.init();
    drv8847.init();
    if(drv8847.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }

    /* Initialize sin, cos table, call get_sin() and get_cos() to get current value */
    /* Use in test */
    // init_sin_cos_table(PERIOD_COUNT, N_STEP);

    /* Initialize add adjust calculator */
    init_cangle_inc(&adj_v);
    set_cangle_wback(&adj_v, 1);

    /* Initialize excited angle feedback */
    init_exc_ang_para(&fb_exc_angle, EXC_KI);

    /* Initialize current feedback */
    init_current_para(&fb_current, I_SVPWM_KP, I_SVPWM_KI, I_SVPWM_LOW, I_SVPWM_HIGH);

    /* Initialize step accumulator */
    set_caccum_k(&c_accum, STEP_C_THETA_TO_LENGTH);
    set_saccum_k(&s_accum, STEP_S_THETA_TO_LENGTH);

    hal_delay(500);

    /* Initialize machanical angle */
    for(int i=0;i<16;i++) {
        as5047d.update();    // update encoder angle to be first machanical angle
    }
    init_sangle(&sangle, as5047d.angle);
    init_cangle(&cangle, N_STEP, 0);


    /* SysTick initialize */
    systick_init();
    RS485_trm("start \r\n");
    hal_delay(100);

    // Enable interrupt
    __enable_irqn(FTM0_IRQn);
    __enable_irqn(FTM1_IRQn);
    __enable_irqn(PIT0_IRQn);
    __enable_irqn(PIT1_IRQn);

    uint32_t cnt = 0;
    while (true) {
        cnt++;
        /* idle */
    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}

/* period : 2 ms */
void PIT0_IRQHandler(void) {
    as5047d.update();
    update_sangle(&sangle, as5047d.angle);
    cal_exc_ang_correct(&fb_exc_angle, sangle.ele_dangle, cangle.ele_dangle);
    cal_current_correct(&fb_exc_angle, &fb_current);
    cal_pwmAB(&pwm12, &fb_exc_angle, &fb_current);

    SET_PHASEA_DUTY(pwm12.pwm1);
    SET_PHASEB_DUTY(pwm12.pwm2);

    /* clear flag */
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
}


/* period : 10 ms */
/* Do experiment, print whole words need 7 ms => choose 100 Hz */
void PIT1_IRQHandler(void) {
    /* Test code */
    /*
    int32_t temp_sin = get_sin();
    int32_t temp_cos = get_cos();
    RS485_trm("===== update =====, %ld, %ld, %ld\n", temp_sin, temp_cos, as5047d.angle);

    SET_PHASEA_DUTY((temp_sin+PERIOD_COUNT)>>1);
    SET_PHASEB_DUTY((temp_cos+PERIOD_COUNT)>>1);
    */

    /* i1, i2, angle, sangle, cangle, th_svpwm, i_svpwm, th_er, th_cum, pwm1, pwm2 */
    RS485_trm(", %d, %d, %d, %.3f, %.2f, %.2f, %.2f, %.2f, %.2f, %ld, %ld, \r\n", drv8847.drv->v_r1, drv8847.drv->v_r2, as5047d.angle, sangle.ele_dangle, cangle.ele_dangle,
                                                    fb_exc_angle.th_esvpwm, fb_current.i_svpwm, fb_exc_angle.th_er, fb_exc_angle.th_cum, pwm12.pwm1, pwm12.pwm2);
    // RS485_trm("Hello World !!!\r\n");
    /* 1 s update command angle */
    if(presc_cnt % 10 == 0) {
        update_cangle(&cangle, get_cangle_inc(&adj_v));
    }
    presc_cnt++;

    /* clear flag */
    PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;

}


