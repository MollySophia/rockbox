/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 by Karl Kurbjun
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
#include "system.h"
#include "kernel.h"
#include "timer.h"
#include "thread.h"

void tick_start(unsigned int interval_in_ms)
{
    bitset16(&IO_CLK_MOD2, CLK_MOD2_TMR1); /* enable TIMER1 clock */
    IO_TIMER1_TMMD = CONFIG_TIMER1_TMMD_STOP;
    
    /*  Setup the Prescalar (Divide by 10)
     *  Based on linux/include/asm-arm/arch-integrator/timex.h 
     */
    IO_TIMER1_TMPRSCL = 0x0009;

    /* Setup the Divisor */
    IO_TIMER1_TMDIV = (TIMER_FREQ / (10*1000))*interval_in_ms - 1;

    /* Turn Timer1 to Free Run mode */
    IO_TIMER1_TMMD = CONFIG_TIMER1_TMMD_FREE_RUN;
    
    /* Enable the interrupt */
    bitset16(&IO_INTC_EINT0, INTR_EINT0_TMR1);
}

#ifdef BOOTLOADER
void tick_stop(void)
{
    bitclr16(&IO_CLK_MOD2, CLK_MOD2_TMR1); /* disable TIMER1 clock */
}
#endif

void TIMER1(void) __attribute__ ((section(".icode")));
void TIMER1(void)
{
    IO_INTC_IRQ0 = INTR_IRQ0_TMR1;

    /* Run through the list of tick tasks */
    call_tick_tasks();
}

