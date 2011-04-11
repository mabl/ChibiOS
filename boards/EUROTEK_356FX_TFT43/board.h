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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Eurotek 356FX_TFT43 proto board.
 */

/*
 * Board identifier.
 */
#define BOARD_EUROTEK_356FX_TFT43


/*
 * Clock constants.
 */
#define CRYSTAL_FREQ    4000000.0           // quarzo da 4M
#define CRYSTAL_SUBFRQ  32768.0             // quarzo subclock da 32768Hz

#define SYSTEM_FREQ    	56000000.0          // frequenza di sistema con PLL=6x
#define CLKP1_FREQ    	56000000.0          // frequenza periferal clock CLKP1
#define CLKP2_FREQ    	14000000.0          // frequenza periferal clock CLKP2 -> usato per CAN & soundgen

/*
 * Pin definitions for the Eurotek 356FX_TFT43 proto board.
 */

#define	PinBK_EN		PDR06_P1
#define	PinTOUCH_X1		EPSR06_PS3
#define	PinTOUCH_Y1		EPSR06_PS4
#define	PinTOUCH_X2		EPSR06_PS5
#define	PinTOUCH_Y2		EPSR06_PS6
#define	PinBK_PWM		PDR06_P7

#define	ddrTOUCH_X1		DDR06_D3
#define	ddrTOUCH_Y1		DDR06_D4
#define	ddrTOUCH_X2		DDR06_D5
#define	ddrTOUCH_Y2		DDR06_D6

#define	lchTOUCH_X1		PDR06_P3
#define	lchTOUCH_Y1		PDR06_P4
#define	lchTOUCH_X2		PDR06_P5
#define	lchTOUCH_Y2		PDR06_P6

#define	pulTOUCH_X1		PUCR06_PU3
#define	pulTOUCH_Y1		PUCR06_PU4
#define	pulTOUCH_X2		PUCR06_PU5
#define	pulTOUCH_Y2		PUCR06_PU6

#define	adenTOUCH_X1	ADER0_ADE3
#define	adenTOUCH_Y1	ADER0_ADE4
#define	adenTOUCH_X2	ADER0_ADE5
#define	adenTOUCH_Y2	ADER0_ADE6

#define	PinEnStep1		PDR02_P5
#define	PinClkStep1		PDR03_P4
#define	PinDirStep1		PDR03_P5

#define	PinEnStep2		PDR02_P6
#define	PinClkStep2		PDR03_P6
#define	PinDirStep2		PDR03_P6


/*
 * Initial I/O ports settings.
 */


#ifdef __cplusplus
extern "C" {
#endif
  void hwinit(void);
#ifdef __cplusplus
}
#endif

#endif /* _BOARD_H_ */
