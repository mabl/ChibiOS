[#ftl]

[#function DMA selection]
  [#return "STM32_DMA_STREAM_ID(" +
           selection?replace("DMA ", "")?replace(" Stream ", ", ") +
           ")" /]
[/#function]

[@pp.dropOutputFile /]
[@pp.changeOutputFile name="mcuconf.h" /]
/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

/*
 * STM32F4xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#define STM32F4xx_MCUCONF

/*
 * HAL driver system settings.
 */
#define STM32_NO_INIT                       ${conf.groups.initialization_settings.do_not_init[0]?upper_case}
#define STM32_HSI_ENABLED                   ${conf.groups.initialization_settings.enable_hsi_oscillator[0]?upper_case}
#define STM32_LSI_ENABLED                   ${conf.groups.initialization_settings.enable_lsi_oscillator[0]?upper_case}
#define STM32_HSE_ENABLED                   ${conf.groups.initialization_settings.enable_hse_oscillator[0]?upper_case}
#define STM32_LSE_ENABLED                   ${conf.groups.initialization_settings.enable_lse_oscillator[0]?upper_case}
#define STM32_CLOCK48_REQUIRED              ${conf.groups.initialization_settings.enable_48mhz_clock[0]?upper_case}
#define STM32_SW                            STM32_SW_${conf.groups.initialization_settings.system_clock_source[0]?upper_case}
#define STM32_PLLSRC                        STM32_PLLSRC_${conf.groups.initialization_settings.pll_clock_source[0]?upper_case}
#define STM32_PLLM_VALUE                    ${conf.groups.initialization_settings.pll_m_input_divider[0]}
#define STM32_PLLN_VALUE                    ${conf.groups.initialization_settings.pll_n_multiplier[0]}
#define STM32_PLLP_VALUE                    ${conf.groups.initialization_settings.pll_p_divider[0]}
#define STM32_PLLQ_VALUE                    ${conf.groups.initialization_settings.pll_q_divider[0]}
#define STM32_HPRE                          STM32_HPRE_DIV${conf.groups.initialization_settings.ahb_divider[0]}
#define STM32_PPRE1                         STM32_PPRE1_DIV${conf.groups.initialization_settings.apb1_divider[0]}
#define STM32_PPRE2                         STM32_PPRE2_DIV${conf.groups.initialization_settings.apb2_divider[0]}
#define STM32_RTCSEL                        STM32_RTCSEL_${conf.groups.initialization_settings.rtc_clock_source[0]}
#define STM32_RTCPRE_VALUE                  ${conf.groups.initialization_settings.rtc_hse_divider[0]}
#define STM32_MCO1SEL                       STM32_MCO1SEL_${conf.groups.initialization_settings.mco1_clock_source[0]}
#define STM32_MCO1PRE                       STM32_MCO1PRE_DIV${conf.groups.initialization_settings.mco1_divider[0]}
#define STM32_MCO2SEL                       STM32_MCO2SEL_${conf.groups.initialization_settings.mco2_clock_source[0]}
#define STM32_MCO2PRE                       STM32_MCO2PRE_DIV${conf.groups.initialization_settings.mco2_divider[0]}
#define STM32_I2SSRC                        STM32_I2SSRC_${conf.groups.initialization_settings.i2s_clock_source[0]}
#define STM32_PLLI2SN_VALUE                 ${conf.groups.initialization_settings.i2s_pll_n_multiplier[0]}
#define STM32_PLLI2SR_VALUE                 ${conf.groups.initialization_settings.i2s_pll_r_divider[0]}
[#if conf.groups.initialization_settings.voltage_output_scaling[0].@index[0]?string?number == 0]
#define STM32_VOS                           STM32_VOS_HIGH
[#else]
#define STM32_VOS                           STM32_VOS_LOW
[/#if]
#define STM32_PVD_ENABLE                    ${conf.groups.initialization_settings.enable_power_voltage_detector[0]?upper_case}
#define STM32_PLS                           STM32_PLS_LEV${conf.groups.initialization_settings.power_voltage_detector_level[0].@index[0]}

/*
 * ADC driver system settings.
 */
#define STM32_ADC_ADCPRE                    ADC_CCR_ADCPRE_DIV${conf.groups.adc_settings.adc_clock_divider[0]}
#define STM32_ADC_USE_ADC1                  ${conf.groups.adc_settings.use_adc1[0]?upper_case}
#define STM32_ADC_USE_ADC2                  ${conf.groups.adc_settings.use_adc2[0]?upper_case}
#define STM32_ADC_USE_ADC3                  ${conf.groups.adc_settings.use_adc3[0]?upper_case}
#define STM32_ADC_ADC1_DMA_STREAM           ${DMA(conf.groups.dma_settings.adc1_stream[0])}
#define STM32_ADC_ADC2_DMA_STREAM           ${DMA(conf.groups.dma_settings.adc2_stream[0])}
#define STM32_ADC_ADC3_DMA_STREAM           ${DMA(conf.groups.dma_settings.adc3_stream[0])}
#define STM32_ADC_ADC1_DMA_PRIORITY         ${conf.groups.dma_settings.adc1_stream_priority[0].@index[0]?string}
#define STM32_ADC_ADC2_DMA_PRIORITY         ${conf.groups.dma_settings.adc2_stream_priority[0].@index[0]?string}
#define STM32_ADC_ADC3_DMA_PRIORITY         ${conf.groups.dma_settings.adc3_stream_priority[0].@index[0]?string}
#define STM32_ADC_IRQ_PRIORITY              ${conf.groups.irq_settings.adc_shared_irq_priority[0].@index[0]?string}
#define STM32_ADC_ADC1_DMA_IRQ_PRIORITY     ${conf.groups.irq_settings.adc1_dma_irq_priority[0].@index[0]?string}
#define STM32_ADC_ADC2_DMA_IRQ_PRIORITY     ${conf.groups.irq_settings.adc2_dma_irq_priority[0].@index[0]?string}
#define STM32_ADC_ADC3_DMA_IRQ_PRIORITY     ${conf.groups.irq_settings.adc3_dma_irq_priority[0].@index[0]?string}

/*
 * CAN driver system settings.
 */
#define STM32_CAN_USE_CAN1                  ${conf.groups.can_settings.use_can1[0]?upper_case}
#define STM32_CAN_USE_CAN2                  ${conf.groups.can_settings.use_can2[0]?upper_case}
#define STM32_CAN_CAN1_IRQ_PRIORITY         ${conf.groups.irq_settings.can1_irq_priority[0].@index[0]?string}
#define STM32_CAN_CAN2_IRQ_PRIORITY         ${conf.groups.irq_settings.can2_irq_priority[0].@index[0]?string}

/*
 * EXT driver system settings.
 */
#define STM32_EXT_EXTI0_IRQ_PRIORITY        ${conf.groups.irq_settings.exti0_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI1_IRQ_PRIORITY        ${conf.groups.irq_settings.exti1_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI2_IRQ_PRIORITY        ${conf.groups.irq_settings.exti2_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI3_IRQ_PRIORITY        ${conf.groups.irq_settings.exti3_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI4_IRQ_PRIORITY        ${conf.groups.irq_settings.exti4_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI5_9_IRQ_PRIORITY      ${conf.groups.irq_settings.exti5_9_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI10_15_IRQ_PRIORITY    ${conf.groups.irq_settings.exti10_15_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI16_IRQ_PRIORITY       ${conf.groups.irq_settings.exti16_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI17_IRQ_PRIORITY       ${conf.groups.irq_settings.exti17_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI18_IRQ_PRIORITY       ${conf.groups.irq_settings.exti18_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI19_IRQ_PRIORITY       ${conf.groups.irq_settings.exti19_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI20_IRQ_PRIORITY       ${conf.groups.irq_settings.exti20_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI21_IRQ_PRIORITY       ${conf.groups.irq_settings.exti21_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI22_IRQ_PRIORITY       ${conf.groups.irq_settings.exti22_irq_priority[0].@index[0]?string}

/*
 * GPT driver system settings.
 */
#define STM32_GPT_USE_TIM1                  ${(conf.groups.tim_settings.tim1_assignment[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM2                  ${(conf.groups.tim_settings.tim2_assignment[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM3                  ${(conf.groups.tim_settings.tim3_assignment[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM4                  ${(conf.groups.tim_settings.tim4_assignment[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM5                  ${(conf.groups.tim_settings.tim5_assignment[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM8                  ${(conf.groups.tim_settings.tim8_assignment[0] == "GPT")?string?upper_case}
#define STM32_GPT_TIM1_IRQ_PRIORITY         ${conf.groups.irq_settings.tim1_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM2_IRQ_PRIORITY         ${conf.groups.irq_settings.tim2_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM3_IRQ_PRIORITY         ${conf.groups.irq_settings.tim3_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM4_IRQ_PRIORITY         ${conf.groups.irq_settings.tim4_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM5_IRQ_PRIORITY         ${conf.groups.irq_settings.tim5_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM8_IRQ_PRIORITY         ${conf.groups.irq_settings.tim8_irq_priority[0].@index[0]?string}

/*
 * I2C driver system settings.
 */
#define STM32_I2C_USE_I2C1                  ${conf.groups.i2c_settings.use_i2c1[0]?upper_case}
#define STM32_I2C_USE_I2C2                  ${conf.groups.i2c_settings.use_i2c2[0]?upper_case}
#define STM32_I2C_USE_I2C3                  ${conf.groups.i2c_settings.use_i2c3[0]?upper_case}
#define STM32_I2C_I2C1_RX_DMA_STREAM        ${DMA(conf.groups.dma_settings.i2c1_rx_stream[0])}
#define STM32_I2C_I2C1_TX_DMA_STREAM        ${DMA(conf.groups.dma_settings.i2c1_tx_stream[0])}
#define STM32_I2C_I2C2_RX_DMA_STREAM        ${DMA(conf.groups.dma_settings.i2c2_rx_stream[0])}
#define STM32_I2C_I2C2_TX_DMA_STREAM        ${DMA(conf.groups.dma_settings.i2c2_tx_stream[0])}
#define STM32_I2C_I2C3_RX_DMA_STREAM        ${DMA(conf.groups.dma_settings.i2c3_rx_stream[0])}
#define STM32_I2C_I2C3_TX_DMA_STREAM        ${DMA(conf.groups.dma_settings.i2c3_tx_stream[0])}
#define STM32_I2C_I2C1_IRQ_PRIORITY         ${conf.groups.irq_settings.i2c1_irq_priority[0].@index[0]?string}
#define STM32_I2C_I2C2_IRQ_PRIORITY         ${conf.groups.irq_settings.i2c2_irq_priority[0].@index[0]?string}
#define STM32_I2C_I2C3_IRQ_PRIORITY         ${conf.groups.irq_settings.i2c3_irq_priority[0].@index[0]?string}
#define STM32_I2C_I2C1_DMA_PRIORITY         ${conf.groups.dma_settings.i2c1_streams_priority[0].@index[0]?string}
#define STM32_I2C_I2C2_DMA_PRIORITY         ${conf.groups.dma_settings.i2c2_streams_priority[0].@index[0]?string}
#define STM32_I2C_I2C3_DMA_PRIORITY         ${conf.groups.dma_settings.i2c3_streams_priority[0].@index[0]?string}
#define STM32_I2C_I2C1_DMA_ERROR_HOOK()     ${conf.groups.dma_settings.i2c_dmas_error_hook[0]}
#define STM32_I2C_I2C2_DMA_ERROR_HOOK()     ${conf.groups.dma_settings.i2c_dmas_error_hook[0]}
#define STM32_I2C_I2C3_DMA_ERROR_HOOK()     ${conf.groups.dma_settings.i2c_dmas_error_hook[0]}

/*
 * ICU driver system settings.
 */
#define STM32_ICU_USE_TIM1                  ${(conf.groups.tim_settings.tim1_assignment[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM2                  ${(conf.groups.tim_settings.tim2_assignment[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM3                  ${(conf.groups.tim_settings.tim3_assignment[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM4                  ${(conf.groups.tim_settings.tim4_assignment[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM5                  ${(conf.groups.tim_settings.tim5_assignment[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM8                  ${(conf.groups.tim_settings.tim8_assignment[0] == "ICU")?string?upper_case}
#define STM32_ICU_TIM1_IRQ_PRIORITY         ${conf.groups.irq_settings.tim1_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM2_IRQ_PRIORITY         ${conf.groups.irq_settings.tim2_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM3_IRQ_PRIORITY         ${conf.groups.irq_settings.tim3_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM4_IRQ_PRIORITY         ${conf.groups.irq_settings.tim4_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM5_IRQ_PRIORITY         ${conf.groups.irq_settings.tim5_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM8_IRQ_PRIORITY         ${conf.groups.irq_settings.tim8_irq_priority[0].@index[0]?string}

/*
 * MAC driver system settings.
 */
#define STM32_MAC_TRANSMIT_BUFFERS          ${conf.groups.mac_settings.transmit_buffers[0]}
#define STM32_MAC_RECEIVE_BUFFERS           ${conf.groups.mac_settings.receive_buffers[0]}
#define STM32_MAC_BUFFERS_SIZE              1522
#define STM32_MAC_PHY_TIMEOUT               ${conf.groups.mac_settings.phy_search_timeout[0]}
#define STM32_MAC_ETH1_CHANGE_PHY_STATE     ${conf.groups.mac_settings.enable_phy_state_change[0]}
#define STM32_MAC_ETH1_IRQ_PRIORITY         ${conf.groups.irq_settings.eth_irq_priority[0].@index[0]?string}
#define STM32_MAC_IP_CHECKSUM_OFFLOAD       ${conf.groups.mac_settings.checksum_offload[0]}

/*
 * PWM driver system settings.
 */
#define STM32_PWM_USE_ADVANCED              ${conf.groups.tim_settings.advanced_pwm_mode[0]?upper_case}
#define STM32_PWM_USE_TIM1                  ${(conf.groups.tim_settings.tim1_assignment[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM2                  ${(conf.groups.tim_settings.tim2_assignment[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM3                  ${(conf.groups.tim_settings.tim3_assignment[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM4                  ${(conf.groups.tim_settings.tim4_assignment[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM5                  ${(conf.groups.tim_settings.tim5_assignment[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM8                  ${(conf.groups.tim_settings.tim8_assignment[0] == "PWM")?string?upper_case}
#define STM32_PWM_TIM1_IRQ_PRIORITY         ${conf.groups.irq_settings.tim1_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM2_IRQ_PRIORITY         ${conf.groups.irq_settings.tim2_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM3_IRQ_PRIORITY         ${conf.groups.irq_settings.tim3_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM4_IRQ_PRIORITY         ${conf.groups.irq_settings.tim4_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM5_IRQ_PRIORITY         ${conf.groups.irq_settings.tim5_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM8_IRQ_PRIORITY         ${conf.groups.irq_settings.tim8_irq_priority[0].@index[0]?string}

/*
 * SERIAL driver system settings.
 */
#define STM32_SERIAL_USE_USART1             ${(conf.groups.usart_settings.usart1_assignment[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_USART2             ${(conf.groups.usart_settings.usart2_assignment[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_USART3             ${(conf.groups.usart_settings.usart3_assignment[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_UART4              ${(conf.groups.usart_settings.uart4_assignment[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_UART5              ${(conf.groups.usart_settings.uart5_assignment[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_USART6             ${(conf.groups.usart_settings.usart6_assignment[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USART1_PRIORITY        ${conf.groups.irq_settings.usart1_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_USART2_PRIORITY        ${conf.groups.irq_settings.usart2_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_USART3_PRIORITY        ${conf.groups.irq_settings.usart3_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_UART4_PRIORITY         ${conf.groups.irq_settings.uart4_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_UART5_PRIORITY         ${conf.groups.irq_settings.uart5_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_USART6_PRIORITY        ${conf.groups.irq_settings.usart6_irq_priority[0].@index[0]?string}

/*
 * SPI driver system settings.
 */
#define STM32_SPI_USE_SPI1                  ${conf.groups.spi_settings.use_spi1[0]?upper_case}
#define STM32_SPI_USE_SPI2                  ${conf.groups.spi_settings.use_spi2[0]?upper_case}
#define STM32_SPI_USE_SPI3                  ${conf.groups.spi_settings.use_spi3[0]?upper_case}
#define STM32_SPI_SPI1_RX_DMA_STREAM        ${DMA(conf.groups.dma_settings.spi1_rx_stream[0])}
#define STM32_SPI_SPI1_TX_DMA_STREAM        ${DMA(conf.groups.dma_settings.spi1_tx_stream[0])}
#define STM32_SPI_SPI2_RX_DMA_STREAM        ${DMA(conf.groups.dma_settings.spi2_rx_stream[0])}
#define STM32_SPI_SPI2_TX_DMA_STREAM        ${DMA(conf.groups.dma_settings.spi2_tx_stream[0])}
#define STM32_SPI_SPI3_RX_DMA_STREAM        ${DMA(conf.groups.dma_settings.spi3_rx_stream[0])}
#define STM32_SPI_SPI3_TX_DMA_STREAM        ${DMA(conf.groups.dma_settings.spi3_tx_stream[0])}
#define STM32_SPI_SPI1_DMA_PRIORITY         ${conf.groups.dma_settings.spi1_streams_priority[0].@index[0]?string}
#define STM32_SPI_SPI2_DMA_PRIORITY         ${conf.groups.dma_settings.spi2_streams_priority[0].@index[0]?string}
#define STM32_SPI_SPI3_DMA_PRIORITY         ${conf.groups.dma_settings.spi3_streams_priority[0].@index[0]?string}
#define STM32_SPI_SPI1_IRQ_PRIORITY         ${conf.groups.irq_settings.spi1_irq_priority[0].@index[0]?string}
#define STM32_SPI_SPI2_IRQ_PRIORITY         ${conf.groups.irq_settings.spi2_irq_priority[0].@index[0]?string}
#define STM32_SPI_SPI3_IRQ_PRIORITY         ${conf.groups.irq_settings.spi3_irq_priority[0].@index[0]?string}
#define STM32_SPI_DMA_ERROR_HOOK(spip)      ${conf.groups.dma_settings.spi_dmas_error_hook[0]}

/*
 * UART driver system settings.
 */
#define STM32_UART_USE_USART1               ${(conf.groups.usart_settings.usart1_assignment[0] == "UART")?string?upper_case}
#define STM32_UART_USE_USART2               ${(conf.groups.usart_settings.usart2_assignment[0] == "UART")?string?upper_case}
#define STM32_UART_USE_USART3               ${(conf.groups.usart_settings.usart3_assignment[0] == "UART")?string?upper_case}
#define STM32_UART_USE_USART6               ${(conf.groups.usart_settings.usart6_assignment[0] == "UART")?string?upper_case}
#define STM32_UART_USART1_RX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart1_rx_stream[0])}
#define STM32_UART_USART1_TX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart1_tx_stream[0])}
#define STM32_UART_USART2_RX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart2_rx_stream[0])}
#define STM32_UART_USART2_TX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart2_tx_stream[0])}
#define STM32_UART_USART3_RX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart3_rx_stream[0])}
#define STM32_UART_USART3_TX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart3_tx_stream[0])}
#define STM32_UART_USART6_RX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart6_rx_stream[0])}
#define STM32_UART_USART6_TX_DMA_STREAM     ${DMA(conf.groups.dma_settings.usart6_tx_stream[0])}
#define STM32_UART_USART1_IRQ_PRIORITY      ${conf.groups.irq_settings.usart1_irq_priority[0].@index[0]?string}
#define STM32_UART_USART2_IRQ_PRIORITY      ${conf.groups.irq_settings.usart2_irq_priority[0].@index[0]?string}
#define STM32_UART_USART3_IRQ_PRIORITY      ${conf.groups.irq_settings.usart3_irq_priority[0].@index[0]?string}
#define STM32_UART_USART6_IRQ_PRIORITY      ${conf.groups.irq_settings.usart6_irq_priority[0].@index[0]?string}
#define STM32_UART_USART1_DMA_PRIORITY      ${conf.groups.dma_settings.usart1_streams_priority[0].@index[0]?string}
#define STM32_UART_USART2_DMA_PRIORITY      ${conf.groups.dma_settings.usart2_streams_priority[0].@index[0]?string}
#define STM32_UART_USART3_DMA_PRIORITY      ${conf.groups.dma_settings.usart3_streams_priority[0].@index[0]?string}
#define STM32_UART_USART6_DMA_PRIORITY      ${conf.groups.dma_settings.usart6_streams_priority[0].@index[0]?string}
#define STM32_UART_DMA_ERROR_HOOK(uartp)    ${conf.groups.dma_settings.usart_dmas_error_hook[0]}

/*
 * USB driver system settings.
 */
#define STM32_USB_USE_OTG1                  ${conf.groups.otg_settings.use_otg_fs[0]?upper_case}
#define STM32_USB_USE_OTG2                  ${conf.groups.otg_settings.use_otg_hs[0]?upper_case}
#define STM32_USB_OTG1_IRQ_PRIORITY         ${conf.groups.irq_settings.otgfs_irq_priority[0].@index[0]?string}
#define STM32_USB_OTG2_IRQ_PRIORITY         ${conf.groups.irq_settings.otghs_irq_priority[0].@index[0]?string}
#define STM32_USB_OTG1_RX_FIFO_SIZE         ${conf.groups.otg_settings.otg_fs_rx_fifo_size[0]}
#define STM32_USB_OTG2_RX_FIFO_SIZE         ${conf.groups.otg_settings.otg_hs_rx_fifo_size[0]}
#define STM32_USB_OTG_THREAD_PRIO           LOWPRIO
#define STM32_USB_OTG_THREAD_STACK_SIZE     128
#define STM32_USB_OTGFIFO_FILL_BASEPRI      0
