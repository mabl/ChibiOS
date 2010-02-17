/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file    SPC563/serial_lld.c
 * @brief   SPC563 low level serial driver code.
 *
 * @addtogroup SPC563_SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   eSCI-A serial driver identifier.
 */
#if USE_SPC563_ESCIA || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   eSCI-B serial driver identifier.
 */
#if USE_SPC563_ESCIB || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
  SERIAL_DEFAULT_BITRATE
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   eSCI initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void esci_init(SerialDriver *sdp) {
  volatile struct ESCI_tag *escip = sdp->escip;

}

/**
 * @brief   eSCI de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] escip     pointer to an eSCI I/O block
 */
static void esci_deinit(volatile struct ESCI_tag *escip) {

}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] sr        eSCI SR register value
 */
static void set_error(SerialDriver *sdp, uint32_t sr) {
  sdflags_t sts = 0;

/*  if (sr & USART_SR_ORE)
    sts |= SD_OVERRUN_ERROR;
  if (sr & USART_SR_PE)
    sts |= SD_PARITY_ERROR;
  if (sr & USART_SR_FE)
    sts |= SD_FRAMING_ERROR;
  if (sr & USART_SR_NE)
    sts |= SD_NOISE_ERROR;
  if (sr & USART_SR_LBD)
    sts |= SD_BREAK_DETECTED;*/
  chSysLockFromIsr();
  sdAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

/**
 * @brief   Common IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void serve_interrupt(SerialDriver *sdp) {
  volatile struct ESCI_tag *escip = sdp->escip;

}

#if USE_SPC563_ESCIA || defined(__DOXYGEN__)
static void notify1(void) {

}
#endif

#if USE_SPC563_ESCIB || defined(__DOXYGEN__)
static void notify2(void) {

}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_SPC563_ESCIA || defined(__DOXYGEN__)
/**
 * @brief   eSCI-A interrupt handler.
 */
CH_IRQ_HANDLER(vector146) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD1);

  CH_IRQ_EPILOGUE();
}
#endif

#if USE_SPC563_ESCIB || defined(__DOXYGEN__)
/**
 * @brief   eSCI-B interrupt handler.
 */
CH_IRQ_HANDLER(vector149) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD2);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 */
void sd_lld_init(void) {

#if USE_SPC563_ESCIA
  sdObjectInit(&SD1, NULL, notify1);
  SD1.escip = &ESCI_A;
#endif

#if USE_SPC563_ESCIB
  sdObjectInit(&SD2, NULL, notify2);
  SD2.escip = &ESCI_B;
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
void sd_lld_start(SerialDriver *sdp) {

  if (sdp->config == NULL)
    sdp->config = &default_config;

  if (sdp->state == SD_STOP) {
#if USE_SPC563_ESCIA
    if (&SD1 == sdp) {
    }
#endif
#if USE_SPC563_ESCIB
    if (&SD2 == sdp) {
    }
#endif
  }
  esci_init(sdp);
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the eSCI, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
    esci_deinit(sdp->escip);
#if USE_SPC563_ESCIA
    if (&SD1 == sdp) {
      return;
    }
#endif
#if USE_SPC563_ESCIB
    if (&SD2 == sdp) {
      return;
    }
#endif
  }
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */
