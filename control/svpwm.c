#include <math.h>
#include "svpwm.h"
#include "i_excite_angle.h"
#include "pi_current.h"

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current) {
    float i_a = fb_current->i_svpwm*cos(fb_exc_angle->th_esvpwm);
    float i_b = fb_current->i_svpwm*sin(fb_exc_angle->th_esvpwm);
    pwmAB->pwma = 0.5 + i_a/Ia_MAX*0.5;
    pwmAB->pwmb = 0.5 + i_b/Ib_MAX*0.5;
}
