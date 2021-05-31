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
#include "kernel.h"
#include "thread.h"
#include "system.h"
#include "backlight.h"
#include "backlight-target.h"
#include "pcf50606.h"
#include "lcd.h"

bool backlight_hw_init(void)
{
    or_l(0x00020000, &GPIO1_ENABLE);
    or_l(0x00020000, &GPIO1_FUNCTION);
    or_l(0x00020000, &GPIO1_OUT);  /* Start with the backlight ON */
    
    return true; /* Backlight always ON after boot. */
}

void backlight_hw_on(void)
{
    lcd_enable(true);
    sleep(HZ/100); /* lcd needs time - avoid flashing for dark screens */
    or_l(0x00020000, &GPIO1_OUT);
}

void backlight_hw_off(void)
{
    and_l(~0x00020000, &GPIO1_OUT);
    lcd_enable(false);
}

/* set brightness by changing the PWM */
void backlight_hw_brightness(int val)
{
    /* disable IRQs while bitbanging */
    int old_irq_level = disable_irq_save();
    pcf50606_write(0x35, (val << 1) | 0x01); /* 512Hz, Enable PWM */
    /* enable IRQs again */
    restore_irq(old_irq_level);
}

void remote_backlight_hw_on(void)
{
    and_l(~0x00000002, &GPIO1_OUT);
}

void remote_backlight_hw_off(void)
{
    or_l(0x00000002, &GPIO1_OUT);
}
