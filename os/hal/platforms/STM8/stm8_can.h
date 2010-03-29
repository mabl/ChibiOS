#ifndef _STM8_CAN_H_
#define _STM8_CAN_H_

#if CH_HAL_USE_CAN || defined(__DOXYGEN__)

#define CAN_STDID_MASK ((uint16_t)0x07FF)
#define CAN_EXTID_MASK ((uint32_t)0x1FFFFFFF)
#define CAN_DLC_MAX ((uint8_t)0x08)

typedef struct {
  volatile uint8_t          MCR;
  volatile uint8_t          MSR;
  volatile uint8_t          TSR;
  volatile uint8_t          TPR;
  volatile uint8_t          RFR;
  volatile uint8_t          IER;
  volatile uint8_t          DGR;
  volatile uint8_t          PSR;

  /* Paged Registers */
  union {
    struct {
      volatile uint8_t MCSR;
      volatile uint8_t MDLCR;
      volatile uint8_t MIDR1;
      volatile uint8_t MIDR2;
      volatile uint8_t MIDR3;
      volatile uint8_t MIDR4;
      volatile uint8_t MDAR1;
      volatile uint8_t MDAR2;
      volatile uint8_t MDAR3;
      volatile uint8_t MDAR4;
      volatile uint8_t MDAR5;
      volatile uint8_t MDAR6;
      volatile uint8_t MDAR7;
      volatile uint8_t MDAR8;
      volatile uint8_t MTSRL;
      volatile uint8_t MTSRH;
    } TxMailbox;

    struct {
      volatile uint8_t FR01;
      volatile uint8_t FR02;
      volatile uint8_t FR03;
      volatile uint8_t FR04;
      volatile uint8_t FR05;
      volatile uint8_t FR06;
      volatile uint8_t FR07;
      volatile uint8_t FR08;

      volatile uint8_t FR09;
      volatile uint8_t FR10;
      volatile uint8_t FR11;
      volatile uint8_t FR12;
      volatile uint8_t FR13;
      volatile uint8_t FR14;
      volatile uint8_t FR15;
      volatile uint8_t FR16;
    } Filter;

    struct {
      volatile uint8_t F0R1;
      volatile uint8_t F0R2;
      volatile uint8_t F0R3;
      volatile uint8_t F0R4;
      volatile uint8_t F0R5;
      volatile uint8_t F0R6;
      volatile uint8_t F0R7;
      volatile uint8_t F0R8;

      volatile uint8_t F1R1;
      volatile uint8_t F1R2;
      volatile uint8_t F1R3;
      volatile uint8_t F1R4;
      volatile uint8_t F1R5;
      volatile uint8_t F1R6;
      volatile uint8_t F1R7;
      volatile uint8_t F1R8;
    } Filter01;

    struct {
      volatile uint8_t F2R1;
      volatile uint8_t F2R2;
      volatile uint8_t F2R3;
      volatile uint8_t F2R4;
      volatile uint8_t F2R5;
      volatile uint8_t F2R6;
      volatile uint8_t F2R7;
      volatile uint8_t F2R8;

      volatile uint8_t F3R1;
      volatile uint8_t F3R2;
      volatile uint8_t F3R3;
      volatile uint8_t F3R4;
      volatile uint8_t F3R5;
      volatile uint8_t F3R6;
      volatile uint8_t F3R7;
      volatile uint8_t F3R8;
    } Filter23;

    struct {
      volatile uint8_t F4R1;
      volatile uint8_t F4R2;
      volatile uint8_t F4R3;
      volatile uint8_t F4R4;
      volatile uint8_t F4R5;
      volatile uint8_t F4R6;
      volatile uint8_t F4R7;
      volatile uint8_t F4R8;

      volatile uint8_t F5R1;
      volatile uint8_t F5R2;
      volatile uint8_t F5R3;
      volatile uint8_t F5R4;
      volatile uint8_t F5R5;
      volatile uint8_t F5R6;
      volatile uint8_t F5R7;
      volatile uint8_t F5R8;
    } Filter45;

    struct {
      volatile uint8_t ESR;
      volatile uint8_t EIER;
      volatile uint8_t TECR;
      volatile uint8_t RECR;
      volatile uint8_t BTR1;
      volatile uint8_t BTR2;
      uint8_t          reserved1[2];
      volatile uint8_t FMR1;
      volatile uint8_t FMR2;
      volatile uint8_t FCR1;
      volatile uint8_t FCR2;
      volatile uint8_t FCR3;
      uint8_t          reserved2[3];
    } Config;

    struct {
      volatile uint8_t MFMI;
      volatile uint8_t MDLCR;
      volatile uint8_t MIDR1;
      volatile uint8_t MIDR2;
      volatile uint8_t MIDR3;
      volatile uint8_t MIDR4;
      volatile uint8_t MDAR1;
      volatile uint8_t MDAR2;
      volatile uint8_t MDAR3;
      volatile uint8_t MDAR4;
      volatile uint8_t MDAR5;
      volatile uint8_t MDAR6;
      volatile uint8_t MDAR7;
      volatile uint8_t MDAR8;
      volatile uint8_t MTSRL;
      volatile uint8_t MTSRH;
    } RxFIFO;
  } Page;
} STM8CAN_t;

/**
  * @brief CAN Page Mapping
  */
typedef enum {
  STM8CAN_Page_TxMailBox0       = ((uint8_t) 0), /*!< CAN TX mailbox 0 reg page */
  STM8CAN_Page_TxMailBox1       = ((uint8_t) 1), /*!< CAN TX mailbox 1 reg page */
  STM8CAN_Page_TxMailBox2       = ((uint8_t) 5), /*!< CAN TX mailbox 2 reg page */
  STM8CAN_Page_Filter01         = ((uint8_t) 2), /*!< CAN Filters 0 & 1 reg page*/
  STM8CAN_Page_Filter23         = ((uint8_t) 3), /*!< CAN Filters 2 & 3 reg page*/
  STM8CAN_Page_Filter45         = ((uint8_t) 4), /*!< CAN Filters 4 & 5 reg page*/
  STM8CAN_Page_Config           = ((uint8_t) 6), /*!< CAN Configuration control/status reg page*/
  STM8CAN_Page_RxFifo           = ((uint8_t) 7) /*!< CAN RX FIFO registers page */
} STM8CAN_Page_t;

/**
 * @brief CAN Port as a whole
 */
#define CAN             ((STM8CAN_t *)&CAN_MCR)

/* CAN Master Control Register bits */
#define CAN_MCR_INRQ     ((uint8_t)0x01)
#define CAN_MCR_SLEEP    ((uint8_t)0x02)
#define CAN_MCR_TXFP     ((uint8_t)0x04)
#define CAN_MCR_RFLM     ((uint8_t)0x08)
#define CAN_MCR_NART     ((uint8_t)0x10)
#define CAN_MCR_AWUM     ((uint8_t)0x20)
#define CAN_MCR_ABOM     ((uint8_t)0x40)
#define CAN_MCR_TTCM     ((uint8_t)0x80)

/* CAN Master Status Register bits */
#define CAN_MSR_INAK     ((uint8_t)0x01)
#define CAN_MSR_SLAK     ((uint8_t)0x02)
#define CAN_MSR_ERRI     ((uint8_t)0x04)
#define CAN_MSR_WKUI     ((uint8_t)0x08)
#define CAN_MSR_TX       ((uint8_t)0x10)
#define CAN_MSR_RX       ((uint8_t)0x20)

/* CAN Transmit Status Register bits */
#define CAN_TSR_RQCP0    ((uint8_t)0x01)
#define CAN_TSR_RQCP1    ((uint8_t)0x02)
#define CAN_TSR_RQCP2    ((uint8_t)0x04)
#define CAN_TSR_RQCP012  ((uint8_t)0x07)
#define CAN_TSR_TXOK0    ((uint8_t)0x10)
#define CAN_TSR_TXOK1    ((uint8_t)0x20)
#define CAN_TSR_TXOK2    ((uint8_t)0x40)

#define CAN_TPR_CODE0    ((uint8_t)0x01)
#define CAN_TPR_CODE01   ((uint8_t)0x03)
#define CAN_TPR_TME0     ((uint8_t)0x04)
#define CAN_TPR_TME1     ((uint8_t)0x08)
#define CAN_TPR_TME2     ((uint8_t)0x10)
#define CAN_TPR_TME012   ((uint8_t)0x1C)
#define CAN_TPR_LOW0     ((uint8_t)0x20)
#define CAN_TPR_LOW1     ((uint8_t)0x40)
#define CAN_TPR_LOW2     ((uint8_t)0x80)

/* CAN Receive FIFO Register bits */
#define CAN_RFR_FMP01  ((uint8_t)0x03)
#define CAN_RFR_FULL   ((uint8_t)0x08)
#define CAN_RFR_FOVR   ((uint8_t)0x10)
#define CAN_RFR_RFOM   ((uint8_t)0x20)

/* CAN Interrupt Register bits */
#define CAN_IER_TMEIE  ((uint8_t)0x01)
#define CAN_IER_FMPIE  ((uint8_t)0x02)
#define CAN_IER_FFIE   ((uint8_t)0x04)
#define CAN_IER_FOVIE  ((uint8_t)0x08)
#define CAN_IER_WKUIE  ((uint8_t)0x80)

/* CAN diagnostic Register bits */
#define CAN_DGR_LBKM  ((uint8_t)0x01)
#define CAN_DGR_SLIM  ((uint8_t)0x02)
#define CAN_DGR_SAMP  ((uint8_t)0x04)
#define CAN_DGR_RX    ((uint8_t)0x08)
#define CAN_DGR_TXM2E ((uint8_t)0x10)

/* CAN page select Register bits */
#define CAN_PSR_PS0  ((uint8_t)0x01)
#define CAN_PSR_PS1  ((uint8_t)0x02)
#define CAN_PSR_PS2  ((uint8_t)0x04)

/* Tx MailBox & Fifo Page common bits */
#define CAN_MCSR_TXRQ    ((uint8_t)0x01)
#define CAN_MCSR_ABRQ    ((uint8_t)0x02)
#define CAN_MCSR_RQCP    ((uint8_t)0x04)
#define CAN_MCSR_TXOK    ((uint8_t)0x08)
#define CAN_MCSR_ALST    ((uint8_t)0x10)
#define CAN_MCSR_TERR    ((uint8_t)0x20)

#define CAN_MDLCR_DLC    ((uint8_t)0x0F)
#define CAN_MDLCR_TGT    ((uint8_t)0x80)

#define CAN_MIDR1_RTR    ((uint8_t)0x20)
#define CAN_MIDR1_IDE    ((uint8_t)0x40)

/* CAN Error Status Register bits */
#define CAN_ESR_EWGF     ((uint8_t)0x01)
#define CAN_ESR_EPVF     ((uint8_t)0x02)
#define CAN_ESR_BOFF     ((uint8_t)0x04)
#define CAN_ESR_LEC0     ((uint8_t)0x10)
#define CAN_ESR_LEC1     ((uint8_t)0x20)
#define CAN_ESR_LEC2     ((uint8_t)0x40)
#define CAN_ESR_LEC      ((uint8_t)0x70)

/* CAN Error Status Register bits */
#define CAN_EIER_EWGIE    ((uint8_t)0x01)
#define CAN_EIER_EPVIE    ((uint8_t)0x02)
#define CAN_EIER_BOFIE    ((uint8_t)0x04)
#define CAN_EIER_LECIE    ((uint8_t)0x10)
#define CAN_EIER_ERRIE    ((uint8_t)0x80)

/* CAN transmit error counter Register bits(CAN_TECR) */
#define CAN_TECR_TEC0     ((uint8_t)0x01)
#define CAN_TECR_TEC1     ((uint8_t)0x02)
#define CAN_TECR_TEC2     ((uint8_t)0x04)
#define CAN_TECR_TEC3     ((uint8_t)0x08)
#define CAN_TECR_TEC4     ((uint8_t)0x10)
#define CAN_TECR_TEC5     ((uint8_t)0x20)
#define CAN_TECR_TEC6     ((uint8_t)0x40)
#define CAN_TECR_TEC7     ((uint8_t)0x80)

/* CAN receive error counter Register bits(CAN_TECR) */
#define CAN_RECR_REC0     ((uint8_t)0x01)
#define CAN_RECR_REC1     ((uint8_t)0x02)
#define CAN_RECR_REC2     ((uint8_t)0x04)
#define CAN_RECR_REC3     ((uint8_t)0x08)
#define CAN_RECR_REC4     ((uint8_t)0x10)
#define CAN_RECR_REC5     ((uint8_t)0x20)
#define CAN_RECR_REC6     ((uint8_t)0x40)
#define CAN_RECR_REC7     ((uint8_t)0x80)

/* CAN Bit Timing Register 2 bits(CAN_BTR2) */
#define CAN_BTR2_CLKS     ((uint8_t)0x80)

/* CAN filter mode register bits (CAN_FMR) */
#define CAN_FMR1_FML0     ((uint8_t)0x01)
#define CAN_FMR1_FMH0     ((uint8_t)0x02)
#define CAN_FMR1_FML1     ((uint8_t)0x04)
#define CAN_FMR1_FMH1     ((uint8_t)0x08)
#define CAN_FMR1_FML2     ((uint8_t)0x10)
#define CAN_FMR1_FMH2     ((uint8_t)0x20)
#define CAN_FMR1_FML3     ((uint8_t)0x40)
#define CAN_FMR1_FMH3     ((uint8_t)0x80)

#define CAN_FMR2_FML4     ((uint8_t)0x01)
#define CAN_FMR2_FMH4     ((uint8_t)0x02)
#define CAN_FMR2_FML5     ((uint8_t)0x04)
#define CAN_FMR2_FMH5     ((uint8_t)0x08)

/* CAN filter Config register bits (CAN_FCR) */
#define CAN_FCR1_FACT0     ((uint8_t)0x01)
#define CAN_FCR1_FACT1     ((uint8_t)0x10)
#define CAN_FCR2_FACT2     ((uint8_t)0x01)
#define CAN_FCR2_FACT3     ((uint8_t)0x10)
#define CAN_FCR3_FACT4     ((uint8_t)0x01)
#define CAN_FCR3_FACT5     ((uint8_t)0x10)

#define CAN_FCR1_FSC00    ((uint8_t)0x02)
#define CAN_FCR1_FSC01    ((uint8_t)0x04)
#define CAN_FCR1_FSC10    ((uint8_t)0x20)
#define CAN_FCR1_FSC11    ((uint8_t)0x40)
#define CAN_FCR2_FSC20    ((uint8_t)0x02)
#define CAN_FCR2_FSC21    ((uint8_t)0x04)
#define CAN_FCR2_FSC30    ((uint8_t)0x20)
#define CAN_FCR2_FSC31    ((uint8_t)0x40)
#define CAN_FCR3_FSC40    ((uint8_t)0x02)
#define CAN_FCR3_FSC41    ((uint8_t)0x04)
#define CAN_FCR3_FSC50    ((uint8_t)0x20)
#define CAN_FCR3_FSC51    ((uint8_t)0x40)

#endif

#endif
