/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @file    STM32F37x/stm32_registry.h
 * @brief   STM32F37x capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _STM32_REGISTRY_H_
#define _STM32_REGISTRY_H_

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    STM32F37x capabilities
 * @{
 */
/*===========================================================================*/
/* STM32F373xC.                                                              */
/*===========================================================================*/
#if defined(STM32F373xC) || defined(__DOXYGEN__)
/* ADC attributes.*/
#define STM32_HAS_ADC1                      TRUE
#define STM32_HAS_ADC2                      FALSE
#define STM32_HAS_ADC3                      FALSE
#define STM32_HAS_ADC4                      FALSE

#define STM32_HAS_SDADC1                    TRUE
#define STM32_HAS_SDADC2                    TRUE
#define STM32_HAS_SDADC3                    TRUE

/* CAN attributes.*/
#define STM32_HAS_CAN1                      TRUE
#define STM32_HAS_CAN2                      FALSE
#define STM32_CAN_MAX_FILTERS               14

/* DAC attributes.*/
#define STM32_HAS_DAC1                      TRUE
#define STM32_HAS_DAC2                      TRUE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA                  FALSE
#define STM32_HAS_DMA1                      TRUE
#define STM32_HAS_DMA2                      TRUE

/* ETH attributes.*/
#define STM32_HAS_ETH                       FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS             29

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     TRUE
#define STM32_HAS_GPIOF                     TRUE
#define STM32_HAS_GPIOG                     FALSE
#define STM32_HAS_GPIOH                     FALSE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_AHBENR_GPIOAEN |           \
                                             RCC_AHBENR_GPIOBEN |           \
                                             RCC_AHBENR_GPIOCEN |           \
                                             RCC_AHBENR_GPIODEN |           \
                                             RCC_AHBENR_GPIOEEN |           \
                                             RCC_AHBENR_GPIOFEN)

/* I2C attributes.*/
#define STM32_HAS_I2C1                      TRUE
#define STM32_I2C_I2C1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 7)
#define STM32_I2C_I2C1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 6)

#define STM32_HAS_I2C2                      TRUE
#define STM32_I2C_I2C2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 5)
#define STM32_I2C_I2C2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)

#define STM32_HAS_I2C3                      FALSE

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE
#define STM32_RTC_HAS_SUBSECONDS            TRUE
#define STM32_RTC_HAS_PERIODIC_WAKEUPS      TRUE
#define STM32_RTC_NUM_ALARMS                2
#define STM32_RTC_HAS_INTERRUPTS            FALSE

/* SDIO attributes.*/
#define STM32_HAS_SDIO                      FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE
#define STM32_SPI_SPI1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_SPI_SPI1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 3)

#define STM32_HAS_SPI2                      TRUE
#define STM32_SPI_SPI2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)
#define STM32_SPI_SPI2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 5)

#define STM32_HAS_SPI3                      TRUE
#define STM32_SPI_SPI3_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 1)
#define STM32_SPI_SPI3_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 2)

#define STM32_HAS_SPI4                      FALSE
#define STM32_HAS_SPI5                      FALSE
#define STM32_HAS_SPI6                      FALSE

/* TIM attributes.*/
#define STM32_TIM_MAX_CHANNELS              4

#define STM32_HAS_TIM2                      TRUE
#define STM32_TIM2_IS_32BITS                TRUE
#define STM32_TIM2_CHANNELS                 4

#define STM32_HAS_TIM3                      TRUE
#define STM32_TIM3_IS_32BITS                FALSE
#define STM32_TIM3_CHANNELS                 4

#define STM32_HAS_TIM4                      TRUE
#define STM32_TIM4_IS_32BITS                FALSE
#define STM32_TIM4_CHANNELS                 4

#define STM32_HAS_TIM5                      TRUE
#define STM32_TIM5_IS_32BITS                TRUE
#define STM32_TIM5_CHANNELS                 4

#define STM32_HAS_TIM6                      TRUE
#define STM32_TIM6_IS_32BITS                FALSE
#define STM32_TIM6_CHANNELS                 0

#define STM32_HAS_TIM7                      TRUE
#define STM32_TIM7_IS_32BITS                FALSE
#define STM32_TIM7_CHANNELS                 0

#define STM32_HAS_TIM12                     TRUE
#define STM32_TIM12_IS_32BITS               FALSE
#define STM32_TIM12_CHANNELS                2

#define STM32_HAS_TIM13                     TRUE
#define STM32_TIM13_IS_32BITS               FALSE
#define STM32_TIM13_CHANNELS                2

#define STM32_HAS_TIM14                     TRUE
#define STM32_TIM14_IS_32BITS               FALSE
#define STM32_TIM14_CHANNELS                2

#define STM32_HAS_TIM15                     TRUE
#define STM32_TIM15_IS_32BITS               FALSE
#define STM32_TIM15_CHANNELS                2

#define STM32_HAS_TIM16                     TRUE
#define STM32_TIM16_IS_32BITS               FALSE
#define STM32_TIM16_CHANNELS                2

#define STM32_HAS_TIM17                     TRUE
#define STM32_TIM17_IS_32BITS               FALSE
#define STM32_TIM17_CHANNELS                2

#define STM32_HAS_TIM18                     TRUE
#define STM32_TIM18_IS_32BITS               FALSE
#define STM32_TIM18_CHANNELS                0

#define STM32_HAS_TIM19                     TRUE
#define STM32_TIM19_IS_32BITS               FALSE
#define STM32_TIM19_CHANNELS                4

#define STM32_HAS_TIM1                      FALSE
#define STM32_HAS_TIM8                      FALSE
#define STM32_HAS_TIM9                      FALSE
#define STM32_HAS_TIM10                     FALSE
#define STM32_HAS_TIM11                     FALSE

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_UART_USART1_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 5)
#define STM32_UART_USART1_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 4)

#define STM32_HAS_USART2                    TRUE
#define STM32_UART_USART2_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 6)
#define STM32_UART_USART2_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 7)

#define STM32_HAS_USART3                    TRUE
#define STM32_UART_USART3_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 3)
#define STM32_UART_USART3_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 2)

#define STM32_HAS_UART4                     FALSE
#define STM32_HAS_UART5                     FALSE
#define STM32_HAS_USART6                    FALSE

/* USB attributes.*/
#define STM32_HAS_USB                       TRUE
#define STM32_USB_ACCESS_SCHEME_2x16        FALSE
#define STM32_USB_PMA_SIZE                  512
#define STM32_USB_HAS_BCDR                  FALSE
#define STM32_HAS_OTG1                      FALSE
#define STM32_HAS_OTG2                      FALSE
#endif /* defined(STM32F373xC) */

/*===========================================================================*/
/* STM32F378xx.                                                              */
/*===========================================================================*/
#if defined(STM32F378xx) || defined(__DOXYGEN__)
/* ADC attributes.*/
#define STM32_HAS_ADC1                      TRUE
#define STM32_HAS_ADC2                      FALSE
#define STM32_HAS_ADC3                      FALSE
#define STM32_HAS_ADC4                      FALSE

#define STM32_HAS_SDADC1                    TRUE
#define STM32_HAS_SDADC2                    TRUE
#define STM32_HAS_SDADC3                    TRUE

/* CAN attributes.*/
#define STM32_HAS_CAN1                      TRUE
#define STM32_HAS_CAN2                      FALSE
#define STM32_CAN_MAX_FILTERS               14

/* DAC attributes.*/
#define STM32_HAS_DAC1                      TRUE
#define STM32_HAS_DAC2                      TRUE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA                  FALSE
#define STM32_HAS_DMA1                      TRUE
#define STM32_HAS_DMA2                      TRUE

/* ETH attributes.*/
#define STM32_HAS_ETH                       FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS             29

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     TRUE
#define STM32_HAS_GPIOF                     TRUE
#define STM32_HAS_GPIOG                     FALSE
#define STM32_HAS_GPIOH                     FALSE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_AHBENR_GPIOAEN |           \
                                             RCC_AHBENR_GPIOBEN |           \
                                             RCC_AHBENR_GPIOCEN |           \
                                             RCC_AHBENR_GPIODEN |           \
                                             RCC_AHBENR_GPIOEEN |           \
                                             RCC_AHBENR_GPIOFEN)

/* I2C attributes.*/
#define STM32_HAS_I2C1                      TRUE
#define STM32_I2C_I2C1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 7)
#define STM32_I2C_I2C1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 6)

#define STM32_HAS_I2C2                      TRUE
#define STM32_I2C_I2C2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 5)
#define STM32_I2C_I2C2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)

#define STM32_HAS_I2C3                      FALSE

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE
#define STM32_RTC_HAS_SUBSECONDS            TRUE
#define STM32_RTC_HAS_PERIODIC_WAKEUPS      TRUE
#define STM32_RTC_NUM_ALARMS                2
#define STM32_RTC_HAS_INTERRUPTS            FALSE

/* SDIO attributes.*/
#define STM32_HAS_SDIO                      FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE
#define STM32_SPI_SPI1_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 2)
#define STM32_SPI_SPI1_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 3)

#define STM32_HAS_SPI2                      TRUE
#define STM32_SPI_SPI2_RX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)
#define STM32_SPI_SPI2_TX_DMA_STREAM        STM32_DMA_STREAM_ID(1, 5)

#define STM32_HAS_SPI3                      TRUE
#define STM32_SPI_SPI3_RX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 1)
#define STM32_SPI_SPI3_TX_DMA_STREAM        STM32_DMA_STREAM_ID(2, 2)

#define STM32_HAS_SPI4                      FALSE
#define STM32_HAS_SPI5                      FALSE
#define STM32_HAS_SPI6                      FALSE

/* TIM attributes.*/
#define STM32_TIM_MAX_CHANNELS              4

#define STM32_HAS_TIM2                      TRUE
#define STM32_TIM2_IS_32BITS                TRUE
#define STM32_TIM2_CHANNELS                 4

#define STM32_HAS_TIM3                      TRUE
#define STM32_TIM3_IS_32BITS                FALSE
#define STM32_TIM3_CHANNELS                 4

#define STM32_HAS_TIM4                      TRUE
#define STM32_TIM4_IS_32BITS                FALSE
#define STM32_TIM4_CHANNELS                 4

#define STM32_HAS_TIM5                      TRUE
#define STM32_TIM5_IS_32BITS                TRUE
#define STM32_TIM5_CHANNELS                 4

#define STM32_HAS_TIM6                      TRUE
#define STM32_TIM6_IS_32BITS                FALSE
#define STM32_TIM6_CHANNELS                 0

#define STM32_HAS_TIM7                      TRUE
#define STM32_TIM7_IS_32BITS                FALSE
#define STM32_TIM7_CHANNELS                 0

#define STM32_HAS_TIM12                     TRUE
#define STM32_TIM12_IS_32BITS               FALSE
#define STM32_TIM12_CHANNELS                2

#define STM32_HAS_TIM13                     TRUE
#define STM32_TIM13_IS_32BITS               FALSE
#define STM32_TIM13_CHANNELS                2

#define STM32_HAS_TIM14                     TRUE
#define STM32_TIM14_IS_32BITS               FALSE
#define STM32_TIM14_CHANNELS                2

#define STM32_HAS_TIM15                     TRUE
#define STM32_TIM15_IS_32BITS               FALSE
#define STM32_TIM15_CHANNELS                2

#define STM32_HAS_TIM16                     TRUE
#define STM32_TIM16_IS_32BITS               FALSE
#define STM32_TIM16_CHANNELS                2

#define STM32_HAS_TIM17                     TRUE
#define STM32_TIM17_IS_32BITS               FALSE
#define STM32_TIM17_CHANNELS                2

#define STM32_HAS_TIM18                     TRUE
#define STM32_TIM18_IS_32BITS               FALSE
#define STM32_TIM18_CHANNELS                0

#define STM32_HAS_TIM19                     TRUE
#define STM32_TIM19_IS_32BITS               FALSE
#define STM32_TIM19_CHANNELS                4

#define STM32_HAS_TIM1                      FALSE
#define STM32_HAS_TIM8                      FALSE
#define STM32_HAS_TIM9                      FALSE
#define STM32_HAS_TIM10                     FALSE
#define STM32_HAS_TIM11                     FALSE

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_UART_USART1_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 5)
#define STM32_UART_USART1_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 4)

#define STM32_HAS_USART2                    TRUE
#define STM32_UART_USART2_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 6)
#define STM32_UART_USART2_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 7)

#define STM32_HAS_USART3                    TRUE
#define STM32_UART_USART3_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 3)
#define STM32_UART_USART3_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 2)

#define STM32_HAS_UART4                     FALSE
#define STM32_HAS_UART5                     FALSE
#define STM32_HAS_USART6                    FALSE

/* USB attributes.*/
#define STM32_HAS_USB                       FALSE
#define STM32_USB_ACCESS_SCHEME_2x16        FALSE
#define STM32_USB_PMA_SIZE                  512
#define STM32_HAS_OTG1                      FALSE
#define STM32_HAS_OTG2                      FALSE
#endif /* defined(STM32F378xx) */
/** @} */

#endif /* _STM32_REGISTRY_H_ */

/** @} */
