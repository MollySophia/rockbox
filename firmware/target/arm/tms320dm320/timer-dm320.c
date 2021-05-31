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
#include "cpu.h"
#include "system.h"
#include "timer.h"
#include "logf.h"

/* GPB0/TOUT0 should already have been configured as output so that pin
  should not be a functional pin and TIMER0 output unseen there */
void TIMER0(void)  __attribute__ ((section(".icode")));
void TIMER0(void)
{
    IO_INTC_IRQ0 = INTR_IRQ0_TMR0; /* clear TIMER0 interrupt */
    if (pfn_timer != NULL)
        pfn_timer();
}

bool timer_set(long cycles, bool start)
{
    int oldlevel;
    unsigned int divider=cycles, prescaler=0;

    if(cycles<1)
        return false;

    oldlevel = set_irq_level(HIGHEST_IRQ_LEVEL);
    
    bitset16(&IO_CLK_MOD2, CLK_MOD2_TMR0); /* enable TIMER0 clock */

    IO_TIMER0_TMMD = CONFIG_TIMER0_TMMD_STOP;

    if (start && pfn_unregister != NULL)
    {
        pfn_unregister();
        pfn_unregister = NULL;
    }

    oldlevel = disable_irq_save();

    /* Increase prescale values starting from 0 to make the cycle count fit */
    while(divider>65535 && prescaler<1024)
    {
        prescaler++;
        divider=cycles/(prescaler+1);
    }

    IO_TIMER0_TMPRSCL = prescaler;
    IO_TIMER0_TMDIV = divider;

    restore_irq(oldlevel);

    return true;
}

static void stop_timer(void)
{
    /* disable TIMER0 interrupt */
    bitclr16(&IO_INTC_EINT0, INTR_EINT0_TMR0);

    /* clear TIMER0 interrupt */
    IO_INTC_IRQ0 = INTR_IRQ0_TMR0;
    
    IO_TIMER0_TMMD = CONFIG_TIMER0_TMMD_STOP;
    
    /* disable TIMER0 clock */
    bitclr16(&IO_CLK_MOD2, CLK_MOD2_TMR0);
}

bool timer_start(void)
{
    int oldstatus = disable_interrupt_save(IRQ_FIQ_STATUS);
    
    stop_timer();
    
    /* enable TIMER0 clock */
    bitset16(&IO_CLK_MOD2, CLK_MOD2_TMR0);

    /* Turn Timer0 to Free Run mode */
    IO_TIMER0_TMMD = CONFIG_TIMER0_TMMD_FREE_RUN;

    /* enable TIMER0 interrupt */
    bitset16(&IO_INTC_EINT0, INTR_EINT0_TMR0);

    restore_interrupt(oldstatus);

    return true;
}

void timer_stop(void)
{
    int oldstatus = disable_interrupt_save(IRQ_FIQ_STATUS);
    stop_timer();
    restore_interrupt(oldstatus);
}
