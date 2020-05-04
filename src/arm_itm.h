#include <stdint.h>

#ifndef ARM_ITM_H
#define ARM_ITM_H

#ifndef DEBUG_ITM
void SWO_Init(uint32_t portBits, uint32_t cpuCoreFreqHz);
void SWO_PrintChar(char c, uint8_t portNo);
#endif

#endif /* ARM_ITM_H */
