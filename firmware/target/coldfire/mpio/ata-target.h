/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010 Marcin Bukat
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

#define ATA_OPTIMIZED_READING
#define ATA_OPTIMIZED_WRITING

#define ATA_IOBASE      0x20000000
#define ATA_DATA        (*((volatile unsigned short*)(ATA_IOBASE + 0x20)))
#define ATA_CONTROL     (*((volatile unsigned short*)(ATA_IOBASE + 0x1c)))

#define ATA_ERROR       (*((volatile unsigned short*)(ATA_IOBASE + 0x22)))
#define ATA_NSECTOR     (*((volatile unsigned short*)(ATA_IOBASE + 0x24)))
#define ATA_SECTOR      (*((volatile unsigned short*)(ATA_IOBASE + 0x26)))
#define ATA_LCYL        (*((volatile unsigned short*)(ATA_IOBASE + 0x28)))
#define ATA_HCYL        (*((volatile unsigned short*)(ATA_IOBASE + 0x2a)))
#define ATA_SELECT      (*((volatile unsigned short*)(ATA_IOBASE + 0x2c)))
#define ATA_COMMAND     (*((volatile unsigned short*)(ATA_IOBASE + 0x2e)))


#endif
