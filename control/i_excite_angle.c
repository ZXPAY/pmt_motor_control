#include "i_excite_angle.h"

void init_exc_ang_para(fb_exc_angle_t *fb_exc_angle, float ki) {
    fb_exc_angle->pid.kp = 0;
    fb_exc_angle->pid.ki = ki;
    fb_exc_angle->pid.kd = 0;
    fb_exc_angle->th_cum = 0;
    fb_exc_angle->th_er = 0;
    fb_exc_angle->th_esvpwm = 0;
}

void cal_exc_ang_correct(fb_exc_angle_t *fb_exc_angle, float  e_sdegree, float e_cdegree) {
    fb_exc_angle->th_er = e_sdegree - e_cdegree;
    fb_exc_angle->th_cum += fb_exc_angle->th_er;

    if(fb_exc_angle->th_cum > ACCUMULATE_MAX) fb_exc_angle->th_cum = ACCUMULATE_MAX;
    if(fb_exc_angle->th_cum < ACCUMULATE_MIN) fb_exc_angle->th_cum = ACCUMULATE_MIN;
    /* Update the SVPWM I feedback */
    if(abs_int(fb_exc_angle->th_cum) < 90) {
        fb_exc_angle->th_esvpwm = e_cdegree - fb_exc_angle->pid.ki*fb_exc_angle->th_cum;
    }
    else {
        /* Positive */
        if(fb_exc_angle->th_cum >= 0) {
            fb_exc_angle->th_esvpwm = e_cdegree - 90;
        }
        /* Negative */
        else {
            fb_exc_angle->th_esvpwm = e_cdegree + 90;
        }
    }


}
