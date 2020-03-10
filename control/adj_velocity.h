/**
 * @file adj_velocity.h
 * @author Xiang-Guan Deng
 * @brief Adjust the stepper motor velocity
 * @date 2020.xx.xx
 */

#ifndef ADJ_VELOCITY_H
#define ADJ_VELOCITY_H
#include <stdint.h>

/* Define the adject velocity status */
#define SM_READY        0
#define SM_OVERLIMIT    1
#define SM_RUNNING      2
#define SM_OVERFLOW     3

/* Define maximum velocity */
#define ADJ_MAX         5

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

/** brief TODO
 *
 */
void init_cangle_inc(adj_v_t *adj_v);

/** brief TODO
 *
 */
void set_cangle_inc(adj_v_t *adj_v, int16_t th_add);

/** brief TODO
 *
 */
void set_cangle_wback(adj_v_t *adj_v, int16_t w_back);

/** brief TODO
 *
 */
int16_t get_cangle_inc(adj_v_t *adj_v);

#endif /* ADJ_VELOCITY_H */
