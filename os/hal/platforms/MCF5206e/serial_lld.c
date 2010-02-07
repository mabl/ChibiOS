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
 * @file ports/ColdFire/coldfire_serial.c
 * @brief ColdFire Serial driver code.
 * @addtogroup COLDFIRE_SERIAL
 * @{
 */

#include <ch.h>
#include <signal.h>

#include "board.h"
#include "coldfire_serial.h"
#include "mcf5206e.h"

static void SetError(uint8_t usr, FullDuplexDriver *com) {
/*
  TODO: test it
  dflags_t sts = 0;

  if (usr & mcf5206e_UART_USR_OE)
    sts |= SD_OVERRUN_ERROR;
  if (usr & mcf5206e_UART_USR_PE)
    sts |= SD_PARITY_ERROR;
  if (usr & mcf5206e_UART_USR_FE)
    sts |= SD_FRAMING_ERROR;
  if (usr & mcf5206e_UART_USR_RB)
    sts |= SD_BREAK_DETECTED;
  chSysLockFromIsr();
  chFDDAddFlagsI(com, sts);
  chSysUnlockFromIsr();
*/
}

#if USE_COLDFIRE_USART0 || defined(__DOXYGEN__)

/** @brief USART0 serial driver identifier.*/
FullDuplexDriver COM1;

static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];


void ServeInterrupt(mcf5206e_UART1 *u, FullDuplexDriver *com)
{
	uint8_t usr = u->USR;

/*
  TODO: test it
  if (usr & (mcf5206e_UART_USR_RB | mcf5206e_UART_USR_FE | mcf5206e_UART_USR_PE | mcf5206e_UART_USR_OE)) {
    SetError(usr, com);
  }
*/
	if (usr & mcf5206e_UART_USR_RXRDY)
	{
		chSysLockFromIsr();
		chFDDIncomingDataI(com, u->UBUF);
		chSysUnlockFromIsr();
	}
	if (usr & mcf5206e_UART_USR_TXRDY)
	{
		chSysLockFromIsr();
		msg_t b = chFDDRequestDataI(com);
		chSysUnlockFromIsr();
		if (b < Q_OK) {
			u->UIR = mcf5206e_UART_UIMR_FFULL;
		} else {
			u->UBUF = b;
		}
	}
}


CH_IRQ_HANDLER(UARTIrqHandler) {

	CH_IRQ_PROLOGUE();

	ServeInterrupt((mcf5206e_UART1 *)&sim->uart1, &COM1);

	CH_IRQ_EPILOGUE();
}


static void OutNotify1(void) {

	sim->uart1.UIR = mcf5206e_UART_UIMR_FFULL | mcf5206e_UART_UIMR_TXRDY;
}

/**
 * @brief USART0 setup.
 * @details This function must be invoked with interrupts disabled.
 * @param div The divider value as calculated by the @p UBR() macro.
 * @param mod The value for the @p U0MCTL register.
 * @param ctl The value for the @p U0CTL register.
 * @note Does not reset the I/O queues.
 */
void usart0_setup(uint32_t baud) {
	uint32_t b;

	b = F_CPU*10/32L;
	if (baud)
		b /= baud;
	if ((b % 10) > 5)
		b += 10;

	sim->uart1.UCR = mcf5206e_UART_UCR_RESET_TX;
	sim->uart1.UCR = mcf5206e_UART_UCR_RESET_RX;
	sim->uart1.UCR = mcf5206e_UART_UCR_RESET_MR;
	sim->uart1.UBG1 = (b/10)>>8;
	sim->uart1.UBG2 = (b/10)&0xFF;
	sim->uart1.USR = mcf5206e_UART_UCSR_RX_TIMER|mcf5206e_UART_UCSR_TX_TIMER;
	sim->uart1.UMR = mcf5206e_UART_UMR1_PM_NONE|mcf5206e_UART_UMR1_BC_8;
	sim->uart1.UCR = mcf5206e_UART_UCR_TX_ENABLED|mcf5206e_UART_UCR_RX_ENABLED;
	sim->uart1.UIR = mcf5206e_UART_UIMR_FFULL;

	sim->uart1.UIR = mcf5206e_UART_UIMR_FFULL;
	*(volatile uint32_t *)0x78 = (uint32_t)UARTIrqHandler;
	sim->sim.ICR12		=	mcf5206e_SIM_ICR_AVEC|
									mcf5206e_SIM_ICR_IL(6)|
									mcf5206e_SIM_ICR_IP(2);
	sim->sim.IMR &= ~mcf5206e_SIM_IMR_UART1;
}
#endif /* USE_COLDFIRE_USART0 */

#if USE_COLDFIRE_USART1 || defined(__DOXYGEN__)
#endif

/**
 * @brief Serial driver initialization.
 * @note The serial ports are initialized at @p 57600-8-N-1 by default.
 */
void serial_init(void) {

  /* I/O queues setup.*/
#if USE_COLDFIRE_USART0
  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  usart0_setup(DEFAULT_USART_BITRATE);
#endif

#if USE_COLDFIRE_USART1
#endif
}

/** @} */
