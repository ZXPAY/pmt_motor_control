/**
 * @file mkv30f_it.c
 * @author Xiang-Guan Deng
 * @brief Interrupt file
 * @date 2020.xx.xx
 *
 */


#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "hal_drv8847.h"
#include "hal_tick.h"
#include "control_board_v2.h"
#include <stdio.h>

extern drv8847_t drv8847;

/**
* @brief DRV8847 pin map
* 1A : PTB0   FTM1_CH0
* 1B : PTB1   FTM1_CH1
* 2B : PTC1   FTM0_CH0
* 2A : PTC2   FTM0_CH1
* MODE : PTE18
* NFAULT : PTD5
* TRQ : PTE19
* NSLEEP : PTC3
* Rsense1 : PTE24
* Rsense2 : ADC0_SE23
*/

int16_t sin_data[100];
int16_t cos_data[100];
volatile int d1 = 0;
volatile int d2 = 0;

/** @brief 2A 2B timer/PWM handler
 *
 */
void FTM0_IRQHandler(void) {
    // drv8847.drv->set_duty1(sin_data[d1++]);
    // if(d1 == 100) d1 = 0;
    /* clear overflow flag */
    FTM_2A2B->SC &= ~FTM_SC_TOF_MASK;
}

/** @brief 1A 1B timer/PWM handler
 *
 */
void FTM1_IRQHandler(void) {
    // drv8847.drv->set_duty2(sin_data[d2++]);
    // if(d2 == 100) d2 = 0;
    /* clear overflow flag */
    FTM_1A1B->SC &= ~FTM_SC_TOF_MASK;
}

uint8_t data;
/** brief TODO
 *
 */
void UART1_RX_TX_IRQHandler(void) {
    // Data receive
    data = RS485_UART->D;
}

/** brief TODO
 *
 */
void ADC0_IRQHandler(void) {
    drv8847.handler();
}

/** brief TODO
 *
 */
void SysTick_Handler(void) {
    inc_tick();
}
