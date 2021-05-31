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
#ifndef _ADC_IMX233_H_
#define _ADC_IMX233_H_

#include "adc.h"
#include "powermgmt-target.h"

enum imx233_adc_channel_t
{
    ADC_BATTERY, /* Battery voltage (mV) */
    ADC_DIE_TEMP, /* Die temperature (°C) */
    ADC_VDDIO, /* VDDIO (mV) */
#if IMX233_SUBTARGET >= 3700
    ADC_VDD5V, /* VDD5V (mV) */
#endif
#ifdef IMX233_BATT_TEMP_SENSOR
    ADC_BATT_TEMP, /* Battery temperature (°C) */
#endif
    NUM_ADC_CHANNELS,
};

/* Return channel name */
const char *adc_name(int channel);

#endif
