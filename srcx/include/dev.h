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

#ifndef _DEV_H_
#define _DEV_H_

#define NODEV (dev_t)-1

#define MINOR(d) ((d) & 0xFF)
#define MAJOR(d) ((d) >> 8)
#define DEVICE(major, minor) (dev_t)((minor) | ((major) << 8))

#define notbdev(d) ((u_short)MAJOR(d) > bdev_number)
#define notcdev(d) ((u_short)MAJOR(d) > cdev_number)

struct buf;

union data_block {
  uint8_t       u8[CH_BUFFER_SIZE / sizeof(uint8_t)];
  uint16_t      u16[CH_BUFFER_SIZE / sizeof(uint16_t)];
  uint32_t      u32[CH_BUFFER_SIZE / sizeof(uint32_t)];
};

/**
 * Block Device access table.
 */
struct bdev {
  void (*init)(void);
  sysret_t (*open)(unsigned minor, bool_t ro);
  sysret_t (*close)(unsigned minor);
  void (*strategy)(struct buf *bp);
};

extern const struct bdev bdevtbl[];

#endif /* _DEV_H_ */
