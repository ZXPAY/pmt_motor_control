#include "pi_current.h"
#include "i_excite_angle.h"

void init_current_para(fb_current_t *fb_current, float kp, float ki, float low_limit, float high_limit) {
    fb_current->pid.kp = kp;
    fb_current->pid.ki = ki;
    fb_current->i_svpwm = 0;
    fb_current->low_limit = low_limit;
    fb_current->high_limit = high_limit;
}

void cal_current_correct(fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current) {
    fb_current->i_svpwm = 0.5*fb_current->low_limit + 0.5*fb_current->high_limit - fb_current->pid.kp*fb_exc_angle->th_er - fb_current->pid.ki*fb_exc_angle->th_cum;
    if(fb_current->i_svpwm < fb_current->low_limit) fb_current->i_svpwm = fb_current->low_limit;
    if(fb_current->i_svpwm > fb_current->high_limit) fb_current->i_svpwm = fb_current->high_limit;
}
