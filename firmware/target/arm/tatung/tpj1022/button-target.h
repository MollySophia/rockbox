/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Robert Kukla
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

#define HAS_BUTTON_HOLD

/* Main unit's buttons */

#define BUTTON_VOL_DOWN     0x00000001

/* bit position in GPIOA */
#define BUTTON_REW          0x00000002
#define BUTTON_FF           0x00000004
#define BUTTON_POWER        0x00000008
#define BUTTON_UP           0x00000010
#define BUTTON_DOWN         0x00000020
#define BUTTON_AB           0x00000040
#define BUTTON_RIGHT        0x00000080

/* still unknown */
#define BUTTON_MENU         0x00000100
#define BUTTON_REC          0x00000200
#define BUTTON_VOL_UP       0x00000400
#define BUTTON_LEFT         0x00000800

#define BUTTON_MAIN         0x00000FFF

#define POWEROFF_BUTTON BUTTON_POWER
#define POWEROFF_COUNT 10

#endif /* _BUTTON_TARGET_H_ */
