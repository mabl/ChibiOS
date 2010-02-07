/*
 * File:		mcf5206e.h
 * Purpose:		mcf5206e definitions
 *
 */

#ifndef _CPU_mcf5206e_H
#define _CPU_mcf5206e_H

#ifdef __MWERKS__
/* Turn off C++ so functions can be called from assembly */
#pragma cplusplus off

/* Turn off optimizations to prevent inadvertent optimizing of critical code */
#pragma optimization_level 0

#endif

/***********************************************************************/
/*
 * Misc. Defines
 */

#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	(1)

#ifdef	NULL
#undef	NULL
#endif
#define NULL	(0)

/***********************************************************************/
/*
 * The basic data types
 */

typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef signed char			int8;   /*  8 bits */
typedef signed short int	int16;  /* 16 bits */
typedef signed long int		int32;  /* 32 bits */

/***********************************************************************/
/*
 * Common M68K & ColdFire definitions
 */

#define ADDRESS			uint32
#define INSTRUCTION		uint16
#define ILLEGAL			0x4AFC
#define CPU_WORD_SIZE	16

/***********************************************************************/

/*
 * Routines and macros for accessing Input/Output devices
 */

#define cpu_iord_8(ADDR)        *((volatile uint8 *)(ADDR))
#define cpu_iord_16(ADDR)       *((volatile uint16 *)(ADDR))
#define cpu_iord_32(ADDR)       *((volatile uint32 *)(ADDR))

#define cpu_iowr_8(ADDR,DATA)   *((volatile uint8 *)(ADDR)) = (DATA)
#define cpu_iowr_16(ADDR,DATA)  *((volatile uint16 *)(ADDR)) = (DATA)
#define cpu_iowr_32(ADDR,DATA)  *((volatile uint32 *)(ADDR)) = (DATA)

/***********************************************************************/

#define MCF5200_SR_T	(0x8000)
#define MCF5200_SR_S	(0x2000)
#define MCF5200_SR_M	(0x1000)
#define MCF5200_SR_IPL	(0x0700)
#define MCF5200_SR_IPL_0	(0x0000)
#define MCF5200_SR_IPL_1	(0x0100)
#define MCF5200_SR_IPL_2	(0x0200)
#define MCF5200_SR_IPL_3	(0x0300)
#define MCF5200_SR_IPL_4	(0x0400)
#define MCF5200_SR_IPL_5	(0x0500)
#define MCF5200_SR_IPL_6	(0x0600)
#define MCF5200_SR_IPL_7	(0x0700)
#define MCF5200_SR_X	(0x0010)
#define MCF5200_SR_N	(0x0008)
#define MCF5200_SR_Z	(0x0004)
#define MCF5200_SR_V	(0x0002)
#define MCF5200_SR_C	(0x0001)

/***********************************************************************/

/*
 * The ColdFire family of processors has a simplified exception stack
 * frame that looks like the following:
 *
 *              3322222222221111 111111
 *              1098765432109876 5432109876543210
 *           8 +----------------+----------------+
 *             |         Program Counter         |
 *           4 +----------------+----------------+
 *             |FS/Fmt/Vector/FS|      SR        |
 *   SP -->  0 +----------------+----------------+
 *
 * The stack self-aligns to a 4-byte boundary at an exception, with
 * the FS/Fmt/Vector/FS field indicating the size of the adjustment
 * (SP += 0,1,2,3 bytes).
 */

#define MCF5200_RD_SF_FORMAT(PTR)	\
	((*((uint16 *)(PTR)) >> 12) & 0x00FF)

#define MCF5200_RD_SF_VECTOR(PTR)	\
	((*((uint16 *)(PTR)) >>  2) & 0x00FF)

#define MCF5200_RD_SF_FS(PTR)		\
	( ((*((uint16 *)(PTR)) & 0x0C00) >> 8) | (*((uint16 *)(PTR)) & 0x0003) )

#define MCF5200_SF_SR(PTR)	*((uint16 *)(PTR)+1)
#define MCF5200_SF_PC(PTR)	*((uint32 *)(PTR)+1)

#if 0
typedef struct
{
	uint16	SR;
	uint16	FS_FMT_VECTOR_FS;
	uint32	PC;
} MCF5200_STACK_FRAME;
#endif
/***********************************************************************/

/*
 * Function prototypes for ColdFire processors
 *
 * MCF5200/D:	Only VBR valid
 * MCF5202:		Only VBR, CACR, ACR0, ACR1 valid
 * MCF5204:		All valid
 * MCF5206:		All valid
 * MCF5206e:	All valid
 */
void
mcf5206e_wr_vbr (uint32);

void
mcf5206e_wr_cacr (uint32);

void
mcf5206e_wr_acr0 (uint32);

void
mcf5206e_wr_acr1 (uint32);

void
mcf5206e_wr_rambar (uint32);

void
mcf5206e_wr_mbar (uint32);

/***********************************************************************/

/*
 * Cache and Access Control Register routines and defintions
 */
#define mcf5206e_CACR_CENB		(0x80000000)
#define mcf5206e_CACR_CFRZ		(0x08000000)
#define mcf5206e_CACR_CINV		(0x01000000)
#define mcf5206e_CACR_CMOD		(0x00000200)
#define mcf5206e_CACR_CWRP		(0x00000020)
#define mcf5206e_CACR_CLNF_00	(0x00000000)
#define mcf5206e_CACR_CLNF_01	(0x00000001)
#define mcf5206e_CACR_CLNF_10	(0x00000002)

#define mcf5206e_ACR_BASE(a)	((a)&0xFF000000)
#define mcf5206e_ACR_MASK(a)	(((a)&0xFF000000) >> 8)
#define mcf5206e_ACR_EN			(0x00008000)
#define mcf5206e_ACR_S_USER		(0x00000000)
#define mcf5206e_ACR_S_SUPER	(0x00002000)
#define mcf5206e_ACR_S_IGNORE	(0x00006000)
#define mcf5206e_ACR_ENIB		(0x00000080)
#define mcf5206e_ACR_CM			(0x00000040)
#define mcf5206e_ACR_WP			(0x00000004)

#define mcf5206e_SRAMBAR_BASE(a) ((a)&0xFFFFFE00)
#define mcf5206e_SRAMBAR_WP		(0x00000100)
#define mcf5206e_SRAMBAR_AS_CI	(0x00000080)
#define mcf5206e_SRAMBAR_AS_SC	(0x00000040)
#define mcf5206e_SRAMBAR_AS_SD	(0x00000020)
#define mcf5206e_SRAMBAR_AS_UC	(0x00000004)
#define mcf5206e_SRAMBAR_AS_UD	(0x00000002)
#define mcf5206e_SRAMBAR_V		(0x00000001)

#define mcf5206e_MBAR_BASE(a)	((a)&0xFFFFFC00)
#define mcf5206e_MBAR_SC		(0x00000010)
#define mcf5206e_MBAR_SD		(0x00000008)
#define mcf5206e_MBAR_UC		(0x00000004)
#define mcf5206e_MBAR_UD		(0x00000002)
#define mcf5206e_MBAR_V			(0x00000001)

/***********************************************************************/

/*
 * Macro for computing address of on-chip peripheral registers
 */
#define mcf5206e_addr(IMMP,OFFSET)   ((void *)&((uint8 *)IMMP)[OFFSET])

/*
 * Macros for accessing the on-chip I/O resources
 */
#define mcf5206e_iord(IMMP,OFFSET,SIZE)		\
    *(volatile uint ## SIZE *)(mcf5206e_addr(IMMP,OFFSET))

#define mcf5206e_iowr(IMMP,OFFSET,SIZE,DATA)  \
    *(volatile uint ## SIZE *)(mcf5206e_addr(IMMP,OFFSET)) = (DATA)

/***********************************************************************/

/*
 * System Integration Modules, SIM
 */

/* Offsets of the registers from the MBAR */
#define mcf5206e_SIM_SIMR		(0x0003)
#define mcf5206e_SIM_ICR1		(0x0014)
#define mcf5206e_SIM_ICR2		(0x0015)
#define mcf5206e_SIM_ICR3		(0x0016)
#define mcf5206e_SIM_ICR4		(0x0017)
#define mcf5206e_SIM_ICR5		(0x0018)
#define mcf5206e_SIM_ICR6		(0x0019)
#define mcf5206e_SIM_ICR7		(0x001A)
#define mcf5206e_SIM_ICR8		(0x001B)
#define mcf5206e_SIM_ICR9		(0x001C)
#define mcf5206e_SIM_ICR10		(0x001D)
#define mcf5206e_SIM_ICR11		(0x001E)
#define mcf5206e_SIM_ICR12		(0x001F)
#define mcf5206e_SIM_ICR13		(0x0020)
#define mcf5206e_SIM_IMR			(0x0036)
#define mcf5206e_SIM_IPR			(0x003A)
#define mcf5206e_SIM_RSR			(0x0040)
#define mcf5206e_SIM_SYPCR		(0x0041)
#define mcf5206e_SIM_SWIVR		(0x0042)
#define mcf5206e_SIM_SWSR		(0x0043)
#define mcf5206e_SIM_PAR			(0x00CA)

/* Read access macros for general use */
#define mcf5206e_RD_SIM_SIMR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_SIMR,8)
#define mcf5206e_RD_SIM_ICR1(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR1,8)
#define mcf5206e_RD_SIM_ICR2(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR2,8)
#define mcf5206e_RD_SIM_ICR3(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR3,8)
#define mcf5206e_RD_SIM_ICR4(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR4,8)
#define mcf5206e_RD_SIM_ICR5(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR5,8)
#define mcf5206e_RD_SIM_ICR6(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR6,8)
#define mcf5206e_RD_SIM_ICR7(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR7,8)
#define mcf5206e_RD_SIM_ICR8(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR8,8)
#define mcf5206e_RD_SIM_ICR9(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR9,8)
#define mcf5206e_RD_SIM_ICR10(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR10,8)
#define mcf5206e_RD_SIM_ICR11(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR11,8)
#define mcf5206e_RD_SIM_ICR12(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR12,8)
#define mcf5206e_RD_SIM_ICR13(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_ICR13,8)
#define mcf5206e_RD_SIM_IMR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_IMR,16)
#define mcf5206e_RD_SIM_IPR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_IPR,16)
#define mcf5206e_RD_SIM_RSR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_RSR,8)
#define mcf5206e_RD_SIM_SYPCR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_SYPCR,8)
#define mcf5206e_RD_SIM_SWIVR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_SWIVR,8)
#define mcf5206e_RD_SIM_PAR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_SIM_PAR,16)

/* Write access macros for general use */
#define mcf5206e_WR_SIM_SIMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_SIMR,8,DATA)
#define mcf5206e_WR_SIM_ICR1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR1,8,DATA)
#define mcf5206e_WR_SIM_ICR2(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR2,8,DATA)
#define mcf5206e_WR_SIM_ICR3(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR3,8,DATA)
#define mcf5206e_WR_SIM_ICR4(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR4,8,DATA)
#define mcf5206e_WR_SIM_ICR5(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR5,8,DATA)
#define mcf5206e_WR_SIM_ICR6(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR6,8,DATA)
#define mcf5206e_WR_SIM_ICR7(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR7,8,DATA)
#define mcf5206e_WR_SIM_ICR8(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR8,8,DATA)
#define mcf5206e_WR_SIM_ICR9(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR9,8,DATA)
#define mcf5206e_WR_SIM_ICR10(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR10,8,DATA)
#define mcf5206e_WR_SIM_ICR11(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR11,8,DATA)
#define mcf5206e_WR_SIM_ICR12(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR12,8,DATA)
#define mcf5206e_WR_SIM_ICR13(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_ICR13,8,DATA)
#define mcf5206e_WR_SIM_IMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_IMR,16,DATA)
#define mcf5206e_WR_SIM_RSR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_RSR,8,DATA)
#define mcf5206e_WR_SIM_SYPCR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_SYPCR,8,DATA)
#define mcf5206e_WR_SIM_SWIVR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_SWIVR,8,DATA)
#define mcf5206e_WR_SIM_SWSR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_SWSR,8,DATA)
#define mcf5206e_WR_SIM_PAR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_SIM_PAR,16,DATA)

#if 1
typedef volatile struct
{
	uint8	reserved0;
	uint8	reserved1;
	uint8	reserved2;
	uint8	SIMR;
	uint32	reserved3;
	uint32	reserved4;
	uint32	reserved5;
	uint32	reserved6;
	uint8	ICR1;
	uint8	ICR2;
	uint8	ICR3;
	uint8	ICR4;
	uint8	ICR5;
	uint8	ICR6;
	uint8	ICR7;
	uint8	ICR8;
	uint8	ICR9;
	uint8	ICR10;
	uint8	ICR11;
	uint8	ICR12;
	uint8	ICR13;
	uint8	reserved7;
	uint8	reserved8;
	uint8	reserved9;
	uint32	reserved10[4];
	uint8	reserved11;
	uint8	reserved12;
	uint16	IMR;
	uint16	reserved13;
	uint16	IPR;
	uint32	reserved14;
	uint8	RSR;
	uint8	SYPCR;
	uint8	SWIVR;
	uint8	SWSR;
	uint8	reserved31[134];
	uint8	PAR;
} mcf5206e_SIM;
#endif

#define mcf5206e_SIM_SIMR_FRZ1		(0x80)
#define mcf5206e_SIM_SIMR_FRZ0		(0x40)
#define mcf5206e_SIM_SIMR_BL		(0x01)

#define mcf5206e_SIM_ICR_AVEC		(0x80)
#define mcf5206e_SIM_ICR_IL(a)		(((a)&0x07)<<2)
#define mcf5206e_SIM_ICR_IP(a)		(((a)&0x03))

#define mcf5206e_SIM_IMR_DMA1		(0x8000)
#define mcf5206e_SIM_IMR_DMA0		(0x4000)

#define mcf5206e_SIM_IMR_UART2		(0x2000)
#define mcf5206e_SIM_IMR_UART1		(0x1000)
#define mcf5206e_SIM_IMR_MBUS		(0x0800)
#define mcf5206e_SIM_IMR_T2			(0x0400)
#define mcf5206e_SIM_IMR_T1			(0x0200)
#define mcf5206e_SIM_IMR_SWT			(0x0100)
#define mcf5206e_SIM_IMR_EINT7		(0x0080)
#define mcf5206e_SIM_IMR_EINT6		(0x0040)
#define mcf5206e_SIM_IMR_EINT5		(0x0020)
#define mcf5206e_SIM_IMR_EINT4		(0x0010)
#define mcf5206e_SIM_IMR_EINT3		(0x0008)
#define mcf5206e_SIM_IMR_EINT2		(0x0004)
#define mcf5206e_SIM_IMR_EINT1		(0x0002)

#define mcf5206e_SIM_IPR_UART2		(0x2000)
#define mcf5206e_SIM_IPR_UART1		(0x1000)
#define mcf5206e_SIM_IPR_MBUS		(0x0800)
#define mcf5206e_SIM_IPR_T2			(0x0400)
#define mcf5206e_SIM_IPR_T1			(0x0200)
#define mcf5206e_SIM_IPR_SWT			(0x0100)
#define mcf5206e_SIM_IPR_EINT7		(0x0080)
#define mcf5206e_SIM_IPR_EINT6		(0x0040)
#define mcf5206e_SIM_IPR_EINT5		(0x0020)
#define mcf5206e_SIM_IPR_EINT4		(0x0010)
#define mcf5206e_SIM_IPR_EINT3		(0x0008)
#define mcf5206e_SIM_IPR_EINT2		(0x0004)
#define mcf5206e_SIM_IPR_EINT1		(0x0002)

#define mcf5206e_SIM_RSR_HRST		(0x80)
#define mcf5206e_SIM_RSR_SWTR		(0x20)

#define mcf5206e_SIM_SYPCR_SWE		(0x80)
#define mcf5206e_SIM_SYPCR_SWRI		(0x40)
#define mcf5206e_SIM_SYPCR_SWT_2_9	(0x00)
#define mcf5206e_SIM_SYPCR_SWT_2_11	(0x08)
#define mcf5206e_SIM_SYPCR_SWT_2_13	(0x10)
#define mcf5206e_SIM_SYPCR_SWT_2_15	(0x18)
#define mcf5206e_SIM_SYPCR_SWT_2_18	(0x20)
#define mcf5206e_SIM_SYPCR_SWT_2_20	(0x28)
#define mcf5206e_SIM_SYPCR_SWT_2_22	(0x30)
#define mcf5206e_SIM_SYPCR_SWT_2_24	(0x38)
#define mcf5206e_SIM_SYPCR_BME		(0x04)
#define mcf5206e_SIM_SYPCR_BM_1024	(0x00)
#define mcf5206e_SIM_SYPCR_BM_512	(0x01)
#define mcf5206e_SIM_SYPCR_BM_256	(0x02)
#define mcf5206e_SIM_SYPCR_BM_128	(0x03)

#define mcf5206e_SIM_SWIVR_SWIV(a)	((a)&0x00FF)

#define mcf5206e_SIM_PAR_PAR9		(0x0200)
#define mcf5206e_SIM_PAR_PAR8_DMA1	(0x0200)
#define mcf5206e_SIM_PAR_PAR8_TIN1	(0x0000)
#define mcf5206e_SIM_PAR_PAR8_DMA0	(0x0100)
#define mcf5206e_SIM_PAR_PAR8_TIN0	(0x0000)
#define mcf5206e_SIM_PAR_PAR8		(0x0100)
#define mcf5206e_SIM_PAR_PAR7		(0x0080)
#define mcf5206e_SIM_PAR_PAR7_RSTO	(0x0000)
#define mcf5206e_SIM_PAR_PAR7_RTS2	(0x0080)
#define mcf5206e_SIM_PAR_PAR6		(0x0040)
#define mcf5206e_SIM_PAR_PAR6_IRQ	(0x0000)
#define mcf5206e_SIM_PAR_PAR6_IPL	(0x0040)
#define mcf5206e_SIM_PAR_PAR5		(0x0020)
#define mcf5206e_SIM_PAR_PAR5_PP74	(0x0000)
#define mcf5206e_SIM_PAR_PAR5_PST	(0x0020)
#define mcf5206e_SIM_PAR_PAR4		(0x0010)
#define mcf5206e_SIM_PAR_PAR4_PP30	(0x0000)
#define mcf5206e_SIM_PAR_PAR4_DDATA	(0x0010)
#define mcf5206e_SIM_PAR_PAR3		(0x0008)
#define mcf5206e_SIM_PAR_PAR2		(0x0004)
#define mcf5206e_SIM_PAR_PAR1		(0x0002)
#define mcf5206e_SIM_PAR_PAR0		(0x0001)

/***********************************************************************/

/*
 * Chip-Select Module, CS
 */

/* Offsets of the registers from the MBAR */
#define mcf5206e_CS_CSAR0		(0x0064)
#define mcf5206e_CS_CSMR0		(0x0068)
#define mcf5206e_CS_CSCR0		(0x006E)
#define mcf5206e_CS_CSAR1		(0x0070)
#define mcf5206e_CS_CSMR1		(0x0074)
#define mcf5206e_CS_CSCR1		(0x007A)
#define mcf5206e_CS_CSAR2		(0x007C)
#define mcf5206e_CS_CSMR2		(0x0080)
#define mcf5206e_CS_CSCR2		(0x0086)
#define mcf5206e_CS_CSAR3		(0x0088)
#define mcf5206e_CS_CSMR3		(0x008C)
#define mcf5206e_CS_CSCR3		(0x0092)
#define mcf5206e_CS_CSAR4		(0x0094)
#define mcf5206e_CS_CSMR4		(0x0098)
#define mcf5206e_CS_CSCR4		(0x009E)
#define mcf5206e_CS_CSAR5		(0x00A0)
#define mcf5206e_CS_CSMR5		(0x00A4)
#define mcf5206e_CS_CSCR5		(0x00AA)
#define mcf5206e_CS_CSAR6		(0x00AC)
#define mcf5206e_CS_CSMR6		(0x00B0)
#define mcf5206e_CS_CSCR6		(0x00B6)
#define mcf5206e_CS_CSAR7		(0x00B8)
#define mcf5206e_CS_CSMR7		(0x00BC)
#define mcf5206e_CS_CSCR7		(0x00C2)
#define mcf5206e_CS_DMCR		(0x00C6)

/* Read access macros for general use */
#define mcf5206e_RD_CS_CSAR0(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR0,16)
#define mcf5206e_RD_CS_CSMR0(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR0,32)
#define mcf5206e_RD_CS_CSCR0(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR0,16)
#define mcf5206e_RD_CS_CSAR1(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR1,16)
#define mcf5206e_RD_CS_CSMR1(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR1,32)
#define mcf5206e_RD_CS_CSCR1(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR1,16)
#define mcf5206e_RD_CS_CSAR2(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR2,16)
#define mcf5206e_RD_CS_CSMR2(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR2,32)
#define mcf5206e_RD_CS_CSCR2(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR2,16)
#define mcf5206e_RD_CS_CSAR3(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR3,16)
#define mcf5206e_RD_CS_CSMR3(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR3,32)
#define mcf5206e_RD_CS_CSCR3(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR3,16)
#define mcf5206e_RD_CS_CSAR4(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR4,16)
#define mcf5206e_RD_CS_CSMR4(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR4,32)
#define mcf5206e_RD_CS_CSCR4(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR4,16)
#define mcf5206e_RD_CS_CSAR5(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR5,16)
#define mcf5206e_RD_CS_CSMR5(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR5,32)
#define mcf5206e_RD_CS_CSCR5(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR5,16)
#define mcf5206e_RD_CS_CSAR6(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR6,16)
#define mcf5206e_RD_CS_CSMR6(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR6,32)
#define mcf5206e_RD_CS_CSCR6(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR6,16)
#define mcf5206e_RD_CS_CSAR7(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSAR7,16)
#define mcf5206e_RD_CS_CSMR7(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSMR7,32)
#define mcf5206e_RD_CS_CSCR7(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_CSCR7,16)
#define mcf5206e_RD_CS_DMCR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_CS_DMCR,16)

/* Write access macros for general use */
#define mcf5206e_WR_CS_CSAR0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR0,16,DATA)
#define mcf5206e_WR_CS_CSMR0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR0,32,DATA)
#define mcf5206e_WR_CS_CSCR0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR0,16,DATA)
#define mcf5206e_WR_CS_CSAR1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR1,16,DATA)
#define mcf5206e_WR_CS_CSMR1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR1,32,DATA)
#define mcf5206e_WR_CS_CSCR1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR1,16,DATA)
#define mcf5206e_WR_CS_CSAR2(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR2,16,DATA)
#define mcf5206e_WR_CS_CSMR2(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR2,32,DATA)
#define mcf5206e_WR_CS_CSCR2(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR2,16,DATA)
#define mcf5206e_WR_CS_CSAR3(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR3,16,DATA)
#define mcf5206e_WR_CS_CSMR3(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR3,32,DATA)
#define mcf5206e_WR_CS_CSCR3(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR3,16,DATA)
#define mcf5206e_WR_CS_CSAR4(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR4,16,DATA)
#define mcf5206e_WR_CS_CSMR4(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR4,32,DATA)
#define mcf5206e_WR_CS_CSCR4(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR4,16,DATA)
#define mcf5206e_WR_CS_CSAR5(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR5,16,DATA)
#define mcf5206e_WR_CS_CSMR5(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR5,32,DATA)
#define mcf5206e_WR_CS_CSCR5(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR5,16,DATA)
#define mcf5206e_WR_CS_CSAR6(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR6,16,DATA)
#define mcf5206e_WR_CS_CSMR6(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR6,32,DATA)
#define mcf5206e_WR_CS_CSCR6(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR6,16,DATA)
#define mcf5206e_WR_CS_CSAR7(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSAR7,16,DATA)
#define mcf5206e_WR_CS_CSMR7(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSMR7,32,DATA)
#define mcf5206e_WR_CS_CSCR7(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_CSCR7,16,DATA)
#define mcf5206e_WR_CS_DMCR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_CS_DMCR,16,DATA)

#if 1
typedef volatile struct
{
	uint32	reserved1[0x19];
	uint16	CSAR0;
	uint16  fillerA;
	uint32	CSMR0;
	uint16	reserved2;
	uint16	CSCR0;
	uint16	CSAR1;
	uint16  fillerB;
	uint32	CSMR1;
	uint16	reserved4;
	uint16	CSCR1;
	uint16	CSAR2;
	uint16  fillerC;
	uint32	CSMR2;
	uint16	reserved6;
	uint16	CSCR2;
	uint16	CSAR3;
	uint16  fillerD;
	uint32	CSMR3;
	uint16	reserved8;
	uint16	CSCR3;
	uint16	CSAR4;
	uint16  fillerE;
	uint32	CSMR4;
	uint16	reserved10;
	uint16	CSCR4;
	uint16	CSAR5;
	uint16  fillerF;
	uint32	CSMR5;
	uint16	reserved12;
	uint16	CSCR5;
	uint16	CSAR6;
	uint16  fillerG;
	uint32	CSMR6;
	uint16	reserved14;
	uint16	CSCR6;
	uint16	CSAR7;
	uint16  fillerH;
	uint32	CSMR7;
	uint16	reserved16;
	uint16	CSCR7;
	uint16	reserved17;
	uint16	DMCR;
} mcf5206e_CS;
#endif

#define mcf5206e_CS_CSAR_BASE(a)		(((a)&0xFFFF0000)>>16)

#define mcf5206e_CS_CSMR_MASK_32M		(0x01FF0000)
#define mcf5206e_CS_CSMR_MASK_16M		(0x00FF0000)
#define mcf5206e_CS_CSMR_MASK_8M		(0x007F0000)
#define mcf5206e_CS_CSMR_MASK_4M		(0x003F0000)
#define mcf5206e_CS_CSMR_MASK_2M		(0x001F0000)
#define mcf5206e_CS_CSMR_MASK_1M		(0x000F0000)
#define mcf5206e_CS_CSMR_MASK_1024K		(0x000F0000)
#define mcf5206e_CS_CSMR_MASK_512K		(0x00070000)
#define mcf5206e_CS_CSMR_MASK_256K		(0x00030000)
#define mcf5206e_CS_CSMR_MASK_128K		(0x00010000)
#define mcf5206e_CS_CSMR_MASK_64K		(0x00000000)
#define mcf5206e_CS_CSMR_SC				(0x00000010)
#define mcf5206e_CS_CSMR_SD				(0x00000008)
#define mcf5206e_CS_CSMR_UC				(0x00000004)
#define mcf5206e_CS_CSMR_UD				(0x00000002)
#define mcf5206e_CS_CSMR1_CPU			(0x00000020)

#define mcf5206e_CS_CSCR_WS_MASK		(0x3C00)
#define mcf5206e_CS_CSCR_WS(a)			(((a)&0x0F)<<10)
#define mcf5206e_CS_CSCR_BRST			(0x0200)
#define mcf5206e_CS_CSCR_AA				(0x0100)
#define mcf5206e_CS_CSCR_PS_8			(0x0040)
#define mcf5206e_CS_CSCR_PS_16			(0x0080)
#define mcf5206e_CS_CSCR_PS_32			(0x0000)
#define mcf5206e_CS_CSCR_EMAA			(0x0020)
#define mcf5206e_CS_CSCR_ASET			(0x0010)
#define mcf5206e_CS_CSCR_WRAH			(0x0008)
#define mcf5206e_CS_CSCR_RDAH			(0x0004)
#define mcf5206e_CS_CSCR_WR				(0x0002)
#define mcf5206e_CS_CSCR_RD				(0x0001)

#define mcf5206e_CS_DMCR_WS_MASK		(0x3C00)
#define mcf5206e_CS_DMCR_WS(a)			(((a)&0x0F)<<10)
#define mcf5206e_CS_DMCR_BRST			(0x0200)
#define mcf5206e_CS_DMCR_AA				(0x0100)
#define mcf5206e_CS_DMCR_PS_8			(0x0040)
#define mcf5206e_CS_DMCR_PS_16			(0x0080)
#define mcf5206e_CS_DMCR_PS_32			(0x0000)
#define mcf5206e_CS_DMCR_EMAA			(0x0020)
#define mcf5206e_CS_DMCR_WRAH			(0x0008)
#define mcf5206e_CS_DMCR_RDAH			(0x0004)

#define mcf5206e_IACK_ADDRESS	(mcf5206e_CS_CSAR_BASE(0xFFFFFFE0))
#define mcf5206e_IACK_MASK		(mcf5206e_CS_CSMR_MASK_64K)

/***********************************************************************/

/*
 * Parallel Port (General Purpose I/O) Module, PP
 */

/* Offsets of the registers from the MBAR */
#define mcf5206e_PP_PPDDR		(0x01C5)
#define mcf5206e_PP_PPDAT		(0x01C9)

/* Read access macros for general use */
#define mcf5206e_RD_PP_PPDDR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_PP_PPDDR,8)
#define mcf5206e_RD_PP_PPDAT(IMMP)	mcf5206e_iord(IMMP,mcf5206e_PP_PPDAT,8)

/* Write access macros for general use */
#define mcf5206e_WR_PP_PPDDR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_PP_PPDDR,8,DATA)
#define mcf5206e_WR_PP_PPDAT(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_PP_PPDAT,8,DATA)

#if 1
typedef volatile struct
{
	uint8	reserved0[0x1C5];
	uint8	PPDDR;
	uint8	reserved34;
	uint8	reserved35;
	uint8	reserved36;
	uint8	PPDAT;
} mcf5206e_PP;
#endif

#define mcf5206e_PP_PPDDR_DDR7			(0x80)
#define mcf5206e_PP_PPDDR_DDR6			(0x40)
#define mcf5206e_PP_PPDDR_DDR5			(0x20)
#define mcf5206e_PP_PPDDR_DDR4			(0x10)
#define mcf5206e_PP_PPDDR_DDR3			(0x08)
#define mcf5206e_PP_PPDDR_DDR2			(0x04)
#define mcf5206e_PP_PPDDR_DDR1			(0x02)
#define mcf5206e_PP_PPDDR_DDR0			(0x01)

#define mcf5206e_PP_PPDDR_DDR7_INPUT		(~0x80)
#define mcf5206e_PP_PPDDR_DDR7_OUTPUT	( 0x80)
#define mcf5206e_PP_PPDDR_DDR6_INPUT		(~0x40)
#define mcf5206e_PP_PPDDR_DDR6_OUTPUT	( 0x40)
#define mcf5206e_PP_PPDDR_DDR5_INPUT		(~0x20)
#define mcf5206e_PP_PPDDR_DDR5_OUTPUT	( 0x20)
#define mcf5206e_PP_PPDDR_DDR4_INPUT		(~0x10)
#define mcf5206e_PP_PPDDR_DDR4_OUTPUT	( 0x10)
#define mcf5206e_PP_PPDDR_DDR3_INPUT		(~0x08)
#define mcf5206e_PP_PPDDR_DDR3_OUTPUT	( 0x08)
#define mcf5206e_PP_PPDDR_DDR2_INPUT		(~0x04)
#define mcf5206e_PP_PPDDR_DDR2_OUTPUT	( 0x04)
#define mcf5206e_PP_PPDDR_DDR1_INPUT		(~0x02)
#define mcf5206e_PP_PPDDR_DDR1_OUTPUT	( 0x02)
#define mcf5206e_PP_PPDDR_DDR0_INPUT		(~0x01)
#define mcf5206e_PP_PPDDR_DDR0_OUTPUT	( 0x01)

#define mcf5206e_PP_PPDAT_DAT7			(0x80)
#define mcf5206e_PP_PPDAT_DAT6			(0x40)
#define mcf5206e_PP_PPDAT_DAT5			(0x20)
#define mcf5206e_PP_PPDAT_DAT4			(0x10)
#define mcf5206e_PP_PPDAT_DAT3			(0x08)
#define mcf5206e_PP_PPDAT_DAT2			(0x04)
#define mcf5206e_PP_PPDAT_DAT1			(0x02)
#define mcf5206e_PP_PPDAT_DAT0			(0x01)

/***********************************************************************/

/*
 * DRAM Controller Module, DRAMC
 */

/* Offsets of the registers from the MBAR */
#define mcf5206e_DRAMC_DCRR		(0x0046)
#define mcf5206e_DRAMC_DCTR		(0x004A)
#define mcf5206e_DRAMC_DCAR0		(0x004C)
#define mcf5206e_DRAMC_DCMR0		(0x0050)
#define mcf5206e_DRAMC_DCCR0		(0x0057)
#define mcf5206e_DRAMC_DCAR1		(0x0058)
#define mcf5206e_DRAMC_DCMR1		(0x005C)
#define mcf5206e_DRAMC_DCCR1		(0x0063)

/* Read access macros for general use */
#define mcf5206e_RD_DRAMC_DCRR(IMMP)		\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCRR,16)
#define mcf5206e_RD_DRAMC_DCTR(IMMP)		\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCTR,16)
#define mcf5206e_RD_DRAMC_DCAR0(IMMP)	\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCAR0,16)
#define mcf5206e_RD_DRAMC_DCMR0(IMMP)	\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCMR0,32)
#define mcf5206e_RD_DRAMC_DCCR0(IMMP)	\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCCR0,8)
#define mcf5206e_RD_DRAMC_DCAR1(IMMP)	\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCAR1,16)
#define mcf5206e_RD_DRAMC_DCMR1(IMMP)	\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCMR1,32)
#define mcf5206e_RD_DRAMC_DCCR1(IMMP)	\
	mcf5206e_iord(IMMP,mcf5206e_DRAMC_DCCR1,8)

/* Write access macros for general use */
#define mcf5206e_WR_DRAMC_DCRR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCRR,16,DATA)
#define mcf5206e_WR_DRAMC_DCTR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCTR,16,DATA)
#define mcf5206e_WR_DRAMC_DCAR0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCAR0,16,DATA)
#define mcf5206e_WR_DRAMC_DCMR0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCMR0,32,DATA)
#define mcf5206e_WR_DRAMC_DCCR0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCCR0,8,DATA)
#define mcf5206e_WR_DRAMC_DCAR1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCAR1,16,DATA)
#define mcf5206e_WR_DRAMC_DCMR1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCMR1,32,DATA)
#define mcf5206e_WR_DRAMC_DCCR1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_DRAMC_DCCR1,8,DATA)

#if 1
typedef volatile struct
{
	uint16	reserved1[0x23];
	uint16	DCRR;
	uint16	reserved2;
	uint16	DCTR;
	uint16	DCAR0;
	uint16	reserved4;
	uint32	DCMR0;
	uint8	reserved5[3];
	uint8	DCCR0;
	uint16	DCAR1;
	uint16	reserved6;
	uint32	DCMR1;
	uint8	reserved7[3];
	uint8	DCCR1;
} mcf5206e_DRAMC;
#endif

#define mcf5206e_DRAMC_DCRR_RC(a)	((a)&0x0FFF)

#define mcf5206e_DRAMC_DCTR_DAEM	(0x8000)
#define mcf5206e_DRAMC_DCTR_EDO		(0x4000)
#define mcf5206e_DRAMC_DCTR_RCD		(0x1000)
#define mcf5206e_DRAMC_DCTR_RSH_1	(0x0000)
#define mcf5206e_DRAMC_DCTR_RSH_2	(0x0200)
#define mcf5206e_DRAMC_DCTR_RSH_3	(0x0400)
#define mcf5206e_DRAMC_DCTR_CRP_15	(0x0000)
#define mcf5206e_DRAMC_DCTR_CRP_25	(0x0020)
#define mcf5206e_DRAMC_DCTR_CRP_35	(0x0040)
#define mcf5206e_DRAMC_DCTR_CAS		(0x0008)
#define mcf5206e_DRAMC_DCTR_CP		(0x0002)
#define mcf5206e_DRAMC_DCTR_CSR		(0x0001)

#define mcf5206e_DRAMC_DCAR_BASE(a)	(((a)&0xFFFE0000)>>16)

#define mcf5206e_DRAMC_DCMR_MASK_128M	(0x07FE0000)
#define mcf5206e_DRAMC_DCMR_MASK_64M	(0x03FE0000)
#define mcf5206e_DRAMC_DCMR_MASK_32M	(0x01FE0000)
#define mcf5206e_DRAMC_DCMR_MASK_16M	(0x00FE0000)
#define mcf5206e_DRAMC_DCMR_MASK_8M		(0x007E0000)
#define mcf5206e_DRAMC_DCMR_MASK_4M		(0x003E0000)
#define mcf5206e_DRAMC_DCMR_MASK_2M		(0x001E0000)
#define mcf5206e_DRAMC_DCMR_MASK_1M		(0x000E0000)
#define mcf5206e_DRAMC_DCMR_MASK_512K	(0x00060000)
#define mcf5206e_DRAMC_DCMR_MASK_256K	(0x00020000)
#define mcf5206e_DRAMC_DCMR_MASK_128K	(0x00000000)
#define mcf5206e_DRAMC_DCMR_SC			(0x00000010)
#define mcf5206e_DRAMC_DCMR_SD			(0x00000008)
#define mcf5206e_DRAMC_DCMR_UC			(0x00000004)
#define mcf5206e_DRAMC_DCMR_UD			(0x00000002)

#define mcf5206e_DRAMC_DCCR_PS_32		(0x00)
#define mcf5206e_DRAMC_DCCR_PS_16		(0x80)
#define mcf5206e_DRAMC_DCCR_PS_8		(0x40)
#define mcf5206e_DRAMC_DCCR_PS_MASK		(0xC0)
#define mcf5206e_DRAMC_DCCR_BPS_512B	(0x00)
#define mcf5206e_DRAMC_DCCR_BPS_1K		(0x10)
#define mcf5206e_DRAMC_DCCR_BPS_2K		(0x20)
#define mcf5206e_DRAMC_DCCR_PM_NORMAL	(0x00)
#define mcf5206e_DRAMC_DCCR_PM_BURSTPAGE (0x04)
#define mcf5206e_DRAMC_DCCR_PM_FASTPAGE	(0x0C)
#define mcf5206e_DRAMC_DCCR_WR			(0x02)
#define mcf5206e_DRAMC_DCCR_RD			(0x01)

/***********************************************************************/

/*
 * UART Module, UART
 */

#define mcf5206e_UART1_UMR		(0x0140)
#define mcf5206e_UART1_USR		(0x0144)
#define mcf5206e_UART1_UCSR		(0x0144)
#define mcf5206e_UART1_UCR		(0x0148)
#define mcf5206e_UART1_URB		(0x014C)
#define mcf5206e_UART1_UTB		(0x014C)
#define mcf5206e_UART1_UIPCR	(0x0150)
#define mcf5206e_UART1_UACR		(0x0150)
#define mcf5206e_UART1_UISR		(0x0154)
#define mcf5206e_UART1_UIMR		(0x0154)
#define mcf5206e_UART1_UBG1		(0x0158)
#define mcf5206e_UART1_UBG2		(0x015C)
#define mcf5206e_UART1_UIVR		(0x0170)
#define mcf5206e_UART1_UIP		(0x0174)
#define mcf5206e_UART1_UOP1		(0x0178)
#define mcf5206e_UART1_UOP0		(0x017C)

#define mcf5206e_UART2_UMR		(0x0180)
#define mcf5206e_UART2_USR		(0x0184)
#define mcf5206e_UART2_UCSR		(0x0184)
#define mcf5206e_UART2_UCR		(0x0188)
#define mcf5206e_UART2_URB		(0x018C)
#define mcf5206e_UART2_UTB		(0x018C)
#define mcf5206e_UART2_UIPCR	(0x0190)
#define mcf5206e_UART2_UACR		(0x0190)
#define mcf5206e_UART2_UISR		(0x0194)
#define mcf5206e_UART2_UIMR		(0x0194)
#define mcf5206e_UART2_UBG1		(0x0198)
#define mcf5206e_UART2_UBG2		(0x019C)
#define mcf5206e_UART2_UIVR		(0x01B0)
#define mcf5206e_UART2_UIP		(0x01B4)
#define mcf5206e_UART2_UOP1		(0x01B8)
#define mcf5206e_UART2_UOP0		(0x01BC)

/* Read access macros for general use */
#define mcf5206e_RD_UART1_UMR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART1_UMR,8)
#define mcf5206e_RD_UART1_USR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART1_USR,8)
#define mcf5206e_RD_UART1_URB(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART1_URB,8)
#define mcf5206e_RD_UART1_UIPCR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART1_UIPCR,8)
#define mcf5206e_RD_UART1_UISR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART1_UISR,8)
#define mcf5206e_RD_UART1_UBG1(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART1_UBG1,8)
#define mcf5206e_RD_UART1_UBG2(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART1_UBG2,8)
#define mcf5206e_RD_UART1_UIVR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART1_UIVR,8)
#define mcf5206e_RD_UART1_UIP(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART1_UIP,8)

#define mcf5206e_RD_UART2_UMR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART2_UMR,8)
#define mcf5206e_RD_UART2_USR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART2_USR,8)
#define mcf5206e_RD_UART2_URB(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART2_URB,8)
#define mcf5206e_RD_UART2_UIPCR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART2_UIPCR,8)
#define mcf5206e_RD_UART2_UISR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART2_UISR,8)
#define mcf5206e_RD_UART2_UBG1(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART2_UBG1,8)
#define mcf5206e_RD_UART2_UBG2(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART2_UBG2,8)
#define mcf5206e_RD_UART2_UIVR(IMMP)	mcf5206e_iord(IMMP,mcf5206e_UART2_UIVR,8)
#define mcf5206e_RD_UART2_UIP(IMMP)		mcf5206e_iord(IMMP,mcf5206e_UART2_UIP,8)

/* Write access macros for general use */
#define mcf5206e_WR_UART1_UMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UMR,8,DATA)
#define mcf5206e_WR_UART1_UCSR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UCSR,8,DATA)
#define mcf5206e_WR_UART1_UCR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UCR,8,DATA)
#define mcf5206e_WR_UART1_UTB(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UTB,8,DATA)
#define mcf5206e_WR_UART1_UACR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UACR,8,DATA)
#define mcf5206e_WR_UART1_UIMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UIMR,8,DATA)
#define mcf5206e_WR_UART1_UBG1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UBG1,8,DATA)
#define mcf5206e_WR_UART1_UBG2(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UBG2,8,DATA)
#define mcf5206e_WR_UART1_UIVR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UIVR,8,DATA)
#define mcf5206e_WR_UART1_UOP1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UOP1,8,DATA)
#define mcf5206e_WR_UART1_UOP0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART1_UOP0,8,DATA)

#define mcf5206e_WR_UART2_UMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UMR,8,DATA)
#define mcf5206e_WR_UART2_UCSR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UCSR,8,DATA)
#define mcf5206e_WR_UART2_UCR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UCR,8,DATA)
#define mcf5206e_WR_UART2_UTB(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UTB,8,DATA)
#define mcf5206e_WR_UART2_UACR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UACR,8,DATA)
#define mcf5206e_WR_UART2_UIMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UIMR,8,DATA)
#define mcf5206e_WR_UART2_UBG1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UBG1,8,DATA)
#define mcf5206e_WR_UART2_UBG2(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UBG2,8,DATA)
#define mcf5206e_WR_UART2_UIVR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UIVR,8,DATA)
#define mcf5206e_WR_UART2_UOP1(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UOP1,8,DATA)
#define mcf5206e_WR_UART2_UOP0(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_UART2_UOP0,8,DATA)

#if 1
typedef volatile struct
{
	uint8	reserved1[0x140];
	uint8	UMR;
	uint8	reserved2;
	uint8	reserved3;
	uint8	reserved4;
	uint8	USR;
	uint8	reserved5;
	uint8	reserved6;
	uint8	reserved7;
	uint8	UCR;
	uint8	reserved8;
	uint8	reserved9;
	uint8	reserved10;
	uint8	UBUF;
	uint8	reserved11;
	uint8	reserved12;
	uint8	reserved13;
	uint8	UACR;
	uint8	reserved14;
	uint8	reserved15;
	uint8	reserved16;
	uint8	UIR;
	uint8	reserved17;
	uint8	reserved18;
	uint8	reserved19;
	uint8	UBG1;
	uint8	reserved20;
	uint8	reserved21;
	uint8	reserved22;
	uint8	UBG2;
	uint32	reserved23[4];
	uint8	UIVR;
	uint8	reserved24;
	uint8	reserved25;
	uint8	reserved26;
	uint8	UIP;
	uint8	reserved27;
	uint8	reserved28;
	uint8	reserved29;
	uint8	UOP1;
	uint8	reserved30;
	uint8	reserved31;
	uint8	reserved32;
	uint8	UOP0;
} mcf5206e_UART1;

typedef volatile struct
{
	uint8	reserved1[0x180];
	uint8	UMR;
	uint8	reserved2;
	uint8	reserved3;
	uint8	reserved4;
	uint8	USR;
	uint8	reserved5;
	uint8	reserved6;
	uint8	reserved7;
	uint8	UCR;
	uint8	reserved8;
	uint8	reserved9;
	uint8	reserved10;
	uint8	UBUF;
	uint8	reserved11;
	uint8	reserved12;
	uint8	reserved13;
	uint8	UACR;
	uint8	reserved14;
	uint8	reserved15;
	uint8	reserved16;
	uint8	UIR;
	uint8	reserved17;
	uint8	reserved18;
	uint8	reserved19;
	uint8	UBG1;
	uint8	reserved20;
	uint8	reserved21;
	uint8	reserved22;
	uint8	UBG2;
	uint32	reserved23[4];
	uint8	UIVR;
	uint8	reserved24;
	uint8	reserved25;
	uint8	reserved26;
	uint8	UIP;
	uint8	reserved27;
	uint8	reserved28;
	uint8	reserved29;
	uint8	UOP1;
	uint8	reserved30;
	uint8	reserved31;
	uint8	reserved32;
	uint8	UOP0;
} mcf5206e_UART2;
#endif

#define mcf5206e_UART_UMR1_RXRTS			(0x80)
#define mcf5206e_UART_UMR1_RXIRQ			(0x40)
#define mcf5206e_UART_UMR1_ERR				(0x20)
#define mcf5206e_UART_UMR1_PM1				(0x10)
#define mcf5206e_UART_UMR1_PM0				(0x08)
#define mcf5206e_UART_UMR1_PMT				(0x04)
#define mcf5206e_UART_UMR1_BC1				(0x02)
#define mcf5206e_UART_UMR1_BC0				(0x01)
#define mcf5206e_UART_UMR1_PM_MULTI_ADDR	(0x1C)
#define mcf5206e_UART_UMR1_PM_MULTI_DATA	(0x18)
#define mcf5206e_UART_UMR1_PM_NONE			(0x10)
#define mcf5206e_UART_UMR1_PM_FORCE_HI		(0x0C)
#define mcf5206e_UART_UMR1_PM_FORCE_LO		(0x08)
#define mcf5206e_UART_UMR1_PM_ODD			(0x04)
#define mcf5206e_UART_UMR1_PM_EVEN			(0x00)
#define mcf5206e_UART_UMR1_BC_5				(0x00)
#define mcf5206e_UART_UMR1_BC_6				(0x01)
#define mcf5206e_UART_UMR1_BC_7				(0x02)
#define mcf5206e_UART_UMR1_BC_8				(0x03)

#define mcf5206e_UART_UMR2_CM1				(0x80)
#define mcf5206e_UART_UMR2_CM0				(0x40)
#define mcf5206e_UART_UMR2_TXRTS			(0x20)
#define mcf5206e_UART_UMR2_TXCTS			(0x10)
#define mcf5206e_UART_UMR2_SB3				(0x08)
#define mcf5206e_UART_UMR2_SB2				(0x04)
#define mcf5206e_UART_UMR2_SB1				(0x02)
#define mcf5206e_UART_UMR2_SB0				(0x01)
#define mcf5206e_UART_UMR2_STOP_BITS(a)     ((a)&0x0f)      /* Stop Bit Length      */
#define mcf5206e_UART_UMR2_CM_NORMAL		(0x00)
#define mcf5206e_UART_UMR2_CM_ECHO			(0x40)
#define mcf5206e_UART_UMR2_CM_LOCAL_LOOP	(0x80)
#define mcf5206e_UART_UMR2_CM_REMOTE_LOOP	(0xC0)
#define mcf5206e_UART_UMR2_STOP_BITS_1		(0x07)
#define mcf5206e_UART_UMR2_STOP_BITS_15		(0x08)
#define mcf5206e_UART_UMR2_STOP_BITS_2		(0x0F)

#define mcf5206e_UART_USR_RB				(0x80)
#define mcf5206e_UART_USR_FE				(0x40)
#define mcf5206e_UART_USR_PE				(0x20)
#define mcf5206e_UART_USR_OE				(0x10)
#define mcf5206e_UART_USR_TXEMP				(0x08)
#define mcf5206e_UART_USR_TXRDY				(0x04)
#define mcf5206e_UART_USR_FFULL				(0x02)
#define mcf5206e_UART_USR_RXRDY				(0x01)

#define mcf5206e_UART_UCSR_RCS3				(0x80)
#define mcf5206e_UART_UCSR_RCS2				(0x40)
#define mcf5206e_UART_UCSR_RCS1				(0x20)
#define mcf5206e_UART_UCSR_RCS0				(0x10)
#define mcf5206e_UART_UCSR_TCS3				(0x08)
#define mcf5206e_UART_UCSR_TCS2				(0x04)
#define mcf5206e_UART_UCSR_TCS1				(0x02)
#define mcf5206e_UART_UCSR_TCS0				(0x01)
#define mcf5206e_UART_UCSR_RX_TIMER			(0xD0)
#define mcf5206e_UART_UCSR_RX_16EXT			(0xE0)
#define mcf5206e_UART_UCSR_RX_1EXT			(0xF0)
#define mcf5206e_UART_UCSR_TX_TIMER			(0x0D)
#define mcf5206e_UART_UCSR_TX_16EXT			(0x0E)
#define mcf5206e_UART_UCSR_TX_1EXT			(0x0F)
#define mcf5206e_UART_UCSR_RCS(a)			(((a)&0x0f)<<4)
#define mcf5206e_UART_UCSR_TCS(a)			((a)&0x0f)



#define mcf5206e_UART_UCR_MISC2				(0x40)
#define mcf5206e_UART_UCR_MISC1				(0x20)
#define mcf5206e_UART_UCR_MISC0				(0x10)
#define mcf5206e_UART_UCR_TC1				(0x08)
#define mcf5206e_UART_UCR_TC0				(0x04)
#define mcf5206e_UART_UCR_RC1				(0x02)
#define mcf5206e_UART_UCR_RC0				(0x01)
#define mcf5206e_UART_UCR_NONE				(0x00)
#define mcf5206e_UART_UCR_STOP_BREAK		(0x70)
#define mcf5206e_UART_UCR_START_BREAK		(0x60)
#define mcf5206e_UART_UCR_RESET_BKCHGINT	(0x50)
#define mcf5206e_UART_UCR_RESET_ERROR		(0x40)
#define mcf5206e_UART_UCR_RESET_TX			(0x30)
#define mcf5206e_UART_UCR_RESET_RX			(0x20)
#define mcf5206e_UART_UCR_RESET_MR			(0x10)
#define mcf5206e_UART_UCR_TX_DISABLED		(0x08)
#define mcf5206e_UART_UCR_TX_ENABLED		(0x04)
#define mcf5206e_UART_UCR_RX_DISABLED		(0x02)
#define mcf5206e_UART_UCR_RX_ENABLED		(0x01)

#define mcf5206e_UART_UIPCR_COS				(0x10)
#define mcf5206e_UART_UIPCR_CTS				(0x01)

#define mcf5206e_UART_UACR_BRG				(0x80)
#define mcf5206e_UART_UACR_CTMS_TIMER		(0x60)
#define mcf5206e_UART_UACR_IEC				(0x01)

#define mcf5206e_UART_UISR_COS				(0x80)
#define mcf5206e_UART_UISR_DB				(0x04)
#define mcf5206e_UART_UISR_RXRDY			(0x02)
#define mcf5206e_UART_UISR_TXRDY			(0x01)

#define mcf5206e_UART_UIMR_COS				(0x80)
#define mcf5206e_UART_UIMR_DB				(0x04)
#define mcf5206e_UART_UIMR_FFULL			(0x02)
#define mcf5206e_UART_UIMR_TXRDY			(0x01)

#define mcf5206e_UART_UIP_CTS				(0x01)

#define mcf5206e_UART_UOP1_RTS				(0x01)

#define mcf5206e_UART_UOP0_RTS				(0x01)

/***********************************************************************/

/*
 * M-Bus Module, MBUS
 */

#define mcf5206e_MBUS_MADR		(0x01E0)
#define mcf5206e_MBUS_MFDR		(0x01E4)
#define mcf5206e_MBUS_MBCR		(0x01E8)
#define mcf5206e_MBUS_MBSR		(0x01EC)
#define mcf5206e_MBUS_MBDR		(0x01F0)

/* Read access macros for general use */
#define mcf5206e_RD_MBUS_MADR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_MBUS_MADR,8)
#define mcf5206e_RD_MBUS_MFDR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_MBUS_MFDR,8)
#define mcf5206e_RD_MBUS_MBCR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_MBUS_MBCR,8)
#define mcf5206e_RD_MBUS_MBSR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_MBUS_MBSR,8)
#define mcf5206e_RD_MBUS_MBDR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_MBUS_MBDR,8)

/* Write access macros for general use */
#define mcf5206e_WR_MBUS_MADR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_MBUS_MADR,8,DATA)
#define mcf5206e_WR_MBUS_MFDR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_MBUS_MFDR,8,DATA)
#define mcf5206e_WR_MBUS_MBCR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_MBUS_MBCR,8,DATA)
#define mcf5206e_WR_MBUS_MBSR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_MBUS_MBSR,8,DATA)
#define mcf5206e_WR_MBUS_MBDR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_MBUS_MBDR,8,DATA)

#if 1
typedef volatile struct
{
	uint8	reserved1[0x1E0];
	uint8	MADR;
	uint8	reserved2;
	uint8	reserved3;
	uint8	reserved4;
	uint8	MFDR;
	uint8	reserved5;
	uint8	reserved6;
	uint8	reserved7;
	uint8	MBCR;
	uint8	reserved8;
	uint8	reserved9;
	uint8	reserved10;
	uint8	MBSR;
	uint8	reserved11;
	uint8	reserved12;
	uint8	reserved13;
	uint8	MBDR;
} mcf5206e_MBUS;
#endif

#define mcf5206e_MBUS_MADR_ADDR(a)	((a)&0xFE)

#define mcf5206e_MBUS_MFDR_MBC(a)	((a)&0x3F)

#define mcf5206e_MBUS_MBCR_MEN		(0x80)
#define mcf5206e_MBUS_MBCR_MIEN		(0x40)
#define mcf5206e_MBUS_MBCR_MSTA		(0x20)
#define mcf5206e_MBUS_MBCR_MTX		(0x10)
#define mcf5206e_MBUS_MBCR_TXAK		(0x08)
#define mcf5206e_MBUS_MBCR_RSTA		(0x04)

#define mcf5206e_MBUS_MBSR_mcf		(0x80)
#define mcf5206e_MBUS_MBSR_MAAS		(0x40)
#define mcf5206e_MBUS_MBSR_MBB		(0x20)
#define mcf5206e_MBUS_MBSR_MAL		(0x10)
#define mcf5206e_MBUS_MBSR_SRW		(0x04)
#define mcf5206e_MBUS_MBSR_MIF		(0x02)
#define mcf5206e_MBUS_MBSR_RXAK		(0x01)

/***********************************************************************/

/*
 * Timer Module, TIMER
 */

#define mcf5206e_TIMER1_TMR		(0x0100)
#define mcf5206e_TIMER1_TRR		(0x0104)
#define mcf5206e_TIMER1_TCR		(0x0108)
#define mcf5206e_TIMER1_TCN		(0x010C)
#define mcf5206e_TIMER1_TER		(0x0111)

#define mcf5206e_TIMER2_TMR		(0x0120)
#define mcf5206e_TIMER2_TRR		(0x0124)
#define mcf5206e_TIMER2_TCR		(0x0128)
#define mcf5206e_TIMER2_TCN		(0x012C)
#define mcf5206e_TIMER2_TER		(0x0131)

/* Read access macros for general use */
#define mcf5206e_RD_TIMER1_TMR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER1_TMR,16)
#define mcf5206e_RD_TIMER1_TRR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER1_TRR,16)
#define mcf5206e_RD_TIMER1_TCR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER1_TCR,16)
#define mcf5206e_RD_TIMER1_TCN(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER1_TCN,16)
#define mcf5206e_RD_TIMER1_TER(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER1_TER,8)

#define mcf5206e_RD_TIMER2_TMR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER2_TMR,16)
#define mcf5206e_RD_TIMER2_TRR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER2_TRR,16)
#define mcf5206e_RD_TIMER2_TCR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER2_TCR,16)
#define mcf5206e_RD_TIMER2_TCN(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER2_TCN,16)
#define mcf5206e_RD_TIMER2_TER(IMMP)		mcf5206e_iord(IMMP,mcf5206e_TIMER2_TER,8)

/* Write access macros for general use */
#define mcf5206e_WR_TIMER1_TMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER1_TMR,16,DATA)
#define mcf5206e_WR_TIMER1_TRR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER1_TRR,16,DATA)
#define mcf5206e_WR_TIMER1_TCN(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER1_TCN,16,DATA)
#define mcf5206e_WR_TIMER1_TER(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER1_TER,8,DATA)

#define mcf5206e_WR_TIMER2_TMR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER2_TMR,16,DATA)
#define mcf5206e_WR_TIMER2_TRR(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER2_TRR,16,DATA)
#define mcf5206e_WR_TIMER2_TCN(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER2_TCN,16,DATA)
#define mcf5206e_WR_TIMER2_TER(IMMP,DATA)	\
	mcf5206e_iowr(IMMP,mcf5206e_TIMER2_TER,8,DATA)

#if 1
typedef volatile struct
{
	uint8	reserved0[0x100];
	uint16	TMR1;
	uint16	reserved1;
	uint16	TRR1;
	uint16	reserved2;
	uint16	TCR1;
	uint16	reserved3;
	uint16	TCN1;
	uint16	reserved4;
	uint8	reserved5;
	uint8	TER1;
	uint32	reserved6;
	uint32	reserved7;
	uint32	reserved8;
	uint16	TMR2;
	uint16	reserved9;
	uint16	TRR2;
	uint16	reserved10;
	uint16	TCR2;
	uint16	reserved11;
	uint16	TCN2;
	uint16	reserved12;
	uint8	reserved13;
	uint8	TER2;
} mcf5206e_TIMER;
#endif

#define mcf5206e_TIMER_TMR_PS(a)	(((a)&0x00FF)<<8)
#define mcf5206e_TIMER_TMR_CE_ANY	(0x00C0)
#define mcf5206e_TIMER_TMR_CE_RISE	(0x0080)
#define mcf5206e_TIMER_TMR_CE_FALL	(0x0040)
#define mcf5206e_TIMER_TMR_CE_NONE	(0x0000)
#define mcf5206e_TIMER_TMR_OM		(0x0020)
#define mcf5206e_TIMER_TMR_ORI		(0x0010)
#define mcf5206e_TIMER_TMR_FRR		(0x0008)
#define mcf5206e_TIMER_TMR_CLK_TIN	(0x0006)
#define mcf5206e_TIMER_TMR_CLK_DIV16	(0x0004)
#define mcf5206e_TIMER_TMR_CLK_MSCLK	(0x0002)
#define mcf5206e_TIMER_TMR_CLK_STOP	(0x0000)
#define mcf5206e_TIMER_TMR_RST		(0x0001)

#define mcf5206e_TIMER_TER_REF		(0x02)
#define mcf5206e_TIMER_TER_CAP		(0x01)

/***********************************************************************/

/*
 * DMA Module, DMA
 */

#define mcf5206e_DMA0_SAR		(0x0200)
#define mcf5206e_DMA0_DAR		(0x0204)
#define mcf5206e_DMA0_DCR		(0x0208)
#define mcf5206e_DMA0_BCR		(0x020C)
#define mcf5206e_DMA0_DSR		(0x0210)
#define mcf5206e_DMA0_DIVR		(0x0214)

#define mcf5206e_DMA1_SAR		(0x0240)
#define mcf5206e_DMA1_DAR		(0x0244)
#define mcf5206e_DMA1_DCR		(0x024C)
#define mcf5206e_DMA1_BCR		(0x0250)
#define mcf5206e_DMA1_DSR		(0x0250)
#define mcf5206e_DMA1_DIVR		(0x0254)

/* Read access macros for general use */
#define mcf5206e_RD_DMA0_SAR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA0_SAR,32)
#define mcf5206e_RD_DMA0_DAR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA0_DAR,32)
#define mcf5206e_RD_DMA0_DCR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA0_DCR,16)
#define mcf5206e_RD_DMA0_BCR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA0_BCR,16)
#define mcf5206e_RD_DMA0_DSR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA0_DSR,8)
#define mcf5206e_RD_DMA0_DIVR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA0_DIVR,8)

#define mcf5206e_RD_DMA1_SAR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA1_SAR,32)
#define mcf5206e_RD_DMA1_DAR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA1_DAR,32)
#define mcf5206e_RD_DMA1_DCR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA1_DCR,16)
#define mcf5206e_RD_DMA1_BCR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA1_BCR,16)
#define mcf5206e_RD_DMA1_DSR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA1_DSR,8)
#define mcf5206e_RD_DMA1_DIVR(IMMP)		mcf5206e_iord(IMMP,mcf5206e_DMA1_DIVR,8)

/* Write access macros for general use */
#define mcf5206e_WR_DMA0_SAR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA0_SAR,32,DATA)
#define mcf5206e_WR_DMA0_DAR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA0_DAR,32,DATA)
#define mcf5206e_WR_DMA0_DCR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA0_DCR,16,DATA)
#define mcf5206e_WR_DMA0_BCR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA0_BCR,16,DATA)
#define mcf5206e_WR_DMA0_DSR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA0_DSR,8,DATA)
#define mcf5206e_WR_DMA0_DIVR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA0_DIVR,8,DATA)

#define mcf5206e_WR_DMA1_SAR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA1_SAR,32,DATA)
#define mcf5206e_WR_DMA1_DAR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA1_DAR,32,DATA)
#define mcf5206e_WR_DMA1_DCR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA1_DCR,16,DATA)
#define mcf5206e_WR_DMA1_BCR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA1_BCR,16,DATA)
#define mcf5206e_WR_DMA1_DSR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA1_DSR,8,DATA)
#define mcf5206e_WR_DMA1_DIVR(IMMP,DATA)	mcf5206e_iowr(IMMP,mcf5206e_DMA1_DIVR,8,DATA)

#if 1
typedef volatile struct
{
	uint8	reserved1[0x200];
	uint32	SAR0;
	uint32	DAR0;
	uint16	DCR0;
	uint16	reserved2;
	uint16	BCR0;
	uint16	reserved3;
	uint8	DSR0;
	uint8	reserved4;
	uint16	reserved5;
	uint8	DIVR0;
	uint8	reserved6;
	uint16	reserved7;
	uint32	reserved8[0xa];
	uint32	SAR1;
	uint32	DAR1;
	uint16	DCR1;
	uint16	reserved9;
	uint16	BCR1;
	uint16	reserved10;
	uint8	DSR1;
	uint8	reserved11;
	uint16	reserved12;
	uint8	DIVR1;
} mcf5206e_DMA;
#endif

#define mcf5206e_DMA_DCR_INT             (0x8000) /* Interrupt on Completion     */
#define mcf5206e_DMA_DCR_EEXT            (0x4000) /* Enable External Request     */
#define mcf5206e_DMA_DCR_CS              (0x2000) /* Cycle Steal                 */
#define mcf5206e_DMA_DCR_AA              (0x1000) /* Auto Align                  */
#define mcf5206e_DMA_DCR_BWC_DMA         (0x0000) /* Bandwidth: DMA Priority     */
#define mcf5206e_DMA_DCR_BWC_512         (0x0200) /* Bandwidth:   512 Bytes      */
#define mcf5206e_DMA_DCR_BWC_1024        (0x0400) /* Bandwidth:  1024 Bytes      */
#define mcf5206e_DMA_DCR_BWC_2048        (0x0600) /* Bandwidth:  2048 Bytes      */
#define mcf5206e_DMA_DCR_BWC_4096        (0x0800) /* Bandwidth:  4096 Bytes      */
#define mcf5206e_DMA_DCR_BWC_8192        (0x0a00) /* Bandwidth:  8192 Bytes      */
#define mcf5206e_DMA_DCR_BWC_16384       (0x0c00) /* Bandwidth: 16384 Bytes      */
#define mcf5206e_DMA_DCR_BWC_32768       (0x0e00) /* Bandwidth: 32768 Bytes      */
#define mcf5206e_DMA_DCR_SAA             (0x0100) /* Single Address Access       */
#define mcf5206e_DMA_DCR_SRW             (0x0080) /* Forces MRW Signal High      */
#define mcf5206e_DMA_DCR_SINC            (0x0040) /* Source Increment            */
#define mcf5206e_DMA_DCR_SSIZE_LONG      (0x0000) /* Source Size:  Longword      */
#define mcf5206e_DMA_DCR_SSIZE_BYTE      (0x0010) /* Source Size:  Byte          */
#define mcf5206e_DMA_DCR_SSIZE_WORD      (0x0020) /* Source Size:  Word          */
#define mcf5206e_DMA_DCR_SSIZE_LINE      (0x0030) /* Source Size:  Line          */
#define mcf5206e_DMA_DCR_DINC            (0x0008) /* Destination Increment       */
#define mcf5206e_DMA_DCR_DSIZE_LONG      (0x0000) /* Destination Size:  Longword */
#define mcf5206e_DMA_DCR_DSIZE_BYTE      (0x0002) /* Destination Size:  Byte     */
#define mcf5206e_DMA_DCR_DSIZE_WORD      (0x0004) /* Destination Size:  Word     */
#define mcf5206e_DMA_DCR_START           (0x0001) /* Start Transfer              */
 
#define mcf5206e_DMA_DSR_CE              (0x40)  /* Configuration Error          */
#define mcf5206e_DMA_DSR_BES             (0x20)  /* Bus Error on Source          */
#define mcf5206e_DMA_DSR_BED             (0x10)  /* Bus Error on Destination     */
#define mcf5206e_DMA_DSR_REQ             (0x04)  /* Request                      */
#define mcf5206e_DMA_DSR_BSY             (0x02)  /* Busy                         */
#define mcf5206e_DMA_DSR_DONE            (0x01)  /* Transaction Done             */


/***********************************************************************/

#if 1
/*
 * Here we put the modules together.  An example access for the UART mode
 * register would be: (assuming we have a pointer to the IMM):
 *
 *  imm->uart1.UMR
 */
typedef volatile union
{
	mcf5206e_SIM	sim;
	mcf5206e_PP		pp;
	mcf5206e_UART1	uart1;
	mcf5206e_UART2	uart2;
	mcf5206e_TIMER	timer;
	mcf5206e_CS		cs;
	mcf5206e_DRAMC	dramc;
	mcf5206e_MBUS	mbus;
	mcf5206e_DMA	dma;
} mcf5206e_IMM;
#endif

/***********************************************************************/

#endif	/* _CPU_mcf5206e_H */
