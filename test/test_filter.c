/**
 * @file main.c
 * @author Xiang-Guan Deng
 * @brief Main code for motor control board
 * @date 2020.xx.xx
 *
 * Compiler: arm-none-eabi-gcc (8.3.1)
 * Setting:
 * UART_DMA = NO
 */

/* Include C standard library */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
#include "control.h"
#include "fir_filter.h"

/* Include board support package */
#include "control_board.h"
#include "hal_as5047d.h"
#include "hal_drv8847_s.h"
#include "tick.h"
#include "rs485.h"

extern drv8847_s_t drv8847_s;     /* DRV8847 motor drive IC */
extern as50474_t as5047d;         /* AS5047D motor encoder IC */
extern mv_avg_t enc_mv_avg;       /* Moving average object */
extern fir_t enc_fir;             /* FIR filter object */

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

    as5047d.init();
    drv8847_s.init();
    if(drv8847_s.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }
    drv8847_s.setMode(DRV8847_MODE_SLEEP);

    control_init();
    hal_delay(100);
    /* reload PIT0 every 1 ms (1000 Hz), SYSTEM_CLOCK_FREQUENCY = 72MHz */
    PIT->CHANNEL[0].LDVAL = SYS_CLOCK_FREQ / 40000;
    /* enable PIT0 timer and enable interrupt */
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    RS485_trm("start %d, %p\r\n", enc_fir.feat_sz, enc_fir.fir_buf);
    __enable_irqn(PIT0_IRQn);

    while (true) {
        __disable_irq();
        RS485_trm("%.0f, %d, %.0f\r\n", roundf(enc_fir.fir_output), enc_mv_avg.mv_output, roundf(enc_fir.fir_output)-(float)enc_mv_avg.mv_output);
        __enable_irq();
    }


    return 0;
}

void HardFault_Handler(void) {
    /* Delay a while */
    for(uint32_t i=0;i<100000;i++);
    RS485_trm("Hardware error occur\r\n");
    while(true);
}


void PIT0_IRQHandler(void) {
    as5047d.update();
    ENABLE_TEST1();
    set_fir(&enc_fir, (float)as5047d.angle);
    DISABLE_TEST1();
    ENABLE_TEST2();
    set_mv_avg(&enc_mv_avg, as5047d.angle);
    DISABLE_TEST2();
    /* clear flag */
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
}
