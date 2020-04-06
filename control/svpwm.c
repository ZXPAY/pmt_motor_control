#include <math.h>
#include "svpwm.h"
#include "i_excite_angle.h"
#include "pi_current.h"
#include "control_board.h"

/* TODO */

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current) {
#ifndef DISABLE_I_PI
    /* 由角差I回饋 和 電流PI回饋計算SVPWM */
    float ia_svpwm = (-fb_current->i_svpwm)*sin(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);
    float ib_svpwm = (-fb_current->i_svpwm)*cos(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);
#else
    float ia_svpwm = sin(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);
    float ib_svpwm = cos(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);
#endif
    pwmAB->pwm1 = ((int32_t)(ia_svpwm * PERIOD_COUNT) + PERIOD_COUNT) >> 1;
    pwmAB->pwm2 = ((int32_t)(ib_svpwm * PERIOD_COUNT) + PERIOD_COUNT) >> 1;
}
