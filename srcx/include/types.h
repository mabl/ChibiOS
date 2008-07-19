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

#ifndef _TYPES_H_
#define _TYPES_H_

/*
 * Kernel base types.
 */
typedef int             sysret_t;       /* Syscall return type.         */

/*
 * CHFS related types.
 */
typedef uint16_t        dev_t;          /* Device identifier.           */
typedef uint32_t        lba_t;          /* Logical block address.       */
typedef uint32_t        off_t;          /* File size/offset.            */
typedef uint32_t        ino_t;          /* Virtual node index.          */
typedef uint16_t        umode_t;        /* Virtual node flags.          */

#endif /* _TYPES_H_ */
