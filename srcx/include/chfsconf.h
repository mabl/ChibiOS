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

#ifndef _CHFSCONF_H_
#define _CHFSCONF_H_

/**
 * Number of block device drivers in the system.
 */
#define CH_BDEV_TABLE_SIZE      1

/**
 * Number of disk buffers, must be a power of two.
 */
#define CH_BUFFERS              8

/**
 * Number of vnodes, must be a power of two.
 */
#define CH_VNODES               16

/*
 * Derived values or unmodifiable parameters.
 */
#define CH_BUF_HASH_SIZE        (CH_BUFFERS * 2)
#define CH_BUF_HASH_MASK        (CH_BUF_HASH_SIZE - 1)
#define CH_BUFFER_SIZE          512

#define CH_VNODE_HASH_SIZE      (CH_VNODES * 2)
#define CH_VNODE_HASH_MASK      (CH_VNODE_HASH_SIZE - 1)

#endif /* _CHFSCONF_H_ */
