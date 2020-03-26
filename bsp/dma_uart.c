#include <stdint.h>
#include "control_board.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "rs485.h"

#ifdef USE_UART_DMA
char send_buf[SEND_BUFF_SIZE];
static volatile uint8_t dma_fg = DMA_IDLE;
static volatile uint8_t dma_er = DMA_IDLE;

void rs485_send_edma(size_t buf_sz) {
    if(dma_fg & 1) {
        dma_fg = DMA_BUZY;
        DMA0->TCD[0].SOFF = 1;                           /* 8 bits */
        DMA0->TCD[0].DOFF = 0;                           /* 8 bits */
        DMA0->TCD[0].SADDR = (uint32_t)&send_buf[0];
        DMA0->TCD[0].DADDR = (uint32_t)&RS485_UART->D;
        DMA0->TCD[0].NBYTES_MLNO = 1;                    /* 1 byte */
        DMA0->TCD[0].CITER_ELINKYES = buf_sz;
        DMA0->TCD[0].BITER_ELINKYES = buf_sz;
        ENABLE_RS485_TRM();
        enable_rs485_txdma();
    }
    else {
        dma_er = DMA_MISS_DATA;
    }
}

uint8_t get_dma_status(void) {
    return dma_fg;
}

uint8_t get_dma_error(void) {
    return dma_er;
}

void clear_dma_error(void) {
    dma_er = DMA_IDLE;
}

/* DMA interrupt */
void RS485_DMA_Handler(void) {
    /* Clear flag */
    DMA0->INT |= DMA_INT_INT0_MASK;

    /* Check done or not */
    if(DMA0->TCD[0].CSR & DMA_CSR_DONE_MASK) {
        /* Done */
        dma_fg = DMA_IDLE;
    }
    else {
        /* Fail */
        dma_fg = DMA_IDLE_BUT_FAIL;
    }
    disable_rs485_txdma();
    RS485_UART->C2 |= UART_C2_TCIE_MASK;
}

#endif /* USE_UART_DMA */
