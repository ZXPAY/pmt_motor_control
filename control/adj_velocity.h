/**
 * @file adj_velocity.h
 * @author Xiang-Guan Deng
 * @brief Adjust the stepper motor velocity
 * @date 2020.xx.xx
 *
 * 相位調整限速器
 */

#ifndef ADJ_VELOCITY_H
#define ADJ_VELOCITY_H
#include <stdint.h>

/* Define the adject velocity status */
#define SM_READY              0
#define SM_RUNNING            1
#define SM_OVERLIMIT          2

/**
 *  @brief Adjust velocity parameter, state structure
 *  @ingroup
 */
typedef struct _adj_v_ {
    int16_t limit;           /* Limit angle velocity */
    int16_t add;             /* Add new phase angle*/
    int16_t w_back;          /* Background angle velocity */
    int16_t residual_phase;  /* Phase residual state variable */
    int16_t w_phase;         /* Phase angle velocity */
    uint8_t sm;              /* Current status */
}adj_v_t;

/**
 * @brief Initialize command eletrical angle increment
 */
void init_cangle_inc(adj_v_t *adj_v);

/**
 * @brief Set command eletrical angle increment
 */
void set_cangle_inc(adj_v_t *adj_v, int16_t th_add);

/**
 * @brief Set command background eletrical angle speed
 */
void set_cangle_wback(adj_v_t *adj_v, int16_t w_back);

/**
 * @brief Get command angle
 *
 * If angle more than max speed, it will return max spped value
 */
int16_t get_cangle_inc(adj_v_t *adj_v);

#endif /* ADJ_VELOCITY_H */
