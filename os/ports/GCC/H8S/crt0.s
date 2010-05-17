
/****************************************************************
;KPIT Cummins Infosystems Ltd, Pune, India. - 19-Apr-2005
;
;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
;
;*****************************************************************/


	.h8300s

	.global _start
	.global _exit

#ifdef CPPAPP
___dso_handle:
	.global ___dso_handle
#endif

	.extern _hw_initialise
	.extern _main

	.extern _data
	.extern _mdata
	.extern _edata
	.extern _bss
	.extern _ebss
	.extern _stack

	.text
	.section	.start, "ax"

_start:
	bra		1f

	.ascii	"FIRM"
	.long	0
	.long	0
	.long	0

1:
	; initialise the SP for non-vectored code
	mov.l   #__ram_end__,er7
	; call the hardware initialiser
;	jsr     @_hw_initialise

#ifdef ROMSTART
	; get the boundaries for the .data section initialisation
    mov.l   #_data,er0
    mov.l   #_edata,er1
    mov.l   #_etext,er2
    cmp.l   er0,er1
	beq     2f
1:
    mov.b   @er2,r3l  ;get from src
    mov.b   r3l,@er0  ;place in dest
    inc.l   #1,er2    ;inc src
    inc.l   #1,er0    ;inc dest
    cmp.l   er0,er1   ;dest == edata?
	bne     1b
2:
#endif		//ROMSTART

	; zero out bss
    mov.l   #__bss_start,er0
    mov.l   #__bss_end,er1
    cmp.l   er0,er1
	beq     2f
	sub.b   r2l,r2l
1:
    mov.b   r2l,@er0
    inc.l   #1,er0
    cmp.l   er0,er1
	bne     1b
2:

	jsr		@_hwinit

/*
	TODO: Insert ctors init here.
*/

#ifdef CPPAPP
	;Initialize global constructor
	jsr		@___main
#endif

	jsr		@_main
	rts

	; jump to exit
;	mov.l   er0,er4
;	jmp     @_exit

_exit:
	nop
	bra     _exit

