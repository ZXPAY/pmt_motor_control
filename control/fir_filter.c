#include <stdint.h>
#include <stdlib.h>
#include "fir_filter.h"
#include "control_board.h"

void init_fir(fir_t *fir, const float *feat, uint_fast8_t feat_sz) {
    fir->fir_index = 0;
    fir->fir_output = 0;
    fir->feat_sz = feat_sz;
    fir->fir_feat = feat;
    for(uint_fast32_t i=0;i<fir->feat_sz;i++) {
        fir->fir_buf[i] = 0;
    }
}

void set_fir(fir_t *fir, float fir_input) {
    /* Test 1, 8 parameters, 7.68 us */
    // for(int_fast16_t i=fir->feat_sz-1;i>0;i--) {
    //     fir->fir_buf[i] = fir->fir_buf[i-1];
    // }
    // fir->fir_buf[0] = fir_input;
    // float temp = 0;
    // for(int_fast16_t i=0;i<fir->feat_sz;i++) {
    //     temp += fir->fir_buf[i] * fir->fir_feat[i];
    // }
    // fir->fir_output = temp;

    /* Test 2, 8 parameters, 6.4 us  */
    fir->fir_buf[fir->fir_index] = fir_input;  // newest data, fir_index
    int_fast16_t buf_index = fir->fir_index;
    float temp = 0;
    for(uint_fast16_t i=0;i<fir->feat_sz;i++) {
        temp += fir->fir_buf[buf_index--] * fir->fir_feat[i];
        if(buf_index < 0) {
            buf_index = fir->feat_sz-1;
        }
    }
    fir->fir_index++;
    if(fir->fir_index == fir->feat_sz) {
        fir->fir_index = 0;
    }

    fir->fir_output = temp;
}

float get_fir(fir_t *fir) {
    return fir->fir_output;
}


void init_mv_avg(mv_avg_t *mv_avg, uint_fast8_t window_size) {
    for(uint_fast16_t i=0;i<window_size;i++) {
        mv_avg->mv_buf[i] = 0;
    }
    mv_avg->mv_index = 0;
    mv_avg->mv_output = 0;
    mv_avg->shift = 0;
    mv_avg->wd_sz = window_size;
    uint_fast8_t temp = window_size;
    while(1) {
        if(temp > 1) {
            mv_avg->shift++;
            temp >>= 1;
        }
        else {
            break;
        }
    }
}

void set_mv_avg(mv_avg_t *mv_avg, int_fast16_t mv_input) {
    /* 8 moving average, 3.44 us */
    int_fast16_t temp = 0;

    mv_avg->mv_buf[mv_avg->mv_index++] = mv_input;
    if(mv_avg->mv_index == mv_avg->wd_sz) {
        mv_avg->mv_index = 0;
    }

    for(uint_fast16_t i=0;i<mv_avg->wd_sz;i++) {
        temp += mv_avg->mv_buf[i];
    }

    mv_avg->mv_output = temp >> mv_avg->shift;
}

int_fast16_t get_mv_avg(mv_avg_t *mv_avg) {
    return mv_avg->mv_output;
}
