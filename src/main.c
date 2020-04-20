/**
 * @file main.c
 * @author Xiang-Guan Deng
 * @brief Main code for motor control board
 * @date 2020.xx.xx
 *
 * Compiler: arm-none-eabi-gcc (8.3.1)
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
#include "freqdiv.h"
extern freq_div_t freq_div_pwmA;
extern freq_div_t freq_div_pwmB;


/* Include other library */
#include "hal_tick.h"

/* Include board support package */
#include "control_board.h"
#include "hal_as5047d.h"
#include "hal_drv8847_s.h"
#include "tick.h"
#include "rs485.h"
extern drv8847_s_t drv8847_s;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

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
    drv8847_s.init();
    if(drv8847_s.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }
    // drv8847_s.setMode(DRV8847_MODE_SLEEP);

    /* Initialize control library */
    control_init();

    /* Initialize freqency divider */
    freq_div_init(&freq_div_pwmA);
    freq_div_init(&freq_div_pwmB);
    freq_div_add(&freq_div_pwmA, 10, (void *)control_handle, NULL, 0);
    freq_div_add(&freq_div_pwmA, 20, (void *)drv8847_s.adc_trig1A1B, NULL, 5);
    freq_div_add(&freq_div_pwmB, 20, (void *)drv8847_s.adc_trig2A2B, NULL, 15);

    /* SysTick initialize */
    systick_init();
    RS485_trm("start !!!\r\n");
    hal_delay(100);

    // Enable interrupt
    ENABLE_PHA_INT();
    ENABLE_PHB_INT();
    ENABLE_ADC_PHAB_INT();
    ENABLE_PRINT_INT();
    ENABLE_HARDWARE_INT();

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

/* period : 10 ms */
/* Do experiment, print whole words need 7 ms => choose 100 Hz */
void PRINT_IRQHandler(void) {
    control_print();

    /* clear flag */
    PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
}


