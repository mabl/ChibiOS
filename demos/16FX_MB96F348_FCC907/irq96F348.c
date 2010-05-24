/* THIS SAMPLE CODE IS PROVIDED AS IS AND IS SUBJECT TO ALTERATIONS. FUJITSU */
/* MICROELECTRONICS ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR */
/* ELIGIBILITY FOR ANY PURPOSES.                                             */
/*                 (C) Fujitsu Microelectronics Europe GmbH                  */
/*---------------------------------------------------------------------------
  VECTORS.C
  - Interrupt level (priority) setting
  - Interrupt vector definition
-----------------------------------------------------------------------------*/

#include "global.h"
#include "appboard.h"

#include <ch.h>
#include <hal.h>

#include "irq96f348.h"


extern void sw_reset(void);
extern __interrupt void start(void);

int ErrorLevel;

/*---------------------------------------------------------------------------
   InitIrqLevels()
   This function  pre-sets all interrupt control registers. It can be used
   to set all interrupt priorities in static applications. If this file
   contains assignments to dedicated resources, verify  that the
   appropriate controller is used.
   NOTE: value 7 disables the interrupt and value 0 sets highest priority.
-----------------------------------------------------------------------------*/

#define MIN_ICR  11
#define MAX_ICR  93

#define DEFAULT_ILM_MASK 7

void InitIrqLevels(void)
{
volatile int irq;
  
    for (irq = MIN_ICR; irq <= MAX_ICR; irq++) 
    {
        ICR = (irq << 8) | DEFAULT_ILM_MASK;
    }
}

/*---------------------------------------------------------------------------
   Prototypes
   Add your own prototypes here. Each vector definition needs is proto-
   type. Either do it here or include a header file containing them.
-----------------------------------------------------------------------------*/

__interrupt void DefaultIRQHandler (void);

/*---------------------------------------------------------------------------
   Vector definiton for MB9635x
   Use following statements to define vectors. All resource related
   vectors are predefined. Remaining software interrupts can be added here
   as well.
   NOTE: If software interrupts 0 to 7 are defined here, this might 
   conflict with the reset vector in the start-up file.
-----------------------------------------------------------------------------*/

#pragma intvect irq_nmi					11   /* Non-maskable Interrupt       */
#pragma intvect irq_delayed_int			12   /* Delayed Interrupt            */
#pragma intvect irq_rc_timer 			13   /* RC Timer                     */
#pragma intvect irq_mainclk_timer		14   /* Main Clock Timer             */
#pragma intvect irq_subclk_timer		15   /* Sub Clock Timer              */
#pragma intvect DefaultIRQHandler 		16   /* Reserved                     */
#pragma intvect irq_ext0				17   /* EXT0                         */
#pragma intvect irq_ext1				18   /* EXT1                         */
#pragma intvect irq_ext2				19   /* EXT2                         */
#pragma intvect irq_ext3				20   /* EXT3                         */
#pragma intvect irq_ext4				21   /* EXT4                         */
#pragma intvect irq_ext5				22   /* EXT5                         */
#pragma intvect irq_ext6				23   /* EXT6                         */
#pragma intvect irq_ext7				24   /* EXT7                         */
#pragma intvect irq_ext8				25   /* EXT8                         */
#pragma intvect irq_ext9				26   /* EXT9                         */
#pragma intvect irq_ext10				27   /* EXT10                        */
#pragma intvect irq_ext11				28   /* EXT11                        */
#pragma intvect irq_ext12				29   /* EXT12                        */
#pragma intvect irq_ext13				30   /* EXT13                        */
#pragma intvect irq_ext14				31   /* EXT14                        */
#pragma intvect irq_ext15				32   /* EXT15                        */
#pragma intvect irq_can1_tx_rx			33   /* CAN1                         */
#pragma intvect irq_can2_tx_rx			34   /* CAN2                         */
#pragma intvect irq_ppg0				35   /* PPG0                         */
#pragma intvect irq_ppg1				36   /* PPG1                         */
#pragma intvect irq_ppg2				37   /* PPG2                         */
#pragma intvect irq_ppg3				38   /* PPG3                         */
#pragma intvect irq_ppg4				39   /* PPG4                         */
#pragma intvect irq_ppg5				40   /* PPG5                         */
#pragma intvect irq_ppg6				41   /* PPG6                         */
#pragma intvect irq_ppg7				42   /* PPG7                         */
#pragma intvect irq_ppg8				43   /* PPG8                         */
#pragma intvect irq_ppg9				44   /* PPG9                         */
#pragma intvect irq_ppg10				45   /* PPG10                        */
#pragma intvect irq_ppg11				46   /* PPG11                        */
#pragma intvect irq_ppg12				47   /* PPG12                        */
#pragma intvect irq_ppg13				48   /* PPG13                        */
#pragma intvect irq_ppg14				49   /* PPG14                        */
#pragma intvect irq_ppg15				50   /* PPG15                        */
#pragma intvect irq_reload0				51   /* RLT0                         */
#pragma intvect irq_reload1				52   /* RLT1                         */
#pragma intvect irq_reload2				53   /* RLT2                         */
#pragma intvect irq_reload3				54   /* RLT3                         */
#pragma intvect irq_reload6_ppgrlt		55   /* PPGRLT - RLT6                */
#pragma intvect irq_icu0 				56   /* ICU0                         */
#pragma intvect irq_icu1				57   /* ICU1                         */
#pragma intvect irq_icu2		 		58   /* ICU2                         */
#pragma intvect irq_icu3		 		59   /* ICU3                         */
#pragma intvect irq_icu4		 		60   /* ICU4                         */
#pragma intvect irq_icu5		 		61   /* ICU5                         */
#pragma intvect irq_icu6		 		62   /* ICU6                         */
#pragma intvect irq_icu7		 		63   /* ICU7                         */
#pragma intvect irq_ocu0 				64   /* OCU0                         */
#pragma intvect irq_ocu1 				65   /* OCU1                         */
#pragma intvect irq_ocu2 				66   /* OCU2                         */
#pragma intvect irq_ocu3 				67   /* OCU3                         */
#pragma intvect irq_ocu4 				68   /* OCU4                         */
#pragma intvect irq_ocu5 				69   /* OCU5                         */
#pragma intvect irq_ocu6 				70   /* OCU6                         */
#pragma intvect irq_ocu7 				71   /* OCU7                         */
#pragma intvect irq_frt0				72   /* FRT0                         */
#pragma intvect irq_frt1				73   /* FRT1                         */
#pragma intvect irq_i2c0				74   /* I2C0                         */
#pragma intvect irq_i2c1				75   /* I2C1  						 */
#pragma intvect irq_adc					76   /* ADC                          */
#pragma intvect DefaultIRQHandler 		77   /* ALARM0                       */
#pragma intvect DefaultIRQHandler 		78   /* ALARM1                       */
#pragma intvect irq_uart0_rx			79   /* LIN-UART 2 RX                */
#pragma intvect irq_uart0_tx			80   /* LIN-UART 2 TX                */
#pragma intvect irq_uart1_rx			81   /* LIN-UART 3 RX                */
#pragma intvect irq_uart1_tx			82   /* LIN-UART 3 TX                */
#pragma intvect irq_uart2_rx			83   /* LIN-UART 2 RX                */
#pragma intvect irq_uart2_tx			84   /* LIN-UART 2 TX                */
#pragma intvect irq_uart3_rx			85   /* LIN-UART 3 RX                */
#pragma intvect irq_uart3_tx			86   /* LIN-UART 3 TX                */
#pragma intvect irq_flash		 		87   /* MAIN FLASH IRQ               */
#pragma intvect DefaultIRQHandler 		88   /* SATELLITE FLASH IRQ (not on all devices) */
#pragma intvect irq_uart7_rx			89   /* LIN-UART 7 RX                */
#pragma intvect irq_uart7_tx			90   /* LIN-UART 7 TX                */
#pragma intvect irq_uart8_rx			91   /* LIN-UART 8 RX                */
#pragma intvect irq_uart8_tx			92   /* LIN-UART 8 TX                */
#pragma intvect irq_uart9_rx			93   /* LIN-UART 8 RX                */
#pragma intvect irq_uart9_tx			94   /* LIN-UART 8 TX                */
#pragma intvect irq_rtc0		 		95   /* RTC (not on all devices)     */
#pragma intvect irq_cal0				96   /* CAL (not on all devices)     */



/*---------------------------------------------------------------------------
   DefaultIRQHandler()
   This function is a placeholder for all vector definitions. Either use
   your own placeholder or add necessary code here. 
-----------------------------------------------------------------------------*/

__interrupt 
void DefaultIRQHandler (void)
{
    __DI();                              /* disable interrupts */
    while(1)
    {
        __wait_nop();                    /* halt system */
    }
}

void sw_reset(void)
{
    while(1)
    {
        __wait_nop();                    /* halt system */
    }
}

__interrupt void  irq_nmi				(void)
{
	ErrorLevel = 11;   /* Non-maskable Interrupt       */
	sw_reset();
}


__interrupt void  irq_delayed_int		(void)
{
	ErrorLevel = 12;   // Delayed Interrupt
	sw_reset();
}


__interrupt void  irq_rc_timer 			(void)
{
	ErrorLevel = 13;   /* RC Timer                     */
	sw_reset();
}

__interrupt void  irq_mainclk_timer		(void)
{
	ErrorLevel = 14;   /* Main Clock Timer             */
	sw_reset();
}

__interrupt void  irq_subclk_timer		(void)
{
	ErrorLevel = 15;   /* Sub Clock Timer              */

	sw_reset();
}

__interrupt void  irq_ext0				(void)
{
	ErrorLevel = 17;   /* EXT0                         */
	sw_reset();
}

__interrupt void  irq_ext1				(void)
{
	ErrorLevel = 18;   /* EXT1                         */
	sw_reset();
}

__interrupt void  irq_ext2				(void)
{
	ErrorLevel = 19;   /* EXT2                         */
	sw_reset();
}

__interrupt void  irq_ext3				(void)
{
	ErrorLevel = 20;   /* EXT3                         */
	sw_reset();
}

__interrupt void  irq_ext4				(void)
{
	ErrorLevel = 21;   /* EXT4                         */
	sw_reset();
}

__interrupt void  irq_ext5				(void)
{
	ErrorLevel = 22;   /* EXT5                         */
	sw_reset();
}

__interrupt void  irq_ext6				(void)
{
	ErrorLevel = 22;   /* EXT5                         */
	sw_reset();
}

__interrupt void  irq_ext7				(void)
{
	ErrorLevel = 23;   /* EXT7                         */
	sw_reset();
}

__interrupt void  irq_ext8				(void)
{
	ErrorLevel = 24;   /* EXT8                         */
	sw_reset();
}

__interrupt void  irq_ext9				(void)
{
	ErrorLevel = 25;   /* EXT9                         */
	sw_reset();
}

__interrupt void  irq_ext10				(void)
{
	ErrorLevel = 26;   /* EXT10                        */
	sw_reset();
}

__interrupt void  irq_ext11				(void)
{
	ErrorLevel = 27;   /* EXT11                        */
	sw_reset();
}

__interrupt void  irq_ext12				(void)
{
	ErrorLevel = 28;   /* EXT12                        */
	sw_reset();
}

__interrupt void  irq_ext13				(void)
{
	ErrorLevel = 29;   /* EXT13                        */
	sw_reset();
}

__interrupt void  irq_ext14				(void)
{
	ErrorLevel = 30;   /* EXT14                        */
	sw_reset();
}

__interrupt void  irq_ext15				(void)
{
	ErrorLevel = 31;   /* EXT15                        */
	sw_reset();
}

__interrupt void  irq_can1_tx_rx		(void)
{
	ErrorLevel = 32;   /* CAN1                         */
	sw_reset();
}

__interrupt void  irq_can2_tx_rx		(void)
{
	ErrorLevel = 33;   /* CAN2                         */
	sw_reset();
}

__interrupt void  irq_ppg0				(void)
{
	ErrorLevel = 34;   /* PPG0                         */
	sw_reset();
}

__interrupt void  irq_ppg1				(void)
{
	ErrorLevel = 35;   /* PPG1                         */
	sw_reset();
}

__interrupt void  irq_ppg2				(void)
{
	ErrorLevel = 36;   /* PPG2                         */
	sw_reset();
}

__interrupt void  irq_ppg3				(void)
{
	ErrorLevel = 37;   /* PPG3                         */
	sw_reset();
}

__interrupt void  irq_ppg4				(void)
{
	ErrorLevel = 38;   /* PPG4                         */
	sw_reset();
}

__interrupt void  irq_ppg5				(void)
{
	ErrorLevel = 39;   /* PPG5                         */
	sw_reset();
}

__interrupt void  irq_ppg6				(void)
{
	ErrorLevel = 40;   /* PPG6                         */
	sw_reset();
}

__interrupt void  irq_ppg7				(void)
{
	ErrorLevel = 41;   /* PPG7                         */
	sw_reset();
}

__interrupt void  irq_ppg8				(void)
{
	ErrorLevel = 42;   /* PPG8                         */
	sw_reset();
}

__interrupt void  irq_ppg9				(void)
{
	ErrorLevel = 43;   /* PPG9                         */
	sw_reset();
}

__interrupt void  irq_ppg10				(void)
{
	ErrorLevel = 44;   /* PPG10                        */
	sw_reset();
}

__interrupt void  irq_ppg11				(void)
{
	ErrorLevel = 45;   /* PPG11                        */
	sw_reset();
}

__interrupt void  irq_ppg12				(void)
{
	ErrorLevel = 46;   /* PPG12                        */
	sw_reset();
}

__interrupt void  irq_ppg13				(void)
{
	ErrorLevel = 47;   /* PPG13                        */
	sw_reset();
}

__interrupt void  irq_ppg14				(void)
{
	ErrorLevel = 48;   /* PPG14                        */
	sw_reset();
}

__interrupt void  irq_ppg15				(void)
{
	ErrorLevel = 49;   /* PPG15                        */
	sw_reset();
}

__interrupt void  irq_ppg16				(void)
{
	ErrorLevel = 50;   /* PPG16                        */
	sw_reset();
}

__interrupt void  irq_ppg17				(void)
{
	ErrorLevel = 51;   /* PPG17                        */
	sw_reset();
}

__interrupt void  irq_ppg18				(void)
{
	ErrorLevel = 52;   /* PPG18                        */
	sw_reset();
}

__interrupt void  irq_ppg19				(void)
{
	ErrorLevel = 53;   /* PPG19                        */
	sw_reset();
}


/* emigrato */
//__interrupt void  irq_reload0 			(void)
//{
//	ErrorLevel = 54;   /* RLT0                         */
//	sw_reset();
//}

/* emigrato */
//__interrupt void  irq_reload1			(void)
//{
//	ErrorLevel = 55;   /* RLT1                         */
//	sw_reset();
//}

/* emigrato */
//__interrupt void  irq_reload2			(void)
//{
//	ErrorLevel = 56;   /* RLT2                         */
//	sw_reset();
//}

//__interrupt void  irq_reload3			(void)
//{
//	ErrorLevel = 57;   /* RLT3                         */
//	sw_reset();
//}

/* emigrato */
//__interrupt void  irq_reload6_ppgrlt	(void)
//{
//	ErrorLevel = 58;   /* PPGRLT - RLT6                */
//	sw_reset();
//}

__interrupt void  irq_icu0 				(void)
{
	ErrorLevel = 59;   /* ICU0                         */
	sw_reset();
}

__interrupt void  irq_icu1				(void)
{
	ErrorLevel = 60;   /* ICU1                         */
	sw_reset();
}

__interrupt void  irq_icu2				(void)
{
	ErrorLevel = 60;   /* ICU2                         */
	sw_reset();
}

__interrupt void  irq_icu3				(void)
{
	ErrorLevel = 60;   /* ICU3                         */
	sw_reset();
}

__interrupt void  irq_icu4				(void)
{
	ErrorLevel = 63;   /* ICU4                         */
	sw_reset();
}

__interrupt void  irq_icu5				(void)
{
	ErrorLevel = 64;   /* ICU5                         */
	sw_reset();
}

__interrupt void  irq_icu6				(void)
{
	ErrorLevel = 65;   /* ICU6                         */
	sw_reset();
}

__interrupt void  irq_icu7				(void)
{
	ErrorLevel = 66;   /* ICU7                         */
	sw_reset();
}

__interrupt void  irq_ocu0				(void)
{
	ErrorLevel = 71;   /* OCU4                         */
	sw_reset();
}

__interrupt void  irq_ocu1				(void)
{
	ErrorLevel = 71;   /* OCU4                         */
	sw_reset();
}

__interrupt void  irq_ocu2				(void)
{
	ErrorLevel = 71;   /* OCU4                         */
	sw_reset();
}

__interrupt void  irq_ocu3				(void)
{
	ErrorLevel = 71;   /* OCU4                         */
	sw_reset();
}


__interrupt void  irq_ocu4				(void)
{
	ErrorLevel = 71;   /* OCU4                         */
	sw_reset();
}

__interrupt void  irq_ocu5				(void)
{
	ErrorLevel = 72;   /* OCU5                         */
	sw_reset();
}

__interrupt void  irq_ocu6				(void)
{
	ErrorLevel = 73;   /* OCU6                         */
	sw_reset();
}

__interrupt void  irq_ocu7				(void)
{
	ErrorLevel = 74;   /* OCU7                         */
	sw_reset();
}

__interrupt void  irq_frt0				(void)
{
	ErrorLevel = 77;   /* FRT0                         */
	sw_reset();
}

__interrupt void  irq_frt1				(void)
{
	ErrorLevel = 78;   /* FRT1                         */
	sw_reset();
}

__interrupt void  irq_rtc0				(void)
{
	ErrorLevel = 81;   /* RTC0                         */
	sw_reset();
}

__interrupt void  irq_cal0				(void)
{
	ErrorLevel = 82;   /* CAL0                         */
	sw_reset();
}

__interrupt void  irq_i2c0				(void)
{
	ErrorLevel = 83;   /* I2C0                         */
	sw_reset();
}


__interrupt void  irq_i2c1				(void)
{
	ErrorLevel = 83;   /* I2C0                         */
	sw_reset();
}


__interrupt void  irq_adc				(void)
{
	ErrorLevel = 84;   /* ADC                          */
	sw_reset();
}

__interrupt void  irq_uart0_rx			(void)
{
	ErrorLevel = 85;   /* LIN-UART 2 RX                */
	sw_reset();
}

__interrupt void  irq_uart0_tx			(void)
{
	ErrorLevel = 86;   /* LIN-UART 2 TX                */
	sw_reset();
}

#if USE_MB96F3xx_USART1 || defined(__DOXYGEN__)
#else

__interrupt void  irq_uart1_rx			(void)
{
	ErrorLevel = 85;   // LIN-UART 2 RX                
	sw_reset();
}

__interrupt void  irq_uart1_tx			(void)
{
	ErrorLevel = 86;   // LIN-UART 2 TX                
	sw_reset();
}
#endif

#if USE_MB96F3xx_USART1 || defined(__DOXYGEN__)
#else
__interrupt void  irq_uart2_rx			(void)
{
	ErrorLevel = 85;   // LIN-UART 2 RX                
	sw_reset();
}

__interrupt void  irq_uart2_tx			(void)
{
	ErrorLevel = 86;   // LIN-UART 2 TX                
	sw_reset();
}
#endif


#if USE_MB96F3xx_USART3 || defined(__DOXYGEN__)
#else
__interrupt void  irq_uart3_rx(void)
{
	ErrorLevel = 87;   // LIN-UART 3 RX                
	sw_reset();
}

__interrupt void  irq_uart3_tx(void)
{
	ErrorLevel = 88;   // LIN-UART 3 TX
	sw_reset();
}

#endif


__interrupt void  irq_uart7_rx(void)
{
	ErrorLevel = 89;   /* LIN-UART 7 RX                */
	sw_reset();
}

__interrupt void  irq_uart7_tx(void)
{
	ErrorLevel = 90;   /* LIN-UART 7 TX                */
	sw_reset();
}

__interrupt void  irq_uart8_rx(void)
{
	ErrorLevel = 91;   /* LIN-UART 8 RX                */
	sw_reset();
}

__interrupt void  irq_uart8_tx(void)
{
	ErrorLevel = 92;   /* LIN-UART 8 TX                */
	sw_reset();
}

__interrupt void  irq_uart9_rx(void)
{
	ErrorLevel = 91;   /* LIN-UART 8 RX                */
	sw_reset();
}

__interrupt void  irq_uart9_tx			(void)
{
	ErrorLevel = 92;   /* LIN-UART 8 TX                */
	sw_reset();
}


__interrupt void  irq_flash				(void)
{
	ErrorLevel = 93;   /* MAIN FLASH IRQ               */

	sw_reset();
}


