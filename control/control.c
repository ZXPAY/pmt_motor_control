#include "control.h"

/* Include standard C library */
#include <math.h>

/* Include BSP, it't hardware dependent */
#include "rs485.h"
#include "hal_as5047d.h"
#include "hal_drv8847_s.h"
#include "control_board.h"

/* Include hal delay library */
#include "hal_tick.h"

/* Include control config file */
#include "control_config.h"

/* Include control library */
#include "ele_angle.h"
#include "adj_velocity.h"
#include "pi_current.h"
#include "i_excite_angle.h"
#include "svpwm.h"
#include "step_accumulator.h"
#include "fir_filter.h"

#include "cortex_m4.h"

extern as50474_t as5047d;               /* AS5047D motor encoder IC */
extern drv8847_s_t drv8847_s;               /* DRV8847 motor drive IC */

/* Define stepper motor control object */
sangle_t sangle;                         /* 感測角 */
cangle_t cangle;                         /* 命令角 */
adj_v_t adj_v;                           /* 相位調整限速器 */
fb_exc_angle_t fb_exc_angle;             /* 激磁角回饋 */
fb_current_t fb_current;                 /* 電流回饋 */
pwmAB_t pwm12;                           /* 1A1B 2A2B PWM */
mv_avg_t enc_mv_avg;                     /* 馬達編碼器 Moving Average object */
fir_t enc_fir;                           /* 馬達編碼器 FIR filter object */
mv_avg_t pwma_mv_avg;                    /* PWM phase A Moving Average object */
mv_avg_t pwmb_mv_avg;                    /* PWM phase B Moving Average object */
extern step_caccumulator_t c_accum;      /* 命令微步累加器, 在control/ele_angle.c內初始化 */
extern step_saccumulator_t s_accum;      /* 感測微步累加器, 在control/ele_angle.c內初始化 */

const float fir_para8[] = FIR_PARA_8;
const float fir_para4[] = FIR_PARA_4;


void control_init(void) {
    /* Initialize add adjust calculator */
    init_cangle_inc(&adj_v);
    set_cangle_wback(&adj_v, 1);

    /* Initialize excited angle feedback */
    init_exc_ang_para(&fb_exc_angle, EXC_KI);

    /* Initialize current feedback */
    init_current_para(&fb_current, I_SVPWM_KP, I_SVPWM_KI, I_SVPWM_LOW, I_SVPWM_HIGH);

    /* Initialize step accumulator */
    set_caccum_k(&c_accum, STEP_C_THETA_TO_LENGTH);
    set_saccum_k(&s_accum, STEP_S_THETA_TO_LENGTH);

    /* Initialize moving average object */
    init_mv_avg(&enc_mv_avg, 8);
    init_mv_avg(&pwma_mv_avg, 16);
    init_mv_avg(&pwmb_mv_avg, 16);

    /* Initialize FIR filter object */
    init_fir(&enc_fir, fir_para8, (sizeof(fir_para8)>>2));

    hal_delay(500);

    /* Initialize machanical angle */
    for(int i=0;i<16;i++) {
        as5047d.update();    // update encoder angle to be first machanical angle
    }
    set_saccum_th_init(&s_accum, as5047d.angle);
    init_sangle(&sangle, as5047d.angle);
    init_cangle(&cangle, N_STEP, 0);
}

void control_print(void) {
    static uint8_t prec_cnt = 0;
    /* i1, i2, angle, sangle, cangle, th_svpwm, i_svpwm, th_er, th_cum, pwm1, pwm2 */
    RS485_trm("%d,%d,%.0f,%.3f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,\r\n", drv8847_s.drv->v_r1, drv8847_s.drv->v_r2, roundf(enc_fir.fir_output), sangle.ele_dangle, cangle.ele_dangle,
                                                    fb_exc_angle.th_esvpwm, fb_current.i_svpwm, fb_exc_angle.th_er, fb_exc_angle.th_cum, pwm12.pwm1, pwm12.pwm2);

    if(++prec_cnt == 5) {
        update_cangle(&cangle, get_cangle_inc(&adj_v));
        prec_cnt = 0;
    }
}
// volatile uint16_t temp[4];
// volatile uint8_t ccc = 0;
void control_handle(void) {
    // update_sangle(&sangle, enc_mv_avg.mv_output);
    update_sangle(&sangle, (uint16_t)roundf(enc_fir.fir_output));
    cal_exc_ang_correct(&fb_exc_angle, sangle.ele_dangle, cangle.ele_dangle);  /* about 3.225 us */
    cal_current_correct(&fb_exc_angle, &fb_current); /* about 5.25 us */
    cal_pwmAB(&pwm12, &fb_exc_angle, &fb_current);   /* about 26.32 us */

    /* update duty cycle */
    /* 0 => 1A is high, 1B is low */
    /* 0 => 2B is high, 2A is low */
    SET_PHASEA_DUTY(pwma_mv_avg.mv_output);
    SET_PHASEB_DUTY(pwmb_mv_avg.mv_output);

    // SET_PHASEA_DUTY(pwm12.pwm1);
    // SET_PHASEB_DUTY(pwm12.pwm2);
}
