#ifndef FREQ_DIV_H
#define FREQ_DIV_H

#include <stdint.h>

#define MAX_FUN_NUM       5

typedef void (*FREQ_DIV_FUN_T)(void *);

typedef struct _freq_div_unit {
    uint16_t presc_cnt;
    uint16_t current_cnt;
    FREQ_DIV_FUN_T fun_cb;
    void *para_p;
}freq_div_unit_t;

typedef struct _freq_div {
    freq_div_unit_t freq_div_unit[MAX_FUN_NUM];
    uint8_t reg_num;
}freq_div_t;

void freq_div_init(freq_div_t *freq_div);
void freq_div_add(freq_div_t *freq_div, uint16_t presc_cnt, FREQ_DIV_FUN_T func_cb, void *para_p, uint16_t init_cnt);
void freq_div_handle(freq_div_t *freq_div);



#endif
