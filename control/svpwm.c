#include <math.h>
#include "svpwm.h"
#include "i_excite_angle.h"
#include "pi_current.h"

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current) {
    /* 由角差I回饋 和 電流PI回饋計算SVPWM */
    float i_a = fb_current->i_svpwm*sin(fb_exc_angle->th_esvpwm*2*3.141593/360);
    float i_b = fb_current->i_svpwm*cos(fb_exc_angle->th_esvpwm*2*3.141593/360);

    /* 設定i_a、i_b上下界 */
    if(i_a > 0) {
        if(i_a > Ia_MAX) {
            i_a = Ia_MAX;
        }
    }
    else {
        if(i_a < -Ia_MAX) {
            i_a = -Ia_MAX;
        }
    }
    if(i_b > 0) {
        if(i_b > Ib_MAX) {
            i_b = Ib_MAX;
        }
    }
    else {
        if(i_b < -Ib_MAX) {
            i_b = -Ib_MAX;
        }
    }

    pwmAB->pwma = (0.5 + i_a/Ia_MAX*0.5)*60000;
    pwmAB->pwmb = (0.5 + i_b/Ib_MAX*0.5)*60000;
    // pwmAB->pwma = i_a/Ia_MAX*60000;
    // pwmAB->pwmb = i_b/Ib_MAX*60000;
}
