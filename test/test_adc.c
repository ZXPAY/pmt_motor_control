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

freq_div_t freq_div_pwmA;
freq_div_t freq_div_pwmB;

#define NO_OUTPUT
#define CALIBRATE
#define DUTY0
// #define DUTY25
// #define DUTY50
// #define DUTY75
// #define DUTY100

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
#ifdef NO_OUTPUT
    drv8847_s.setMode(DRV8847_MODE_SLEEP);
#endif
    /* Initialize freqency divider */
    freq_div_init(&freq_div_pwmA);
    freq_div_init(&freq_div_pwmB);
    freq_div_add(&freq_div_pwmA, 20, (void *)drv8847_s.adc_trig1A1B, NULL, 0);
    freq_div_add(&freq_div_pwmB, 20, (void *)drv8847_s.adc_trig2A2B, NULL, 10);
#ifdef NO_OUTPUT
    SET_1A_DUTY = 0;
    SET_1B_DUTY = PERIOD_COUNT;
    SET_2B_DUTY = 0;
    SET_2A_DUTY = PERIOD_COUNT;
#endif
    hal_delay(100);
    RS485_trm("start \r\n");
    ENABLE_PHA_INT();
    ENABLE_PHB_INT();
    ENABLE_ADC_PHAB_INT();
    while (true) {
        #ifdef DUTY0
            /* duty 0 % */
            SET_PHASEA_DUTY(0);
            SET_PHASEB_DUTY(0);
            hal_delay(100);
            // RS485_trm(", 0, %.4f, %.4f, \r\n", (3.3*(float)drv8847_s.drv->v_r1/65535.0-1.65)/20/0.1, (3.3*(float)drv8847_s.drv->v_r2/65535.0-1.65)/20/0.1);
            RS485_trm(", 0, %.4f, %.4f, \r\n", (3.3*(float)drv8847_s.drv->v_r1/65535.0), (3.3*(float)drv8847_s.drv->v_r2/65535.0));
            hal_delay(1000);
        #endif

        #ifdef DUTY25
            /* duty 25 % */
            SET_PHASEA_DUTY(PERIOD_COUNT>>2);
            SET_PHASEB_DUTY(PERIOD_COUNT>>2);
            hal_delay(100);
            RS485_trm(", 25, %.4f, %.4f, \r\n", (3.3*(float)drv8847_s.drv->v_r1/65535.0-1.65)/20/0.1, (3.3*(float)drv8847_s.drv->v_r2/65535.0-1.65)/20/0.1);
            hal_delay(1000);
        #endif

        #ifdef DUTY50
            /* duty 50 % */
            SET_PHASEA_DUTY(PERIOD_COUNT>>1);
            SET_PHASEB_DUTY(PERIOD_COUNT>>1);
            hal_delay(100);
            RS485_trm(", 50, %.4f, %.4f, \r\n", (3.3*(float)drv8847_s.drv->v_r1/65535.0-1.65)/20/0.1, (3.3*(float)drv8847_s.drv->v_r2/65535.0-1.65)/20/0.1);
            hal_delay(1000);
        #endif

        #ifdef DUTY75
            /* duty 75 % */
            SET_PHASEA_DUTY(PERIOD_COUNT - (PERIOD_COUNT>>2));
            SET_PHASEB_DUTY(PERIOD_COUNT - (PERIOD_COUNT>>2));
            hal_delay(100);
            RS485_trm(", 75, %.4f, %.4f, \r\n", (3.3*(float)drv8847_s.drv->v_r1/65535.0-1.65)/20/0.1, (3.3*(float)drv8847_s.drv->v_r2/65535.0-1.65)/20/0.1);
            hal_delay(1000);
        #endif

        #ifdef DUTY100
            /* duty 100 % */
            SET_PHASEA_DUTY(PERIOD_COUNT);
            SET_PHASEB_DUTY(PERIOD_COUNT);
            hal_delay(100);
            RS485_trm(", 100, %.4f, %.4f, \r\n", (3.3*(float)drv8847_s.drv->v_r1/65535.0-1.65)/20/0.1, (3.3*(float)drv8847_s.drv->v_r2/65535.0-1.65)/20/0.1);
            hal_delay(1000);
        #endif
        #ifdef CALIBRATE
            float suma = 0, sumb = 0;
            for(uint16_t i=0;i<10000;i++) {
                suma += (3.3*(float)drv8847_s.drv->v_r1/65535.0);
                sumb += (3.3*(float)drv8847_s.drv->v_r2/65535.0);
                hal_delay(1);
            }
            suma /= 10000;
            sumb /= 10000;
            RS485_trm(", cal, %.4f, %.4f, \r\n", suma, sumb);
            hal_delay(1000);
        #endif
    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}



