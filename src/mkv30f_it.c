/**
 * @file mkv30f_it.c
 * @author Xiang-Guan Deng
 * @brief Interrupt file
 * @date 2020.xx.xx
 *
 */

#define FTM_1A1B_Handler         FTM1_IRQHandler
#define FTM_2A2B_Handler         FTM0_IRQHandler

#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "cortex_m4.h"
#include "hal_drv8847.h"
#include "control_board.h"
#include "uart.h"
#include "rs485.h"

extern drv8847_t drv8847;
volatile uint8_t fff = 0;

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

volatile uint32_t ccc = 0;
volatile uint8_t fg = 0;
/** @brief 2A 2B timer/PWM handler
 *
 */
void FTM_2A2B_Handler(void) {
    if(ccc > 3000) {
        fg = 1;
        drv8847.adc_trig2A2B();
        ccc = 0;
    }

    /* clear overflow flag */
    FTM_2A2B->SC &= ~FTM_SC_TOF_MASK;
}


/** @brief 1A 1B timer/PWM handler
 *
 */
void FTM_1A1B_Handler(void) {
    ccc++;
    if(ccc == 1500) {
        fg = 2;
        drv8847.adc_trig1A1B();
    }
    /* clear overflow flag */
    FTM_1A1B->SC &= ~FTM_SC_TOF_MASK;
}

/** brief TODO
 *
 */
uint8_t buf[200];
uint8_t cc = 0;
void RS485_INT_HANDLER(void) {
    /* Read and clear hardware register automatically */
    uint8_t uart_status = RS485_UART->S1;

    /* Transmit complete */
    if(uart_status & UART_S1_TC_MASK) {
        DISABLE_RS485_TRM();
        RS485_UART->C2 &= ~UART_C2_TCIE_MASK;
    }

    /* Receive data */
    if(uart_status & UART_S1_RDRF_MASK) {
        // Data receive
        buf[cc] = RS485_UART->D;
        cc++;
        if(cc == 200) {cc = 0;}
    }
}

void ADC0_IRQHandler(void) {
    if(fg == 1) {
        drv8847.drv->v_r2 = ADC_PHA->R[0];
    }
    else if(fg == 2) {
        drv8847.drv->v_r1 = ADC_PHA->R[0];
    }
}

/** brief TODO
 *
 */
void SysTick_Handler(void) {

}
