/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2011 by Amaury Pouly
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
bool button_debug_screen(void);
void touchpad_set_sensitivity(int level);
void touchpad_set_deadzone(int touchpad_deadzone);
void touchpad_enable_device(bool en);

/* Main unit's buttons */
#define BUTTON_POWER                0x00000001
#define BUTTON_VOL_UP               0x00000002
#define BUTTON_VOL_DOWN             0x00000004
/* Virtual buttons */
#define BUTTON_LEFT                 0x00000008
#define BUTTON_UP                   0x00000010
#define BUTTON_RIGHT                0x00000020
#define BUTTON_DOWN                 0x00000040
#define BUTTON_SELECT               0x00000080
#define BUTTON_PLAYPAUSE            0x00000100
#define BUTTON_BACK                 0x00000200
#define BUTTON_BOTTOMLEFT           0x00000400
#define BUTTON_BOTTOMRIGHT          0x00000800


#define BUTTON_MAIN (BUTTON_VOL_UP|BUTTON_VOL_DOWN|BUTTON_POWER|BUTTON_LEFT| \
                     BUTTON_UP|BUTTON_RIGHT|BUTTON_DOWN|BUTTON_SELECT| \
                     BUTTON_PLAYPAUSE|BUTTON_BACK| \
                     BUTTON_BOTTOMRIGHT|BUTTON_BOTTOMLEFT)

#define BUTTON_TOUCHPAD (BUTTON_LEFT|BUTTON_UP|BUTTON_RIGHT|BUTTON_DOWN| \
                         BUTTON_SELECT|BUTTON_PLAYPAUSE|BUTTON_BACK| \
                         BUTTON_BOTTOMRIGHT|BUTTON_BOTTOMLEFT)

/* Software power-off */
#define POWEROFF_BUTTON BUTTON_POWER
#define POWEROFF_COUNT 10

#endif /* _BUTTON_TARGET_H_ */
