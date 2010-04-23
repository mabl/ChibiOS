
#include <ch.h>
#include <hal.h>

#include "board.h"

volatile mcf5206e_IMM *sim = (mcf5206e_IMM *)MBAR;

CH_IRQ_HANDLER(SYSIrqHandler) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  sim->timer.TER1 |= mcf5206e_TIMER_TER_REF;
  CH_IRQ_EPILOGUE();
}

/*
 * Board initialization code.
 */
void hwinit(void) {

  /*
   * HAL initialization.
   */
  halInit();

  /*
   * PAL initialization.
   */
//  palClearPad(IOPORT1, PIO_LED);
  palSetPad(IOPORT1, PIO_LED);
  palSetPadMode(IOPORT1, PIO_LED, PAL_MODE_OUTPUT_PUSHPULL);

  /*
   * Disable interrupts.
   */
  sim->sim.IMR = 0xFFFF;

  /*
   * Timer 1 setup.
   */
  sim->timer.TMR1 =	0;										// disable timer
  *(volatile uint32_t *)0x30000074 = (uint32_t)SYSIrqHandler;
  sim->sim.ICR9		=	mcf5206e_SIM_ICR_AVEC|
							mcf5206e_SIM_ICR_IL(5)|
							mcf5206e_SIM_ICR_IP(3);

  sim->timer.TCN1	=	0;
  sim->timer.TRR1 =	200000L/CH_FREQUENCY;					// period = 200 / 200000 = 0.001s
  sim->timer.TER1 =	0xFF;
  sim->timer.TMR1 =	mcf5206e_TIMER_TMR_PS(250-1)|			// prescaler: 50MHz/250=200kHz
									mcf5206e_TIMER_TMR_CE_NONE|	// disable interrupt on capture event
									mcf5206e_TIMER_TMR_ORI|		// enable interrupt upon reaching the reference value (trr)
									mcf5206e_TIMER_TMR_FRR|		// timer count is reset immediately after reaching the reference value
									mcf5206e_TIMER_TMR_CLK_MSCLK|	// input clock source is F_CPU
									mcf5206e_TIMER_TMR_RST;		// enable timer
  sim->sim.IMR &= ~mcf5206e_SIM_IMR_T1;							// enable timer interrupt

  /*
   * Other initializations.
   */
  chSysInit();
}

