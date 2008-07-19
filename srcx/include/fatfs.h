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

#ifndef _FATFS_H_
#define _FATFS_H_

struct fs;

struct fatfs {
  unsigned      fatnum;                 /* Number of FATs.              */
  unsigned      fatsize;                /* FAT size in blocks.          */
  lba_t         fatbase;                /* FAT1 starting block.         */
  unsigned      csize;                  /* Cluster size in blocks.      */
  unsigned      rootsize;               /* Root dir. size in blocks.    */
  lba_t         clbase;                 /* Start of data clusters.      */
  uint16_t      clnum;                  /* Total clusters number.       */
  unsigned      type;                   /* FAT type.                    */
  union {
    struct {
      lba_t     rootbase;               /* Root dir. starting block.    */
    } fat1x;
    struct {
    } fat32;
  };
};

#define FAT12_TYPE      0
#define FAT16_TYPE      1
#define FAT32_TYPE      2

struct fatnode {
};

#ifdef __cplusplus
extern "C" {
#endif
  sysret_t fat_mount(struct fs *fsp);
#ifdef __cplusplus
}
#endif

#endif /* _FATFS_H_ */
