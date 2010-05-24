/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "global.h"
#include "f2mc_16fxs.h"

#include <ch.h>
#include <hal.h>

#include "appboard.h"

void pin_init(void);


/*
 * Hardware initialization goes here.
 * NOTE: Interrupts are still disabled.
 */
void app_hwinit(void) 
{
int irq;

	pin_init();

	InitIrqLevels();

	DIRR = 0;		// reset delay irq req

	/* init reload timer 0 & 1 per treno impulsi stepper 
	*/
	TMRLR0 = uSEC_TO_RELTIM0(20000);
	TMCSR0 = 0x181b;						// seleziona clk = clkp1 / 32

	irq = 51;   /* RLT0                         */
    ICR = (irq << 8) | 0x02;	// livello irq = 2

	TMRLR1 = uSEC_TO_RELTIM1(20000);
	TMCSR1 = 0x181b;						// seleziona clk = clkp1 / 32

	irq = 52;   /* RLT1                         */
    ICR = (irq << 8) | 0x02;  


	/* init reload timer 2 & 3 per treno impulsi stepper 
	*/
	TMRLR2 = uSEC_TO_RELTIM2(10000);						
	TMCSR2 = 0x041b;					// seleziona clk = clkp1 / 16

	irq = 53;   /* RLT2                         */
    ICR = (irq << 8) | 0x02;			// livello irq = 2

	TMRLR3 = uSEC_TO_RELTIM3(10000);
	TMCSR3 = 0x041b;					// seleziona clk = clkp1 / 16

	irq = 54;   /* RLT3                         */
    ICR = (irq << 8) | 0x02;  


	/* init reload timer 6 come tick di sistema
	*/
	TMRLR6 = uSEC_TO_RELTIM0(1000);
	TMCSR6 = 0x181b;					// seleziona clk = clkp1 / 32

	irq = 55;   /* PPGRLT - RLT6                */
    ICR = (irq << 8) | CH_IRQ_PRIORITY;	// livello irq = come richiesto

	/* init uart3 interrupt levels
	*/
	irq = 83;   /* UART2_RX                */
    ICR = (irq << 8) | CH_IRQ_PRIORITY;	// livello irq = come richiesto

	irq = 84;   /* UART2_TX                */
    ICR = (irq << 8) | CH_IRQ_PRIORITY;	// livello irq = come richiesto


/*
irq_uart0_rx			79  
irq_uart0_tx			80  
irq_uart1_rx			81  
irq_uart1_tx			82  
irq_uart2_rx			83  
irq_uart2_tx			84  
irq_uart3_rx			85  
irq_uart3_tx			86  
*/


}


// Inizializza alla bell'emeglio i pinozzi
void pin_init(void)
{

	// ****************  PORT 0  ***********************

	// gpio 
	/*
		P00	-> AD0	
		P01 -> AD1	
		P02 -> AD2  
		P03 -> AD3 	
		P04 -> AD4  
		P05 -> AD5 	
		P06 -> AD6  
		P07 -> AD7  
	*/    									
	/* Data register  		(0000-0000)  */
    PDR00 = 0x00;		

	/* Direction register  	(0000-0000)  */   
	DDR00 = 0x00;

	/* Input enable register (1111-1111)  */
    PIER00 = 0xFF;		

	/* Input level register (0000-0000)  */
    PILR00 = 0x00;		
										
	/* Extended Input level register (1111-1111) tutti in pin a livello TTL */
    EPILR00 = 0xFF;		

	/* Output drive register (0000-0000)  -> tutti in normal drive */
    PODR00 = 0x00;

	/* High drive register  (0000-0000)  -> tutti in no-high drive */
//    PHDR00 = 0x00;		

	/* Pull-up register 	(0000-0000)  */
	PUCR00 = 0x00;		
    					
	EBAE0 = 0xFF;			// abilita l'uso di P00-P07 come AD00-AD07


	// ****************  PORT 1  ***********************

	// address/data bus 
	/*
		P01_0 -> P01_0
		P01_1 -> P01_1	
		P01_2 -> P01_2 
		P01_3 -> P01_3
		P01_4 -> P01_4
		P01_5 -> P01_5
		P01_6 -> P01_6
		P01_7 -> P01_7
	*/    									
	/* Data register  		(0000-0010)  */
    PDR01 = 0x00;		

	/* Direction register  	(0001-1010)  */   
	DDR01 = 0x00;

	/* Input enable register (1111-1111)  */
    PIER01 = 0xFF;		

	/* Input level register (0000-0000)  */
    PILR01 = 0x00;		
										
	/* Extended Input level register (1111-1111)  tutti in pin a livello TTL  */
    EPILR01 = 0xFF;		

	/* Output drive register (0000-0000)  */
    PODR01 = 0x00;

	/* High drive register  (0000-0000)  */
//    PHDR01 = 0x00;		

	/* Pull-up register 	(1110-0001)  */
	PUCR01 = 0xFF;

	// ****************  PORT 2  ***********************

	// gpio 
	/*
		P02_0 -> P02_0
		P02_1 -> P02_1
		P02_2 -> P02_2
		P02_3 -> P02_3
		P02_4 -> P02_4
		P02_5 -> P02_5
		P02_6 -> P02_6
		P02_7 -> P02_7
	*/    									
	/* Data register  		(0001-0100)  */
    PDR02 = 0x00;		

	/* Direction register  	(0011-0100)  */   
	DDR02 = 0x00;

	/* Input enable register (0011-1111)  */
    PIER02 = 0xFF;		

	/* Input level register (0000-0000)  */
    PILR02 = 0x00;		
										
	/* Extended Input level register (1111-1111) tutti in pin a livello TTL  */
    EPILR02 = 0xFF;

	/* Output drive register (0000-0000)  */
    PODR02 = 0x00;

	/* High drive register  (0000-0000)  */
    PODR02 = 0x00;		

	/* Pull-up register 	(0010-0000)  */
	PUCR02 = 0x23;		

	// ****************  PORT 3  ***********************

	/* gpio
		P03_0 -> P03_0
		P03_1 -> P03_1
		P03_2 -> P03_2
		P03_3 -> P03_3
		P03_4 -> P03_4
		P03_5 -> P03_5
		P03_6 -> P03_6
		P03_7 -> P03_7
	*/    									
	/* Data register  		(0000-0000)  */
    PDR03 = 0x00;		

	/* Direction register  	(1011-0111)  */   
	DDR03 = 0x00;

	/* Input enable register (1111-1111)  */
    PIER03 = 0xFF;		

	/* Input level register (0000-0000)  */
    PILR03 = 0x00;		
										
	/* Extended Input level register (1111-1111) tutti in pin a livello TTL  */
    EPILR03 = 0xFF;		

	/* Output drive register (0000-0000)  */
    PODR03 = 0x00;

	/* High drive register  (0000-0000)  */
    PODR03 = 0x00;		

	/* Pull-up register 	(0000-0000)  */
	PUCR03 = 0x00;		



	// ****************  PORT 4  ***********************

	/* gpio
		P04_0 -> 
		P04_1 -> 
		P04_2 -> CAN_RX
		P04_3 -> CAN_TX
		P04_4 -> SDA0
		P04_5 -> SCL0
		P04_6 -> 
		P04_7 -> 
	*/    									
	/* Data register  		(0011-1100)  */
    PDR04 = 0x3C;		

	/* Direction register  	(0010-1000)  */   
	DDR04 = 0x28;

	/* Input enable register (0011-1100)  */
    PIER04 = 0x3C;		

	/* Input level register (0000-0000)  */
    PILR04 = 0x00;		
										
	/* Extended Input level register (1111-1111) tutti in pin a livello TTL  */
    EPILR04 = 0xFF;		

	/* Output drive register (0000-0000)  */
    PODR04 = 0x00;

	/* High drive register  (0000-0000)  */
    PODR04 = 0x00;		

	/* Pull-up register 	(0011-1100)  */
	PUCR04 = 0x3C;		

	// ****************  PORT 5  ***********************

	/*
		P05_0 -> SIN2
		P05_1 -> SOT2
		P05_2 -> SCK2
		P05_3 -> KEY3
		P05_4 -> KEY2
		P05_5 -> KEY1
		P05_6 -> KEY0
		P05_7 -> 
	*/    									
	/* Data register  		(0000-0000)  */
    PDR05 = 0x00;		

	/* Direction register  	(0000-0110)  */   
	DDR05 = 0x06;

	/* Input enable register (0111-1111)  */
    PIER05 = 0x00;		

	/* Input level register (0000-0000)  */
    PILR05 = 0x00;		
										
	/* Extended Input level register (1111-1111) tutti in pin a livello TTL  */
    EPILR05 = 0xFF;		

	/* Output drive register (0000-0000)  */
    PODR05 = 0x00;

	/* High drive register  (0000-0000)  */
    PODR05 = 0x00;		

	/* Pull-up register 	(0000-0110)  */
	PUCR05 = 0x06;		


                
	// ****************  PORT 6  ***********************

	/* gpio
		P06_0 -> P06_0
		P06_1 -> P06_1
		P06_2 -> P06_2
		P06_3 -> P06_3
		P06_4 -> P06_4
		P06_5 -> P06_5
		P06_6 -> P06_6
		P06_7 -> P06_7
	*/
	
	/* Data register  		(0000-0101)  */
    PDR06 = 0x00;		

	/* Direction register  	(1000-1111)  */   
	DDR06 = 0x00;

	/* Input enable register (1000-0111)  */
    PIER06 = 0xFF;		

	/* Input level register (0000-0000)  */
    PILR06 = 0x00;		
										
	/* Extended Input level register (1111-1111) tutti in pin a livello TTL  */
    EPILR06 = 0xFF;		

	/* Output drive register (0000-0000)  */
    PODR06 = 0x00;

	/* High drive register  (0000-0000)  */
    PODR06 = 0x00;		

	/* Pull-up register 	(0000-0000)  */
	PUCR06 = 0x00;


}


__interrupt void irq_reload6_ppgrlt(void)
{
	CH_IRQ_PROLOGUE();

	chSysLockFromIsr();
	chSysTimerHandlerI();
	chSysUnlockFromIsr();

	TMCSR6_UF = 0;

	CH_IRQ_EPILOGUE();
}


