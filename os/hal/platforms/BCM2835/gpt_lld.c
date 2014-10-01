/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
*/

/**
 * @file    templates/gpt_lld.c
 * @brief   GPT Driver subsystem low level driver source template.
 *
 * @addtogroup GPT
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD1 driver identifier.
 * @note    The driver GPTD1 allocates the complex timer TIMER1 when enabled.
 */
#if BCM2835_GPT_USE_TIMER1 || defined(__DOXYGEN__)
GPTDriver GPTD1;
#endif

/**
 * @brief   GPTD2 driver identifier.
 * @note    The driver GPTD2 allocates the timer TIMER2 when enabled.
 */
#if BCM2835_GPT_USE_TIMER2 || defined(__DOXYGEN__)
GPTDriver GPTD2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void gpt_lld_serve_driver_interrupt(GPTDriver *gptp) {
  uint32_t state = gptp->state;
  if (state == GPT_CONTINUOUS || state == GPT_ONESHOT) {
    uint32_t match_mask = gptp->match_mask;
    if (SYSTIMER_CS & match_mask) {
      uint32_t period = gptp->period;
      if (gptp->state == GPT_CONTINUOUS && period > 0) {
	*(gptp->compare) += period;
	SYSTIMER_CS |= match_mask;
	gptp->config->callback(&GPTD1);
      }
      else {
	SYSTIMER_CS |= match_mask;
	IRQ_DISABLE1 |= gptp->irq_mask;
	gptp->config->callback(&GPTD1);
	gptp->state = GPT_READY;
      }
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 */
void gpt_lld_serve_interrupt(void) {
  if ((IRQ_PEND1 & 0x0F) == 0) return;

#if BCM2835_GPT_USE_TIMER1
  gpt_lld_serve_driver_interrupt(&GPTD1);
#endif

#if BCM2835_GPT_USE_TIMER2
  gpt_lld_serve_driver_interrupt(&GPTD2);
#endif
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GPT driver initialization.
 * @details It seems that system timers 0 and 2 were being used for other purposes (?).
 *
 * @notapi
 */
void gpt_lld_init(void) {
#if BCM2835_GPT_USE_TIMER1
  /* Driver initialization.*/
  GPTD1.compare = &SYSTIMER_CMP1;
  GPTD1.match_mask = SYSTIMER_CS_MATCH1;
  GPTD1.irq_mask = SYSTIMER_IRQEN1;
  gptObjectInit(&GPTD1);
#endif

#if BCM2835_GPT_USE_TIMER2
  /* Driver initialization.*/
  GPTD2.compare = &SYSTIMER_CMP3;
  GPTD2.match_mask = SYSTIMER_CS_MATCH3;
  GPTD2.irq_mask = SYSTIMER_IRQEN3;
  gptObjectInit(&GPTD2);
#endif

  /* Clear all match bits.*/
  SYSTIMER_CS |= 0x0F;
}

/**
 * @brief   Configures and activates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_start(GPTDriver *gptp) {
  UNUSED(gptp);
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop(GPTDriver *gptp) {
  gptp->period = 0;
  IRQ_DISABLE1 |= gptp->irq_mask;
}

/**
 * @brief   Starts the timer in continuous mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] period    period in ticks
 *
 * @notapi
 */
void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t period) {
  gptp->period = period;
  *(gptp->compare) = SYSTIMER_CLO + period;
  IRQ_ENABLE1 |= gptp->irq_mask;
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop_timer(GPTDriver *gptp) {
  UNUSED(gptp);
  // TODO check if interrupts should be disabled
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 * @details This function specifically polls the timer waiting for completion
 *          in order to not have extra delays caused by interrupt servicing,
 *          this function is only recommended for short delays.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @notapi
 */
void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval) {
  UNUSED(gptp);
  uint32_t compare = SYSTIMER_CLO + interval;
  while (SYSTIMER_CLO < compare);
}

#endif /* HAL_USE_GPT */

/** @} */
