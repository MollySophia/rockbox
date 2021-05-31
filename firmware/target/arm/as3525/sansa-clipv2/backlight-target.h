/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright © 2009 Rafaël Carré
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
#ifndef BACKLIGHT_TARGET_H
#define BACKLIGHT_TARGET_H

#include <stdbool.h>
#include "cpu.h"

void backlight_hw_on(void);
void backlight_hw_off(void);
#define _backlight_panic_on() backlight_hw_on()

static inline bool backlight_hw_init(void)
{
    GPIOA_DIR |= 1<<5; /* for button light */
    return true;
}

static inline void buttonlight_hw_on(void)
{
    GPIOA_PIN(5) = 1<<5;
}

static inline void buttonlight_hw_off(void)
{
    GPIOA_PIN(5) = 0;
}


#endif
