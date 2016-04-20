[#ftl]
[#--
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,
              2011,2012,2013,2014,2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="board.c" /]
[#if doc1.board.configuration_settings.hal_version[0]?trim == "2.6.x"]
  [#assign varbool = "bool_t" /]
  [#assign varfalse = "FALSE" /]
  [#assign vartrue = "TRUE" /]
[#else]
  [#assign varbool = "bool" /]
  [#assign varfalse = "false" /]
  [#assign vartrue = "true" /]
[/#if]
/*
[@license.EmitLicenseAsText /]
*/

[#list doc1.board.headers.header as header]
#include "${header[0]?string?trim}"
[/#list]
[#if doc1.board.configuration_settings.hal_version[0]?trim == "2.6.x"]
#include "ch.h"
[/#if]
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)
/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
const PALConfig pal_default_config = {
#if STM32_HAS_GPIOA
  {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
   VAL_GPIOA_ODR,   VAL_GPIOA_AFRL,   VAL_GPIOA_AFRH,    VAL_GPIOA_ASCR,
   VAL_GPIOA_LOCKR},
#endif
#if STM32_HAS_GPIOB
  {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
   VAL_GPIOB_ODR,   VAL_GPIOB_AFRL,   VAL_GPIOB_AFRH,    VAL_GPIOB_ASCR,
   VAL_GPIOB_LOCKR},
#endif
#if STM32_HAS_GPIOC
  {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
   VAL_GPIOC_ODR,   VAL_GPIOC_AFRL,   VAL_GPIOC_AFRH,    VAL_GPIOC_ASCR,
   VAL_GPIOC_LOCKR},
#endif
#if STM32_HAS_GPIOD
  {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
   VAL_GPIOD_ODR,   VAL_GPIOD_AFRL,   VAL_GPIOD_AFRH,    VAL_GPIOD_ASCR,
   VAL_GPIOD_LOCKR},
#endif
#if STM32_HAS_GPIOE
  {VAL_GPIOE_MODER, VAL_GPIOE_OTYPER, VAL_GPIOE_OSPEEDR, VAL_GPIOE_PUPDR,
   VAL_GPIOE_ODR,   VAL_GPIOE_AFRL,   VAL_GPIOE_AFRH,    VAL_GPIOE_ASCR,
   VAL_GPIOE_LOCKR},
#endif
#if STM32_HAS_GPIOF
  {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
   VAL_GPIOF_ODR,   VAL_GPIOF_AFRL,   VAL_GPIOF_AFRH,    VAL_GPIOF_ASCR,
   VAL_GPIOF_LOCKR},
#endif
#if STM32_HAS_GPIOG
  {VAL_GPIOG_MODER, VAL_GPIOG_OTYPER, VAL_GPIOG_OSPEEDR, VAL_GPIOG_PUPDR,
   VAL_GPIOG_ODR,   VAL_GPIOG_AFRL,   VAL_GPIOG_AFRH,    VAL_GPIOG_ASCR,
   VAL_GPIOG_LOCKR},
#endif
#if STM32_HAS_GPIOH
  {VAL_GPIOH_MODER, VAL_GPIOH_OTYPER, VAL_GPIOH_OSPEEDR, VAL_GPIOH_PUPDR,
   VAL_GPIOH_ODR,   VAL_GPIOH_AFRL,   VAL_GPIOH_AFRH,    VAL_GPIOH_ASCR,
   VAL_GPIOH_LOCKR},
#endif
};
#endif

/**
 * @brief   Early initialization code.
 * @details This initialization must be performed just after stack setup
 *          and before any other initialization.
 */
void __early_init(void) {

  stm32_clock_init();
[#if doc1.board.board_functions.__early_init[0]??]
  ${doc1.board.board_functions.__early_init[0]}
[/#if]
}

#if HAL_USE_SDC || defined(__DOXYGEN__)
/**
 * @brief   SDC card detection.
 */
${varbool} sdc_lld_is_card_inserted(SDCDriver *sdcp) {
[#if doc1.board.board_functions.sdc_lld_is_card_inserted[0]??]
${doc1.board.board_functions.sdc_lld_is_card_inserted[0]}
[#else]

  (void)sdcp;
  /* TODO: Fill the implementation.*/
  return ${vartrue};
[/#if]
}

/**
 * @brief   SDC card write protection detection.
 */
${varbool} sdc_lld_is_write_protected(SDCDriver *sdcp) {
[#if doc1.board.board_functions.sdc_lld_is_write_protected[0]??]
${doc1.board.board_functions.sdc_lld_is_write_protected[0]}
[#else]

  (void)sdcp;
  /* TODO: Fill the implementation.*/
  return ${varfalse};
[/#if]
}
#endif /* HAL_USE_SDC */

#if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
/**
 * @brief   MMC_SPI card detection.
 */
${varbool} mmc_lld_is_card_inserted(MMCDriver *mmcp) {
[#if doc1.board.board_functions.mmc_lld_is_card_inserted[0]??]
${doc1.board.board_functions.mmc_lld_is_card_inserted[0]}
[#else]

  (void)mmcp;
  /* TODO: Fill the implementation.*/
  return ${vartrue};
[/#if]
}

/**
 * @brief   MMC_SPI card write protection detection.
 */
${varbool} mmc_lld_is_write_protected(MMCDriver *mmcp) {
[#if doc1.board.board_functions.mmc_lld_is_write_protected[0]??]
${doc1.board.board_functions.mmc_lld_is_write_protected[0]}
[#else]

  (void)mmcp;
  /* TODO: Fill the implementation.*/
  return ${varfalse};
[/#if]
}
#endif

/**
 * @brief   Board-specific initialization code.
 * @todo    Add your board-specific code, if any.
 */
void boardInit(void) {
}
