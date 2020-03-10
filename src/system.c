/**
 * @file system.c
 * @author LiYu87
 * @date 2020.01.07
 * @brief system clock、system power、pinconnect之設定函式。
 */

#include "system.h"

#include "MKV30F12810.h"


/**
 * CONFIG
 */
#define PLL0_M 20
#define PLL0_N 1
#define CPU_CLK_DEVIDER 4

/**
 * clk_pll0 = (2 *  M * sys_clk) / N
 *          = (2 * 20 *    12e6) / 1
 *          = 480e6
 *          = 480 MHz
 *
 * M should be 6~512
 * N should be 1~32
 *
 * clk_cpu = clk_pll0 / Devider
 *         = 480 MHz  / 4
 *         = 120 MHz
 */
void system_clock_init(void) {
/* WDOG->UNLOCK: WDOGUNLOCK=0xC520 */
  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520); /* Key 1 */
  /* WDOG->UNLOCK: WDOGUNLOCK=0xD928 */
  WDOG->UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928); /* Key 2 */
  /* WDOG->STCTRLH: ?=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,?=0,?=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
  WDOG->STCTRLH = WDOG_STCTRLH_BYTESEL(0x00) |
                 WDOG_STCTRLH_WAITEN_MASK |
                 WDOG_STCTRLH_STOPEN_MASK |
                 WDOG_STCTRLH_ALLOWUPDATE_MASK |
                 WDOG_STCTRLH_CLKSRC_MASK |
                 0x0100U;
  MCG->C4 |= MCG_C4_DMX32_MASK;
}

void system_clock_deinit(void) {
    // PLL0CON = 0;
    // PLL0FEED = 0xAA;
    // PLL0FEED = 0x55;

    // PLL0CFG = 0;
    // PLL0FEED = 0xAA;
    // PLL0FEED = 0x55;

    // SCS = 0;
    // CLKSRCSEL = 0;

    // CCLKCFG = 0;
    // PCLKSEL0 = 0;
    // PCLKSEL1 = 0;
}

void system_power_init(void) {
    // PCONP = PCONP_UART3 | PCONP_GPIO;
}

void system_power_deinit(void) {
    // PCONP = 0;
}

void pinconnect_init(void) {
    // // set P0.0 and P0.1 as TXD3 and RXD3
    // PINSEL0 |= (PIN_FUNC_2 << 0) | (PIN_FUNC_2 << 2);
}

void pinconnect_deinit(void) {
    // PINSEL0 = 0;
}
