#include <stdint.h>
#include "cortex_m4.h"

void SWO_Init(uint32_t portBits, uint32_t cpuCoreFreqHz) {
    uint32_t SWOSpeed = 64000; /* default 64k baud rate */
    uint32_t SWOPrescaler = (cpuCoreFreqHz / SWOSpeed) - 1; /* SWOSpeed in Hz, note that cpuCoreFreqHz is expected to be match the CPU core clock */

    DEMCR = ((1<<24U));

    // CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk; /* enable trace in core debug */
    TPIU_SPPR = 0x00000002; /* "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO NRZ, 1: SWO Manchester encoding) */
    TPIU_ACPR = SWOPrescaler; /* "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output */
    *((volatile unsigned *)(ITM_BASE + 0x00FB0)) = 0xC5ACCE55; /* ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC */

    // ITM_TCR = ITM_TCR_TraceID(1) | ITM_TCR_SWOENA_MASK | ITM_TCR_SYNCENA_MASK | ITM_TCR_ITMENA_MASK; /* ITM Trace Control Register */
    ITM_TCR = ITM_TCR_SWOENA_MASK | ITM_TCR_SYNCENA_MASK | ITM_TCR_ITMENA_MASK; /* ITM Trace Control Register */
    ITM_TPR = 1;              /* ITM Trace Privilege Register */
    ITM_TER = portBits;
    DWT_CTRL = 0x400003FE;    /* DWT_CTRL */
    TPIU_FFSR = 0x00000100;   /* Formatter and Flush Control Register */

}

void SWO_PrintChar(char c, uint8_t portNo) {
    if((ITM_TCR & ITM_TCR_ITMENA_MASK) == 0) {
        return;
    }

    if((ITM_TER & (1UL << portNo)) == 0) {
        return;
    }

    while(ITM_Port32(portNo) == 0);

    ITM_Port8(portNo) = c;
}

