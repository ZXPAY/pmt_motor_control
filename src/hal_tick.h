#ifndef HAL_TICK_H
#define HAL_TICK_H

#include <stdint.h>

void init_tick(void);
void inc_tick(void);
void hal_delay(uint32_t delay_ms);
uint32_t get_tick(void);

#endif /* HAL_TICK */
