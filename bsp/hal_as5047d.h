/**
 * @file hal_as5047d.h
 * @author Xiang-Guan Deng
 * @brief AS5047D HAL
 * @date 2020.xx.xx
 *
 * AS5047D IC HAL
 * Encoder resolution : 16 bits
 */

#ifndef HAL_AS5047D_H
#define HAL_AS5047D_H
#include <stdint.h>

#define CHIP_CS1                       1
#define CHIP_CS2                       2
#define CHIP_CS3                       3

#define AS5047D_PROG                   (0x0003UL)
#define AS5047D_CORDICMAG              (0x3FFDUL)
#define AS5047D_DIAAGC                 (0x3FFCUL)
#define AS5047D_ANGLECOM               (0x3FFFUL)
#define AS5047D_ANGLE                  (0x3FFEUL)
#define AS5047D_ERRFL                  (0x0001UL)
#define AS5047D_ZPOSM                  (0x0016UL)
#define AS5047D_ZPOSL                  (0x0017UL)
#define AS5047D_SETTINGS2              (0x0019UL)
#define AS5047D_SETTINGS1              (0x0018UL)
#define AS5047D_ABIBIN                 (0b0000000000100000UL)  // Select Binary
#define AS5047D_ABIRES                 (0b0000000111000000UL)  // select resolution 2048

#define AS5047D_ANGLECOM_READ_ADDRESS  (0xFFFFUL)

typedef struct __as5047d_dri {
    uint8_t CHIP_SELECT;
    void (*write)(uint16_t address, uint16_t value);
    uint16_t (*read)(uint16_t address);
    uint16_t (*read_anglecom)(void);
    void (*cs_high)(void);
    void (*cs_low)(void);
    uint8_t (*get_parity)(uint16_t n);
}as50474_dri_t;

typedef struct __as5047d {
    uint16_t angle;
    as50474_dri_t *instance;
    void (*set_chip)(uint8_t cs_num);
    void (*init)(void);
    void (*update)(void);
}as50474_t;

#endif  /* HAL_AS5047D_H */
