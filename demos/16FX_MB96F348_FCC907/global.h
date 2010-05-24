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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//--------------------------------------------------------//
// Definizione delle costanti di compilazione
//--------------------------------------------------------//


//--------------------------------------------------------//
// Definizioni globali di applicazione
//--------------------------------------------------------//

									// definizione tipologie di applicazione disponibili
									// tramite compilazione condizionale
#define APP_DBGGSM	    0x9999
#define APP_ECHOSER     0xaaaa

                                    // Configurazione fondamentale
#define APP_TYPE            APP_DBGGSM    	// tipo app da realizzare
#define __DEBUG__                           // debug mode oppure no
//#define __SIMUL__                         // simulate mode oppure no
//#define __ACCEMIC__          				// MONITOR Accemic incluso oppure no
											// adesso si chiama EUROSCOPE ma non 
											// stiamo a cambiare le defines

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

#define ON      1
#define OFF     0
#define PI_GR       3.141592654         /* Pi Greco                             */

#define LOBYTE(x)             ((UBYTE) ((x) & 0xff))
#define HIBYTE(x)             ((UBYTE) ((x) >> 8))
#define BYTES_TO_WORD(hi,lo)  (UWORD) (((UWORD)(hi) << 8) | (UWORD)(lo))

#define LOWORD(x)             ((UWORD) ((x) & 0xffff))
#define HIWORD(x)             ((UWORD) ((x) >> 16))
#define WORD_TO_LONG(hi,lo)   (ULONG) (((ULONG)(hi) << 16) | (ULONG)(lo))


//--------------------------------------------------------//
// Definizione delle costanti di applicazione
//--------------------------------------------------------//


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//


#define __BYTE__
typedef unsigned char    byte;

#define __WORD__
typedef unsigned short   word;

#define __DWORD__
typedef unsigned long   dword;


#ifdef __cplusplus
#else
#define __BOOL__

enum bool { False, True };          // nel 'C' ancora non c'erano.....
typedef enum bool bool;

#endif


typedef const char* textp;          // serve per mettere in un segmento const
                                    // gli array di puntatori a stringhe

typedef void (*funp)();             // serve per mettere in un segmento const
                                    // gli array di puntatori a funzioni

typedef void (*funpv)(void);        // serve per mettere in un segmento const
                                    // gli array di puntatori a funzioni

typedef void (*funpi)(int);         // serve per mettere in un segmento const
                                    // gli array di puntatori a funzioni

typedef void (*funpb)(byte);


struct BitVar{                       /* tipo per variabile gestita a bittini */
    unsigned B0  : 1;                /* nota: devo definire 16 bit perche` il */
    unsigned B1  : 1;                /* compilatore comunque riserva 2 bytes */
    unsigned B2  : 1;                /* anche se definisco 8 bit */
    unsigned B3  : 1;
    unsigned B4  : 1;
    unsigned B5  : 1;
    unsigned B6  : 1;
    unsigned B7  : 1;
    unsigned B8  : 1;
    unsigned B9  : 1;
    unsigned B10 : 1;
    unsigned B11 : 1;
    unsigned B12 : 1;
    unsigned B13 : 1;
    unsigned B14 : 1;
    unsigned B15 : 1;
};

typedef struct BitVar BitVar;

union IoVar{                        /* aggiunta per gestione a word e char */
    BitVar  bits;
    word    var;
    char    sing[2];
};

typedef union IoVar IoVar;


//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//


//--------------------------------------------------------//
// Inclusione altri files .....
//--------------------------------------------------------//


#endif /* _GLOBAL_H_ */
