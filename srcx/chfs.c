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

/**
 * File System subsystem initialization.
 * @note Must be invoked after the kernel initialization.
 */
void chFSInit(void) {

  buf_init();
  vnode_init();
}

sysret_t chFSMount(FileSystem *fsp, char *mnt, dev_t dev, bool_t ro, unsigned type) {
  sysret_t ret;
  struct vnode *vp;

  if (mnt)
    return EINVAL;      /* Mount points not implemented yet. */

  if ((ret = (*bdevtbl[MAJOR(dev)].open)(MINOR(dev), ro)) < SYSOK)
    return ret;

  chSemInit(&fsp->fs_sem, 1);
  fsp->fs_ro = ro;
  fsp->fs_dev = dev;
  fsp->fs_type = type;
  switch (type) {
  case FS_TYPE_FAT:
    ret = fat_mount(fsp);
    break;
  default:
    ret = -EINVAL;
  }
  if (ret < SYSOK)
    goto exit0;

  /*
   * Gets a reference to the root vnode and reads it.
   */
  vp = vnget(fsp, ROOT_VNODE);
  if (!vp) {
    ret = -ENFILE;
    goto exit0;
  }
  if (!vp->v_flags & VN_DONE) {
    ret = fsp->fs_vmt->read_root_node(vp);
    if (ret < SYSOK) {
      VNUNLOCK(vp);
      goto exit1;
    }
  }
  vp->v_flags = 0;
  VNUNLOCK(vp);
  fsp->fs_root = vp;
  return SYSOK;
exit1:
  vnrelse(vp);
exit0:
  (*bdevtbl[MAJOR(dev)].close)(MINOR(dev));
  return ret;
}

sysret_t chFSUnmount(FileSystem *fsp) {
  sysret_t ret;

  vnrelse(fsp->fs_root);
  ret = (*bdevtbl[MAJOR(fsp->fs_dev)].close)(MINOR(fsp->fs_dev));
  return ret;
}
