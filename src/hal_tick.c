#include "hal_tick.h"

volatile uint32_t _tick = 0;

void init_tick(void) {
    _tick = 0;
}

void inc_tick(void) {
    _tick++;
}

void hal_delay(uint32_t delay_ms) {
    uint32_t now_tick = _tick;
    while(_tick - now_tick < delay_ms);
}

uint32_t get_tick(void) {
    return _tick;
}
