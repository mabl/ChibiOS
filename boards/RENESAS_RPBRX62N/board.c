/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config = {
 {VAL_GPIO0DATA, VAL_GPIO0DIR},
 {VAL_GPIO1DATA, VAL_GPIO1DIR},
 {VAL_GPIO2DATA, VAL_GPIO2DIR},
 {VAL_GPIO3DATA, VAL_GPIO3DIR},
};
#endif

/**
 * @brief   CMT3 interrupt handler.
 */
CH_IRQ_HANDLER(Excep_CMTU1_CMT3) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  rx62n_clock_init();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /* Setup tick timer */
  MSTP(CMT3) = 0;             /* Enable CMT3 timer (cancel module stop state) */
  CMT.CMSTR1.BIT.STR3 = 0;    /* Stop timer */
  CMT3.CMCR.BIT.CKS = 2;      /* PCLK/128 */
  CMT3.CMCNT = 0;
  CMT3.CMCOR = RX62N_PERCLK / 128 / CH_FREQUENCY;
  CMT3.CMCR.BIT.CMIE = 1;     /* Enable compare match interrupts */
  IEN(CMT3,CMI3) = 1;         /* Set Interrupt Enable Register */
  IPR(CMT3,CMI3) = 8;         /* Setup interrupt priority */
  CMT.CMSTR1.BIT.STR3 = 1;    /* Start timer */
}
