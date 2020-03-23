#include <math.h>
#include "svpwm.h"
#include "i_excite_angle.h"
#include "pi_current.h"
#include "control_board.h"

/* TODO */

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current) {
    /* 由角差I回饋 和 電流PI回饋計算SVPWM */
    float ia_svpwm = (-fb_current->i_svpwm)*sin(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);
    float ib_svpwm = (-fb_current->i_svpwm)*cos(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);
    // float ia = sin(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);
    // float ib = cos(fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN);

    /* 設定i_a、i_b上下界 */
    // if(i_a > 0) {
    //     if(i_a > Ia_MAX) {
    //         i_a = Ia_MAX;
    //     }
    // }
    // else {
    //     if(i_a < -Ia_MAX) {
    //         i_a = -Ia_MAX;
    //     }
    // }
    // if(i_b > 0) {
    //     if(i_b > Ib_MAX) {
    //         i_b = Ib_MAX;
    //     }
    // }
    // else {
    //     if(i_b < -Ib_MAX) {
    //         i_b = -Ib_MAX;
    //     }
    // }

    pwmAB->pwm1 = ((int32_t)(ia_svpwm * PERIOD_COUNT) + PERIOD_COUNT) >> 1;
    pwmAB->pwm2 = ((int32_t)(ib_svpwm * PERIOD_COUNT) + PERIOD_COUNT) >> 1;

    // pwmAB->pwm1 = ia*PERIOD_COUNT;
    // pwmAB->pwm2 = ib*PERIOD_COUNT;
}
