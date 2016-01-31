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
 * @file    chheap.c
 * @brief   Heaps code.
 *
 * @addtogroup heaps
 * @details Heap Allocator related APIs.
 *          <h2>Operation mode</h2>
 *          The heap allocator implements a first-fit strategy and its APIs
 *          are functionally equivalent to the usual @p malloc() and @p free()
 *          library functions. The main difference is that the OS heap APIs
 *          are guaranteed to be thread safe and there is the ability to
 *          return memory blocks aligned to arbitrary powers of two.<br>
 * @pre     In order to use the heap APIs the @p CH_CFG_USE_HEAP option must
 *          be enabled in @p chconf.h.
 * @{
 */

#include "ch.h"

#if (CH_CFG_USE_HEAP == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*
 * Defaults on the best synchronization mechanism available.
 */
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
#define H_LOCK(h)       chMtxLock(&(h)->mtx)
#define H_UNLOCK(h)     chMtxUnlock(&(h)->mtx)
#else
#define H_LOCK(h)       (void) chSemWait(&(h)->sem)
#define H_UNLOCK(h)     chSemSignal(&(h)->sem)
#endif

#define H_BLOCK(hp)     ((hp) + 1U)

#define H_NEXT(hp)      ((hp)->h.u.next)

#define H_HEAP(hp)      ((hp)->h.u.heap)

#define H_SIZE(hp)      ((hp)->h.size)

#define H_LIMIT(hp)     (H_BLOCK(hp) + H_SIZE(hp))

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Default heap descriptor.
 */
static memory_heap_t default_heap;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the default heap.
 *
 * @notapi
 */
void _heap_init(void) {

  default_heap.provider = chCoreAllocAligned;
  H_NEXT(&default_heap.free) = NULL;
  H_SIZE(&default_heap.free) = 0;
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  chMtxObjectInit(&default_heap.mtx);
#else
  chSemObjectInit(&default_heap.sem, (cnt_t)1);
#endif
}

/**
 * @brief   Initializes a memory heap from a static memory area.
 * @pre     Both the heap buffer base and the heap size must be aligned to
 *          the @p heap_header_t type size.
 *
 * @param[out] heapp    pointer to the memory heap descriptor to be initialized
 * @param[in] buf       heap buffer base
 * @param[in] size      heap size
 *
 * @init
 */
void chHeapObjectInit(memory_heap_t *heapp, void *buf, size_t size) {
  heap_header_t *hp = buf;

  chDbgCheck((heapp != NULL) && (size > 0U) &&
             MEM_IS_ALIGNED(buf, CH_HEAP_ALIGNMENT) &&
             MEM_IS_ALIGNED(size, CH_HEAP_ALIGNMENT));

  heapp->provider = NULL;
  H_NEXT(&heapp->free) = hp;
  H_SIZE(&heapp->free) = 0;
  H_NEXT(hp) = NULL;
  H_SIZE(hp) = (size - sizeof (heap_header_t)) / CH_HEAP_ALIGNMENT;
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  chMtxObjectInit(&heapp->mtx);
#else
  chSemObjectInit(&heapp->sem, (cnt_t)1);
#endif
}

/**
 * @brief   Allocates a block of memory from the heap by using the first-fit
 *          algorithm.
 * @details The allocated block is guaranteed to be properly aligned to the
 *          specified alignment.
 *
 * @param[in] heapp     pointer to a heap descriptor or @p NULL in order to
 *                      access the default heap.
 * @param[in] size      the size of the block to be allocated. Note that the
 *                      allocated block may be a bit bigger than the requested
 *                      size for alignment and fragmentation reasons.
 * @param[in] align     desired memory alignment
 * @return              A pointer to the aligned allocated block.
 * @retval NULL         if the block cannot be allocated.
 *
 * @api
 */
void *chHeapAllocAligned(memory_heap_t *heapp, size_t size, unsigned align) {
  heap_header_t *qp, *hp;

  chDbgCheck((size > 0U) && MEM_IS_VALID_ALIGNMENT(align));

  /* If an heap is not specified then the default system header is used.*/
  if (heapp == NULL) {
    heapp = &default_heap;
  }

  /* Minimum alignment is constrained by the heap header structure size.*/
  if (align < CH_HEAP_ALIGNMENT) {
    align = CH_HEAP_ALIGNMENT;
  }

  /* Size is converted in number of elementary allocation units.*/
  size = MEM_ALIGN_NEXT(size, CH_HEAP_ALIGNMENT) / CH_HEAP_ALIGNMENT;

  /* Taking heap mutex/semaphore.*/
  H_LOCK(heapp);

  /* Start of the free blocks list.*/
  qp = &heapp->free;
  while (qp->h.u.next != NULL) {
    heap_header_t *ahp;

    /* Next free block.*/
    hp = qp->h.u.next;

    /* Pointer aligned to the requested alignment.*/
    ahp = (heap_header_t *)MEM_ALIGN_NEXT(H_BLOCK(hp), align) - 1U;

    if ((ahp < H_LIMIT(hp)) && (size <= (size_t)(H_LIMIT(hp) - 1U - ahp))) {
      /* The block is large enough to contain a correctly aligned area
         of sufficient size.*/

      if (ahp > hp) {
        /* The block is not properly aligned, must split it.*/
        H_SIZE(ahp) = H_LIMIT(hp) - H_BLOCK(ahp);
        H_SIZE(hp) = ahp - H_BLOCK(hp);

        if ((size_t)(H_LIMIT(hp) - H_BLOCK(ahp)) > size) {
          /* The block is bigger than required, must split the excess.*/
          heap_header_t *fp;

          fp = H_BLOCK(ahp) + size;
          H_NEXT(fp) = H_NEXT(hp);
          H_SIZE(fp) = H_LIMIT(ahp) - H_BLOCK(fp);
          H_NEXT(hp) = fp;
          H_SIZE(ahp) = size;
        }

        hp = ahp;
      }
      else {
        /* The block is already properly aligned.*/

        if (H_SIZE(hp) == size) {
          /* Exact size, getting the whole block.*/
          H_NEXT(qp) = H_NEXT(hp);
        }
        else {
          /* The block is bigger than required, must split the excess.*/
          heap_header_t *fp;

          fp = H_BLOCK(hp) + size;
          H_NEXT(fp) = H_NEXT(hp);
          H_SIZE(fp) = H_LIMIT(hp) - H_BLOCK(fp);
          H_NEXT(qp) = fp;
          H_SIZE(hp) = size;
        }
      }

      /* Marking the block with the owner heap.*/
      H_HEAP(hp) = heapp;

      /* Releasing heap mutex/semaphore.*/
      H_UNLOCK(heapp);

      /*lint -save -e9087 [11.3] Safe cast.*/
      return (void *)H_BLOCK(hp);
      /*lint -restore*/
    }

    /* Next in the free blocks list.*/
    qp = hp;
  }

  /* Releasing heap mutex/semaphore.*/
  H_UNLOCK(heapp);

  /* More memory is required, tries to get it from the associated provider
     else fails.*/
  if (heapp->provider != NULL) {
    hp = heapp->provider(size + sizeof (heap_header_t), align);
    if (hp != NULL) {
      H_HEAP(hp) = heapp;
      H_SIZE(hp) = size;

      /*lint -save -e9087 [11.3] Safe cast.*/
      return (void *)H_BLOCK(hp);
      /*lint -restore*/
    }
  }

  return NULL;
}

/**
 * @brief   Frees a previously allocated memory block.
 *
 * @param[in] p         pointer to the memory block to be freed
 *
 * @api
 */
void chHeapFree(void *p) {
  heap_header_t *qp, *hp;
  memory_heap_t *heapp;

  chDbgCheck((p != NULL) && MEM_IS_ALIGNED(p, CH_HEAP_ALIGNMENT));

  /*lint -save -e9087 [11.3] Safe cast.*/
  hp = (heap_header_t *)p - 1U;
  /*lint -restore*/
  heapp = hp->h.u.heap;
  qp = &heapp->free;

  /* Taking heap mutex/semaphore.*/
  H_LOCK(heapp);

  while (true) {
    chDbgAssert((hp < qp) || (hp >= H_LIMIT(qp)), "within free block");

    if (((qp == &heapp->free) || (hp > qp)) &&
        ((H_NEXT(qp) == NULL) || (hp < H_NEXT(qp)))) {
      /* Insertion after qp.*/
      H_NEXT(hp) = H_NEXT(qp);
      H_NEXT(qp) = hp;
      /* Verifies if the newly inserted block should be merged.*/
      if (H_LIMIT(hp) == H_NEXT(hp)) {
        /* Merge with the next block.*/
        H_SIZE(hp) += H_SIZE(H_NEXT(hp)) + 1U;
        H_NEXT(hp) = H_NEXT(H_NEXT(hp));
      }
      if ((H_LIMIT(qp) == hp)) {
        /* Merge with the previous block.*/
        H_SIZE(qp) += H_SIZE(hp) + 1U;
        H_NEXT(qp) = H_NEXT(hp);
      }
      break;
    }
    qp = H_NEXT(qp);
  }

  /* Releasing heap mutex/semaphore.*/
  H_UNLOCK(heapp);

  return;
}

/**
 * @brief   Reports the heap status.
 * @note    This function is meant to be used in the test suite, it should
 *          not be really useful for the application code.
 *
 * @param[in] heapp     pointer to a heap descriptor or @p NULL in order to
 *                      access the default heap.
 * @param[in] sizep     pointer to a variable that will receive the total
 *                      fragmented free space or @ NULL
 * @return              The number of fragments in the heap.
 *
 * @api
 */
size_t chHeapStatus(memory_heap_t *heapp, size_t *sizep) {
  heap_header_t *qp;
  size_t n, sz;

  if (heapp == NULL) {
    heapp = &default_heap;
  }

  H_LOCK(heapp);
  sz = 0U;
  n = 0U;
  qp = &heapp->free;
  while (H_NEXT(qp) != NULL) {
    sz += H_SIZE(H_NEXT(qp));
    n++;
    qp = H_NEXT(qp);
  }
  if (sizep != NULL) {
    *sizep = sz * CH_HEAP_ALIGNMENT;
  }
  H_UNLOCK(heapp);

  return n;
}

#endif /* CH_CFG_USE_HEAP == TRUE */

/** @} */
