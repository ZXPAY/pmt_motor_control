#ifndef SVPWM_H
#define SVPWM_H

#include <stdint.h>
#include "pi_current.h"

typedef struct _pwmAB_ {
    int32_t pwm1;
    int32_t pwm2;
}pwmAB_t;

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current);


#endif
