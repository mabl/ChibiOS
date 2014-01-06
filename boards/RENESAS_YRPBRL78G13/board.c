/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

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
 * @brief   Option bytes setup.
 */
const uint8_t OptionBytes[] __attribute__ ((section (".option"))) = {
  0x60,    /* Watchdog disabled */
  0x1F,
  0xE8, /* HS, 32MHz */
#if 0
  0xE0, /* HS, 14MHz */
  0xE9, /* HS, 16MHz */
  0xE1, /* HS, 12MHz */
  0xEA, /* HS, 8MHz */
  0xEB, /* HS, 4MHz */
  0xED, /* HS, 1MHz */
#endif
  0x04  /* On Chip Debug disabled */
};

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config = {
 {VAL_PORT0DATA, VAL_PORT0DIR},
};
#endif

/**
 * @brief   TM00 interrupt handler.
 */
CH_IRQ_HANDLER(INT_TM00) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();
  TMIF00 = 0;

  CH_IRQ_EPILOGUE();
}

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /* Setup tick timer */
  TAU0EN = 1;
  TPS0 = 5;      /* presc=32, freq=1MHz */
  TDR00 = 1000;  /* hardcoded 1KHz */
/*  TDR00 = RL78_HSOCOCLK / 32 / CH_FREQUENCY; */
  TMR00 = 0;
  TMIF00 = 0;
  TMMK00 = 0;    /* enable interrupt */
  TS0 = 1;       /* timer start */
}
