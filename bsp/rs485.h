#include <stdio.h>
#include "control_board.h"

#define ENABLE_RS485_TRM()    GPIO_RS485_CTR->PSOR |= (1<<PIN_RS485_CTR)
#define DISABLE_RS485_TRM()   GPIO_RS485_CTR->PCOR |= (1<<PIN_RS485_CTR)

/* RS485 transmit macro */
#ifdef USE_UART_DMA

extern char send_buf[SEND_BUFF_SIZE];

#define DMA_IDLE                       1
#define DMA_BUZY                       2
#define DMA_IDLE_BUT_FAIL              3
#define DMA_MISS_DATA                  4
#define RS485_DMA_Handler              DMA0_IRQHandler
#define RS485_trm(format, args...)     rs485_send_edma(sprintf(send_buf, "[485],"format, ##args));  \
                                       /* Disable when transmit complete */

void rs485_send_edma(size_t buf_sz);
uint8_t get_dma_status(void);
uint8_t get_dma_error(void);
void clear_dma_error(void);


#else
#define RS485_trm(format, args...)     ENABLE_RS485_TRM();                                        \
                                       printf("[485],"format, ##args);                          \
                                       RS485_UART->C2 |= UART_C2_TCIE_MASK;
#endif /* USE_UART_DMA */





