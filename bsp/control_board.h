/**
 * @file control_board.h
 * @author Xiang-Guan Deng
 * @brief Hardware setting
 * @date 2020.xx.xx
 *
 */

#ifndef CONTROL_BOARD_H
#define CONTROL_BOARD_H

#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup

/* Define system clock freqency */
#define SYS_CLOCK_FREQ         72000000U

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
/* PWM frequency = 40K, PERIOD_COUNT = SYS_CLOCK_FREQ / 40K */
#define PERIOD_COUNT           SYS_CLOCK_FREQ / 40000 / 2
#define SET_1A1B_PERIOD        FTM_1A1B->MOD
#define SET_1A_DUTY            FTM_1A1B->CONTROLS[CH_1A].CnV
#define SET_1B_DUTY            FTM_1A1B->CONTROLS[CH_1B].CnV
#define SET_PHASEA_DUTY(duty)  SET_1A_DUTY = duty; \
                               SET_1B_DUTY = duty;
#define ENABLE_PHA_INT()       __enable_irqn(FTM1_IRQn);
#define DISABLE_PHA_INT()      __disable_irqn(FTM1_IRQn);

#define SET_2A2B_PERIOD        FTM_2A2B->MOD
#define SET_2B_DUTY            FTM_2A2B->CONTROLS[CH_2B].CnV
#define SET_2A_DUTY            FTM_2A2B->CONTROLS[CH_2A].CnV
#define SET_PHASEB_DUTY(duty)  SET_2B_DUTY = duty; \
                               SET_2A_DUTY = duty;
#define ENABLE_PHB_INT()       __enable_irqn(FTM0_IRQn);
#define DISABLE_PHB_INT()      __disable_irqn(FTM0_IRQn);

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

#define GPIO_PHA               GPIOE
#define PORT_PHA               PORTE
#define PIN_PHA                24

#define GPIO_PHB               GPIOE
#define PORT_PHB               PORTE
#define PIN_PHB                23

/* Use the same ADC hardware setting  */
#define ADC_PHA                ADC0
#define ADC_PHB                ADC0
#define ADC_PHAB               ADC0

#define ADC_CH_PHA             0x11 | ADC_SC1_AIEN_MASK
#define ADC_CH_PHB             0x17 | ADC_SC1_AIEN_MASK

#define DRV8847S_I2C           I2C0

#define GPIO_SDA               GPIOE
#define PORT_SDA               PORTE
#define PIN_SDA                18

#define GPIO_SCL               GPIOE
#define PORT_SCL               PORTE
#define PIN_SCL                19

#define ENABLE_ADC_PHAB_INT()  __enable_irqn(ADC0_IRQn);
#define ADC_PHAB_Handler       ADC0_IRQHandler

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

#define GPIO_RS485_CTR        GPIOD
#define PORT_RS485_CTR        PORTD
#define PIN_RS485_CTR         7

#define GPIO_TX1              GPIOE
#define PORT_TX1              PORTE
#define PIN_TX1               16

#define GPIO_RX1              GPIOE
#define PORT_RX1              PORTE
#define PIN_RX1               17

#define ENABLE_RS485_INT()    __enable_irqn(UART1_RX_TX_IRQn)
#define RS485_INT_HANDLER     UART1_RX_TX_IRQHandler
#define ENABLE_DMA_INT()      __enable_irqn(DMA0_IRQn);

#define SEND_BUFF_SIZE        128

/**
* @brief test pin map
* Test1 : PTD6
* Test2 : PTD4
*/
#define GPIO_TEST1            GPIOD
#define PORT_TEST1            PORTD
#define PIN_TEST1             6
#define ENABLE_TEST1()        GPIO_TEST1->PSOR |= (1<<PIN_TEST1);
#define DISABLE_TEST1()       GPIO_TEST1->PCOR |= (1<<PIN_TEST1);

#define GPIO_TEST2            GPIOD
#define PORT_TEST2            PORTD
#define PIN_TEST2             4
#define ENABLE_TEST2()        GPIO_TEST2->PSOR |= (1<<PIN_TEST2);
#define DISABLE_TEST2()       GPIO_TEST2->PCOR |= (1<<PIN_TEST2);

/**
 * @brief Correction period handle
 * Correction period : 2 ms (500 Hz)
 * Print period : 10 ms (100 Hz)
 */
#define CORRECTION_FREQ       500
#define PERIOD_PRINT_FREQ     100

/* initialize motor control IC */
void init_hw_drv8847_s(void);

/* initialize AS5047D, encoder IC */
void init_hw_as5047d(void);

/* initialize test pin */
void init_test_pin(void);

/* initialize RS485 */
void init_hw_rs485(void);
void init_rs485_txdma(void);
void enable_rs485_txdma(void);
void disable_rs485_txdma(void);

/* initialize PIT */
void init_hw_pit(void);

/* board initialize */
void board_init(void);

#endif /* CONTROL_BOARD_H */
