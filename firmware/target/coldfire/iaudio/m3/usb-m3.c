/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by Jens Arnold
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
#include <stdbool.h>
#include "cpu.h"
#include "system.h"
#include "usb.h"

void usb_init_device(void)
{
    and_l(~0x10000000, &GPIO_OUT);   /* USB enable bit */
    or_l(0x10000000, &GPIO_ENABLE);
    or_l(0x10000000, &GPIO_FUNCTION);
    
    or_l(0x00000400, &GPIO1_FUNCTION);  /* USB detect */
}

int usb_detect(void)
{
    return (GPIO1_READ & 0x00000400) ? USB_INSERTED : USB_EXTRACTED;
}

void usb_enable(bool on)
{
    if(on) {
        or_l(0x10000000, &GPIO_OUT);
    } else {
        and_l(~0x10000000, &GPIO_OUT);
    }
}
