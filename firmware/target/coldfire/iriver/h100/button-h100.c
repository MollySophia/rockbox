/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Jonathan Gordon
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
#include "system.h"
#include "button.h"
#include "backlight.h"
#include "adc.h"
#include "lcd-remote.h"

void button_init_device(void)
{
    /* Set GPIO33, GPIO37, GPIO38  and GPIO52 as general purpose inputs */
    GPIO1_FUNCTION |= 0x00100062;
    GPIO1_ENABLE &= ~0x00100060;
}

bool button_hold(void)
{
    return (GPIO1_READ & 0x00000002)?true:false;
}

static bool remote_button_hold_only(void)
{
    if(remote_type() == REMOTETYPE_H300_NONLCD)
        return adc_scan(ADC_REMOTE)<0x0d; /* hold should be 0x00 */
    else
        return (GPIO1_READ & 0x00100000)?true:false;
}

/* returns true only if there is remote present */
bool remote_button_hold(void)
{
    /* H300's NON-LCD remote doesn't set the "remote present" bit */
    if(remote_type() == REMOTETYPE_H300_NONLCD)
        return remote_button_hold_only();
    else
        return ((GPIO_READ & 0x40000000) == 0)?remote_button_hold_only():false;
}

/*
 * Get button pressed from hardware
 */
int button_read_device(void)
{
    int btn = BUTTON_NONE;
    int data;
    static bool hold_button = false;
    static bool remote_hold_button = false;
    static int prev_data = 0xff;
    static int last_valid = 0xff;
#ifndef BOOTLOADER
    bool hold_button_old;
    bool remote_hold_button_old;
#endif

    /* normal buttons */
#ifndef BOOTLOADER
    hold_button_old = hold_button;
#endif
    hold_button = button_hold();

#ifndef BOOTLOADER
    if (hold_button != hold_button_old)
        backlight_hold_changed(hold_button);
#endif

    if (!hold_button)
    {
        data = adc_scan(ADC_BUTTONS);

        /* ADC debouncing: Only accept new reading if it's
         * stable (+/-1). Use latest stable value otherwise. */
        if ((unsigned)(data - prev_data + 1) <= 2)
            last_valid = data;
        prev_data = data;
        data = last_valid;
        
        if (data < 0xf0)
        {
            if (data < 0x80)
                if (data < 0x30)
                    if (data < 0x18)
                        btn = BUTTON_SELECT;
                    else
                        btn = BUTTON_UP;
                else
                    if (data < 0x50)
                        btn = BUTTON_LEFT;
                    else
                        btn = BUTTON_DOWN;
            else
                if (data < 0xb0)
                    if (data < 0xa0)
                        btn = BUTTON_RIGHT;
                    else
                        btn = BUTTON_OFF;
                else
                    if (data < 0xd0)
                        btn = BUTTON_MODE;
                    else
                        btn = BUTTON_REC;
        }
    }

    /* remote buttons */
#ifndef BOOTLOADER
    remote_hold_button_old = remote_hold_button;
#endif
    remote_hold_button = remote_button_hold_only();

#ifndef BOOTLOADER
    if (remote_hold_button != remote_hold_button_old)
        remote_backlight_hold_changed(remote_hold_button);
#endif

    if (!remote_hold_button)
    {
        data = adc_scan(ADC_REMOTE);
        switch (remote_type())
        {
            case REMOTETYPE_H100_LCD:
                if (data < 0xf5)
                {
                    if (data < 0x73)
                        if (data < 0x3f)
                            if (data < 0x25)
                                if(data < 0x0c)
                                    btn |= BUTTON_RC_STOP;
                                else
                                    btn |= BUTTON_RC_VOL_DOWN;
                            else
                                btn |= BUTTON_RC_MODE;
                        else
                            if (data < 0x5a)
                                btn |= BUTTON_RC_VOL_UP;
                            else
                                btn |= BUTTON_RC_BITRATE;
                    else
                        if (data < 0xa8)
                            if (data < 0x8c)
                                btn |= BUTTON_RC_REC;
                            else
                                btn |= BUTTON_RC_SOURCE;
                        else
                            if (data < 0xdf)
                                if(data < 0xc5)
                                    btn |= BUTTON_RC_FF;
                                else
                                    btn |= BUTTON_RC_MENU;
                            else
                                btn |= BUTTON_RC_REW;
                }
                break;
            case REMOTETYPE_H300_LCD:
                if (data < 0xf5)
                {
                    if (data < 0x73)
                        if (data < 0x42)
                            if (data < 0x27)
                                if(data < 0x0c)
                                    btn |= BUTTON_RC_VOL_DOWN;
                                else
                                    btn |= BUTTON_RC_FF;
                            else
                                btn |= BUTTON_RC_STOP;
                        else
                            if (data < 0x5b)
                                btn |= BUTTON_RC_MODE;
                            else
                                btn |= BUTTON_RC_REC;
                    else
                        if (data < 0xab)
                            if (data < 0x8e)
                                btn |= BUTTON_RC_ON;
                            else
                                btn |= BUTTON_RC_BITRATE;
                        else
                            if (data < 0xde)
                                if(data < 0xc5)
                                    btn |= BUTTON_RC_SOURCE;
                                else
                                    btn |= BUTTON_RC_VOL_UP;
                            else
                                btn |= BUTTON_RC_REW;
                }
                break;
            case REMOTETYPE_H300_NONLCD:
                if (data < 0xf1)
                {
                    if (data < 0x7d)
                        if (data < 0x25)
                            btn |= BUTTON_RC_FF;
                        else
                            btn |= BUTTON_RC_REW;
                    else
                        if (data < 0xd5)
                            btn |= BUTTON_RC_VOL_DOWN;
                        else
                            btn |= BUTTON_RC_VOL_UP;
                }
                break;
        }
    }

    data = GPIO1_READ;
    if (!hold_button && ((data & 0x20) == 0))
        btn |= BUTTON_ON;
    if (!remote_hold_button && ((data & 0x40) == 0))
        switch(remote_type())
        {
            case REMOTETYPE_H100_LCD:
            case REMOTETYPE_H300_NONLCD:
                btn |= BUTTON_RC_ON;
                break;
            case REMOTETYPE_H300_LCD:
                btn |= BUTTON_RC_MENU;
                break;
        }
        
    return btn;
}
