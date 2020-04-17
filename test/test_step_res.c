/**
 * @file main.c
 * @author Xiang-Guan Deng
 * @brief Test code to collect full step each  step response
 * @date 2020.xx.xx
 *
 * Each step response data is 1024
 * All is 1024*200 = 204800
 * Sample rate is about 350 kHz
 * Compiler: arm-none-eabi-gcc (8.3.1)
 * TODO: overflow problem (0~360)
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
#include "hal_drv8847.h"
#include "tick.h"
#include "rs485.h"

#define ADC_SAMPLE_NUM 2048

extern drv8847_t drv8847;        /* DRV8847 motor drive IC */
extern as50474_t as5047d;        /* AS5047D motor encoder IC */

/* Not used */
extern freq_div_t freq_div_pwmA;
extern freq_div_t freq_div_pwmB;

void adc_trig(void);
void full_step(void);
void stop_step(void);
void handle_response(void);

volatile uint16_t buff_adc[ADC_SAMPLE_NUM];
volatile uint16_t sample_n = 0;
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
    ADC_PHAB->SC3 = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(0);
    ADC_PHAB->CFG2 = ADC_CFG2_ADHSC_MASK;

    drv8847.init();
    if(drv8847.status == I2C_STATUS_TIMEOUT) {
        RS485_trm("DRV8847S Timeout !!! \r\n");
        hal_delay(1000);
    }

    SET_1A_DUTY = 0;
    SET_1A_DUTY = PERIOD_COUNT;
    SET_2B_DUTY = 0;
    SET_2A_DUTY = PERIOD_COUNT;

    hal_delay(100);

    /* Replace the default ADC handler */
    drv8847.drv->handle = handle_response;

    /* Initialize adc buff */
    for(uint16_t i=0;i<ADC_SAMPLE_NUM;i++) buff_adc[i] = 0;

    RS485_trm("start step response %lx, %lx\r\n", ADC_PHAB->CFG1, ADC_PHAB->CFG2);
    hal_delay(100);
    __set_irqn_priority(ADC0_IRQn, 1);
    ENABLE_ADC_PHAB_INT();

    uint16_t angle, cnt = 0;
    ENABLE_TEST1();
    adc_trig();
    full_step();
    while (true) {
        if(flag) {
            DISABLE_TEST1();
            angle = as5047d.instance->read_anglecom();
            for(uint16_t i=0;i<ADC_SAMPLE_NUM;i++) {
                RS485_trm(",%d,%d,%d,%d,\r\n", i, buff_adc[i], angle, tog_fg);
            }
            cnt++;
            if(cnt == 201) {
                RS485_trm("Full step done !!!\r\n");
                while(1);
            }
            sample_n = 0;
            flag = 0;
            tog_fg ^= 1;
            stop_step();
            ENABLE_TEST1();
            ENABLE_ADC_PHAB_INT();
            adc_trig();
            full_step();
        }
        // stop_step();
        // hal_delay(1);
        // full_step();
        // hal_delay(100);
    }


    return 0;
}

void HardFalut_Handler(void) {
    RS485_trm("Error occur\n");
}

void adc_trig(void) {
    if(tog_fg == 0) {
        ADC_PHA->SC1[0] = ADC_CH_PHA;
    }
    else {
        ADC_PHA->SC1[0] = ADC_CH_PHB;
    }
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
    buff_adc[sample_n++] =  ADC_PHA->R[0];
    if(sample_n == ADC_SAMPLE_NUM) {
        DISABLE_ADC_PHAB_INT();
        flag = 1;
        return;
    }
    adc_trig();
}


