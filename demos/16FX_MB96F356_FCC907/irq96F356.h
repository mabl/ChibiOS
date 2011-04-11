/*
                             *******************
******************************* C HEADER FILE *******************************
**                           *******************                           **
**                                                                         **
** project      : General modules Fujitsu F2MC-16                          **
** filename     : VECTORS.H                                                **
** version      : 2                                                        **
** last revised : September 26, 2004                                       **
**                                                                         **
*****************************************************************************
**                                                                         **
** Copyright (c) 1998-2004, P.K. van der Vlugt                             **
** All rights reserved.                                                    **
**                                                                         **
*****************************************************************************

VERSION HISTORY:
----------------

Version     : 1
Date        : November 09, 2003
Revised by  : Peter van der Vlugt
Description : Original version.

Version     : 2
Date        : September 18, 2004 
Revised by  : P.K. van der Vlugt
Description : * Changed include of ck_cfg.h to comply to new kernel module 
                naming convention.
*/

#ifndef _VECTORS_INCLUDED
#define _VECTORS_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                     MODULES USED                                       **/
/**                                                                        **/
/****************************************************************************/


/****************************************************************************/
/**                                                                        **/
/**                     DEFINITIONS AND MACROS                             **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                     TYPEDEFS AND STRUCTURES                            **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED VARIABLES                                 **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                     EXPORTED FUNCTIONS                                 **/
/**                                                                        **/
/****************************************************************************/


/****************************************************************************/
void InitIrqLevels (void);
/****************************************************************************/
/*
 * This function presets all interrupt control registers. It must be used
 * to set all interrupt priorities. 
 *
 * NOTE: Two resource interrupts always share one ICR register.
 * NOTE: Finally sets ILM to 7, thus allowing all interrupts levels.
 * NOTE: The application has to use _enable () for AFTER InitIrqLevels.
 */


/****************************************************************************
 * Interrupt Service Routines prototypes
 ****************************************************************************/

/****************************************************************************/
__interrupt void DefaultIRQHandler (void);
/****************************************************************************/

/*
 * Interrupt handler used by the INT 9 instruction for performing a task
 * context switch.
 */
      

__interrupt void  irq_nmi				(void);         //	11   /* Non-maskable Interrupt       */
__interrupt void  irq_delayed_int		(void);         //  12   /* Delayed Interrupt            */
__interrupt void  irq_rc_timer 			(void);         //  13   /* RC Timer                     */
__interrupt void  irq_mainclk_timer		(void);         //  14   /* Main Clock Timer             */
__interrupt void  irq_subclk_timer		(void);         //  15   /* Sub Clock Timer              */
__interrupt void  DefaultIRQHandler 	(void);         //  16   /* Reserved                     */
__interrupt void  irq_ext0				(void);         //  17   /* EXT0                         */
__interrupt void  irq_ext1				(void);         //	18   /* EXT1                         */
__interrupt void  irq_ext2				(void);         //  19   /* EXT2                         */
__interrupt void  irq_ext3				(void);         //  20   /* EXT3                         */
__interrupt void  irq_ext4				(void);         //  21   /* EXT4                         */
__interrupt void  irq_ext5				(void);         //  22   /* EXT5                         */
__interrupt void  irq_ext7				(void);         //  23   /* EXT7                         */
__interrupt void  irq_ext8				(void);         //  24   /* EXT8                         */
__interrupt void  irq_ext9				(void);         //	25   /* EXT9                         */
__interrupt void  irq_ext10				(void);         //  26   /* EXT10                        */
__interrupt void  irq_ext11				(void);         //  27   /* EXT11                        */
__interrupt void  irq_ext12				(void);         //  28   /* EXT12                        */
__interrupt void  irq_ext13				(void);         //  29   /* EXT13                        */
__interrupt void  irq_ext14				(void);         //  30   /* EXT14                        */
__interrupt void  irq_ext15				(void);         //  31   /* EXT15                        */
__interrupt void  irq_can1_tx_rx		(void);         //	32   /* CAN1                         */
__interrupt void  irq_can2_tx_rx		(void);         //  33   /* CAN2                         */
__interrupt void  irq_ppg0				(void);         //  34   /* PPG0                         */
__interrupt void  irq_ppg1				(void);         //  35   /* PPG1                         */
__interrupt void  irq_ppg2				(void);         //  36   /* PPG2                         */
__interrupt void  irq_ppg3				(void);         //  37   /* PPG3                         */
__interrupt void  irq_ppg4				(void);         //  38   /* PPG4                         */
__interrupt void  irq_ppg5				(void);         //	39   /* PPG5                         */
__interrupt void  irq_ppg6				(void);         //  40   /* PPG6                         */
__interrupt void  irq_ppg7				(void);         //  41   /* PPG7                         */
__interrupt void  irq_ppg8				(void);         //  42   /* PPG8                         */
__interrupt void  irq_ppg9				(void);         //  43   /* PPG9                         */
__interrupt void  irq_ppg10				(void);         //  44   /* PPG10                        */
__interrupt void  irq_ppg11				(void);         //  45   /* PPG11                        */
__interrupt void  irq_ppg12				(void);         //	46   /* PPG12                        */
__interrupt void  irq_ppg13				(void);         //  47   /* PPG13                        */
__interrupt void  irq_ppg14				(void);         //  48   /* PPG14                        */
__interrupt void  irq_ppg15				(void);         //  49   /* PPG15                        */
__interrupt void  irq_ppg16				(void);         //  50   /* PPG16                        */
__interrupt void  irq_ppg17				(void);         //  51   /* PPG17                        */
__interrupt void  irq_ppg18				(void);         //  52   /* PPG18                        */
__interrupt void  irq_ppg19				(void);         //	53   /* PPG19                        */
__interrupt void  irq_reload0 			(void);         //  54   /* RLT0                         */
__interrupt void  irq_reload1			(void);         //  55   /* RLT1                         */
__interrupt void  irq_reload2			(void);         //  56   /* RLT2                         */
__interrupt void  irq_reload3			(void);         //  57   /* RLT3                         */
extern __interrupt void  irq_reload6_ppgrlt	(void);         //  58   /* PPGRLT - RLT6                */
__interrupt void  irq_icu0 				(void);         //  59   /* ICU0                         */
__interrupt void  irq_icu1				(void);         //	60   /* ICU1                         */
__interrupt void  irq_icu4				(void);         //  63   /* ICU4                         */
__interrupt void  irq_icu5				(void);         //  64   /* ICU5                         */
__interrupt void  irq_icu6				(void);         //  65   /* ICU6                         */
__interrupt void  irq_icu7				(void);         //  66   /* ICU7                         */
__interrupt void  irq_ocu4				(void);         //  71   /* OCU4                         */
__interrupt void  irq_ocu5				(void);         //  72   /* OCU5                         */
__interrupt void  irq_ocu6				(void);         //	73   /* OCU6                         */
__interrupt void  irq_ocu7				(void);         //  74   /* OCU7                         */
__interrupt void  irq_frt0				(void);         //  77   /* FRT0                         */
__interrupt void  irq_frt1				(void);         //  78   /* FRT1                         */
__interrupt void  irq_rtc0				(void);         //  81   /* RTC0                         */
__interrupt void  irq_cal0				(void);         //  82   /* CAL0                         */
__interrupt void  irq_i2c0				(void);         //  83   /* I2C0                         */
__interrupt void  irq_adc				(void);         //	84   /* ADC                          */
__interrupt void  irq_uart2_rx			(void);         //  85   /* LIN-UART 2 RX                */
__interrupt void  irq_uart2_tx			(void);         //  86   /* LIN-UART 2 TX                */
__interrupt void  irq_uart3_rx			(void);         //  87   /* LIN-UART 3 RX                */
__interrupt void  irq_uart3_tx			(void);         //  88   /* LIN-UART 3 TX                */
__interrupt void  irq_uart7_rx			(void);         //  89   /* LIN-UART 7 RX                */
__interrupt void  irq_uart7_tx			(void);         //  90   /* LIN-UART 7 TX                */
__interrupt void  irq_uart8_rx			(void);         //  91   /* LIN-UART 8 RX                */
__interrupt void  irq_uart8_tx			(void);         //  92   /* LIN-UART 8 TX                */
__interrupt void  irq_flash				(void);         //  93   /* MAIN FLASH IRQ               */



#endif

/****************************************************************************/
/**                                                                        **/
/**                               EOF                                      **/
/**                                                                        **/
/****************************************************************************/
