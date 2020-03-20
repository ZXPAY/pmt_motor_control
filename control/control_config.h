/**
 * @file control_config.h
 * @author Xiang-Guan Deng
 * @brief Config control parameters
 * @date 2020.xx.xx
 *
 */

#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

/* Define parameters */
#define EXC_KI                   0.1          /* 激磁角Ki回饋 */
#define I_SVPWM_KP               1/360.0      /* 電流Kp回饋 */
#define I_SVPWM_KI               0.0          /* 電流Ki回饋 */
#define I_SVPWM_LOW              0.6          /* 電流下限 */
#define I_SVPWM_HIGH             0.6          /* 電流上限 */
#define STEP_C_THETA_TO_LENGTH   1.0          /* 命令微步累加器角度轉換成長度增益 */
#define STEP_S_THETA_TO_LENGTH   1.0          /* 感測微步累加器角度轉換成長度增益 */


/* Define default N-step */
#define N_STEP 16


#endif /* CONTROL_CONFIG_H */
