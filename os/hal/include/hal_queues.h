/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    hal_queues.h
 * @brief   I/O Queues macros and structures.
 *
 * @addtogroup HAL_QUEUES
 * @{
 */

#ifndef _HAL_QUEUES_H_
#define _HAL_QUEUES_H_

/* The ChibiOS/RT kernel provides the following definitions by itself, this
   check is performed in order to avoid conflicts. */
#if !defined(_CHIBIOS_RT_) || !CH_CFG_USE_QUEUES || defined(__DOXYGEN__)

/**
 * @name    Queue functions returned status value
 * @{
 */
#define Q_OK            MSG_OK      /**< @brief Operation successful.       */
#define Q_TIMEOUT       MSG_TIMEOUT /**< @brief Timeout condition.          */
#define Q_RESET         MSG_RESET   /**< @brief Queue has been reset.       */
#define Q_EMPTY         -3          /**< @brief Queue empty.                */
#define Q_FULL          -4          /**< @brief Queue full,                 */
/** @} */

/**
 * @brief   Type of a generic I/O queue structure.
 */
typedef struct io_queue io_queue_t;

/**
 * @brief   Queue notification callback type.
 *
 * @param[in] qp        the queue pointer.
 */
typedef void (*qnotify_t)(io_queue_t *qp);

/**
 * @brief   Generic I/O queue structure.
 * @details This structure represents a generic Input or Output asymmetrical
 *          queue. The queue is asymmetrical because one end is meant to be
 *          accessed from a thread context, and thus can be blocking, the other
 *          end is accessible from interrupt handlers or from within a kernel
 *          lock zone and is non-blocking.
 */
struct io_queue {
  threads_queue_t       q_waiting;  /**< @brief Waiting thread.             */
  size_t                q_counter;  /**< @brief Resources counter.          */
  uint8_t               *q_buffer;  /**< @brief Pointer to the queue buffer.*/
  uint8_t               *q_top;     /**< @brief Pointer to the first
                                         location after the buffer.         */
  uint8_t               *q_wrptr;   /**< @brief Write pointer.              */
  uint8_t               *q_rdptr;   /**< @brief Read pointer.               */
  qnotify_t             q_notify;   /**< @brief Data notification callback. */
  void                  *q_link;    /**< @brief Application defined field.  */
};

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the queue's buffer size.
 *
 * @param[in] qp        pointer to a @p io_queue_t structure.
 * @return              The buffer size.
 *
 * @iclass
 */
#define qSizeI(qp) ((size_t)((qp)->q_top - (qp)->q_buffer))

/**
 * @brief   Queue space.
 * @details Returns the used space if used on an input queue or the empty
 *          space if used on an output queue.
 *
 * @param[in] qp        pointer to a @p io_queue_t structure.
 * @return              The buffer space.
 *
 * @iclass
 */
#define qSpaceI(qp) ((qp)->q_counter)

/**
 * @brief   Returns the queue application-defined link.
 * @note    This function can be called in any context.
 *
 * @param[in] qp        pointer to a @p io_queue_t structure.
 * @return              The application-defined link.
 *
 * @special
 */
#define qGetLink(qp) ((qp)->q_link)
/** @} */

/**
 * @extends io_queue_t
 *
 * @brief   Type of an input queue structure.
 * @details This structure represents a generic asymmetrical input queue.
 *          Writing to the queue is non-blocking and can be performed from
 *          interrupt handlers or from within a kernel lock zone.
 *          Reading the queue can be a blocking operation and is supposed to
 *          be performed by a system thread.
 */
typedef io_queue_t input_queue_t;

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the filled space into an input queue.
 *
 * @param[in] iqp       pointer to an @p input_queue_t structure
 * @return              The number of full bytes in the queue.
 * @retval 0            if the queue is empty.
 *
 * @iclass
 */
#define iqGetFullI(iqp) qSpaceI(iqp)

/**
 * @brief   Returns the empty space into an input queue.
 *
 * @param[in] iqp       pointer to an @p input_queue_t structure
 * @return              The number of empty bytes in the queue.
 * @retval 0            if the queue is full.
 *
 * @iclass
 */
#define iqGetEmptyI(iqp) (qSizeI(iqp) - qSpaceI(iqp))

/**
 * @brief   Evaluates to @p TRUE if the specified input queue is empty.
 *
 * @param[in] iqp       pointer to an @p input_queue_t structure.
 * @return              The queue status.
 * @retval FALSE        if the queue is not empty.
 * @retval TRUE         if the queue is empty.
 *
 * @iclass
 */
#define iqIsEmptyI(iqp) ((bool)(qSpaceI(iqp) <= 0))

/**
 * @brief   Evaluates to @p TRUE if the specified input queue is full.
 *
 * @param[in] iqp       pointer to an @p input_queue_t structure.
 * @return              The queue status.
 * @retval FALSE        if the queue is not full.
 * @retval TRUE         if the queue is full.
 *
 * @iclass
 */
#define iqIsFullI(iqp) ((bool)(((iqp)->q_wrptr == (iqp)->q_rdptr) &&        \
                               ((iqp)->q_counter != 0)))

/**
 * @brief   Input queue read.
 * @details This function reads a byte value from an input queue. If the queue
 *          is empty then the calling thread is suspended until a byte arrives
 *          in the queue.
 *
 * @param[in] iqp       pointer to an @p input_queue_t structure
 * @return              A byte value from the queue.
 * @retval Q_RESET      if the queue has been reset.
 *
 * @api
 */
#define iqGet(iqp) iqGetTimeout(iqp, TIME_INFINITE)
/** @} */

/**
 * @brief   Data part of a static input queue initializer.
 * @details This macro should be used when statically initializing an
 *          input queue that is part of a bigger structure.
 *
 * @param[in] name      the name of the input queue variable
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] inotify   input notification callback pointer
 * @param[in] link      application defined pointer
 */
#define _INPUTQUEUE_DATA(name, buffer, size, inotify, link) {               \
  NULL,                                                                     \
  0,                                                                        \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer) + (size),                                             \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer),                                                      \
  (inotify),                                                                \
  (link)                                                                    \
}

/**
 * @brief   Static input queue initializer.
 * @details Statically initialized input queues require no explicit
 *          initialization using @p iqInit().
 *
 * @param[in] name      the name of the input queue variable
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] inotify   input notification callback pointer
 * @param[in] link      application defined pointer
 */
#define INPUTQUEUE_DECL(name, buffer, size, inotify, link)                  \
  input_queue_t name = _INPUTQUEUE_DATA(name, buffer, size, inotify, link)

/**
 * @extends io_queue_t
 *
 * @brief   Type of an output queue structure.
 * @details This structure represents a generic asymmetrical output queue.
 *          Reading from the queue is non-blocking and can be performed from
 *          interrupt handlers or from within a kernel lock zone.
 *          Writing the queue can be a blocking operation and is supposed to
 *          be performed by a system thread.
 */
typedef io_queue_t output_queue_t;

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the filled space into an output queue.
 *
 * @param[in] oqp       pointer to an @p output_queue_t structure
 * @return              The number of full bytes in the queue.
 * @retval 0            if the queue is empty.
 *
 * @iclass
 */
#define oqGetFullI(oqp) (qSizeI(oqp) - qSpaceI(oqp))

/**
 * @brief   Returns the empty space into an output queue.
 *
 * @param[in] oqp       pointer to an @p output_queue_t structure
 * @return              The number of empty bytes in the queue.
 * @retval 0            if the queue is full.
 *
 * @iclass
 */
#define oqGetEmptyI(oqp) qSpaceI(oqp)

/**
 * @brief   Evaluates to @p TRUE if the specified output queue is empty.
 *
 * @param[in] oqp       pointer to an @p output_queue_t structure.
 * @return              The queue status.
 * @retval FALSE        if the queue is not empty.
 * @retval TRUE         if the queue is empty.
 *
 * @iclass
 */
#define oqIsEmptyI(oqp) ((bool)(((oqp)->q_wrptr == (oqp)->q_rdptr) &&       \
                                ((oqp)->q_counter != 0)))

/**
 * @brief   Evaluates to @p TRUE if the specified output queue is full.
 *
 * @param[in] oqp       pointer to an @p output_queue_t structure.
 * @return              The queue status.
 * @retval FALSE        if the queue is not full.
 * @retval TRUE         if the queue is full.
 *
 * @iclass
 */
#define oqIsFullI(oqp) ((bool)(qSpaceI(oqp) <= 0))

/**
 * @brief   Output queue write.
 * @details This function writes a byte value to an output queue. If the queue
 *          is full then the calling thread is suspended until there is space
 *          in the queue.
 *
 * @param[in] oqp       pointer to an @p output_queue_t structure
 * @param[in] b         the byte value to be written in the queue
 * @return              The operation status.
 * @retval Q_OK         if the operation succeeded.
 * @retval Q_RESET      if the queue has been reset.
 *
 * @api
 */
#define oqPut(oqp, b) oqPutTimeout(oqp, b, TIME_INFINITE)
 /** @} */

/**
 * @brief   Data part of a static output queue initializer.
 * @details This macro should be used when statically initializing an
 *          output queue that is part of a bigger structure.
 *
 * @param[in] name      the name of the output queue variable
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] onotify   output notification callback pointer
 * @param[in] link      application defined pointer
 */
#define _OUTPUTQUEUE_DATA(name, buffer, size, onotify, link) {              \
  NULL,                                                                     \
  (size),                                                                   \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer) + (size),                                             \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer),                                                      \
  (onotify),                                                                \
  (link)                                                                    \
}

/**
 * @brief   Static output queue initializer.
 * @details Statically initialized output queues require no explicit
 *          initialization using @p oqInit().
 *
 * @param[in] name      the name of the output queue variable
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] onotify   output notification callback pointer
 * @param[in] link      application defined pointer
 */
#define OUTPUTQUEUE_DECL(name, buffer, size, onotify, link)                 \
  output_queue_t name = _OUTPUTQUEUE_DATA(name, buffer, size, onotify, link)

#ifdef __cplusplus
extern "C" {
#endif
  void iqObjectInit(input_queue_t *iqp, uint8_t *bp, size_t size,
                    qnotify_t infy, void *link);
  void iqResetI(input_queue_t *iqp);
  msg_t iqPutI(input_queue_t *iqp, uint8_t b);
  msg_t iqGetTimeout(input_queue_t *iqp, systime_t time);
  size_t iqReadTimeout(input_queue_t *iqp, uint8_t *bp,
                       size_t n, systime_t time);

  void oqObjectInit(output_queue_t *oqp, uint8_t *bp, size_t size,
                    qnotify_t onfy, void *link);
  void oqResetI(output_queue_t *oqp);
  msg_t oqPutTimeout(output_queue_t *oqp, uint8_t b, systime_t time);
  msg_t oqGetI(output_queue_t *oqp);
  size_t oqWriteTimeout(output_queue_t *oqp, const uint8_t *bp,
                        size_t n, systime_t time);
#ifdef __cplusplus
}
#endif

#else /* defined(_CHIBIOS_RT_) && CH_CFG_USE_QUEUES */

/* If ChibiOS is being used and its own queues subsystem is activated then
   this module will use the ChibiOS queues code.*/
#define qSizeI(qp)                          chQSizeI(qp)
#define qSpaceI(qp)                         chQSpaceI(qp)
#define qGetLink(qp)                        chQGetLinkX(qp)
#define iqGetFullI(iqp)                     chIQGetFullI(iqp)
#define iqGetEmptyI(iqp)                    chIQGetEmptyI(iqp)
#define iqIsEmptyI(iqp)                     chIQIsEmptyI(iqp)
#define iqIsFullI(iqp)                      chIQIsFullI(iqp)
#define iqGet(iqp)                          chIQGet(iqp)
#define oqGetFullI(oqp)                     chOQGetFullI(oqp)
#define oqGetEmptyI(oqp)                    chOQGetEmptyI(oqp)
#define oqIsEmptyI(oqp)                     chOQIsEmptyI(oqp)
#define oqIsFullI(oqp)                      chOQIsFullI(oqp)
#define oqPut(oqp, b)                       chOQPut(oqp, b)
#define iqObjectInit(iqp, bp, size, infy, link)                             \
  chIQObjectInit(iqp, bp, size, infy, link)
#define iqResetI(iqp)                       chIQResetI(iqp)
#define iqPutI(iqp, b)                      chIQPutI(iqp, b)
#define iqGetTimeout(iqp, time)             chIQGetTimeout(iqp, time)
#define iqReadTimeout(iqp, bp, n, time)     chIQReadTimeout(iqp, bp, n, time)
#define oqObjectInit(oqp, bp, size, onfy, link)                             \
  chOQObjectInit(oqp, bp, size, onfy, link)
#define oqResetI(oqp)                       chOQResetI(oqp)
#define oqPutTimeout(oqp, b, time)          chOQPutTimeout(oqp, b, time)
#define oqGetI(oqp)                         chOQGetI(oqp)
#define oqWriteTimeout(oqp, bp, n, time)    chOQWriteTimeout(oqp, bp, n, time)

#endif /* defined(_CHIBIOS_RT_) && CH_CFG_USE_QUEUES */

#endif /* _HAL_QUEUES_H_ */

/** @} */
