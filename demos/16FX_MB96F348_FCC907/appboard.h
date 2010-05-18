/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _APPBOARD_H_
#define _APPBOARD_H_

//--------------------------------------------------------//
// Definizione delle costanti di compilazione
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizioni globali di applicazione
//--------------------------------------------------------//


//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define RESOLUTION_RELOAD_TIMER_0    (1000000.0/(CLKP1_FREQ/32))
#define RESOLUTION_RELOAD_TIMER_1    (1000000.0/(CLKP1_FREQ/32))
#define RESOLUTION_RELOAD_TIMER_2    (1000000.0/(CLKP1_FREQ/16))
#define RESOLUTION_RELOAD_TIMER_3    (1000000.0/(CLKP1_FREQ/16))
#define RESOLUTION_RELOAD_TIMER_6    (1000000.0/(CLKP1_FREQ/32))

											// conversione in timer counts
#define uSEC_TO_RELTIM0(usec) ((usec/RESOLUTION_RELOAD_TIMER_0)-1)
#define uSEC_TO_RELTIM1(usec) ((usec/RESOLUTION_RELOAD_TIMER_1)-1)
#define uSEC_TO_RELTIM2(usec) ((usec/RESOLUTION_RELOAD_TIMER_2)-1)
#define uSEC_TO_RELTIM3(usec) ((usec/RESOLUTION_RELOAD_TIMER_3)-1)
#define uSEC_TO_RELTIM6(usec) ((usec/RESOLUTION_RELOAD_TIMER_6)-1)

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//
void app_hwinit(void);

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//


//--------------------------------------------------------//
// Inclusione altri files .....
//--------------------------------------------------------//


#endif /* _APPBOARD_H_ */
