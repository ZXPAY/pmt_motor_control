/**
 * @file ele_angle.h
 * @author Xiang-Guan Deng
 * @brief Calculate and manage the machanical and electrical angle
 * @date 2020.xx.xx
 *
 * Encoder resolutio: 16 bits
 * C電子角累進器和S電子角除餘器實作
 */

#ifndef ELE_ANGLE_H
#define ELE_ANGLE_H
#include <stdint.h>

#ifndef abs_int
#define abs_int(X) ((X < 0)? (-X) : (X))
#endif

#define MAX_ELETRICAL_ROTOR_ANGLE  50
#define MAX_ENCODER_VALUE          16384
#define HALF_ENCODER_VALUE         8192
// 327.68
#define ENC_PER_ELE_ANGLE_L        327
#define ENC_PER_ELE_ANGLE_H        328
#define ENC_PER_ELE_ANGLE_UP       32
#define ENC_PER_ELE_ANGLE_UP_THR   -50
#define ENC_PER_ELE_ANGLE_DW       68
#define ENC_PER_ELE_ANGLE_DW_THR   50
#define EMC_PER_ELE_AMGLE_COUNT    100   // per 100 error counters, plus or minus 1 count

#define FULL_STEPS_NUM             200
#define SENSOR2DEGREE              360.0/327.68

/* Define sensor angle type */
typedef struct _sangle_ {
    float K_degree;
    uint16_t init_mach_angle;
    int16_t mach_angle;
    int16_t poles;
    int16_t ele_angle;
    int16_t delta_angle;
    int16_t err_count;
    float ele_dangle;
}sangle_t;

/* Define command angle type */
typedef struct _cangle_ {
    float K_degree;
    int16_t init_ele_angle;
    int16_t ele_angle;
    int16_t ele_limit;
    float ele_dangle;
}cangle_t;


/**
 * @brief get eletrical rotor angle(50) from encoder(16384)
 * 1. encoder is 14-bit resolution, 16384 per revolution
 * 2. 16384/50 = 327.68 (pulses per eletrical rotor angle)
 */
void init_sangle(sangle_t *angle, uint16_t init_mach_angle);

/**
 * @brief TODO
 *
 */
void update_sangle(sangle_t *angle, uint16_t enc_angle);

/**
 * @brief TODO
 *
 */
void init_cangle(cangle_t *angle, uint16_t N_step, int16_t init_ele_angle);

/**
 * @brief TODO
 *
 */
void update_cangle(cangle_t *angle, int16_t th_inc);

#endif
