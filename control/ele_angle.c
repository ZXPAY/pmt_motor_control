#include "ele_angle.h"
#include <stdio.h>

void init_sangle(sangle_t *angle, uint16_t init_mach_angle) {
    angle->K_degree = SENSOR2DEGREE;
    angle->init_mach_angle = init_mach_angle;
    angle->poles = 0;
    angle->ele_angle = 0;
    angle->err_count = 0;
    angle->mach_angle = 0;
    angle->delta_angle = 0;
}

void update_sangle(sangle_t *angle, uint16_t enc_angle) {
    int16_t temp_ang = enc_angle - angle->init_mach_angle;
    if(temp_ang < 0) temp_ang += MAX_ENCODER_VALUE;
    angle->delta_angle = temp_ang - angle->mach_angle;
    angle->mach_angle = temp_ang;

    if(abs_int(angle->delta_angle) > HALF_ENCODER_VALUE) {
        if(angle->delta_angle < 0) {
            angle->delta_angle += MAX_ENCODER_VALUE;
        }
        else {
            angle->delta_angle -= MAX_ENCODER_VALUE;
        }
    }
    angle->ele_angle += angle->delta_angle;
    if(angle->ele_angle > ENC_PER_ELE_ANGLE_L) {
        angle->ele_angle -= ENC_PER_ELE_ANGLE_H;
        angle->err_count -= ENC_PER_ELE_ANGLE_UP;
        if(angle->err_count < ENC_PER_ELE_ANGLE_UP_THR) {
            angle->ele_angle--;
            angle->err_count += EMC_PER_ELE_AMGLE_COUNT;
        }
        angle->poles++;
        if(angle->poles >= MAX_ELETRICAL_ROTOR_ANGLE) {
            angle->poles -= MAX_ELETRICAL_ROTOR_ANGLE;
        }
    }
    else if(angle->ele_angle < 0) {
        angle->ele_angle += ENC_PER_ELE_ANGLE_H;
        angle->err_count += ENC_PER_ELE_ANGLE_DW;
        if(angle->err_count > ENC_PER_ELE_ANGLE_DW_THR) {
            angle->ele_angle++;
            angle->err_count -= EMC_PER_ELE_AMGLE_COUNT;
        }
        angle->poles--;
        if(angle->poles < 0) {
            angle->poles += MAX_ELETRICAL_ROTOR_ANGLE;
        }
    }
    // reset the angle
    if(angle->mach_angle == angle->init_mach_angle) {
        angle->ele_angle = 0;
        angle->err_count = 0;
    }
    angle->ele_dangle = (float)angle->ele_angle*angle->K_degree;
}

void init_cangle(cangle_t *angle, uint16_t N_step, int16_t init_ele_angle) {
    angle->K_degree = 360.0/(N_step*FULL_STEPS_NUM);
    angle->ele_limit = N_step*FULL_STEPS_NUM;
    angle->init_ele_angle = init_ele_angle;
    angle->ele_angle = 0;
}

void update_cangle(cangle_t *angle, int16_t th_inc) {
    angle->ele_angle += th_inc;
    if(angle->ele_angle > angle->ele_limit) angle->ele_angle = 0;

    angle->ele_dangle = (float)angle->ele_angle*angle->K_degree;
    /* Constrain angle */
    if(angle->ele_dangle > 360.0) {angle->ele_dangle = 0;}
    if(angle->ele_dangle < -360.0) {angle->ele_dangle = 0;}
}
