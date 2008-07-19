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

#ifndef _MBR_H_
#define _MBR_H_

PACK_STRUCT_BEGIN
struct mbr_part {
  uint8_t       bootable;
  uint8_t       chs_first[3];
  uint8_t       partition_type;
  uint8_t       chs_last[3];
  uint8_t       lba_first[4];
  uint8_t       lba_length[4];
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

PACK_STRUCT_BEGIN
struct mbr {
  uint8_t       code[440];
  uint8_t       disk_signature[4];
  uint8_t       unused[2];
  struct mbr_part partitions[4];
  uint8_t       mbr_signature[2];
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#define get_lba(p) (*(p) | (*((p) + 1) << 8) |                          \
                    (*((p) + 1) << 16) | (*((p) + 1) << 24))

#endif /* _MBR_H_ */
