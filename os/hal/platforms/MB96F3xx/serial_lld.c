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
 * @file MB96F3xx/serial_lld.c
 * @brief MB96F3xx low level serial driver code.
 * @addtogroup MB96F3xx_SERIAL
 * @{
 */

#include <ch.h>
#include <hal.h>

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if USE_MB96F3xx_USART0 || defined(__DOXYGEN__)
/** @brief USART0 serial driver identifier.*/
SerialDriver SD0;
#endif
#if USE_MB96F3xx_USART1 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD1;
#endif
#if USE_MB96F3xx_USART2 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD2;
#endif
#if USE_MB96F3xx_USART3 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD3;
#endif
#if USE_MB96F3xx_USART4 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD4;
#endif
#if USE_MB96F3xx_USART5 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD5;
#endif
#if USE_MB96F3xx_USART6 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD6;
#endif
#if USE_MB96F3xx_USART7 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD7;
#endif
#if USE_MB96F3xx_USART8 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD8;
#endif



/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config = {
  DEFAULT_USART_BITRATE,
  0x17, 	// 8 Data, No parity, 1 Stop, Clear all errors, Enable Rx+Tx
  0x01		// Async, Internal BRG, Restart, SW reset, SOT output enable
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief USART initialization.
 * @param[in] pUartReg pointer to an UART I/O block
 * @param[in] config the architecture-dependent serial driver configuration
 */
static void usart_init(SerialDriver *sdp) {

	struct _uart_lin_reg *pReg = sdp->reg;
	uint32_t baudr;

	/* remove reset bit */
	pReg->SMR.byte = sdp->config->smr;

	/* clear pending bits */
	pReg->SCR.bit._RXE = 0;
	pReg->SCR.bit._TXE = 0;
	pReg->SCR.bit._CRE = 1;

	pReg->SSR.bit._RIE = 0;
	pReg->SSR.bit._TIE = 0;

	/* Baudrate setting based on CLKP1 domain clock speed */
	baudr = CLKP1_FREQ / sdp->config->speed;
    baudr -= 1;
    pReg->BGR.word = baudr;

	/*  */
	pReg->SCR.byte = sdp->config->scr;

	pReg->SCR.bit._CRE = 1;	// clear error
	pReg->SMR.bit._UPCL = 0;	// software reset uart

	/* interrupt enable	*/
	pReg->SSR.bit._RIE = 1;
	pReg->SSR.bit._TIE = 0;	// rx only, TX is enable on request
}

/**
 * @brief USART de-initialization.
 * @param[in] u pointer to an UART I/O block
 */
static void usart_deinit(SerialDriver *sdp) {

	struct _uart_lin_reg *pReg = sdp->reg;

	/* interrupt disable	*/
	pReg->SSR.bit._RIE = 0;
	pReg->SSR.bit._TIE = 0;

	/* clear pending bits */
	pReg->SCR.bit._RXE = 0;
	pReg->SCR.bit._TXE = 0;
	pReg->SCR.bit._CRE = 1;	// clear error

	pReg->SMR.bit._SOE = 0;		// Tx output disable
	pReg->SMR.bit._SCKE = 0;	// external serial clock input
	pReg->SMR.bit._UPCL = 1;	// software reset uart
}


/**
 * @brief Error handling routine.
 * @param[in] err UART SSR register value
 * @param[in] sdp communication channel associated to the UART
 */
#pragma inline set_error
static void set_error(struct _uart_lin_reg *pReg, SerialDriver *sdp) {
  sdflags_t sts = 0;

  if (pReg->SSR.bit._ORE)
    sts |= SD_OVERRUN_ERROR;
  if (pReg->SSR.bit._PE)
    sts |= SD_PARITY_ERROR;
  if (pReg->SSR.bit._FRE)
    sts |= SD_FRAMING_ERROR;

  chSysLockFromIsr();
  sdAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}


#pragma inline USARTx_IrqRx
static void USARTx_IrqRx(SerialDriver *sdp) {

	struct _uart_lin_reg *pReg = sdp->reg;

	if (pReg->SSR.bit._ORE || pReg->SSR.bit._FRE || pReg->SSR.bit._PE)
	{
		set_error(pReg, sdp);
		pReg->SCR.bit._CRE = 1;			/* clear error flags             */
	}
	else
	{
		chSysLockFromIsr();
		if (chIQPutI(&sdp->iqueue, pReg->DATA) < Q_OK)
			sdAddFlagsI(sdp, SD_OVERRUN_ERROR);
		chEvtBroadcastI(&sdp->ievent);
		chSysUnlockFromIsr();
	}
}

#pragma inline USARTx_IrqTx
static void USARTx_IrqTx(SerialDriver *sdp) {

	msg_t b;

	chSysLockFromIsr();
	b = sdRequestDataI(sdp);
	chSysUnlockFromIsr();
	if (b < Q_OK)
		sdp->reg->SSR.bit._TIE = 0;
	else
		sdp->reg->DATA = b;
}



#pragma inline notifyUSARTx
static void notifyUSARTx(SerialDriver *sdp) {
	struct _uart_lin_reg *pReg = sdp->reg;

	if (!pReg->SSR.bit._TIE) {
		chSysLockFromIsr();
		pReg->DATA = (uint8_t)sdRequestDataI(sdp);
		chSysUnlockFromIsr();
		pReg->SSR.bit._TIE = 1;
	}
}

#if USE_MB96F3xx_USART0 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART0, &SD0);
#endif

#if USE_MB96F3xx_USART1 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART1, &SD1);
#endif

#if USE_MB96F3xx_USART2 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART2, &SD2);
#endif

#if USE_MB96F3xx_USART3 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART3, &SD3);
#endif

#if USE_MB96F3xx_USART4 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART4, &SD4);
#endif

#if USE_MB96F3xx_USART5 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART5, &SD5);
#endif

#if USE_MB96F3xx_USART6 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART6, &SD6);
#endif

#if USE_MB96F3xx_USART7 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART7, &SD7);
#endif

#if USE_MB96F3xx_USART8 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART8, &SD8);
#endif

#if USE_MB96F3xx_USART9 || defined(__DOXYGEN__)
NOTIFY_USART_X(notifyUSART9, &SD9);
#endif




/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_MB96F3xx_USART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART0_IrqRx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD0);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART0_IrqTx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD0);
  CH_IRQ_EPILOGUE();
}
#endif	/* USE_MB96F3xx_USART0 */

#if USE_MB96F3xx_USART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(irq_uart1_rx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD1);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(irq_uart1_tx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD1);
  CH_IRQ_EPILOGUE();
}

#endif	/* USE_MB96F3xx_USART1 */

#if USE_MB96F3xx_USART2 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(irq_uart2_rx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD2);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(irq_uart2_tx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD2);
  CH_IRQ_EPILOGUE();
}

#endif	/* USE_MB96F3xx_USART2 */

#if USE_MB96F3xx_USART3 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(irq_uart3_rx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD3);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(irq_uart3_tx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD3);
  CH_IRQ_EPILOGUE();
}

#endif	/* USE_MB96F3xx_USART0 */

#if USE_MB96F3xx_USART4 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART4_IrqRx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD4);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART4_IrqTx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD4);
  CH_IRQ_EPILOGUE();
}
#endif	/* USE_MB96F3xx_USART0 */

#if USE_MB96F3xx_USART5 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART5_IrqRx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD5);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART5_IrqTx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD5);
  CH_IRQ_EPILOGUE();
}
#endif	/* USE_MB96F3xx_USART0 */

#if USE_MB96F3xx_USART6 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART6_IrqRx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD6);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART6_IrqTx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD6);
  CH_IRQ_EPILOGUE();
}
#endif	/* USE_MB96F3xx_USART0 */

#if USE_MB96F3xx_USART7 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART7_IrqRx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD7);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART7_IrqTx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD7);
  CH_IRQ_EPILOGUE();
}
#endif	/* USE_MB96F3xx_USART0 */

#if USE_MB96F3xx_USART8 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART8_IrqRx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD8);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART8_IrqTx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD8);
  CH_IRQ_EPILOGUE();
}
#endif	/* USE_MB96F3xx_USART0 */

#if USE_MB96F3xx_USART9 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART0_IrqRx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqRx(&SD0);
  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART9_IrqTx) {
  CH_IRQ_PROLOGUE();
  USARTx_IrqTx(&SD9);
  CH_IRQ_EPILOGUE();
}
#endif	/* USE_MB96F3xx_USART0 */



/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * Low level serial driver initialization.
 */
void sd_lld_init(void) {

#if USE_MB96F3xx_USART0
  sdObjectInit(&SD0, (qnotify_t) NULL, notifyUSART0);

  /* I/O pins for USART0.*/
#if defined (__CPU_MB96330_SERIES) || defined (__CPU_MB96340_SERIES)
  SD0.reg = USART0_Base;
  DDR08_D2 = 0;		// set SIN0 as input
  PIER08_IE2 = 1;	// enable port input
#else
#error USART0 unavailable
#endif
#endif

#if USE_MB96F3xx_USART1
  sdObjectInit(&SD1, (qnotify_t) NULL, notifyUSART1);

  /* I/O pins for USART1.*/
#if defined (__CPU_MB96330_SERIES)  || defined (__CPU_MB96340_SERIES)
  SD1.reg = USART1_Base;
  DDR08_D5 = 0;		// set SIN1 as input
  PIER08_IE5 = 1;	// enable port input
#else
#error USART1 unavailable
#endif
#endif

#if USE_MB96F3xx_USART2
  sdObjectInit(&SD2, (qnotify_t) NULL, notifyUSART2);

  /* I/O pins for USART2.*/
#if defined (__CPU_MB96330_SERIES) || defined (__CPU_MB96340_SERIES) || defined (__CPU_MB96350_SERIES)
  SD2.reg = USART2_Base;
  DDR05_D0 = 0;		// set SIN2 as input
  PIER05_IE0 = 1;	// enable port input
#else
#error USART2 unavailable
#endif
#endif

#if USE_MB96F3xx_USART3
  sdObjectInit(&SD3, (qnotify_t) NULL, notifyUSART3);

  /* I/O pins for USART3.*/
#if defined (__CPU_MB96330_SERIES) || defined (__CPU_MB96340_SERIES) || defined (__CPU_MB96350_SERIES)
  SD3.reg = USART3_Base;
  DDR01_D2 = 0;		// set SIN3 as input
  PIER01_IE2 = 1;	// enable port input
#else
#error USART3 unavailable
#endif
#endif

#if USE_MB96F3xx_USART4
  sdObjectInit(&SD4, (qnotify_t) NULL, notifyUSART4);

  /* I/O pins for USART4.*/
#error USART4 unavailable
#endif

#if USE_MB96F3xx_USART5
  sdObjectInit(&SD5, (qnotify_t) NULL, notifyUSART5);

  /* I/O pins for USART5.*/
#if defined (__CPU_MB96330_SERIES)
  SD5.reg = USART5_Base;
  DDR10_D4 = 0;		// set SIN5 as input
  PIER10_IE4 = 1;	// enable port input
#endif	//	__CPU_MB96330_SERIES
#endif

#if USE_MB96F3xx_USART6
  sdObjectInit(&SD6, (qnotify_t) NULL, notifyUSART6);

  /* I/O pins for USART6.*/
#error USART6 unavailable
#endif

#if USE_MB96F3xx_USART7
  sdObjectInit(&SD7, (qnotify_t) NULL, notifyUSART7);

  /* I/O pins for USART7.*/
#if defined (__CPU_MB96330_SERIES) || defined (__CPU_MB96350_SERIES)	// NOTE: only redirected pin used
  SD7.reg = USART7_Base;
  DDR00_D2 = 0;		// set SIN7_R as input
  PIER00_IE2 = 1;	// enable port input
#else
#error USART7 unavailable
#endif
#endif

#if USE_MB96F3xx_USART8
  sdObjectInit(&SD8, (qnotify_t) NULL, notifyUSART8);

  /* I/O pins for USART8.*/
#if defined (__CPU_MB96330_SERIES) || defined (__CPU_MB96350_SERIES)	// NOTE: only redirected pin used
  SD8.reg = USART8_Base;
  DDR00_D5 = 0;		// set SIN8_R as input
  PIER00_IE5 = 1;	// enable port input
#endif	//	__CPU_MB96330_SERIES
#endif

#if USE_MB96F3xx_USART9
  sdObjectInit(&SD9, (qnotify_t) NULL, notifyUSART9);

  /* I/O pins for USART9.*/
#if defined (__CPU_MB96330_SERIES)	// NOTE: only redirected pin used
  SD9.reg = USART9_Base;
  DDR17_D3 = 0;		// set SIN8_R as input
  PIER17_IE3 = 1;	// enable port input
#endif	//	__CPU_MB96330_SERIES
#endif

}

/**
 * @brief Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 * @param[in] config the architecture-dependent serial driver configuration.
 *                   If this parameter is set to @p NULL then a default
 *                   configuration is used.
 */
void sd_lld_start(SerialDriver *sdp) {

  if (sdp->config == NULL)
    sdp->config = &default_config;

if (
#if USE_MB96F3xx_USART0
   (&SD0 == sdp) ||
#endif
#if USE_MB96F3xx_USART1
   (&SD1 == sdp) ||
#endif
#if USE_MB96F3xx_USART2
   (&SD2 == sdp) ||
#endif
#if USE_MB96F3xx_USART3
   (&SD3 == sdp) ||
#endif
#if USE_MB96F3xx_USART4
   (&SD4 == sdp) ||
#endif
#if USE_MB96F3xx_USART5
   (&SD5 == sdp) ||
#endif
#if USE_MB96F3xx_USART6
   (&SD6 == sdp) ||
#endif
#if USE_MB96F3xx_USART7
   (&SD7 == sdp) ||
#endif
#if USE_MB96F3xx_USART8
   (&SD8 == sdp) ||
#endif
#if USE_MB96F3xx_USART9
   (&SD9 == sdp) ||
#endif
  (0) ) {
    usart_init(sdp);
    return;
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

if (
#if USE_MB96F3xx_USART0
   (&SD0 == sdp) ||
#endif
#if USE_MB96F3xx_USART1
   (&SD1 == sdp) ||
#endif
#if USE_MB96F3xx_USART2
   (&SD2 == sdp) ||
#endif
#if USE_MB96F3xx_USART3
   (&SD3 == sdp) ||
#endif
#if USE_MB96F3xx_USART4
   (&SD4 == sdp) ||
#endif
#if USE_MB96F3xx_USART5
   (&SD5 == sdp) ||
#endif
#if USE_MB96F3xx_USART6
   (&SD6 == sdp) ||
#endif
#if USE_MB96F3xx_USART7
   (&SD7 == sdp) ||
#endif
#if USE_MB96F3xx_USART8
   (&SD8 == sdp) ||
#endif
#if USE_MB96F3xx_USART9
   (&SD9 == sdp) ||
#endif
  (0) ) {
    usart_deinit(sdp);
    return;
  }
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */
