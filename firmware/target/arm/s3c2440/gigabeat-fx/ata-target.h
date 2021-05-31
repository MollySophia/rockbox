/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Linus Nielsen Feltzing
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
#ifndef ATA_TARGET_H
#define ATA_TARGET_H

/* Plain C read & write loops */
#define PREFER_C_READING
#define PREFER_C_WRITING
#if !defined(BOOTLOADER)
#define ATA_OPTIMIZED_READING
#endif

#define ATA_IOBASE      0x18000000
#define ATA_DATA        (*((volatile unsigned short*)(ATA_IOBASE)))
#define ATA_ERROR       (*((volatile unsigned char*)(ATA_IOBASE + 0x02)))
#define ATA_NSECTOR     (*((volatile unsigned char*)(ATA_IOBASE + 0x04)))
#define ATA_SECTOR      (*((volatile unsigned char*)(ATA_IOBASE + 0x06)))
#define ATA_LCYL        (*((volatile unsigned char*)(ATA_IOBASE + 0x08)))
#define ATA_HCYL        (*((volatile unsigned char*)(ATA_IOBASE + 0x0A)))
#define ATA_SELECT      (*((volatile unsigned char*)(ATA_IOBASE + 0x0C)))
#define ATA_COMMAND     (*((volatile unsigned char*)(ATA_IOBASE + 0x0E)))
#define ATA_CONTROL     (*((volatile unsigned char*)(0x20000000 + 0x1C)))


#endif
