#include "uart.h"

#ifndef USE_UART_DMA

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
