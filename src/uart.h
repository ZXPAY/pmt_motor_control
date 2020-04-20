/**
 * @file uart.h
 * @author Deng Xiang-Guan
 * @date 2020.XX.XX
 * @brief RS485 uart operation functions
 */

#ifndef UART_H
#define UART_H

#include <inttypes.h>

void uart_init(void);
void uart_putc(uint8_t data);
uint8_t uart_getc(void);

#endif /* UART_H */
