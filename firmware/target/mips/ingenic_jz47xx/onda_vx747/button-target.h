/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by Maurus Cuelenaere
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
#ifndef BUTTON_TARGET_H
#define BUTTON_TARGET_H

#include "config.h"

#ifndef ONDA_VX777
#define HAS_BUTTON_HOLD
#endif

void button_set_touch_available(void);

/* Main unit's buttons */
#define BUTTON_POWER        0x00000001
#define BUTTON_VOL_UP       0x00000002
#define BUTTON_VOL_DOWN     0x00000004
#define BUTTON_MENU         0x00000008

/* Compatibility hacks for flipping. Needs a somewhat better fix. */
#define BUTTON_LEFT         BUTTON_MIDLEFT
#define BUTTON_RIGHT        BUTTON_MIDRIGHT
#define BUTTON_UP           BUTTON_TOPMIDDLE
#define BUTTON_DOWN         BUTTON_BOTTOMMIDDLE

/* Touch Screen Area Buttons */
#define BUTTON_TOPLEFT      0x00000010
#define BUTTON_TOPMIDDLE    0x00000020
#define BUTTON_TOPRIGHT     0x00000040
#define BUTTON_MIDLEFT      0x00000080
#define BUTTON_CENTER       0x00000100
#define BUTTON_MIDRIGHT     0x00000200
#define BUTTON_BOTTOMLEFT   0x00000400
#define BUTTON_BOTTOMMIDDLE 0x00000800
#define BUTTON_BOTTOMRIGHT  0x00001000

#define DEFAULT_TOUCHSCREEN_CALIBRATION {.A=0xFFF9FDA2, .B=0xFFFFE82A, \
                                         .C=0xA22AA2C,  .D=0x23DC, .E=0x8E3E6, \
                                         .F=0x76CF88AA, .divider=0xFFAD4013}

#ifdef ONDA_VX777
#define BUTTON_MAIN (BUTTON_POWER    | BUTTON_TOPLEFT    | BUTTON_TOPMIDDLE    | \
                     BUTTON_TOPRIGHT | BUTTON_MIDLEFT    | BUTTON_CENTER       | \
                     BUTTON_MIDRIGHT | BUTTON_BOTTOMLEFT | BUTTON_BOTTOMMIDDLE | \
                     BUTTON_BOTTOMRIGHT)
#else
#define BUTTON_MAIN (BUTTON_POWER    | BUTTON_VOL_UP     | BUTTON_VOL_DOWN     | \
                     BUTTON_MENU     | BUTTON_TOPLEFT    | BUTTON_TOPMIDDLE    | \
                     BUTTON_TOPRIGHT | BUTTON_MIDLEFT    | BUTTON_CENTER       | \
                     BUTTON_MIDRIGHT | BUTTON_BOTTOMLEFT | BUTTON_BOTTOMMIDDLE | \
                     BUTTON_BOTTOMRIGHT)
#endif

/* Software power-off */
#define POWEROFF_BUTTON BUTTON_POWER
#define POWEROFF_COUNT  10

#endif /* BUTTON_TARGET_H */
