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
 * @file    chsys.c
 * @brief   System related code.
 *
 * @addtogroup system
 * @details System related APIs and services:
 *          - Initialization.
 *          - Locks.
 *          - Interrupt Handling.
 *          - Power Management.
 *          - Abnormal Termination.
 *          - Realtime counter.
 *          .
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if !CH_CFG_NO_IDLE_THREAD || defined(__DOXYGEN__)
/**
 * @brief   This function implements the idle thread infinite loop.
 * @details The function puts the processor in the lowest power mode capable
 *          to serve interrupts.<br>
 *          The priority is internally set to the minimum system value so
 *          that this thread is executed only if there are no other ready
 *          threads in the system.
 *
 * @param[in] p         the thread parameter, unused in this scenario
 */
static void _idle_thread(void *p) {

  (void)p;
  chRegSetThreadName("idle");
  while (true) {
    port_wait_for_interrupt();
    CH_CFG_IDLE_LOOP_HOOK();
  }
}
#endif /* CH_CFG_NO_IDLE_THREAD */

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   ChibiOS/RT initialization.
 * @details After executing this function the current instructions stream
 *          becomes the main thread.
 * @pre     Interrupts must disabled before invoking this function.
 * @post    The main thread is created with priority @p NORMALPRIO and
 *          interrupts are enabled.
 *
 * @special
 */
void chSysInit(void) {
#if CH_DBG_ENABLE_STACK_CHECK
  extern stkalign_t __main_thread_stack_base__;
#endif

  port_init();
  _scheduler_init();
  _vt_init();
#if CH_CFG_USE_TM
  _tm_init();
#endif
#if CH_CFG_USE_MEMCORE
  _core_init();
#endif
#if CH_CFG_USE_HEAP
  _heap_init();
#endif
#if CH_DBG_STATISTICS
  _stats_init();
#endif
#if CH_DBG_ENABLE_TRACE
  _dbg_trace_init();
#endif

#if !CH_CFG_NO_IDLE_THREAD
  /* Now this instructions flow becomes the main thread.*/
  setcurrp(_thread_init(&ch.mainthread, NORMALPRIO));
#else
  /* Now this instructions flow becomes the idle thread.*/
  setcurrp(_thread_init(&ch.mainthread, IDLEPRIO));
#endif

  currp->p_state = CH_STATE_CURRENT;
#if CH_DBG_ENABLE_STACK_CHECK
  /* This is a special case because the main thread thread_t structure is not
     adjacent to its stack area.*/
  currp->p_stklimit = &__main_thread_stack_base__;
#endif
  chSysEnable();

  /* Note, &ch_debug points to the string "main" if the registry is
     active, else the parameter is ignored.*/
  chRegSetThreadName((const char *)&ch_debug);

#if !CH_CFG_NO_IDLE_THREAD
  /* This thread has the lowest priority in the system, its role is just to
     serve interrupts in its context while keeping the lowest energy saving
     mode compatible with the system status.*/
  chThdCreateStatic(ch.idle_thread_wa, sizeof(ch.idle_thread_wa), IDLEPRIO,
                    (tfunc_t)_idle_thread, NULL);
#endif
}

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected, for example because a programming
 *          error in the application code that triggers an assertion while
 *          in debug mode.
 * @note    Can be invoked from any system state.
 *
 * @param[in] reason        pointer to an error string
 *
 * @special
 */
void chSysHalt(const char *reason) {

  port_disable();

#if defined(CH_CFG_SYSTEM_HALT_HOOK) || defined(__DOXYGEN__)
  CH_CFG_SYSTEM_HALT_HOOK(reason);
#endif

  /* Pointing to the passed message.*/
  ch.dbg.panic_msg = reason;

  /* Harmless infinite loop.*/
  while (true)
    ;
}

/**
 * @brief   Handles time ticks for round robin preemption and timer increments.
 * @details Decrements the remaining time quantum of the running thread
 *          and preempts it when the quantum is used up. Increments system
 *          time and manages the timers.
 * @note    The frequency of the timer determines the system tick granularity
 *          and, together with the @p CH_CFG_TIME_QUANTUM macro, the round robin
 *          interval.
 *
 * @iclass
 */
void chSysTimerHandlerI(void) {

  chDbgCheckClassI();

#if CH_CFG_TIME_QUANTUM > 0
  /* Running thread has not used up quantum yet? */
  if (currp->p_preempt > 0)
    /* Decrement remaining quantum.*/
    currp->p_preempt--;
#endif
#if CH_DBG_THREADS_PROFILING
  currp->p_time++;
#endif
  chVTDoTickI();
#if defined(CH_CFG_SYSTEM_TICK_HOOK)
  CH_CFG_SYSTEM_TICK_HOOK();
#endif
}

/**
 * @brief   Returns the execution status and enters a critical zone.
 * @details This functions enters into a critical zone and can be called
 *          from any context. Because its flexibility it is less efficient
 *          than @p chSysLock() which is preferable when the calling context
 *          is known.
 * @post    The system is in a critical zone.
 *
 * @return              The previous system status, the encoding of this
 *                      status word is architecture-dependent and opaque.
 *
 * @xclass
 */
syssts_t chSysGetStatusAndLockX(void) {

  syssts_t sts = port_get_irq_status();
  if (port_irq_enabled(sts)) {
    if (port_is_isr_context())
      chSysLockFromISR();
    else
      chSysLock();
  }
  return sts;
}

/**
 * @brief   Restores the specified execution status and leaves a critical zone.
 * @note    A call to @p chSchRescheduleS() is automatically performed
 *          if exiting the critical zone and if not in ISR context.
 *
 * @param[in] sts       the system status to be restored.
 *
 * @xclass
 */
void chSysRestoreStatusX(syssts_t sts) {

  if (port_irq_enabled(sts)) {
    if (port_is_isr_context())
      chSysUnlockFromISR();
    else {
      chSchRescheduleS();
      chSysUnlock();
    }
  }
}

#if PORT_SUPPORTS_RT || defined(__DOXYGEN__)
/**
 * @brief   Realtime window test.
 * @details This function verifies if the current realtime counter value
 *          lies within the specified range or not. The test takes care
 *          of the realtime counter wrapping to zero on overflow.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    This function is only available if the port layer supports the
 *          option @p PORT_SUPPORTS_RT.
 *
 * @param[in] cnt       the counter value to be tested
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
bool chSysIsCounterWithinX(rtcnt_t cnt, rtcnt_t start, rtcnt_t end) {

  return end > start ? (cnt >= start) && (cnt < end) :
                       (cnt >= start) || (cnt < end);
}

/**
 * @brief   Polled delay.
 * @note    The real delay is always few cycles in excess of the specified
 *          value.
 * @note    This function is only available if the port layer supports the
 *          option @p PORT_SUPPORTS_RT.
 *
 * @param[in] cycles    number of cycles
 *
 * @xclass
 */
void chSysPolledDelayX(rtcnt_t cycles) {
  rtcnt_t start = chSysGetRealtimeCounterX();
  rtcnt_t end  = start + cycles;
  while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end))
    ;
}
#endif /* PORT_SUPPORTS_RT */

/** @} */
