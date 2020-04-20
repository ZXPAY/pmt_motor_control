/**
 * @file test_timeout.c
 * @author Xiang-Guan Deng
 * @brief Test timeout
 * @date 2020.xx.xx
 *
 * @note Replace makefile main.c to test_timeout.c
 */

/* Include C standard library */
#include <stdbool.h>
#include <stdio.h>

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
#include "hal_drv8847_s.h"
#include "rs485.h"

#include "freqdiv.h"

extern drv8847_s_t drv8847_s;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

/* not use these object */
freq_div_t freq_div_pwmA;
freq_div_t freq_div_pwmB;

#define TIMEOUT_MS    100

int main() {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    /* Initilize hardware */
    board_init();
    enable_fpu();

    RS485_trm("initialize hardware ...\r\n");
    as5047d.init();
    drv8847_s.init();
    if(drv8847_s.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }
    drv8847_s.setMode(DRV8847_MODE_SLEEP);

    timeout_set_ms(TIMEOUT_MS);
    RS485_trm("Test timeout ms : %d\n", TIMEOUT_MS);
    __enable_irqn(PIT2_IRQn);  // enable interrupt
    while (true) {
        uint8_t timeout_status = is_timeout();
        RS485_trm("tick : %ld, timeout? : %d\n", get_tick(), timeout_status);
        if(timeout_status) {
            /* Dead loop to hold */
            while(true);
        }
    }

}
