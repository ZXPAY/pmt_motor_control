/**
 * @file step_accuimulator.h
 * @author Xiang-Guan Deng
 * @brief Collect and update command and sensor step and length
 * @date 2020.xx.xx
 *
 */
#ifndef STEP_ACCUMULATOR
#define STEP_ACCUMULATOR
#include <stdint.h>

#ifndef abs_int
#define abs_int(X) ((X < 0)? (-X) : (X))
#endif

#define STEP_CACCUMUILATOR_INIT   {\
    .th_inc = 0,                   \
    .c_theta_total = 0,            \
    .c_length = 0.0,               \
    .k_theta2legnth = 0.0,         \
    }                              \

/**
 * @brief Define command step accumulator structure
 *
 */
typedef struct _step_caccumulator {
    int32_t th_inc;              /* theta increased */
    int32_t c_theta_total;       /* total command angle */
    float c_length;              /* total length */
    float k_theta2legnth;        /* theta to length gain */
}step_caccumulator_t;

/**
 * @brief Set command theta accumulated value to length
 */
void set_caccum_k(step_caccumulator_t *c_accum, float k_theta2legnth);

/**
 * @brief Update command theta accumulated value
 */
void update_step_caccum(step_caccumulator_t *c_accum, int32_t th_inc);

/**
 * @brief Get command length
 */
float get_step_caccum_length(step_caccumulator_t *c_accum);


#define SENSOR_RES         (16384)
#define SENSOR_HALF        (SENSOR_RES>>1)
#define STEP_SACCUMUILATOR_INIT   {\
    .th_s = 0,                     \
    .last_th_s = 0,                \
    .delta_th = 0,                 \
    .th_init = 0,                  \
    .cycles = 0,                   \
    .s_theta_total = 0,            \
    .s_length = 0.0,               \
    .k_theta2legnth = 0.0,         \
    }                              \

/**
 * @brief Define sensor step accumulator structure
 *
 */
typedef struct _step_saccumulator {
    int32_t th_s;                  /* current sensor theta */
    int32_t last_th_s;             /* last sensor theta */
    int32_t delta_th;              /* delta theta */
    int32_t th_init;               /* initial theta position */
    int32_t cycles;                /* rounds */
    int32_t s_theta_total;         /* total command angle */
    float s_length;                /* total length */
    float k_theta2legnth;          /* theta to length gain */
}step_saccumulator_t;

/**
 * @brief Set sensor theta accumulated value to length
 */
void set_saccum_k(step_saccumulator_t *s_accum, float k_theta2legnth);

/**
 * @brief Set sensor theta initial position
 */
void set_saccum_th_init(step_saccumulator_t *s_accum, int32_t th_init);

/**
 * @brief Update sensor theta accumulated value
 */
void update_step_saccum(step_saccumulator_t *s_accum, int32_t th_s);

/**
 * @brief Get sensor length
 */
float get_step_saccum_length(step_saccumulator_t *s_accum);

#endif /* STEP_ACCUMULATOR */
