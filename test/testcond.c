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

#include <ch.h>

#include "test.h"

static CondVar c1;

static char *cond1_gettest(void) {

  return "CondVar, signal test";
}

static void cond1_setup(void) {

  chCondInit(&c1);
}

static void cond1_teardown(void) {
}

static msg_t thread1(void *p) {

  chCondLock(&c1);
  chCondWait();
  test_emit_token(*(char *)p);
  chCondUnlock();
  return 0;
}

static void cond1_execute(void) {

  // Bacause priority inheritance.
  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreate(prio+1, 0, wa[0], STKSIZE, thread1, "C");
  threads[1] = chThdCreate(prio+2, 0, wa[1], STKSIZE, thread1, "B");
  threads[2] = chThdCreate(prio+3, 0, wa[2], STKSIZE, thread1, "A");
  test_assert(prio == chThdGetPriority(), "priority return failure");
  int i = 6;
  while (i--)
  {
    chCondLock(&c1);
    chCondSignal();
    chCondUnlock();
  }
  test_wait_threads();
  test_assert_sequence("ABC");
}

const struct testcase testcond1 = {
  cond1_gettest,
  cond1_setup,
  cond1_teardown,
  cond1_execute
};

static char *cond2_gettest(void) {

  return "CondVar, broadcast test";
}

static void cond2_execute(void) {

  // Bacause priority inheritance.
  tprio_t prio = chThdGetPriority();
  //threads[0] = chThdCreate(prio+1, 0, wa[0], STKSIZE, thread1, "C");
  //threads[1] = chThdCreate(prio+2, 0, wa[1], STKSIZE, thread1, "B");
  //threads[2] = chThdCreate(prio+3, 0, wa[2], STKSIZE, thread1, "A");
  test_assert(prio == chThdGetPriority(), "priority return failure");
  chCondLock(&c1);
  //chCondBroadcast();
  chCondUnlock();
  //test_wait_threads();
  //test_assert_sequence("ABCDE");
}

const struct testcase testcond2 = {
  cond2_gettest,
  cond1_setup,
  cond1_teardown,
  cond2_execute
};

