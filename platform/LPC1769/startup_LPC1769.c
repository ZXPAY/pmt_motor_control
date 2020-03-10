/**
 * @file startup_LPC1769.c
 * @author LiYu87
 * @brief Startup Module for NXP LPC176x Microcontrollers
 * @date 2020.02.17
 *
 * This module defines vector table, startup code, default
 * exception handlers, main stack and miscellanous functions.
 */

#include "LPC1769.h"

#include <inttypes.h>

#define STACK_SIZE 2048 /* Size of Main stack (must be multiple of 8) */

/* LPC17XX CRP options. */
#define NO_CRP 0xFFFFFFFF
#define CRP1 0x12345678
#define CRP2 0x87654321
#define CRP3 0x43218765

static uint32_t _crp_magic __attribute__((section(".crp"), used)) = NO_CRP;

/* Section address defined in linker script */
extern uint32_t __etext[];
extern uint32_t __data_start__[];
extern uint32_t __data_end__[];
extern uint32_t __bss_start__[];
extern uint32_t __bss_end__[];

extern int main(void);

/* System exception vector handler */
void Reset_Handler(void)        __attribute__((weak, alias("default_reset_handler")));
void NMI_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void HardFault_Hander(void)     __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)      __attribute__((weak, alias("Default_Handler")));

/* External interrupt vector handler */
void WDT_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void TIMER0_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void TIMER1_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void TIMER2_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void TIMER3_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void UART0_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void UART1_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void UART2_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void UART3_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void PWM1_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void I2C0_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void I2C1_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void I2C2_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void SPI_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void SSP0_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void SSP1_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void PLL0_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void EINT0_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void EINT1_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void EINT2_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void EINT3_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void ADC_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void BOD_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void USB_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void CAN_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void GPDMA_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void I2S_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void ETHER_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void RIT_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void MCPWM_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void QEI_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void PLL1_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void USBACT_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void CANACT_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));

/**
 * @brief Main Stack
 */
static char mstk[STACK_SIZE] __attribute__((aligned(8), section(".stack")));

/**
 * @brief Exception Vector Table
 *
 * see below info:
 *
 *   UM10360 p.757
 *   Table 640. Properties of the different exception types
 *
 *   UM10360 p.75
 *   Table 50. Connection of interrupt sources to the Vectored Interrupt
 *   Controller
 *
 */
static void *const vector[] __attribute__((section(".isr_vector"))) = {
    // 0x00 is stack pointer
    &mstk[STACK_SIZE], // 0x00

    // 0x04 ~ 0x3C is Exception vector
    Reset_Handler,      // 0x04
    NMI_Handler,        // 0x08
    HardFault_Hander,   // 0x0C
    MemManage_Handler,  // 0x10
    BusFault_Handler,   // 0x14
    UsageFault_Handler, // 0x18
    0,                  // 0x1C
    0,                  // 0x20
    0,                  // 0x24
    0,                  // 0x28
    SVC_Handler,        // 0x2C
    DebugMon_Handler,   // 0x30
    0,                  // 0x34
    PendSV_Handler,     // 0x38
    SysTick_Handler,    // 0x3C

    // 0x40 ~ 0xC8 is External Interrupt vector
    WDT_IRQHandler,    // 0x40
    TIMER0_IRQHandler, // 0x44
    TIMER1_IRQHandler, // 0x48
    TIMER2_IRQHandler, // 0x4C
    TIMER3_IRQHandler, // 0x50
    UART0_IRQHandler,  // 0x54
    UART1_IRQHandler,  // 0x58
    UART2_IRQHandler,  // 0x5C
    UART3_IRQHandler,  // 0x60
    PWM1_IRQHandler,   // 0x64
    I2C0_IRQHandler,   // 0x68
    I2C1_IRQHandler,   // 0x6C
    I2C2_IRQHandler,   // 0x70
    SPI_IRQHandler,    // 0x74
    SSP0_IRQHandler,   // 0x78
    SSP1_IRQHandler,   // 0x7C
    PLL0_IRQHandler,   // 0x80
    RTC_IRQHandler,    // 0x84
    EINT0_IRQHandler,  // 0x88
    EINT1_IRQHandler,  // 0x8C
    EINT2_IRQHandler,  // 0x90
    EINT3_IRQHandler,  // 0x94
    ADC_IRQHandler,    // 0x98
    BOD_IRQHandler,    // 0x9C
    USB_IRQHandler,    // 0xA0
    CAN_IRQHandler,    // 0xA4
    GPDMA_IRQHandler,  // 0xA8
    I2S_IRQHandler,    // 0xAC
    ETHER_IRQHandler,  // 0xB0
    RIT_IRQHandler,    // 0xB4
    MCPWM_IRQHandler,  // 0xB8
    QEI_IRQHandler,    // 0xBC
    PLL1_IRQHandler,   // 0xC0
    USBACT_IRQHandler, // 0xC4
    CANACT_IRQHandler  // 0xC8
};

/**
 * @brief Initialize .data section.
 *
 */
static void do_copy_data() {
    uint32_t *s, *d;
    for (s = __etext, d = __data_start__; d < __data_end__; *d++ = *s++) {
        ;
    }
}

/**
 * @brief Initialize .bss section.
 *
 */
static void do_clear_bss() {
    uint32_t *d;
    for (d = __bss_start__; d < __bss_end__; *d++ = 0) {
        ;
    }
}

/**
 * @brief The entry point after HW reset.
 *
 * Initialize .data and .bss sections and then start main().
 */
void default_reset_handler(void) {

    VTOR = (uint32_t)vector;

    do_copy_data();
    do_clear_bss();

    /* Start main() with MSP and privileged mode */
    main();

    for (;;) {
        ;
    }
}

/**
 * @brief Unexpected Exception/IRQ Trap
 *
 * Just a dead loop. Used for interrupt handler alias.
 */
void Default_Handler(void) {
    for (;;) {
        ;
    }
}
