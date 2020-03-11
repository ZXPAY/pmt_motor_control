#ifndef SIN_COS_VAL_TABLE_H
#define SIN_COS_VAL_TABLE_H

#include <stdint.h>

/* Generate sine and cose data size */
#define SIN_COS_DOTS_MAX   256
#define SIN_START_INDEX    0

typedef struct _sin_cos_data {
    uint16_t MAX_NUM;
    int16_t index_sin;
    int16_t sign_sin;
    int16_t index_cos;
    int16_t sign_cos;
    int32_t sin_data[SIN_COS_DOTS_MAX];
}sin_cos_data_t;

void init_sin_cos_table(uint16_t period_counts, uint16_t n_step);
int32_t get_sin(void);
int32_t get_cos(void);

#endif /* SIN_COS_VAL_TABLE_H */
