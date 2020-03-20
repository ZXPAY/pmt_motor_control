#include <stdio.h>
#include "control_board.h"

#define ENABLE_RS485_TRM()    GPIO_CTR1->PSOR |= (1<<PIN_CTR1)
#define DISABLE_RS485_TRM()   GPIO_CTR1->PCOR |= (1<<PIN_CTR1)

/* RS485 transmit macro */
#define RS485_trm(format, args...)  ENABLE_RS485_TRM();                    \
                                    printf("\r[RS485] "format, ##args);    \
                                    DISABLE_RS485_TRM();                   \



