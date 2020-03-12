#include "hal_drv8847.h"
#define NOP_DELAY(void)  for(uint32_t i=0;i<STEP_PULSE_COUNT;i++) __asm("nop")

/* Step motor direction flag */
uint8_t drv8847_dir = DIR_CLOCKWISE;

extern drv8847_io_t drv8847_dri;

static void drv8847_init(void);
static void drv8847_clockwise(void);
static void drv8847_counterclockwise(void);
static void drv8847_setMode(uint8_t mode);
static void drv8847_setTorque(uint8_t trq);
static void drv8847_trigger(void);
static void drv8847_update_current(void);

drv8847_t drv8847 = {                              \
    .drv = &drv8847_dri,                           \
    .i1 = 0.0,                                     \
    .i2 = 0.0,                                     \
    .status = DRV8847_STATUS_OK,                   \
    .init = drv8847_init,                          \
    .clockwise = drv8847_clockwise,                \
    .counterclockwise = drv8847_counterclockwise,  \
    .setMode = drv8847_setMode,                    \
    .setTorque = drv8847_setTorque,                \
    .trigger = drv8847_trigger,                    \
    .update_current = drv8847_update_current,      \

};

static void drv8847_setMode(uint8_t mode) {
    switch(mode) {
        case DRV8847_MODE_TWO_PIN:
            drv8847_dri.mode_high();
            break;
        case DRV8847_MODE_FOUR_PIN:
            drv8847_dri.mode_low();
            break;
        case DRV8847_MODE_SLEEP:
            drv8847_dri.sleep_low();
            break;
        case DRV8847_MODE_OPERATION:
            drv8847_dri.sleep_high();
            break;
    }
}

static void drv8847_init(void) {
    drv8847.setMode(DRV8847_MODE_OPERATION);
    drv8847.setMode(DRV8847_MODE_FOUR_PIN);
    drv8847.setTorque(DRV8847_TRQ_FULL);
    drv8847.clockwise();
}

static void drv8847_clockwise(void) {
    if(drv8847_dir == DIR_COUNTERCLOCKWISE) {
        drv8847.drv->clockwise();
        drv8847_dir = DIR_CLOCKWISE;
    }
}

static void drv8847_counterclockwise(void) {
    if(drv8847_dir == DIR_CLOCKWISE) {
        drv8847.drv->counterclockwise();
        drv8847_dir = DIR_COUNTERCLOCKWISE;
    }
}

static void drv8847_setTorque(uint8_t trq) {
    switch(trq) {
        case DRV8847_TRQ_HALF:
            drv8847_dri.trq_high();
            break;
        case DRV8847_TRQ_FULL:
            drv8847_dri.trq_low();
            break;
    }
}

static void drv8847_trigger(void) {
    drv8847.drv->Rsense_trig();
}

static void drv8847_update_current(void) {
    drv8847.i1 = drv8847.drv->v_r1*ADC_REF/ADC_RES/R_SENSE;
    drv8847.i2 = drv8847.drv->v_r2*ADC_REF/ADC_RES/R_SENSE;
}
