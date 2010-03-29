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
 * Setup for Raisonance REva V3 + STM8S208RB daughter board.
 */

/*
 * Board identifiers.
 */
#define BOARD_STM8128EVAL_STM8S208MB
#define BOARD_NAME "STMicro STM8/128-EVAL + STM8S208MB"

/*
 * Board frequencies.
 */
#define HSECLK          24000000

/*
 * MCU model used on the board.
 * The available models are listed in the file ./os/hal/platforms/stm8/stm8.h
 */
#define STM8_PLATFORM   PLATFORM_STM8S208MB

/*
 * Pin definitions.
 */
#define PA_OSCIN        1
#define PA_OSCOUT       2
#define PA_MEAS_PH_C    3 /* MC_pin34 / Measure Phase C / QST_pin7 */
#define PA_UART1_RX     4
#define PA_UART1_TX     5

#define PB_CUR_PH_C     0 /* MC_pin19 / Current phase C */
#define PB_CUR_PH_B     1 /* MC_pin17 / Current phase B */
#define PB_CUR_PH_A     2 /* MC_pin15 / Current phase A */
#define PB_BUS_VOLT     3 /* MC_pin14 / BUS voltage */
#define PB_JOY_L        4 /* Joystick input left */
#define PB_JOY_R        5 /* Joystick input right */
#define PB_JOY_U        6 /* Joystick input up */
#define PB_JOY_D        7 /* Joystick input down */

#define PC_BUTTON       0 /* Push_button */
#define PC_T1_CH1_UH    1 /* MC_pin3 / T1_CH1 UH */
#define PC_T1_CH2_VH    2 /* MC_pin7 / T1_CH2 VH */
#define PC_T1_CH3_WH    3 /* MC_pin11 / T1_CH3 WH */
#define PC_SPEAKER      4 /* PWM to speaker */
#define PC_SCK          5 /* SPI SCK / QST_pin4 */
#define PC_MOSI         6 /* SPI MOSI / QST_pin6 */
#define PC_MISO         7 /* SPI MISO / QST_pin8 */

#define PD_PFC_PWM      0 /* MC_pin29/PFC_PWM, or MC_pin23/dissipative brake */
#define PD_SWIM         1 /* SWIM */
#define PD_PFC_SYNC     2 /* MC_pin27 / PFC SYNC */
#define PD_MEAS_PH_A    3 /* MC_pin31 / Measure Phase A / QST_pin11 */
#define PD_MEAS_PH_B    4 /* MC_pin33 / Measure Phase B / BEEP */
#define PD_UART3_TX     5 /* UART3_TX */
#define PD_UART3_RX     6 /* UART3_RX */
#define PD_JOY_SEL      7 /* Joystick input selection */

#define PE_SMRT_OFF     0 /* Smartcard_OFF */
#define PE_SCL          1 /* I2C_SCLK / EEPROM / QST_pin3 */
#define PE_SDA          2 /* I2C_SDA / EEPROM / QST_pin5 */
#define PE_T1_BRK       3 /* MC_pin1 / T1_BRK emergency */
#define PE_SD_DETECT    4 /* MicroSDcard detect */
#define PE_SD_CS        5 /* SD Card CS (for SPI) */
#define PE_POT          6 /* Potentiometer read */
#define PE_HS_TEMP      7 /* MC_pin26 / Heatsink temp */

#define PF_LCD_CS       0 /* LCD CS (for SPI) */
#define PF_VREFN        1 /* VREF- */
#define PF_VREFP        2 /* VREF+ */
#define PF_MIC_OUT      3 /* AIN11 / Mic_Out */
#define PF_BNC          4 /* BNC Connector */
#define PF_USART_MX1    5 /* USART multiplexer 1 */
#define PF_USART_MX2    6 /* USART multiplexer 2 */
#define PF_LIN1_EN      7 /* LIN_1_EN */

#define PG_CAN_TX       0 /* CAN_Tx */
#define PG_CAN_RX       1 /* CAN_Rx */
#define PG_NTC_BYPASS   2 /* MC_pin21 / NTC Bypass relay */
#define PG_LIN2_EN      3 /* LIN_2_EN */
#define PG_SC_VOLT      4 /* Smartcard_3/5V */
#define PG_SC_RST       5 /* Smartcard_RST */
#define PG_LIN_MX1      6 /* LIN multiplexer 1 */
#define PG_SC_CMDVCC    7 /* SmartCard_CMDVCC */

#define PH_LD4          0 /* PH0 - LD4 */
#define PH_LD3          1 /* PH1 - LD3 */
#define PH_LD2          2 /* PH2 - LD2 */
#define PH_LD1          3 /* PH3 - LD1 */
#define PH_LCD_LIGHT    4 /* LCD backlight control */
#define PH_T1_NCH3_WL   5 /* MC_pin13 / T1_NCH3 WL */
#define PH_T1_NCH2_VL   6 /* MC_pin9 / T1_NCH2 VL */
#define PH_T1_NCH1_UL   7 /* MC_pin5 / T1_NCH1 UL */

#define PI_DIP(n)       (n)
#define PI_DIP_0        0 /* Dip-switch 0 / QST_pin9 */
#define PI_DIP_1        1 /* Dip-switch 1 / QST_pin10 */
#define PI_DIP_2        2 /* Dip-switch 2 */
#define PI_DIP_3        3 /* Dip-switch 3 */
#define PI_DIP_4        4 /* Dip-switch 4 */
#define PI_DIP_5        5 /* Dip-switch 5 */
#define PI_DIP_6        6 /* Dip-switch 6 */
#define PI_DIP_7        7 /* Dip-switch 7 */

/*
 * Port A initial setup.
 */
#define VAL_GPIOAODR    (1 << PA_UART1_TX)    /* PA_TX initially to 1.            */
#define VAL_GPIOADDR    (1 << PA_UART1_TX)    /* PA_TX output, others inputs.     */
#define VAL_GPIOACR1    (1 << PA_UART1_TX)    /* PA_TX Push-Pull */
#define VAL_GPIOACR2    0x00

/*
 * Port B initial setup.
 */
#define VAL_GPIOBODR    0x00
#define VAL_GPIOBDDR    0x00
#define VAL_GPIOBCR1    0x00
#define VAL_GPIOBCR2    0x00

/*
 * Port C initial setup.
 */
#define VAL_GPIOCODR    0x00
#define VAL_GPIOCDDR    0x00
#define VAL_GPIOCCR1    0x00
#define VAL_GPIOCCR2    0x00

/*
 * Port D initial setup.
 */
#define VAL_GPIODODR    0x00
#define VAL_GPIODDDR    0x00
#define VAL_GPIODCR1    0x00
#define VAL_GPIODCR2    0x00

/*
 * Port E initial setup.
 */
#define VAL_GPIOEODR    0x00
#define VAL_GPIOEDDR    0x00
#define VAL_GPIOECR1    0x00
#define VAL_GPIOECR2    0x00

/*
 * Port F initial setup.
 */
#define VAL_GPIOFODR    0x00
#define VAL_GPIOFDDR    0x00
#define VAL_GPIOFCR1    0x00
#define VAL_GPIOFCR2    0x00

/*
 * Port G initial setup.
 */
#define VAL_GPIOGODR    (1 << PG_CAN_TX)/* CAN_TX initially to 1.           */
#define VAL_GPIOGDDR    (1 << PG_CAN_TX)/* CAN_TX output, others inputs.    */
#define VAL_GPIOGCR1    (1 << PG_CAN_TX)/* CAN_TX push-pull */
#define VAL_GPIOGCR2    0x00

/*
 * Port H initial setup.
 */
#define VAL_GPIOHODR    0x00
#define VAL_GPIOHDDR    ((1<<PH_LD1)|(1<<PH_LD2)|(1<<PH_LD3)|(1<PH_LD4))
#define VAL_GPIOHCR1    ((1<<PH_LD1)|(1<<PH_LD2)|(1<<PH_LD3)|(1<PH_LD4))
#define VAL_GPIOHCR2    0x00

/*
 * Port I initial setup.
 */
#define VAL_GPIOIODR    0x00
#define VAL_GPIOIDDR    0x00
#define VAL_GPIOICR1    0x00
#define VAL_GPIOICR2    0x00

#ifdef __cplusplus
extern "C" {
#endif
  void hwinit(void);
#ifdef __cplusplus
}
#endif

#endif /* _BOARD_H_ */
