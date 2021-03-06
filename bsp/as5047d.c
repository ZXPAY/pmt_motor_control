#include "hal_as5047d.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup
#include "control_board.h"

/* SPI2 pin below
 * MISO  PB14
 * MOSI  PB15
 * SCK   PB13
 * /CS   PB12
*/

static void as5047d_write(uint16_t address, uint16_t value);
static uint16_t as5047d_read(uint16_t address);
static uint16_t as5047d_read_angle(void);
static uint16_t as5047d_read_anglecom(void);
static void as5047d_cs_high(void);
static void as5047d_cs_low(void);
static uint8_t as5047d_get_parity(uint16_t value);

as50474_dri_t as50474_dri = {                 \
    .CHIP_SELECT = 0,                         \
    .write = as5047d_write,                   \
    .read = as5047d_read,                     \
    .read_angle = as5047d_read_angle,   \
    .read_anglecom = as5047d_read_anglecom,   \
    .cs_high = as5047d_cs_high,               \
    .cs_low = as5047d_cs_low,                 \
    .get_parity = as5047d_get_parity,         \
};

static void as5047d_write(uint16_t address, uint16_t value) {
    // address parity should be even
    address &= ~(1<<14);    // bit14 0 is write, 1 is read
    if(as50474_dri.get_parity(address&0x3fff)) address |= 0x8000;

    ENCODER_SPI->PUSHR = address | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;

    if(as50474_dri.get_parity(value&0x3fff)) value |= 0x8000;
    ENCODER_SPI->PUSHR = value | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;
}

static uint16_t as5047d_read(uint16_t address) {
    // address parity should be even
    address |= (1<<14);     // bit14 0 is write, 1 is read
    if(as50474_dri.get_parity(address&0x7fff)) address |= 0x8000;

    ENCODER_SPI->PUSHR = address | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;

    ENCODER_SPI->PUSHR = 0x00 | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;

    uint16_t value = ENCODER_SPI->RXFR1;
    // flush RX FIFO
    ENCODER_SPI->MCR |= SPI_MCR_CLR_RXF_MASK;
    return value & 0x3fff;
}

static uint16_t as5047d_read_angle(void) {
    // address parity should be even
    ENCODER_SPI->PUSHR = AS5047D_ANGLE_READ_ADDRESS | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;

    ENCODER_SPI->PUSHR = 0x00 | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;

    uint16_t value = ENCODER_SPI->RXFR1;
    // flush RX FIFO
    ENCODER_SPI->MCR |= SPI_MCR_CLR_RXF_MASK;
    return value & 0x3fff;
}

static uint16_t as5047d_read_anglecom(void) {
    // address parity should be even
    ENCODER_SPI->PUSHR = AS5047D_ANGLECOM_READ_ADDRESS | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;

    ENCODER_SPI->PUSHR = 0x00 | SPI_CMD;
    while(!(ENCODER_SPI->SR & SPI_SR_TCF_MASK));
    ENCODER_SPI->SR |= SPI_SR_TCF_MASK;

    uint16_t value = ENCODER_SPI->RXFR1;
    // flush RX FIFO
    ENCODER_SPI->MCR |= SPI_MCR_CLR_RXF_MASK;
    return value & 0x3fff;
}

static void as5047d_cs_high(void) {
    switch(as50474_dri.CHIP_SELECT) {
        case CHIP_CS1:

            break;
        case CHIP_CS2:

            break;
        case CHIP_CS3:

            break;
    }
}

static void as5047d_cs_low(void) {
    switch(as50474_dri.CHIP_SELECT) {
        case CHIP_CS1:

            break;
        case CHIP_CS2:

            break;
        case CHIP_CS3:

            break;
    }
}

static uint8_t as5047d_get_parity(uint16_t n) {
    int parity = 0;
    while (n) {
        parity = !parity;
        n = n & (n-1);
    }
    return parity;
}
