#ifndef HAL_DRV8847_H
#define HAL_DRV8847_H
#include <stdint.h>

/* ADC parameters */
#define R_SENSE 0.15
#define ADC_RES 65535
#define ADC_REF 3.3

/* PWM parameters */

#define DRV8847_STATUS_OK      0
#define DRV8847_STATUS_FAULT   1

#define DRV8847_MODE_TWO_PIN   0
#define DRV8847_MODE_FOUR_PIN  1
#define DRV8847_MODE_SLEEP     2
#define DRV8847_MODE_OPERATION 3

#define DRV8847_TRQ_HALF       4
#define DRV8847_TRQ_FULL       5

/* Step motor direction, default is clockwise */

enum STEP_DIR{
    DIR_CLOCKWISE,
    DIR_COUNTERCLOCKWISE
};

typedef struct _drv8847_io {
    uint8_t ch;
    uint16_t R_sense1;
    uint16_t R_sense2;
    uint16_t period1;
    uint16_t period2;
    uint16_t duty1;
    uint16_t duty2;
    volatile uint8_t flag;
    void (*sleep_low)(void);
    void (*sleep_high)(void);
    void (*mode_low)(void);
    void (*mode_high)(void);
    void (*trq_low)(void);
    void (*trq_high)(void);
    uint8_t (*get_fault)(void);
    void (*clockwise)(void);
    void (*counterclockwise)(void);
    void (*set_period1)(uint16_t period);
    void (*set_period2)(uint16_t period);
    void (*set_duty1)(uint16_t duty);
    void (*set_duty2)(uint16_t duty);
    void (*set_channel)(uint8_t ch);
    void (*Rsense_trig)(void);
    void (*update_Rsenses)(void);
}drv8847_io_t;

typedef struct _a4988 {
    drv8847_io_t *drv;
    volatile float current_Rsense1;
    volatile float current_Rsense2;
    uint8_t status;
    void (*init)(void);
    void (*clockwise)(void);
    void (*counterclockwise)(void);
    void (*setMode)(uint8_t mode);
    void (*setTorque)(uint8_t trq);
    void (*trigger)(void);
    void (*handler)(void);
    void (*update_current)(void);
}drv8847_t;

#endif /* HAL_DRV8847_H */
