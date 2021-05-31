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
#include "rtc-imx233.h"
#include "string.h"

static void imx233_rtc_write_reg(volatile uint32_t *reg, uint32_t val)
{
    while(BF_RD(RTC_STAT, NEW_REGS) != 0);
    *reg = val;
    while(BF_RD(RTC_STAT, NEW_REGS) != 0);
}

void imx233_rtc_write_seconds(uint32_t seconds)
{
    imx233_rtc_write_reg(&HW_RTC_SECONDS, seconds);
}

void imx233_rtc_write_persistent(int idx, uint32_t val)
{
    imx233_rtc_write_reg(&HW_RTC_PERSISTENTn(idx), val);
}

void imx233_rtc_write_alarm(uint32_t seconds)
{
    imx233_rtc_write_reg(&HW_RTC_ALARM, seconds);
}

struct imx233_rtc_info_t imx233_rtc_get_info(void)
{
    struct imx233_rtc_info_t info;
    memset(&info, 0, sizeof(info));
    info.seconds = HW_RTC_SECONDS;
    for(int i = 0; i < 6; i++)
        info.persistent[i] = HW_RTC_PERSISTENTn(i);
    info.alarm = imx233_rtc_read_alarm();
    info.alarm_en = BF_RD(RTC_PERSISTENT0, ALARM_EN);
    info.alarm_wake_en = BF_RD(RTC_PERSISTENT0, ALARM_WAKE_EN);
    info.alarm_wake = BF_RD(RTC_PERSISTENT0, ALARM_WAKE);
    info.alarm_irq = BF_RD(RTC_CTRL, ALARM_IRQ);
    return info;
}
