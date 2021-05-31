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

#define HAS_BUTTON_HOLD

/* Main unit's buttons */
#define BUTTON_SELECT     0x00000001
#define BUTTON_VOL_UP     0x00000002
#define BUTTON_VOL_DOWN   0x00000004
#define BUTTON_MENU       0x00000008
#define BUTTON_BACK       0x00000010
#define BUTTON_REWIND     0x00000020
#define BUTTON_FAST_FWD   0x00000040

#define BUTTON_LEFT BUTTON_REWIND
#define BUTTON_RIGHT BUTTON_FAST_FWD

#define BUTTON_MAIN  (BUTTON_SELECT | BUTTON_VOL_UP | BUTTON_VOL_DOWN | BUTTON_MENU \
                      BUTTON_MENU | BUTTON_BACK | BUTTON_REWIND | BUTTON_FAST_FWD)

#define BUTTON_POWER 0 /* HACK */

/* Software power-off */
#define POWEROFF_BUTTON BUTTON_SELECT /* TODO */
#define POWEROFF_COUNT 10

#endif /* BUTTON_TARGET_H */
