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

/**
 * @file H8S/serial_lld.c
 * @brief H8S low level serial driver code.
 * @addtogroup H8S_SERIAL
 * @{
 */



#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief UART1 serial driver identifier.*/
#if USE_H8S_UART1 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/** @brief UART2 serial driver identifier.*/
#if USE_H8S_UART2 || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config =
{
  .sc_speed = SERIAL_DEFAULT_BITRATE,
  .sc_smr = 0,
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

CH_IRQ_HANDLER(UART1EriHandler);
CH_IRQ_HANDLER(UART1RxiHandler);
CH_IRQ_HANDLER(UART1TxiHandler);
CH_IRQ_HANDLER(UART2EriHandler);
CH_IRQ_HANDLER(UART2RxiHandler);
CH_IRQ_HANDLER(UART2TxiHandler);

/**
 * @brief UART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
static void uart_init(SerialDriver *sdp) {

  uint32_t b;

//  b = F_CPU*10/(32L * (sdp->config->sc_speed + 1));
//  if ((b % 10) >= 5)
//    b += 10;
//  b /= 10;
  b = F_CPU/(32L * (sdp->config->sc_speed + 1));

#if USE_H8S_UART1 || defined(__DOXYGEN__)
  if (sdp == &SD1)
  {
    sdp->uart->scr = 0;
    sdp->uart->smr = sdp->config->sc_smr;
    sdp->uart->ssr = 0;
    sdp->uart->brr = b;

    *(volatile uint32_t *)(SCI_ERI0) = (uint32_t)UART1EriHandler;
    *(volatile uint32_t *)(SCI_RXI0) = (uint32_t)UART1RxiHandler;
    *(volatile uint32_t *)(SCI_TXI0) = (uint32_t)UART1TxiHandler;

    sdp->uart->scr = _BV(RE)|_BV(TE)|_BV(RIE);
  }
#endif

#if USE_H8S_UART2 || defined(__DOXYGEN__)
#error CODE NOT TESTED
  if (sdp == &SD2)
  {
    sdp->uart->scr = 0;
    sdp->uart->smr = sdp->config->sc_smr;
    sdp->uart->ssr = 0;
    sdp->uart->brr = b;

    *(volatile uint32_t *)(SCI_ERI1) = (uint32_t)UART2EriHandler;
    *(volatile uint32_t *)(SCI_RXI1) = (uint32_t)UART2RxiHandler;
    *(volatile uint32_t *)(SCI_TXI1) = (uint32_t)UART2TxiHandler;

    sdp->uart->scr = _BV(RE)|_BV(TE)|_BV(RIE);
  }
#endif

}

/**
 * @brief UART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
static void uart_deinit(SerialDriver *sdp) {

#if USE_H8S_UART1 || defined(__DOXYGEN__)
  if (sdp == &SD1)
  {
    sdp->uart->scr = 0;
  }
#endif

#if USE_H8S_UART2 || defined(__DOXYGEN__)
  if (sdp == &SD2)
  {
    sdp->uart->scr = 0;
  }
#endif

}

#if USE_H8S_UART1 || defined(__DOXYGEN__)
static void uart_notify1(void) {

    SD1.uart->scr = _BV(RE)|_BV(TE)|_BV(RIE)|_BV(TIE);
}
#endif

#if USE_H8S_UART2 || defined(__DOXYGEN__)
static void uart_notify2(void) {

    SD2.uart->scr = _BV(RE)|_BV(TE)|_BV(RIE)|_BV(TIE);
}
#endif

/**
 * @brief Error handling routine.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 * @param[in] sr UART SR register value
 */
static void set_error(SerialDriver *sdp, uint8_t ssr) {
  sdflags_t sts = 0;

//  if (ssr & _BV(BER))
//    sts |= SD_BREAK_DETECTED;
  if (ssr & _BV(ORER))
    sts |= SD_OVERRUN_ERROR;
  if (ssr & _BV(PER))
    sts |= SD_PARITY_ERROR;
  if (ssr & _BV(FER))
    sts |= SD_FRAMING_ERROR;
  chSysLockFromIsr();
  sdAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();

}

/**
 * @brief ERI IRQ handler.
 *
 * @param[in] sdp pointer to a @p UART1 SerialDriver object
 */
void serve_eri_interrupt(SerialDriver *sdp)
{
  uint8_t ssr = sdp->uart->ssr;

// TODO: test it
  if (ssr & (0 | _BV(FER) | _BV(PER) | _BV(ORER))) {
    set_error(sdp, ssr);
  }
}

/**
 * @brief RXI IRQ handler.
 *
 * @param[in] sdp pointer to a @p UART1 SerialDriver object
 */
void serve_rxi_interrupt(SerialDriver *sdp)
{
  chSysLockFromIsr();
  sdp->uart->ssr &= ~_BV(RDRF);
  sdIncomingDataI(sdp, (uint8_t)sdp->uart->rdr);
  chSysUnlockFromIsr();
}

/**
 * @brief TXI IRQ handler.
 *
 * @param[in] sdp pointer to a @p UART1 SerialDriver object
 */
void serve_txi_interrupt(SerialDriver *sdp)
{
  msg_t b;

  chSysLockFromIsr();
  b = chOQGetI(&sdp->oqueue);
  if (b < Q_OK) {
    chEvtBroadcastI(&sdp->oevent);
    sdp->uart->scr = _BV(RE)|_BV(TE)|_BV(RIE);
  } else {
    sdp->uart->tdr = b;
    sdp->uart->ssr &= ~_BV(TDRE);
  }
  chSysUnlockFromIsr();
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_H8S_UART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(UART1EriHandler) {

	CH_IRQ_PROLOGUE();

	serve_eri_interrupt(&SD1);

	CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(UART1RxiHandler) {

	CH_IRQ_PROLOGUE();

	serve_rxi_interrupt(&SD1);

	CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(UART1TxiHandler) {

	CH_IRQ_PROLOGUE();

	serve_txi_interrupt(&SD1);

	CH_IRQ_EPILOGUE();
}
#endif

#if USE_H8S_UART2 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(UART2EriHandler) {

	CH_IRQ_PROLOGUE();

	serve_eri_interrupt(&SD2);

	CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(UART2RxiHandler) {

	CH_IRQ_PROLOGUE();

	serve_rxi_interrupt(&SD2);

	CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(UART2TxiHandler) {

	CH_IRQ_PROLOGUE();

	serve_txi_interrupt(&SD2);

	CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * Low level serial driver initialization.
 */
void sd_lld_init(void) {

#if USE_H8S_UART1
  sdObjectInit(&SD1, NULL, uart_notify1);
  SD1.uart = (h8s_uart *)0xFFFFFF78;
#endif

#if USE_H8S_UART2
  sdObjectInit(&SD2, NULL, uart_notify2);
  SD2.uart = (h8s_uart *)0xFFFFFF80;
#endif

}

/**
 * @brief Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
void sd_lld_start(SerialDriver *sdp) {

  if (sdp->config == NULL)
    sdp->config = &default_config;
  if (sdp->state == SD_STOP) {
	  uart_init(sdp);
  }
}

/**
 * @brief Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
    uart_deinit(sdp);
  }
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */
