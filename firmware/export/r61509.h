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

/*
 * Register definitions for the Renesas R61509 TFT Panel
 */
#ifndef __R61509_H
#define __R61509_H

/* Register list */
#define REG_DEVICE_CODE                  0x000
#define REG_DRIVER_OUTPUT                0x001
#define REG_LCD_DR_WAVE_CTRL             0x002
#define REG_ENTRY_MODE                   0x003
#define REG_OUTL_SHARP_CTRL              0x006
#define REG_DISP_CTRL1                   0x007
#define REG_DISP_CTRL2                   0x008
#define REG_DISP_CTRL3                   0x009
#define REG_LPCTRL                       0x00B
#define REG_EXT_DISP_CTRL1               0x00C
#define REG_EXT_DISP_CTRL2               0x00F
#define REG_PAN_INTF_CTRL1               0x010
#define REG_PAN_INTF_CTRL2               0x011
#define REG_PAN_INTF_CTRL3               0x012
#define REG_PAN_INTF_CTRL4               0x020
#define REG_PAN_INTF_CTRL5               0x021
#define REG_PAN_INTF_CTRL6               0x022
#define REG_FRM_MRKR_CTRL                0x090

#define REG_PWR_CTRL1                    0x100
#define REG_PWR_CTRL2                    0x101
#define REG_PWR_CTRL3                    0x102
#define REG_PWR_CTRL4                    0x103
#define REG_PWR_CTRL5                    0x107
#define REG_PWR_CTRL6                    0x110
#define REG_PWR_CTRL7                    0x112

#define REG_RAM_HADDR_SET                0x200
#define REG_RAM_VADDR_SET                0x201
#define REG_RW_GRAM                      0x202
#define REG_RAM_HADDR_START              0x210
#define REG_RAM_HADDR_END                0x211
#define REG_RAM_VADDR_START              0x212
#define REG_RAM_VADDR_END                0x213
#define REG_RW_NVM                       0x280
#define REG_VCOM_HVOLTAGE1               0x281
#define REG_VCOM_HVOLTAGE2               0x282

#define REG_GAMMA_CTRL1                  0x300
#define REG_GAMMA_CTRL2                  0x301
#define REG_GAMMA_CTRL3                  0x302
#define REG_GAMMA_CTRL4                  0x303
#define REG_GAMMA_CTRL5                  0x304
#define REG_GAMMA_CTRL6                  0x305
#define REG_GAMMA_CTRL7                  0x306
#define REG_GAMMA_CTRL8                  0x307
#define REG_GAMMA_CTRL9                  0x308
#define REG_GAMMA_CTRL10                 0x309
#define REG_GAMMA_CTRL11                 0x30A
#define REG_GAMMA_CTRL12                 0x30B
#define REG_GAMMA_CTRL13                 0x30C
#define REG_GAMMA_CTRL14                 0x30D

#define REG_BIMG_NR_LINE                 0x400
#define REG_BIMG_DISP_CTRL               0x401
#define REG_BIMG_VSCROLL_CTRL            0x404

#define REG_PARTIMG1_POS                 0x500
#define REG_PARTIMG1_RAM_START           0x501
#define REG_PARTIMG1_RAM_END             0x502
#define REG_PARTIMG2_POS                 0x503
#define REG_PARTIMG2_RAM_START           0x504
#define REG_PARTIMG2_RAM_END             0x505

#define REG_SOFT_RESET                   0x600
#define REG_ENDIAN_CTRL                  0x606
#define REG_NVM_ACCESS_CTRL              0x6F0

/* Bits */
#define DRIVER_OUTPUT_SS_BIT             (1 << 8)
#define DRIVER_OUTPUT_SM_BIT             (1 << 10)

#define ENTRY_MODE_TRI                   (1 << 15)
#define ENTRY_MODE_DFM                   (1 << 14)
#define ENTRY_MODE_BGR                   (1 << 12)
#define ENTRY_MODE_HWM                   (1 << 9)
#define ENTRY_MODE_ORG                   (1 << 7)
#define ENTRY_MODE_VID                   (1 << 5)
#define ENTRY_MODE_HID                   (1 << 4)
#define ENTRY_MODE_AM                    (1 << 3)
#define ENTRY_MODE_EPF(n)                (n & 3)

#define OUTL_SHARP_CTRL_EGMODE           (1 << 15)
#define OUTL_SHARP_CTRL_AVST(n)          ((n & 7) << 7)
#define OUTL_SHARP_CTRL_ADST(n)          ((n & 7) << 4)
#define OUTL_SHARP_CTRL_DTHU(n)          ((n & 3) << 2)
#define OUTL_SHARP_CTRL_DTHL(n)          (n & 3)

#define DISP_CTRL1_PTDE(n)               ((n & 4) << 12)
#define DISP_CTRL1_BASEE                 (1 << 8)
#define DISP_CTRL1_VON                   (1 << 6)
#define DISP_CTRL1_GON                   (1 << 5)
#define DISP_CTRL1_DTE                   (1 << 4)
#define DISP_CTRL1_D(n)                  (n & 3)

#define EXT_DISP_CTRL1_ENC(n)            ((n & 7) << 12)
#define EXT_DISP_CTRL1_RM(n)             ((n & 1) << 8)
#define EXT_DISP_CTRL1_DM(n)             ((n & 3) << 4)
#define EXT_DISP_CTRL1_RIM(n)            (n & 3)

#define PWR_CTRL1_SAP(n)                 ((n & 3) << 13)
#define PWR_CTRL1_SAPE                   (1 << 12)
#define PWR_CTRL1_BT(n)                  ((n & 7) << 8)
#define PWR_CTRL1_APE                    (1 << 7)
#define PWR_CTRL1_AP(n)                  ((n & 7) << 4)
#define PWR_CTRL1_DSTB                   (1 << 2)
#define PWR_CTRL1_SLP                    (1 << 1)

#define SOFT_RESET(n)                    (n << 0)

#endif /* __R61509_H */
