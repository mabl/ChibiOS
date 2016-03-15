/*
    HSM RTOS - Copyright (C) 2016 STMicroelectronics.
*/

/**
 * @file    SPC57EMxx_HSM/ppcparams.h
 * @brief   PowerPC parameters for the SPC57EMxx_HSM.
 *
 * @defgroup PPC_SPC57EMxx_HSM SPC57EMxx_HSM Specific Parameters
 * @ingroup PPC_SPECIFIC
 * @details This file contains the PowerPC specific parameters for the
 *          SPC57EMxx_HSM platform.
 * @{
 */

#ifndef _PPCPARAMS_H_
#define _PPCPARAMS_H_

/**
 * @brief   Family identification macro.
 */
#define PPC_SPC560Dxx

/**
 * @brief   Alternate identification macro.
 */
#define PPC_SPC57EMxx_HSM

/**
 * @brief   PPC core model.
 */
#define PPC_VARIANT                 PPC_VARIANT_e200z0

/**
 * @brief   Number of cores.
 */
#define PPC_CORE_NUMBER             1

/**
 * @brief   Number of writable bits in IVPR register.
 */
#define PPC_IVPR_BITS               20

/**
 * @brief   IVORx registers support.
 */
#define PPC_SUPPORTS_IVORS          FALSE

/**
 * @brief   Book E instruction set support.
 */
#define PPC_SUPPORTS_BOOKE          FALSE

/**
 * @brief   VLE instruction set support.
 */
#define PPC_SUPPORTS_VLE            TRUE

/**
 * @brief   Supports VLS Load/Store Multiple Volatile instructions.
 */
#define PPC_SUPPORTS_VLE_MULTI      TRUE

/**
 * @brief   Supports the decrementer timer.
 */
#define PPC_SUPPORTS_DECREMENTER    FALSE

/**
 * @brief   Number of interrupt sources.
 */
#define PPC_NUM_VECTORS             64

#endif /* _PPCPARAMS_H_ */

/** @} */
