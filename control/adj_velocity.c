#include "adj_velocity.h"

void init_cangle_inc(adj_v_t *adj_v) {
    adj_v->add = 0;
    adj_v->limit = ADJ_MAX;
    adj_v->sm = SM_READY;
    adj_v->w_back = 0;
    adj_v->w_phase = 0;
    adj_v->residual_phase = 0;
}

void set_cangle_inc(adj_v_t *adj_v, int16_t th_add) {
    /* Update the new phase adjust angle */
    adj_v->add = th_add;
}

void set_cangle_wback(adj_v_t *adj_v, int16_t w_back) {
    adj_v->w_back = w_back;
}

int16_t get_cangle_inc(adj_v_t *adj_v) {
    if(adj_v->residual_phase > adj_v->limit) {
        adj_v->w_phase = adj_v->limit;
    }
    else {
        adj_v->w_phase = adj_v->residual_phase;
    }
    adj_v->residual_phase += adj_v->add - adj_v->w_phase;
    adj_v->add = 0;

    return adj_v->w_back + adj_v->w_phase;
}

