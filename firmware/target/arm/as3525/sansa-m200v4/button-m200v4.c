/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by ??
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
#include "button.h"

#ifndef BOOTLOADER
#include "backlight.h"
#endif

void button_init_device(void)
{
    GPIOA_DIR &= ~((1<<3) | (1<<2) | (1<<1) | (1<<0));  /* A3-A0 is input */
    GPIOA_DIR |= ((1<<6) | (1<<5) | (1<<4)); /* A4-A6 row outputs */
}

/* short delay is needed between raising a colum pin, and reading the row pin. values is arbitraty */
static inline void btn_delay(void) { int i = 5; while(i--) ; }

int button_read_device(void)
{
    int result = BUTTON_NONE;
    if(button_hold())
        return result;
    
    /* direct GPIO connections */
    if (GPIOA_PIN(3))
        result |= BUTTON_POWER;

    /* This is a keypad using A4-A6 as columns and A0-A2 as rows */
    GPIOA_PIN(4) = (1<<4);
    btn_delay();
   
    /* A4A0 is unused */

    if (GPIOA_PIN(1))
        result |= BUTTON_VOL_DOWN;

    if (GPIOA_PIN(2))
        result |= BUTTON_UP;

    GPIOA_PIN(4) = 0x00;  
    
    GPIOA_PIN(5) = (1<<5);
    btn_delay();
    
    if (GPIOA_PIN(0))
        result |= BUTTON_LEFT;

    if (GPIOA_PIN(1))
        result |= BUTTON_SELECT;

    if (GPIOA_PIN(2))
        result |= BUTTON_RIGHT;

    GPIOA_PIN(5) = 0x00;


    GPIOA_PIN(6) = (1<<6);
    btn_delay();
    
    if (GPIOA_PIN(0))
        result |= BUTTON_DOWN;

    if (GPIOA_PIN(1))
        result |= BUTTON_VOL_UP;

    /* hold button is read in button_hold() */
        
    GPIOA_PIN(6) = 0x00;

    return result;
}

bool button_hold(void)
{
#ifndef BOOTLOADER
    static bool hold_button_old = false;
#endif
    bool hold_button = false;
    
    GPIOA_PIN(6) = (1<<6);
    if (GPIOA_PIN(2))
        hold_button = true;
    GPIOA_PIN(6) = 0x00; 

#ifndef BOOTLOADER
    /* light handling */
    if (hold_button != hold_button_old)
    {
        hold_button_old = hold_button;
        backlight_hold_changed(hold_button);
    }
#endif /* BOOTLOADER */

    return hold_button;
}
