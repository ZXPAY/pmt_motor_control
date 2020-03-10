/**
 * @file uart.c
 * @author LiYu87
 * @brief
 * @date 2020.02.17
 */

/**
 * @file uart.c
 * @author LiYu87
 * @date 2020.02.17
 * @brief uart3 operation functions
 */

#include "uart.h"

#include "MKV30F12810.h"
#include "control_board_v2.h"

#define MUL_VAL 4
#define DIVADD_VAL 1

#define DLL_VAL 39
#define DLM_VAL 0

void uart_init(void) {
    init_hw_rs485();
}

void uart_putc(uint8_t data) {
    RS485_UART->D = (uint8_t)data;
    while(!(RS485_UART->S1 & UART_S1_TDRE_MASK));
}

uint8_t uart_getc(void) {
    while(!(RS485_UART->S1 & UART_S1_RDRF_MASK));
    return RS485_UART->D;
}
