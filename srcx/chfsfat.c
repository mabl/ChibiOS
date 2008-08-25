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

#include <string.h>

static sysret_t read_root_node(struct vnode *vp);
static sysret_t update_node(struct vnode *vp);
static sysret_t follow(char *name, unsigned len, struct vnode **vpp);

/*
 * FAT File System VMT.
 */
struct fsvmt fatvmt = {
  read_root_node,
  update_node,
  follow
};

#define BPB_SECTOR_SIZE         0xB
#define BPB_SECTORS_PER_CLUSTER 0xD
#define BPB_RESERVED_SECTORS    0xE
#define BPB_NUM_FATS            0x10
#define BPB_ROOT_ENTRIES        0x11
#define BPB_TOTAL_SECTORS_OLD   0x13
#define BPB_MEDIA_DESCRIPTOR    0x15
#define BPB_SECTORS_PER_FAT     0x16
#define BPB_TOTAL_SECTORS       0x20
#define BPB_FILE_SYSTEM_TYPE    0x36
#define BPB_SIGNATURE           0x1FE

#define get8(bp, offset) ((bp)->b_dptr->u8[offset])

static uint16_t get16(struct buf *bp, unsigned offset) {
  uint8_t *p = &bp->b_dptr->u8[offset];

  return (p[0]) | (p[1] << 8);
}

static uint32_t get32(struct buf *bp, unsigned offset) {
  uint8_t *p = &bp->b_dptr->u8[offset];

  return (p[0]) | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

sysret_t fat_mount(FileSystem *fsp) {
  struct buf *bp;
  sysret_t ret;
  uint32_t n;

  /*
   * Read boot sector.
   */
  bp = bread(fsp->fs_dev, 0);
  if (bp->b_flags & B_ERROR) {
    ret =  -EIO;
    goto exit0;
  }

  /*
   * File system checks.
   */
  ret =  -EINVAL;
  if ((get16(bp, BPB_SIGNATURE) != 0xAA55) ||
      memcmp(&bp->b_dptr->u8[BPB_FILE_SYSTEM_TYPE], "FAT", 3))
    goto exit0;

  /*
   * Reads and checks various file system parameters.
   */
  n = get16(bp, BPB_TOTAL_SECTORS_OLD);
  if (n == 0)
    n = get32(bp, BPB_TOTAL_SECTORS);
  if (n == 0)
    goto exit0;

  if (get16(bp, BPB_SECTOR_SIZE) != 512)
    goto exit0;

  fsp->fs_fat.fatnum = get8(bp, BPB_NUM_FATS);
  if ((fsp->fs_fat.fatnum < 0) || (fsp->fs_fat.fatnum > 2))
    goto exit0;

  fsp->fs_fat.rootsize = get16(bp, BPB_ROOT_ENTRIES) / 32;
  if (fsp->fs_fat.rootsize == 0)
//    fsp->fs_fat.type = FAT32_TYPE;
    goto exit0; /* FAT32 not supported yet. */
  else if (n < 0xFF7)
//    fsp->fs_fat.type = FAT12_TYPE;
    goto exit0; /* FAT12 not supported yet. */
  else
    fsp->fs_fat.type = FAT16_TYPE;

  fsp->fs_fat.fatsize = get16(bp, BPB_SECTORS_PER_FAT);
  fsp->fs_fat.fatbase = get16(bp, BPB_RESERVED_SECTORS);
  fsp->fs_fat.csize = get8(bp, BPB_SECTORS_PER_CLUSTER);
  fsp->fs_fat.clbase = fsp->fs_fat.fatsize * fsp->fs_fat.fatnum +
                       fsp->fs_fat.fatbase + fsp->fs_fat.rootsize;
  fsp->fs_fat.clnum = (n - fsp->fs_fat.clbase) / fsp->fs_fat.csize;

  switch (fsp->fs_fat.type) {
  case FAT12_TYPE:
  case FAT16_TYPE:
    fsp->fs_fat.fat1x.rootbase = fsp->fs_fat.clbase - fsp->fs_fat.rootsize;
    break;
  case FAT32_TYPE:
    ;
  }

  ret = SYSOK;
exit0:
  brelse(bp);
  return ret;
}

static sysret_t read_root_node(struct vnode *vp) {

  vp->v_flags = VN_DONE;
  return SYSOK;
}

static sysret_t update_node(struct vnode *vp) {

  return SYSOK;
}

static sysret_t follow(char *name, unsigned len, struct vnode **vpp) {
  char buf[12];
  unsigned i;

  memset(buf, ' ', 11);
  buf[11] = 0;

  i = 0;
  while (TRUE) {
  }

  return SYSOK;
}
