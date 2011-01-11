/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the STMicroelectronics STM3210C-EVAL evaluation board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM3210C_EVAL
#define BOARD_NAME "ST STM3210C-EVAL"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            25000000

/*
 * MCU type, this macro is used by both the ST library and the ChibiOS/RT
 * native STM32 HAL.
 */
#ifndef STM32F10X_CL
#define STM32F10X_CL
#endif

/*
 * IO pins assignments.
 * *********************TO BE COMPLETED*********************
 */

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * Everything input except:
 * PA0 - ETH_MII_CRS CRS Digital Input
 * PA1 - ETH_MII_RX_CLK / ETH_RMII_REF_CLK Digital Input
 * PA2 - ETH_MII_MDIO / ETH_RMII_MDIO AF PP Output
 * PA3 - ETH_MII_COL Digital Input
 */
#define VAL_GPIOACRL            0x44444B44      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x44444444      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFFFF

/*
 * Port B setup.
 * Everything input except:
 * PB5 - ETH_MII_PPS_OUT / ETH_RMII_PPS_OUT AF PP Output
 * PB8 - ETH_MII_TXD3 AF PP Output
 * PB10 - ETH_MII_RX_ER Digital Input
 * PB11 - ETH_MII_TX_EN / ETH_RMII_TX_EN AF PP Output
 * PB12 - ETH_MII_TXD0 / ETH_RMII_TXD0 AF PP Output
 * PB13 - ETH_MII_TXD1 / ETH_RMII_TXD1 AF PP Output
 */
#define VAL_GPIOBCRL            0x44B44444      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0x44BBB44B      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup.
 * Everything input except:
 * PC1 - ETH_MII_MDC / ETH_RMII_MDC AF PP Output
 * PC2 - ETH_MII_TXD2 AF PP Output
 * PC3 - ETH_MII_TX_CLK Digital Input
 */
#define VAL_GPIOCCRL            0x44444BB4      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x44444444      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFFF

/*
 * Port D setup.
 * Everything input except:
 * PD5 - USART2TX (remapped) AF PP Output
 * PD6 - USART2RX (remapped) Digital Input
 * PD7 - LED (LD1) PP Output
 * PD8 - ETH_MII_RX_DV / ETH_RMII_CRS_DV Digital Input
 * PD9 - ETH_MII_RXD0 / ETH_RMII_RXD0 Digital Input
 * PD10 - ETH_MII_RXD1 / ETH_RMII_RXD1 Digital Input
 * PD11 - ETH_MII_RXD2 Digital Input
 * PD12 - ETH_MII_RXD3 Digital Input
 *
 */
#define VAL_GPIODCRL            0x34B44444      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x44444444      /* PD15...PD8 */
#define VAL_GPIODODR            0x0F0FFFFF

/*
 * Port E setup.
 * Everything input except:
 */
#define VAL_GPIOECRL            0x44444444      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x44444444      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

#endif /* _BOARD_H_ */
