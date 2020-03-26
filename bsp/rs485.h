#include <stdio.h>
#include "control_board.h"

#define ENABLE_RS485_TRM()    GPIO_CTR1->PSOR |= (1<<PIN_CTR1)
#define DISABLE_RS485_TRM()   GPIO_CTR1->PCOR |= (1<<PIN_CTR1)

extern char send_buf[SEND_BUFF_SIZE];

/* RS485 transmit macro */
#ifdef USE_UART_DMA
#define DMA_IDLE                    1
#define DMA_BUZY                    2
#define DMA_IDLE_BUT_FAIL           3
#define DMA_MISS_DATA               4
#define RS485_Handler               DMA0_IRQHandler
#define RS485_trm(format, args...)  ENABLE_RS485_TRM();                                               \
                                    rs485_send_edma(sprintf(send_buf, "[RS485] "format, ##args));     \
                                    DISABLE_RS485_TRM();

void rs485_send_edma(size_t buf_sz);
uint8_t get_dma_error(void);
void clear_dma_error(void);


#else
    #define RS485_trm(format, args...)  ENABLE_RS485_TRM();                                           \
                                        printf("[RS485] "format, ##args);                             \
                                        DISABLE_RS485_TRM();
#endif /* USE_UART_DMA */





