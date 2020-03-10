#include "pid.h"

float upadte_pid(fb_pid_t *pid, float value) {
    float error = value - pid->ref;
    float temp_d_er = error - pid->pre_er;
    if(abs_float(error) > abs_float(pid->pre_er)) {
        pid->int_er += error;
    }
    else {
        pid->int_er = 0;
    }
    pid->pre_er = error;
    float correct = pid->kp*error + pid->ki*pid->int_er + pid->kd*temp_d_er;
    if(correct > MAX_CORRECT_POSITIVE) correct = MAX_CORRECT_POSITIVE;
    if(correct < MAX_CORRECT_NEGATIVE) correct = MAX_CORRECT_NEGATIVE;
    return correct;
}



