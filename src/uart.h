/**
 * @file uart.h
 * @author LiYu87
 * @date 2020.01.07
 * @brief uart3 operaction functions.
 */

#ifndef UART_H
#define UART_H

#include <inttypes.h>

void uart_init(void);
void uart_putc(uint8_t data);
uint8_t uart_getc(void);

#endif  // UART_H
