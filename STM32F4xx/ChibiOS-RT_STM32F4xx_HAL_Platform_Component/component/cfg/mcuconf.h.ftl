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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
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
#define STM32F40_41xxx

/*
 * HAL driver system settings.
 */
#define STM32_NO_INIT                       ${conf.instance.initialization_settings.do_not_init.value[0]?upper_case}
#define STM32_HSI_ENABLED                   ${conf.instance.initialization_settings.enable_hsi_oscillator.value[0]?upper_case}
#define STM32_LSI_ENABLED                   ${conf.instance.initialization_settings.enable_lsi_oscillator.value[0]?upper_case}
#define STM32_HSE_ENABLED                   ${conf.instance.initialization_settings.enable_hse_oscillator.value[0]?upper_case}
#define STM32_LSE_ENABLED                   ${conf.instance.initialization_settings.enable_lse_oscillator.value[0]?upper_case}
#define STM32_CLOCK48_REQUIRED              ${conf.instance.initialization_settings.enable_48mhz_clock.value[0]?upper_case}
#define STM32_SW                            STM32_SW_${conf.instance.initialization_settings.system_clock_source.value[0]?upper_case}
#define STM32_PLLSRC                        STM32_PLLSRC_${conf.instance.initialization_settings.pll_clock_source.value[0]?upper_case}
#define STM32_PLLM_VALUE                    ${conf.instance.initialization_settings.pll_m_input_divider.value[0]}
#define STM32_PLLN_VALUE                    ${conf.instance.initialization_settings.pll_n_multiplier.value[0]}
#define STM32_PLLP_VALUE                    ${conf.instance.initialization_settings.pll_p_divider.value[0]}
#define STM32_PLLQ_VALUE                    ${conf.instance.initialization_settings.pll_q_divider.value[0]}
#define STM32_HPRE                          STM32_HPRE_DIV${conf.instance.initialization_settings.ahb_divider.value[0]}
#define STM32_PPRE1                         STM32_PPRE1_DIV${conf.instance.initialization_settings.apb1_divider.value[0]}
#define STM32_PPRE2                         STM32_PPRE2_DIV${conf.instance.initialization_settings.apb2_divider.value[0]}
#define STM32_RTCSEL                        STM32_RTCSEL_${conf.instance.initialization_settings.rtc_clock_source.value[0]}
#define STM32_RTCPRE_VALUE                  ${conf.instance.initialization_settings.rtc_hse_divider.value[0]}
#define STM32_MCO1SEL                       STM32_MCO1SEL_${conf.instance.initialization_settings.mco1_clock_source.value[0]}
#define STM32_MCO1PRE                       STM32_MCO1PRE_DIV${conf.instance.initialization_settings.mco1_divider.value[0]}
#define STM32_MCO2SEL                       STM32_MCO2SEL_${conf.instance.initialization_settings.mco2_clock_source.value[0]}
#define STM32_MCO2PRE                       STM32_MCO2PRE_DIV${conf.instance.initialization_settings.mco2_divider.value[0]}
#define STM32_I2SSRC                        STM32_I2SSRC_${conf.instance.initialization_settings.i2s_clock_source.value[0]}
#define STM32_PLLI2SN_VALUE                 ${conf.instance.initialization_settings.i2s_pll_n_multiplier.value[0]}
#define STM32_PLLI2SR_VALUE                 ${conf.instance.initialization_settings.i2s_pll_r_divider.value[0]}
[#if conf.instance.initialization_settings.voltage_output_scaling[0].@index[0]?string?number == 0]
#define STM32_VOS                           STM32_VOS_HIGH
[#else]
#define STM32_VOS                           STM32_VOS_LOW
[/#if]
#define STM32_PVD_ENABLE                    ${conf.instance.initialization_settings.enable_power_voltage_detector.value[0]?upper_case}
#define STM32_PLS                           STM32_PLS_LEV${conf.instance.initialization_settings.power_voltage_detector_level[0].@index[0]}

/*
 * ADC driver system settings.
 */
#define STM32_ADC_ADCPRE                    ADC_CCR_ADCPRE_DIV${conf.instance.adc_settings.adc_clock_divider.value[0]}
#define STM32_ADC_USE_ADC1                  ${conf.instance.adc_settings.use_adc1.value[0]?upper_case}
#define STM32_ADC_USE_ADC2                  ${conf.instance.adc_settings.use_adc2.value[0]?upper_case}
#define STM32_ADC_USE_ADC3                  ${conf.instance.adc_settings.use_adc3.value[0]?upper_case}
#define STM32_ADC_ADC1_DMA_STREAM           ${DMA(conf.instance.dma_settings.adc1_stream.value[0])}
#define STM32_ADC_ADC2_DMA_STREAM           ${DMA(conf.instance.dma_settings.adc2_stream.value[0])}
#define STM32_ADC_ADC3_DMA_STREAM           ${DMA(conf.instance.dma_settings.adc3_stream.value[0])}
#define STM32_ADC_ADC1_DMA_PRIORITY         ${conf.instance.dma_settings.adc1_stream_priority[0].@index[0]?string}
#define STM32_ADC_ADC2_DMA_PRIORITY         ${conf.instance.dma_settings.adc2_stream_priority[0].@index[0]?string}
#define STM32_ADC_ADC3_DMA_PRIORITY         ${conf.instance.dma_settings.adc3_stream_priority[0].@index[0]?string}
#define STM32_ADC_IRQ_PRIORITY              ${conf.instance.irq_settings.adc_shared_irq_priority[0].@index[0]?string}
#define STM32_ADC_ADC1_DMA_IRQ_PRIORITY     ${conf.instance.irq_settings.adc1_dma_irq_priority[0].@index[0]?string}
#define STM32_ADC_ADC2_DMA_IRQ_PRIORITY     ${conf.instance.irq_settings.adc2_dma_irq_priority[0].@index[0]?string}
#define STM32_ADC_ADC3_DMA_IRQ_PRIORITY     ${conf.instance.irq_settings.adc3_dma_irq_priority[0].@index[0]?string}

/*
 * CAN driver system settings.
 */
#define STM32_CAN_USE_CAN1                  ${conf.instance.can_settings.use_can1.value[0]?upper_case}
#define STM32_CAN_USE_CAN2                  ${conf.instance.can_settings.use_can2.value[0]?upper_case}
#define STM32_CAN_CAN1_IRQ_PRIORITY         ${conf.instance.irq_settings.can1_irq_priority[0].@index[0]?string}
#define STM32_CAN_CAN2_IRQ_PRIORITY         ${conf.instance.irq_settings.can2_irq_priority[0].@index[0]?string}

/*
 * EXT driver system settings.
 */
#define STM32_EXT_EXTI0_IRQ_PRIORITY        ${conf.instance.irq_settings.exti0_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI1_IRQ_PRIORITY        ${conf.instance.irq_settings.exti1_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI2_IRQ_PRIORITY        ${conf.instance.irq_settings.exti2_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI3_IRQ_PRIORITY        ${conf.instance.irq_settings.exti3_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI4_IRQ_PRIORITY        ${conf.instance.irq_settings.exti4_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI5_9_IRQ_PRIORITY      ${conf.instance.irq_settings.exti5_9_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI10_15_IRQ_PRIORITY    ${conf.instance.irq_settings.exti10_15_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI16_IRQ_PRIORITY       ${conf.instance.irq_settings.exti16_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI17_IRQ_PRIORITY       ${conf.instance.irq_settings.exti17_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI18_IRQ_PRIORITY       ${conf.instance.irq_settings.exti18_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI19_IRQ_PRIORITY       ${conf.instance.irq_settings.exti19_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI20_IRQ_PRIORITY       ${conf.instance.irq_settings.exti20_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI21_IRQ_PRIORITY       ${conf.instance.irq_settings.exti21_irq_priority[0].@index[0]?string}
#define STM32_EXT_EXTI22_IRQ_PRIORITY       ${conf.instance.irq_settings.exti22_irq_priority[0].@index[0]?string}

/*
 * GPT driver system settings.
 */
#define STM32_GPT_USE_TIM1                  ${(conf.instance.tim_settings.tim1_assignment.value[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM2                  ${(conf.instance.tim_settings.tim2_assignment.value[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM3                  ${(conf.instance.tim_settings.tim3_assignment.value[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM4                  ${(conf.instance.tim_settings.tim4_assignment.value[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM5                  ${(conf.instance.tim_settings.tim5_assignment.value[0] == "GPT")?string?upper_case}
#define STM32_GPT_USE_TIM8                  ${(conf.instance.tim_settings.tim8_assignment.value[0] == "GPT")?string?upper_case}
#define STM32_GPT_TIM1_IRQ_PRIORITY         ${conf.instance.irq_settings.tim1_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM2_IRQ_PRIORITY         ${conf.instance.irq_settings.tim2_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM3_IRQ_PRIORITY         ${conf.instance.irq_settings.tim3_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM4_IRQ_PRIORITY         ${conf.instance.irq_settings.tim4_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM5_IRQ_PRIORITY         ${conf.instance.irq_settings.tim5_irq_priority[0].@index[0]?string}
#define STM32_GPT_TIM8_IRQ_PRIORITY         ${conf.instance.irq_settings.tim8_irq_priority[0].@index[0]?string}

/*
 * I2C driver system settings.
 */
#define STM32_I2C_USE_I2C1                  ${conf.instance.i2c_settings.use_i2c1.value[0]?upper_case}
#define STM32_I2C_USE_I2C2                  ${conf.instance.i2c_settings.use_i2c2.value[0]?upper_case}
#define STM32_I2C_USE_I2C3                  ${conf.instance.i2c_settings.use_i2c3.value[0]?upper_case}
#define STM32_I2C_I2C1_RX_DMA_STREAM        ${DMA(conf.instance.dma_settings.i2c1_rx_stream.value[0])}
#define STM32_I2C_I2C1_TX_DMA_STREAM        ${DMA(conf.instance.dma_settings.i2c1_tx_stream.value[0])}
#define STM32_I2C_I2C2_RX_DMA_STREAM        ${DMA(conf.instance.dma_settings.i2c2_rx_stream.value[0])}
#define STM32_I2C_I2C2_TX_DMA_STREAM        ${DMA(conf.instance.dma_settings.i2c2_tx_stream.value[0])}
#define STM32_I2C_I2C3_RX_DMA_STREAM        ${DMA(conf.instance.dma_settings.i2c3_rx_stream.value[0])}
#define STM32_I2C_I2C3_TX_DMA_STREAM        ${DMA(conf.instance.dma_settings.i2c3_tx_stream.value[0])}
#define STM32_I2C_I2C1_IRQ_PRIORITY         ${conf.instance.irq_settings.i2c1_irq_priority[0].@index[0]?string}
#define STM32_I2C_I2C2_IRQ_PRIORITY         ${conf.instance.irq_settings.i2c2_irq_priority[0].@index[0]?string}
#define STM32_I2C_I2C3_IRQ_PRIORITY         ${conf.instance.irq_settings.i2c3_irq_priority[0].@index[0]?string}
#define STM32_I2C_I2C1_DMA_PRIORITY         ${conf.instance.dma_settings.i2c1_streams_priority[0].@index[0]?string}
#define STM32_I2C_I2C2_DMA_PRIORITY         ${conf.instance.dma_settings.i2c2_streams_priority[0].@index[0]?string}
#define STM32_I2C_I2C3_DMA_PRIORITY         ${conf.instance.dma_settings.i2c3_streams_priority[0].@index[0]?string}
#define STM32_I2C_I2C1_DMA_ERROR_HOOK()     ${conf.instance.dma_settings.i2c_dmas_error_hook.value[0]}
#define STM32_I2C_I2C2_DMA_ERROR_HOOK()     ${conf.instance.dma_settings.i2c_dmas_error_hook.value[0]}
#define STM32_I2C_I2C3_DMA_ERROR_HOOK()     ${conf.instance.dma_settings.i2c_dmas_error_hook.value[0]}

/*
 * ICU driver system settings.
 */
#define STM32_ICU_USE_TIM1                  ${(conf.instance.tim_settings.tim1_assignment.value[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM2                  ${(conf.instance.tim_settings.tim2_assignment.value[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM3                  ${(conf.instance.tim_settings.tim3_assignment.value[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM4                  ${(conf.instance.tim_settings.tim4_assignment.value[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM5                  ${(conf.instance.tim_settings.tim5_assignment.value[0] == "ICU")?string?upper_case}
#define STM32_ICU_USE_TIM8                  ${(conf.instance.tim_settings.tim8_assignment.value[0] == "ICU")?string?upper_case}
#define STM32_ICU_TIM1_IRQ_PRIORITY         ${conf.instance.irq_settings.tim1_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM2_IRQ_PRIORITY         ${conf.instance.irq_settings.tim2_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM3_IRQ_PRIORITY         ${conf.instance.irq_settings.tim3_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM4_IRQ_PRIORITY         ${conf.instance.irq_settings.tim4_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM5_IRQ_PRIORITY         ${conf.instance.irq_settings.tim5_irq_priority[0].@index[0]?string}
#define STM32_ICU_TIM8_IRQ_PRIORITY         ${conf.instance.irq_settings.tim8_irq_priority[0].@index[0]?string}

/*
 * MAC driver system settings.
 */
#define STM32_MAC_TRANSMIT_BUFFERS          ${conf.instance.mac_settings.transmit_buffers.value[0]}
#define STM32_MAC_RECEIVE_BUFFERS           ${conf.instance.mac_settings.receive_buffers.value[0]}
#define STM32_MAC_BUFFERS_SIZE              1522
#define STM32_MAC_PHY_TIMEOUT               ${conf.instance.mac_settings.phy_search_timeout.value[0]}
#define STM32_MAC_ETH1_CHANGE_PHY_STATE     ${conf.instance.mac_settings.enable_phy_state_change.value[0]}
#define STM32_MAC_ETH1_IRQ_PRIORITY         ${conf.instance.irq_settings.eth_irq_priority[0].@index[0]?string}
#define STM32_MAC_IP_CHECKSUM_OFFLOAD       ${conf.instance.mac_settings.checksum_offload.value[0]}

/*
 * PWM driver system settings.
 */
#define STM32_PWM_USE_ADVANCED              ${conf.instance.tim_settings.advanced_pwm_mode.value[0]?upper_case}
#define STM32_PWM_USE_TIM1                  ${(conf.instance.tim_settings.tim1_assignment.value[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM2                  ${(conf.instance.tim_settings.tim2_assignment.value[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM3                  ${(conf.instance.tim_settings.tim3_assignment.value[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM4                  ${(conf.instance.tim_settings.tim4_assignment.value[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM5                  ${(conf.instance.tim_settings.tim5_assignment.value[0] == "PWM")?string?upper_case}
#define STM32_PWM_USE_TIM8                  ${(conf.instance.tim_settings.tim8_assignment.value[0] == "PWM")?string?upper_case}
#define STM32_PWM_TIM1_IRQ_PRIORITY         ${conf.instance.irq_settings.tim1_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM2_IRQ_PRIORITY         ${conf.instance.irq_settings.tim2_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM3_IRQ_PRIORITY         ${conf.instance.irq_settings.tim3_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM4_IRQ_PRIORITY         ${conf.instance.irq_settings.tim4_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM5_IRQ_PRIORITY         ${conf.instance.irq_settings.tim5_irq_priority[0].@index[0]?string}
#define STM32_PWM_TIM8_IRQ_PRIORITY         ${conf.instance.irq_settings.tim8_irq_priority[0].@index[0]?string}

/*
 * SERIAL driver system settings.
 */
#define STM32_SERIAL_USE_USART1             ${(conf.instance.usart_settings.usart1_assignment.value[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_USART2             ${(conf.instance.usart_settings.usart2_assignment.value[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_USART3             ${(conf.instance.usart_settings.usart3_assignment.value[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_UART4              ${(conf.instance.usart_settings.uart4_assignment.value[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_UART5              ${(conf.instance.usart_settings.uart5_assignment.value[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USE_USART6             ${(conf.instance.usart_settings.usart6_assignment.value[0] == "Serial")?string?upper_case}
#define STM32_SERIAL_USART1_PRIORITY        ${conf.instance.irq_settings.usart1_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_USART2_PRIORITY        ${conf.instance.irq_settings.usart2_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_USART3_PRIORITY        ${conf.instance.irq_settings.usart3_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_UART4_PRIORITY         ${conf.instance.irq_settings.uart4_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_UART5_PRIORITY         ${conf.instance.irq_settings.uart5_irq_priority[0].@index[0]?string}
#define STM32_SERIAL_USART6_PRIORITY        ${conf.instance.irq_settings.usart6_irq_priority[0].@index[0]?string}

/*
 * SPI driver system settings.
 */
#define STM32_SPI_USE_SPI1                  ${conf.instance.spi_settings.use_spi1.value[0]?upper_case}
#define STM32_SPI_USE_SPI2                  ${conf.instance.spi_settings.use_spi2.value[0]?upper_case}
#define STM32_SPI_USE_SPI3                  ${conf.instance.spi_settings.use_spi3.value[0]?upper_case}
#define STM32_SPI_SPI1_RX_DMA_STREAM        ${DMA(conf.instance.dma_settings.spi1_rx_stream.value[0])}
#define STM32_SPI_SPI1_TX_DMA_STREAM        ${DMA(conf.instance.dma_settings.spi1_tx_stream.value[0])}
#define STM32_SPI_SPI2_RX_DMA_STREAM        ${DMA(conf.instance.dma_settings.spi2_rx_stream.value[0])}
#define STM32_SPI_SPI2_TX_DMA_STREAM        ${DMA(conf.instance.dma_settings.spi2_tx_stream.value[0])}
#define STM32_SPI_SPI3_RX_DMA_STREAM        ${DMA(conf.instance.dma_settings.spi3_rx_stream.value[0])}
#define STM32_SPI_SPI3_TX_DMA_STREAM        ${DMA(conf.instance.dma_settings.spi3_tx_stream.value[0])}
#define STM32_SPI_SPI1_DMA_PRIORITY         ${conf.instance.dma_settings.spi1_streams_priority[0].@index[0]?string}
#define STM32_SPI_SPI2_DMA_PRIORITY         ${conf.instance.dma_settings.spi2_streams_priority[0].@index[0]?string}
#define STM32_SPI_SPI3_DMA_PRIORITY         ${conf.instance.dma_settings.spi3_streams_priority[0].@index[0]?string}
#define STM32_SPI_SPI1_IRQ_PRIORITY         ${conf.instance.irq_settings.spi1_irq_priority[0].@index[0]?string}
#define STM32_SPI_SPI2_IRQ_PRIORITY         ${conf.instance.irq_settings.spi2_irq_priority[0].@index[0]?string}
#define STM32_SPI_SPI3_IRQ_PRIORITY         ${conf.instance.irq_settings.spi3_irq_priority[0].@index[0]?string}
#define STM32_SPI_DMA_ERROR_HOOK(spip)      ${conf.instance.dma_settings.spi_dmas_error_hook.value[0]}

/*
 * UART driver system settings.
 */
#define STM32_UART_USE_USART1               ${(conf.instance.usart_settings.usart1_assignment.value[0] == "UART")?string?upper_case}
#define STM32_UART_USE_USART2               ${(conf.instance.usart_settings.usart2_assignment.value[0] == "UART")?string?upper_case}
#define STM32_UART_USE_USART3               ${(conf.instance.usart_settings.usart3_assignment.value[0] == "UART")?string?upper_case}
#define STM32_UART_USE_USART6               ${(conf.instance.usart_settings.usart6_assignment.value[0] == "UART")?string?upper_case}
#define STM32_UART_USART1_RX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart1_rx_stream.value[0])}
#define STM32_UART_USART1_TX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart1_tx_stream.value[0])}
#define STM32_UART_USART2_RX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart2_rx_stream.value[0])}
#define STM32_UART_USART2_TX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart2_tx_stream.value[0])}
#define STM32_UART_USART3_RX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart3_rx_stream.value[0])}
#define STM32_UART_USART3_TX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart3_tx_stream.value[0])}
#define STM32_UART_USART6_RX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart6_rx_stream.value[0])}
#define STM32_UART_USART6_TX_DMA_STREAM     ${DMA(conf.instance.dma_settings.usart6_tx_stream.value[0])}
#define STM32_UART_USART1_IRQ_PRIORITY      ${conf.instance.irq_settings.usart1_irq_priority[0].@index[0]?string}
#define STM32_UART_USART2_IRQ_PRIORITY      ${conf.instance.irq_settings.usart2_irq_priority[0].@index[0]?string}
#define STM32_UART_USART3_IRQ_PRIORITY      ${conf.instance.irq_settings.usart3_irq_priority[0].@index[0]?string}
#define STM32_UART_USART6_IRQ_PRIORITY      ${conf.instance.irq_settings.usart6_irq_priority[0].@index[0]?string}
#define STM32_UART_USART1_DMA_PRIORITY      ${conf.instance.dma_settings.usart1_streams_priority[0].@index[0]?string}
#define STM32_UART_USART2_DMA_PRIORITY      ${conf.instance.dma_settings.usart2_streams_priority[0].@index[0]?string}
#define STM32_UART_USART3_DMA_PRIORITY      ${conf.instance.dma_settings.usart3_streams_priority[0].@index[0]?string}
#define STM32_UART_USART6_DMA_PRIORITY      ${conf.instance.dma_settings.usart6_streams_priority[0].@index[0]?string}
#define STM32_UART_DMA_ERROR_HOOK(uartp)    ${conf.instance.dma_settings.usart_dmas_error_hook.value[0]}

/*
 * USB driver system settings.
 */
#define STM32_USB_USE_OTG1                  ${conf.instance.otg_settings.use_otg_fs.value[0]?upper_case}
#define STM32_USB_USE_OTG2                  ${conf.instance.otg_settings.use_otg_hs.value[0]?upper_case}
#define STM32_USB_OTG1_IRQ_PRIORITY         ${conf.instance.irq_settings.otgfs_irq_priority[0].@index[0]?string}
#define STM32_USB_OTG2_IRQ_PRIORITY         ${conf.instance.irq_settings.otghs_irq_priority[0].@index[0]?string}
#define STM32_USB_OTG1_RX_FIFO_SIZE         ${conf.instance.otg_settings.otg_fs_rx_fifo_size.value[0]}
#define STM32_USB_OTG2_RX_FIFO_SIZE         ${conf.instance.otg_settings.otg_hs_rx_fifo_size.value[0]}
#define STM32_USB_OTG_THREAD_PRIO           LOWPRIO
#define STM32_USB_OTG_THREAD_STACK_SIZE     128
#define STM32_USB_OTGFIFO_FILL_BASEPRI      0
