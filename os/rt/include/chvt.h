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
 * @file    chvt.h
 * @brief   Time and Virtual Timers module macros and structures.
 *
 * @addtogroup time
 * @{
 */

#ifndef _CHVT_H_
#define _CHVT_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Special time constants
 * @{
 */
/**
 * @brief   Zero time specification for some functions with a timeout
 *          specification.
 * @note    Not all functions accept @p TIME_IMMEDIATE as timeout parameter,
 *          see the specific function documentation.
 */
#define TIME_IMMEDIATE  ((systime_t)0)

/**
 * @brief   Infinite time specification for all functions with a timeout
 *          specification.
 * @note    Not all functions accept @p TIME_INFINITE as timeout parameter,
 *          see the specific function documentation.
 */
#define TIME_INFINITE   ((systime_t)-1)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (CH_CFG_ST_RESOLUTION != 16) && (CH_CFG_ST_RESOLUTION != 32)
#error "invalid CH_CFG_ST_RESOLUTION specified, must be 16 or 32"
#endif

#if CH_CFG_ST_FREQUENCY <= 0
#error "invalid CH_CFG_ST_FREQUENCY specified, must be greater than zero"
#endif

#if (CH_CFG_ST_TIMEDELTA < 0) || (CH_CFG_ST_TIMEDELTA == 1)
#error "invalid CH_CFG_ST_TIMEDELTA specified, must "                       \
       "be zero or greater than one"
#endif

#if (CH_CFG_ST_TIMEDELTA > 0) && (CH_CFG_TIME_QUANTUM > 0)
#error "CH_CFG_TIME_QUANTUM not supported in tickless mode"
#endif

#if (CH_CFG_ST_TIMEDELTA > 0) && CH_DBG_THREADS_PROFILING
#error "CH_DBG_THREADS_PROFILING not supported in tickless mode"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Time conversion utilities
 * @{
 */
/**
 * @brief   Seconds to system ticks.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define S2ST(sec)                                                           \
  ((systime_t)((uint32_t)(sec) * (uint32_t)CH_CFG_ST_FREQUENCY))

/**
 * @brief   Milliseconds to system ticks.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define MS2ST(msec)                                                         \
  ((systime_t)(((((uint32_t)(msec)) *                                       \
                 ((uint32_t)CH_CFG_ST_FREQUENCY) - 1UL) / 1000UL) + 1UL))

/**
 * @brief   Microseconds to system ticks.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define US2ST(usec)                                                         \
  ((systime_t)(((((uint32_t)(usec)) *                                       \
                 ((uint32_t)CH_CFG_ST_FREQUENCY) - 1UL) / 1000000UL) + 1UL))

/**
 * @brief   System ticks to seconds.
 * @details Converts from system ticks number to seconds.
 * @note    The result is rounded up to the next second boundary.
 *
 * @param[in] n         number of system ticks
 * @return              The number of seconds.
 *
 * @api
 */
#define ST2S(n) (((((n) - 1UL) * 1UL) / CH_CFG_ST_FREQUENCY) + 1UL)

/**
 * @brief   System ticks to milliseconds.
 * @details Converts from system ticks number to milliseconds.
 * @note    The result is rounded up to the next millisecond boundary.
 *
 * @param[in] n         number of system ticks
 * @return              The number of milliseconds.
 *
 * @api
 */
#define ST2MS(n) (((((n) - 1UL) * 1000UL) / CH_CFG_ST_FREQUENCY) + 1UL)

/**
 * @brief   System ticks to microseconds.
 * @details Converts from system ticks number to microseconds.
 * @note    The result is rounded up to the next microsecond boundary.
 *
 * @param[in] n         number of system ticks
 * @return              The number of microseconds.
 *
 * @api
 */
#define ST2US(n) (((((n) - 1UL) * 1000000UL) / CH_CFG_ST_FREQUENCY) + 1UL)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*
 * Virtual Timers APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void _vt_init(void);
  void chVTDoSetI(virtual_timer_t *vtp, systime_t delay,
                  vtfunc_t vtfunc, void *par);
  void chVTDoResetI(virtual_timer_t *vtp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Initializes a @p virtual_timer_t object.
 * @note    Initializing a timer object is not strictly required because
 *          the function @p chVTSetI() initializes the object too. This
 *          function is only useful if you need to perform a @p chVTIsArmed()
 *          check before calling @p chVTSetI().
 *
 * @param[out] vtp      the @p virtual_timer_t structure pointer
 *
 * @init
 */
static inline void chVTObjectInit(virtual_timer_t *vtp) {

  vtp->vt_func = NULL;
}

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p chSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 * @note    This function can be called from any context but its atomicity
 *          is not guaranteed on architectures whose word size is less than
 *          @p systime_t size.
 *
 * @return              The system time in ticks.
 *
 * @xclass
 */
static inline systime_t chVTGetSystemTimeX(void) {

#if CH_CFG_ST_TIMEDELTA == 0
  return ch.vtlist.vt_systime;
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  return port_timer_get_time();
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
}

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p chSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 *
 * @return              The system time in ticks.
 *
 * @api
 */
static inline systime_t chVTGetSystemTime(void) {
  systime_t systime;

  chSysLock();
  systime = chVTGetSystemTimeX();
  chSysUnlock();
  return systime;
}

/**
 * @brief   Returns the elapsed time since the specified start time.
 *
 * @param[in] start     start time
 * @return              The elapsed time.
 *
 * @xclass
 */
static inline systime_t chVTTimeElapsedSinceX(systime_t start) {

  return chVTGetSystemTimeX() - start;
}

/**
 * @brief   Checks if the specified time is within the specified time window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    This function can be called from any context.
 *
 * @param[in] time      the time to be verified
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
static inline bool chVTIsTimeWithinX(systime_t time,
                                     systime_t start,
                                     systime_t end) {

  return (bool)(time - start < end - start);
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
static inline bool chVTIsSystemTimeWithinX(systime_t start, systime_t end) {

  chDbgCheckClassI();

  return chVTIsTimeWithinX(chVTGetSystemTimeX(), start, end);
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @api
 */
static inline bool chVTIsSystemTimeWithin(systime_t start, systime_t end) {

  return chVTIsTimeWithinX(chVTGetSystemTime(), start, end);
}

/**
 * @brief   Returns @p true if the specified timer is armed.
 * @pre     The timer must have been initialized using @p chVTObjectInit()
 *          or @p chVTDoSetI().
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 * @return              true if the timer is armed.
 *
 * @iclass
 */
static inline bool chVTIsArmedI(virtual_timer_t *vtp) {

  chDbgCheckClassI();

  return (bool)(vtp->vt_func != NULL);
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer is first checked and disabled only if armed.
 * @pre     The timer must have been initialized using @p chVTObjectInit()
 *          or @p chVTDoSetI().
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @iclass
 */
static inline void chVTResetI(virtual_timer_t *vtp) {

  if (chVTIsArmedI(vtp))
    chVTDoResetI(vtp);
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer is first checked and disabled only if armed.
 * @pre     The timer must have been initialized using @p chVTObjectInit()
 *          or @p chVTDoSetI().
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @api
 */
static inline void chVTReset(virtual_timer_t *vtp) {

  chSysLock();
  chVTResetI(vtp);
  chSysUnlock();
}

/**
 * @brief   Enables a virtual timer.
 * @details If the virtual timer was already enabled then it is re-enabled
 *          using the new parameters.
 * @pre     The timer must have been initialized using @p chVTObjectInit()
 *          or @p chVTDoSetI().
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @iclass
 */
static inline void chVTSetI(virtual_timer_t *vtp, systime_t delay,
                            vtfunc_t vtfunc, void *par) {

  chVTResetI(vtp);
  chVTDoSetI(vtp, delay, vtfunc, par);
}

/**
 * @brief   Enables a virtual timer.
 * @details If the virtual timer was already enabled then it is re-enabled
 *          using the new parameters.
 * @pre     The timer must have been initialized using @p chVTObjectInit()
 *          or @p chVTDoSetI().
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @api
 */
static inline void chVTSet(virtual_timer_t *vtp, systime_t delay,
                           vtfunc_t vtfunc, void *par) {

  chSysLock();
  chVTSetI(vtp, delay, vtfunc, par);
  chSysUnlock();
}

/**
 * @brief   Virtual timers ticker.
 * @note    The system lock is released before entering the callback and
 *          re-acquired immediately after. It is callback's responsibility
 *          to acquire the lock if needed. This is done in order to reduce
 *          interrupts jitter when many timers are in use.
 *
 * @iclass
 */
static inline void chVTDoTickI(void) {

  chDbgCheckClassI();

#if CH_CFG_ST_TIMEDELTA == 0
  ch.vtlist.vt_systime++;
  if (&ch.vtlist != (virtual_timers_list_t *)ch.vtlist.vt_next) {
    virtual_timer_t *vtp;

    --ch.vtlist.vt_next->vt_delta;
    while (!(vtp = ch.vtlist.vt_next)->vt_delta) {
      vtfunc_t fn = vtp->vt_func;
      vtp->vt_func = (vtfunc_t)NULL;
      vtp->vt_next->vt_prev = (virtual_timer_t *)&ch.vtlist;
      ch.vtlist.vt_next = vtp->vt_next;
      chSysUnlockFromISR();
      fn(vtp->vt_par);
      chSysLockFromISR();
    }
  }
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  virtual_timer_t *vtp;
  systime_t now;

  while (true) {
    vtfunc_t fn;
    systime_t delta;

    /* Getting the current system time and calculating the time window since
       the last time has expired.*/
    now = chVTGetSystemTimeX();
    delta = now - ch.vtlist.vt_lasttime;

    /* The next element is outside the current time window, the loop
       is stopped here.*/
    if ((vtp = ch.vtlist.vt_next)->vt_delta > delta)
      break;

    /* The "last time" becomes this timer's expiration time.*/
    delta -= vtp->vt_delta;
    ch.vtlist.vt_lasttime += vtp->vt_delta;

    /* The timer is removed from the list and marked as non-armed.*/
    vtp->vt_next->vt_prev = (virtual_timer_t *)&ch.vtlist;
    ch.vtlist.vt_next = vtp->vt_next;
    fn = vtp->vt_func;
    vtp->vt_func = (vtfunc_t)NULL;

    /* The callback is invoked outside the kernel critical zone.*/
    chSysUnlockFromISR();
    fn(vtp->vt_par);
    chSysLockFromISR();
  }
  if (&ch.vtlist == (virtual_timers_list_t *)ch.vtlist.vt_next) {
    /* The list is empty, no tick event needed so the alarm timer
       is stopped.*/
    port_timer_stop_alarm();
  }
  else {
    /* Updating the alarm to the next deadline, deadline that must not be
       closer in time than the minimum time delta.*/
    if (vtp->vt_delta >= CH_CFG_ST_TIMEDELTA)
      port_timer_set_alarm(now + vtp->vt_delta);
    else
      port_timer_set_alarm(now + CH_CFG_ST_TIMEDELTA);
  }
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
}

#endif /* _CHVT_H_ */

/** @} */
