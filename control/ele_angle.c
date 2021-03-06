#include "ele_angle.h"
#include "step_accumulator.h"

step_caccumulator_t c_accum = STEP_CACCUMUILATOR_INIT;  /* 命令微步累加器 */
step_saccumulator_t s_accum = STEP_SACCUMUILATOR_INIT;  /* 感測微步累加器 */

void init_cangle(cangle_t *cangle, uint16_t N_step, int16_t init_ele_angle) {
    cangle->K_degree = 360.0/(N_step*FULL_STEPS_NUM)*(90.0/1.8);
    cangle->ele_limit = N_step*FULL_STEPS_NUM;
    cangle->init_ele_angle = init_ele_angle;
    cangle->ele_angle = 0;
}

void set_cangle_step(cangle_t *cangle, uint16_t N_step) {
    cangle->K_degree = 360.0/(N_step*FULL_STEPS_NUM)*(90.0/1.8);
    cangle->ele_limit = N_step*FULL_STEPS_NUM;
}

void update_cangle(cangle_t *cangle, int16_t th_inc) {
    cangle->ele_angle += th_inc;
    if(cangle->ele_angle > 0) {
        if(cangle->ele_angle > cangle->ele_limit) cangle->ele_angle = 0;
    }
    else {
        if(cangle->ele_angle < -cangle->ele_limit) cangle->ele_angle = 0;
    }
    /* 360度餘數 */
    cangle->ele_dangle = (int16_t)(cangle->ele_angle * cangle->K_degree) % 360;

    /* command angle accumlate */
    update_step_caccum(&c_accum, th_inc);
}


void init_sangle(sangle_t *sangle, uint16_t init_mach_angle) {
    sangle->K_degree = SENSOR2DEGREE;
    sangle->init_mach_angle = init_mach_angle;
    sangle->poles = 0;
    sangle->ele_angle = 0;
    sangle->err_count = 0;
    sangle->mach_angle = 0;
    sangle->delta_angle = 0;
}

void update_sangle(sangle_t *sangle, uint16_t enc_angle) {
    int16_t temp_mach = enc_angle - sangle->init_mach_angle;
    if(temp_mach < 0) temp_mach += MAX_ENCODER_VALUE;
    sangle->delta_angle = temp_mach - sangle->mach_angle;
    sangle->mach_angle = temp_mach;

    if(abs_int(sangle->delta_angle) > HALF_ENCODER_VALUE) {
        if(sangle->delta_angle < 0) {
            sangle->delta_angle += MAX_ENCODER_VALUE;
        }
        else {
            sangle->delta_angle -= MAX_ENCODER_VALUE;
        }
    }
    sangle->ele_angle += sangle->delta_angle;
    if(sangle->ele_angle > ENC_ELE_ANGLE_TH_L) {
        sangle->ele_angle -= ENC_ELE_ANGLE_TH_H;
        sangle->err_count += ENC_ELE_ANGLE_ER;
        if(sangle->err_count > ENC_ELE_ANGLE_ER_TH) {
            sangle->ele_angle++;
            sangle->err_count -= EMC_ELE_AMGLE_COUNT;
        }
        sangle->poles++;
        if(sangle->poles >= MAX_ELETRICAL_ROTOR_ANGLE) {
            sangle->poles -= MAX_ELETRICAL_ROTOR_ANGLE;
        }
    }
    else if(sangle->ele_angle < -ENC_ELE_ANGLE_TH_L) {
        sangle->ele_angle += ENC_ELE_ANGLE_TH_H;
        sangle->err_count -= ENC_ELE_ANGLE_ER;
        if(sangle->err_count < -ENC_ELE_ANGLE_ER_TH) {
            sangle->ele_angle--;
            sangle->err_count += EMC_ELE_AMGLE_COUNT;
        }
        sangle->poles--;
        if(sangle->poles < 0) {
            sangle->poles += MAX_ELETRICAL_ROTOR_ANGLE;
        }
    }

    /* Uncomment the below code, it will cause system unstable sometimes */
    /* reset the angle */
    // if(abs_int(sangle->mach_angle - sangle->init_mach_angle) == 0) {
    //     sangle->ele_angle = 0;
    //     sangle->err_count = 0;
    // }
    
    /* 順時針為正 */
    sangle->ele_dangle = (float)sangle->ele_angle * sangle->K_degree;

    update_step_saccum(&s_accum, enc_angle);
}
