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
#include "system.h"
#include "button.h"
#include "backlight.h"
#include "adc.h"
#include "lcd-remote.h"

static bool hold_button        = false;
static bool remote_hold_button = false;

void button_init_device(void)
{
    /* Remote Play */
    GPIO_FUNCTION |= 0x80000000;
    GPIO_ENABLE &= ~0x80000000;
}

bool button_hold(void)
{
    return (GPIO1_READ & 0x00000200) == 0;
}

bool remote_button_hold(void)
{
    return remote_hold_button;
}

int button_read_device(void)
{
    int  btn = BUTTON_NONE;
#ifndef BOOTLOADER
    bool remote_hold_button_old;
#endif
    int  data;

    /* normal buttons */
    hold_button = button_hold();

    if (!hold_button)
    {
        data = adc_read(ADC_BUTTONS);

        if (data < 0xc0)
        {
            if (data < 0x67)
                if (data < 0x37)
                    btn = BUTTON_VOL_DOWN;
                else
                    if (data < 0x51)
                        btn = BUTTON_MODE;
                    else
                        btn = BUTTON_VOL_UP;
            else
                if (data < 0x7f)
                    btn = BUTTON_REC;
                else
                    if (data < 0x98)
                        btn = BUTTON_LEFT;
                    else
                        btn = BUTTON_RIGHT;
        }
        if (!(GPIO1_READ & 0x00000002))
            btn |= BUTTON_PLAY;
    }

    /* remote buttons */
    data = remote_detect() ? adc_read(ADC_REMOTE) : 0xff;

#ifndef BOOTLOADER
    remote_hold_button_old = remote_hold_button;
#endif
    remote_hold_button = data < 0x14;

#ifndef BOOTLOADER
    if (remote_hold_button != remote_hold_button_old)
        backlight_hold_changed(remote_hold_button);
#endif

    if (!remote_hold_button)
    {
        if (data < 0xd0)
        {
            if (data < 0x67)
                if (data < 0x37)
                    btn |= BUTTON_RC_FF;
                else
                    if (data < 0x51)
                        btn |= BUTTON_RC_REW;
                    else
                        btn |= BUTTON_RC_MODE;
            else
                if (data < 0x98)
                    if (data < 0x7f)
                        btn |= BUTTON_RC_REC;
                    else
                        btn |= BUTTON_RC_MENU;
                else
                    if (data < 0xb0)
                        btn |= BUTTON_RC_VOL_UP;
                    else
                        btn |= BUTTON_RC_VOL_DOWN;
        }
        if ((GPIO_READ & 0x80000000) == 0)
            btn |= BUTTON_RC_PLAY;
    }

    return btn;
}
