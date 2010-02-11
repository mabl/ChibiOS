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

/**
 * @file    PPC/vectors.s
 * @brief   PowerPC vectors table.
 *
 * @addtogroup PPC_CORE
 * @{
 */
/** @cond never */

        .section    vectors
        .align		2
        .long       IVOR0
        .long       IVOR1
        .long       IVOR2
        .long       IVOR3
        .long       IVOR4
        .long       IVOR5
        .long       IVOR6
        .long       IVOR7
        .long       IVOR8
        .long       IVOR9
        .long       IVOR10
        .long       IVOR11
        .long       IVOR12
        .long       IVOR13
        .long       IVOR14
        .long       IVOR15
        .long       IVOR16
        .long       IVOR17
        .long       IVOR18
        .long       IVOR19
        .long       IVOR20
        .long       IVOR21
        .long       IVOR22
        .long       IVOR23
        .long       IVOR24
        .long       IVOR25
        .long       IVOR26
        .long       IVOR27
        .long       IVOR28
        .long       IVOR29
        .long       IVOR30
        .long       IVOR31
        .long       IVOR32
        .long       IVOR33
        .long       IVOR34

        .section    .text
        .align		2

        .weak       IVOR0
        .globl      IVOR0
IVOR0:
        .weak       IVOR1
        .globl      IVOR1
IVOR1:
        .weak       IVOR2
        .globl      IVOR2
IVOR2:
        .weak       IVOR3
        .globl      IVOR3
IVOR3:
        .weak       IVOR4
        .globl      IVOR4
IVOR4:
        .weak       IVOR5
        .globl      IVOR5
IVOR5:
        .weak       IVOR6
        .globl      IVOR6
IVOR6:
        .weak       IVOR7
        .globl      IVOR7
IVOR7:
        .weak       IVOR8
        .globl      IVOR8
IVOR8:
        .weak       IVOR9
        .globl      IVOR9
IVOR9:
        .weak       IVOR10
        .globl      IVOR10
IVOR10:
        .weak       IVOR11
        .globl      IVOR11
IVOR11:
        .weak       IVOR12
        .globl      IVOR12
IVOR12:
        .weak       IVOR13
        .globl      IVOR13
IVOR13:
        .weak       IVOR14
        .globl      IVOR14
IVOR14:
        .weak       IVOR15
        .globl      IVOR15
IVOR15:
        .weak       IVOR16
        .globl      IVOR16
IVOR16:
        .weak       IVOR17
        .globl      IVOR17
IVOR17:
        .weak       IVOR18
        .globl      IVOR18
IVOR18:
        .weak       IVOR19
        .globl      IVOR19
IVOR19:
        .weak       IVOR20
        .globl      IVOR20
IVOR20:
        .weak       IVOR21
        .globl      IVOR21
IVOR21:
        .weak       IVOR22
        .globl      IVOR22
IVOR22:
        .weak       IVOR23
        .globl      IVOR23
IVOR23:
        .weak       IVOR24
        .globl      IVOR24
IVOR24:
        .weak       IVOR25
        .globl      IVOR25
IVOR25:
        .weak       IVOR26
        .globl      IVOR26
IVOR26:
        .weak       IVOR27
        .globl      IVOR27
IVOR27:
        .weak       IVOR28
        .globl      IVOR28
IVOR28:
        .weak       IVOR29
        .globl      IVOR29
IVOR29:
        .weak       IVOR30
        .globl      IVOR30
IVOR30:
        .weak       IVOR31
        .globl      IVOR31
IVOR31:
        .weak       IVOR32
        .globl      IVOR32
IVOR32:
        .weak       IVOR33
        .globl      IVOR33
IVOR33:
        .weak       IVOR34
        .globl      IVOR34
IVOR34:
        .weak       IVOR35
        .globl      IVOR35
IVOR35:
        .weak       IVOR36
        .globl      IVOR36
IVOR36:
        .weak       IVOR37
        .globl      IVOR37
IVOR37:

here:   b           here

/** @endcond */
/** @} */
