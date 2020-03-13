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
#include "sin_cos_val_table.h"
#include "uart.h"
#include "rs485.h"
#include "svpwm.h"

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

volatile int8_t sign1, sign2;
extern int fg2;
extern pwmAB_t pwm12;                   /* 1A1B 2A2B PWM */
/** @brief 2A 2B timer/PWM handler
 *
 */
void FTM0_IRQHandler(void) {
    int32_t temp = get_cos();
    // int32_t temp = pwm12.pwmb;
    if(temp >= 0) {
        SET_2B_DUTY = temp;
        SET_2A_DUTY = 0;
        sign2 = 1;
    }
    else {
        SET_2B_DUTY = 0;
        SET_2A_DUTY = -temp;
        sign2 = -1;
    }

    /* clear overflow flag */
    FTM_2A2B->SC &= ~FTM_SC_TOF_MASK;
}

/** @brief 1A 1B timer/PWM handler
 *
 */
void FTM1_IRQHandler(void) {
    int32_t temp = get_sin();
    // int32_t temp = pwm12.pwma;
    if(temp >= 0) {
        SET_1A_DUTY = temp;
        SET_1B_DUTY = 0;
        sign1 = 1;
    }
    else {
        SET_1A_DUTY = 0;
        SET_1B_DUTY = -temp;
        sign1 = -1;
    }

    /* clear overflow flag */
    FTM_1A1B->SC &= ~FTM_SC_TOF_MASK;
}

/** brief TODO
 *
 */
uint8_t buf[200];
uint8_t cc = 0;
void UART1_RX_TX_IRQHandler(void) {
    // Data receive
    buf[cc] = RS485_UART->D;
    cc++;
    if(cc == 200) {cc = 0;}
}

/** brief TODO
 *
 */
void SysTick_Handler(void) {
    inc_tick();
}
