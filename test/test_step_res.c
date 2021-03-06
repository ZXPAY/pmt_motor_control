/**
 * @file test_step_res.c
 * @author Xiang-Guan Deng
 * @brief Test code to collect full step each  step response
 * @date 2020.xx.xx
 *
 * Each step response data is 1024
 * All is 1024*200 = 204800
 * Sample rate is about 350 kHz
 * Compiler: arm-none-eabi-gcc (8.3.1)
 */

/* Include C standard library */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Include hardware library */
#include "uart.h"
#include "system.h"
#include "cortex_m4.h"
#include "fpu.h"
#include "drv8847_s.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "control_config.h"

/* Include other library */
#include "hal_tick.h"
#include "freqdiv.h"

/* Include board support package */
#include "control_board.h"
#include "hal_as5047d.h"
#include "hal_drv8847_s.h"
#include "tick.h"
#include "rs485.h"

#define ADC_SAMPLE_NUM 1024

// #define ID_PHASE_A
#define ID_PHASE_B

extern drv8847_s_t drv8847_s;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

/* Not used */
extern freq_div_t freq_div_pwmA;
extern freq_div_t freq_div_pwmB;

void adc_trig(void);
void full_step(void);
void stop_step(void);
void handle_response(void);

volatile uint16_t buff_adc[ADC_SAMPLE_NUM];
volatile uint16_t buff_angle[ADC_SAMPLE_NUM];
volatile uint16_t sample_n = 0;
volatile uint16_t sample_ang = 0;
volatile uint8_t flag = 0, tog_fg = 0;

/* Main code */
int main (void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    enable_fpu();

    /* Initilize hardware */
    board_init();
    enable_fpu();
    enable_tick();
    DISABLE_TEST1();

    /* Setting ADC sampling rate */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    PORT_PHA->PCR[PIN_PHA] &= ~PORT_PCR_MUX_MASK;
    PORT_PHB->PCR[PIN_PHB] &= ~PORT_PCR_MUX_MASK;
    ADC_PHAB->CFG1 = ADC_CFG1_MODE(3) | ADC_CFG1_ADIV(0);  // 16 bit
    ADC_PHAB->SC3 = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(1);

    drv8847_s.init();
    if(drv8847_s.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }
    // drv8847_s.setMode(DRV8847_MODE_SLEEP);

    /* enable clock source */
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    /* enable, and do not stop in DEBUG mode */
    PIT->MCR = 0;

    /* 100k ADC */
    PIT->CHANNEL[0].LDVAL = SYS_CLOCK_FREQ / 100000;
    /* enable PIT1 timer and enable interrupt */
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;

    SET_1A_DUTY = 0;
    SET_1A_DUTY = PERIOD_COUNT;
    SET_2B_DUTY = 0;
    SET_2A_DUTY = PERIOD_COUNT;

    hal_delay(100);

    /* Replace the default ADC handler */
    drv8847_s.drv->handle = handle_response;

    /* Initialize adc buff */
    for(uint16_t i=0;i<ADC_SAMPLE_NUM;i++) buff_adc[i] = 0;

    RS485_trm("start step response %lx, %lx\r\n", ADC_PHAB->CFG1, ADC_PHAB->CFG2);
#ifdef ID_PHASE_A
    SET_PHASEA_DUTY(PERIOD_COUNT);
#else
    SET_PHASEB_DUTY(PERIOD_COUNT);
#endif
    hal_delay(1000);
    __set_irqn_priority(ADC0_IRQn, 1);
    ENABLE_ADC_PHAB_INT();
    stop_step();
    hal_delay(1000);
    uint16_t cnt = 0;
    ENABLE_TEST1();
    __enable_irqn(PIT0_IRQn);

#ifdef ID_PHASE_A
    SET_PHASEA_DUTY(PERIOD_COUNT);
#else
    SET_PHASEB_DUTY(PERIOD_COUNT);
#endif
    // full_step();

    while (true) {
        if(flag) {
            stop_step();
            DISABLE_TEST1();
            for(uint16_t i=0;i<ADC_SAMPLE_NUM;i++) {
                RS485_trm("%d,%d,%d,\r\n", i, buff_adc[i], buff_angle[i]);
            }
            cnt++;
            if(cnt == 201) {
                RS485_trm("Full step done !!!\r\n");
                while(1);
            }
            while(1);
            sample_n = 0;
            flag = 0;
            tog_fg ^= 1;
            ENABLE_TEST1();
            ENABLE_ADC_PHAB_INT();
            __enable_irqn(PIT0_IRQn);
            full_step();
        }
        // stop_step();
        // hal_delay(1);
        // full_step();
        // hal_delay(100);
    }


    return 0;
}

void HardFault_Handler(void) {
    RS485_trm("Hardware error occur\n");
    while(1);
}

void adc_trig(void) {
#ifdef ID_PHASE_A
    ADC_PHA->SC1[0] = ADC_CH_PHA;
#else
    ADC_PHA->SC1[0] = ADC_CH_PHB;
#endif
    as5047d.update();
    buff_angle[sample_ang++] = as5047d.angle;
}

void stop_step(void) {
    SET_1A_DUTY = 0;
    SET_1B_DUTY = PERIOD_COUNT;
    SET_2B_DUTY = 0;
    SET_2A_DUTY = PERIOD_COUNT;
}

/* 單線圈激磁 */
void full_step(void) {
    static uint8_t step_fg = 0;
    switch (step_fg){
    case 0:
        /* A */
        SET_PHASEA_DUTY(0);
        step_fg = 1;
        break;
    case 1:
        /* B */
        SET_PHASEB_DUTY(PERIOD_COUNT);
        step_fg = 2;
        break;
    case 2:
        /* A bar */
        SET_PHASEA_DUTY(PERIOD_COUNT);
        step_fg = 3;
        break;
    case 3:
        /* B bar */
        SET_PHASEB_DUTY(0);
        step_fg = 0;
        break;
    default:
        break;
    }
}

void handle_response(void) {
    buff_adc[sample_n++] =  ADC_PHAB->R[0];
    if(sample_n == ADC_SAMPLE_NUM) {
        flag = 1;
        __disable_irqn(PIT0_IRQn);
        DISABLE_ADC_PHAB_INT();
        return;
    }
}

void PIT0_IRQHandler(void) {
    if(!flag) {
        adc_trig();
    }
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
}
