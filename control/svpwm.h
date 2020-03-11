#ifndef SVPWM_H
#define SVPWM_H

#include <stdint.h>
#include "pi_current.h"

typedef struct _pwmAB_ {
    uint32_t pwma;
    uint32_t pwmb;
}pwmAB_t;

void cal_pwmAB(pwmAB_t *pwmAB, fb_exc_angle_t *fb_exc_angle, fb_current_t *fb_current);


#endif
