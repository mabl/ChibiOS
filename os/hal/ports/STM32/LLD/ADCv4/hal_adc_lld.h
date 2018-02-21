/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    ADCv4/hal_adc_lld.h
 * @brief   STM32 ADC subsystem low level driver header.
 *
 * @addtogroup ADC
 * @{
 */

#ifndef HAL_ADC_LLD_H
#define HAL_ADC_LLD_H

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Available analog channels
 * @{
 */
#define ADC_CHANNEL_IN0         0U  /**< @brief External analog input 0.    */
#define ADC_CHANNEL_IN1         1U  /**< @brief External analog input 1.    */
#define ADC_CHANNEL_IN2         2U  /**< @brief External analog input 2.    */
#define ADC_CHANNEL_IN3         3U  /**< @brief External analog input 3.    */
#define ADC_CHANNEL_IN4         4U  /**< @brief External analog input 4.    */
#define ADC_CHANNEL_IN5         5U  /**< @brief External analog input 5.    */
#define ADC_CHANNEL_IN6         6U  /**< @brief External analog input 6.    */
#define ADC_CHANNEL_IN7         7U  /**< @brief External analog input 7.    */
#define ADC_CHANNEL_IN8         8U  /**< @brief External analog input 8.    */
#define ADC_CHANNEL_IN9         9U  /**< @brief External analog input 9.    */
#define ADC_CHANNEL_IN10        10U /**< @brief External analog input 10.   */
#define ADC_CHANNEL_IN11        11U /**< @brief External analog input 11.   */
#define ADC_CHANNEL_IN12        12U /**< @brief External analog input 12.   */
#define ADC_CHANNEL_IN13        13U /**< @brief External analog input 13.   */
#define ADC_CHANNEL_IN14        14U /**< @brief External analog input 14.   */
#define ADC_CHANNEL_IN15        15U /**< @brief External analog input 15.   */
#define ADC_CHANNEL_IN16        16U /**< @brief External analog input 16.   */
#define ADC_CHANNEL_IN17        17U /**< @brief External analog input 17.   */
#define ADC_CHANNEL_IN18        18U /**< @brief External analog input 18.   */
/** @} */

/**
 * @name    Sampling rates
 * @{
 */
#if defined(STM32H7XX)
#define ADC_SMPR_SMP_1P5        0U  /**< @brief 9 cycles conversion time    */
#define ADC_SMPR_SMP_2P5        1U  /**< @brief 10 cycles conversion time.  */
#define ADC_SMPR_SMP_8P5        2U  /**< @brief 16 cycles conversion time.  */
#define ADC_SMPR_SMP_16P5       3U  /**< @brief 24 cycles conversion time.  */
#define ADC_SMPR_SMP_32P5       4U  /**< @brief 40 cycles conversion time.  */
#define ADC_SMPR_SMP_64P5       5U  /**< @brief 72 cycles conversion time.  */
#define ADC_SMPR_SMP_384P5      6U  /**< @brief 392 cycles conversion time. */
#define ADC_SMPR_SMP_810P5      7U  /**< @brief 818 cycles conversion time. */
#endif
/** @} */

/**
 * @name    CFGR register configuration helpers
 * @{
 */
#define ADC_CFGR_DMNGT_MASK             (3U << 0U)
#define ADC_CFGR_DMNGT_NODMA            (0U << 0U)
#define ADC_CFGR_DMNGT_ONESHOT          (1U << 0U)
#define ADC_CFGR_DMNGT_DFSDM            (2U << 0U)
#define ADC_CFGR_DMNGT_CIRCULAR         (3U << 0U)

#define ADC_CFGR_RES_MASK               (7U << 2U)
#define ADC_CFGR_RES_16BITS             (0U << 2U)
#define ADC_CFGR_RES_14BITS             (1U << 2U)
#define ADC_CFGR_RES_12BITS             (2U << 2U)
#define ADC_CFGR_RES_10BITS             (3U << 2U)
#define ADC_CFGR_RES_8BITS              (4U << 2U)

#define ADC_CFGR_EXTSEL_MASK            (15U << 5U)
#define ADC_CFGR_EXTSEL_SRC(n)          ((n) << 5U)

#define ADC_CFGR_EXTEN_MASK             (3U << 10U)
#define ADC_CFGR_EXTEN_DISABLED         (0U << 10U)
#define ADC_CFGR_EXTEN_RISING           (1U << 10U)
#define ADC_CFGR_EXTEN_FALLING          (2U << 10U)
#define ADC_CFGR_EXTEN_BOTH             (3U << 10U)

#define ADC_CFGR_DISCEN_MASK            (1U << 16U)
#define ADC_CFGR_DISCEN_DISABLED        (0U << 16U)
#define ADC_CFGR_DISCEN_ENABLED         (1U << 16U)

#define ADC_CFGR_DISCNUM_MASK           (7U << 17U)
#define ADC_CFGR_DISCNUM_VAL(n)         ((n) << 17U)
/** @} */

/**
 * @name    CCR register configuration helpers
 * @{
 */
#define ADC_CCR_DUAL_MASK               (31U << 0U)
#define ADC_CCR_DUAL_FIELD(n)           ((n) << 0U)
#define ADC_CCR_DELAY_MASK              (15U << 8U)
#define ADC_CCR_DELAY_FIELD(n)          ((n) << 8U)
#define ADC_CCR_DAMDF_MASK              (3U << 14U)
#define ADC_CCR_DAMDF_DISABLED          (0U << 14U)
#define ADC_CCR_DAMDF_HWORD             (2U << 14U)
#define ADC_CCR_DAMDF_WORD              (3U << 14U)
#define ADC_CCR_CKMODE_MASK             (3U << 16U)
#define ADC_CCR_CKMODE_ADCCK            (0U << 16U)
#define ADC_CCR_CKMODE_AHB_DIV1         (1U << 16U)
#define ADC_CCR_CKMODE_AHB_DIV2         (2U << 16U)
#define ADC_CCR_CKMODE_AHB_DIV4         (3U << 16U)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Enables the ADC1 and ADC2 master/slave mode.
 */
#if !defined(STM32_ADC_DUAL_MODE) || defined(__DOXYGEN__)
#define STM32_ADC_DUAL_MODE                 FALSE
#endif

/**
 * @brief   Makes the ADC samples type an 8bits one.
 * @note    10, 12, 14 and 16 bits sampling mode must not be used when this
 *          option is enabled.
 */
#if !defined(STM32_ADC_COMPACT_SAMPLES) || defined(__DOXYGEN__)
#define STM32_ADC_COMPACT_SAMPLES           FALSE
#endif

/**
 * @brief   ADC1/ADC2 driver enable switch.
 * @details If set to @p TRUE the support for ADC1/ADC2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_ADC_USE_ADC12) || defined(__DOXYGEN__)
#define STM32_ADC_USE_ADC12                 FALSE
#endif

/**
 * @brief   ADC3 driver enable switch.
 * @details If set to @p TRUE the support for ADC3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_ADC_USE_ADC3) || defined(__DOXYGEN__)
#define STM32_ADC_USE_ADC3                  FALSE
#endif

/**
 * @brief   ADC1/ADC2 DMA channel.
 */
#if !defined(STM32_ADC_ADC12_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_ADC_ADC12_DMA_CHANNEL         0
#endif

/**
 * @brief   ADC3 DMA channel.
 */
#if !defined(STM32_ADC_ADC3_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_ADC_ADC3_DMA_CHANNEL          1
#endif

/**
 * @brief   ADC1/ADC2 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_ADC_ADC12_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC12_DMA_PRIORITY        2
#endif

/**
 * @brief   ADC3 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_ADC_ADC3_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC3_DMA_PRIORITY         2
#endif

/**
 * @brief   ADC1/ADC2 interrupt priority level setting.
 */
#if !defined(STM32_ADC_ADC12_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC12_IRQ_PRIORITY        5
#endif

/**
 * @brief   ADC3 interrupt priority level setting.
 */
#if !defined(STM32_ADC_ADC3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC3_IRQ_PRIORITY         5
#endif

/**
 * @brief   ADC1/ADC2 clock source and mode.
 */
#if !defined(STM32_ADC_ADC12_CLOCK_MODE) || defined(__DOXYGEN__)
#define STM32_ADC_ADC12_CLOCK_MODE          ADC_CCR_CKMODE_AHB_DIV4
#endif

/**
 * @brief   ADC3 clock source and mode.
 */
#if !defined(STM32_ADC_ADC3_CLOCK_MODE) || defined(__DOXYGEN__)
#define STM32_ADC_ADC3_CLOCK_MODE           ADC_CCR_CKMODE_AHB_DIV4
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Supported devices checks.*/
#if !defined(STM32H7XX)
#error "ADCv4 only supports H7 STM32 devices"
#endif

/* Registry checks.*/
#if !defined(STM32_HAS_ADC1) || !defined(STM32_HAS_ADC2) ||                 \
    !defined(STM32_HAS_ADC3)
#error "STM32_ADC_USE_ADCx not defined in registry"
#endif

/* Units checks.*/
#if STM32_ADC_USE_ADC12 && !STM32_HAS_ADC1
#error "ADC1 not present in the selected device"
#endif

#if STM32_ADC_DUAL_MODE && !STM32_HAS_ADC2
#error "ADC2 not present in the selected device"
#endif

#if STM32_ADC_USE_ADC3 && !STM32_HAS_ADC3
#error "ADC3 not present in the selected device"
#endif

/* IRQ handlers checks.*/
#if STM32_HAS_ADC1 && !defined(STM32_ADC12_HANDLER)
#error "STM32_ADC12_HANDLER not defined in registry"
#endif

#if STM32_HAS_ADC2 && !defined(STM32_ADC12_HANDLER)
#error "STM32_ADC12_HANDLER not defined in registry"
#endif

#if STM32_HAS_ADC3 && !defined(STM32_ADC3_HANDLER)
#error "STM32_ADC3_HANDLER not defined in registry"
#endif

/* IRQ vector numbers checks.*/
#if STM32_HAS_ADC1 && !defined(STM32_ADC12_NUMBER)
#error "STM32_ADC12_NUMBER not defined in registry"
#endif

#if STM32_HAS_ADC2 && !defined(STM32_ADC12_NUMBER)
#error "STM32_ADC12_NUMBER not defined in registry"
#endif

#if STM32_HAS_ADC3 && !defined(STM32_ADC3_NUMBER)
#error "STM32_ADC3_NUMBER not defined in registry"
#endif

/* Units checks related to dual mode.*/
#if STM32_ADC_DUAL_MODE && STM32_ADC_USE_ADC1 && !STM32_HAS_ADC2
#error "ADC2 not present in the selected device, required for dual mode"
#endif

/* At least one ADC must be assigned.*/
#if !STM32_ADC_USE_ADC12 && !STM32_ADC_USE_ADC3
#error "ADC driver activated but no ADC peripheral assigned"
#endif

/* DMA channel range tests.*/
#if STM32_ADC_USE_ADC12 &&                                                  \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_ADC_ADC12_DMA_CHANNEL)
#error "Invalid DMA channel assigned to ADC12"
#endif

#if STM32_ADC_USE_ADC3 &&                                                   \
    !STM32_BDMA_IS_VALID_CHANNEL(STM32_ADC_ADC3_DMA_CHANNEL)
#error "Invalid DMA channel assigned to ADC3"
#endif

/* DMA priority tests.*/
#if STM32_ADC_USE_ADC12 &&                                                  \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_ADC_ADC12_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC1"
#endif

#if STM32_ADC_USE_ADC3 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_ADC_ADC3_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC3"
#endif

/* ADC IRQ priority tests.*/
#if STM32_ADC_USE_ADC12 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_ADC_ADC12_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC1"
#endif

#if STM32_ADC_USE_ADC3 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_ADC_ADC34_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC3"
#endif

/* ADC clock source checks.*/
#if STM32_ADC_ADC12_CLOCK_MODE == ADC_CCR_CKMODE_ADCCK
#define STM32_ADC12_CLOCK               STM32_ADCCLK
#elif STM32_ADC_ADC12_CLOCK_MODE == ADC_CCR_CKMODE_AHB_DIV1
#define STM32_ADC12_CLOCK               (STM32_HCLK / 1)
#elif STM32_ADC_ADC12_CLOCK_MODE == ADC_CCR_CKMODE_AHB_DIV2
#define STM32_ADC12_CLOCK               (STM32_HCLK / 2)
#elif STM32_ADC_ADC12_CLOCK_MODE == ADC_CCR_CKMODE_AHB_DIV4
#define STM32_ADC12_CLOCK               (STM32_HCLK / 4)
#else
#error "invalid clock mode selected for STM32_ADC_ADC12_CLOCK_MODE"
#endif

#if STM32_ADC_ADC3_CLOCK_MODE == ADC_CCR_CKMODE_ADCCK
#define STM32_ADC3_CLOCK                STM32_ADCCLK
#elif STM32_ADC_ADC3_CLOCK_MODE == ADC_CCR_CKMODE_AHB_DIV1
#define STM32_ADC3_CLOCK                (STM32_HCLK / 1)
#elif STM32_ADC_ADC3_CLOCK_MODE == ADC_CCR_CKMODE_AHB_DIV2
#define STM32_ADC3_CLOCK                (STM32_HCLK / 2)
#elif STM32_ADC_ADC3_CLOCK_MODE == ADC_CCR_CKMODE_AHB_DIV4
#define STM32_ADC3_CLOCK                (STM32_HCLK / 4)
#else
#error "invalid clock mode selected for STM32_ADC_ADC3_CLOCK_MODE"
#endif

#if STM32_ADC12_CLOCK > STM32_ADCCLK_MAX
#error "STM32_ADC12_CLOCK exceeding maximum frequency (STM32_ADCCLK_MAX)"
#endif

#if STM32_ADC3_CLOCK > STM32_ADCCLK_MAX
#error "STM32_ADC3_CLOCK exceeding maximum frequency (STM32_ADCCLK_MAX)"
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

#if STM32_ADC_USE_ADC12
#define STM32_ADC_DMA_REQUIRED
#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif
#endif

#if STM32_ADC_USE_ADC3
#define STM32_ADC_BDMA_REQUIRED
#if !defined(STM32_BDMA_REQUIRED)
#define STM32_BDMA_REQUIRED
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ADC sample data type.
 */
#if !STM32_ADC_COMPACT_SAMPLES || defined(__DOXYGEN__)
typedef uint16_t adcsample_t;
#else
typedef uint8_t adcsample_t;
#endif

/**
 * @brief   Channels number in a conversion group.
 */
typedef uint32_t adc_channels_num_t;

/**
 * @brief   Possible ADC failure causes.
 * @note    Error codes are architecture dependent and should not relied
 *          upon.
 */
typedef enum {
  ADC_ERR_DMAFAILURE = 0,                   /**< DMA operations failure.    */
  ADC_ERR_OVERFLOW = 1,                     /**< ADC overflow condition.    */
  ADC_ERR_AWD1 = 2,                         /**< Watchdog 1 triggered.      */
  ADC_ERR_AWD2 = 3,                         /**< Watchdog 2 triggered.      */
  ADC_ERR_AWD3 = 4                          /**< Watchdog 3 triggered.      */
} adcerror_t;

/**
 * @brief   Type of a structure representing an ADC driver.
 */
typedef struct ADCDriver ADCDriver;

/**
 * @brief   ADC notification callback type.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object triggering the
 *                      callback
 * @param[in] buffer    pointer to the most recent samples data
 * @param[in] n         number of buffer rows available starting from @p buffer
 */
typedef void (*adccallback_t)(ADCDriver *adcp, adcsample_t *buffer, size_t n);

/**
 * @brief   ADC error callback type.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object triggering the
 *                      callback
 * @param[in] err       ADC error code
 */
typedef void (*adcerrorcallback_t)(ADCDriver *adcp, adcerror_t err);

/**
 * @brief   Conversion group configuration structure.
 * @details This implementation-dependent structure describes a conversion
 *          operation.
 * @note    The use of this configuration structure requires knowledge of
 *          STM32 ADC cell registers interface, please refer to the STM32
 *          reference manual for details.
 */
typedef struct {
  /**
   * @brief   Enables the circular buffer mode for the group.
   */
  bool                      circular;
  /**
   * @brief   Number of the analog channels belonging to the conversion group.
   */
  adc_channels_num_t        num_channels;
  /**
   * @brief   Callback function associated to the group or @p NULL.
   */
  adccallback_t             end_cb;
  /**
   * @brief   Error callback or @p NULL.
   */
  adcerrorcallback_t        error_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   ADC CFGR register initialization data.
   * @note    The bits DMNGT are enforced internally to the driver, keep
   *          them to zero.
   * @note    The bits @p ADC_CFGR_CONT or @p ADC_CFGR_DISCEN must be
   *          specified in continuous mode or if the buffer depth is
   *          greater than one.
   */
  uint32_t                  cfgr;
#if STM32_ADC_DUAL_MODE || defined(__DOXYGEN__)
  /**
   * @brief   ADC CCR register initialization data.
   * @note    The bits CKMODE and DAMDF are enforced internally to the
   *          driver, keep them to zero.
   * @note    This field is only present in dual mode.
   */
  uint32_t                  ccr;
#endif
  /**
   * @brief   ADC PCSEL register initialization data.
   */
  uint32_t                  pcsel;
  /**
   * @brief   ADC LTR1 register initialization data.
   */
  uint32_t                  ltr1;
  /**
   * @brief   ADC HTR1 register initialization data.
   */
  uint32_t                  htr1;
  /**
   * @brief   ADC LTR2 register initialization data.
   */
  uint32_t                  ltr2;
  /**
   * @brief   ADC HTR2 register initialization data.
   */
  uint32_t                  htr2;
  /**
   * @brief   ADC LTR3 register initialization data.
   */
  uint32_t                  ltr3;
  /**
   * @brief   ADC HTR3 register initialization data.
   */
  uint32_t                  htr3;
  /**
   * @brief   ADC SMPRx registers initialization data.
   */
  uint32_t                  smpr[2];
  /**
   * @brief   ADC SQRx register initialization data.
   */
  uint32_t                  sqr[4];
#if STM32_ADC_DUAL_MODE || defined(__DOXYGEN__)
  /**
   * @brief   Slave ADC PCSEL register initialization data.
   */
  uint32_t                  spcsel;
  /**
   * @brief   Slave ADC LTR1 register initialization data.
   */
  uint32_t                  sltr1;
  /**
   * @brief   Slave ADC HTR1 register initialization data.
   */
  uint32_t                  shtr1;
  /**
   * @brief   Slave ADC LTR2 register initialization data.
   */
  uint32_t                  sltr2;
  /**
   * @brief   Slave ADC HTR2 register initialization data.
   */
  uint32_t                  shtr2;
  /**
   * @brief   Slave ADC LTR3 register initialization data.
   */
  uint32_t                  sltr3;
  /**
   * @brief   Slave ADC HTR3 register initialization data.
   */
  uint32_t                  shtr3;
  /**
   * @brief   Slave ADC SMPRx registers initialization data.
   * @note    This field is only present in dual mode.
   */
  uint32_t                  ssmpr[2];
  /**
   * @brief   Slave ADC SQRx register initialization data.
   * @note    This field is only present in dual mode.
   */
  uint32_t                  ssqr[4];
#endif /* STM32_ADC_DUAL_MODE */
} ADCConversionGroup;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief   ADC DIFSEL register initialization data.
   */
  uint32_t                  difsel;
} ADCConfig;

/**
 * @brief   Structure representing an ADC driver.
 */
struct ADCDriver {
  /**
   * @brief   Driver state.
   */
  adcstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const ADCConfig           *config;
  /**
   * @brief   Current samples buffer pointer or @p NULL.
   */
  adcsample_t               *samples;
  /**
   * @brief   Current samples buffer depth or @p 0.
   */
  size_t                    depth;
  /**
   * @brief   Current conversion group pointer or @p NULL.
   */
  const ADCConversionGroup  *grpp;
#if ADC_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t        thread;
#endif
#if ADC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* ADC_USE_MUTUAL_EXCLUSION */
#if defined(ADC_DRIVER_EXT_FIELDS)
  ADC_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Pointer to the master ADCx registers block.
   */
  ADC_TypeDef               *adcm;
#if STM32_ADC_DUAL_MODE || defined(__DOXYGEN__)
  /**
   * @brief   Pointer to the slave ADCx registers block.
   */
  ADC_TypeDef               *adcs;
#endif /* STM32_ADC_DUAL_MODE */
  /**
   * @brief   Pointer to the common ADCx_y registers block.
   */
  ADC_Common_TypeDef        *adcc;
  /**
   * @brief   Union of the DMA data streams.
   */
  union {
#if defined(STM32_ADC_DMA_REQUIRED) || defined(__DOXYGEN__)
    /**
     * @brief   DMA stream.
     */
    const stm32_dma_stream_t  *dma;
#endif
#if defined(STM32_ADC_BDMA_REQUIRED) || defined(__DOXYGEN__)
    /**
     * @brief   BDMA stream.
     */
    const stm32_bdma_stream_t  *bdma;
#endif
  } data;
  /**
   * @brief   DMA mode bit mask.
   */
  uint32_t                  dmamode;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Sequences building helper macros
 * @{
 */
/**
 * @brief   Number of channels in a conversion sequence.
 */
#define ADC_SQR1_NUM_CH(n)      (((n) - 1U) << 0U)

#define ADC_SQR1_SQ1_N(n)       ((n) << 6U) /**< @brief 1st channel in seq. */
#define ADC_SQR1_SQ2_N(n)       ((n) << 12U)/**< @brief 2nd channel in seq. */
#define ADC_SQR1_SQ3_N(n)       ((n) << 18U)/**< @brief 3rd channel in seq. */
#define ADC_SQR1_SQ4_N(n)       ((n) << 24U)/**< @brief 4th channel in seq. */

#define ADC_SQR2_SQ5_N(n)       ((n) << 0U) /**< @brief 5th channel in seq. */
#define ADC_SQR2_SQ6_N(n)       ((n) << 6U) /**< @brief 6th channel in seq. */
#define ADC_SQR2_SQ7_N(n)       ((n) << 12U)/**< @brief 7th channel in seq. */
#define ADC_SQR2_SQ8_N(n)       ((n) << 18U)/**< @brief 8th channel in seq. */
#define ADC_SQR2_SQ9_N(n)       ((n) << 24U)/**< @brief 9th channel in seq. */

#define ADC_SQR3_SQ10_N(n)      ((n) << 0U) /**< @brief 10th channel in seq.*/
#define ADC_SQR3_SQ11_N(n)      ((n) << 6U) /**< @brief 11th channel in seq.*/
#define ADC_SQR3_SQ12_N(n)      ((n) << 12U)/**< @brief 12th channel in seq.*/
#define ADC_SQR3_SQ13_N(n)      ((n) << 18U)/**< @brief 13th channel in seq.*/
#define ADC_SQR3_SQ14_N(n)      ((n) << 24U)/**< @brief 14th channel in seq.*/

#define ADC_SQR4_SQ15_N(n)      ((n) << 0U) /**< @brief 15th channel in seq.*/
#define ADC_SQR4_SQ16_N(n)      ((n) << 6U) /**< @brief 16th channel in seq.*/
/** @} */

/**
 * @name    Sampling rate settings helper macros
 * @{
 */
#define ADC_SMPR1_SMP_AN0(n)    ((n) << 0U) /**< @brief AN0 sampling time.  */
#define ADC_SMPR1_SMP_AN1(n)    ((n) << 3U) /**< @brief AN1 sampling time.  */
#define ADC_SMPR1_SMP_AN2(n)    ((n) << 6U) /**< @brief AN2 sampling time.  */
#define ADC_SMPR1_SMP_AN3(n)    ((n) << 9U) /**< @brief AN3 sampling time.  */
#define ADC_SMPR1_SMP_AN4(n)    ((n) << 12U)/**< @brief AN4 sampling time.  */
#define ADC_SMPR1_SMP_AN5(n)    ((n) << 15U)/**< @brief AN5 sampling time.  */
#define ADC_SMPR1_SMP_AN6(n)    ((n) << 18U)/**< @brief AN6 sampling time.  */
#define ADC_SMPR1_SMP_AN7(n)    ((n) << 21U)/**< @brief AN7 sampling time.  */
#define ADC_SMPR1_SMP_AN8(n)    ((n) << 24U)/**< @brief AN8 sampling time.  */
#define ADC_SMPR1_SMP_AN9(n)    ((n) << 27U)/**< @brief AN9 sampling time.  */

#define ADC_SMPR2_SMP_AN10(n)   ((n) << 0U) /**< @brief AN10 sampling time. */
#define ADC_SMPR2_SMP_AN11(n)   ((n) << 3U) /**< @brief AN11 sampling time. */
#define ADC_SMPR2_SMP_AN12(n)   ((n) << 6U) /**< @brief AN12 sampling time. */
#define ADC_SMPR2_SMP_AN13(n)   ((n) << 9U) /**< @brief AN13 sampling time. */
#define ADC_SMPR2_SMP_AN14(n)   ((n) << 12U)/**< @brief AN14 sampling time. */
#define ADC_SMPR2_SMP_AN15(n)   ((n) << 15U)/**< @brief AN15 sampling time. */
#define ADC_SMPR2_SMP_AN16(n)   ((n) << 18U)/**< @brief AN16 sampling time. */
#define ADC_SMPR2_SMP_AN17(n)   ((n) << 21U)/**< @brief AN17 sampling time. */
#define ADC_SMPR2_SMP_AN18(n)   ((n) << 24U)/**< @brief AN18 sampling time. */
#define ADC_SMPR2_SMP_AN19(n)   ((n) << 27U)/**< @brief AN19 sampling time. */
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_ADC_USE_ADC12 && !defined(__DOXYGEN__)
extern ADCDriver ADCD1;
#endif

#if STM32_ADC_USE_ADC3 && !defined(__DOXYGEN__)
extern ADCDriver ADCD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void adc_lld_init(void);
  void adc_lld_start(ADCDriver *adcp);
  void adc_lld_stop(ADCDriver *adcp);
  void adc_lld_start_conversion(ADCDriver *adcp);
  void adc_lld_stop_conversion(ADCDriver *adcp);
  void adcSTM32EnableVREF(ADCDriver *adcp);
  void adcSTM32DisableVREF(ADCDriver *adcp);
  void adcSTM32EnableTS(ADCDriver *adcp);
  void adcSTM32DisableTS(ADCDriver *adcp);
  void adcSTM32EnableVBAT(ADCDriver *adcp);
  void adcSTM32DisableVBAT(ADCDriver *adcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ADC */

#endif /* HAL_ADC_LLD_H */

/** @} */
