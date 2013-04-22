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

/*
 * STM8 drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 */

/*
 * HAL general settings.
 */
#define STM8S_NO_CLOCK_INIT             FALSE
#define STM8S_HSI_ENABLED               TRUE
#define STM8S_LSI_ENABLED               TRUE
#define STM8S_HSE_ENABLED               FALSE
#define STM8S_SYSCLK_SOURCE             CLK_SYSSEL_HSI
#define STM8S_HSI_DIVIDER               CLK_HSI_DIV1
#define STM8S_CPU_DIVIDER               CLK_CPU_DIV1

/*
 * SERIAL driver system settings.
 */
#define STM8S_SERIAL_USE_UART1          TRUE
#define STM8S_SERIAL_USE_UART2          FALSE
#define STM8S_SERIAL_USE_UART3          FALSE

/*
 * SPI driver system settings.
 */
#define STM8S_SPI_USE_SPI               TRUE
#define STM8S_SPI_ERROR_HOOK(spip)      chSysHalt()
