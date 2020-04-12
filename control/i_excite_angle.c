#include "i_excite_angle.h"
#include "rs485.h"
void init_exc_ang_para(fb_exc_angle_t *fb_exc_angle, float ki) {
    fb_exc_angle->pid.kp = 0;
    fb_exc_angle->pid.ki = ki;
    fb_exc_angle->pid.kd = 0;
    fb_exc_angle->th_cum = 0;
    fb_exc_angle->th_er = 0;
    fb_exc_angle->th_esvpwm = 0;
    fb_exc_angle->cum_limit = 90.0/fb_exc_angle->pid.ki;
    fb_exc_angle->last_er = 0;
}

void cal_exc_ang_correct(fb_exc_angle_t *fb_exc_angle, float  e_sdegree, float e_cdegree) {
    fb_exc_angle->th_er = e_sdegree - e_cdegree;     /* 計算C電子角(命令) 與 S電子角誤差(感測) (度度量) */
    if(abs_float(fb_exc_angle->th_er) >= 180) {
        /* e_sdegree - e_cdegree > 180 */
        if(fb_exc_angle->th_er >= 0) {
            /* 馬達落後 */
            fb_exc_angle->th_er -= 360;
        }
        else {
            /* 馬達領先 */
            fb_exc_angle->th_er += 360;
        }
    }
    
    fb_exc_angle->th_cum += fb_exc_angle->th_er;     /* 累計誤差 */

    /* 限制上下界 */
    if(fb_exc_angle->th_cum >= fb_exc_angle->cum_limit) fb_exc_angle->th_cum  =  fb_exc_angle->cum_limit;
    if(fb_exc_angle->th_cum <= -fb_exc_angle->cum_limit) fb_exc_angle->th_cum = -fb_exc_angle->cum_limit;

#ifdef ENABLE_EXI_ANGLE_I
    /* 計算 theta_svpwm 值 (角差I回饋) */
    fb_exc_angle->th_esvpwm = e_cdegree - fb_exc_angle->pid.ki*fb_exc_angle->th_cum;
#else
    fb_exc_angle->th_esvpwm = e_cdegree;
#endif
    /* theta svpwm to positive */
    if(fb_exc_angle->th_esvpwm > 360)  fb_exc_angle->th_esvpwm -= 360;
    else if(fb_exc_angle->th_esvpwm < 0)    fb_exc_angle->th_esvpwm += 360;

    // if(abs_float(fb_exc_angle->th_esvpwm - e_sdegree) > 90) {
    //     if(e_cdegree > e_sdegree) {
    //         fb_exc_angle->th_esvpwm -= 30;
    //     }
    //     else {
    //         fb_exc_angle->th_esvpwm += 30;
    //     }
    // }

    fb_exc_angle->last_er = fb_exc_angle->th_er;
}
