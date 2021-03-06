/**
 * @file adj_velocity.h
 * @author Xiang-Guan Deng
 * @brief Adjust the stepper motor velocity
 * @date 2020.xx.xx
 *
 * 電流PI回饋功能方塊
 */

#ifndef PI_CURRENT_H
#define PI_CURRENT_H
#include "i_excite_angle.h"

typedef struct _fb_current_ {
    fb_pid_t pid;
    volatile float i_svpwm;
    float low_limit;
    float high_limit;
}fb_current_t;

/**
 * @brief Initialize PI current object patameters
 */
void init_current_para(fb_current_t *fb_current, float kp, float ki, float low_limit, float high_limit);

/**
 * @brief Calculate PI current value to excite SVPWM
 */
void cal_current_correct(fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current);

#endif /* PI_CURRENT_H */
