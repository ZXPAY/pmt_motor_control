#include <stdint.h>
#include "control_board.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup

uint8_t send_buf[SEND_BUFF_SIZE];

void rs485_send_edma(uint16_t size) {
    DMA0->TCD[0].SOFF = 1;     /* 8 bits */
    DMA0->TCD[0].DOFF = 0;     /* 8 bits */
    DMA0->TCD[0].SADDR = (uint32_t)&send_buf[0];
    DMA0->TCD[0].DADDR = (uint32_t)&RS485_UART->D;
    DMA0->TCD[0].NBYTES_MLNO = 1;  /* 1 byte */
    DMA0->TCD[0].CITER_ELINKYES = SEND_BUFF_SIZE;
    DMA0->TCD[0].BITER_ELINKYES = SEND_BUFF_SIZE;
    enable_rs485_txdma();
}



