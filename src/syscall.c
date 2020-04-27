/**
 * @file syscall.c
 * @author Xiang-Guan Deng
 * @brief implment , overwrite the printf, scanf method
 * @date 2020.xx.xx
 *
 * If not use UART DMA, "printf" will call these function, it will increase CPU load
 */
#include "uart.h"

#ifdef DEBUG_ITM

int _write (__attribute__((unused)) int fd, char *ptr, int len) {
    int i;
    for(i = 0; i< len; i++) {
        uart_putc(*ptr++);
    }
    return len;
}

int _read (__attribute__((unused)) int fd, char *ptr, int len) {
    int i;
    for(i = 0; i< len; i++) {
        *ptr++ = uart_getc();
    }
    return len;
}

void _ttywrch(int ch) {
    uart_putc(ch);
}

#endif
