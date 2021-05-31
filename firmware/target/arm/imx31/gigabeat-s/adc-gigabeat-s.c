/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by Michael Sevakis
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
#include "mc13783.h"
#include "adc.h"
#include "adc-target.h"
#include "kernel.h"

/* Do this so we may read all channels in a single SPI message */
static const unsigned char reg_array[4] =
{
    MC13783_ADC2,
    MC13783_ADC2,
    MC13783_ADC2,
    MC13783_ADC2,
};

static uint32_t channels[2][4];
static struct semaphore adc_done_signal;
static struct mutex adc_mtx;
static long last_adc_read[2]; /* One for each input group */

/* Read 10-bit ADC channel */
unsigned short adc_read(int channel)
{
    uint32_t data;
    int input_select;

    if ((unsigned)channel >= NUM_ADC_CHANNELS)
        return ADC_READ_ERROR;

    input_select = channel >> 3;

    mutex_lock(&adc_mtx);

    /* Limit the traffic through here */
    if (current_tick != last_adc_read[input_select])
    {
        /* Keep enable, start conversion, increment from channel 0,
         * increment from channel 4 */
        uint32_t adc1 = MC13783_ADEN | MC13783_ASC |
                        (0 << MC13783_ADA1_POS) | (4 << MC13783_ADA2_POS);

        if (input_select == 1)
            adc1 |= MC13783_ADSEL; /* 2nd set of inputs */

        /* Start conversion */
        mc13783_write(MC13783_ADC1, adc1);

        /* Wait for done signal */
        semaphore_wait(&adc_done_signal, TIMEOUT_BLOCK);

        /* Read all 8 channels that are converted - two channels in each
         * word. */
        mc13783_read_regs(reg_array, channels[input_select], 4);

        last_adc_read[input_select] = current_tick;
    }

    data = channels[input_select][channel & 3];

    mutex_unlock(&adc_mtx);

    /* Channels 0-3/8-11 in ADD1, 4-7/12-15 in ADD2 */
    return (channel & 4) ?
        ((data & MC13783_ADD2) >> MC13783_ADD2_POS) :
        ((data & MC13783_ADD1) >> MC13783_ADD1_POS);
}

bool adc_enable_channel(int channel, bool enable)
{
    uint32_t bit, mask;

    switch (channel)
    {
    case ADC_CHARGER_CURRENT:
        mask = MC13783_CHRGICON;
        break;

    case ADC_BATTERY_TEMP:
        mask = MC13783_RTHEN;
        break;

    default:
        return false;
    }

    bit = enable ? mask : 0;

    return mc13783_write_masked(MC13783_ADC0, bit, mask)
                != MC13783_DATA_ERROR;
}

/* ADC conversion complete event - called from PMIC ISR */
void MC13783_EVENT_CB_ADCDONE(void)
{
    semaphore_release(&adc_done_signal);
}

void adc_init(void) 
{
    semaphore_init(&adc_done_signal, 1, 0);
    mutex_init(&adc_mtx);

    /* Init so first reads get data */
    last_adc_read[0] = last_adc_read[1] = current_tick-1;

    /* Enable increment-by-read, turn off extra conversions. */
    mc13783_write(MC13783_ADC0, MC13783_ADINC2 | MC13783_ADINC1);

    /* Enable ADC, set multi-channel mode */
    mc13783_write(MC13783_ADC1, MC13783_ADEN);

    /* Enable ADCDONE event */
    mc13783_write(MC13783_INTERRUPT_STATUS0, MC13783_ADCDONEI);
    mc13783_enable_event(MC13783_INT_ID_ADCDONE, true);
}
