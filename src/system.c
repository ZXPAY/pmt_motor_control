/**
 * @file system.c
 * @author Deng Xiang-Guan
 * @date 2020.01.07
 * @brief system clock初始化和預設systick設定為1K
 */

#include "system.h"
#include "control_board.h"
#include "cortex_m4.h"

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

    /* Internal clock setting 72 MHz */
    MCG->C2 = MCG_C2_IRCS_MASK;
    MCG->C4 |= MCG_C4_DRST_DRS(2) | MCG_C4_DMX32_MASK;
}

void system_clock_deinit(void) {

}

void systick_init(void) {
    // Initialize SysTick, setting slot 1 ms
    SYST_RVR = (uint32_t)(SYS_CLOCK_FREQ/1000);   /* 24 bits */
    SYST_CSR = 0x07;
}

