/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Daniel Ankers
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
#include "system.h"
#include "cpu.h"
#include "ascodec.h"
#include "tuner.h"
#include "as3514.h"
#include "power.h"
#include "usb.h"

void power_init(void)
{
}

void power_off(void)
{
    char byte;

    /* Send shutdown command to PMU */
    byte = ascodec_read(AS3514_SYSTEM);
    byte &= ~0x1;   
    ascodec_write(AS3514_SYSTEM, byte);

    /* Stop interrupts on both cores */
    disable_interrupt(IRQ_FIQ_STATUS);
    COP_INT_DIS = -1;
    CPU_INT_DIS = -1;

    /* Halt everything and wait for device to power off */
    while (1)
    {
        COP_CTL = 0x40000000;
        CPU_CTL = 0x40000000;
    }
}

unsigned int power_input_status(void)
{
    unsigned int status = POWER_INPUT_NONE;

#if defined(SANSA_E200)
    #define _charger_present() (GPIOB_INPUT_VAL & 0x10)
#elif defined(SANSA_C200)
    #define _charger_present() (GPIOH_INPUT_VAL & 0x2)
#else
    #define _charger_present() 0
#endif
    
    if (_charger_present())
        status = POWER_INPUT_MAIN_CHARGER;

    /* No separate source for USB */

    return status;
}

void ide_power_enable(bool on)
{
    (void)on;
}

#if CONFIG_TUNER

/** Tuner **/
static bool powered = false;

bool tuner_power(bool status)
{
    bool old_status;
    lv24020lp_lock();

    old_status = powered;

    if (status != old_status)
    {
        if (status)
        {
            /* init mystery amplification device */
#if defined(SANSA_E200)
            GPO32_ENABLE |= 0x1;
#else /* SANSA_C200 */
            DEV_INIT2 &= ~0x800;
#endif
            udelay(5);

            /* When power up, host should initialize the 3-wire bus
               in host read mode: */

            /* 1. Set direction of the DATA-line to input-mode. */
            GPIO_CLEAR_BITWISE(GPIOH_OUTPUT_EN, 1 << 5); 
            GPIO_SET_BITWISE(GPIOH_ENABLE, 1 << 5); 

            /* 2. Drive NR_W low */
            GPIO_CLEAR_BITWISE(GPIOH_OUTPUT_VAL, 1 << 3); 
            GPIO_SET_BITWISE(GPIOH_OUTPUT_EN, 1 << 3); 
            GPIO_SET_BITWISE(GPIOH_ENABLE, 1 << 3); 

            /* 3. Drive CLOCK high */
            GPIO_SET_BITWISE(GPIOH_OUTPUT_VAL, 1 << 4); 
            GPIO_SET_BITWISE(GPIOH_OUTPUT_EN, 1 << 4); 
            GPIO_SET_BITWISE(GPIOH_ENABLE, 1 << 4);

            lv24020lp_power(true);
        }
        else
        {
            lv24020lp_power(false);

            /* set all as inputs */
            GPIO_CLEAR_BITWISE(GPIOH_OUTPUT_EN, (1 << 5) | (1 << 3) | (1 << 4));
            GPIO_CLEAR_BITWISE(GPIOH_ENABLE, (1 << 3) | (1 << 4)); 

            /* turn off mystery amplification device */
#if defined (SANSA_E200)
            GPO32_ENABLE &= ~0x1;
#else
            DEV_INIT2 |= 0x800;
#endif
        }

        powered = status;
    }

    lv24020lp_unlock();
    return old_status;
}

#endif /* CONFIG_TUNER */
