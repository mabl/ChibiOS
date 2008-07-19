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

#ifndef _VNODE_H_
#define _VNODE_H_

struct vnode {
  struct vnode          *v_hnext;       /* Next in the hash slot.       */
  struct vnode          *v_hprev;       /* Previous in the hash slot.   */
  struct vnode          *v_lnext;       /* Next in the LRU queue.       */
  struct vnode          *v_lprev;       /* Previous in the LRU queue.   */
  ino_t                 v_node;         /* Physical node reference.     */
  uint16_t              v_nrefs;        /* References counter.          */
  struct fs             *v_fs;          /* Associated file system.      */
  Semaphore             v_datasem;      /* Mutex on the physical node.  */
  /*
   * The following fields must be accessed under lock using the
   * v_datasem semaphore. The fields before this comment are accessed
   * under the global cache lock.
   */
  uint16_t              v_flags;        /* Node attributes.             */
  uint16_t              v_links;        /* Number of links to the node. */
  umode_t               v_mode;         /* File attributes, see stat.h. */
  off_t                 v_size;         /* File size.                   */
  /*
   * FS driver specific node data fields.
   */
  union {
    struct fatnode      fat;            /* FAT file system node data.   */
  };
};

#define VN_DONE         1               /* Node data populated.         */
#define VN_MODIFIED     2               /* Modified node data.          */

#define ROOT_VNODE      0

#define VNLOCK(vn)      chSemWait(&(vn)->v_datasem)
#define VNUNLOCK(vn)    chSemSignal(&(vn)->v_datasem)

void vnode_init(void);
struct vnode *vnget(struct fs *fsp, ino_t vnode);
void vnrelse(struct vnode *vp);
struct vnode *vndup(struct vnode *vp);

#endif /* _VNODE_H_ */
