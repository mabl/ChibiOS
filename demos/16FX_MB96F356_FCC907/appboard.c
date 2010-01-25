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
	
//	irq = 12;   /* Delayed Interrupt            */
//    ICR = (irq << 8) | CH_IRQ_PRIORITY;	// livello irq = come da richiesta



	/* init reload timer 0 & 1 per treno impulsi stepper 
	*/
	TMRLR0 = uSEC_TO_RELTIM0(20000);
	TMCSR0 = 0x181b;						// seleziona clk = clkp1 / 32

	irq = 54;   /* RLT0                         */
    ICR = (irq << 8) | 0x02;	// livello irq = 2

	TMRLR1 = uSEC_TO_RELTIM1(20000);
	TMCSR1 = 0x181b;						// seleziona clk = clkp1 / 32

	irq = 55;   /* RLT1                         */
    ICR = (irq << 8) | 0x02;  


	/* init reload timer 2 & 3 per treno impulsi stepper 
	*/
	TMRLR2 = uSEC_TO_RELTIM2(10000);						
	TMCSR2 = 0x041b;					// seleziona clk = clkp1 / 16

	irq = 56;   /* RLT2                         */
    ICR = (irq << 8) | 0x02;			// livello irq = 2

	TMRLR3 = uSEC_TO_RELTIM3(10000);
	TMCSR3 = 0x041b;					// seleziona clk = clkp1 / 16

	irq = 57;   /* RLT3                         */
    ICR = (irq << 8) | 0x02;  


	/* init reload timer 6 come tick di sistema
	*/
	TMRLR6 = uSEC_TO_RELTIM0(1000);
	TMCSR6 = 0x181b;					// seleziona clk = clkp1 / 32

	irq = 58;   /* PPGRLT - RLT6                */
    ICR = (irq << 8) | CH_IRQ_PRIORITY;	// livello irq = come richiesto

}


// Inizializza alla bell'emeglio i pinozzi
void pin_init(void)
{

	// ****************  PORT 0  ***********************

	// address/data bus 
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
    PODR00 = 0x00;		

	/* Pull-up register 	(0000-0000)  */
	PUCR00 = 0x00;		
    					
	EBAE0 = 0xFF;			// abilita l'uso di P00-P07 come AD00-AD07


	// ****************  PORT 1  ***********************

	// address/data bus 
	/*
		P01_0 -> AD8
		P01_1 -> AD9_CS_MMC	
		P01_2 -> AD10_SIN3  
		P01_3 -> AD11_SOT3
		P01_4 -> AD12_SCK3 
		P01_5 -> AD13	
		P01_6 -> AD14 
		P01_7 -> AD15 
	*/    									
	/* Data register  		(0000-0010)  */
    PDR01 = 0x02;		

	/* Direction register  	(0001-1010)  */   
	DDR01 = 0x1A;

	/* Input enable register (1111-1111)  */
    PIER01 = 0xFF;		

	/* Input level register (0000-0000)  */
    PILR01 = 0x00;		
										
	/* Extended Input level register (1111-1111)  tutti in pin a livello TTL  */
    EPILR01 = 0xFF;		

	/* Output drive register (0000-0000)  */
    PODR01 = 0x00;

	/* High drive register  (0000-0000)  */
    PODR01 = 0x00;		

	/* Pull-up register 	(1110-0001)  */
	PUCR01 = 0xFF;

	EBAE1 = 0x00;			// abilita l'uso di P10-P17 come AD08-AD15

	// ****************  PORT 2  ***********************

	// address/data bus 
	/*
		P02_0 -> A16
		P02_1 -> A17	
		P02_2 -> CPU_CLKOUT
		P02_3 -> A19
		P02_4 -> LVDS_CD
		P02_5 -> TOUCH_PULL/LVDS_CE
		P02_6 -> 
		P02_7 -> 
	*/    									
	/* Data register  		(0001-0100)  */
    PDR02 = 0x14;		

	/* Direction register  	(0011-0100)  */   
	DDR02 = 0x34;

	/* Input enable register (0011-1111)  */
    PIER02 = 0x3F;		

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

	EBAE2 = 0x00;			// abilita l'uso di P20-P21 come port

	/* peripheral resource relocation */
	PRRR9 = 0x20;			// CLKOT0_R -> selcted at pin 42


// note relative ai clock necessari alla cpld
// per generare i timing sui display
//
//	input 	_gclkcpu_i;					// global clock dalla cpu per le periferiche
										// in particolare i controller LCD
										// usiamo:
										// a) 32Mhz per il tft 320x240
										// b) 48Mhz per il tft 480x272
										// c) 64Mhz per il tft 800x480
										
//	input 	_gclkbus_i;					// global clock dalla cpu
										// per comodità usiamo:
										// a) 32Mhz quando c'è il tft 320x240
										// b) 24Mhz quando c'è il tft 480x272
										// c) 32Mhz quando c'è il tft 800x480


	COCR0 = 0x06;			// seleziona CLOCK-OUT0 a partire da CLKS1 senza prescaler aggiuntivo

	COAR = 	0x01;			// abilita l'uscita su CLOCK-OUT0 (CKTO0)

	COAR |= 0x04;			// starta l'attivazione di CKOT0

	// ****************  PORT 3  ***********************

	/*
		P03_0 -> CPU_ALE
		P03_1 -> /CPU_RDX
		P03_2 -> /CPU_WRL
		P03_3 -> /CPU_WRH
		P03_4 -> LVDS_CLK
		P03_5 -> LVDS_DATAO
		P03_6 -> CPU_RDY
		P03_7 -> CPU_BLCK
	*/    									
	/* Data register  		(0000-0000)  */
    PDR03 = 0x00;		

	/* Direction register  	(1011-0111)  */   
	DDR03 = 0xB7;

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

	/*
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

	/*
		P06_0 -> /CPU_CS0
		P06_1 -> BK_EN
		P06_2 -> /CPU_CS1
		P06_3 -> TOUCH_X1
		P06_4 -> TOUCH_X2
		P06_5 -> TOUCH_Y1
		P06_6 -> TOUCH_Y2
		P06_7 -> BK_PWM
	*/
	
	/* Data register  		(0000-0101)  */
    PDR06 = 0x00;		

	/* Direction register  	(1000-1111)  */   
	DDR06 = 0x8F;

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

	/* peripheral resource relocation */
	PRRR12 = 0x05;			// CS0_R -> selcted at pin 62 + CS2_R -> selcted at pin 3

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


