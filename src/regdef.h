/**
 * @file regdef.h
 * @author LiYu87
 * @date 2020.01.07
 * @brief 定義暫存器相關巨集，方便開發
 *
 * 只定義專案用到部分，如下：
 *   1. system control
 *   2. pinconnect
 *   3. usart
 *
 * 其中，部分巨集為修改 KEIL 內建標頭檔做成，如下：
 *   1. pinconnect 部分為修改 LPC1700_DFP\2.6.0\RTE_Driver\PIN_LPC17xx.h 做成。
 *   2. usart 部分為修改 LPC1700_DFP\2.6.0\RTE_Driver\UART_LPC17xx.h 做成。
 *   3. i2c(twi) 部分為修改 LPC1700_DFP\2.6.0\RTE_Driver\i2c_LPC17xx.h 做成。
 */

#ifndef REGDEF_H
#define REGDEF_H

/*----- system control section ----------------------------------------------*/

/**
 *  LPC_SC->CLKSRCSEL
 */
#define CLKSRCSEL_CLKSRC_POS (0U)
#define CLKSRCSEL_CLKSRC_MSK (3U << CLKSRCSEL_CLKSRC_POS)
#define CLKSRCSEL_CLKSRC_RC (0U << CLKSRCSEL_CLKSRC_POS)
#define CLKSRCSEL_CLKSRC_OSC (1U << CLKSRCSEL_CLKSRC_POS)
#define CLKSRCSEL_CLKSRC_RTC (2U << CLKSRCSEL_CLKSRC_POS)

/**
 * LPC_SC->SCS
 *
 * OSCRANGE must be 0 for oscillator is  1 MHz to 20 MHz
 * OSCRANGE must be 1 for oscillator is 15 MHz to 20 MHz
 */
#define SCS_OSCRANGE_POS (4U)
#define SCS_OSCEN_POS (5U)
#define SCS_OSCSTAT_POS (6U)
#define SCS_OSCRANGE (1U << SCS_OSCRANGE_POS)
#define SCS_OSCEN (1U << SCS_OSCEN_POS)
#define SCS_OSCSTAT (1U << SCS_OSCSTAT_POS)

/**
 * LPC_SC->PLL0CFG
 *
 * MSEL must be (M - 1)
 * NSEL must be (N - 1)
 */
#define PLL0CFG_MSEL_POS (0U)
#define PLL0CFG_NSEL_POS (16U)
#define PLL0CFG_MSEL_MSK (0x7FFFU << PLL0CFG_MSEL_POS)
#define PLL0CFG_NSEL_MSK (0xFFU << PLL0CFG_NSEL_POS)

/**
 * LPC_SC->PLL0CON
 *
 * PLLE0 is PLL0 Enable
 * PLLC0 is PLL0 Connect
 */
#define PLL0CON_PLLE0_POS (0U)
#define PLL0CON_PLLC0_POS (1U)
#define PLL0CON_ENABLE (1U << PLL0CON_PLLE0_POS)
#define PLL0CON_CONNECT (1U << PLL0CON_PLLC0_POS)

/**
 * LPC_SC->PLL0STAT
 */
#define PLL0STAT_MSEL_POS (0U)
#define PLL0STAT_NSEL_POS (16U)
#define PLL0STAT_ENABLE_POS (24U)
#define PLL0STAT_CONNECT_POS (25U)
#define PLL0STAT_LOCK_POS (25U)
#define PLL0STAT_ENABLE (1U << PLL0STAT_ENABLE_POS)
#define PLL0STAT_CONNECT (1U << PLL0STAT_CONNECT_POS)
#define PLL0STAT_LOCK (1U << PLL0STAT_LOCK_POS)

/**
 * LPC_SC->PCLKSEL0
 * LPC_SC->PCLKSEL1
 *
 * for PCLKSEL_DEVIDER_8:
 *  PCLK_peripheral = CCLK/8
 *  except for CAN1, CAN2, and CAN filtering = CCLK/6
 */
#define PCLKSEL_DEVIDER_POS (0U)
#define PCLKSEL_DEVIDER_4 (0U << PCLKSEL_DEVIDER_POS)
#define PCLKSEL_DEVIDER_1 (1U << PCLKSEL_DEVIDER_POS)
#define PCLKSEL_DEVIDER_2 (2U << PCLKSEL_DEVIDER_POS)
#define PCLKSEL_DEVIDER_8 (3U << PCLKSEL_DEVIDER_POS)

/**
 * LPC_SC->PCONP
 */
#define PCONP_TIM0_POS (1U)
#define PCONP_TIM1_POS (2U)
#define PCONP_UART0_POS (3U)
#define PCONP_UART1_POS (4U)
#define PCONP_PWM1_POS (6U)
#define PCONP_I2C0_POS (7U)
#define PCONP_SPI_POS (8U)
#define PCONP_RTC_POS (9U)
#define PCONP_SSP1_POS (10U)
#define PCONP_ADC_POS (12U)
#define PCONP_CAN1_POS (13U)
#define PCONP_CAN2_POS (14U)
#define PCONP_GPIO_POS (15U)
#define PCONP_RIT_POS (16U)
#define PCONP_MCPWM_POS (17U)
#define PCONP_QEI_POS (18U)
#define PCONP_I2C1_POS (19U)
#define PCONP_SSP0_POS (21U)
#define PCONP_TIM2_POS (22U)
#define PCONP_TIM3_POS (23U)
#define PCONP_UART2_POS (24U)
#define PCONP_UART3_POS (25U)
#define PCONP_I2C2_POS (26U)
#define PCONP_I2S_POS (27U)
#define PCONP_GPDMA_POS (29U)
#define PCONP_ENET_POS (30U)
#define PCONP_USB_POS (31U)

#define PCONP_TIM0 (1U << PCONP_TIM0_POS)
#define PCONP_TIM1 (1U << PCONP_TIM1_POS)
#define PCONP_UART0 (1U << PCONP_UART0_POS)
#define PCONP_UART1 (1U << PCONP_UART1_POS)
#define PCONP_PWM1 (1U << PCONP_PWM1_POS)
#define PCONP_I2C0 (1U << PCONP_I2C0_POS)
#define PCONP_SPI (1U << PCONP_SPI_POS)
#define PCONP_RTC (1U << PCONP_RTC_POS)
#define PCONP_SSP1 (1U << PCONP_SSP1_POS)
#define PCONP_ADC (1U << PCONP_ADC_POS)
#define PCONP_CAN1 (1U << PCONP_CAN1_POS)
#define PCONP_CAN2 (1U << PCONP_CAN2_POS)
#define PCONP_GPIO (1U << PCONP_GPIO_POS)
#define PCONP_RIT (1U << PCONP_RIT_POS)
#define PCONP_MCPWM (1U << PCONP_MCPWM_POS)
#define PCONP_QEI (1U << PCONP_QEI_POS)
#define PCONP_I2C1 (1U << PCONP_I2C1_POS)
#define PCONP_SSP0 (1U << PCONP_SSP0_POS)
#define PCONP_TIM2 (1U << PCONP_TIM2_POS)
#define PCONP_TIM3 (1U << PCONP_TIM3_POS)
#define PCONP_UART2 (1U << PCONP_UART2_POS)
#define PCONP_UART3 (1U << PCONP_UART3_POS)
#define PCONP_I2C2 (1U << PCONP_I2C2_POS)
#define PCONP_I2S (1U << PCONP_I2S_POS)
#define PCONP_GPDMA (1U << PCONP_GPDMA_POS)
#define PCONP_ENET (1U << PCONP_ENET_POS)
#define PCONP_USB (1U << PCONP_USB_POS)

/*----- pinconnect section --------------------------------------------------*/

// Pin Function selection
#define PIN_FUNC_0 ((uint32_t)(0))
#define PIN_FUNC_1 ((uint32_t)(1))
#define PIN_FUNC_2 ((uint32_t)(2))
#define PIN_FUNC_3 ((uint32_t)(3))

// Pin mode
#define PIN_PINMODE_PULLUP ((uint32_t)(0))
#define PIN_PINMODE_REPEATER ((uint32_t)(1))
#define PIN_PINMODE_TRISTATE ((uint32_t)(2))
#define PIN_PINMODE_PULLDOWN ((uint32_t)(3))

// Pin mode (normal/open drain)
#define PIN_PINMODE_NORMAL ((uint32_t)(0))
#define PIN_PINMODE_OPENDRAIN ((uint32_t)(1))

// I2C mode
#define PIN_I2C_Normal_Mode ((uint32_t)(0))
#define PIN_I2C_Fast_Mode ((uint32_t)(1))
#define PIN_I2C_Fast_Plus_Mode ((uint32_t)(2))

/*----- usart section -------------------------------------------------------*/

// USART register interface definitions
// USART Divisor Latch register LSB
#define USART_DLL_DLLSB_POS (0U)
#define USART_DLL_DLLSB_MSK (0xFFU << USART_DLL_DLLSB_POS)

// USART Divisor Latch register MSB
#define USART_DLM_DLMSB_POS (0U)
#define USART_DLM_DLMSB_MSK (0xFFU << USART_DLM_DLMSB_POS)

// USART Interrupt enable register
#define USART_IER_RBRIE (1U << 0)
#define USART_IER_THREIE (1U << 1)
#define USART_IER_RXIE (1U << 2)
#define UART_IER_MSIE \
    (1U << 3)  // Only for UART1 - modem status interrupt enable
#define USART_IER_ABEOINTEN (1U << 8)
#define USART_IER_ABTOINTEN (1U << 9)

// USART Interrupt identification register
#define USART_IIR_INTSTATUS (1U << 0)
#define USART_IIR_INTID_POS (1U)
#define USART_IIR_INTID_MSK (7U << USART_IIR_INTID_POS)
#define USART_IIR_FIFOENABLE_POS (6U)
#define USART_IIR_FIFOENABLE_MSK (3U << USART_IIR_FIFOENABLE_POS)
#define USART_IIR_ABEOINT (1U << 8)
#define USART_IIR_ABTOINT (1U << 9)

#define USART_IIR_INTID_RLS (3U << USART_IIR_INTID_POS)
#define USART_IIR_INTID_RDA (2U << USART_IIR_INTID_POS)
#define USART_IIR_INTID_CTI (6U << USART_IIR_INTID_POS)
#define USART_IIR_INTID_THRE (1U << USART_IIR_INTID_POS)
#define UART_IIR_INTID_MS (0U << USART_IIR_INTID_POS)  // UART1 only

// USART FIFO control register
#define USART_FCR_FIFOEN (1U << 0)
#define USART_FCR_RXFIFORES (1U << 1)
#define USART_FCR_TXFIFORES (1U << 2)
#define USART_FCR_DMAMODE (1U << 3)
#define USART_FCR_RXTRIGLVL_POS (6U)
#define USART_FCR_RXTRIGLVL_MSK (3U << USART_FCR_RXTRIGLVL_POS)

// USART Line control register
#define USART_LCR_WLS_POS (0U)
#define USART_LCR_WLS_MSK (3U << USART_LCR_WLS_POS)
#define USART_LCR_SBS (1U << 2)
#define USART_LCR_PE (1U << 3)
#define USART_LCR_PS_POS (4U)
#define USART_LCR_PS_MSK (3U << USART_LCR_PS_POS)
#define USART_LCR_BC (1U << 6)
#define USART_LCR_DLAB (1U << 7)

// USART Line status register
#define USART_LSR_RDR (1U << 0)
#define USART_LSR_OE (1U << 1)
#define USART_LSR_PE (1U << 2)
#define USART_LSR_FE (1U << 3)
#define USART_LSR_BI (1U << 4)
#define USART_LSR_THRE (1U << 5)
#define USART_LSR_TEMT (1U << 6)
#define USART_LSR_RXFE (1U << 7)
#define USART_LSR_TXERR (1U << 8)

#define USART_LSR_LINE_INT \
    (USART_LSR_OE | USART_LSR_PE | USART_LSR_FE | USART_LSR_BI)

// USART IrDA control register
#define USART_ICR_IRDAEN (1U << 0)
#define USART_ICR_FIXPULSEEN (1U << 1)
#define USART_ICR_IRDAINV (1U << 2)
#define USART_ICR_PULSEDIV_POS (3U)
#define USART_ICR_PULSEDIV_MSK (7U << USART_ICR_PULSEDIV_POS)

// USART Fractional divider register
#define USART_FDR_DIVADDVAL_POS (0U)
#define USART_FDR_DIVADDVAL_MSK (0x0FU << USART_FDR_DIVADDVAL_POS)
#define USART_FDR_MULVAL_POS (4U)
#define USART_FDR_MULVAL_MSK (0x0FU << USART_FDR_MULVAL_POS)

// USART oversampling register
#define USART_OSR_OSFRAC_POS (1U)
#define USART_OSR_OSFRAC_MSK (7U << USART_OSR_OSFRAC_POS)
#define USART_OSR_OSINT_POS (4U)
#define USART_OSR_OSINT_MSK (0x0FU << USART_OSR_OSINT_POS)
#define USART_OSR_FDINT_POS (8U)
#define USART_OSR_FDINT_MSK (0x7FU << USART_OSR_FDINT_MSK)

// USART Half duplex enable register
#define USART_HDEN_HDEN (1U << 0U)

// USART SmartCard interface control register
#define USART_SCICTRL_SCIEN (1U << 0)
#define USART_SCICTRL_NACKDIS (1U << 1)
#define USART_SCICTRL_PROTSEL (1U << 2)
#define USART_SCICTRL_TXRETRY_POS (5U)
#define USART_SCICTRL_TXRETRY_MSK (7U << USART_SCICTRL_TXRETRY_POS)
#define USART_SCICTRL_GUARDTIME_POS (8U)
#define USART_SCICTRL_GUARDTIME_MSK (0xFFU << USART_SCICTRL_GUARDTIME_POS)

// USART Synchronous mode control register
#define USART_SYNCCTRL_SYNC (1U << 0)
#define USART_SYNCCTRL_CSRC (1U << 1)
#define USART_SYNCCTRL_FES (1U << 2)
#define USART_SYNCCTRL_TSBYPASS (1U << 3)
#define USART_SYNCCTRL_CSCEN (1U << 4)
#define USART_SYNCCTRL_SSSDIS (1U << 5)
#define USART_SYNCCTRL_CCCLR (1U << 6)

// UART Modem control register
#define UART_MCR_DTRCTRL (1U << 0)
#define UART_MCR_RTSCTRL (1U << 1)
#define UART_MCR_LMS (1U << 4)
#define UART_MCR_RTSEN (1U << 6)
#define UART_MCR_CTSEN (1U << 7)

// UART Modem status register
#define UART_MSR_DCTS (1U << 0)
#define UART_MSR_DDSR (1U << 1)
#define UART_MSR_TERI (1U << 2)
#define UART_MSR_DDCD (1U << 3)
#define UART_MSR_CTS (1U << 4)
#define UART_MSR_DSR (1U << 5)
#define UART_MSR_RI (1U << 6)
#define UART_MSR_DCD (1U << 7)

// USART RS485 control register
#define USART_RS485CTRL_NMMEN (1U << 0)
#define USART_RS485CTRL_RXDIS (1U << 1)
#define USART_RS485CTRL_AADEN (1U << 2)
#define USART_RS485CTRL_DCTRL (1U << 4)
#define USART_RS485CTRL_OINV (1U << 5)

// USART Transmitter enable register
#define USART_TER_TXEN (1U << 0)

// USART flags
#define USART_FLAG_INITIALIZED (1U << 0)
#define USART_FLAG_POWERED (1U << 1)
#define USART_FLAG_CONFIGURED (1U << 2)
#define USART_FLAG_TX_ENABLED (1U << 3)
#define USART_FLAG_RX_ENABLED (1U << 4)

// USART synchronous xfer modes
#define USART_SYNC_MODE_TX (1U)
#define USART_SYNC_MODE_RX (2U)
#define USART_SYNC_MODE_TX_RX (USART_SYNC_MODE_TX | USART_SYNC_MODE_RX)

#define FRACT_BITS (12U)
#define FRACT_MASK (0XFFFU)

#define FIXED_OVERSAMPLING_DIVIDER_LIMIT (51U << FRACT_BITS)
#define INTEGER_OVERSAMPLING_DIVIDER_LIMIT \
    ((12U << FRACT_BITS) + (8 << FRACT_BITS) / 10)

// Baudrate accepted error
#define USART_MAX_BAUDRATE_ERROR (3U)
#define USART_MAX_DIVIDER_ERROR (3U)

// USART TX FIFO trigger level
#define USART_TRIG_LVL_1 (0x00U)
#define USART_TRIG_LVL_4 (0x40U)
#define USART_TRIG_LVL_8 (0x80U)
#define USART_TRIG_LVL_14 (0xC0U)

#define FRACT_DIV(add, mul)                                                 \
    {                                                                       \
        ((uint16_t)((1U << 12) + (((uint32_t)(add << 24) / (mul)) >> 12))), \
            ((uint8_t)(((mul) << 4) | add)),                                \
    }

/* I2C Driver state flags */
#define I2C_FLAG_INIT       (1U << 0)       // Driver initialized
#define I2C_FLAG_POWER      (1U << 1)       // Driver power on
#define I2C_FLAG_SETUP      (1U << 2)       // Master configured, clock set
#define I2C_FLAG_SLAVE_RX   (1U << 3)       // Slave receive registered

/* I2C Common Control flags */
#define I2C_CON_AA          (1U << 2)       // Assert acknowledge bit
#define I2C_CON_SI          (1U << 3)       // I2C interrupt bit
#define I2C_CON_STO         (1U << 4)       // STOP bit
#define I2C_CON_STA         (1U << 5)       // START bit
#define I2C_CON_I2EN        (1U << 6)       // I2C interface enable
#define I2C_CON_FLAGS       (I2C_CON_AA | I2C_CON_SI | I2C_CON_STO | I2C_CON_STA)

/* I2C Stalled Status flags */
#define I2C_MASTER          (1U << 0)       // Master stalled
#define I2C_SLAVE_TX        (1U << 1)       // Slave stalled on transmit
#define I2C_SLAVE_RX        (1U << 2)       // Slave stalled on receive
#define I2C_SLAVE           (I2C_SLAVE_TX | I2C_SLAVE_RX)

/* I2C Status Miscellaneous states */
#define I2C_STAT_BUSERR      0x00U          // I2C Bus error

/* I2C Status Master mode */
#define I2C_STAT_MA_START    0x08U          // START transmitted
#define I2C_STAT_MA_RSTART   0x10U          // Repeated START transmitted
#define I2C_STAT_MA_SLAW_A   0x18U          // SLA+W transmitted, ACK received
#define I2C_STAT_MA_SLAW_NA  0x20U          // SLA+W transmitted, no ACK recvd
#define I2C_STAT_MA_DT_A     0x28U          // Data transmitted, ACK received
#define I2C_STAT_MA_DT_NA    0x30U          // Data transmitted, no ACK recvd
#define I2C_STAT_MA_ALOST    0x38U          // Arbitration lost SLA+W or data
#define I2C_STAT_MA_SLAR_A   0x40U          // SLA+R transmitted, ACK received
#define I2C_STAT_MA_SLAR_NA  0x48U          // SLA+R transmitted, no ACK recvd
#define I2C_STAT_MA_DR_A     0x50U          // Data received, ACK returned
#define I2C_STAT_MA_DR_NA    0x58U          // Data received, no ACK returned

/* I2C Status Slave mode */
#define I2C_STAT_SL_SLAW_A   0x60U          // SLA+W received, ACK returned
#define I2C_STAT_SL_ALOST_MW 0x68U          // Arbitration lost SLA+W in Master mode
#define I2C_STAT_SL_GCA_A    0x70U          // General address recvd, ACK returned
#define I2C_STAT_SL_ALOST_GC 0x78U          // Arbitration lost in General call
#define I2C_STAT_SL_DR_A     0x80U          // Data received, ACK returned
#define I2C_STAT_SL_DR_NA    0x88U          // Data received, no ACK returned
#define I2C_STAT_SL_DRGC_A   0x90U          // Data recvd General call, ACK returned
#define I2C_STAT_SL_DRGC_NA  0x98U          // Data recvd General call, no ACK returned
#define I2C_STAT_SL_STOP     0xA0U          // STOP received while addressed
#define I2C_STAT_SL_SLAR_A   0xA8U          // SLA+R received, ACK returned
#define I2C_STAT_SL_ALOST_MR 0xB0U          // Arbitration lost SLA+R in Master mode
#define I2C_STAT_SL_DT_A     0xB8U          // Data transmitted, ACK received
#define I2C_STAT_SL_DT_NA    0xC0U          // Data transmitted, no ACK received
#define I2C_STAT_SL_LDT_A    0xC8U          // Last data transmitted, ACK received

#endif  // REFDEF_H


