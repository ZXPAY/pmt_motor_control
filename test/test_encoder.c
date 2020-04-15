/**
 * @file main.c
 * @author Xiang-Guan Deng
 * @brief test encoder
 * @date 2020.xx.xx
 *
 * Setting makefile UART_DMA = NO
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
#include "freqdiv.h"

/* Include board support package */
#include "control_board.h"
#include "hal_as5047d.h"
#include "hal_drv8847.h"
#include "tick.h"
#include "rs485.h"

extern drv8847_t drv8847;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

freq_div_t freq_div_pwmA;
freq_div_t freq_div_pwmB;

/* Main code */
int main (void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    enable_fpu();

    /* Initilize hardware */
    board_init();
    enable_fpu();
    enable_tick();

    drv8847.init();
    if(drv8847.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }
    drv8847.setMode(DRV8847_MODE_SLEEP);

    /* Initialize freqency divider */
    freq_div_init(&freq_div_pwmA);
    freq_div_init(&freq_div_pwmB);
    freq_div_add(&freq_div_pwmA, 20, (void *)drv8847.adc_trig1A1B, NULL, 0);
    freq_div_add(&freq_div_pwmB, 20, (void *)drv8847.adc_trig2A2B, NULL, 10);

    hal_delay(100);
    RS485_trm("start \r\n");
    ENABLE_PHA_INT();
    ENABLE_PHB_INT();
    ENABLE_ADC_PHAB_INT();
    uint16_t angle;
    uint16_t angle_com;
    uint16_t dia;
    while (true) {

        angle = as5047d.instance->read(AS5047D_ANGLE);
        ENABLE_TEST1();
        angle_com = as5047d.instance->read_anglecom();
        DISABLE_TEST1();
        dia = as5047d.instance->read(AS5047D_DIAAGC);

        RS485_trm(",%d, %d, %x,\r\n", angle, angle_com, dia);
    }
    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}



