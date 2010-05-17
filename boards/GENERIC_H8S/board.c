
#include "ch.h"
#include "hal.h"

#include "board.h"

CH_IRQ_HANDLER(SYSIrqHandler) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  TCSRTCNT = 0x5A00; // watchdog reset
  TPU_TSR1 &= ~TSR_TGFA;
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
//  palClearPad(IOPORT2, GPIO_LED);
//  palSetPad(IOPORT2, GPIO_LED);
//  palSetPadMode(IOPORT2, GPIO_LED, PAL_MODE_OUTPUT_PUSHPULL);

  /*
   * Disable interrupts from TPU.
   */
  TPU_TIER1 = 0;

  /*
   * Timer 1 setup.
   */
  TPU_TSTR &= ~TSTR_T1;                                     // stop timer
  TPU_TCNT1 = 0x0000;

  *(uint32_t *)(TPU_TGI1A) = (uint32_t)SYSIrqHandler;       // init TPU1 interrupt vector

  TPU_TCR1 = TCR_PRESC64|TCR_TGRA;                          // fi/64, compare match TGRA
  TPU_TIOR1 = 0x00;                                         // output compare disabled
  TPU_TMDR1 = 0x00;                                         // normal operation
  TPU_TGRA1 = F_CPU/64/CH_FREQUENCY;                        // compare match on GRA	- 1kHz
  TPU_TIER1 = TIER_TGRA;                                    // interrupt on compare match A
  TPU_TSTR |= TSTR_T1;                                      // start TPU1

  /*
   * Other initializations.
   */
  chSysInit();
}
