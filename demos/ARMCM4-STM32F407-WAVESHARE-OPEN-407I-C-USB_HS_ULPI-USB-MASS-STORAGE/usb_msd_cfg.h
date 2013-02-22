/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

#ifndef USB_MSD_CFG_H_
#define USB_MSD_CFG_H_


#define USB_MS_DATA_EP 1

#if STM32_USE_USB_OTG2_HS
#  define USB_MS_EP_SIZE 512
#else
#  define USB_MS_EP_SIZE 64
#endif


#ifdef MSD_USB_DRIVER_EXT_FIELDS_NAME
#  define   USBD_PARAM_NAME     MSD_USB_DRIVER_EXT_FIELDS_NAME
#else
#  define   USBD_PARAM_NAME     param
#endif







#endif /* USB_MSD_CFG_H_ */
