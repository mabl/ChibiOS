
#ifndef myIOH82368_H
#define myIOH82368_H


/*
 * Bit definitions
*/

#define TSR_TGFA 					0x01
#define TSTR_T1						2
#define TCR_PRESC64 				3
#define TCR_TGRA					0x20
#define TIER_TGRA					0x01

/*
 * Serial Communication Interface
*/
// Serial Control Register (SCR)
#define TIE							7
#define RIE							6
#define TE							5
#define RE							4
#define TEIE						2

// Serial Status Register (SSR)
#define TDRE						7			// Transmit Data Register Empty
#define RDRF						6			// Receive Data Register Full
#define ORER						5			// Overrun Error
#define FER							4			// Framing Error
#define PER							3			// Parity Error
#define TEND						2			// Transmit End

// Serial Mode Register (SMR)
#define PE							5			// Parity Enable
#define O_E_						4			// Parity Mode (1 - odd parity)

// Interrupt Controller
#define ITSR            (*(volatile unsigned short *) (0x00FFFE16))
#define ISCRL           (*(volatile unsigned short *) (0x00FFFE1C))
#define IER             (*(volatile unsigned short *) (0xFFFFFF32))
#define ISR             (*(volatile unsigned short *) (0xFFFFFF34))


// Bus Controller
#define ABWCR          (*(volatile unsigned char *)  (0x00FFFEC0))
#define ASTCR          (*(volatile unsigned char *)  (0x00FFFEC1))
#define WTCRAH         (*(volatile unsigned char *)  (0x00FFFEC2))
#define WTCRAL         (*(volatile unsigned char *)  (0x00FFFEC3))

// I/O Port 2
#define P2DDR          (*(volatile unsigned char *)  (0x00FFFE21))
#define PORT2          (*(volatile unsigned char *)  (0xFFFFFF51))
#define P2DR           (*(volatile unsigned char *)  (0xFFFFFF61))

// I/O Port F
#define PFDDR          (*(volatile unsigned char *)  (0x00FFFE2E))
#define PFCR0          (*(volatile unsigned char *)  (0x00FFFE32))

// TPU1
#define TPU_TCR1       (*(volatile unsigned char *)  (0xFFFFFFE0))
#define TPU_TMDR1      (*(volatile unsigned char *)  (0xFFFFFFE1))
#define TPU_TIOR1      (*(volatile unsigned char *)  (0xFFFFFFE2))
#define TPU_TIER1      (*(volatile unsigned char *)  (0xFFFFFFE4))
#define TPU_TSR1       (*(volatile unsigned char *)  (0xFFFFFFE5))
#define TPU_TCNT1      (*(volatile unsigned short *) (0xFFFFFFE6))
#define TPU_TGRA1      (*(volatile unsigned short *) (0xFFFFFFE8))
#define TPU_TGRB1      (*(volatile unsigned short *) (0xFFFFFFEA))

#define TPU_TSTR       (*(volatile unsigned char *)  (0xFFFFFFC0))


// Watchdog Timer
#define TCSRTCNT       (*(volatile unsigned short *) (0xFFFFFFBC))



// SCI0
#define SCI0_SMR0      (*(volatile unsigned char *)  (0xFFFFFF78))
#define SCI0_BRR0      (*(volatile unsigned char *)  (0xFFFFFF79))
#define SCI0_SCR0      (*(volatile unsigned char *)  (0xFFFFFF7A))
#define SCI0_TDR0      (*(volatile unsigned char *)  (0xFFFFFF7B))
#define SCI0_SSR0      (*(volatile unsigned char *)  (0xFFFFFF7C))
#define SCI0_RDR0      (*(volatile unsigned char *)  (0xFFFFFF7D))

// Interrupt vectors
#define IRQ_7          0x05C

#define TPU_TGI1A      0x0C0

#define SCI_ERI0       0x160
#define SCI_RXI0       0x164
#define SCI_TXI0       0x168


typedef struct {
  volatile unsigned char ddr;
  unsigned char reserved1[303];
  volatile unsigned char in;
  unsigned char reserved2[15];
  volatile unsigned char out;
} h8s_ioport;

typedef struct {
  volatile unsigned char smr;
  volatile unsigned char brr;
  volatile unsigned char scr;
  volatile unsigned char tdr;
  volatile unsigned char ssr;
  volatile unsigned char rdr;
  volatile unsigned char scmr;
} h8s_uart;

#endif

