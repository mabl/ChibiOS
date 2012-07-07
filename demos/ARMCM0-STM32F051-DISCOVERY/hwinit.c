/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nil.h"
#include "hwinit.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static const GPIOConfig gpio_default_config =
{
  {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR, VAL_GPIOA_ODR, VAL_GPIOA_AFRL, VAL_GPIOA_AFRH},
  {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR, VAL_GPIOB_ODR, VAL_GPIOB_AFRL, VAL_GPIOB_AFRH},
  {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR, VAL_GPIOC_ODR, VAL_GPIOC_AFRL, VAL_GPIOC_AFRH},
  {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR, VAL_GPIOD_ODR, VAL_GPIOD_AFRL, VAL_GPIOD_AFRH},
  {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR, VAL_GPIOF_ODR, VAL_GPIOF_AFRL, VAL_GPIOF_AFRH}
};

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void initgpio(GPIO_TypeDef *gpiop, const stm32_gpio_setup_t *config) {

  gpiop->MODER   = config->moder;
  gpiop->OTYPER  = config->otyper;
  gpiop->OSPEEDR = config->ospeedr;
  gpiop->PUPDR   = config->pupdr;
  gpiop->ODR     = config->odr;
  gpiop->AFRL    = config->afrl;
  gpiop->AFRH    = config->afrh;
}

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   RCC initialization.
 */
void hwInit(void) {

  /* Reset of all peripherals.*/
  rccResetAPB1(0xFFFFFFFF);
  rccResetAPB2(!RCC_APB2RSTR_DBGMCURST);

#if !STM32_NO_INIT
  /* HSI setup, it enforces the reset situation in order to handle possible
     problems with JTAG probes and re-initializations.*/
  RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                                       /* Wait until HSI is stable.    */
  RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
  RCC->CFGR = 0;                            /* CFGR reset value.            */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;                                       /* Waits until HSI is selected. */

#if STM32_HSE_ENABLED
  /* HSE activation.*/
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                                       /* Waits until HSE is stable.   */
#endif

#if STM32_HSE14_ENABLED
  /* HSI14 activation.*/
  RCC->CR2 |= RCC_CR2_HSI14ON;
  while (!(RCC->CR2 & RCC_CR2_HSI14RDY))
    ;                                       /* Waits until HSI14 is stable. */
#endif

#if STM32_LSI_ENABLED
  /* LSI activation.*/
  RCC->CSR |= RCC_CSR_LSION;
  while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
    ;                                       /* Waits until LSI is stable.   */
#endif

#if STM32_ACTIVATE_PLL
  /* PLL activation.*/
  RCC->CFGR |= STM32_PLLMUL | STM32_PLLXTPRE | STM32_PLLSRC;
  RCC->CR   |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                                       /* Waits until PLL is stable.   */
#endif

  /* Clock settings.*/
  RCC->CFGR  = STM32_MCOSEL |                STM32_PLLMUL | STM32_PLLXTPRE |
               STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE   | STM32_HPRE;
  RCC->CFGR3 = STM32_ADCSW  | STM32_CECSW  | STM32_I2C1SW | STM32_USART1SW;

  /* Flash setup and final clock selection.   */
  FLASH->ACR = STM32_FLASHBITS;

  /* Switching to the configured clock source if it is different from HSI.*/
#if (STM32_SW != STM32_SW_HSI)
  /* Switches clock source.*/
  RCC->CFGR |= STM32_SW;
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;                                       /* Waits selection complete.    */
#endif

  /* SYSCFG clock enabled here because it is a multi-functional unit shared
     among multiple drivers.*/
  rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, TRUE);

  /* Programmable voltage detector enable.*/
#if STM32_PVD_ENABLE
  PWR->CR |= PWR_CR_PVDE | (STM32_PLS & STM32_PLS_MASK);
#endif /* STM32_PVD_ENABLE */

  /* GPIO initialization.*/
  rccEnableAHB((RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN |
                RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOFEN),
               TRUE);
  initgpio(GPIOA, &gpio_default_config.PAData);
  initgpio(GPIOB, &gpio_default_config.PBData);
  initgpio(GPIOC, &gpio_default_config.PCData);
  initgpio(GPIOD, &gpio_default_config.PDData);
  initgpio(GPIOF, &gpio_default_config.PFData);

#endif /* !STM32_NO_INIT */

  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = STM32_HCLK / NIL_CFG_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;
}
