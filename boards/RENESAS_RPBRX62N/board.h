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
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define RX62NXBDFP


/*
 * PORT 0 initial setup.
 */
#define VAL_PORT0DIR            0x00
#define VAL_PORT0DATA           0x00
#define VAL_PORT0ICR            0x00
#define VAL_PORT0ODR            0x00

/*
 * PORT 1 initial setup.
 */
#define VAL_PORT1DIR            0x00
#define VAL_PORT1DATA           0x00
#define VAL_PORT1ICR            0x00
#define VAL_PORT1ODR            0x00

/*
 * PORT 2 initial setup.
 */
#define VAL_PORT2DIR            0x00
#define VAL_PORT2DATA           0x00
#define VAL_PORT2ICR            0x00
#define VAL_PORT2ODR            0x00

/*
 * PORT 3 initial setup.
 */
#define VAL_PORT3DIR            0x00
#define VAL_PORT3DATA           0x00
#define VAL_PORT3ICR            0x00
#define VAL_PORT3ODR            0x00

/*
 * PORT 4 initial setup.
 */
#define VAL_PORT4DIR            0x00
#define VAL_PORT4DATA           0x00
#define VAL_PORT4ICR            0x00

/*
 * PORT 5 initial setup.
 */
#define VAL_PORT5DIR            0x00
#define VAL_PORT5DATA           0x00
#define VAL_PORT5ICR            0x00

/*
 * GPIO A initial setup.
 */
#define VAL_PORTADIR            PAL_PORT_BIT(PORTA_LED0) |                  \
                                PAL_PORT_BIT(PORTA_LED1) |                  \
                                PAL_PORT_BIT(PORTA_LED2)
#define VAL_PORTADATA           PAL_PORT_BIT(PORTA_LED0) |                  \
                                PAL_PORT_BIT(PORTA_LED1) |                  \
                                PAL_PORT_BIT(PORTA_LED2)
#define VAL_PORTAICR            0x00
#define VAL_PORTAPCR            0x00

/*
 * PORT B initial setup.
 */
#define VAL_PORTBDIR            0x00
#define VAL_PORTBDATA           0x00
#define VAL_PORTBICR            0x00
#define VAL_PORTBPCR            0x00

/*
 * PORT C initial setup.
 */
#define VAL_PORTCDIR            0x00
#define VAL_PORTCDATA           0x00
#define VAL_PORTCICR            0x00
#define VAL_PORTCODR            0x00
#define VAL_PORTCPCR            0x00

/*
 * PORT D initial setup.
 */
#define VAL_PORTDDIR            0x00
#define VAL_PORTDDATA           0x00
#define VAL_PORTDICR            0x00
#define VAL_PORTDPCR            0x00

/*
 * PORT E initial setup.
 */
#define VAL_PORTEDIR            0x00
#define VAL_PORTEDATA           0x00
#define VAL_PORTEICR            0x00
#define VAL_PORTEPCR            0x00


/*
 * Pin definitions.
 */
#define PORTA_LED0              2
#define PORTA_LED1              1
#define PORTA_LED2              0

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
