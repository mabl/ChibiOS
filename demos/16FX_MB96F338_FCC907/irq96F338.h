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
void irq_init(void);
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
      

__interrupt void DefaultIRQHandler(void);
__interrupt void irq_nmi(void);				/* Non-maskable Interrupt       */
__interrupt void irq_delayed_int(void);		/* Delayed Interrupt            */
__interrupt void irq_rc_timer(void);		/* RC Timer                     */
__interrupt void irq_mainclk_timer(void);	/* Main Clock Timer             */
__interrupt void irq_subclk_timer(void);	/* Sub Clock Timer              */
__interrupt void irq_16(void);				/* Reserved                     */
__interrupt void irq_ext0(void);			/* EXT0                         */
__interrupt void irq_ext1(void);			/* EXT1                         */
__interrupt void irq_ext2(void);			/* EXT2                         */
__interrupt void irq_ext3(void);			/* EXT3                         */
__interrupt void irq_ext4(void);			/* EXT4                         */
__interrupt void irq_ext5(void);			/* EXT5                         */
__interrupt void irq_ext6(void);			/* EXT6                         */
__interrupt void irq_ext7(void);			/* EXT7                         */
__interrupt void irq_ext8(void);			/* EXT8                         */
__interrupt void irq_ext9(void);			/* EXT9                         */
__interrupt void irq_ext10(void);			/* EXT10                        */
__interrupt void irq_ext11(void);			/* EXT11                        */
__interrupt void irq_ext12(void);			/* EXT12                        */
__interrupt void irq_ext13(void);			/* EXT13                        */
__interrupt void irq_ext14(void);			/* EXT14                        */
__interrupt void irq_ext15(void);			/* EXT15                        */
__interrupt void irq_can0_tx_rx(void);		/* CAN0                         */
__interrupt void irq_can1_tx_rx(void);		/* CAN1                         */
__interrupt void irq_can2_tx_rx(void);		/* CAN2                         */
__interrupt void irq_ppg0(void);			/* PPG0                         */
__interrupt void irq_ppg1(void);			/* PPG1                         */
__interrupt void irq_ppg2(void);			/* PPG2                         */
__interrupt void irq_ppg3(void);			/* PPG3                         */
__interrupt void irq_ppg4(void);			/* PPG4                         */
__interrupt void irq_ppg5(void);			/* PPG5                         */
__interrupt void irq_ppg6(void);			/* PPG6                         */
__interrupt void irq_ppg7(void);			/* PPG7                         */
__interrupt void irq_ppg8(void);			/* PPG8                         */
__interrupt void irq_ppg9(void);			/* PPG9                         */
__interrupt void irq_ppg10(void);			/* PPG10                        */
__interrupt void irq_ppg11(void);			/* PPG11                        */
__interrupt void irq_ppg12(void);			/* PPG12                        */
__interrupt void irq_ppg13(void);			/* PPG13                        */
__interrupt void irq_ppg14(void);			/* PPG14                        */
__interrupt void irq_ppg15(void);			/* PPG15                        */
__interrupt void irq_ppg16(void);			/* PPG16                        */
__interrupt void irq_ppg17(void);			/* PPG17                        */
__interrupt void irq_ppg18(void);			/* PPG18                        */
__interrupt void irq_ppg19(void);			/* PPG19                        */
__interrupt void irq_reload0(void);			/* RLT0                         */
__interrupt void irq_reload1(void);			/* RLT1                         */
__interrupt void irq_reload2(void);			/* RLT2                         */
__interrupt void irq_reload3(void);			/* RLT3                         */
__interrupt void irq_reload6_ppgrlt(void);	/* RLT6-PPG                     */
__interrupt void irq_icu0(void);			/* ICU0                         */
__interrupt void irq_icu1(void);			/* ICU1                         */
__interrupt void irq_icu2(void);			/* ICU2                         */
__interrupt void irq_icu3(void);			/* ICU3                         */
__interrupt void irq_icu4(void);			/* ICU4                         */
__interrupt void irq_icu5(void);			/* ICU5                         */
__interrupt void irq_icu6(void);			/* ICU6                         */
__interrupt void irq_icu7(void);			/* ICU7                         */
__interrupt void irq_icu8(void);			/* ICU8                         */
__interrupt void irq_icu9(void);			/* ICU9                         */
__interrupt void irq_ocu0(void);			/* OCU0                         */
__interrupt void irq_ocu1(void);			/* OCU1                         */
__interrupt void irq_ocu2(void);			/* OCU2                         */
__interrupt void irq_ocu3(void);			/* OCU3                         */
__interrupt void irq_ocu4(void);			/* OCU4                         */
__interrupt void irq_ocu5(void);			/* OCU5                         */
__interrupt void irq_ocu6(void);			/* OCU6                         */
__interrupt void irq_ocu7(void);			/* OCU7                         */
__interrupt void irq_ocu8(void);			/* OCU8                         */
__interrupt void irq_ocu9(void);			/* OCU9                         */
__interrupt void irq_ocu10(void);			/* OCU10                        */
__interrupt void irq_ocu11(void);			/* OCU11                        */
__interrupt void irq_frt0(void);			/* FRT0                         */
__interrupt void irq_frt1(void);			/* FRT1                         */
__interrupt void irq_frt2(void);			/* FRT2                         */
__interrupt void irq_frt3(void);			/* FRT3                         */
__interrupt void irq_rtc0(void);			/* Real Time Clock 0            */
__interrupt void irq_cal0(void);			/* Clock Calibration Unit 0     */
__interrupt void irq_i2c0(void);			/* I2C0                         */
__interrupt void irq_i2c1(void);			/* I2C1                         */
__interrupt void irq_adc0(void);			/* ADC0                         */
__interrupt void irq_alarm0(void);			/* ALARM0                       */
__interrupt void irq_alarm1(void);			/* ALARM1                       */
__interrupt void irq_uart0_rx(void);		/* LIN-UART 0 RX                */
__interrupt void irq_uart0_tx(void);		/* LIN-UART 0 TX                */
__interrupt void irq_uart1_rx(void);		/* LIN-UART 1 RX                */
__interrupt void irq_uart1_tx(void);		/* LIN-UART 1 TX                */
__interrupt void irq_uart2_rx(void);		/* LIN-UART 2 RX                */
__interrupt void irq_uart2_tx(void);		/* LIN-UART 2 TX                */
__interrupt void irq_uart3_rx(void);		/* LIN-UART 3 RX                */
__interrupt void irq_uart3_tx(void);		/* LIN-UART 3 TX                */
__interrupt void irq_uart5_rx(void);		/* LIN-UART 5 RX                */
__interrupt void irq_uart5_tx(void);		/* LIN-UART 5 TX                */
__interrupt void irq_uart7_rx(void);		/* LIN-UART 7 RX                */
__interrupt void irq_uart7_tx(void);		/* LIN-UART 7 TX                */
__interrupt void irq_uart8_rx(void);		/* LIN-UART 8 RX                */
__interrupt void irq_uart8_tx(void);		/* LIN-UART 8 TX                */
__interrupt void irq_uart9_rx(void);		/* LIN-UART 9 RX                */
__interrupt void irq_uart9_tx(void);		/* LIN-UART 9 TX                */
__interrupt void irq_flash(void);			/* Main Flash IRQ               */
__interrupt void irq_111(void);				/* Reserved                     */
__interrupt void irq_usb_EP0_in(void);		/* USB EP0 IN  (only MB9633xU)  */
__interrupt void irq_usb_EP0_out(void);		/* USB EP0 OUT (only MB9633xU)  */
__interrupt void irq_usb_EP1(void);			/* USB EP1     (only MB9633xU)  */
__interrupt void irq_usb_EP2(void);			/* USB EP2     (only MB9633xU)  */
__interrupt void irq_usb_EP3(void);			/* USB EP3     (only MB9633xU)  */
__interrupt void irq_usb_EP4(void);			/* USB EP4     (only MB9633xU)  */
__interrupt void irq_usb_EP5(void);			/* USB EP5     (only MB9633xU)  */
__interrupt void irq_usb_Function1(void);	/* USB Function Flag 1 (only MB9633xU) */
__interrupt void irq_usb_Function2(void);	/* USB Function Flag 2 (only MB9633xU) */
__interrupt void irq_usb_MiniHost1(void);	/* USB MiniHost Flag 1 (only MB9633xU) */
__interrupt void irq_usb_MiniHost2(void);	/* USB MiniHost Flag 2 (only MB9633xU) */


#endif

/****************************************************************************/
/**                                                                        **/
/**                               EOF                                      **/
/**                                                                        **/
/****************************************************************************/
