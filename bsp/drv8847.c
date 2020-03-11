#include "hal_drv8847.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "control_board_v2.h"

#define CONVERSION_READY   1
#define CONVERSION_BUZY    0

extern uint8_t drv8847_dir;

/**
* @brief DRV8847 pin map
* 1A : PTC2
* 1B : PTC1
* 2B : PTB1
* 2A : PTB0
* MODE : PTE18
* NFAULT : PTD5
* TRQ : PTE19
* NSLEEP : PTC3
* Rsense1 : PTE24
* Rsense2 : ADC0_SE23
*/
static void drv8847_sleep_low(void);
static void drv8847_sleep_high(void);
static void drv8847_mode_low(void);
static void drv8847_mode_high(void);
static void drv8847_trq_low(void);
static void drv8847_trq_high(void);
static uint8_t drv8847_get_fault(void);
static void drv8847_clockwise(void);
static void drv8847_counterclockwise(void);
static void drv8847_set_period1(uint16_t period);
static void drv8847_set_period2(uint16_t period);
static void drv8847_set_duty1(uint16_t duty);
static void drv8847_set_duty2(uint16_t duty);
static void drv8847_Rsense_trig(void);
static void drv8847_update_Rsenses(void);

drv8847_io_t drv8847_dri = {                       \
    .ch = ADC_CH_R_SENSE1,                         \
    .R_sense1 = 0,                                 \
    .R_sense2 = 0,                                 \
    .flag = CONVERSION_READY,                      \
    .sleep_low = drv8847_sleep_low,                \
    .sleep_high = drv8847_sleep_high,              \
    .mode_low = drv8847_mode_low,                  \
    .mode_high = drv8847_mode_high,                \
    .trq_low = drv8847_trq_low,                    \
    .trq_high = drv8847_trq_high,                  \
    .get_fault = drv8847_get_fault,                \
    .clockwise = drv8847_clockwise,                \
    .counterclockwise = drv8847_counterclockwise,  \
    .set_period1 = drv8847_set_period1,            \
    .set_period2 = drv8847_set_period2,            \
    .set_duty1 = drv8847_set_duty1,                \
    .set_duty2 = drv8847_set_duty2,                \
    .Rsense_trig = drv8847_Rsense_trig,            \
    .update_Rsenses = drv8847_update_Rsenses,      \
};

static void drv8847_sleep_low(void) {
    GPIO_NSLEEP->PCOR |= (1<<PIN_NSLEEP);
}

static void drv8847_sleep_high(void) {
    GPIO_NSLEEP->PSOR |= (1<<PIN_NSLEEP);
}

static void drv8847_mode_low(void) {
    GPIO_MODE->PCOR |= (1<<PIN_MODE);
}

static void drv8847_mode_high(void) {
    GPIO_MODE->PSOR |= (1<<PIN_MODE);
}

static void drv8847_trq_low(void) {
    GPIO_TRQ->PCOR |= (1<<PIN_TRQ);
}

static void drv8847_trq_high(void) {
    GPIO_TRQ->PSOR |= (1<<PIN_TRQ);
}

static uint8_t drv8847_get_fault(void) {
    return (GPIO_NFAULT->PDIR & (1<<PIN_NFAULT)) >> PIN_NFAULT;
}

static void drv8847_clockwise(void) {
    FTM_1A1B->CONTROLS[CH_1A].CnSC &=  ~FTM_CnSC_ELSA_MASK;
}

static void drv8847_counterclockwise(void) {
    FTM_1A1B->CONTROLS[CH_1A].CnSC |=  FTM_CnSC_ELSA_MASK;
}

static void drv8847_set_period1(uint16_t period) {
    drv8847_dri.period1 = period;
    FTM_1A1B->MOD = period;
}

static void drv8847_set_period2(uint16_t period) {
    drv8847_dri.period2 = period;
    // FTM_2A2B->CNTIN = drv8847_dri.period1 >> 1;   // FTM_1A1B->MOD / 2
    // FTM_2A2B->MOD = period + (drv8847_dri.period1 >> 1);
}

static void drv8847_set_duty1(uint16_t duty) {
    drv8847_dri.duty1 = duty;
}

static void drv8847_set_duty2(uint16_t duty) {
    drv8847_dri.duty2 = duty;
    // if(duty == 0 || duty == drv8847_dri.period2) {
    //     FTM_2A2B->CNTIN = 0;
    //     FTM_2A2B->MOD = drv8847_dri.period2;
    //     drv8847_dri.duty2 = duty;
    // }
    // else {
    //     drv8847_set_period2(drv8847_dri.period2);
    //     drv8847_dri.duty2 = duty + (drv8847_dri.period1 >> 1);
    // }
}

static void drv8847_Rsense_trig(void) {
    if(drv8847_dri.flag == CONVERSION_READY) {
        switch(drv8847_dri.ch) {
            case ADC_CH_R_SENSE1:
                drv8847_dri.ch = ADC_CH_R_SENSE2;
                break;
            case ADC_CH_R_SENSE2:
                drv8847_dri.ch = ADC_CH_R_SENSE1;
                break;
        }
        ADC_R_SENSE->SC1[0] = ADC_SC1_AIEN_MASK | drv8847_dri.ch;
        drv8847_dri.flag = CONVERSION_BUZY;
    }
}

static void drv8847_update_Rsenses(void) {
    switch(drv8847_dri.ch) {
        case ADC_CH_R_SENSE1:
            drv8847_dri.R_sense1 = ADC_R_SENSE->R[0];
            drv8847_dri.flag = CONVERSION_READY;
            break;
        case ADC_CH_R_SENSE2:
            drv8847_dri.R_sense2 = ADC_R_SENSE->R[0];
            drv8847_dri.flag = CONVERSION_READY;
            break;
    }
}
