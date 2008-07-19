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

#ifndef _BUF_H_
#define _BUF_H_

typedef uint16_t bflags_t;

#define B_WRITE         0x0000  /* Requestes a write operation.         */
#define B_READ          0x0001  /* Requestes a read operation.          */
#define B_INVAL         0x0002  /* Invalid buffer.                      */
#define B_DELAYED       0x0004  /* Write operation not urgent.          */
#define B_MODIFIED      0x0008  /* Write operation required.            */
#define B_FORGET        0x0010  /* Low priority data, forget it.        */
#define B_ASYNC         0x0020  /* Asynchronous operation.              */
#define B_NOTBUF        0x0040  /* Not a buffer (list header).          */
#define B_ERROR         0x0080  /* I/O error (from device driver).      */
#define B_DONE          0x0100  /* Cache hit.                           */
#define B_INLRU         0x0200  /* Buffer in LRU list.                  */
#define B_SHARED        0x0400  /* Shared system buffer.                */

/**
 * Hash slot header.
 */
struct hash_header {
  struct hash_header *b_hnext;  /* Next synonim in the hash slot.       */
  struct hash_header *b_hprev;  /* Previous synonim in the hash slot.   */
};

union data_block;

/**
 * Buffer descriptor.
 */
struct buf {
  struct buf    *b_hnext;       /* Next synonim in the hash slot.       */
  struct buf    *b_hprev;       /* Previous synonim in the hash slot.   */
  struct buf    *b_lnext;       /* Next buffer in the LRU list.         */
  struct buf    *b_lprev;       /* Previous buffer in the LRU list.     */
  union data_block *b_dptr;     /* Buffer pointer.                      */
  bflags_t      b_flags;        /* Buffer flags.                        */
  dev_t         b_dev;          /* Block Device.                        */
  lba_t         b_block;        /* Block number.                        */
  Semaphore     b_sem;          /* Buffer mutex.                        */
  Thread        *b_lock;        /* Used by device drivers.              */
  lba_t         b_physblk;      /* Used by device drivers.              */
};

void buf_init(void);
struct buf *bread(dev_t dev, lba_t rec);
struct buf *breada(dev_t dev, lba_t rec1, lba_t rec2);
struct buf *getblk(dev_t dev, lba_t rec);
void brelse(struct buf *bp);
void bwrite(struct buf *bp);
void bawrite(struct buf *bp);
void bshare(struct buf *bp);
void bunshare(struct buf *bp);
void bflush(dev_t dev, bool_t forget);
void binval(dev_t dev);

#endif /* _BUF_H_ */
