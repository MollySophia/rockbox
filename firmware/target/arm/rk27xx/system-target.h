/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 Rafaël Carré
 * Copyright (C) 2011 Marcin Bukat
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
#ifndef SYSTEM_TARGET_H
#define SYSTEM_TARGET_H

#include "system-arm.h"
#include "panic.h"

void udelay(unsigned usecs);
static inline void mdelay(unsigned msecs)
{
    udelay(1000 * msecs);
}

/* Datasheet is very cryptic how to use this.
 * With cache enabled it simpy hangs here
 */
static inline void core_sleep(void)
{
    enable_irq();
    /* SCU_CPUPD = 0xdeedbabe; */
}

/* Write DCache back to RAM for the given range and remove cache lines
 * from DCache afterwards */
void commit_discard_dcache_range(const void *base, unsigned int size);

static inline void commit_dcache(void) {}
void commit_discard_dcache(void);
void commit_discard_idcache(void);

#define CPUFREQ_DEFAULT  50000000
#define CPUFREQ_NORMAL   50000000
#define CPUFREQ_MAX     200000000

#define STORAGE_WANTS_ALIGN
#define STORAGE_NEEDS_BOUNCE_BUFFER

#endif /* SYSTEM_TARGET_H */
