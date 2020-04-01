#include "freqdiv.h"
#include <stdint.h>

void freq_div_init(freq_div_t *freq_div) {
    freq_div->reg_num = 0;
}

void freq_div_add(freq_div_t *freq_div, uint16_t presc_cnt, FREQ_DIV_FUN_T func_cb, void *para_p, uint16_t init_cnt) {
    freq_div->freq_div_unit[freq_div->reg_num].presc_cnt = presc_cnt;
    freq_div->freq_div_unit[freq_div->reg_num].fun_cb = func_cb;
    freq_div->freq_div_unit[freq_div->reg_num].para_p = para_p;
    freq_div->freq_div_unit[freq_div->reg_num].current_cnt = init_cnt;
    freq_div->reg_num++;
}

void freq_div_handle(freq_div_t *freq_div) {
    /* Check whether call the callback function */
    for(uint8_t i=0;i<freq_div->reg_num;i++) {
        if(freq_div->freq_div_unit[i].current_cnt == freq_div->freq_div_unit[i].presc_cnt) {
            freq_div->freq_div_unit[i].fun_cb(freq_div->freq_div_unit[i].para_p);
            freq_div->freq_div_unit[i].current_cnt = 0;
        }
        freq_div->freq_div_unit[i].current_cnt++;
    }
}
