/**
 * @file control_config.h
 * @author Xiang-Guan Deng
 * @brief Config control parameters
 * @date 2020.xx.xx
 *
 * 設定control相關參數
 */

#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

/* Define default N-step */
#define N_STEP      2

/* Define parameters */
#define EXC_KI                       0.001                                   /* 激磁角Ki回饋 */
#define SVPWM_LIMIT_UNIT             (1/N_STEP/EXC_KI)                       /* SVPWM超過sensor電子角正負90度修正 */
#define I_SVPWM_LOW                  -0.65                                   /* 電流下限 */
#define I_SVPWM_HIGH                 0.65                                    /* 電流上限 */
#define I_SVPWM_MEAN                 ((-I_SVPWM_LOW + I_SVPWM_HIGH) * 0.5)   /* 電流平均值 */
#define I_SVPWM_NORMALIZE_GAIN       (1 / (I_SVPWM_MEAN))                    /* I_SVPWM to 0~1 */
#define I_SVPWM_KP                   (I_SVPWM_MEAN / (90 / EXC_KI) * 10)     /* 電流Kp回饋 */
#define I_SVPWM_KI                   (I_SVPWM_MEAN / (90 / EXC_KI) * 1.5)    /* 電流Ki回饋，越小越省電 */
#define STEP_C_THETA_TO_LENGTH       1.0                                     /* 命令微步累加器角度轉換成長度增益 */
#define STEP_S_THETA_TO_LENGTH       1.0                                     /* 感測微步累加器角度轉換成長度增益 */



/* Define maximum velocity */
#define ADJ_MAX     N_STEP                                                   /* 最大調整正反轉步數 */


#endif /* CONTROL_CONFIG_H */
