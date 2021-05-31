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

#include "config.h"
#include "cpu.h"
#include "kernel.h"
#include "system.h"
#include "backlight.h"
#include "backlight-target.h"
#include "lcd.h"

static bool _backlight_on = true;
static int _brightness = DEFAULT_BRIGHTNESS_SETTING;

/* Returns the current state of the backlight (true=ON, false=OFF). */
bool backlight_hw_init(void)
{
#ifdef BOOTLOADER
    and_l(~(1<<28),&GPIO_OUT);
#endif
    or_l((1<<28),&GPIO_FUNCTION);
    or_l((1<<28),&GPIO_ENABLE);
    return true;
}

void backlight_hw_on(void)
{
#ifndef BOOTLOADER
    if (_backlight_on)
        return;
#endif
   
    backlight_hw_brightness(_brightness);
    _backlight_on = true;

}

void backlight_hw_off(void)
{
    /* GPIO28 low */
    and_l(~(1<<28),&GPIO_OUT);
    _backlight_on = false;
}

void backlight_hw_brightness(int val)
{
    unsigned char i;

#ifndef BOOTLOADER
    if( _brightness == val && _backlight_on == true )
        return;
#endif

    and_l(~(1<<28),&GPIO_OUT);
    sleep(4);

    for (i=0;i<val;i++)
    {
        or_l((1<<28),&GPIO_OUT);
        and_l(~(1<<28),&GPIO_OUT);
    }
    
    or_l((1<<28),&GPIO_OUT);

    _brightness = val;
}

void remote_backlight_hw_on(void)
{
    /* I don't have remote to play with */
}

void remote_backlight_hw_off(void)
{
    /* I don't have remote to play with */
}
