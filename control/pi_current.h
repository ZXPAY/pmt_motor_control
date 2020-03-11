/**
 * @file adj_velocity.h
 * @author Xiang-Guan Deng
 * @brief Adjust the stepper motor velocity
 * @date 2020.xx.xx
 *
 * 電流回饋
 */

#ifndef PI_CURRENT_H
#define PI_CURRENT_H
#include "i_excite_angle.h"

/* Experiment value */
#define Ia_MAX        0.5
#define Ib_MAX        0.5

typedef struct _fb_current_ {
    fb_pid_t pid;
    float i_svpwm;
    float low_limit;
    float high_limit;
}fb_current_t;

/** @brief TODO
 *
 */
void init_current_para(fb_current_t *fb_current, float kp, float ki, float low_limit, float high_limit);

/** @brief TODO
 *
 */
void cal_current_correct(fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current);

#endif /* PI_CURRENT_H */
