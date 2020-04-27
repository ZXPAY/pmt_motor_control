/* Cortex-M4 System timer */
#define SYST_CSR                 (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR                 (*(volatile uint32_t *)0xE000E014)
#define SYST_CVR                 (*(volatile uint32_t *)0xE000E018)
#define SYST_CALIB               (*(volatile uint32_t *)0xE000E01C)

/* Cortex-M4 NVIC */
#define NVIC_ISER                ( (volatile uint32_t *)0xE000E100)
#define NVIC_ICER                ( (volatile uint32_t *)0xE000E180)
#define NVIC_ISPR                ( (volatile uint32_t *)0xE000E200)
#define NVIC_ICPR                ( (volatile uint32_t *)0xE000E280)
#define NVIC_IABR                ( (volatile uint32_t *)0xE000E300)
#define NVIC_IPR                 ( (volatile uint8_t *)0xE000E400)
#define NVIC_STIR                (*(volatile uint32_t *)0xE000EF00)

/* Cortex-M4 SCB */
#define SCB_CPUID               (*(volatile uint32_t *)0xE000ED00)  /*!< Offset: 0x000 (R/ )  CPUID Base Register                                   */
#define SCB_ICSR                (*(volatile uint32_t *)0xE000ED04)  /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register                  */
#define SCB_VTOR                (*(volatile uint32_t *)0xE000ED08)  /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
#define SCB_AIRCR               (*(volatile uint32_t *)0xE000ED0C)  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register      */
#define SCB_SCR                 (*(volatile uint32_t *)0xE000ED10)  /*!< Offset: 0x010 (R/W)  System Control Register                               */
#define SCB_CCR                 (*(volatile uint32_t *)0xE000ED14)  /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
#define SCB_SHPR                ( (volatile uint8_t *)0xE000ED18)   /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
#define SCB_SHCSR               (*(volatile uint32_t *)0xE000ED28)  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
#define SCB_CFSR                (*(volatile uint32_t *)0xE000ED28)  /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register                    */
#define SCB_HFSR                (*(volatile uint32_t *)0xE000ED2C)  /*!< Offset: 0x02C (R/W)  HardFault Status Register   `                         */
#define SCB_DFSR                (*(volatile uint32_t *)0xE000ED30)  /*!< Offset: 0x030 (R/W)  Debug Fault Status Register                           */
#define SCB_MMFAR               (*(volatile uint32_t *)0xE000ED34)  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register                      */
#define SCB_BFAR                (*(volatile uint32_t *)0xE000ED38)  /*!< Offset: 0x038 (R/W)  BusFault Address Register                             */
#define SCB_AFSR                (*(volatile uint32_t *)0xE000ED3C)  /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register                       */
#define SCB_PFR                 (*(volatile uint32_t *)0xE000ED40)  /*!< Offset: 0x040 (R/ )  Processor Feature Register                            */
#define SCB_DFR                 (*(volatile uint32_t *)0xE000ED48)  /*!< Offset: 0x048 (R/ )  Debug Feature Register                                */
#define SCB_ADR                 (*(volatile uint32_t *)0xE000ED4C)  /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register                            */
#define SCB_MMFR                (*(volatile uint32_t *)0xE000ED50)  /*!< Offset: 0x050 (R/ )  Memory Model Feature Register                         */
#define SCB_ISAR                (*(volatile uint32_t *)0xE000ED60)  /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register                   */
#define SCB_RESEARVED           (*(volatile uint32_t *)0xE000ED74)  /*!< Offset: 0x074 (R/W)  Researved Register                                    */
#define SCB_CPACR               (*(volatile uint32_t *)0xE000ED88)  /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register                   */

/* Cortex-M4 ITM */
#define ITM_BASE                (*(volatile uint32_t *)0xE0000000)               /*!< Offset: 0x0000 (R/W)  Stimulus Port Registers 0-31            */
#define ITM_Port8(n)            (*((volatile uint8_t *) (0xE0000000 + 4 * n)))   /*!< ITM_Port8                                                     */
#define ITM_Port16(n)           (*((volatile uint16_t*) (0xE0000000 + 4 * n)))   /*!< ITM_Port16                                                    */
#define ITM_Port32(n)           (*((volatile uint32_t *) (0xE0000000 + 4 * n)))  /*!< ITM_Port32                                                    */
#define ITM_TER                 (*(volatile uint32_t *)0xE0000E00)               /*!< Offset: 0x0E00 (R/W)  Trace Enable Register                   */
#define ITM_TPR                 (*(volatile uint32_t *)0xE0000E40)               /*!< Offset: 0x0E40 (R/W)  ITM Trace Privilege Register            */
#define ITM_TCR                 (*(volatile uint32_t *)0xE0000E80)               /*!< Offset: 0x0E80 (R/W)  Trace Control Register                  */
#define ITM_PID4                (*(volatile uint32_t *)0xE0000FD0)               /*!< Offset: 0x0FD0 (R/W)  Peripheral Identification registers 4   */
#define ITM_PID5                (*(volatile uint32_t *)0xE0000FD4)               /*!< Offset: 0x0FD4 (R/W)  Peripheral Identification registers 5   */
#define ITM_PID6                (*(volatile uint32_t *)0xE0000FD8)               /*!< Offset: 0x0FD8 (R/W)  Peripheral Identification registers 6   */
#define ITM_PID7                (*(volatile uint32_t *)0xE0000FDC)               /*!< Offset: 0x0FDC (R/W)  Peripheral Identification registers 7   */
#define ITM_PID0                (*(volatile uint32_t *)0xE0000FE0)               /*!< Offset: 0x0FE0 (R/W)  Peripheral Identification registers 0   */
#define ITM_PID1                (*(volatile uint32_t *)0xE0000FE4)               /*!< Offset: 0x0FE4 (R/W)  Peripheral Identification registers 1   */
#define ITM_PID2                (*(volatile uint32_t *)0xE0000FE8)               /*!< Offset: 0x0FE8 (R/W)  Peripheral Identification registers 2   */
#define ITM_PID3                (*(volatile uint32_t *)0xE0000FEC)               /*!< Offset: 0x0FEC (R/W)  Peripheral Identification registers 3   */
#define ITM_CID0                (*(volatile uint32_t *)0xE0000FF0)               /*!< Offset: 0x0F00 (R/W)  Component Identification registers  0   */
#define ITM_CID1                (*(volatile uint32_t *)0xE0000FF4)               /*!< Offset: 0x0FF4 (R/W)  Component Identification registers  1   */
#define ITM_CID2                (*(volatile uint32_t *)0xE0000FF8)               /*!< Offset: 0x0FF8 (R/W)  Component Identification registers  2   */
#define ITM_CID3                (*(volatile uint32_t *)0xE0000FFC)               /*!< Offset: 0x0FFC (R/W)  Component Identification registers  3   */

/*--------------------------------------------------------------*/
/* Cortex-M4 core/peripheral access macros                      */
/*--------------------------------------------------------------*/

/* These are for only privileged mode */
#define __enable_irq() __asm volatile("CPSIE i\n")
#define __disable_irq() __asm volatile("CPSID i\n")
#define __enable_irqn(n) NVIC_ISER[(n) >> 5] = 1 << ((n) & 0x1F)
#define __disable_irqn(n) NVIC_ICER[(n) / 32] = 1 << ((n) % 32)
#define __test_irqn_enabled(n) (NVIC_ISER[(n) / 32] & (1 << ((n) % 32)))
#define __set_irqn(n) NVIC_ISPR[(n) / 32] = 1 << ((n) % 32)
#define __clear_irqn(n) NVIC_ICPR[(n) / 32] = 1 << ((n) % 32)
#define __test_irqn(n) (NVIC_ICPR[(n) / 32] & (1 << ((n) % 32)))
#define __test_irqn_active(n) (NVIC_IABR[n / 32] & (1 << ((n) % 32)))
#define __set_irqn_priority(n, v) NVIC_IPR[n] = (v)
#define __set_faultn_priority(n, v) SCB_SHPR[(n) + 16] = (v)
#define __get_MSP()                                                            \
  ({                                                                           \
    uint32_t __rv;                                                             \
    __asm("MRS %0, MSP\n" : "=r"(__rv));                                       \
    __rv;                                                                      \
  })
#define __get_PSP()                                                            \
  ({                                                                           \
    uint32_t __rv;                                                             \
    __asm("MRS %0, PSP\n" : "=r"(__rv));                                       \
    __rv;                                                                      \
  })
#define __get_PRIMASK()                                                        \
  ({                                                                           \
    uint32_t __rv;                                                             \
    __asm("MRS %0, PRIMASK\n" : "=r"(__rv));                                   \
    __rv;                                                                      \
  })
#define __get_FAULTMASK()                                                      \
  ({                                                                           \
    uint32_t __rv;                                                             \
    __asm("MRS %0, FAULTMASK\n" : "=r"(__rv));                                 \
    __rv;                                                                      \
  })
#define __get_BASEPRI()                                                        \
  ({                                                                           \
    uint32_t __rv;                                                             \
    __asm("MRS %0, BASEPRI\n" : "=r"(__rv));                                   \
    __rv;                                                                      \
  })
#define __get_CONTROL()                                                        \
  ({                                                                           \
    uint32_t __rv;                                                             \
    __asm("MRS %0, CONTROL\n" : "=r"(__rv));                                   \
    __rv;                                                                      \
  })
#define __set_MSP(arg)                                                         \
  {                                                                            \
    uint32_t __v = arg;                                                        \
    __asm("MSR MSP, %0\n" ::"r"(__v));                                         \
  }
#define __set_PSP(arg)                                                         \
  {                                                                            \
    uint32_t __v = arg;                                                        \
    __asm("MSR PSP, %0\n" ::"r"(__v));                                         \
  }
#define __set_PRIMASK(arg)                                                     \
  {                                                                            \
    uint32_t __v = arg;                                                        \
    __asm("MSR PRIMASK, %0\n" ::"r"(__v));                                     \
  }
#define __set_FAULTMASK(arg)                                                   \
  {                                                                            \
    uint32_t __v = arg;                                                        \
    __asm("MSR FAULTMASK, %0\n" ::"r"(__v));                                   \
  }
#define __set_BASEPRI(arg)                                                     \
  {                                                                            \
    uint32_t __v = arg;                                                        \
    __asm("MSR BASEPRI, %0\n" ::"r"(__v));                                     \
  }
#define __set_CONTORL(arg)                                                     \
  {                                                                            \
    uint32_t __v = arg;                                                        \
    __asm("MSR CONTROL, %0\nISB\n" ::"r"(__v));                                \
  }
