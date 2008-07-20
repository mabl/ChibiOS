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

#ifndef _CHFS_H_
#define _CHFS_H_

#include <chfsconf.h>
#include <errno.h>
#include <types.h>
#include <dev.h>
#include <buf.h>
#include <stat.h>
#include <fatfs.h>
#include <vnode.h>

typedef struct fs {
  struct fsvmt          *fs_vmt;
  Semaphore             fs_sem;
//  struct vnode          *fs_where;
  struct vnode          *fs_root;
  bool_t                fs_ro;
  dev_t                 fs_dev;
  unsigned              fs_type;
  union {
    struct fatfs        fs_fat;
  };
} FileSystem;

struct fsvmt {
  sysret_t (*read_root_node)(struct vnode *vp);
  sysret_t (*update_node)(struct vnode *vp);
  sysret_t (*follow)(char *name, unsigned len, struct vnode **vpp);
};

#define FS_TYPE_FAT     0

#define FSLOCK(f)   chSemWait(&(f)->fs_sem)
#define FSUNLOCK(f) chSemSignal(&(f)->fs_sem)

#ifdef __cplusplus
extern "C" {
#endif
  void chFSInit(void);
  sysret_t chFSMount(FileSystem *fsp, char *mnt, dev_t dev, bool_t ro, unsigned type);
  sysret_t chFSUnmount(FileSystem *fsp);
#ifdef __cplusplus
}
#endif

#endif /* _CHFS_H_ */
