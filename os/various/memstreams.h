/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

/**
 * @file    memstreams.h
 * @brief   Memory streams structures and macros.
 
 * @addtogroup memory_streams
 * @{
 */

#ifndef _MEMSTREAMS_H_
#define _MEMSTREAMS_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   @p RamStream specific data.
 */
#define _memory_stream_data                                                 \
  _base_sequential_stream_data                                              \
  /* Pointer to the stream buffer.*/                                        \
  uint8_t               *buffer;                                            \
  /* Size of the stream.*/                                                  \
  size_t                size;                                               \
  /* Current end of stream.*/                                               \
  size_t                eos;                                                \
  /* Current read offset.*/                                                 \
  size_t                offset;

/**
 * @brief   @p MemStream virtual methods table.
 */
struct MemStreamVMT {
  _base_sequential_stream_methods
};

/**
 * @extends BaseSequentialStream
 *
 * @brief Memory stream object.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct MemStreamVMT *vmt;
  _memory_stream_data
} MemoryStream;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void msObjectInit(MemoryStream *msp, uint8_t *buffer, size_t size, size_t eos);
#ifdef __cplusplus
}
#endif

#endif /* _MEMSTREAMS_H_ */

/** @} */
