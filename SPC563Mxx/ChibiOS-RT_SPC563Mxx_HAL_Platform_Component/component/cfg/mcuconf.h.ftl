[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="mcuconf.h" /]
/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * SPC563Mxx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 1...15       Lowest...Highest.
 */

#define SPC563Mxx_MCUCONF

/*
 * HAL driver system settings.
 */
#define SPC5_NO_INIT                        ${conf.instance.clock_settings.do_not_init[0]?upper_case}
#define SPC5_CLK_BYPASS                     ${conf.instance.clock_settings.clock_bypass[0]?upper_case}
#define SPC5_ALLOW_OVERCLOCK                ${conf.instance.clock_settings.allow_overclocking[0]?upper_case}
#define SPC5_CLK_PREDIV_VALUE               ${conf.instance.clock_settings.fmpll0_settings.prediv_value[0]}
#define SPC5_CLK_MFD_VALUE                  ${conf.instance.clock_settings.fmpll0_settings.mfd_value[0]}
#define SPC5_CLK_RFD                        ${conf.instance.clock_settings.fmpll0_settings.rfd_value[0]}
#define SPC5_FLASH_BIUCR                    (BIUCR_BANK1_TOO |              \
                                             BIUCR_MASTER4_PREFETCH |       \
                                             BIUCR_MASTER0_PREFETCH |       \
                                             BIUCR_DPFEN |                  \
                                             BIUCR_IPFEN |                  \
                                             BIUCR_PFLIM_ON_MISS |          \
                                             BIUCR_BFEN)

/*
 * ADC driver settings.
 */
#define SPC5_ADC_USE_ADC0_Q0                ${conf.instance.eqadc_settings.fifo0[0]?upper_case}
#define SPC5_ADC_USE_ADC0_Q1                ${conf.instance.eqadc_settings.fifo1[0]?upper_case}
#define SPC5_ADC_USE_ADC0_Q2                ${conf.instance.eqadc_settings.fifo2[0]?upper_case}
#define SPC5_ADC_USE_ADC1_Q3                ${conf.instance.eqadc_settings.fifo3[0]?upper_case}
#define SPC5_ADC_USE_ADC1_Q4                ${conf.instance.eqadc_settings.fifo4[0]?upper_case}
#define SPC5_ADC_USE_ADC1_Q5                ${conf.instance.eqadc_settings.fifo5[0]?upper_case}
#define SPC5_ADC_FIFO0_DMA_PRIO             ${conf.instance.edma_settings.eqadc_fifo0[0]}
#define SPC5_ADC_FIFO1_DMA_PRIO             ${conf.instance.edma_settings.eqadc_fifo1[0]}
#define SPC5_ADC_FIFO2_DMA_PRIO             ${conf.instance.edma_settings.eqadc_fifo2[0]}
#define SPC5_ADC_FIFO3_DMA_PRIO             ${conf.instance.edma_settings.eqadc_fifo3[0]}
#define SPC5_ADC_FIFO4_DMA_PRIO             ${conf.instance.edma_settings.eqadc_fifo4[0]}
#define SPC5_ADC_FIFO5_DMA_PRIO             ${conf.instance.edma_settings.eqadc_fifo5[0]}
#define SPC5_ADC_FIFO0_DMA_IRQ_PRIO         ${conf.instance.irq_priority_settings.eqadc_fifo0[0]}
#define SPC5_ADC_FIFO1_DMA_IRQ_PRIO         ${conf.instance.irq_priority_settings.eqadc_fifo1[0]}
#define SPC5_ADC_FIFO2_DMA_IRQ_PRIO         ${conf.instance.irq_priority_settings.eqadc_fifo2[0]}
#define SPC5_ADC_FIFO3_DMA_IRQ_PRIO         ${conf.instance.irq_priority_settings.eqadc_fifo3[0]}
#define SPC5_ADC_FIFO4_DMA_IRQ_PRIO         ${conf.instance.irq_priority_settings.eqadc_fifo4[0]}
#define SPC5_ADC_FIFO5_DMA_IRQ_PRIO         ${conf.instance.irq_priority_settings.eqadc_fifo5[0]}
#define SPC5_ADC_CR_CLK_PS                  ADC_CR_CLK_PS(${conf.instance.eqadc_settings.divider[0]})
[#assign AN0 = conf.instance.eqadc_settings.an0[0]?upper_case?replace(" ", "_")]
[#assign AN1 = conf.instance.eqadc_settings.an1[0]?upper_case?replace(" ", "_")]
[#assign AN2 = conf.instance.eqadc_settings.an2[0]?upper_case?replace(" ", "_")]
[#assign AN3 = conf.instance.eqadc_settings.an3[0]?upper_case?replace(" ", "_")]
[#assign AN4 = conf.instance.eqadc_settings.an4[0]?upper_case?replace(" ", "_")]
[#assign AN5 = conf.instance.eqadc_settings.an5[0]?upper_case?replace(" ", "_")]
[#assign AN6 = conf.instance.eqadc_settings.an6[0]?upper_case?replace(" ", "_")]
[#assign AN7 = conf.instance.eqadc_settings.an7[0]?upper_case?replace(" ", "_")]
#define SPC5_ADC_PUDCR                      {ADC_PUDCR_${AN0},ADC_PUDCR_${AN1},ADC_PUDCR_${AN2},ADC_PUDCR_${AN3},ADC_PUDCR_${AN4},ADC_PUDCR_${AN5},ADC_PUDCR_${AN6},ADC_PUDCR_${AN7}}

/*
 * SERIAL driver system settings.
 */
#define SPC5_USE_ESCIA                      ${(conf.instance.esci_settings.esci0[0] == "Serial")?string?upper_case}
#define SPC5_USE_ESCIB                      ${(conf.instance.esci_settings.esci1[0] == "Serial")?string?upper_case}
#define SPC5_ESCIA_PRIORITY                 ${conf.instance.irq_priority_settings.esci0[0]}
#define SPC5_ESCIB_PRIORITY                 ${conf.instance.irq_priority_settings.esci1[0]}
