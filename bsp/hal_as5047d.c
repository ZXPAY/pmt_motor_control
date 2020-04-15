#include "hal_as5047d.h"

static void as5047d_set_chip(uint8_t cs_num);
static void as5047d_init(void);
static void as5047d_update(void);

extern as50474_dri_t as50474_dri;

as50474_t as5047d = {                   \
    .angle = 0,                         \
    .instance = &as50474_dri,           \
    .set_chip = as5047d_set_chip,       \
    .init = as5047d_init,               \
    .update = as5047d_update,           \
};

static void as5047d_set_chip(uint8_t cs_num) {
    as5047d.instance->CHIP_SELECT = cs_num;
}

static void as5047d_init(void) {
    as50474_dri.write(AS5047D_SETTINGS1, 0x0004);
    as50474_dri.write(AS5047D_SETTINGS2, 0x0000);
    as50474_dri.write(AS5047D_ZPOSM, 0x0000);
    as50474_dri.write(AS5047D_ZPOSL, 0x0000);
}

static void as5047d_update(void) {
    uint16_t temp = as5047d.instance->read(AS5047D_ANGLE);
    if(temp != 0) {
        as5047d.angle = temp;
    }

}
