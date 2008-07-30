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

/*
 * @param path pointer to a relative path (no trailing "/").
 * @param vpp on entry it is the vnode representing the root, on exit it
 *            points to the found vnode
 * @return \p SYSOK if the operation was successful else an error code.
 */
static sysret_t scan_path(char *path, struct vnode **vpp) {
  sysret_t ret;
  char *p;

  while (TRUE) {
    /* Trailing and consecutive '/' are skipped and ignored.*/
    while (*path && (*path == '/'))
      path++;

    p = path;
    /* Sizes a single path element.*/
    while (*p && (*p != '/'))
      p++;
    if (p - path == 0)
      return SYSOK;

    /* If not at the path end the vnode must represent a directory.*/
    if ((*p == '/') && !S_ISDIR(*vpp->v_flags))
      return -ENOTDIR;

    /* Note it is the virtual follow function that checks the syntax and the
       filename length of the path element, it is FS-dependent.*/
    ret = (*vpp)->v_fs->fs_vmt->follow(path, p - path, vpp);
    if (ret < SYSOK)
      return ret;

    path = p;
  }
}
