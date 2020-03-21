#include "hal_tick.h"
#include "stdbool.h"

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

/* Define global variable to check whether timeout */
uint16_t timeout_tick, last_timeout__tick;

void timeout_set_ms(uint16_t _out) {
    timeout_tick = _out;
    last_timeout__tick = _tick;
}

uint8_t is_timeout(void) {
    if(get_tick() - last_timeout__tick > timeout_tick) {
        return true;
    }
    else {
        return false;
    }
}
