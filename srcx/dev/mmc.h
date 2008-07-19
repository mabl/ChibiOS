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

#ifndef _MMC_H_
#define _MMC_H_

#define CMD0_RETRY              10
#define CMD1_RETRY              100
#define POLLING_INTERVAL        10
#define MMC_WAIT_DATA           10000

#define CMDGOIDLE               0
#define CMDINIT                 1
#define CMDREADCSD              9
#define CMDSTOP                 12
#define CMDREAD                 17
#define CMDREADMULTIPLE         18
#define CMDWRITE                24
#define CMDWRITEMULTIPLE        25

void mmc_init(void);
sysret_t mmc_open(unsigned minor, bool_t ro);
sysret_t mmc_close(unsigned minor);
void mmc_strategy(struct buf *bp);

extern EventSource MMCInsertEventSource, MMCRemoveEventSource;

#endif /* _MMC_H_ */
