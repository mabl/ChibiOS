#ifndef _BCM2835_H_
#define _BCM2835_H_

#include "chtypes.h"

/* Note: Also see https://github.com/dwelch67/raspberrypi for more information about interfacing to
   BCM2825 peripherals.*/

#define REG(x) (*(volatile uint32_t *)(x))
#define BIT(n) (1 << (n))

#define BCM2835_CLOCK_FREQ 250000000

// *****************************************************************************
//                        General Purpose I/O (GPIO)
// *****************************************************************************

// -------- GPIO Registers --------
#define GPSET0  	REG(0x2020001C)
#define GPSET1  	REG(0x20200020)
#define GPCLR0  	REG(0x20200028)
#define GPCLR1  	REG(0x2020002C)
#define GPLEV0		REG(0x20200034)
#define GPLEV1		REG(0x20200038)

#define GPFSEL0 	REG(0x20200000)
#define GPFSEL1 	REG(0x20200004)
#define GPFSEL2 	REG(0x20200008)
#define GPFSEL3 	REG(0x2020000C)
#define GPFSEL4 	REG(0x20200010)
#define GPFSEL6 	REG(0x20200014)

#define GPPUD           REG(0x20200094)
#define GPPUDCLK0       REG(0x20200098)
#define GPPUDCLK1       REG(0x2020009C)

#define GPIO_PUD_OFF      0x00
#define GPIO_PUD_TRISTATE 0x00
#define GPIO_PUD_PULLDOWN 0x01
#define GPIO_PUD_PULLUP   0x02

// -------- GPIO Functions --------
#define GPFN_IN	  	0x00
#define GPFN_OUT   	0x01
#define GPFN_ALT0  	0x04
#define GPFN_ALT1  	0x05
#define GPFN_ALT2  	0x06
#define GPFN_ALT3  	0x07
#define GPFN_ALT4  	0x03
#define GPFN_ALT5  	0x02

#define GPIO0_PAD       0
#define GPIO1_PAD       1
#define GPIO2_PAD       2
#define GPIO3_PAD       3
#define GPIO4_PAD       4
#define GPIO5_PAD       5
#define GPIO6_PAD       6
#define GPIO7_PAD       7
#define GPIO8_PAD       8
#define GPIO9_PAD       9
#define GPIO10_PAD      10
#define GPIO11_PAD      11
#define GPIO12_PAD      12
#define GPIO13_PAD      13
#define GPIO14_PAD      14
#define GPIO15_PAD      15
#define GPIO16_PAD      16
#define GPIO17_PAD      17
#define GPIO18_PAD      18
#define GPIO19_PAD      19
#define GPIO20_PAD      20
#define GPIO21_PAD      21
#define GPIO22_PAD      22
#define GPIO23_PAD      23
#define GPIO24_PAD      24
#define GPIO25_PAD      25
#define GPIO26_PAD      26
#define GPIO27_PAD      27
#define GPIO28_PAD      28
#define GPIO29_PAD      29
#define GPIO30_PAD      30
#define GPIO31_PAD      31
#define GPIO32_PAD      0
#define GPIO33_PAD      1
#define GPIO34_PAD      2
#define GPIO35_PAD      3
#define GPIO36_PAD      4
#define GPIO37_PAD      5
#define GPIO38_PAD      6
#define GPIO39_PAD      7
#define GPIO40_PAD      8
#define GPIO41_PAD      9
#define GPIO42_PAD      10
#define GPIO43_PAD      11
#define GPIO44_PAD      12
#define GPIO45_PAD      13
#define GPIO46_PAD      14
#define GPIO47_PAD      15
#define GPIO48_PAD      16
#define GPIO49_PAD      17
#define GPIO50_PAD      18
#define GPIO51_PAD      19
#define GPIO52_PAD      20
#define GPIO53_PAD      21

#define GPIO0_PORT      &IOPORT0
#define GPIO1_PORT      &IOPORT0
#define GPIO2_PORT      &IOPORT0
#define GPIO3_PORT      &IOPORT0
#define GPIO4_PORT      &IOPORT0
#define GPIO5_PORT      &IOPORT0
#define GPIO6_PORT      &IOPORT0
#define GPIO7_PORT      &IOPORT0
#define GPIO8_PORT      &IOPORT0
#define GPIO9_PORT      &IOPORT0
#define GPIO10_PORT     &IOPORT0
#define GPIO11_PORT     &IOPORT0
#define GPIO12_PORT     &IOPORT0
#define GPIO13_PORT     &IOPORT0
#define GPIO14_PORT     &IOPORT0
#define GPIO15_PORT     &IOPORT0
#define GPIO16_PORT     &IOPORT0
#define GPIO17_PORT     &IOPORT0
#define GPIO18_PORT     &IOPORT0
#define GPIO19_PORT     &IOPORT0
#define GPIO20_PORT     &IOPORT0
#define GPIO21_PORT     &IOPORT0
#define GPIO22_PORT     &IOPORT0
#define GPIO23_PORT     &IOPORT0
#define GPIO24_PORT     &IOPORT0
#define GPIO25_PORT     &IOPORT0
#define GPIO26_PORT     &IOPORT0
#define GPIO27_PORT     &IOPORT0
#define GPIO28_PORT     &IOPORT0
#define GPIO29_PORT     &IOPORT0
#define GPIO30_PORT     &IOPORT0
#define GPIO31_PORT     &IOPORT0
#define GPIO32_PORT     &IOPORT1
#define GPIO33_PORT     &IOPORT1
#define GPIO34_PORT     &IOPORT1
#define GPIO35_PORT     &IOPORT1
#define GPIO36_PORT     &IOPORT1
#define GPIO37_PORT     &IOPORT1
#define GPIO38_PORT     &IOPORT1
#define GPIO39_PORT     &IOPORT1
#define GPIO40_PORT     &IOPORT1
#define GPIO41_PORT     &IOPORT1
#define GPIO42_PORT     &IOPORT1
#define GPIO43_PORT     &IOPORT1
#define GPIO44_PORT     &IOPORT1
#define GPIO45_PORT     &IOPORT1
#define GPIO46_PORT     &IOPORT1
#define GPIO47_PORT     &IOPORT1
#define GPIO48_PORT     &IOPORT1
#define GPIO49_PORT     &IOPORT1
#define GPIO50_PORT     &IOPORT1
#define GPIO51_PORT     &IOPORT1
#define GPIO52_PORT     &IOPORT1
#define GPIO53_PORT     &IOPORT1

// ----- GPIO - Onboard LED -------
#define ONBOARD_LED_PORT        GPIO16_PORT
#define ONBOARD_LED_PAD         GPIO16_PAD

// *****************************************************************************
//                          Timer (ARM Side)
// *****************************************************************************

#define ARM_TIMER_LOD REG(0x2000B400)
#define ARM_TIMER_VAL REG(0x2000B404)
#define ARM_TIMER_CTL REG(0x2000B408)
#define ARM_TIMER_CLI REG(0x2000B40C)
#define ARM_TIMER_RIS REG(0x2000B410)
#define ARM_TIMER_MIS REG(0x2000B414)
#define ARM_TIMER_RLD REG(0x2000B418)
#define ARM_TIMER_DIV REG(0x2000B41C)
#define ARM_TIMER_CNT REG(0x2000B420)

// *****************************************************************************
//                        System Timer
// *****************************************************************************

#define SYSTIMER_CS         REG(0x20003000)
#define SYSTIMER_CLO        REG(0x20003004)
#define SYSTIMER_CHI        REG(0x20003008)

#define SYSTIMER_CMP0       REG(0x2000300C)
#define SYSTIMER_CMP1       REG(0x20003010)
#define SYSTIMER_CMP2       REG(0x20003014)
#define SYSTIMER_CMP3       REG(0x20003018)

#define SYSTIMER_CS_MATCH0  0x01
#define SYSTIMER_CS_MATCH1  0x02
#define SYSTIMER_CS_MATCH2  0x04
#define SYSTIMER_CS_MATCH3  0x08

#define SYSTIMER_IRQEN0     0x01
#define SYSTIMER_IRQEN1     0x02
#define SYSTIMER_IRQEN2     0x04
#define SYSTIMER_IRQEN3     0x08

#define SYSTIMER_CLOCK_FREQ 1000000

// *****************************************************************************
//                         AUX Registers
// *****************************************************************************

#define AUX_ENABLES     REG(0x20215004)

// --- Mini UART Registers -----
#define AUX_MU_IO_REG   REG(0x20215040)
#define AUX_MU_IER_REG  REG(0x20215044)
#define AUX_MU_IIR_REG  REG(0x20215048)
#define AUX_MU_LCR_REG  REG(0x2021504C)
#define AUX_MU_MCR_REG  REG(0x20215050)
#define AUX_MU_LSR_REG  REG(0x20215054)
#define AUX_MU_MSR_REG  REG(0x20215058)
#define AUX_MU_SCRATCH  REG(0x2021505C)
#define AUX_MU_CNTL_REG REG(0x20215060)
#define AUX_MU_STAT_REG REG(0x20215064)
#define AUX_MU_BAUD_REG REG(0x20215068)

#define AUX_MU_IER_TX_IRQEN  BIT(1)

#define AUX_MU_IIR_RX_IRQ     ((AUX_MU_IIR_REG & 0x07) == 0x04)
#define AUX_MU_IIR_TX_IRQ     ((AUX_MU_IIR_REG & 0x07) == 0x02)

#define AUX_MU_LSR_RX_RDY     (AUX_MU_LSR_REG & BIT(0))
#define AUX_MU_LSR_TX_RDY     (AUX_MU_LSR_REG & BIT(5))

// *****************************************************************************
//                        Interrupts
// *****************************************************************************

#define IRQ_BASIC         REG(0x2000B200)
#define IRQ_PEND1         REG(0x2000B204)
#define IRQ_PEND2         REG(0x2000B208)
#define IRQ_FIQ_CONTROL   REG(0x2000B210)
#define IRQ_ENABLE1       REG(0x2000B210)
#define IRQ_ENABLE2       REG(0x2000B214)
#define IRQ_ENABLE_BASIC  REG(0x2000B218)
#define IRQ_DISABLE1      REG(0x2000B21C)
#define IRQ_DISABLE2      REG(0x2000B220)
#define IRQ_DISABLE_BASIC REG(0x2000B220)

#define SPI_IRQ           BIT(22)

// *****************************************************************************
//                 Broadcom Serial Controllers (BSC/I2C)
// *****************************************************************************

struct bscdevice_t {
  volatile unsigned int control;
  volatile unsigned int status; 
  volatile unsigned int dataLength;
  volatile unsigned int slaveAddress;
  volatile unsigned int dataFifo;
  volatile unsigned int clockDivider;
  volatile unsigned int dataDelay;
  volatile unsigned int clockStretchTimeout;
};

typedef struct bscdevice_t bscdevice_t;

/* Only BSC0 is accessible from the RPi pi header.*/
#define BSC0_ADDR ((bscdevice_t *)0x20205000)
#define BSC1_ADDR ((bscdevice_t *)0x20804000)
#define BSC2_ADDR ((bscdevice_t *)0x20805000)

#define BSC_CLOCK_FREQ 150000000

/* I2C control flags */
#define BSC_I2CEN BIT(15)
#define BSC_INTR  BIT(10)
#define BSC_INTT  BIT(9)
#define BSC_INTD  BIT(8)
#define BSC_ST    BIT(7)
#define BSC_CLEAR BIT(4)
#define BSC_READ  BIT(0)

/* I2C status flags */
#define BSC_TA   BIT(0) /** @brief Transfer active.*/
#define BSC_DONE BIT(1) /** @brief Transfer done.*/
#define BSC_TXW  BIT(2) /** @brief FIFO needs writing.*/
#define BSC_RXR  BIT(3) /** @brief FIFO needs reading.*/
#define BSC_TXD  BIT(4) /** @brief FIFO can accept data.*/
#define BSC_RXD  BIT(5) /** @brief FIFO contains data.*/
#define BSC_TXE  BIT(6) /** @brief FIFO empty.*/
#define BSC_RXF  BIT(7) /** @brief FIFO full.*/
#define BSC_ERR  BIT(8) /** @brief ACK error.*/
#define BSC_CLKT BIT(9) /** @brief Clock stretch timeout.*/

/* Rising/Falling Edge Delay Defaults.*/
#define BSC_DEFAULT_FEDL       0x30
#define BSC_DEFAULT_REDL       0x30

/* Clock Stretch Timeout Defaults.*/
#define BSC_DEFAULT_CLKT       0x40

#define CLEAR_STATUS  BSC_CLKT|BSC_ERR|BSC_DONE

#define START_READ    BSC_I2CEN|BSC_ST|BSC_CLEAR|BSC_READ
#define START_WRITE   BSC_I2CEN|BSC_ST

// *****************************************************************************
//                  Serial Peripheral Interface (SPI)
// *****************************************************************************

/// See 10.5 SPI Register Map
#define SPI0_CS        REG(0x20204000) /* @brief SPI Master Control and Status.*/
#define SPI0_FIFO      REG(0x20204004) /* @brief SPI Master TX and RX FIFOs.*/
#define SPI0_CLK       REG(0x20204008) /* @brief SPI Master Clock Divider.*/
#define SPI0_DLEN      REG(0x2020400C) /* @brief SPI Master Data Length.*/
#define SPI0_LTOH      REG(0x20204010) /* @brief SPI LOSSI mode TOH.*/
#define SPI0_DC        REG(0x20204014) /* @brief SPI DMA DREQ Controls.*/

// Register masks for SPI0_CS
#define SPI_CS_LEN_LONG             0x02000000 /* @brief Enable Long data word in Lossi mode if DMA_LEN is set.*/
#define SPI_CS_DMA_LEN              0x01000000 /* @brief Enable DMA mode in Lossi mode.*/
#define SPI_CS_CSPOL2               0x00800000 /* @brief Chip Select 2 Polarity.*/
#define SPI_CS_CSPOL1               0x00400000 /* @brief Chip Select 1 Polarity.*/
#define SPI_CS_CSPOL0               0x00200000 /* @brief Chip Select 0 Polarity.*/
#define SPI_CS_RXF                  0x00100000 /* @brief RXF - RX FIFO Full.*/
#define SPI_CS_RXR                  0x00080000 /* @brief RXR RX FIFO needs Reading ( full).*/
#define SPI_CS_TXD                  0x00040000 /* @brief TXD TX FIFO can accept Data.*/
#define SPI_CS_RXD                  0x00020000 /* @brief RXD RX FIFO contains Data.*/
#define SPI_CS_DONE                 0x00010000 /* @brief Done transfer Done.*/
#define SPI_CS_TE_EN                0x00008000 /* @brief Unused.*/
#define SPI_CS_LMONO                0x00004000 /* @brief Unused.*/
#define SPI_CS_LEN                  0x00002000 /* @brief LEN LoSSI enable.*/
#define SPI_CS_REN                  0x00001000 /* @brief REN Read Enable.*/
#define SPI_CS_ADCS                 0x00000800 /* @brief ADCS Automatically Deassert Chip Select.*/
#define SPI_CS_INTR                 0x00000400 /* @brief INTR Interrupt on RXR.*/
#define SPI_CS_INTD                 0x00000200 /* @brief INTD Interrupt on Done.*/
#define SPI_CS_DMAEN                0x00000100 /* @brief DMAEN DMA Enable.*/
#define SPI_CS_TA                   0x00000080 /* @brief Transfer Active.*/
#define SPI_CS_CSPOL                0x00000040 /* @brief Chip Select Polarity.*/
#define SPI_CS_CLEAR                0x00000030 /* @brief Clear FIFO Clear RX and TX.*/
#define SPI_CS_CLEAR_RX             0x00000020 /* @brief Clear FIFO Clear RX .*/
#define SPI_CS_CLEAR_TX             0x00000010 /* @brief Clear FIFO Clear TX .*/
#define SPI_CS_CPOL                 0x00000008 /* @brief Clock Polarity.*/
#define SPI_CS_CPHA                 0x00000004 /* @brief Clock Phase.*/
#define SPI_CS_CS                   0x00000003 /* @brief Chip Select.*/

// *****************************************************************************
//                  Pulse Width Modulation (PWM)
// *****************************************************************************

#define PWM_CTL        REG(0x2020C000)
#define PWM_STATUS     REG(0x2020C004)

#define PWM0_RANGE     REG(0x2020C010)
#define PWM0_DATA      REG(0x2020C014)

#define PWM1_RANGE     REG(0x2020C020)
#define PWM1_DATA      REG(0x2020C024)

#define PWM0_ENABLE    BIT(0)
#define PWM0_MODE_MS   BIT(7)

#define PWM1_ENABLE    BIT(8)
#define PWM1_MODE_MS   BIT(15)

#define PWM_MODE_MS    0xFF

#define GPIO_CLK_PWD   0x5a000000

#define GPIO0_CLK_CTL  REG(0x201010A0)
#define GPIO0_CLK_DIV  REG(0x201010A4)

// *****************************************************************************
//       Power Management, Reset controller and Watchdog registers 
// *****************************************************************************

#define PM_BASE                  (0x20100000) 
#define PM_RSTC                  REG(PM_BASE+0x1c)
#define PM_WDOG                  REG(PM_BASE+0x24)

#define PM_WDOG_RESET            0000000000
#define PM_PASSWORD              0x5a000000
#define PM_WDOG_TIME_SET         0x000fffff
#define PM_RSTC_WRCFG_CLR        0xffffffcf
#define PM_RSTC_WRCFG_SET        0x00000030
#define PM_RSTC_WRCFG_FULL_RESET 0x00000020
#define PM_RSTC_RESET            0x00000102

// *****************************************************************************
//                 Support Functions
// *****************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

  void bcm2835_gpio_fnsel(uint32_t gpio_pin, uint32_t gpio_function);
  void bcm2835_delay(uint32_t n);

#ifdef __cplusplus
  }
#endif

#endif
