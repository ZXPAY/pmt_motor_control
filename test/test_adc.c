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

    hal_delay(100);
    RS485_trm("start \r\n");
    ENABLE_PHA_INT();
    ENABLE_PHB_INT();
    while (true) {
        /* duty 0 % */
        SET_PHASEA_DUTY(0);
        SET_PHASEB_DUTY(0);
        hal_delay(5);
        RS485_trm("0 : %ld, %ld\r\n", drv8847.drv->v_r1, drv8847.drv->v_r2);
        hal_delay(100);

        /* duty 25 % */
        SET_PHASEA_DUTY(500);
        SET_PHASEB_DUTY(500);
        hal_delay(5);
        RS485_trm("25 : %ld, %ld\r\n", drv8847.drv->v_r1, drv8847.drv->v_r2);
        hal_delay(100);

        /* duty 50 % */
        SET_PHASEA_DUTY(1000);
        SET_PHASEB_DUTY(1000);
        hal_delay(5);
        RS485_trm("50 : %ld, %ld\r\n", drv8847.drv->v_r1, drv8847.drv->v_r2);
        hal_delay(100);

        /* duty 75 % */
        SET_PHASEA_DUTY(1500);
        SET_PHASEB_DUTY(1500);
        hal_delay(5);
        RS485_trm("75 : %ld, %ld\r\n", drv8847.drv->v_r1, drv8847.drv->v_r2);
        hal_delay(100);

        /* duty 100 % */
        SET_PHASEA_DUTY(2000);
        SET_PHASEB_DUTY(2000);
        hal_delay(5);
        RS485_trm("100 : %ld, %ld\r\n", drv8847.drv->v_r1, drv8847.drv->v_r2);
        hal_delay(100);

    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}



