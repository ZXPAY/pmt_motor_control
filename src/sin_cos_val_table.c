#include "math.h"
#include "sin_cos_val_table.h"

sin_cos_data_t sin_cos_table = {       \
    .index_sin = SIN_START_INDEX,      \
    .sign_sin = 0,                     \
    .sign_cos = 0,                     \
};


void init_sin_cos_table(uint16_t period_counts, uint16_t n_step) {
    sin_cos_table.index_cos = (n_step*4)/4;
    sin_cos_table.MAX_NUM = (n_step*4);
    for(uint16_t i=0;i<sin_cos_table.MAX_NUM;i++) {
        sin_cos_table.sin_data[i] = (float)period_counts*sin(2*3.14159265359*i/(float)sin_cos_table.MAX_NUM);
    }
}

int16_t get_sin_sign(void) {
    if(sin_cos_table.sin_data[sin_cos_table.index_sin] < 0) {
        return -1;
    }
    else {
        return 1;
    }
}

int32_t get_sin(void) {
    int32_t temp = sin_cos_table.sin_data[sin_cos_table.index_sin++];
    if(sin_cos_table.index_sin == sin_cos_table.MAX_NUM) sin_cos_table.index_sin = 0;
    return temp;
}


int16_t get_cos_sign(void) {
    if(sin_cos_table.sin_data[sin_cos_table.index_cos] < 0) {
        return -1;
    }
    else {
        return 1;
    }
}

int32_t get_cos(void) {
    int32_t temp = sin_cos_table.sin_data[sin_cos_table.index_cos++];
    if(sin_cos_table.index_cos == sin_cos_table.MAX_NUM) sin_cos_table.index_cos = 0;
    return temp;
}
