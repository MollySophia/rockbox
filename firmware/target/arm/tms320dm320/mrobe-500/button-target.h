/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 by Jonathan Gordon
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
 
#ifndef _BUTTON_TARGET_H_
#define _BUTTON_TARGET_H_

#include <stdbool.h>

#define HAS_BUTTON_HOLD

/* This is called from the tsc2100 interupt handler in adc-mr500.c */
void touch_read_coord(void);

struct touch_calibration_point {
    short px_x; /* known pixel value */
    short px_y;
    short val_x; /* touchscreen value at the known pixel */
    short val_y;
};
void use_calibration(bool enable);

/* M:Robe 500 specific button codes */

#define BUTTON_POWER        0x00000100

/* Remote control buttons */

#define BUTTON_RC_PLAY      0x00000001
#define BUTTON_RC_DOWN      0x00000002
#define BUTTON_RC_FF        0x00000004
#define BUTTON_RC_REW       0x00000008
#define BUTTON_RC_VOL_UP    0x00000010
#define BUTTON_RC_VOL_DOWN  0x00000020
#define BUTTON_RC_MODE      0x00000040
#define BUTTON_RC_HEART     0x00000080

#define BUTTON_TOUCH        0x00000200

/* Touch Screen Area Buttons */
#define BUTTON_TOPLEFT      0x00004000
#define BUTTON_TOPMIDDLE    0x00008000
#define BUTTON_TOPRIGHT     0x00010000
#define BUTTON_MIDLEFT      0x00020000
#define BUTTON_CENTER       0x00040000
#define BUTTON_MIDRIGHT     0x00080000
#define BUTTON_BOTTOMLEFT   0x00100000
#define BUTTON_BOTTOMMIDDLE 0x00200000
#define BUTTON_BOTTOMRIGHT  0x00400000

/* compatibility hacks
   not mapped to the touchscreen button areas because
   the touchscreen is not always in that mode */
#define BUTTON_LEFT     BUTTON_RC_REW
#define BUTTON_RIGHT    BUTTON_RC_FF

#define POWEROFF_BUTTON BUTTON_POWER
#define POWEROFF_COUNT  10

#define BUTTON_MAIN (BUTTON_POWER| \
                     BUTTON_TOPLEFT|BUTTON_TOPMIDDLE|BUTTON_TOPRIGHT| \
                     BUTTON_MIDLEFT|BUTTON_CENTER|BUTTON_MIDRIGHT| \
                     BUTTON_BOTTOMLEFT|BUTTON_BOTTOMMIDDLE|BUTTON_BOTTOMRIGHT)

#define BUTTON_REMOTE (BUTTON_RC_HEART|BUTTON_RC_MODE|      \
                       BUTTON_RC_VOL_DOWN|BUTTON_RC_VOL_UP| \
                       BUTTON_RC_PLAY|BUTTON_RC_DOWN|       \
                       BUTTON_RC_REW|BUTTON_RC_FF)

#endif /* _BUTTON_TARGET_H_ */
