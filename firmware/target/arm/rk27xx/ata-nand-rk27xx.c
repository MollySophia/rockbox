/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 Dave Chapman
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
#include "config.h"
#include "system.h"
#include <string.h>
#include "thread.h"
#include "disk.h"
#include "storage.h"
#include "panic.h"
#include "usb.h"
#include "ftl-target.h"
#include "nand-target.h"

/* This file provides only STUBS for now */

/** static, private data **/ 
static bool initialized = false;

/* API Functions */
int nand_read_sectors(IF_MD(int drive,) unsigned long start, int incount,
                     void* inbuf)
{
    (void)drive;
    return ftl_read(start, incount, inbuf);
}

int nand_write_sectors(IF_MD(int drive,) unsigned long start, int count,
                      const void* outbuf)
{
    (void)drive;
    return ftl_write(start, count, outbuf);
}

void nand_spindown(int seconds)
{
    (void)seconds;
}

void nand_sleep(void)
{
    nand_power_down();
}

void nand_sleepnow(void)
{
    nand_power_down();
}

void nand_spin(void)
{
    nand_set_active();
}

void nand_enable(bool on)
{
    (void)on;
}

void nand_get_info(IF_MD(int drive,) struct storage_info *info)
{
    (void)drive;
    uint32_t ppb = ftl_banks * (*ftl_nand_type).pagesperblock;
    (*info).sector_size = SECTOR_SIZE;
    (*info).num_sectors = (*ftl_nand_type).userblocks * ppb;
    (*info).vendor = "";
    (*info).product = "";
    (*info).revision = "";
}

long nand_last_disk_activity(void)
{
    return nand_last_activity();
}

#ifdef HAVE_STORAGE_FLUSH
int nand_flush(void)
{
    int rc = ftl_sync();
    if (rc != 0) panicf("Failed to unmount flash: %X", rc);
    return rc;
}
#endif

int nand_init(void)
{
    if (ftl_init()) return 1;

    initialized = true;
    return 0;
}

#ifdef CONFIG_STORAGE_MULTI
int nand_num_drives(int first_drive)
{
    /* We don't care which logical drive number(s) we have been assigned */
    (void)first_drive;
    
    return 1;
}
#endif

int nand_event(long id, intptr_t data)
{
    int rc = 0;

#if 0 /* The NAND functions do nothing right now; just provide template */
    if (LIKELY(id == Q_STORAGE_TICK))
    {
        if (!nand_powered ||
            TIME_BEFORE(current_tick, nand_last_activity() + HZ / 5))
        {
            STG_EVENT_ASSERT_ACTIVE(STORAGE_NAND);
        }
    }
    else if (id == Q_STORAGE_SLEEPNOW)
    {
        nand_power_down();
    }
    else
    {
        rc = storage_event_default_handler(id, data, nand_last_activity(),
                                           STORAGE_NAND);
    }
#endif
    return rc;
    (void)id; (void)data;
}
