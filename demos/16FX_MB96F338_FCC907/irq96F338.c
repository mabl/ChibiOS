/* THIS SAMPLE CODE IS PROVIDED AS IS AND IS SUBJECT TO ALTERATIONS. FUJITSU */
/* MICROELECTRONICS ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR */
/* ELIGIBILITY FOR ANY PURPOSES.                                             */
/*                 (C) Fujitsu Microelectronics Europe GmbH                  */
/*---------------------------------------------------------------------------
  VECTORS.C
  - Interrupt level (priority) setting
  - Interrupt vector definition
-----------------------------------------------------------------------------*/

#include <ch.h>
#include <hal.h>

#include "appboard.h"
#include "irq96f338.h"


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

#define MIN_ICR  12			// n° primo  interrupt mascherabile
#define MAX_ICR  122		// n° ultimo interrupt mascherabile

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

#pragma intvect irq_nmi					11   // Non-maskable Interrupt        
#pragma intvect irq_delayed_int			12   // Delayed Interrupt             
#pragma intvect irq_rc_timer 			13   // RC Timer                      
#pragma intvect irq_mainclk_timer		14   // Main Clock Timer              
#pragma intvect irq_subclk_timer		15   // Sub Clock Timer               
#pragma intvect DefaultIRQHandler 		16   /* Reserved                     */                    
#pragma intvect irq_ext0				17   // EXT0                          
#pragma intvect irq_ext1 				18   // EXT1                          
#pragma intvect irq_ext2				19   // EXT2                          
#pragma intvect irq_ext3				20   // EXT3                          
#pragma intvect irq_ext4				21   // EXT4                          
#pragma intvect irq_ext5				22   // EXT5                          
#pragma intvect irq_ext6				23   // EXT6                          
#pragma intvect irq_ext7				24   // EXT7                          
#pragma intvect irq_ext8				25   // EXT8                          
#pragma intvect irq_ext9				26   // EXT9                          
#pragma intvect irq_ext10				27   // EXT10                         
#pragma intvect irq_ext11				28   // EXT11                         
#pragma intvect irq_ext12				29   // EXT12                         
#pragma intvect irq_ext13 				30   // EXT13                         
#pragma intvect irq_ext14 				31   // EXT14                         
#pragma intvect irq_ext15 				32   // EXT15                         
#pragma intvect irq_can0_tx_rx			33   // CAN0                          
#pragma intvect irq_can1_tx_rx		 	34   // CAN1                          
#pragma intvect irq_can2_tx_rx		 	35   // CAN2                          
#pragma intvect irq_ppg0				36   // PPG0                          
#pragma intvect irq_ppg1			 	37   // PPG1                          
#pragma intvect irq_ppg2			 	38   // PPG2                          
#pragma intvect irq_ppg3 				39   // PPG3                          
#pragma intvect irq_ppg4			 	40   // PPG4                          
#pragma intvect irq_ppg5			 	41   // PPG5                          
#pragma intvect irq_ppg6			 	42   // PPG6                          
#pragma intvect irq_ppg7			 	43   // PPG7                          
#pragma intvect irq_ppg8 				44   // PPG8                          
#pragma intvect irq_ppg9 				45   // PPG9                          
#pragma intvect irq_ppg10			 	46   // PPG10                         
#pragma intvect irq_ppg11 				47   // PPG11                         
#pragma intvect irq_ppg12			 	48   // PPG12                         
#pragma intvect irq_ppg13 				49   // PPG13                         
#pragma intvect irq_ppg14			 	50   // PPG14                         
#pragma intvect irq_ppg15			 	51   // PPG15                         
#pragma intvect irq_ppg16			 	52   // PPG16                         
#pragma intvect irq_ppg17			 	53   // PPG17                         
#pragma intvect irq_ppg18			 	54   // PPG18                         
#pragma intvect irq_ppg19			 	55   // PPG19                         
#pragma intvect irq_reload0			 	56   // RLT0                          
#pragma intvect irq_reload1			 	57   // RLT1                          
#pragma intvect irq_reload2				58   // RLT2                          
#pragma intvect irq_reload3 			59   // RLT3                          
#pragma intvect irq_reload6_ppgrlt 		60   // RLT6-PPG
#pragma intvect irq_icu0 				61   // ICU0                          
#pragma intvect irq_icu1 				62   // ICU1                          
#pragma intvect irq_icu2 			 	63   // ICU2                          
#pragma intvect irq_icu3 			 	64   // ICU3                          
#pragma intvect irq_icu4 			 	65   // ICU4                          
#pragma intvect irq_icu5 				66   // ICU5                          
#pragma intvect irq_icu6 				67   // ICU6                          
#pragma intvect irq_icu7 			 	68   // ICU7                          
#pragma intvect irq_icu8 			 	69   // ICU8                          
#pragma intvect irq_icu9 				70   // ICU9                          
#pragma intvect irq_ocu0 				71   // OCU0                          
#pragma intvect irq_ocu1 				72   // OCU1                          
#pragma intvect irq_ocu2 			 	73   // OCU2                          
#pragma intvect irq_ocu3 			 	74   // OCU3                          
#pragma intvect irq_ocu4 				75   // OCU4                          
#pragma intvect irq_ocu5 				76   // OCU5                          
#pragma intvect irq_ocu6			 	77   // OCU6                          
#pragma intvect irq_ocu7			 	78   // OCU7                          
#pragma intvect irq_ocu8			 	79   // OCU8                          
#pragma intvect irq_ocu9			 	80   // OCU9                          
#pragma intvect irq_ocu10	 	 		81   // OCU10                         
#pragma intvect irq_ocu11		 		82   // OCU11                         
#pragma intvect irq_frt0 				83   // FRT0                          
#pragma intvect irq_frt1				84   // FRT1                          
#pragma intvect irq_frt2 				85   // FRT2                          
#pragma intvect irq_frt3				86   // FRT3                          
#pragma intvect irq_rtc0 				87   // Real Time Clock 0             
#pragma intvect irq_cal0 				88   // Clock Calibration Unit 0      
#pragma intvect irq_i2c0				89   // I2C0                          
#pragma intvect irq_i2c1				90   // I2C1                          
#pragma intvect irq_adc0				91   // ADC0                          
#pragma intvect irq_alarm0				92   // ALARM0                        
#pragma intvect irq_alarm1				93   // ALARM1                        
#pragma intvect irq_uart0_rx 			94   // LIN-UART 0 RX                 
#pragma intvect irq_uart0_tx 			95   // LIN-UART 0 TX                 
#pragma intvect irq_uart1_rx 			96   // LIN-UART 1 RX                 
#pragma intvect irq_uart1_tx 			97   // LIN-UART 1 TX                 
#pragma intvect irq_uart2_rx 			98   // LIN-UART 2 RX                 
#pragma intvect irq_uart2_tx 			99   // LIN-UART 2 TX                 
#pragma intvect irq_uart3_rx 			100  // LIN-UART 3 RX                 
#pragma intvect irq_uart3_tx 			101  // LIN-UART 3 TX                 
#pragma intvect irq_uart5_rx 			102  // LIN-UART 5 RX                 
#pragma intvect irq_uart5_tx 			103  // LIN-UART 5 TX                 
#pragma intvect irq_uart7_rx 			104  // LIN-UART 7 RX                 
#pragma intvect irq_uart7_tx 			105  // LIN-UART 7 TX                 
#pragma intvect irq_uart8_rx 			106  // LIN-UART 8 RX                 
#pragma intvect irq_uart8_tx 			107  // LIN-UART 8 TX                 
#pragma intvect irq_uart9_rx 			108  // LIN-UART 9 RX                 
#pragma intvect irq_uart9_tx 			109  // LIN-UART 9 TX                 
#pragma intvect irq_flash 				110  // Main Flash IRQ                
#pragma intvect DefaultIRQHandler 		111  // Reserved             
#pragma intvect irq_usb_EP0_in 			112  // USB EP0 IN  (only MB9633xU)   
#pragma intvect irq_usb_EP0_out 		113  // USB EP0 OUT (only MB9633xU)   
#pragma intvect irq_usb_EP1 			114  // USB EP1     (only MB9633xU)   
#pragma intvect irq_usb_EP2 			115  // USB EP2     (only MB9633xU)   
#pragma intvect irq_usb_EP3 			116  // USB EP3     (only MB9633xU)   
#pragma intvect irq_usb_EP4 			117  // USB EP4     (only MB9633xU)   
#pragma intvect irq_usb_EP5 			118  // USB EP5     (only MB9633xU)   
#pragma intvect irq_usb_Function1		119  // USB Function Flag 1 (only MB9633xU)  
#pragma intvect irq_usb_Function2		120  // USB Function Flag 2 (only MB9633xU)  
#pragma intvect irq_usb_MiniHost1	 	121  // USB MiniHost Flag 1 (only MB9633xU)  
#pragma intvect irq_usb_MiniHost2 		122  // USB MiniHost Flag 2 (only MB9633xU)  


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



__interrupt 
void irq_nmi(void)
{
	ErrorLevel = 11;	// Non-maskable Interrupt        
	sw_reset();
}	

__interrupt 
void irq_delayed_int(void)
{
	ErrorLevel = 12;  	// Delayed Interrupt             
	sw_reset();
}	

__interrupt 
void irq_rc_timer(void)
{
	ErrorLevel = 13;  	// RC Timer                      
	sw_reset();
}	


__interrupt 
void irq_mainclk_timer(void)
{
	ErrorLevel = 14;  	// Main Clock Timer              
	sw_reset();
}


__interrupt 
void irq_subclk_timer(void)
{
	ErrorLevel = 15;  	// Sub Clock Timer               
	sw_reset();
}	

__interrupt 
void irq_16(void)
{
	ErrorLevel = 16;  	// Reserved                      
	sw_reset();
}	

__interrupt 
void irq_ext0(void)
{
	ErrorLevel = 17;  	// EXT0                          
	sw_reset();
}	

__interrupt 
void irq_ext1(void)
{
	ErrorLevel = 18;  	// EXT1                          
	sw_reset();
}	

__interrupt 
void irq_ext2(void)
{
	ErrorLevel = 19;  	// EXT2                          
	sw_reset();
}	

__interrupt 
void irq_ext3(void)
{
	ErrorLevel = 20;  	// EXT3                          
	sw_reset();
}	

__interrupt 
void irq_ext4(void)
{
	ErrorLevel = 21;  	// EXT4                          
	sw_reset();
}	

__interrupt 
void irq_ext5(void)
{
	ErrorLevel = 22;  	// EXT5                          
	sw_reset();
}	

__interrupt 
void irq_ext6(void)
{
	ErrorLevel = 23;  	// EXT6                          
	sw_reset();
}	

__interrupt 
void irq_ext7(void)
{
	ErrorLevel = 24;  	// EXT7                          
	sw_reset();
}	

__interrupt 
void irq_ext8(void)
{
	ErrorLevel = 25;  	// EXT8                          
	sw_reset();
}	

__interrupt 
void irq_ext9(void)
{
	ErrorLevel = 26;  	// EXT9                          
	sw_reset();
}	

__interrupt 
void irq_ext10(void)
{
	ErrorLevel = 27;  	// EXT10                         
	sw_reset();
}	

__interrupt 
void irq_ext11(void)
{
	ErrorLevel = 28;  	// EXT11                         
	sw_reset();
}	

__interrupt 
void irq_ext12(void)
{
	ErrorLevel = 29;  	// EXT12                         
	sw_reset();
}	

__interrupt 
void irq_ext13(void)
{
	ErrorLevel = 30;  	// EXT13                         
	sw_reset();
}	

__interrupt 
void irq_ext14(void)
{
	ErrorLevel = 31;  	// EXT14                         
	sw_reset();
}	

__interrupt 
void irq_ext15(void)
{
	ErrorLevel = 32;  	// EXT15                         
	sw_reset();
}	

__interrupt 
void irq_can0_tx_rx(void)
{
	ErrorLevel = 33;  	// CAN0                          
	sw_reset();
}	

__interrupt 
void irq_can1_tx_rx(void)
{
	ErrorLevel = 34;  	// CAN1                          
	sw_reset();
}	

__interrupt 
void irq_can2_tx_rx(void)
{
	ErrorLevel = 35;  	// CAN2                          
	sw_reset();
}	

__interrupt 
void irq_ppg0(void)
{
	ErrorLevel = 36;  	// PPG0                          
	sw_reset();
}	

__interrupt 
void irq_ppg1(void)
{
	ErrorLevel = 37;  	// PPG1                          
	sw_reset();
}	

__interrupt 
void irq_ppg2(void)
{
	ErrorLevel = 38;  	// PPG2                          
	sw_reset();
}	

__interrupt 
void irq_ppg3(void)
{
	ErrorLevel = 39;  	// PPG3                          
	sw_reset();
}	

__interrupt 
void irq_ppg4(void)
{
	ErrorLevel = 40;  	// PPG4                          
	sw_reset();
}	

__interrupt 
void irq_ppg5(void)
{
	ErrorLevel = 41;  	// PPG5                          
	sw_reset();
}	

__interrupt 
void irq_ppg6(void)
{
	ErrorLevel = 42;  	// PPG6                          
	sw_reset();
}	

__interrupt 
void irq_ppg7(void)
{
	ErrorLevel = 43;  	// PPG7                          
	sw_reset();
}	

__interrupt 
void irq_ppg8(void)
{
	ErrorLevel = 44;  	// PPG8                          
	sw_reset();
}	

__interrupt 
void irq_ppg9(void)
{
	ErrorLevel = 45;  	// PPG9                          
	sw_reset();
}	

__interrupt 
void irq_ppg10(void)
{
	ErrorLevel = 46;  	// PPG10                         
	sw_reset();
}	

__interrupt 
void irq_ppg11(void)
{
	ErrorLevel = 47;  	// PPG11                         
	sw_reset();
}	

__interrupt 
void irq_ppg12(void)
{
	ErrorLevel = 48;  	// PPG12                         
	sw_reset();
}	

__interrupt 
void irq_ppg13(void)
{
	ErrorLevel = 49;  	// PPG13                         
	sw_reset();
}	

__interrupt 
void irq_ppg14(void)
{
	ErrorLevel = 50;  	// PPG14                         
	sw_reset();
}	

__interrupt 
void irq_ppg15(void)
{
	ErrorLevel = 51;  	// PPG15                         
	sw_reset();
}	

__interrupt 
void irq_ppg16(void)
{
	ErrorLevel = 52;  	// PPG16                         
	sw_reset();
}	

__interrupt 
void irq_ppg17(void)
{
	ErrorLevel = 53;  	// PPG17                         
	sw_reset();
}	

__interrupt 
void irq_ppg18(void)
{
	ErrorLevel = 54;  	// PPG18                         
	sw_reset();
}	

__interrupt 
void irq_ppg19(void)
{
	ErrorLevel = 55;  	// PPG19                         
	sw_reset();
}	

/*	__interrupt 
void irq_reload0(void)
{
	ErrorLevel = 56;  	// RLT0
	sw_reset();
}
*/

/*	__interrupt 
void irq_reload1(void)
{
	ErrorLevel = 57;  	// RLT1                          
	sw_reset();
}	
*/

/*
__interrupt 
void irq_reload2(void)
{
	ErrorLevel = 58;  	// RLT2                          
	sw_reset();
}	

__interrupt 
void irq_reload3(void)
{
	ErrorLevel = 59;  	// RLT3                          
	sw_reset();
}	
*/

/* used by system tick
__interrupt 
void irq_reload6_ppgrlt(void)
{
	ErrorLevel = 60;  	// RLT6-PPG                      
	sw_reset();
}	
*/

__interrupt 
void irq_icu0(void)
{
	ErrorLevel = 61;  	// ICU0                          
	sw_reset();
}	

__interrupt 
void irq_icu1(void)
{
	ErrorLevel = 62;  	// ICU1                          
	sw_reset();
}	

__interrupt 
void irq_icu2(void)
{
	ErrorLevel = 63;  	// ICU2                          
	sw_reset();
}	

__interrupt 
void irq_icu3(void)
{
	ErrorLevel = 64;  	// ICU3                          
	sw_reset();
}	

__interrupt 
void irq_icu4(void)
{
	ErrorLevel = 65;  	// ICU4                          
	sw_reset();
}	

__interrupt 
void irq_icu5(void)
{
	ErrorLevel = 66;  	// ICU5                          
	sw_reset();
}	

__interrupt 
void irq_icu6(void)
{
	ErrorLevel = 67;  	// ICU6                          
	sw_reset();
}	

__interrupt 
void irq_icu7(void)
{
	ErrorLevel = 68;  	// ICU7                          
	sw_reset();
}	

__interrupt 
void irq_icu8(void)
{
	ErrorLevel = 69;  	// ICU8                          
	sw_reset();
}	

__interrupt 
void irq_icu9(void)
{
	ErrorLevel = 70;  	// ICU9                          
	sw_reset();
}	

__interrupt 
void irq_ocu0(void)
{
	ErrorLevel = 71;  	// OCU0                          
	sw_reset();
}	

__interrupt 
void irq_ocu1(void)
{
	ErrorLevel = 72;  	// OCU1                          
	sw_reset();
}	

__interrupt 
void irq_ocu2(void)
{
	ErrorLevel = 73;  	// OCU2                          
	sw_reset();
}	

__interrupt 
void irq_ocu3(void)
{
	ErrorLevel = 74;  	// OCU3                          
	sw_reset();
}	

__interrupt 
void irq_ocu4(void)
{
	ErrorLevel = 75;  	// OCU4                          
	sw_reset();
}	

__interrupt 
void irq_ocu5(void)
{
	ErrorLevel = 76;  	// OCU5                          
	sw_reset();
}	

__interrupt 
void irq_ocu6(void)
{
	ErrorLevel = 77;  	// OCU6                          
	sw_reset();
}	

__interrupt 
void irq_ocu7(void)
{
	ErrorLevel = 78;  	// OCU7                          
	sw_reset();
}	

__interrupt 
void irq_ocu8(void)
{
	ErrorLevel = 79;  	// OCU8                          
	sw_reset();
}	

__interrupt 
void irq_ocu9(void)
{
	ErrorLevel = 80;  	// OCU9                          
	sw_reset();
}	

__interrupt 
void irq_ocu10(void)
{
	ErrorLevel = 81;  	// OCU10                         
	sw_reset();
}	

__interrupt 
void irq_ocu11(void)
{
	ErrorLevel = 82;  	// OCU11                         
	sw_reset();
}	

__interrupt 
void irq_frt0(void)
{
	ErrorLevel = 83;  	// FRT0                          
	sw_reset();
}	

__interrupt 
void irq_frt1(void)
{
	ErrorLevel = 84;  	// FRT1                          
	sw_reset();
}	

__interrupt 
void irq_frt2(void)
{
	ErrorLevel = 85;  	// FRT2                          
	sw_reset();
}	

__interrupt 
void irq_frt3(void)
{
	ErrorLevel = 86;  	// FRT3                          
	sw_reset();
}	

__interrupt 
void irq_rtc0(void)
{
	ErrorLevel = 87;  	// Real Time Clock 0             
	sw_reset();
}	

__interrupt 
void irq_cal0(void)
{
	ErrorLevel = 88;  	// Clock Calibration Unit 0      
	sw_reset();
}	

__interrupt 
void irq_i2c0(void)
{
	ErrorLevel = 89;  	// I2C0                          
	sw_reset();
}	

__interrupt 
void irq_i2c1(void)
{
	ErrorLevel = 90;  	// I2C1                          
	sw_reset();
}	

__interrupt 
void irq_adc0(void)
{
	ErrorLevel = 91;  	// ADC0                          
	sw_reset();
}	

__interrupt 
void irq_alarm0(void)
{
	ErrorLevel = 92;  	// ALARM0                        
	sw_reset();
}	

__interrupt 
void irq_alarm1(void)
{
	ErrorLevel = 93;  	// ALARM1                        
	sw_reset();
}	

#if USE_MB96F3xx_USART0 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart0_rx(void)
{
	ErrorLevel = 94;  	// LIN-UART 0 RX                 
	sw_reset();
}	


__interrupt 
void irq_uart0_tx(void)
{
	ErrorLevel = 95;  	// LIN-UART 0 TX                 
	sw_reset();
}	

#endif

#if USE_MB96F3xx_USART1 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart1_rx(void)
{
	ErrorLevel = 96;  	// LIN-UART 1 RX                 
	sw_reset();
}	


__interrupt 
void irq_uart1_tx(void)
{
	ErrorLevel = 97;  	// LIN-UART 1 TX                 
	sw_reset();
}

#endif


#if USE_MB96F3xx_USART2 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart2_rx(void)
{
	ErrorLevel = 98;  	// LIN-UART 2 RX                 
	sw_reset();
}	

__interrupt 
void irq_uart2_tx(void)
{
	ErrorLevel = 99;  	// LIN-UART 2 TX                 
	sw_reset();
}	

#endif


#if USE_MB96F3xx_USART3 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart3_rx(void)
{
	ErrorLevel = 100; 	// LIN-UART 3 RX                 
	sw_reset();
}	

__interrupt 
void irq_uart3_tx(void)
{
	ErrorLevel = 101;	// LIN-UART 3 TX                 
	sw_reset();
}	

#endif


#if USE_MB96F3xx_USART5 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart5_rx(void)
{
	ErrorLevel = 102; 	// LIN-UART 5 RX                 
	sw_reset();
}	

__interrupt 
void irq_uart5_tx(void)
{
	ErrorLevel = 103; 	// LIN-UART 5 TX                 
	sw_reset();
}	

#endif

#if USE_MB96F3xx_USART7 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart7_rx(void)
{
	ErrorLevel = 104; 	// LIN-UART 7 RX                 
	sw_reset();
}	

__interrupt 
void irq_uart7_tx(void)
{
	ErrorLevel = 105; 	// LIN-UART 7 TX                 
	sw_reset();
}	

#endif


#if USE_MB96F3xx_USART8 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart8_rx(void)
{
	ErrorLevel = 106; 	// LIN-UART 8 RX                 
	sw_reset();
}	

__interrupt 
void irq_uart8_tx(void)
{
	ErrorLevel = 107; 	// LIN-UART 8 TX                 
	sw_reset();
}	

#endif


#if USE_MB96F3xx_USART9 || defined(__DOXYGEN__)
#else

__interrupt 
void irq_uart9_rx(void)
{
	ErrorLevel = 108; 	// LIN-UART 9 RX                 
	sw_reset();
}	

__interrupt 
void irq_uart9_tx(void)
{
	ErrorLevel = 109; 	// LIN-UART 9 TX                 
	sw_reset();
}	

#endif

__interrupt 
void irq_flash(void)
{
	ErrorLevel = 110; 	// Main Flash IRQ                
	sw_reset();
}	

__interrupt 
void irq_111(void)
{
	ErrorLevel = 111; 	// Reserved                      
	sw_reset();
}	

__interrupt 
void irq_usb_EP0_in(void)
{
	ErrorLevel = 112; 	// USB EP0 IN  (only MB9633xU)   
	sw_reset();
}	

__interrupt 
void irq_usb_EP0_out(void)
{
	ErrorLevel = 113; 	// USB EP0 OUT (only MB9633xU)   
	sw_reset();
}	

__interrupt 
void irq_usb_EP1(void)
{
	ErrorLevel = 114; 	// USB EP1     (only MB9633xU)   
	sw_reset();
}	

__interrupt 
void irq_usb_EP2(void)
{
	ErrorLevel = 115; 	// USB EP2     (only MB9633xU)   
	sw_reset();
}	

__interrupt 
void irq_usb_EP3(void)
{
	ErrorLevel = 116; 	// USB EP3     (only MB9633xU)   
	sw_reset();
}	

__interrupt 
void irq_usb_EP4(void)
{
	ErrorLevel = 117; 	// USB EP4     (only MB9633xU)   
	sw_reset();
}	

__interrupt 
void irq_usb_EP5(void)
{
	ErrorLevel = 118; 	// USB EP5     (only MB9633xU)   
	sw_reset();
}	

__interrupt 
void irq_usb_Function1(void)
{
	ErrorLevel = 119; 	// USB Function Flag 1 (only MB9633xU)  
	sw_reset();
}	

__interrupt 
void irq_usb_Function2(void)
{
	ErrorLevel = 120; 	// USB Function Flag 2 (only MB9633xU)  
	sw_reset();
}	

__interrupt 
void irq_usb_MiniHost1(void)
{
	ErrorLevel = 121; 	// USB MiniHost Flag 1 (only MB9633xU)  
	sw_reset();
}	

__interrupt 
void irq_usb_MiniHost2(void)
{
	ErrorLevel = 122; 	// USB MiniHost Flag 2 (only MB9633xU)  
	sw_reset();
}	



