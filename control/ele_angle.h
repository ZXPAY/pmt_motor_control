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

#define MAX_ELETRICAL_ROTOR_ANGLE     50
#define MAX_ENCODER_VALUE             16384
#define HALF_ENCODER_VALUE            8192
// 327.68
#define ENC_ELE_ANGLE_TH_L            327
#define ENC_ELE_ANGLE_TH_H            328
#define ENC_ELE_ANGLE_ER              32
#define ENC_ELE_ANGLE_ER_TH           50
#define EMC_ELE_AMGLE_COUNT           100   /* per 100 error counters, plus or minus 1 count */

#define FULL_STEPS_NUM                200
#define SENSOR2DEGREE                 (-360.0/327.68)  /* 順時鐘為正，encoder順時鐘為遞減 */

/* Define command angle type */
typedef struct _cangle_ {
    float K_degree;
    int16_t init_ele_angle;
    int16_t ele_angle;
    int16_t ele_limit;
    float ele_dangle;
}cangle_t;

void init_cangle(cangle_t *cangle, uint16_t N_step, int16_t init_ele_angle);

/**
 * @brief Set command eletrical angle N_step parameter in run-time
 *
 */
void set_cangle_step(cangle_t *cangle, uint16_t N_step);

/**
 * @brief Update command eletrical angle
 *
 */
void update_cangle(cangle_t *cangle, int16_t th_inc);


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

/**
 * @brief get eletrical rotor angle(50) from encoder(16384)
 * 1. encoder is 14-bit resolution, 16384 per revolution
 * 2. 16384/50 = 327.68 (pulses per eletrical rotor angle)
 */
void init_sangle(sangle_t *sangle, uint16_t init_mach_angle);

/**
 * @brief Update sensor eletrical angle
 *
 */
void update_sangle(sangle_t *sangle, uint16_t enc_angle);

/**
 * @brief Initialize command eletrical angle parameters
 *
 */

#endif
