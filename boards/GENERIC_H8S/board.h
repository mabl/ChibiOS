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

#ifndef _BOARD_H_
#define _BOARD_H_

#include "ioh82368.h"
#include "cs8900a.h"

#define BOARD_MY_H8S

#define _BV(x)                  (1<<x)

#define bitLED                  2

#define LEDOn()                 P2DR &= ~_BV(bitLED)
#define LEDOff()                P2DR |= _BV(bitLED)

/*
 * Initial I/O setup.
 */
#define VAL_GPIO2_OUT           0x01      /* Output data. */
#define VAL_GPIO2_DIR           0xFF      /* Direction. */

/*
 * I/O definitions.
 */
#define GPIO_LED                2
#define GPIO_LED_MASK           (1<<GPIO_LED)

/*
 * CS8900A definitions.
 */
#define CS8900A_BASE_ADDRESS    0x00A00300 /* CS8900A base address */
#define CS8900A_IRQ_NUMBER      0x0002 /* CS8900A interrupt number connected to H8S */
#define CS8900A_READ(adr)       ( *(volatile uint16_t *)(CS8900A_BASE_ADDRESS+adr) )
#define CS8900A_WRITE(adr,dat)  ( *(volatile uint16_t *)(CS8900A_BASE_ADDRESS+adr) = dat )
#define CS8900A_IRQENABLE()     (IER |= _BV(7))
#define CS8900A_IRQDISABLE()    (IER &= ~_BV(7))

void hwinit(void);

#endif /* _BOARD_H_ */
