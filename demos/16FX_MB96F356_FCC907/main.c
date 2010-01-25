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
#include "appboard.h"

#include <ch.h>
#include <hal.h>

//#include <chrset.h>
//#include <graph.h>


struct stepper_t{
    long 		count;              // actual position
    long 		endCount;           // target position
	word		wDeltaStep;							// delta inizio movimento: 0

	bool		pleaseStop;

	word (*pStepperClock)(struct stepper_t *pmot);
};



/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
//static WORKING_AREA(waThread1, 64);

static stkalign_t waThreadStepper1[WORKING_SIZE(64)];
static stkalign_t waThreadStepper2[WORKING_SIZE(64)];
static stkalign_t waThreadWatchDog[WORKING_SIZE(32)];

static byte pulseForwBuf[256];		// ram-copy of stepping functions ... only to test execution time between ram & flash
static byte pulseBackBuf[256];

long timerCounter0;
long timerCounter1;
long timerCounter2;
long timerCounter3;

long totCicli;
int delayval;

struct stepper_t stepMotor1;
struct stepper_t stepMotor2;

static msg_t ThreadStepper1(void *arg);
static msg_t ThreadStepper2(void *arg);
static msg_t ThreadWatchdog(void *arg);

word stepper_clockPulseForw(struct stepper_t *pmot);
word stepper_clockPulseBack(struct stepper_t *pmot);
void far_memcpy(__far void *dst, __far void *src, int size);
void delay();


/*
 * Entry point, the interrupts are disabled on entry.
 */
int main(int argc, char **argv) 
{
word testa = 0;
word testb = 0;
word testc = 0;
int i, n;
byte c;

  /*
   * Hardware initialization, see board.c.
   */
	hwinit();			// almost done by start.asm module

  	app_hwinit();

	far_memcpy( ( __far void*) &pulseForwBuf, &stepper_clockPulseForw, sizeof(pulseForwBuf));
	far_memcpy(( __far void*) &pulseBackBuf, &stepper_clockPulseBack, sizeof(pulseBackBuf));

  
  /*
   * The main() function becomes a thread here then the interrupts are
   * enabled and ChibiOS/RT goes live.
   */
  chSysInit();

  /*
   * Creates the stepper thread.
   */
  chThdCreateStatic(waThreadStepper1, sizeof(waThreadStepper1), NORMALPRIO, ThreadStepper1, NULL);
  chThdCreateStatic(waThreadStepper2, sizeof(waThreadStepper2), NORMALPRIO, ThreadStepper2, NULL);
  chThdCreateStatic(waThreadWatchDog, sizeof(waThreadWatchDog), HIGHPRIO, ThreadWatchdog, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
	while (TRUE) 
	{
        PinBK_EN = 1;
        
		chThdSleepMilliseconds(250);

        PinBK_EN = 0;

		chThdSleepMilliseconds(500);
	}

  	
  return 0;
}


void far_memcpy(__far void *dst, __far void *src, int size)
{
__far byte *pd;
__far byte *ps;

	pd = (__far byte *) dst;
	ps = (__far byte *) src;
	
	while( size )
	{
		*pd++ = *ps++;
		size--;
	}
}

static msg_t ThreadStepper1(void *arg) 
{
struct stepper_t *pMot = &stepMotor1;
bool exit = False;

	while( !exit ) 
	{
    	chThdSleepMilliseconds(100);

  		PinEnStep1 = 1;
		PinDirStep1 = 0;

		pMot->count = 0;              			// contatore passi eseguiti
		pMot->endCount = 500;              		// posizione finale da raggiungere

//		pStepperClock1 = stepper_clockPulseForw;
		pMot->pStepperClock = pulseForwBuf;

		do{
	    	chThdSleepMilliseconds(100);

		}while( pMot->pStepperClock );

		PinDirStep1 = 1;

    	chThdSleepMilliseconds(100);

		pMot->endCount = 0;              		// posizione finale da raggiungere
//		pStepperClock1 = stepper_clockPulseBack;
		pMot->pStepperClock = pulseBackBuf;

		do{
	    	chThdSleepMilliseconds(100);

		}while( pMot->pStepperClock );

		totCicli++;

	}
	
	return 0;
}



static msg_t ThreadStepper2(void *arg) 
{
long  countTh2 = 0;
struct stepper_t *pMot = &stepMotor2;

	while (TRUE) 
	{
    	chThdSleepMilliseconds(100);

  		PinEnStep1 = 1;
		PinDirStep2 = 0;

		pMot->count = 0;              			// contatore passi eseguiti
		pMot->endCount = 300;              		// posizione finale da raggiungere
		pMot->pStepperClock = stepper_clockPulseForw;

		do{
	    	chThdSleepMilliseconds(100);

		}while( pMot->pStepperClock );

		PinDirStep1 = 1;

    	chThdSleepMilliseconds(100);

		pMot->endCount = 0;              		// posizione finale da raggiungere
		pMot->wDeltaStep = 0;							// delta inizio movimento: 0
		pMot->pStepperClock = stepper_clockPulseBack;

		do{
	    	chThdSleepMilliseconds(100);

		}while( pMot->pStepperClock );

		countTh2++;
	}
  	return 0;
}


static msg_t ThreadWatchdog(void *arg) 
{
long WatchTheDog;

	for(;;)
	{
		/**** Trigger the watchdog every 200 msec ****/
		//    WDTC_WTE = 0;

		WatchTheDog++;

		chThdSleepMilliseconds(200);
	}
}


/****************************************************************************/
__interrupt void irq_reload0(void)
{

	if( stepMotor1.pStepperClock )
	{
		PinClkStep1 = 0;		// circa 4usec @48Mhz in flash e 3usec in ram 

		if( (TMRLR0 = stepMotor1.pStepperClock(&stepMotor1)) == 0xFFFF )
			stepMotor1.pStepperClock = 0;

		PinClkStep1 = 1;
	}

	TMCSR0_UF = 0;
	timerCounter0++;
}

__interrupt void irq_reload1(void)
{
	if( stepMotor2.pStepperClock )
	{
		PinClkStep2 = 0;		// circa 4usec @48Mhz in flash e 3usec in ram 

		if( (TMRLR1 = stepMotor2.pStepperClock(&stepMotor2)) == 0xFFFF )
			stepMotor2.pStepperClock = 0;

		PinClkStep2 = 1;
	}
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


/* Genera un impulso di clock avanti per lo stepper 1

   nota: viene sempre chiamata sul fronte di discesa del clock per lo stepper
*/   
word stepper_clockPulseForw(struct stepper_t *pMot)
{
word reloadValue = 1000;

    pMot->count++;     					// piu 1 

	if( pMot->count == pMot->endCount || pMot->pleaseStop )		// fine stepping ?
	{
		reloadValue = 0xFFFF;
		return reloadValue;
	}

	return reloadValue;
}


word stepper_clockPulseBack(struct stepper_t *pMot)
{
bool accelerate;
word reloadValue = 1000;


    pMot->count--;     					// meno 1 

	if( pMot->count == pMot->endCount || pMot->pleaseStop )		// fine stepping ?
	{
		reloadValue = 0xFFFF;
		return reloadValue;
	}

	return reloadValue;

}




