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
#ifndef _BUTTON_TARGET_H_
#define _BUTTON_TARGET_H_

#include "config.h"

#define HAS_BUTTON_HOLD

#ifdef BOOTLOADER
#define BUTTON_DRIVER_CLOSE
#endif

void button_close_device(void);
void headphone_init(void);
void button_headphone_set(int button);

/* Toshiba Gigabeat S-specific button codes */

/* These shifts are selected to optimize scanning of the keypad port */
#define BUTTON_LEFT         (1 << 0)
#define BUTTON_UP           (1 << 1)
#define BUTTON_DOWN         (1 << 2)
#define BUTTON_RIGHT        (1 << 3)
#define BUTTON_SELECT       (1 << 4)
#define BUTTON_BACK         (1 << 5)
#define BUTTON_MENU         (1 << 6)
#define BUTTON_VOL_UP       (1 << 7)
#define BUTTON_VOL_DOWN     (1 << 8)
#define BUTTON_PREV         (1 << 9)
#define BUTTON_PLAY         (1 << 10)
#define BUTTON_NEXT         (1 << 11)
#define BUTTON_POWER        (1 << 12) /* Read from PMIC */

#define BUTTON_MAIN         (0x00001fff)

/* Remote control buttons */
#define BUTTON_RC_VOL_UP    (1 << 13)
#define BUTTON_RC_VOL_DOWN  (1 << 14)
#define BUTTON_RC_FF        (1 << 15)
#define BUTTON_RC_REW       (1 << 16)
#define BUTTON_RC_PLAY      (1 << 17)
#define BUTTON_RC_DSP       (1 << 18)

#define BUTTON_REMOTE       (0x0007e000)

#define POWEROFF_BUTTON     BUTTON_POWER
#define POWEROFF_COUNT      10

#endif /* _BUTTON_TARGET_H_ */
