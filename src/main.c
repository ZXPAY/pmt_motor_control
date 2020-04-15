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
#include "control_config.h"

/* Include control library */
#include "control.h"

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
#include "control.h"
#include "freqdiv.h"

extern freq_div_t freq_div_pwmA;
extern freq_div_t freq_div_pwmB;

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
    drv8847.setMode(DRV8847_MODE_SLEEP);

    /* Initialize sin, cos table, call get_sin() and get_cos() to get current value */
    /* Use in test */
    // init_sin_cos_table(PERIOD_COUNT, N_STEP);

    /* Initialize control library */
    control_init();

    /* Initialize freqency divider */
    freq_div_init(&freq_div_pwmA);
    freq_div_init(&freq_div_pwmB);
    freq_div_add(&freq_div_pwmA, 10, (void *)control_handle, NULL, 0);
    freq_div_add(&freq_div_pwmA, 20, (void *)drv8847.adc_trig1A1B, NULL, 5);
    freq_div_add(&freq_div_pwmB, 20, (void *)drv8847.adc_trig2A2B, NULL, 15);

    /* SysTick initialize */
    systick_init();
    RS485_trm("start !!!\r\n");
    hal_delay(100);

    // Enable interrupt
    ENABLE_PHA_INT();
    ENABLE_PHB_INT();
    ENABLE_ADC_PHAB_INT();
    __enable_irqn(PIT1_IRQn);
    __enable_irqn(HardFault_IRQn);

    uint32_t cnt = 0;
    while (true) {
        cnt++;
        /* idle */
    }

    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Hardware error occur\r\n");
    while(true);
}

/* period : 2 ms */
void PIT0_IRQHandler(void) {

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
    control_print();

    /* clear flag */
    PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
}


