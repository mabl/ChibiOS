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
 * @file MB96F3xx/serial_lld.h
 * @brief MB96F3xx low level serial driver header.
 * @addtogroup MB96F3xx_SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Serial buffers size.
 * @details Configuration parameter, you can change the depth of the queue
 * buffers depending on the requirements of your application.
 * @note The default is 32 bytes for both the transmission and receive buffers.
 */
#if !defined(SERIAL_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_BUFFERS_SIZE         32
#endif

/**
 * @brief Default bit rate.
 * @details Configuration parameter, at startup the UARTs are configured at
 * this speed.
 */
#if !defined(DEFAULT_USART_BITRATE) || defined(__DOXYGEN__)
#define DEFAULT_USART_BITRATE       38400
#endif

/**
 * @brief USART0 driver enable switch.
 * @details If set to @p TRUE the support for USART0 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART0) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART0           FALSE
#endif

/**
 * @brief USART1 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART1) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART1           FALSE
#endif

/**
 * @brief USART2 driver enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART2) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART2           FALSE
#endif

/**
 * @brief USART3 driver enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART3) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART3           FALSE
#endif

/**
 * @brief USART4 driver enable switch.
 * @details If set to @p TRUE the support for USART4 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART4) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART4           FALSE
#endif

/**
 * @brief USART5 driver enable switch.
 * @details If set to @p TRUE the support for USART5 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART5) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART5           FALSE
#endif

/**
 * @brief USART6 driver enable switch.
 * @details If set to @p TRUE the support for USART6 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART6) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART6           FALSE
#endif

/**
 * @brief USART7 driver enable switch.
 * @details If set to @p TRUE the support for USART7 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART7) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART7           FALSE
#endif

/**
 * @brief USART8 driver enable switch.
 * @details If set to @p TRUE the support for USART8 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART8) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART8           FALSE
#endif

/**
 * @brief USART9 driver enable switch.
 * @details If set to @p TRUE the support for USART9 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_MB96F3xx_USART9) || defined(__DOXYGEN__)
#define USE_MB96F3xx_USART9           FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * Serial Driver condition flags type.
 */
typedef uint8_t sdflags_t;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _SOE :1;
    IO_BYTE _SCKE :1;
    IO_BYTE _UPCL :1;
    IO_BYTE _REST :1;
    IO_BYTE _EXT :1;
    IO_BYTE _OTO :1;
    IO_BYTE _MD0 :1;
    IO_BYTE _MD1 :1;
  }bit;
  struct{
    IO_BYTE :6;
    IO_BYTE _MD :2;
  }bitc;
 }SMRxSTR;


typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TXE :1;
    IO_BYTE _RXE :1;
    IO_BYTE _CRE :1;
    IO_BYTE _AD :1;
    IO_BYTE _CL :1;
    IO_BYTE _SBL :1;
    IO_BYTE _P :1;
    IO_BYTE _PEN :1;
  }bit;
 }SCRxSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TIE :1;
    IO_BYTE _RIE :1;
    IO_BYTE _BDS :1;
    IO_BYTE _TDRE :1;
    IO_BYTE _RDRF :1;
    IO_BYTE _FRE :1;
    IO_BYTE _ORE :1;
    IO_BYTE _PE :1;
  }bit;
 }SSRxSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TBI :1;
    IO_BYTE _RBI :1;
    IO_BYTE _BIE :1;
    IO_BYTE _SSM :1;
    IO_BYTE _SCDE :1;
    IO_BYTE _MS :1;
    IO_BYTE _LBR :1;
    IO_BYTE _INV :1;
  }bit;
 }ECCRxSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _SCES :1;
    IO_BYTE _CCO :1;
    IO_BYTE _SIOP :1;
    IO_BYTE _SOPE :1;
    IO_BYTE _LBL0 :1;
    IO_BYTE _LBL1 :1;
    IO_BYTE _LBD :1;
    IO_BYTE _LBIE :1;
  }bit;
  struct{
    IO_BYTE :4;
    IO_BYTE _LBL :2;
  }bitc;
 }ESCRxSTR;

typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD _BGR0 :1;
    IO_WORD _BGR1 :1;
    IO_WORD _BGR2 :1;
    IO_WORD _BGR3 :1;
    IO_WORD _BGR4 :1;
    IO_WORD _BGR5 :1;
    IO_WORD _BGR6 :1;
    IO_WORD _BGR7 :1;
    IO_WORD _BGR8 :1;
    IO_WORD _BGR9 :1;
    IO_WORD _BGR10 :1;
    IO_WORD _BGR11 :1;
    IO_WORD _BGR12 :1;
    IO_WORD _BGR13 :1;
    IO_WORD _BGR14 :1;
    IO_WORD _BGR15 :1;
  }bit;
  struct{
    IO_WORD _BGR :16;
  }bitc;
 }BGRxSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _BGR0 :1;
    IO_BYTE _BGR1 :1;
    IO_BYTE _BGR2 :1;
    IO_BYTE _BGR3 :1;
    IO_BYTE _BGR4 :1;
    IO_BYTE _BGR5 :1;
    IO_BYTE _BGR6 :1;
    IO_BYTE _BGR7 :1;
  }bit;
 }BGRLxSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _BGR8 :1;
    IO_BYTE _BGR9 :1;
    IO_BYTE _BGR10 :1;
    IO_BYTE _BGR11 :1;
    IO_BYTE _BGR12 :1;
    IO_BYTE _BGR13 :1;
    IO_BYTE _BGR14 :1;
    IO_BYTE _BGR15 :1;
  }bit;
 }BGRHxSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _AICD :1;
    IO_BYTE _RBI :1;
    IO_BYTE _RDRF :1;
    IO_BYTE _TDRE :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }ESIRxSTR;


/**
 * @brief @p Memory mapped register definition.
 */
struct	_uart_lin_reg{
	SMRxSTR	SMR;
	SCRxSTR	SCR;
	IO_BYTE	DATA;
	SSRxSTR	SSR;

	ECCRxSTR	ECCR;
	ESCRxSTR	ESCR;

	union	{
		IO_BYTE	byte[2];
		IO_WORD word;
	}	BGR;

	ESIRxSTR	ESIR;
};


/**
 * @brief @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  InputQueue                iqueue;                                         \
  /* Output queue.*/                                                        \
  OutputQueue               oqueue;                                         \
  /* Input circular buffer.*/                                               \
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];                        \
  /* Output circular buffer.*/                                              \
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];                        \
  /* End of the mandatory fields.*/                                         \
  /* Memory mapped USART register entry point	*/                          \
  struct _uart_lin_reg *reg;                                              


/**
 * @brief MB96F3xx Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 */
typedef struct {
  uint32_t              speed;
  uint8_t               scr;
  uint8_t               smr;
} SerialConfig;


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define	USART0_Base	((struct _uart_lin_reg*)&_smr0)
#define	USART1_Base	((struct _uart_lin_reg*)&_smr1)
#define	USART2_Base	((struct _uart_lin_reg*)&_smr2)
#define	USART3_Base	((struct _uart_lin_reg*)&_smr3)
#define	USART4_Base	((struct _uart_lin_reg*)&_smr4)
#define	USART5_Base	((struct _uart_lin_reg*)&_smr5)
#define	USART6_Base	((struct _uart_lin_reg*)&_smr6)
#define	USART7_Base	((struct _uart_lin_reg*)&_smr7)
#define	USART8_Base	((struct _uart_lin_reg*)&_smr8)
#define	USART9_Base	((struct _uart_lin_reg*)&_smr9)


/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if USE_MB96F3xx_USART0
extern SerialDriver SD0;
#endif
#if USE_MB96F3xx_USART1
extern SerialDriver SD1;
#endif
#if USE_MB96F3xx_USART2
extern SerialDriver SD2;
#endif
#if USE_MB96F3xx_USART3
extern SerialDriver SD3;
#endif
#if USE_MB96F3xx_USART4
extern SerialDriver SD4;
#endif
#if USE_MB96F3xx_USART5
extern SerialDriver SD5;
#endif
#if USE_MB96F3xx_USART6
extern SerialDriver SD6;
#endif
#if USE_MB96F3xx_USART7
extern SerialDriver SD7;
#endif
#if USE_MB96F3xx_USART8
extern SerialDriver SD8;
#endif
#if USE_MB96F3xx_USART9
extern SerialDriver SD9;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* CH_HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
