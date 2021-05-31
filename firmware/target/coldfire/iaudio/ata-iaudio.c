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
#include "config.h"
#include "cpu.h"
#include <stdbool.h>
#include "kernel.h"
#include "system.h"
#include "power.h"
#include "pcf50606.h"
#include "ata-driver.h"

void ata_reset(void)
{
    and_l(~0x40000000, &GPIO_OUT);
    sleep(1); /* > 25us */
    or_l(0x40000000, &GPIO_OUT);
    sleep(1); /* > 2ms */
}

void ata_enable(bool on)
{
    if(on)
        and_l(~0x00000800, &GPIO1_OUT);
    else
        or_l(0x00000800, &GPIO1_OUT);
}

bool ata_is_coldstart(void)
{
    return true; /* TODO */
}

void ata_device_init(void)
{
    /* ATA reset */
    or_l(0x40000000, &GPIO_OUT);
    or_l(0x40000000, &GPIO_ENABLE);
    or_l(0x40000000, &GPIO_FUNCTION);

    /* ATA enable */
    or_l(0x00000800, &GPIO1_OUT);
    or_l(0x00000800, &GPIO1_ENABLE);
    or_l(0x00000800, &GPIO1_FUNCTION);

    /* USB enable */
    and_l(~0x00000008, &GPIO1_OUT);
    or_l(0x00000008, &GPIO1_ENABLE);
    or_l(0x00000008, &GPIO1_FUNCTION);
}
