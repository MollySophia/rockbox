/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright © 2009 Bertrik Sikken
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
#include <stdbool.h>
#include "config.h"
#include "inttypes.h"
#include "s5l8700.h"
#include "power.h"
#include "panic.h"
#include "pmu-target.h"
#include "usb_core.h"   /* for usb_charging_maxcurrent_change */

void power_off(void)
{
    pmu_ldo_on_in_standby(0, 0);
    pmu_ldo_on_in_standby(1, 0);
    pmu_ldo_on_in_standby(2, 0);
    pmu_ldo_on_in_standby(3, 0);
    pmu_ldo_on_in_standby(4, 0);
    pmu_ldo_on_in_standby(5, 0);
    pmu_ldo_on_in_standby(6, 0);
    pmu_ldo_on_in_standby(7, 0);
    pmu_set_wake_condition(0x42); /* USB inserted or EXTON1 */
    pmu_enter_standby();

    while(1);
}

void power_init(void)
{
    pmu_write(0x1e, 0xf);         /* Vcore = 1.000V */
    pmu_ldo_set_voltage(2, 0x13); /* LCD   = 2.800V */
}

#if CONFIG_CHARGING

#ifdef HAVE_USB_CHARGING_ENABLE
void usb_charging_maxcurrent_change(int maxcurrent)
{
    bool on = (maxcurrent >= 500);
    PDAT11 = (PDAT11 & ~1) | (on ? 1 : 0);
}
#endif

unsigned int power_input_status(void)
{
    /* This checks if USB Vbus is present. */
    if (!(PDAT14 & 0x8)) return POWER_INPUT_USB_CHARGER;

    /* If USB Vbus is not present, check if we have a positive power balance
       regardless. This would indicate FireWire charging. Note that this will
       drop to POWER_INPUT_NONE if FireWire isn't able to supply enough current
       for device operation, e.g. during disk spinup. */
    if (PDAT11 & 0x20) return POWER_INPUT_NONE;

    /* Looks like we have FireWire power. */
    return POWER_INPUT_MAIN_CHARGER;
}

bool charging_state(void)
{
    return (PDAT11 & 0x10) ? 0 : 1;
}
#endif /* CONFIG_CHARGING */
