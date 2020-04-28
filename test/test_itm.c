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

/* Include test header file */
#include "arm_itm.h"

extern drv8847_s_t drv8847_s;        /* DRV8847 motor drive IC */
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

    drv8847_s.init();
    if(drv8847_s.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }
    drv8847_s.setMode(DRV8847_MODE_SLEEP);

    /* MKV30F128, SWO is PTA2 */
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    SIM->SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    /* Something wrong below the code */
    SWO_Init(1, SYS_CLOCK_FREQ);
    /* If I comment the code, it will work. ??? */

    while (true) {
        SWO_PrintChar('H', 0);
        SWO_PrintChar('e', 0);
        SWO_PrintChar('l', 0);
        SWO_PrintChar('l', 0);
        SWO_PrintChar('o', 0);
        SWO_PrintChar('\n', 0);
        hal_delay(1000);
    }


    return 0;
}

void HardFault_Handler(void) {
    while(true);
}



