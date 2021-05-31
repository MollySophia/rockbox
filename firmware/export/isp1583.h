/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by Maurus Cuelenaere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#ifndef ISP1583_H
#define ISP1583_H

#include "dm320.h"

/* General purpose memory region #2 */
#define ISP1583_IOBASE              0x60FFC000

#define ISP1583_INIT_ADDRESS        (*((volatile unsigned char*)(ISP1583_IOBASE+0x0))) //char
#define ISP1583_INIT_MODE           (*((volatile unsigned short*)(ISP1583_IOBASE+0xC*2)))
#define ISP1583_INIT_INTCONF        (*((volatile unsigned char*)(ISP1583_IOBASE+0x10*2))) //char
#define ISP1583_INIT_OTG            (*((volatile unsigned char*)(ISP1583_IOBASE+0x12*2))) //char
#define ISP1583_INIT_INTEN_A        (*((volatile unsigned short*)(ISP1583_IOBASE+0x14*2)))
#define ISP1583_INIT_INTEN_B        (*((volatile unsigned short*)(ISP1583_IOBASE+0x14*2+4)))
#define ISP1583_INIT_INTEN_READ     (unsigned long)( (ISP1583_INIT_INTEN_A & 0xFFFF) | ((ISP1583_INIT_INTEN_B & 0xFFFF) << 16) )
/* Data flow registers */
#define ISP1583_DFLOW_EPINDEX       (*((volatile unsigned char*)(ISP1583_IOBASE+0xC2*2))) //char
#define ISP1583_DFLOW_CTRLFUN       (*((volatile unsigned char*)(ISP1583_IOBASE+0x28*2))) //char
#define ISP1583_DFLOW_DATA          (*((volatile unsigned short*)(ISP1583_IOBASE+0x20*2)))
#define ISP1583_DFLOW_BUFLEN        (*((volatile unsigned short*)(ISP1583_IOBASE+0x1C*2)))
#define ISP1583_DFLOW_BUFSTAT       (*((volatile unsigned char*)(ISP1583_IOBASE+0x1E*2))) //char
#define ISP1583_DFLOW_MAXPKSZ       (*((volatile unsigned short*)(ISP1583_IOBASE+0x04*2)))
#define ISP1583_DFLOW_EPTYPE        (*((volatile unsigned short*)(ISP1583_IOBASE+0x08*2)))
/* DMA registers */
#define ISP1583_DMA_ENDPOINT        (*((volatile unsigned char*)(ISP1583_IOBASE+0x58*2)))
/* General registers */
#define ISP1583_GEN_INT_A           (*((volatile unsigned short*)(ISP1583_IOBASE+0x18*2)))
#define ISP1583_GEN_INT_B           (*((volatile unsigned short*)(ISP1583_IOBASE+0x18*2+4)))
#define ISP1583_GEN_INT_READ        (unsigned long)( (ISP1583_GEN_INT_A & 0xFFFF) | ((ISP1583_GEN_INT_B & 0xFFFF) << 16))
#define ISP1583_GEN_CHIPID_A        (*((volatile unsigned short*)(ISP1583_IOBASE+0x70*2)))
#define ISP1583_GEN_CHIPID_B        (*((volatile unsigned char*)(ISP1583_IOBASE+0x70*2+4))) //char
#define ISP1583_GEN_CHIPID          (unsigned long)( (ISP1583_GEN_CHIPID_A & 0xFFFF) | ((ISP1583_GEN_CHIPID_B & 0xFFFF) << 16) )
#define ISP1583_GEN_FRAMEN0         (*((volatile unsigned short*)(ISP1583_IOBASE+0x74*2)))
#define ISP1583_GEN_SCRATCH         (*((volatile unsigned short*)(ISP1583_IOBASE+0x78*2)))
#define ISP1583_GEN_UNLCKDEV        (*((volatile unsigned short*)(ISP1583_IOBASE+0x7C*2)))
#define ISP1583_GEN_TSTMOD          (*((volatile unsigned char*)(ISP1583_IOBASE+0x84*2))) //char

#define EN_INT_CPU_TARGET           IO_INTC_EINT1 |= INTR_EINT1_EXT7
#define DIS_INT_CPU_TARGET          IO_INTC_EINT1 &= ~INTR_EINT1_EXT7
#define INT_CONF_TARGET             0
//#define INT_CONF_TARGET             2
#define set_int_value(a,b,value)    a = value & 0xFFFF; \
                                    b = value >> 16;
#define ISP1583_UNLOCK_CODE         ((unsigned short)0xAA37)

/* Initialization registers' bits */

/* Initialization OTG register bits */
#define INIT_OTG_BSESS_VALID        (1 << 4)

/* Initialization Mode register bits */
#define INIT_MODE_TEST2             (1 << 15)
#define INIT_MODE_TEST1             (1 << 14)
#define INIT_MODE_TEST0             (1 << 13)
#define INIT_MODE_DMA_CLKON         (1 << 9)
#define INIT_MODE_VBUSSTAT          (1 << 8)
#define INIT_MODE_CLKAON            (1 << 7)
#define INIT_MODE_SNDRSU            (1 << 6)
#define INIT_MODE_GOSUSP            (1 << 5)
#define INIT_MODE_SFRESET           (1 << 4)
#define INIT_MODE_GLINTENA          (1 << 3)
#define INIT_MODE_WKUPCS            (1 << 2)
#define INIT_MODE_PWRON             (1 << 1)
#define INIT_MODE_SOFTCT            (1 << 0)

/* Initialization Interrupt Enable register bits */
#define INIT_INTEN_IEP7TX           (1 << 25)
#define INIT_INTEN_IEP7RX           (1 << 24)
#define INIT_INTEN_IEP6TX           (1 << 23)
#define INIT_INTEN_IEP6RX           (1 << 22)
#define INIT_INTEN_IEP5TX           (1 << 21)
#define INIT_INTEN_IEP5RX           (1 << 20)
#define INIT_INTEN_IEP4TX           (1 << 19)
#define INIT_INTEN_IEP4RX           (1 << 18)
#define INIT_INTEN_IEP3TX           (1 << 17)
#define INIT_INTEN_IEP3RX           (1 << 16)
#define INIT_INTEN_IEP2TX           (1 << 15)
#define INIT_INTEN_IEP2RX           (1 << 14)
#define INIT_INTEN_IEP1TX           (1 << 13)
#define INIT_INTEN_IEP1RX           (1 << 12)
#define INIT_INTEN_IEP0TX           (1 << 11)
#define INIT_INTEN_IEP0RX           (1 << 10)
#define INIT_INTEN_IEP0SETUP        (1 << 8)
#define INIT_INTEN_IEVBUS           (1 << 7)
#define INIT_INTEN_IEDMA            (1 << 6)
#define INIT_INTEN_IEHS_STA         (1 << 5)
#define INIT_INTEN_IERESM           (1 << 4)
#define INIT_INTEN_IESUSP           (1 << 3)
#define INIT_INTEN_IEPSOF           (1 << 2)
#define INIT_INTEN_IESOF            (1 << 1)
#define INIT_INTEN_IEBRST           (1 << 0)

/* Initialization Interrupt Configuration register bits */
#define INIT_INTCONF_INTLVL         (1 << 1)
#define INIT_INTCONF_INTPOL         (1 << 0)

/* Initialization Address register bits */
#define INIT_ADDRESS_DEVEN          (1 << 7)

/* Data Flow registers' bits */

/* Data Flow Endpoint Index register bits */
#define DFLOW_EPINDEX_EP0SETUP      (1 << 5)

/* Data Flow Control Function register bits */
#define DFLOW_CTRLFUN_CLBUF         (1 << 4)
#define DFLOW_CTRLFUN_VENDP         (1 << 3)
#define DFLOW_CTRLFUN_DSEN          (1 << 2)
#define DFLOW_CTRLFUN_STATUS        (1 << 1)
#define DFLOW_CTRLFUN_STALL         (1 << 0)

/* Data Flow Endpoint Type register bits */
#define DFLOW_EPTYPE_NOEMPKT        (1 << 4)
#define DFLOW_EPTYPE_ENABLE         (1 << 3)
#define DFLOW_EPTYPE_DBLBUF         (1 << 2)

/* General registers' bits */

/* General Test Mode register bits */
#define GEN_TSTMOD_FORCEHS          (1 << 7)
#define GEN_TSTMOD_FORCEFS          (1 << 4)
#define GEN_TSTMOD_PRBS             (1 << 3)
#define GEN_TSTMOD_KSTATE           (1 << 2)
#define GEN_TSTMOD_JSTATE           (1 << 1)
#define GEN_TSTMOD_SE0_NAK          (1 << 0)

/* Interrupts */
#define INT_IEP7TX                  (1 << 25)
#define INT_IEP7RX                  (1 << 24)
#define INT_IEP6TX                  (1 << 23)
#define INT_IEP6RX                  (1 << 22)
#define INT_IEP5TX                  (1 << 21)
#define INT_IEP5RX                  (1 << 20)
#define INT_IEP4TX                  (1 << 19)
#define INT_IEP4RX                  (1 << 18)
#define INT_IEP3TX                  (1 << 17)
#define INT_IEP3RX                  (1 << 16)
#define INT_IEP2TX                  (1 << 15)
#define INT_IEP2RX                  (1 << 14)
#define INT_IEP1TX                  (1 << 13)
#define INT_IEP1RX                  (1 << 12)
#define INT_IEP0TX                  (1 << 11)
#define INT_IEP0RX                  (1 << 10)
#define INT_IEP0SETUP               (1 << 8)
#define INT_IEVBUS                  (1 << 7)
#define INT_IEDMA                   (1 << 6)
#define INT_IEHS_STA                (1 << 5)
#define INT_IERESM                  (1 << 4)
#define INT_IESUSP                  (1 << 3)
#define INT_IEPSOF                  (1 << 2)
#define INT_IESOF                   (1 << 1)
#define INT_IEBRST                  (1 << 0)

#define INT_EP_MASK                 ( INT_IEP0RX | INT_IEP0TX | INT_IEP1RX | INT_IEP1TX | INT_IEP2RX | INT_IEP2TX | INT_IEP3RX | INT_IEP3TX | INT_IEP4RX | INT_IEP4TX | INT_IEP5RX | INT_IEP5TX | INT_IEP6RX | INT_IEP6TX | INT_IEP7RX | INT_IEP7TX )

#define STANDARD_INTEN              ( INIT_INTEN_IEBRST | INIT_INTEN_IEHS_STA | INT_IESUSP | INT_IERESM | INIT_INTEN_IEVBUS | INIT_INTEN_IEP0SETUP | INIT_INTEN_IEP0RX | INIT_INTEN_IEP0TX )
#define STANDARD_INIT_MODE          ( INIT_MODE_CLKAON | INIT_MODE_GLINTENA )

#define IRAM_ATTR __attribute__ ((section(".icode")))

#include "usb_drv.h"

#endif
