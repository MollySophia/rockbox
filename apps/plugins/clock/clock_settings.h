/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 Copyright Kévin Ferrare based on Zakk Roberts's work
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

#ifndef _CLOCK_SETTINGS_
#define _CLOCK_SETTINGS_
#include "plugin.h"

enum date_format{
    NONE,
    ENGLISH,
    EUROPEAN,
    JAPANESE,
};

enum hour_format{
    H24,
    H12
};

enum clock_modes{
    ANALOG,
    DIGITAL,
    BINARY,
    NB_CLOCK_MODES
};

enum backlight_handling{
    ALWAS_OFF,
    ROCKBOX_SETTING,
    ALWAYS_ON
};


struct general_settings{
    int hour_format;/* 0:24h, 1:12h*/
    int date_format;
    bool show_counter;
    bool save_settings;
    bool idle_poweroff;
    int backlight;
};

struct analog_settings{
    bool show_date;
    bool show_seconds;
    bool show_border;
};

struct digital_settings{
    int show_seconds;
    int blinkcolon;
};

struct clock_settings{
    int mode; /* clock mode */
    int skin[NB_CLOCK_MODES];/* how does each mode looks like */
    struct general_settings general;
    struct analog_settings analog;
    struct digital_settings digital;
};

extern struct clock_settings clock_settings;

/* settings are saved to this location */
#define settings_filename PLUGIN_APPS_DIR "/.clock_settings"

void clock_settings_skin_next(struct clock_settings* settings);
void clock_settings_skin_previous(struct clock_settings* settings);
void apply_backlight_setting(int backlight_setting);
void clock_settings_reset(struct clock_settings* settings);
void load_settings(void);
void save_settings(void);
void save_settings_wo_gui(void);

#endif /* _CLOCK_SETTINGS_ */
