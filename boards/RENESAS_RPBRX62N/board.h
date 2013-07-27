/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for Renesas RPBRX62N board
 */

/*
 * Board identifiers.
 */
#define BOARD_RPBRX62N
#define BOARD_NAME              "Renesas RPBRX62N"

/*
 * Board frequencies.
 */
#define EXTALCLK                12000000
#define SUBCLK                  32768

/*
 * GPIO A initial setup.
 */
#define VAL_GPIOADIR            PAL_PORT_BIT(GPIOA_LED0) |                  \
                                PAL_PORT_BIT(GPIOA_LED1) |                  \
                                PAL_PORT_BIT(GPIOA_LED2)
#define VAL_GPIOADATA           PAL_PORT_BIT(GPIOA_LED0) |                  \
                                PAL_PORT_BIT(GPIOA_LED1) |                  \
                                PAL_PORT_BIT(GPIOA_LED2)

/*
 * Pin definitions.
 */
#define GPIOA_LED0              2
#define GPIOA_LED1              1
#define GPIO0_LED2              0

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
