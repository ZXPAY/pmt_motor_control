#include <math.h>
#include "svpwm.h"
#include "i_excite_angle.h"
#include "pi_current.h"
#include "control_config.h"
#include "control_board.h"
#include "fir_filter.h"

extern mv_avg_t pwma_mv_avg;      /* PWM phase A Moving Average object */
extern mv_avg_t pwmb_mv_avg;      /* PWM phase B Moving Average object */

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, __attribute__((unused))fb_current_t *fb_current) {
    float temp_ang = fb_exc_angle->th_esvpwm*DEGREE_TO_RADIAN;
    float ia_svpwm;
    float ib_svpwm;
#ifdef ENABLE_CURRENT_PI
    /* 由角差I回饋 和 電流PI回饋計算SVPWM */
    if(fb_current->i_svpwm < 0) {
        ia_svpwm = (-fb_current->i_svpwm)*I_SVPWM_GAIN*sinf(temp_ang);
        ib_svpwm = (-fb_current->i_svpwm)*I_SVPWM_GAIN*cosf(temp_ang);
    }
    else {
        ia_svpwm = (fb_current->i_svpwm)*I_SVPWM_GAIN*sinf(temp_ang);
        ib_svpwm = (fb_current->i_svpwm)*I_SVPWM_GAIN*cosf(temp_ang);
    }
#else
    ia_svpwm = sinf(temp_ang);
    ib_svpwm = cosf(temp_ang);
#endif
    pwmAB->pwm1 = ((int16_t)(ia_svpwm * PERIOD_COUNT) + PERIOD_COUNT) >> 1;
    pwmAB->pwm2 = ((int16_t)(ib_svpwm * PERIOD_COUNT) + PERIOD_COUNT) >> 1;

    set_mv_avg(&pwma_mv_avg, pwmAB->pwm1);
    set_mv_avg(&pwmb_mv_avg, pwmAB->pwm2);

}
