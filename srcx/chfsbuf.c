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

#define HASH(d, r) (((d) + (r)) & CH_BUF_HASH_MASK)

static struct buf lru;

/*
 * NOTA: Il campo next punta verso la coda, invece il campo prev punta
 * verso la testa. Nella lru i prelievi si fanno in coda.
 */

extern struct bdev bdevsw[];

static struct hash_header hash[CH_BUF_HASH_SIZE];
static struct buf bufs[CH_BUFFERS];
static union data_block buffers[CH_BUFFERS];

static Semaphore bufwait;               /* Buffers counter.             */
static Semaphore bufme;                 /* Buffer cache mutex.          */

/*----------------------------------------------------------------------
 * Inserisce un buffer in testa alla lru.
 *----------------------------------------------------------------------
 */
#define LINSHEAD(b) {                                                   \
  (b)->b_lnext = lru.b_lnext;                                           \
  (b)->b_lprev = &lru;                                                  \
  lru.b_lnext->b_lprev = (b);                                           \
  lru.b_lnext = (b);                                                    \
}

/*----------------------------------------------------------------------
 * Inserisce un buffer in coda alla LRU.
 *----------------------------------------------------------------------
 */
#define LINSTAIL(b) {                                                   \
  (b)->b_lprev = lru.b_lprev;                                           \
  (b)->b_lnext = &lru;                                                  \
  lru.b_lprev->b_lnext = (b);                                           \
  lru.b_lprev = (b);                                                    \
}

/*----------------------------------------------------------------------
 * Rimuove un buffer dalla coda in cui si trova.
 *----------------------------------------------------------------------
 */
#define LREMOVE(b) {                                                    \
  (b)->b_lprev->b_lnext = (b)->b_lnext;                                 \
  (b)->b_lnext->b_lprev = (b)->b_lprev;                                 \
}

/*----------------------------------------------------------------------
 * Inserisce un buffer nella tavola hash.
 *----------------------------------------------------------------------
 */
#define INSHASH(hp, bp, dev, rec) {                                     \
  (hp) = (struct buf *)(hash + HASH(dev, rec));                         \
  (bp)->b_hnext = (hp)->b_hnext;                                        \
  (bp)->b_hprev = (hp);                                                 \
  (hp)->b_hnext->b_hprev = (bp);                                        \
  (hp)->b_hnext = (bp);                                                 \
}

/*
 * Buffer cache initialization.
 */
void buf_init() {
  int i;
  struct hash_header *hhp;
  struct buf *bp, *hp;
  const struct bdev *bdp;

  chSemInit(&bufme, 1);
  chSemInit(&bufwait, CH_BUFFERS);
  lru.b_lnext = lru.b_lprev = &lru;             /* LRU initialization.  */
  lru.b_flags = B_NOTBUF;

  /*
   * Hash table initialization.
   */
  for (hhp = hash; hhp < &hash[CH_BUF_HASH_SIZE]; hhp++)
    hhp->b_hprev = hhp->b_hnext = hhp;

  /*
   * Buffer headers initialization.
   */
  for (i = 0, bp = bufs ; i < CH_BUFFERS ; i++, bp++) {
    bp->b_dev = NODEV;
    bp->b_lock = NULL;
    bp->b_block = (lba_t)i;
    bp->b_flags = B_INLRU | B_INVAL;
    bp->b_dptr = &buffers[i];
    chSemInit(&bp->b_sem, 1);
    INSHASH(hp, bp, NODEV, i);
    LINSHEAD(bp);
  }

  /*
   * Device drivers initialization.
   */
  for (bdp = bdevtbl; bdp < &bdevtbl[CH_BDEV_TABLE_SIZE]; bdp++)
    bdp->init();
}

/*
 * Searches a buffer into the cache and returns it.
 */
static struct buf *hfind(dev_t dev, lba_t rec) {
  struct buf *hp, *bp;

  hp = (struct buf *)(hash + HASH(dev, rec));
  bp = hp->b_hnext;
  while (bp != hp) {
    if (bp->b_block == rec && bp->b_dev == dev && !(bp->b_flags & B_INVAL))
      return bp;                                /* Cache hit.           */
    bp = bp->b_hnext;
  }
  return NULL;
}

/**
 * Returns a buffer representing a record on a block device.
 * If \p NODEV is specified as device then a generic temporary buffer is
 * returned.
 * The function is blocking if a buffer is not available or the requested
 * buffer is locked by another thread.
 */
struct buf *getblk(dev_t dev, lba_t rec) {
  struct buf *bp, *hp;

retry:
  chSemWait(&bufme);
  /*
   * If the buffer is not in cache or if a generic buffer was requested.
   */
  if (!(bp = hfind(dev, rec)) || (dev == NODEV)) {

    /* Any buffer will suffice.*/
    if (chSemGetCounter(&bufwait) > 0)
      chSemFastWaitS(&bufwait);         /* A buffer is available.       */
    else
      chSemSignalWait(&bufme, &bufwait);/* Wait for a buffer.           */

    /* Now a buffer is in the LRU for sure. */
    bp = lru.b_lprev;
    LREMOVE(bp);

    chDbgAssert(chSemGetCounter(&bp->b_sem) == 1, "chfsbuf.c, getblk()");
    chSemFastWaitS(&bp->b_sem);         /* Locks the buffer.            */
    if (bp->b_flags & B_DELAYED) {      /* Must write it, cannot use... */
      chSemSignal(&bufme);
      bp->b_flags = B_WRITE | B_ASYNC;
//      currp->p_oublock++;
      (*bdevtbl[MAJOR(bp->b_dev)].strategy)(bp);
      goto retry;
    }
    if (dev == NODEV) {
      bp->b_block = 0;
      bp->b_dev = NODEV;
      bp->b_flags = B_INVAL;
    }
    else {
      bp->b_block = rec;
      bp->b_dev = dev;
      bp->b_flags = 0;                  /* Note, B_DONE not asserted.   */
    }
    /*
     * Adjust its position in the hash table.
     */
    bp->b_hprev->b_hnext = bp->b_hnext;
    bp->b_hnext->b_hprev = bp->b_hprev;
    INSHASH(hp, bp, dev, rec);
    chSemSignal(&bufme);
  }
  else {                                /* It is already cached.        */
    if (chSemGetCounter(&bp->b_sem) > 0) {
      /* It is not locked. */
      chSemFastWaitS(&bp->b_sem);       /* It is not locked, safe.      */
      if (!(bp->b_flags & B_SHARED)) {
        chSemFastWaitS(&bufwait);       /* It is in the LRU, safe.      */
        LREMOVE(bp);
        bp->b_flags &= ~B_INLRU;
      }
      chSemSignal(&bufme);
    }
    else {
      /* Locked, waiting for the buffer to become free. */
      chSemSignalWait(&bufme, &bp->b_sem);
      if (bp->b_flags & B_INVAL) {      /* Was the buffer invalidated ? */
        brelse(bp);
        goto retry;
      }
    }
    bp->b_flags |= B_DONE;
  }
  return bp;
}

/**
 * Makes a buffer shared.
 */
void bshare(struct buf *bp) {

  chSemWait(&bufme);
  bp->b_flags |= B_SHARED;
  chSemSignal(&bp->b_sem);
  chSemSignal(&bufme);
}

/**
 * Returns a buffer to its normal unshared state.
 */
void bunshare(struct buf *bp) {

  chSemWait(&bufme);
  bp->b_flags &= ~B_SHARED;
  chSemSignalWait(&bufme, &bp->b_sem);
}

/**
 * Releases a buffer. The following flags are considered:
 * B_DONE       - If not asserted the buffer is invalidated.
 * B_MODIFIED   - The buffer was modified and must be written.
 * B_FORGET     - Low priority buffer.
 */
void brelse(struct buf *bp) {

  chSemWait(&bufme);
  /*
   * Under the following conditions the buffer should be invalidated.
   */
  if (!(bp->b_flags & B_DONE) || bp->b_flags & (B_ERROR | B_INVAL)) {
    bp->b_flags &= B_SHARED;
    bp->b_flags |= B_INVAL;
  }
  /*
   * If some thread is waiting for this specific buffer I will release it
   * directly without putting it in the LRU.
   */
  if (chSemGetCounter(&bp->b_sem) < 0) {
    chSemSignal(&bp->b_sem);
    chSemSignal(&bufme);
    return;
  }

  chSemFastSignalI(&bp->b_sem);
  /*
   * If it is a shared buffer then it does not go back in the LRU.
   */
  if (bp->b_flags & B_SHARED) {
    bp->b_flags &= B_MODIFIED;
    bp->b_flags |= B_DONE | B_SHARED;
    chSemSignal(&bufme);
    return;
  }

  /*
   * Handles modified buffers by marking them for a delayed write.
   */
  if (bp->b_flags & B_MODIFIED) {
    bp->b_flags |= B_DELAYED;
    bp->b_flags &= B_DELAYED | B_FORGET;
  }

  if (bp->b_flags & (B_INVAL | B_FORGET)) {
    LINSTAIL(bp);
  }
  else {
    LINSHEAD(bp);
  }
  bp->b_flags |= B_INLRU;

  if (chSemGetCounter(&bufwait) >= 0) {
    chSemFastSignalI(&bufwait);         /* Nobody waiting, safe.        */
    chSemSignal(&bufme);
  }
  else
    chSemSignal(&bufwait);              /* Resumed into getblk().       */
}

/**
 * Synchronous write. Setup the B_FORGET flag if needed.
 */
void bwrite(struct buf *bp) {

  bp->b_flags &= B_FORGET | B_SHARED;
//  currp->p_oublock++;
  (*bdevtbl[MAJOR(bp->b_dev)].strategy)(bp);
}

/**
 * Asynchronous write.
 */
void bawrite(struct buf *bp) {

  bp->b_flags &= B_SHARED;
  bp->b_flags |= B_ASYNC;
//  currp->p_oublock++;
  (*bdevtbl[MAJOR(bp->b_dev)].strategy)(bp);
}

/**
 * Block read, it is marked with \p B_ERROR if an error occurs.
 */
struct buf *bread(dev_t dev, lba_t rec) {
  struct buf *bp;

  if (!((bp = getblk(dev, rec))->b_flags & B_DONE)) {
    bp->b_flags &= B_SHARED;
    bp->b_flags |= B_READ;
//    currp->p_inblock++;
    (*bdevtbl[MAJOR(dev)].strategy)(bp);
  }
  return bp;
}

/**
 * Block read with read-ahead specification for a 2nd block.
 */
struct buf *breada(dev_t dev, lba_t rec1, lba_t rec2) {
  struct buf *bp;

  chSemWait(&bufme);
  bp = hfind(dev, rec1);
  chSemSignal(&bufme);
  if (!bp) {
    if ((bp = getblk(dev, rec1))->b_flags & B_DONE)
      brelse(bp);
    else {
      /* Not cached, start an asynchronous read. */
      bp->b_flags &= B_SHARED;
      bp->b_flags |= B_READ | B_ASYNC;
//      currp->p_inblock++;
      (*bdevtbl[MAJOR(dev)].strategy)(bp);
    }
  }
  if ((bp = getblk(dev, rec2))->b_flags & B_DONE)
    brelse(bp);
  else {
    /* Not cached, start an asynchronous read. */
    bp->b_flags &= B_SHARED;
    bp->b_flags |= B_READ | B_ASYNC;
//    currp->p_inblock++;
    (*bdevtbl[MAJOR(dev)].strategy)(bp);
  }
  return bread(dev, rec1);
}

/**
 * Ensures all blocks belongig to a block device are safely written.
 */
void bflush(dev_t dev, bool_t forget) {
  struct buf *bp;

loop:
  chSemWait(&bufme);
  bp = lru.b_lnext;
  while (bp != &lru) {
    chDbgAssert((bp->b_flags & B_INLRU) && !(bp->b_flags & B_SHARED),
                "chfsbuf.c, bflush()");
    if ((bp->b_flags & B_DELAYED) && !(bp->b_flags & B_INVAL) &&
        (dev == NODEV || dev == bp->b_dev)) {
      chSemFastWaitS(&bufwait);
      chSemFastWaitS(&bp->b_sem);
      LREMOVE(bp);
      chSemSignal(&bufme);
      bp->b_flags = B_ASYNC | (forget ? B_FORGET : 0);
//      currp->p_oublock++;
      (*bdevtbl[MAJOR(bp->b_dev)].strategy)(bp);
      goto loop;
    }
    bp = bp->b_lnext;
  }
  chSemSignal(&bufme);
}

/**
 * Invalidates all the buffers associated to a Block Device.
 */
void binval(dev_t dev) {
  struct buf *bp;

  for (bp = bufs; bp < &bufs[CH_BUFFERS]; bp++) {
    if (bp->b_dev == dev && !(bp->b_flags & B_INVAL)) {
      chSemWait(&bufme);
      chDbgAssert(!(bp->b_flags & B_DELAYED), "chfsbuf.c, binval()");
      if (bp->b_flags & B_INLRU)
        bp->b_flags = B_INLRU | B_INVAL;
      else {
        chSemSignalWait(&bufme, &bp->b_sem);
        bp->b_flags &= B_SHARED;
        bp->b_flags |= B_INVAL;
        brelse(bp);
        continue;
      }
      chSemSignal(&bufme);
    }
  }
}
