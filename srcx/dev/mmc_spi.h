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

#ifndef _MMC_SPI_H_
#define _MMC_SPI_H_

/*
 * This file is an adapter between the generic MMC/SD driver and a physical
 * SPI service provider.
 */

#include <lpc214x.h>
#include <lpc214x_ssp.h>

#define spi_init() InitSSP()
#define spi_highspeed() SetSSP(2, CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0), 0)
#define spi_lowspeed() SetSSP(254, CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0), 0)
#define spi_lock() sspAcquireBus()
#define spi_unlock() sspReleaseBus()
#define spi_send(buf, n) sspRW(NULL, buf, n)
#define spi_receive(buf, n) sspRW(buf, NULL, n)
#define spi_ignore(n) sspRW(NULL, NULL, n)

#define card_present() (!(IO1PIN & (1 << 25)))
#define card_protected() (IO1PIN & (1 << 24))

#endif /* _MMC_SPI_H_ */
