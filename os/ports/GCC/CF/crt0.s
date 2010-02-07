
/*	#include "asm.h"*/

	.title "crt0.S for m68k-elf"

	.text
	.section	.start, "ax"

	braw	start

	.ascii	"FIRM"
	.long	0
	.long	0
	.long	0

start:
/*
	move	#0x2700, %sr
	movel	#0xE0002000, %a7
*/
	movel	#__ram_end__, %a7


	movel	#0x01000000, %d0
	movec	%d0, %CACR
	movel	#0x80000100, %d0
	movec	%d0, %CACR

	movel	#__bss_start,%d1
	movel	#__bss_end, %d0
	cmpl	%d0, %d1
	beq		2f
	movl	%d1, %a0
	subl	%d1, %d0
	subql	#1, %d0
1:	clrb	(%a0)+
	subql	#1, %d0
	bpl		1b

2:	lea		_etext,%a1
	lea		_data,%a0
	cmpl	%a0,%a1
	beq.s	NOCOPY
	movel	#_edata,%d0
	bra.s	COPYLOOPTEST
COPYLOOP:
	moveb	%a1@+,%a0@+
COPYLOOPTEST:
	cmpl	%d0,%a0
	bcs.s	COPYLOOP
NOCOPY:

	jsr		hwinit

/*
	TODO: Insert ctors init here.
*/

	move	#0x2000, %sr
	jsr		main
	rts

