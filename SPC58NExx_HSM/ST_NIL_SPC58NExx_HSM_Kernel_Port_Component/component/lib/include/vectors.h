/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,
              2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS 

    A limited, non exclusive, license has been granted to STMicroelectronics
    for use in SPC5xx micro controllers embedded firmware.
*/

/**
 * @file    vectors.h
 * @brief   ISR vector module header.
 *
 * @addtogroup VECTORS
 * @{
 */

#ifndef _VECTORS_H_
#define _VECTORS_H_

#include "ppcparams.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

#if !defined(__DOXYGEN__)
extern uint32_t _vectors[PPC_NUM_VECTORS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void _unhandled_irq(void);
#ifdef __cplusplus
}
#endif

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* _VECTORS_H_ */

/** @} */
