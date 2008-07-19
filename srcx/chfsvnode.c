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

struct hhdr {
  struct vnode          *v_hnext;       /* Next in the hash slot.       */
  struct vnode          *v_hprev;       /* Previous in the hash slot.   */
};

struct lhdr {
  struct vnode          *v_unused1;
  struct vnode          *v_unused2;
  struct vnode          *v_lnext;       /* Next in the LRU queue.       */
  struct vnode          *v_lprev;       /* Previous in the LRU queue.   */
};

static struct hhdr hash[CH_VNODE_HASH_SIZE];
static struct vnode vnodes[CH_VNODES];
static struct lhdr lru;
static Semaphore sem;

#define VHASH(d, r) (((d) + (r)) & CH_VNODE_HASH_MASK)

#define VLINSHEAD(vp) {                                                 \
  (vp)->v_lnext = lru.v_lnext;                                          \
  (vp)->v_lprev = (void *)&lru;                                         \
  lru.v_lnext->v_lprev = (vp);                                          \
  lru.v_lnext = (vp);                                                   \
}

#define VLINSTAIL(vp) {                                                 \
  (vp)->v_lprev = lru.v_lprev;                                          \
  (vp)->v_lnext = (void *)&lru;                                         \
  lru.v_lprev->v_lnext = (vp);                                          \
  lru.v_lprev = (vp);                                                   \
}

#define VLREMOVE(vp) {                                                  \
  (vp)->v_lprev->v_lnext = (vp)->v_lnext;                               \
  (vp)->v_lnext->v_lprev = (vp)->v_lprev;                               \
}

#define VHREMOVE(vp) {                                                  \
  (vp)->v_hprev->v_hnext = (vp)->v_hnext;                               \
  (vp)->v_hnext->v_hprev = (vp)->v_hprev;                               \
}

static struct vnode *vnfind(dev_t dev, ino_t vnode) {
  struct hhdr *hhp;
  struct vnode *vp;

  hhp = &hash[VHASH(dev, vnode)];
  vp = (void *)hhp->v_hnext;
  while (vp != (void *)hhp) {
    if ((vp->v_node == vnode) &&
        (vp->v_fs != NULL) &&
        (vp->v_fs->fs_dev == dev))
      return vp;
    vp = vp->v_hnext;
  }
  return NULL;
}

static void inshash(struct vnode *vp) {
  struct hhdr *hhp;
  dev_t dev;

  if (vp->v_fs)
    dev = vp->v_fs->fs_dev;
  else
    dev = 0;

  hhp = &hash[VHASH(dev, vp->v_node)];
  vp->v_hnext = (void *)hhp->v_hnext;
  vp->v_hprev = (void *)hhp;
  hhp->v_hnext->v_hprev = (void *)vp;
  hhp->v_hnext = (void *)vp;
}

/*
 * Vnode cache initialization.
 */
void vnode_init(void) {
  unsigned i;
  struct hhdr *hhp;
  struct vnode *vp;

  chSemInit(&sem, 1);
  lru.v_lnext = lru.v_lprev = (void *)&lru;     /* LRU initialization.  */

  for (hhp = hash; hhp < &hash[CH_VNODE_HASH_SIZE]; hhp++)
    hhp->v_hprev = hhp->v_hnext = (void *)hhp;

  for (i = 0, vp = vnodes ; i < CH_VNODES; i++, vp++) {
    vp->v_node    = (ino_t)i;
    vp->v_nrefs   = 0;
    vp->v_flags   = 0;
    vp->v_fs      = NULL;
    chSemInit(&vp->v_datasem, 1);
    inshash(vp);
    VLINSHEAD(vp);
  }
}

/*
 * Returns a locked vnode from the vnodes cache.
 * The vnode is marked as VN_DONE if the content is real (from a cache
 * hit), else the real node data must be populated into the vnode.
 */
struct vnode *vnget(struct fs *fsp, ino_t vnode) {
  struct vnode *vp;

  chSemWait(&sem);
  if ((vp = vnfind(fsp->fs_dev, vnode)) != NULL) {
    /* Cache hit. */
    if (!vp->v_nrefs)
        VLREMOVE(vp);
    vp->v_nrefs++;
    chSemSignalWait(&sem, &vp->v_datasem);
    return vp;
  }
  if ((vp = lru.v_lprev) == (void *)&lru) {
    chSemSignal(&sem);
    return NULL;                /* Meaning ENFILE */
  }
  VLREMOVE(vp);
  vp->v_node  = vnode;
  vp->v_nrefs = 1;
  vp->v_fs    = fsp;
  VHREMOVE(vp);                 /* New position in the hash table. */
  inshash(vp);
  chSemSignalWait(&sem, &vp->v_datasem);
  vp->v_flags = 0;              /* Marked as: to be read, not modified. */
  return vp;
}

/*
 * Releases a reference to a vnode eventually writing the modified content to
 * the disk.
 */
void vnrelse(struct vnode *vp) {

  chSemWait(&sem);
  chDbgAssert(vp->v_nrefs > 0, "chfsvnode.c, vnrelse()");
  if (vp->v_nrefs == 1) {
    if (!(vp->v_flags & VN_DONE)) {     /* Unread nodes are inserted    */
      --vp->v_nrefs;                    /* on the tail end of the LRU   */
      VLINSTAIL(vp);                    /* for immediate reuse.         */
      chSemSignal(&sem);
      return;
    }
    chSemSignalWait(&sem, &vp->v_datasem);
    if (vp->v_flags & VN_MODIFIED)
      vp->v_fs->fs_vmt->update_node(vp);
    vp->v_flags &= ~VN_MODIFIED;
    chSemSignalWait(&vp->v_datasem, &sem);
  }
  if (!--vp->v_nrefs)
    VLINSHEAD(vp);
  chSemSignal(&sem);
}

/*
 * Duplicates a reference to a vnode.
 */
struct vnode *vndup(struct vnode *vp) {

  chSemWait(&sem);
  vp->v_nrefs++;
  chSemSignal(&sem);
  return vp;
}
