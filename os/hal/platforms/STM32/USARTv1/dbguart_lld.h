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

/**
 * @file    STM32/dbguart_lld.h
 * @brief   STM32 DBGUART subsystem low level driver header.
 *
 * @addtogroup DBGUART
 * @{
 */

#ifndef _DBGUART_LLD_H_
#define _DBGUART_LLD_H_

#if HAL_USE_DBGUART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   UART driver on USART1 enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_DBGUART_USE_USART1) || defined(__DOXYGEN__)
#define STM32_DBGUART_USE_USART1            FALSE
#endif

/**
 * @brief   UART driver on USART2 enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_DBGUART_USE_USART2) || defined(__DOXYGEN__)
#define STM32_DBGUART_USE_USART2            FALSE
#endif

/**
 * @brief   UART driver on USART3 enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_DBGUART_USE_USART3) || defined(__DOXYGEN__)
#define STM32_DBGUART_USE_USART3            FALSE
#endif

/**
 * @brief   UART driver on UART4 enable switch.
 * @details If set to @p TRUE the support for UART4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_DBGUART_USE_UART4) || defined(__DOXYGEN__)
#define STM32_DBGUART_USE_UART4             FALSE
#endif

/**
 * @brief   UART driver on UART5 enable switch.
 * @details If set to @p TRUE the support for UART5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_DBGUART_USE_UART4) || defined(__DOXYGEN__)
#define STM32_DBGUART_USE_UART5             FALSE
#endif

/**
 * @brief   UART driver on USART6 enable switch.
 * @details If set to @p TRUE the support for USART6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_DBGUART_USE_USART6) || defined(__DOXYGEN__)
#define STM32_DBGUART_USE_USART6            FALSE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_DBGUART_USE_USART1
  #if !STM32_HAS_USART1
  #error "USART1 not present in the selected device"
  #endif
#endif

#if STM32_DBGUART_USE_USART2
  #if !STM32_HAS_USART2
  #error "USART2 not present in the selected device"
  #endif

  #if STM32_DBGUART_USE_USART1
  #error "Only one UART can be selected for debug output"
  #endif
#endif

#if STM32_DBGUART_USE_USART3
  #if !STM32_HAS_USART3
  #error "USART3 not present in the selected device"
  #endif

  #if STM32_DBGUART_USE_USART1 || STM32_DBGUART_USE_USART2
  #error "Only one UART can be selected for debug output"
  #endif
#endif

#if STM32_DBGUART_USE_UART4
  #if !STM32_HAS_UART4
  #error "UART4 not present in the selected device"
  #endif

  #if STM32_DBGUART_USE_USART1 || STM32_DBGUART_USE_USART2 || \
      STM32_DBGUART_USE_USART3
  #error "Only one UART can be selected for debug output"
  #endif
#endif

#if STM32_DBGUART_USE_UART5
  #if !STM32_HAS_UART5
  #error "UART5 not present in the selected device"
  #endif

  #if STM32_DBGUART_USE_USART1 || STM32_DBGUART_USE_USART2 || \
      STM32_DBGUART_USE_USART3 || STM32_DBGUART_USE_UART4
  #error "Only one UART can be selected for debug output"
  #endif
#endif

#if STM32_DBGUART_USE_USART6
  #if !STM32_HAS_USART6
  #error "USART6 not present in the selected device"
  #endif

  #if STM32_DBGUART_USE_USART1 || STM32_DBGUART_USE_USART2 || \
      STM32_DBGUART_USE_USART3 || STM32_DBGUART_USE_UART4  || \
      STM32_DBGUART_USE_UART5
  #error "Only one UART can be selected for debug output"
  #endif
#endif

#if !STM32_DBGUART_USE_USART1 && !STM32_DBGUART_USE_USART2 &&                \
    !STM32_DBGUART_USE_USART3 && !STM32_DBGUART_USE_UART4 &&                 \
    !STM32_DBGUART_USE_UART5  && !STM32_DBGUART_USE_USART6
#error "DBGUART driver activated but no USART/UART peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of structure representing an UART driver.
 */
typedef struct DBGUARTDriver DBGUARTDriver;

/**
 * @brief   Structure representing an UART driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct DBGUARTDriver {
  /**
   * @brief Driver state.
   */
  dbguartstate_t            state;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the USART registers block.
   */
  USART_TypeDef             *usart;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern DBGUARTDriver DBGUARTD;

#ifdef __cplusplus
extern "C" {
#endif
  void dbguart_lld_init(void);
  void dbguart_lld_start(DBGUARTDriver *uartp);
  void dbguart_lld_send(DBGUARTDriver *uartp, size_t n, const void *txbuf);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DBGUART */

#endif /* _DBGUART_LLD_H_ */

/** @} */
