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
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "control_config.h"

/* Include other library */
#include "hal_tick.h"
#include "freqdiv.h"

/* Include board support package */
#include "control_board.h"
#include "hal_as5047d.h"
#include "hal_drv8847_s.h"
#include "tick.h"
#include "rs485.h"

extern drv8847_s_t drv8847_s;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

#define PID0_FREQ     1000
#define COLLECT_LEN   2500
uint16_t collect_buf[COLLECT_LEN];

/* Main code */
int main (void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    enable_fpu();

    /* Initilize hardware */
    board_init();
    enable_fpu();
    enable_tick();

    drv8847_s.init();
    if(drv8847_s.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }
    drv8847_s.setMode(DRV8847_MODE_SLEEP);

    hal_delay(100);
    /* reload PIT0 every 1 ms (1000 Hz), SYSTEM_CLOCK_FREQUENCY = 72MHz */
    PIT->CHANNEL[0].LDVAL = SYS_CLOCK_FREQ / 40000;
    /* enable PIT0 timer and enable interrupt */
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    RS485_trm("start \r\n");
    __enable_irqn(PIT0_IRQn);

    while (true) {
        ;
    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}

void PIT0_IRQHandler(void) {
    static uint16_t cnt = 0;
    as5047d.update();
    collect_buf[cnt++] = as5047d.angle;
    if(cnt >= COLLECT_LEN) {
        __disable_irqn(PIT0_IRQn);
        cnt = 0;
        for (uint16_t i = 0; i < COLLECT_LEN; i++) {
            RS485_trm("%d,\r\n", collect_buf[i]);
        }
        __enable_irqn(PIT0_IRQn);
    }
    /* clear flag */
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
}
