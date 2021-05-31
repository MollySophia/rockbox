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
#ifndef BACKLIGHT_TARGET_H
#define BACKLIGHT_TARGET_H

/* Use these to set the buttonlight mode */
enum buttonlight_mode
{
    /* ON follows the setting */
    BUTTONLIGHT_ON,

    /* buttonlights always off */
    BUTTONLIGHT_OFF,

    /* buttonlights follow backlight */
    BUTTONLIGHT_FOLLOW
};

/* Call this to flicker or signal the button lights. Only is effective for
 * modes that take a trigger input.
 */
void __buttonlight_trigger(void);

/* map the mode from the command into the state machine entries */
/* See enum buttonlight_mode for available functions */
void __buttonlight_mode(enum buttonlight_mode mode);

bool backlight_hw_init(void);
void backlight_hw_on(void);
void backlight_hw_off(void);
void backlight_hw_brightness(int brightness);

void buttonlight_hw_brightness(int brightness);
void buttonlight_hw_on(void);
void buttonlight_hw_off(void);
#endif
