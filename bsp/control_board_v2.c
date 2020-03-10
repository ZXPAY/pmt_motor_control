#include <stdio.h>
#include "control_board_v2.h"
#include "cortex_m4.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup

/**
* @brief DRV8847 pin map
* 1B : PTB1   FTM1_CH1
* 1A : PTB0   FTM1_CH0
* 2A : PTC2   FTM0_CH1
* 2B : PTC1   FTM0_CH0
* MODE : PTE18
* NFAULT : PTD5
* TRQ : PTE19
* NSLEEP : PTC3
* Rsense1 : PTE24
* Rsense2 : ADC0_SE23
*/

void init_hw_drv8847(void) {
    // Enable PORT clock source
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    // MODE and NSLEEP pin as output
    PORT_MODE->PCR[PIN_MODE] |= PORT_PCR_MUX(MUX_ALT_1);
    GPIO_MODE->PDDR |= (1<<PIN_MODE);

    PORT_NSLEEP->PCR[PIN_NSLEEP] |= PORT_PCR_MUX(MUX_ALT_1);
    GPIO_NSLEEP->PDDR |= (1<<PIN_NSLEEP);

    // NFAULT pin and TRQ pin as input
    PORT_NFAULT->PCR[PIN_NFAULT] |= PORT_PCR_MUX(MUX_ALT_1) | PORT_PCR_PE(PCR_PULL_EN) | PORT_PCR_PS(PCR_PULL_UP);
    GPIO_NFAULT->PDDR &= ~(1<<PIN_NFAULT);

    PORT_TRQ->PCR[PIN_TRQ] |= PORT_PCR_MUX(MUX_ALT_1) | PORT_PCR_PE(PCR_PULL_EN) | PORT_PCR_PS(PCR_PULL_UP);
    GPIO_NFAULT->PDDR &= ~(1<<PIN_TRQ);

    // Setting PWM for 1A 1B
    // Enable clock source
    SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
    FTM_1A1B->SC = 0;
    FTM_1A1B->MOD = 0xFFFF;
    FTM_1A1B->CONTROLS[CH_1A].CnSC |=  FTM_CnSC_ELSB_MASK;
    FTM_1A1B->MODE |= FTM_MODE_WPDIS_MASK;
    // FTM1 CH0 and CH1 are complement and enable deadtime
    FTM_1A1B->COMBINE |= FTM_COMBINE_COMP0_MASK;
    FTM_1A1B->CONTROLS[CH_1B].CnSC |=  FTM_CnSC_ELSB_MASK;
    PORT_1A->PCR[PIN_1A] |= PORT_PCR_MUX(MUX_ALT_3);
    GPIO_1A->PDDR |= (1<<PIN_1A);
    PORT_1B->PCR[PIN_1B] |= PORT_PCR_MUX(MUX_ALT_3);
    GPIO_1B->PDDR |= (1<<PIN_1B);

    FTM_1A1B->SC = FTM_SC_CLKS(1) | FTM_SC_CPWMS_MASK | FTM_SC_PS(7) | FTM_SC_TOIE_MASK;
    FTM_1A1B->MOD = 30000;                   // period
    FTM_1A1B->CNTIN = 0;                     // start count
    FTM_1A1B->CONTROLS[CH_1A].CnV = 15000;   // duty

    // Setting PWM for 2A 2B
    // Enable clock source
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
    FTM_2A2B->SC = 0;
    FTM_2A2B->CONTROLS[CH_2B].CnSC |= FTM_CnSC_ELSB_MASK;
    FTM_2A2B->MODE |= FTM_MODE_WPDIS_MASK;
    // FTM0 CH0 and CH1 are complement and enable deadtime
    FTM_2A2B->COMBINE |= FTM_COMBINE_COMP0_MASK;
    FTM_2A2B->CONTROLS[CH_2A].CnSC |= FTM_CnSC_ELSB_MASK;
    PORT_2B->PCR[PIN_2B] |= PORT_PCR_MUX(MUX_ALT_4);
    GPIO_2B->PDDR |= (1<<PIN_2B);
    PORT_2A->PCR[PIN_2A] |= PORT_PCR_MUX(MUX_ALT_4);
    GPIO_2A->PDDR |= (1<<PIN_2A);

    FTM_2A2B->SC = FTM_SC_CLKS(1) | FTM_SC_CPWMS_MASK | FTM_SC_PS(7) | FTM_SC_TOIE_MASK;
    FTM_2A2B->MOD = 45000;     // period + CNTIN
    FTM_2A2B->CNTIN = 15000;   // FTM_1A1B->MOD / 2 (phase shift)
    FTM_2A2B->CONTROLS[CH_2B].CnV = 30000; // duty + CNTIN

    // Setting ADC0 for R sense
    // Enable clock source
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    PORT_R_SENSE1->PCR[PIN_R_SENSE1] &= ~PORT_PCR_MUX_MASK;
    PORT_R_SENSE2->PCR[PIN_R_SENSE2] &= ~PORT_PCR_MUX_MASK;
    ADC_R_SENSE->CFG1 |= ADC_CFG1_MODE(3);   // 16 bit
    ADC_R_SENSE->SC1[0] = ADC_SC1_AIEN_MASK | ADC_CH_R_SENSE1;
    ADC_R_SENSE->SC3 |= ADC_SC3_AVGE_MASK;

    // Enable interrupt
    __enable_irqn(FTM0_IRQn);
    __enable_irqn(FTM1_IRQn);
    __enable_irqn(ADC0_IRQn);
}

void init_hw_drvs8847(void){

}

/**
* @brief AS5047D pin map
* MISO : PTC7
* MOSI : PTC6
* SCK : PTC5
* /CS : PTC4
*/
void init_hw_as5047d(void) {
    // Enable SPI clock source
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;

    PORT_MISO->PCR[PIN_MISO] |= PORT_PCR_MUX(MUX_ALT_2);
    PORT_MOSI->PCR[PIN_MOSI] |= PORT_PCR_MUX(MUX_ALT_2);
    PORT_SCK->PCR[PIN_SCK] |= PORT_PCR_MUX(MUX_ALT_2);
    PORT_CS->PCR[PIN_CS] |= PORT_PCR_MUX(MUX_ALT_2);

    // Disable SPI
    ENCODER_SPI->MCR |= SPI_MCR_HALT_MASK;
    ENCODER_SPI->MCR &= ~SPI_MCR_MDIS_MASK;
    ENCODER_SPI->MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_MTFE_MASK | SPI_MCR_PCSIS_MASK;
    ENCODER_SPI->CTAR[0] = SPI_CTAR_BR(0) | SPI_CTAR_FMSZ(16-1) | SPI_CTAR_PCSSCK(1) | SPI_CTAR_PASC(1) | SPI_CTAR_CPHA_MASK;
    // Enable SPI
    ENCODER_SPI->MCR &= ~SPI_MCR_HALT_MASK;
}

/**
* @brief RS485 pin map
* CTR1 : PTD1
* CTR2 : PTD2
* TX1 : PTE16
* RX1 : PTE17
* TRIG : PTA4
* INTERRUPT : PTD4
*/
void init_hw_rs485(void) {
    // Enable UART and GPIOD GPIOE clock source
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;

    // CTR1 and CTR2 pin as output, default is LOW
    PORT_CTR1->PCR[PIN_CTR1] |= PORT_PCR_MUX(MUX_ALT_1);
    GPIO_CTR1->PDDR |= (1<<PIN_CTR1);
    GPIO_CTR1->PCOR |= (1<<PIN_CTR1);

    PORT_CTR2->PCR[PIN_CTR2] |= PORT_PCR_MUX(MUX_ALT_1);
    GPIO_CTR2->PDDR |= (1<<PIN_CTR2);
    GPIO_CTR2->PCOR |= (1<<PIN_CTR2);

    // TX1 and RX1, and enable RX interrupt
    PORT_TX1->PCR[PIN_TX1] |= PORT_PCR_MUX(MUX_ALT_3);
    PORT_RX1->PCR[PIN_RX1] |= PORT_PCR_MUX(MUX_ALT_3);
    RS485_UART->BDL = UART_BDL_SBR(13);
    RS485_UART->C4 |= UART_C4_BRFA(1);
    // RS485_UART->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK;
    RS485_UART->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;

    __enable_irqn(UART1_RX_TX_IRQn);
}

/*
// Redefine printf to UART1
int fputc(int ch, FILE *f){
    while(!(RS485_UART->S1 & UART_S1_TDRE_MASK));
    RS485_UART->D = (uint8_t)ch;
    return ch;
}

int fgetc(FILE *f){
	while(!(RS485_UART->S1 & UART_S1_RDRF_MASK));
    return RS485_UART->D;
}
*/
