/**
 * @file control_board.h
 * @author Xiang-Guan Deng
 * @brief Hardware setting
 * @date 2020.xx.xx
 *
 */

#ifndef CONTROL_BOARD_H
#define CONTROL_BOARD_H

// MUX alternative function
#define MUX_ALT_0              0
#define MUX_ALT_1              1
#define MUX_ALT_2              2
#define MUX_ALT_3              3
#define MUX_ALT_4              4
#define MUX_ALT_5              5
#define MUX_ALT_6              6
#define MUX_ALT_7              7

#define PCR_PULL_EN            1
#define PCR_PULL_DOWN          1
#define PCR_PULL_UP            1

/**
* @brief DRV8847 pin map
* 1B : PTB1   FTM1_CH1
* 1A : PTB0   FTM1_CH0
* 2A : PTC2   FTM0_CH1
* 2B : PTC1   FTM0_CH0
* MODE : PTE18    (SDA)
* NFAULT : PTD5
* TRQ : PTE19     (SCL)
* NSLEEP : PTC3
* Rsense1 : PTE24
* Rsense2 : ADC0_SE23
*/

#define GPIO_1A                GPIOB
#define PORT_1A                PORTB
#define PIN_1A                 0

#define GPIO_1B                GPIOB
#define PORT_1B                PORTB
#define PIN_1B                 1

#define GPIO_2B                GPIOC
#define PORT_2B                PORTC
#define PIN_2B                 1

#define GPIO_2A                GPIOC
#define PORT_2A                PORTC
#define PIN_2A                 2

#define FTM_1A1B               FTM1
#define FTM_2A2B               FTM0
#define CH_1A                  0
#define CH_1B                  1
#define CH_2B                  0
#define CH_2A                  1

/* Define for setting period and duty cycle */
#define PERIOD_COUNT           2000
#define SET_1A1B_PERIOD        FTM_1A1B->MOD
#define SET_1A_DUTY            FTM_1A1B->CONTROLS[CH_1A].CnV
#define SET_1B_DUTY            FTM_1A1B->CONTROLS[CH_1B].CnV

#define SET_2A2B_PERIOD        FTM_2A2B->MOD
#define SET_2B_DUTY            FTM_2A2B->CONTROLS[CH_2B].CnV
#define SET_2A_DUTY            FTM_2A2B->CONTROLS[CH_2A].CnV

#define GPIO_MODE              GPIOE
#define PORT_MODE              PORTE
#define PIN_MODE               18

#define GPIO_NFAULT            GPIOD
#define PORT_NFAULT            PORTD
#define PIN_NFAULT             5

#define GPIO_TRQ               GPIOE
#define PORT_TRQ               PORTE
#define PIN_TRQ                19

#define GPIO_NSLEEP            GPIOC
#define PORT_NSLEEP            PORTC
#define PIN_NSLEEP             3

#define GPIO_R_SENSE1          GPIOE
#define PORT_R_SENSE1          PORTE
#define PIN_R_SENSE1           24

#define GPIO_R_SENSE2          GPIOE
#define PORT_R_SENSE2          PORTE
#define PIN_R_SENSE2           23

#define ADC_R_SENSE            ADC0
#define ADC_CH_R_SENSE1        0x11
#define ADC_CH_R_SENSE2        0x17

#define DRV8847S_I2C           I2C0

#define GPIO_SDA               GPIOE
#define PORT_SDA               PORTE
#define PIN_SDA                18

#define GPIO_SCL               GPIOE
#define PORT_SCL               PORTE
#define PIN_SCL                19

/**
* @brief AS5047D pin map
* MISO : PTC7
* MOSI : PTC6
* SCK : PTC5
* /CS : PTC4
*/
#define ENCODER_SPI            SPI0

#define GPIO_MISO              GPIOC
#define PORT_MISO              PORTC
#define PIN_MISO               7

#define GPIO_MOSI              GPIOC
#define PORT_MOSI              PORTC
#define PIN_MOSI               6

#define GPIO_SCK               GPIOC
#define PORT_SCK               PORTC
#define PIN_SCK                5

#define GPIO_CS                GPIOC
#define PORT_CS                PORTC
#define PIN_CS                 4

/**
* @brief RS485 pin map
* CTR1 : PTD1
* CTR2 : PTD2
* TX1 : PTE16
* RX1 : PTE17
* TRIG : PTA4
* INTERRUPT : PTD4
*/
#define RS485_UART            UART1

#define GPIO_CTR1             GPIOD
#define PORT_CTR1             PORTD
#define PIN_CTR1              7

#define GPIO_CTR2             GPIOD
#define PORT_CTR2             PORTD
#define PIN_CTR2              6

#define GPIO_TX1              GPIOE
#define PORT_TX1              PORTE
#define PIN_TX1               16

#define GPIO_RX1              GPIOE
#define PORT_RX1              PORTE
#define PIN_RX1               17

#define GPIO_TRIG             GPIOA
#define PORT_TRIG             PORTA
#define PIN_TRIG              4

#define GPIO_INT              GPIOD
#define PORT_INT              PORTD
#define PIN_INT               4

/* initialize motor control IC */
void init_hw_drv8847(void);

/* initialize motor control IC */
void init_hw_drv8847s(void);

/* initialize AS5047D, encoder IC */
void init_hw_as5047d(void);

/* initialize RS485 */
void init_hw_rs485(void);

/* initialize PIT */
void init_hw_pit(void);

/* board initialize */
void board_init(void);

#endif /* CONTROL_BOARD_H */
