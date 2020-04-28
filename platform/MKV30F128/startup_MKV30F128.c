/**
 * @file startup_LPC1769.c
 * @author LiYu87
 * @brief Startup Module for NXP LPC176x Microcontrollers
 * @date 2020.02.17
 *
 * This module defines vector table, startup code, default
 * exception handlers, main stack and miscellanous functions.
 */

#include "MKV30F12810.h"
#include "cortex_m4.h"
#include <inttypes.h>

#define STACK_SIZE	2048		/* Size of Main stack (must be multiple of 8) */

uint32_t flasf_cfg[4] __attribute__ ((section(".flash_config"), used)) = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE
};

/* Section address defined in linker script */
extern uint32_t __etext[];
extern uint32_t __data_start__[];
extern uint32_t __data_end__[];
extern uint32_t __bss_start__[];
extern uint32_t __bss_end__[];

void Default_Handler(void);
extern int main (void);
extern void system_clock_init(void);

/* System exception vector handler */
void Reset_Handler (void)		__attribute__ ((weak, alias ("default_reset_handler")));
void NMI_Handler (void)			__attribute__ ((weak, alias ("Default_Handler")));
void HardFault_Handler (void)	__attribute__ ((weak, alias ("Default_Handler")));
void MemManage_Handler (void)	__attribute__ ((weak, alias ("Default_Handler")));
void BusFault_Handler (void)	__attribute__ ((weak, alias ("Default_Handler")));
void UsageFault_Handler (void)	__attribute__ ((weak, alias ("Default_Handler")));
void SVC_Handler (void)			__attribute__ ((weak, alias ("Default_Handler")));
void DebugMon_Handler (void)	__attribute__ ((weak, alias ("Default_Handler")));
void PendSV_Handler (void)		__attribute__ ((weak, alias ("Default_Handler")));
void SysTick_Handler (void)		__attribute__ ((weak, alias ("Default_Handler")));

/* External interrupt vector handler */
void DMA0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void DMA3_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void Reserved20_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved21_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved22_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved23_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved24_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved25_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved26_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved27_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved28_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved29_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved30_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved31_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void DMA_Error_IRQHandler(void)         __attribute__ ((weak, alias ("Default_Handler")));
void MCM_IRQHandler(void)               __attribute__ ((weak, alias ("Default_Handler")));
void FTF_IRQHandler(void)               __attribute__ ((weak, alias ("Default_Handler")));
void Read_Collision_IRQHandler(void)    __attribute__ ((weak, alias ("Default_Handler")));
void LVD_LVW_IRQHandler(void)           __attribute__ ((weak, alias ("Default_Handler")));
void LLWU_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void WDOG_EWM_IRQHandler(void)          __attribute__ ((weak, alias ("Default_Handler")));
void Reserved39_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void I2C0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void Reserved41_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void SPI0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void Reserved43_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved44_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved45_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved46_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void UART0_RX_TX_IRQHandler(void)       __attribute__ ((weak, alias ("Default_Handler")));
void UART0_ERR_IRQHandler(void)         __attribute__ ((weak, alias ("Default_Handler")));
void UART1_RX_TX_IRQHandler(void)       __attribute__ ((weak, alias ("Default_Handler")));
void UART1_ERR_IRQHandler(void)         __attribute__ ((weak, alias ("Default_Handler")));
void Reserved51_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved52_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved53_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved54_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void ADC0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void CMP0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void CMP1_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void FTM0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void FTM1_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void FTM2_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void Reserved61_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved62_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved63_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void PIT0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void PIT1_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void PIT2_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void PIT3_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void PDB0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void Reserved69_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved70_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved71_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void DAC0_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));
void MCG_IRQHandler(void)               __attribute__ ((weak, alias ("Default_Handler")));
void LPTMR0_IRQHandler(void)            __attribute__ ((weak, alias ("Default_Handler")));
void PORTA_IRQHandler(void)             __attribute__ ((weak, alias ("Default_Handler")));
void PORTB_IRQHandler(void)             __attribute__ ((weak, alias ("Default_Handler")));
void PORTC_IRQHandler(void)             __attribute__ ((weak, alias ("Default_Handler")));
void PORTD_IRQHandler(void)             __attribute__ ((weak, alias ("Default_Handler")));
void PORTE_IRQHandler(void)             __attribute__ ((weak, alias ("Default_Handler")));
void SWI_IRQHandler(void)               __attribute__ ((weak, alias ("Default_Handler")));
void Reserved81_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved82_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved83_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved84_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved85_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved86_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved87_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void Reserved88_IRQHandler(void)        __attribute__ ((weak, alias ("Default_Handler")));
void ADC1_IRQHandler(void)              __attribute__ ((weak, alias ("Default_Handler")));

/**
 * @brief Main Stack
 */
static char mstk[STACK_SIZE] __attribute__ ((aligned(8), section(".stack")));

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
static void* const vector[] __attribute__ ((section(".isr_vector"))) = {
    // 0x00 is stack pointer
    &mstk[STACK_SIZE],	// 0x00

    // 0x04 ~ 0x3C is Exception vector
    Reset_Handler,		// 0x04
    NMI_Handler,		// 0x08
    HardFault_Handler,	// 0x0C
    MemManage_Handler,	// 0x10
    BusFault_Handler,	// 0x14
    UsageFault_Handler,	// 0x18
    0,					// 0x1C
    0,					// 0x20
    0,					// 0x24
    0,					// 0x28
    SVC_Handler,		// 0x2C
    DebugMon_Handler,	// 0x30
    0,               	// 0x34
    PendSV_Handler,  	// 0x38
    SysTick_Handler, 	// 0x3C

    // 0x40 ~ 0xC8 is External Interrupt vector
    DMA0_IRQHandler,
    DMA1_IRQHandler,
    DMA2_IRQHandler,
    DMA3_IRQHandler,
    Reserved20_IRQHandler,
    Reserved21_IRQHandler,
    Reserved22_IRQHandler,
    Reserved23_IRQHandler,
    Reserved24_IRQHandler,
    Reserved25_IRQHandler,
    Reserved26_IRQHandler,
    Reserved27_IRQHandler,
    Reserved28_IRQHandler,
    Reserved29_IRQHandler,
    Reserved30_IRQHandler,
    Reserved31_IRQHandler,
    DMA_Error_IRQHandler,
    MCM_IRQHandler,
    FTF_IRQHandler,
    Read_Collision_IRQHandler,
    LVD_LVW_IRQHandler,
    LLWU_IRQHandler,
    WDOG_EWM_IRQHandler,
    Reserved39_IRQHandler,
    I2C0_IRQHandler,
    Reserved41_IRQHandler,
    SPI0_IRQHandler,
    Reserved43_IRQHandler,
    Reserved44_IRQHandler,
    Reserved45_IRQHandler,
    Reserved46_IRQHandler,
    UART0_RX_TX_IRQHandler,
    UART0_ERR_IRQHandler,
    UART1_RX_TX_IRQHandler,
    UART1_ERR_IRQHandler,
    Reserved51_IRQHandler,
    Reserved52_IRQHandler,
    Reserved53_IRQHandler,
    Reserved54_IRQHandler,
    ADC0_IRQHandler,
    CMP0_IRQHandler,
    CMP1_IRQHandler,
    FTM0_IRQHandler,
    FTM1_IRQHandler,
    FTM2_IRQHandler,
    Reserved61_IRQHandler,
    Reserved62_IRQHandler,
    Reserved63_IRQHandler,
    PIT0_IRQHandler,
    PIT1_IRQHandler,
    PIT2_IRQHandler,
    PIT3_IRQHandler,
    PDB0_IRQHandler,
    Reserved69_IRQHandler,
    Reserved70_IRQHandler,
    Reserved71_IRQHandler,
    DAC0_IRQHandler,
    MCG_IRQHandler,
    LPTMR0_IRQHandler,
    PORTA_IRQHandler,
    PORTB_IRQHandler,
    PORTC_IRQHandler,
    PORTD_IRQHandler,
    PORTE_IRQHandler,
    SWI_IRQHandler,
    Reserved81_IRQHandler,
    Reserved82_IRQHandler,
    Reserved83_IRQHandler,
    Reserved84_IRQHandler,
    Reserved85_IRQHandler,
    Reserved86_IRQHandler,
    Reserved87_IRQHandler,
    Reserved88_IRQHandler,
    ADC1_IRQHandler,

    Default_Handler, // 90
    Default_Handler, // 91
    Default_Handler, // 92
    Default_Handler, // 93
    Default_Handler, // 94
    Default_Handler, // 95
    Default_Handler, // 96
    Default_Handler, // 97
    Default_Handler, // 98
    Default_Handler, // 99
    Default_Handler, // 100
    Default_Handler, // 101
    Default_Handler, // 102
    Default_Handler, // 103
    Default_Handler, // 104
    Default_Handler, // 105
    Default_Handler, // 106
    Default_Handler, // 107
    Default_Handler, // 108
    Default_Handler, // 109
    Default_Handler, // 110
    Default_Handler, // 111
    Default_Handler, // 112
    Default_Handler, // 113
    Default_Handler, // 114
    Default_Handler, // 115
    Default_Handler, // 116
    Default_Handler, // 117
    Default_Handler, // 118
    Default_Handler, // 119
    Default_Handler, // 120
    Default_Handler, // 121
    Default_Handler, // 122
    Default_Handler, // 123
    Default_Handler, // 124
    Default_Handler, // 125
    Default_Handler, // 126
    Default_Handler, // 127
    Default_Handler, // 128
    Default_Handler, // 129
    Default_Handler, // 130
    Default_Handler, // 131
    Default_Handler, // 132
    Default_Handler, // 133
    Default_Handler, // 134
    Default_Handler, // 135
    Default_Handler, // 136
    Default_Handler, // 137
    Default_Handler, // 138
    Default_Handler, // 139
    Default_Handler, // 140
    Default_Handler, // 141
    Default_Handler, // 142
    Default_Handler, // 143
    Default_Handler, // 144
    Default_Handler, // 145
    Default_Handler, // 146
    Default_Handler, // 147
    Default_Handler, // 148
    Default_Handler, // 149
    Default_Handler, // 150
    Default_Handler, // 151
    Default_Handler, // 152
    Default_Handler, // 153
    Default_Handler, // 154
    Default_Handler, // 155
    Default_Handler, // 156
    Default_Handler, // 157
    Default_Handler, // 158
    Default_Handler, // 159
    Default_Handler, // 160
    Default_Handler, // 161
    Default_Handler, // 162
    Default_Handler, // 163
    Default_Handler, // 164
    Default_Handler, // 165
    Default_Handler, // 166
    Default_Handler, // 167
    Default_Handler, // 168
    Default_Handler, // 169
    Default_Handler, // 170
    Default_Handler, // 171
    Default_Handler, // 172
    Default_Handler, // 173
    Default_Handler, // 174
    Default_Handler, // 175
    Default_Handler, // 176
    Default_Handler, // 177
    Default_Handler, // 178
    Default_Handler, // 179
    Default_Handler, // 180
    Default_Handler, // 181
    Default_Handler, // 182
    Default_Handler, // 183
    Default_Handler, // 184
    Default_Handler, // 185
    Default_Handler, // 186
    Default_Handler, // 187
    Default_Handler, // 188
    Default_Handler, // 189
    Default_Handler, // 190
    Default_Handler, // 191
    Default_Handler, // 192
    Default_Handler, // 193
    Default_Handler, // 194
    Default_Handler, // 195
    Default_Handler, // 196
    Default_Handler, // 197
    Default_Handler, // 198
    Default_Handler, // 199
    Default_Handler, // 200
    Default_Handler, // 201
    Default_Handler, // 202
    Default_Handler, // 203
    Default_Handler, // 204
    Default_Handler, // 205
    Default_Handler, // 206
    Default_Handler, // 207
    Default_Handler, // 208
    Default_Handler, // 209
    Default_Handler, // 210
    Default_Handler, // 211
    Default_Handler, // 212
    Default_Handler, // 213
    Default_Handler, // 214
    Default_Handler, // 215
    Default_Handler, // 216
    Default_Handler, // 217
    Default_Handler, // 218
    Default_Handler, // 219
    Default_Handler, // 220
    Default_Handler, // 221
    Default_Handler, // 222
    Default_Handler, // 223
    Default_Handler, // 224
    Default_Handler, // 225
    Default_Handler, // 226
    Default_Handler, // 227
    Default_Handler, // 228
    Default_Handler, // 229
    Default_Handler, // 230
    Default_Handler, // 231
    Default_Handler, // 232
    Default_Handler, // 233
    Default_Handler, // 234
    Default_Handler, // 235
    Default_Handler, // 236
    Default_Handler, // 237
    Default_Handler, // 238
    Default_Handler, // 239
    Default_Handler, // 240
    Default_Handler, // 241
    Default_Handler, // 242
    Default_Handler, // 243
    Default_Handler, // 244
    Default_Handler, // 245
    Default_Handler, // 246
    Default_Handler, // 247
    Default_Handler, // 248
    Default_Handler, // 249
    Default_Handler, // 250
    Default_Handler, // 251
    Default_Handler, // 252
    Default_Handler, // 253
    Default_Handler, // 254
    (void *)0xFFFFFFFF, // 255
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

    SCB_VTOR = (uint32_t)vector;
    system_clock_init();
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
