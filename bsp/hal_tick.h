#ifndef HAL_TICK_H
#define HAL_TICK_H

#include <stdint.h>

/* system tick */
void init_tick(void);

/* This function should put into the timer or systick (1 ms) */
void inc_tick(void);
void hal_delay(uint32_t delay_ms);
uint32_t get_tick(void);


/* timeout */
void timeout_set_ms(uint16_t _out);
uint8_t is_timeout(void);

#endif /* HAL_TICK */
