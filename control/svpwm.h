/**
 * @file svpwm.h
 * @author Xiang-Guan Deng
 * @brief Calculate SVPWM
 * @date 2020.xx.xx
 *
 * 兩相SVPWM功能方塊，計算出控制的PWM duty值
 */

#ifndef SVPWM_H
#define SVPWM_H

#include <stdint.h>
#include "pi_current.h"

#define DEGREE_TO_RADIAN      (3.141593/180.0)

typedef struct _pwmAB_ {
    int16_t pwm1;
    int16_t pwm2;
}pwmAB_t;

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current);


#endif
