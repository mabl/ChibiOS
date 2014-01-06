/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

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
 * Setup for Renesas YRPBRL78G13 board
 */

/*
 * Board identifiers.
 */
#define BOARD_YRPBRL78G13
#define BOARD_NAME              "Renesas YRPBRL78G13"

/*
 * Board frequencies.
 */
#define SUBCLK                  32768

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define R5F100xE

/*
 * PORT 0 initial setup.
 */
#define VAL_PORT0DIR            0x00
#define VAL_PORT0DATA           0x00

/*
 * Pin definitions.
 */
#define PORT7_LED               7

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
