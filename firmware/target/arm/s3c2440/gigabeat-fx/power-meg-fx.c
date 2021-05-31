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
#include "config.h"
#include "cpu.h"
#include <stdbool.h>
#include "kernel.h"
#include "system.h"
#include "power.h"
#include "pcf50606.h"
#include "backlight.h"
#include "backlight-target.h"
#include "usb.h"

void power_init(void)
{
    /* Initialize IDE power pin */
    GPGCON=( GPGCON&~(1<<23) ) | (1<<22); /* Make the pin an output */
    GPGUP |= 1<<11;  /* Disable pullup in SOC as we are now driving */
    ide_power_enable(true);
    /* Charger detect */
}

unsigned int power_input_status(void)
{
    unsigned int status = POWER_INPUT_NONE;

    /* Is the battery switch ON? */
    if ((GPGDAT & (1 << 9)) == 0)
        status |= POWER_INPUT_BATTERY;

    /* Main or cradle power available? */
    if ((GPFDAT & (1 << 4)) == 0)
        status |= POWER_INPUT_MAIN_CHARGER;

    /* Is the USB cable inserted? */
    if (usb_detect() == USB_INSERTED)
        status |= POWER_INPUT_USB_CHARGER;

    return status;
}

/* Returns true if the unit is charging the batteries. */
bool charging_state(void)
{
    return (GPGDAT & (1 << 8)) ? false : true;
}

void ide_power_enable(bool on)
{
    if (on)
        GPGDAT |= (1 << 11);
    else
        GPGDAT &= ~(1 << 11);
}

bool ide_powered(void)
{
    return (GPGDAT & (1 << 11)) != 0;
}

void power_off(void)
{
    int(*reboot_point)(void);
    reboot_point=(void*)(unsigned char*) 0x00000000;
    /* turn off backlight and wait for 1 second */
    backlight_hw_off();
    buttonlight_hw_off();
    sleep(HZ);
    
    /* Do this to allow the drive to properly reset when player restarts 
     * immediately without running OF shutdown.
     */
    GPGCON&=~0x00300000; 

    /* Rockbox never properly shutdown the player.  When the sleep bit is set
     * the player actually wakes up in some type of "zombie" state 
     * because the shutdown routine is not set up properly.  So far the
     * shutdown routines tried leave the player consuming excess power
     * so we rely on the OF to shut everything down instead. (mmu apears to be
     * reset when the sleep bit is set)
     */  
    CLKCON |=(1<<3);

    reboot_point();
}
