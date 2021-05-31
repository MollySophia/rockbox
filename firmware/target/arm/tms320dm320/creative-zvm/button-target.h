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

#ifndef _BUTTON_TARGET_H_
#define _BUTTON_TARGET_H_

#include "config.h"

#define BUTTON_BACK         (1 << 0)
#define BUTTON_CUSTOM       (1 << 1)
#define BUTTON_MENU         (1 << 2)

#define BUTTON_LEFT         (1 << 3)
#define BUTTON_RIGHT        (1 << 4)
#define BUTTON_UP           (1 << 5)
#define BUTTON_DOWN         (1 << 6)
#define BUTTON_SELECT       (1 << 7)

#define BUTTON_POWER        (1 << 8)
#define BUTTON_PLAY         (1 << 9)

#define BUTTON_HOLD         (1 << 10)

#ifdef CREATIVE_ZV
#define BUTTON_PREV         (1 << 11)
#define BUTTON_NEXT         (1 << 12)

#define BUTTON_VOL_UP       (1 << 13)
#define BUTTON_VOL_DOWN     (1 << 14)
#endif

#ifndef CREATIVE_ZV
#define BUTTON_MAIN         ( BUTTON_BACK | BUTTON_MENU | BUTTON_LEFT   | BUTTON_RIGHT \
                            | BUTTON_UP   | BUTTON_DOWN | BUTTON_SELECT | BUTTON_POWER \
                            | BUTTON_PLAY | BUTTON_HOLD | BUTTON_CUSTOM                )
#else
#define BUTTON_MAIN         ( BUTTON_BACK   | BUTTON_MENU     | BUTTON_LEFT   | BUTTON_RIGHT \
                            | BUTTON_UP     | BUTTON_DOWN     | BUTTON_SELECT | BUTTON_POWER \
                            | BUTTON_PLAY   | BUTTON_HOLD     | BUTTON_NEXT   | BUTTON_PREV  \
                            | BUTTON_VOL_UP | BUTTON_VOL_DOWN                                )
#endif

#define POWEROFF_BUTTON     BUTTON_POWER
#define POWEROFF_COUNT      10

#define HAS_BUTTON_HOLD

bool button_usb_connected(void);

#endif /* _BUTTON_TARGET_H_ */
