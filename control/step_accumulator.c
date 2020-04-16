/**
 * @file step_accuimulator.c
 * @author Xiang-Guan Deng
 * @brief Collect and update command and sensor step and length
 * @date 2020.xx.xx
 *
 */
#include "step_accumulator.h"

/* implement command step accumulator */
void set_caccum_k(step_caccumulator_t *c_accum, float k_theta2legnth) {
    c_accum->k_theta2legnth = k_theta2legnth;
}

void update_step_caccum(step_caccumulator_t *c_accum, int32_t th_inc) {
    /* save last th_inc value */
    c_accum->th_inc = th_inc;
    /* accumulate theta */
    c_accum->c_theta_total += c_accum->th_inc;
    /* transfer theta to length */
    c_accum->c_length = (float)c_accum->c_theta_total * c_accum->k_theta2legnth;
}

float get_step_caccum_length(step_caccumulator_t *c_accum) {
    return c_accum->c_length;
}


/* implement sensor step accumulator */
void set_saccum_k(step_saccumulator_t *s_accum, float k_theta2legnth) {
    s_accum->k_theta2legnth = k_theta2legnth;
}

void set_saccum_th_init(step_saccumulator_t *s_accum, int32_t th_init) {
    s_accum->th_init = th_init;
}

void update_step_saccum(step_saccumulator_t *s_accum, int32_t th_s) {
    /* update current sensor theta */
    s_accum->th_s = th_s;
    /* save last delta theta value */
    s_accum->delta_th = s_accum->th_s - s_accum->last_th_s;
    if(abs_int(s_accum->delta_th) > SENSOR_HALF) {
        /* ZERO to MAX */
        if(s_accum->delta_th > 0) {
            s_accum->cycles++;
        }
        /* MAX to ZERO */
        else {
            s_accum->cycles--;
        }
    }
    /* transfer theta to length */
    s_accum->s_length = (float)(s_accum->cycles*SENSOR_RES + s_accum->th_s - s_accum->th_init) * s_accum->k_theta2legnth;
    /* update last sensor theta */
    s_accum->last_th_s = th_s;
}

float get_step_saccum_length(step_saccumulator_t *s_accum) {
    return s_accum->s_length;
}

