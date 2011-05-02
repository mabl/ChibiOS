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

#include <ch.h>
#include <hal.h>
#include <test.h>

#include "appboard.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
//static WORKING_AREA(waThread1, 64);

static stkalign_t waThreadSlowPLC[WORKING_SIZE(64)];
static stkalign_t waThreadFastPLC[WORKING_SIZE(64)];
static stkalign_t waThreadWatchDog[WORKING_SIZE(32)];

long timerCounter0;
long timerCounter1;
long timerCounter2;
long timerCounter3;

long totCicli;
long totCicliSlow;
long totCicliFast;

int delayval;

int plcSlowRun;


static msg_t ThreadSlowPLC(void *arg);
static msg_t ThreadFastPLC(void *arg);
static msg_t ThreadWatchdog(void *arg);


/*
 * Entry point, the interrupts are disabled on entry.
 */
int main(int argc, char **argv) 
{
uint16_t testa = 0;
uint16_t testb = 0;
uint16_t testc = 0;
int i, n;
uint8_t c;

  /*
   * Hardware initialization, see board.c.
   */
	hwinit();			// almost done by start.asm module

  	app_hwinit();
  
    /*
     * The main() function becomes a thread here then the interrupts are
     * enabled and ChibiOS/RT goes live.
     */
    chSysInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
   
//    sdStart(&SD1, NULL);

  /*
   * Creates the plc thread.
   */

//   TestThread(&SD1);
   
    chThdCreateStatic(waThreadSlowPLC, sizeof(waThreadSlowPLC), NORMALPRIO+4, ThreadSlowPLC, NULL);
    chThdCreateStatic(waThreadFastPLC, sizeof(waThreadFastPLC), NORMALPRIO+10, ThreadFastPLC, NULL);
    chThdCreateStatic(waThreadWatchDog, sizeof(waThreadWatchDog), HIGHPRIO, ThreadWatchdog, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
	while (TRUE) 
	{
//        PinLED_LIFE = 1;

		PDR00++;
		
		chThdSleepMilliseconds(250);

		PDR00++;
//        PinLED_LIFE = 0;

		chThdSleepMilliseconds(500);
	}

  	
  return 0;
}

static msg_t ThreadDigInp(void *arg) 
{
int exit = 0;
int blinkDelay = 0;

}


static msg_t ThreadSlowPLC(void *arg) 
{
int exit = 0;
int blinkDelay = 0;

	while( !exit ) 
	{

		if( plcSlowRun == 0x5544 )
		{
	
			blinkDelay++;

			if( blinkDelay >= 10 )
			{
				blinkDelay = 0;
				PinLED_SLOW_PLC = !PinLED_SLOW_PLC;
			}

	    	chThdSleepMilliseconds(50);

			totCicliSlow++;

			while( totCicliSlow == 200 )
			{
				blinkDelay++;

				if( blinkDelay >= 10000 )
				{
					blinkDelay = 0;
					PinLED_SLOW_PLC = !PinLED_SLOW_PLC;
				}
			}
		}
		else
		{
	    	chThdSleepMilliseconds(1000);
		}
	}
	
	return 0;
}



static msg_t ThreadFastPLC(void *arg) 
{
int blinkDelay = 0;

	while( TRUE ) 
	{
		blinkDelay++;

		if( blinkDelay >= 10 )
		{
			blinkDelay = 0;
			PinLED_FAST_PLC = !PinLED_FAST_PLC;
		}
		
    	chThdSleepMilliseconds(10);

		totCicliFast++;

		while( totCicliFast == 600 )
		{
			blinkDelay++;

			if( blinkDelay >= 10000 )
			{
				blinkDelay = 0;
				PinLED_FAST_PLC = !PinLED_FAST_PLC;
			}
		}
		
	}
  	return 0;
}


static msg_t ThreadWatchdog(void *arg) 
{
long WatchTheDog;

long precCicliSlow;
long precCicliFast;

int statoCheck = 0;
int delayWdt;

	precCicliSlow = totCicliSlow;
	precCicliFast = totCicliFast;

	for(;;)
	{


		switch( statoCheck )
		{
			case	0:
				if( (precCicliSlow == totCicliSlow) ||
					(precCicliFast == totCicliFast) )
				{
					statoCheck++;
				}
				else
				{
					precCicliSlow = totCicliSlow;
					precCicliFast = totCicliFast;
				}
				break;

			case	1:
				if( (precCicliSlow == totCicliSlow) ||
					(precCicliFast == totCicliFast) )
				{
					statoCheck++;
					delayWdt = 0;
				}
				else
				{
					precCicliSlow = totCicliSlow;
					precCicliFast = totCicliFast;
					statoCheck = 0;
				}
				break;

			case	2:

				delayWdt++;
				
				if( delayWdt >= 20 )
				{
					totCicliSlow = 1;
					totCicliFast = 1;

					precCicliSlow = totCicliSlow;
					precCicliFast = totCicliFast;

					statoCheck = 0;
				}
				break;
		}

		/**** Trigger the watchdog every 200 msec ****/
		//    WDTC_WTE = 0;

		PinLED_WDOG = !PinLED_WDOG;
		WatchTheDog++;

		chThdSleepMilliseconds(200);
	}
}


/****************************************************************************/
__interrupt void irq_reload0(void)
{

	TMCSR0_UF = 0;
	timerCounter0++;
}

__interrupt void irq_reload1(void)
{
	TMCSR1_UF = 0;

	timerCounter1++;
}


__interrupt void irq_reload2(void)
{
	TMCSR2_UF = 0;

	timerCounter2++;
}

__interrupt void irq_reload3(void)
{
	TMCSR3_UF = 0;

	timerCounter3++;
}


