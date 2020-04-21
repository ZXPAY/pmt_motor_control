/**
 * @file adj_velocity.h
 * @author Xiang-Guan Deng
 * @brief Excited angle feedback
 * @date 2020.xx.xx
 *
 * 激磁角回饋
 */

#ifndef I_EXCITE_ANGLE_H
#define I_EXCITE_ANGLE_H
#include <stdint.h>
#include "pid.h"

#ifndef abs_float
#define abs_float(X) ((X < 0)? (-X) : (X))
#endif
#ifndef abs_int
#define abs_int(X) ((X < 0)? (-X) : (X))
#endif

typedef struct _fb_exc_angle_ {
    fb_pid_t pid;
    float th_esvpwm;
    float th_er;
    float th_cum;
    float cum_limit;
}fb_exc_angle_t;

/**
 * @brief TODO
 *
 */
void init_exc_ang_para(fb_exc_angle_t *fb_exc_angle, float ki);

/**
 * @brief TODO
 *
 */
void cal_exc_ang_correct(fb_exc_angle_t *fb_exc_angle, float  e_sdegree, float e_cdegree);

#endif /* I_EXCITE_ANGLE_H */
