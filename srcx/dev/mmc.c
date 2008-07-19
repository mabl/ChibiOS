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
#include <chfs.h>

#include "mbr.h"
#include "mmc.h"
#include "mmc_spi.h"

#define NICE_WAITING
#define SECTORS_PER_BUFFER (CH_BUFFER_SIZE / 512)

/*
 * Partitions management.
 */
struct partition {
  uint32_t      start;
  uint32_t      length;
};

static bool_t initialized;
static struct partition partitions[4];
static unsigned lockmask;

#define LOCKED0 1
#define LOCKED1 2
#define LOCKED2 4
#define LOCKED3 8
#define LOCKED4 16
#define LOCKED_ANY (LOCKED0 | LOCKED1 | LOCKED2 | LOCKED3 | LOCKED4)

/*
 * Card polling.
 */
EventSource MMCInsertEventSource, MMCRemoveEventSource;
static VirtualTimer vt;
static int cnt;

#define card_stable() (!cnt)

void tmrfunc(void *par) {

  if (cnt) {
    if (card_present()) {
      if (!--cnt)
        initialized = FALSE;
        chEvtSendI(&MMCInsertEventSource);
    }
    else
      cnt = POLLING_INTERVAL;
  }
  else {
    if (!card_present()) {
      cnt = POLLING_INTERVAL;
      chEvtSendI(&MMCRemoveEventSource);
    }
  }
  chVTSetI(&vt, 10, tmrfunc, NULL);
}

static void wait(void) {
  int i;
  uint8_t buf[4];

  for (i = 0; i < 16; i++) {
    spi_receive(buf, 1);
    if (buf[0] == 0xFF)
      break;
  }
  /* Looks like it is a loooong wait.*/
  while (TRUE) {
    spi_receive(buf, 1);
    if (buf[0] == 0xFF)
      break;
#ifdef NICE_WAITING
    chThdSleep(1);      /* Trying to be nice with the other threads.*/
#endif
  }
}

static void send_hdr(uint8_t cmd, uint32_t arg) {
  uint8_t buf[6];

  /* Wait for the bus to become idle if a write operation was in progress. */
  wait();

  buf[0] = 0x40 | cmd;
  buf[1] = arg >> 24;
  buf[2] = arg >> 16;
  buf[3] = arg >> 8;
  buf[4] = arg;
  buf[5] = 0x95;        /* Valid for CMD0 ingnored by other commands. */
  sspRW(NULL, buf, 6);
}

static uint8_t recvr1(void) {
  int i;
  uint8_t r1[1];

  for (i = 0; i < 9; i++) {
    spi_receive(r1, 1);
    if (r1[0] != 0xFF)
      return r1[0];
  }
  return 0xFF;                  /* Timeout. */
}

static uint8_t send_command(uint8_t cmd, uint32_t arg) {
  uint8_t r1;

  spi_lock();
  send_hdr(cmd, arg);
  r1 = recvr1();
  spi_unlock();
  return r1;
}

static bool_t get_data(uint8_t *buf, uint32_t n) {
  int i;

  for (i = 0; i < MMC_WAIT_DATA; i++) {
    spi_receive(buf, 1);
    if (buf[0] == 0xFE) {
      spi_receive(buf, n);
      spi_ignore(2);            /* CRC ignored. */
      return FALSE;
    }
  }
  return TRUE;                  /* Timeout.*/
}

static bool_t init(void) {

  /* Starting initialization with slow clock mode. */
  spi_lowspeed();

  /* SPI mode selection. */
  spi_ignore(16);               /* 128 clock pulses without ~CS asserted. */
  int i = 0;
  while (TRUE) {
    if (send_command(CMDGOIDLE, 0) == 0x01)
      break;
    if (++i >= CMD0_RETRY)
      return TRUE;
    chThdSleep(10);
  }

  /* Initialization. */
  i = 0;
  while (TRUE) {
    uint8_t b = send_command(CMDINIT, 0);
    if (b == 0x00)
      break;
    if (b != 0x01)
      return TRUE;
    if (++i >= CMD1_RETRY)
      return TRUE;
    chThdSleep(10);
  }

  /* Initialization complete, full speed. */
  spi_highspeed();
  return FALSE;
}

static void sync(void) {
  uint8_t buf[4];

  /* Waits until the spi goes idle. */
  spi_lock();
  while (TRUE) {
    spi_receive(buf, 1);
    if (buf[0] == 0xFF)
      break;
#ifdef NICE_WAITING
    chThdSleep(1);      /* Trying to be nice with the other threads.*/
#endif
  }
  spi_unlock();
}

/*
 * Reads multiple sectors.
 * @param blknum the initial block
 * @param n the number of blocks
 * @param buf the pointer to the read buffer
 * @return \p TRUE if an error happened
 */
static bool_t read_sectors(uint8_t *buf, uint32_t blknum, uint32_t n) {
  static const uint8_t stopcmd[] = {0x40 | CMDSTOP, 0, 0, 0, 0, 1, 0xFF};

  spi_lock();
  send_hdr(CMDREADMULTIPLE, blknum << 9);
  if (recvr1() != 0x00) {
    sspReleaseBus();
    return TRUE;
  }
  while (n) {
    if (get_data(buf, 512)) {
      spi_unlock();
      return TRUE;
    }
    buf += 512;
    n--;
  }
  spi_send((uint8_t *)stopcmd, sizeof(stopcmd));
  if (recvr1() != 0x00) {
    spi_unlock();
    return TRUE;
  }
  spi_unlock();
  return FALSE;
}

/*
 * Writes multiple sectors.
 * @param blknum the initial block
 * @param n the number of blocks
 * @param buf the pointer to the write buffer
 * @return \p TRUE if an error happened
 * @note The function DOES NOT wait for the SPI bus to become free after
 *       sending the data, the bus check is done before sending commands to
 *       the card, this allows to not make useless busy waiting. The invoking
 *       thread can do other things while the data is being written.
 */
static bool_t write_sectors(uint8_t *buf, uint32_t blknum, uint32_t n) {
  static const uint8_t start[] = {0xFF, 0xFC},
                       stop[] = {0xFD, 0xFF};
  uint8_t b[4];

  spi_lock();
  send_hdr(CMDWRITEMULTIPLE, blknum << 9);
  if (recvr1() != 0x00) {
    spi_unlock();
    return TRUE;
  }
  while (n) {
    spi_send((uint8_t *)start, sizeof(start));  /* Data prologue.       */
    spi_send(buf, 512);                         /* Data.                */
    spi_ignore(2);                              /* CRC ignored.         */
    spi_receive(b, 1);
    if ((b[0] & 0x1F) != 0x05) {
      spi_unlock();
      return TRUE;
    }
    wait();
    buf += 512;
    n--;
  }
  spi_send((uint8_t *)stop, sizeof(stop));      /* Stops the transfer.  */
  spi_unlock();
  return FALSE;
}

void mmc_init(void) {

  spi_init();

  /*
   * Starts the card polling timer.
   */
  chEvtInit(&MMCInsertEventSource);
  chEvtInit(&MMCRemoveEventSource);
  chSysLock();
  chVTSetI(&vt, 10, tmrfunc, NULL);
  cnt = POLLING_INTERVAL;
  chSysUnlock();

  initialized = FALSE;
  lockmask = 0;
}

sysret_t mmc_open(unsigned minor, bool_t ro) {
  struct partition *pp;
  struct mbr_part *mpp;
  unsigned mask;

  if ((minor > 4) || !card_stable())
    return -ENXIO;
  if (minor == 0)
    mask = LOCKED_ANY;
  else
    mask = LOCKED0 | (1 << minor);
  if (lockmask & mask)
    return -ENXIO;

  if (!initialized) {
    struct buf *bp;
    struct mbr *mbrp;

    if (init())
      return -ENXIO;
    bp = getblk(NODEV, 0);
    mbrp = (struct mbr *)bp->b_dptr->u8;
    if (read_sectors(bp->b_dptr->u8, 0, 1) ||
        (mbrp->mbr_signature[0] != 0x55) ||
        (mbrp->mbr_signature[1] != 0xAA)) {
      brelse(bp);
      return -ENXIO;
    }
    for (pp = partitions, mpp = mbrp->partitions; pp < &partitions[4]; pp++, mpp++) {
      pp->start = get_lba(mpp->lba_first);
      pp->length = get_lba(mpp->lba_length);
    }
    brelse(bp);
    initialized = TRUE;
  }
  if ((minor > 0) && partitions[minor - 1].start == 0)
    return -ENXIO;              /* The requested partition does not exist. */
  lockmask |= (1 << minor);     /* Marks it locked. */
  return SYSOK;
}

sysret_t mmc_close(unsigned minor) {

  if (minor > 4)
    return -ENXIO;

  if (initialized)
    sync();

  lockmask &= ~(1 << minor);
  return SYSOK;
}

void mmc_strategy(struct buf *bp) {
  bool_t error;
  uint32_t partition_start;

  if (!initialized)
    bp->b_flags |= B_ERROR;
  else {
    unsigned minor = MINOR(bp->b_dev);
    if (minor == 0)
      partition_start = 0;
    else
      partition_start = partitions[minor - 1].start;
    if (bp->b_flags & B_READ)
      error = read_sectors(bp->b_dptr->u8,
                           partition_start + bp->b_block * SECTORS_PER_BUFFER,
                           SECTORS_PER_BUFFER);
    else
      error = write_sectors(bp->b_dptr->u8,
                            partition_start + bp->b_block * SECTORS_PER_BUFFER,
                            SECTORS_PER_BUFFER);
    bp->b_flags |= error ? B_ERROR : B_DONE;
  }
  if (bp->b_flags & B_ASYNC)
    brelse(bp);
}
