/**
 * @file pid.h
 * @author Xiang-Guan Deng
 * @brief Correct value by PID method
 * @date 2020.xx.xx
 *
 * Need to tune the kp、ki、kd constant value
 */

#ifndef PID_H
#define PID_H

#ifndef abs_float
#define abs_float(X) ((X < 0)? (-X) : (X))
#endif

#define MAX_CORRECT_POSITIVE   20
#define MAX_CORRECT_NEGATIVE   -20

typedef struct _fb_pid_ {
    float ref;
    float kp;
    float ki;
    float kd;
    float int_er;
    float pre_er;
}fb_pid_t;

float upadte_pid(fb_pid_t *pid, float value);


#endif /* PID_H */
