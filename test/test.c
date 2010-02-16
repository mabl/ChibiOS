/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

#include "ch.h"

#include "test.h"
#include "testthd.h"
#include "testsem.h"
#include "testmtx.h"
#include "testmsg.h"
#include "testmbox.h"
#include "testevt.h"
#include "testheap.h"
#include "testpools.h"
#include "testdyn.h"
#include "testqueues.h"
#include "testbmk.h"

/*
 * Array of all the test patterns.
 */
static const struct testcase **patterns[] = {
  patternthd,
  patternsem,
  patternmtx,
  patternmsg,
  patternmbox,
  patternevt,
  patternheap,
  patternpools,
  patterndyn,
  patternqueues,
  patternbmk,
  NULL
};

static bool_t local_fail, global_fail;
static unsigned failpoint;
static char tokens_buffer[MAX_TOKENS];
static char *tokp;

/*
 * Static working areas, the following areas can be used for threads or
 * used as temporary buffers.
 */
union test_buffers test;

/*
 * Pointers to the spawned threads.
 */
Thread *threads[MAX_THREADS];

/*
 * Pointers to the working areas.
 */
void * const wa[5] = {test.waT0, test.waT1, test.waT2, test.waT3, test.waT4};

/*
 * Console output.
 */
static BaseSequentialStream *bssp;

void test_printn(uint32_t n) {
  char buf[16], *p;

  if (!n)
    chSequentialStreamWrite(bssp, (uint8_t *)"0", 1);
  else {
    p = buf;
    while (n)
      *p++ = (n % 10) + '0', n /= 10;
    while (p > buf)
      chSequentialStreamWrite(bssp, (uint8_t *)--p, 1);
  }
}

void test_print(char *msgp) {

  while (*msgp)
    chSequentialStreamWrite(bssp, (uint8_t *)msgp++, 1);
}

void test_println(char *msgp) {

  test_print(msgp);
  chSequentialStreamWrite(bssp, (uint8_t *)"\r\n", 2);
}

/*
 * Tokens.
 */
static void clear_tokens(void) {

  tokp = tokens_buffer;
}

static void print_tokens(void) {
  char *cp = tokens_buffer;

  chSequentialStreamWrite(bssp, (uint8_t *)cp, tokp - cp);
}

void test_emit_token(char token) {

  chSysLock();
  *tokp++ = token;
  chSysUnlock();
}

/*
 * Assertions.
 */
bool_t _test_fail(unsigned point) {

  local_fail = TRUE;
  global_fail = TRUE;
  failpoint = point;
  return TRUE;
}

bool_t _test_assert(unsigned point, bool_t condition) {

  if (!condition)
    return _test_fail(point);
  return FALSE;
}

bool_t _test_assert_sequence(unsigned point, char *expected) {
  char *cp = tokens_buffer;
  while (cp < tokp) {
    if (*cp++ != *expected++)
     return _test_fail(point);
  }
  if (*expected)
    return _test_fail(point);
  clear_tokens();
  return FALSE;
}

bool_t _test_assert_time_window(unsigned point, systime_t start, systime_t end) {

  return _test_assert(point, chTimeIsWithin(start, end));
}

/*
 * Threads utils.
 */
void test_terminate_threads(void) {
  int i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i])
      chThdTerminate(threads[i]);
}

void test_wait_threads(void) {
  int i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i] != NULL) {
      chThdWait(threads[i]);
      threads[i] = NULL;
    }
}

#if CH_DBG_THREADS_PROFILING
void test_cpu_pulse(unsigned duration) {
  systime_t start, end, now;

  start = chThdSelf()->p_time;
  end = start + MS2ST(duration);
  do {
    now = chThdSelf()->p_time;
#if defined(SIMULATOR)
    ChkIntSources();
#endif
  }
  while (end > start ? (now >= start) && (now < end) :
                       (now >= start) || (now < end));
}
#endif

systime_t test_wait_tick(void) {

  chThdSleep(1);
  return chTimeNow();
}

/*
 * Timer utils.
 */
static VirtualTimer vt;
bool_t test_timer_done;

static void tmr(void *p) {
  (void)p;

  test_timer_done = TRUE;
}

void test_start_timer(unsigned ms) {

  systime_t duration = MS2ST(ms);
  test_timer_done = FALSE;
  chSysLock();
  chVTSetI(&vt, duration, tmr, NULL);
  chSysUnlock();
}

/*
 * Test suite execution.
 */
static void execute_test(const struct testcase *tcp) {
  int i;

  /* Initialization */
  clear_tokens();
  local_fail = FALSE;
  for (i = 0; i < MAX_THREADS; i++)
    threads[i] = NULL;

  if (tcp->setup != NULL)
    tcp->setup();
  tcp->execute();
  if (tcp->teardown != NULL)
    tcp->teardown();

  test_wait_threads();
}

static void print_line(void) {
  unsigned i;

  for (i = 0; i < 76; i++)
    chSequentialStreamWrite(bssp, (uint8_t *)"-", 1);
  chSequentialStreamWrite(bssp, (uint8_t *)"\r\n", 2);
}

msg_t TestThread(void *p) {
  int i, j;

  bssp = p;
  test_println("");
  test_println("*** ChibiOS/RT test suite");
  test_println("***");
  test_print("*** Kernel:       ");
  test_println(CH_KERNEL_VERSION);
  test_print("*** Architecture: ");
  test_println(CH_ARCHITECTURE_NAME);
#ifdef __GNUC__
  test_print("*** GCC Version:  ");
  test_println(__VERSION__);
#endif
  test_println("");

  global_fail = FALSE;
  i = 0;
  while (patterns[i]) {
    j = 0;
    while (patterns[i][j]) {
      print_line();
      test_print("--- Test Case ");
      test_printn(i + 1);
      test_print(".");
      test_printn(j + 1);
      test_print(" (");
      test_print(patterns[i][j]->gettest());
      test_println(")");
#if DELAY_BETWEEN_TESTS > 0
      chThdSleepMilliseconds(DELAY_BETWEEN_TESTS);
#endif
      execute_test(patterns[i][j]);
      if (local_fail) {
        test_print("--- Result: FAILURE (#");
        test_printn(failpoint);
        test_print(" [");
        print_tokens();
        test_println("])");
      }
      else
        test_println("--- Result: SUCCESS");
      j++;
    }
    i++;
  }
  print_line();
  test_println("");
  test_print("Final result: ");
  if (global_fail)
    test_println("FAILURE");
  else
    test_println("SUCCESS");

  return (msg_t)global_fail;
}
