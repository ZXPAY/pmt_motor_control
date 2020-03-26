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

/* Main code */
int main (void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    /* Initilize hardware */
    init_hw_rs485();
    init_hw_tick();
    init_tick();
    enable_tick();
    __enable_irqn(PIT2_IRQn);
    hal_delay(50);

    // Enable interrupt
    __enable_irqn(UART1_RX_TX_IRQn);
    __enable_irqn(DMA0_IRQn);


    uint32_t cnt = 0;
    while (true) {
        cnt++;
        uint8_t temp = get_dma_error();
        /* DMA error should be 1 */
        RS485_trm("hello ~~~ %ld, %d \r\n", cnt, temp);
        if(temp != DMA_IDLE)
            clear_dma_error();
        hal_delay(100);
    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}



