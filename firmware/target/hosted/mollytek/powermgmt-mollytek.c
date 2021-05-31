/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 * Copyright (C) 2017 Marcin Bukat
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
#include "powermgmt.h"
#include "power.h"
#include "power-mollytek.h"

const unsigned short battery_level_dangerous[BATTERY_TYPES_COUNT] = {
    3470
};

const unsigned short battery_level_shutoff[BATTERY_TYPES_COUNT] = {
    3400
};

/* voltages (millivolt) of 0%, 10%, ... 100% when charging disabled */
const unsigned short percent_to_volt_discharge[BATTERY_TYPES_COUNT][11] =
{
    { 3400, 3639, 3697, 3723, 3757, 3786, 3836, 3906, 3980, 4050, 4159 }
};

/* voltages (millivolt) of 0%, 10%, ... 100% when charging enabled */
const unsigned short const percent_to_volt_charge[11] =
{
      3485, 3780, 3836, 3857, 3890, 3930, 3986, 4062, 4158, 4185, 4196
};

unsigned int power_input_status(void) {
    return mollytek_power_input_status();
}

bool charging_state(void) {
    return mollytek_power_charging_status();
}

int _battery_level(void) {
    return mollytek_power_get_battery_capacity();
}