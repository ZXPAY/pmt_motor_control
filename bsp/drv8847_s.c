#include "hal_drv8847.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "control_board.h"
#include "drv8847_s.h"
#include "hal_tick.h"

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
static uint8_t drv8847_get_fault(void);
static void drv8847_mcu_trig1A1B(void);
static void drv8847_mcu_trig2A2B(void);
static void drv8847_adc_handle(void);

#if DRV8847
static void drv8847_mode_4pin(void);
static void drv8847_mode_2pin(void);
static void drv8847_trq_full(void);
static void drv8847_trq_half(void);
drv8847_io_t drv8847_dri = {                       \
    .status = I2C_STATUS_OK,                       \
    .ch = ADC_CH_PHA,                              \
    .v_r1 = 0,                                     \
    .v_r2 = 0,                                     \
    .sleep_low = drv8847_sleep_low,                \
    .sleep_high = drv8847_sleep_high,              \
    .mode_4pin = drv8847_mode_4pin,                \
    .mode_2pin = drv8847_mode_2pin,                \
    .trq_full = drv8847_trq_full,                  \
    .trq_half = drv8847_trq_half,                  \
    .get_fault = drv8847_get_fault,                \
    .mcu_trig1A1B = drv8847_mcu_trig1A1B,          \
    .mcu_trig2A2B = drv8847_mcu_trig2A2B,          \
    .handle = drv8847_adc_handle,                  \
};

#else if DRV8847S
static void start_tansmission(void);
static void repeat_start_tansmission(void);
static void enter_receive_mode(void);
static void stop_tansmission(void);
static void wait_tansmission(void);
static void send_byte(uint8_t data);
static uint8_t get_byte(void);
static uint8_t is_ack(void);
void i2c_write(uint8_t regAdd, uint8_t trm_data);
uint8_t i2c_read(uint8_t regAdd);

static void drv8847s_mode_4pin(void);
static void drv8847s_mode_2pin(void);
static void drv8847s_trq_full(void);
static void drv8847s_trq_half(void);

drv8847_io_t drv8847_dri = {                       \
    .status = I2C_STATUS_OK,                       \
    .ch = ADC_CH_PHA,                              \
    .v_r1 = 0,                                     \
    .v_r2 = 0,                                     \
    .sleep_low = drv8847_sleep_low,                \
    .sleep_high = drv8847_sleep_high,              \
    .mode_4pin = drv8847s_mode_4pin,               \
    .mode_2pin = drv8847s_mode_2pin,               \
    .trq_full = drv8847s_trq_full,                 \
    .trq_half = drv8847s_trq_half,                 \
    .i2c_write = i2c_write,                        \
    .i2c_read = i2c_read,                          \
    .get_fault = drv8847_get_fault,                \
    .mcu_trig1A1B = drv8847_mcu_trig1A1B,          \
    .mcu_trig2A2B = drv8847_mcu_trig2A2B,          \
    .handle = drv8847_adc_handle,                  \
};
#endif

static void drv8847_sleep_low(void) {
    GPIO_NSLEEP->PCOR |= (1<<PIN_NSLEEP);
}

static void drv8847_sleep_high(void) {
    GPIO_NSLEEP->PSOR |= (1<<PIN_NSLEEP);
}

#if DRV8847
static void drv8847_mode_4pin(void) {
    GPIO_MODE->PCOR |= (1<<PIN_MODE);
}

static void drv8847_mode_2pin(void) {
    GPIO_MODE->PSOR |= (1<<PIN_MODE);
}

static void drv8847_trq_full(void) {
    GPIO_TRQ->PCOR |= (1<<PIN_TRQ);
}

static void drv8847_trq_half(void) {
    GPIO_TRQ->PSOR |= (1<<PIN_TRQ);
}

#else if DRV8847S
static void start_tansmission(void) {
    DRV8847S_I2C->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK | I2C_C1_IICEN_MASK;
}

static void repeat_start_tansmission(void) {
    DRV8847S_I2C->C1 |= I2C_C1_RSTA_MASK | I2C_C1_TX_MASK;
}

static void enter_receive_mode(void) {
    DRV8847S_I2C->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
    DRV8847S_I2C->D; // dummy read
    DRV8847S_I2C->C1 |= I2C_C1_TXAK_MASK;
    wait_tansmission();
}

static void stop_tansmission(void) {
    DRV8847S_I2C->C1 = 0;
}

static void wait_tansmission(void) {
    timeout_set_ms(DRV8847S_TIMEOUT_MS);
    while((!(DRV8847S_I2C->S & I2C_S_IICIF_MASK)) && (!is_timeout()));
    if(is_timeout()) drv8847_dri.status = I2C_STATUS_TIMEOUT;
    /* Clear flag */
    DRV8847S_I2C->S |= I2C_S_IICIF_MASK;
}

static void send_byte(uint8_t data) {
    DRV8847S_I2C->D = data;
    wait_tansmission();
}

static uint8_t get_byte(void) {
    timeout_set_ms(DRV8847S_TIMEOUT_MS);
    while((!(DRV8847S_I2C->S & I2C_S_SRW_MASK)) && (!is_timeout()));
    if(is_timeout()) drv8847_dri.status = I2C_STATUS_TIMEOUT;
    /* Clear flag */
    DRV8847S_I2C->S |= I2C_S_SRW_MASK;
    return DRV8847S_I2C->D;
}

static uint8_t is_ack(void) {
    return !((DRV8847S_I2C->S & I2C_S_RXAK_MASK) >> I2C_S_RXAK_MASK);
}

void i2c_write(uint8_t regAdd, uint8_t trm_data) {
    /* Start */
    start_tansmission();
    send_byte(((DRV8847S_ADDRESS << 1U) & 0xFE));
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return;}

    /* point to hardware register address */
    send_byte(regAdd);
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return;}

    /* write data to hardware register address */
    send_byte(trm_data);
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return;}

    stop_tansmission();
}

uint8_t i2c_read(uint8_t regAdd) {
    /* Start */
    start_tansmission();
    send_byte(((DRV8847S_ADDRESS << 1U) & 0xFE));
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return 0;}

    /* point to hardware register address */
    send_byte(regAdd);
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return 0;}

    /* repeat start to read hardware register */
    repeat_start_tansmission();
    send_byte(((DRV8847S_ADDRESS << 1U) | 0x01));
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return 0;}

    enter_receive_mode();
    uint8_t rec_data = get_byte();
    stop_tansmission();

    return rec_data;
}

static void drv8847s_mode_4pin(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_MODE_MASK);
    temp |= IC1_MODE_4PIN;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}

static void drv8847s_mode_2pin(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_MODE_MASK);
    temp |= IC1_MODE_2PIN;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}

static void drv8847s_trq_full(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_TRQ_MASK);
    temp |= IC1_TRQ_FULL;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}

static void drv8847s_trq_half(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_TRQ_MASK);
    temp |= IC1_TRQ_HALF;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}
#endif

static uint8_t drv8847_get_fault(void) {
    return (GPIO_NFAULT->PDIR & (1<<PIN_NFAULT)) >> PIN_NFAULT;
}

static void drv8847_mcu_trig1A1B(void) {
    ENABLE_TEST1();
    ADC_PHA->SC1[0] = ADC_CH_PHA;
    drv8847_dri.ch = ADC_CH_PHA;
}

static void drv8847_mcu_trig2A2B(void) {
    ADC_PHB->SC1[0] =  ADC_CH_PHB;
    drv8847_dri.ch = ADC_CH_PHB;
}

static void drv8847_adc_handle(void) {
    switch(drv8847_dri.ch) {
        case ADC_CH_PHA:
            drv8847_dri.v_r1 = ADC_PHA->R[0];
            break;
        case ADC_CH_PHB:
            drv8847_dri.v_r2 = ADC_PHB->R[0];
            break;
    }
}
