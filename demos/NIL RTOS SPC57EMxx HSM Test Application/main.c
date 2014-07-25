/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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

/* Inclusion of the main header files of all the imported components in the
   order specified in the application wizard. The file is generated
   automatically.*/
#include "components.h"

#include "xpc57em_hsm.h"

/*
 * Application entry point.
 */
int main(void) {
  uint32_t n;

  /* PIT initialization, has to be performed before the RTOS start and
     before IRQs are enabled.*/
  /* PIT channel 0 initialization for Kernel ticks.*/
  n = 16000000 / NIL_CFG_ST_FREQUENCY - 1;
  PIT_HSM.MCR.R            = 1;
  PIT_HSM.TIMER[0].LDVAL.R = n;
  PIT_HSM.TIMER[0].CVAL.R  = n;
  PIT_HSM.TIMER[0].TFLG.R  = 1;     /* Interrupt flag cleared.          */
  PIT_HSM.TIMER[0].TCTRL.R = 3;     /* Timer active, interrupt enabled. */

  /* Initialization of all the imported components in the order specified in
     the application wizard. The function is generated automatically.*/
  componentsInit();

  /* Application main loop.*/
  while (1) {

  }
}
