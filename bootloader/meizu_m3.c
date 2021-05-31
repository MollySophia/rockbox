/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Greg White
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "config.h"

#include "inttypes.h"
#include "cpu.h"
#include "system.h"
#include "lcd.h"
#include "../kernel-internal.h"
#include "storage.h"
#include "disk.h"
#include "font.h"
#include "backlight.h"
#include "backlight-target.h"
#include "button.h"
#include "panic.h"
#include "power.h"
#include "file.h"
#include "common.h"
#include "rbunicode.h"
#include "usb.h"
#include "qt1106.h"
#include "bitmaps/rockboxlogo.h"

#include "i2c-s5l8700.h"
#include "dma-target.h"
#include "pcm.h"
#include "audiohw.h"
#include "rtc.h"
#include "time.h"

#define LONG_DELAY  200000
#define SHORT_DELAY  50000
#define PAUSE_DELAY  50000

static inline void delay(int duration)
{
    volatile int i;
    for(i=0;i<duration;i++);
}


void bl_debug(bool bit)
{
    if (bit)
    {
        PDAT0 ^= (1 << 2); //Toggle backlight
        delay(LONG_DELAY);
        PDAT0 ^= (1 << 2); //Toggle backlight
        delay(LONG_DELAY);
    }
    else
    {
        PDAT0 ^= (1 << 2); //Toggle backlight
        delay(SHORT_DELAY);
        PDAT0 ^= (1 << 2); //Toggle backlight
        delay(SHORT_DELAY);
    }
}

void bl_debug_count(unsigned int input)
{
    unsigned int i;
    delay(SHORT_DELAY*3);
    for (i = 0; i < input; i++)
    {
        PDAT0 ^= (1 << 2); //Toggle backlight
        delay(SHORT_DELAY);
        PDAT0 ^= (1 << 2); //Toggle backlight
        delay(2*SHORT_DELAY);
    }
}
void bl_debug_int(unsigned int input,unsigned int count)
{
    unsigned int i;
    for (i = 0; i < count; i++)
    {
        bl_debug(input>>i & 1);
    }
    delay(SHORT_DELAY*6);
}

void post_mortem_stub(void)
{
}

void main(void)
{
    char mystring[64];
    int i;
    unsigned short data = 0;
    char write_data[2], read_data[16];
    struct tm tm;
    
    //Set backlight pin to output and enable
    int oldval = PCON0;
    PCON0 = ((oldval & ~(3 << 4)) | (1 << 4));
    PDAT0 |= (1 << 2);
    
    // Set codec reset pin inactive
    PCON5 = (PCON5 & ~0xF) | 1;
    PDAT5 &= ~(1 << 0);
    
    //power on
//    oldval = PCON1;
//    PCON1 = ((oldval & ~(0xf << 12)) | (1 << 12));
//    PDAT1|=(1<<3);

    //Set PLAY to EINT4
    oldval = PCON1;
    PCON1 = ((oldval & ~(0xf << 16)) | (2 << 16));

    //Set MENU to EINT0
    oldval = PCON1;
    PCON1 = (oldval & ~(0xf)) | 2;

    // enable external interrupts
    EINTPOL = 0x11;
    INTMSK = 0x11;
    EINTMSK = 0x11;
    asm volatile("msr cpsr_c, #0x13\n\t"); // enable interrupts
        
    system_init();
    kernel_init();

    lcd_init();
    backlight_init(); /* BUGFIX backlight_init MUST BE AFTER lcd_init */
    lcd_update();

    i2c_init();
    
    init_qt1106();

    /* Calibrate the lot */
    qt1106_io(QT1106_MODE_FREE | QT1106_MOD_INF | QT1106_DI \
       | QT1106_SLD_SLIDER | QT1106_CAL_WHEEL | QT1106_CAL_KEYS | QT1106_RES_256);

    lcd_clear_display();
    lcd_bitmap(rockboxlogo, 0, 30, BMPWIDTH_rockboxlogo, BMPHEIGHT_rockboxlogo);
    lcd_update();

    /* Set to maximum sensitivity */
    qt1106_io(QT1106_CT | (0x00 << 8) );

    while(true)
    {
#if 1   /* enable this to see info about the slider touchpad */
        qt1106_wait();

        int slider = qt1106_io(QT1106_MODE_FREE | QT1106_MOD_INF \
            | QT1106_DI | QT1106_SLD_SLIDER | QT1106_RES_256);
        snprintf(mystring, 64, "%x %2.2x",(slider & 0x008000)>>15, slider&0xff);
        lcd_puts(0,1,mystring);
        backlight_hw_brightness((slider & 0xFF) >> 4);
        
        /*
        if(slider & 0x008000)
            bl_debug_count(((slider&0xff)) + 1);
        */
#endif

#if 1   /* enable this to see info about the RTC */
        rtc_read_datetime(&tm);
        snprintf(mystring, 64, "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
        lcd_puts(0, 10, mystring);
#endif 

#if 1   /* enable this so see info about the UDA1380 codec */
        memset(read_data, 0, sizeof(read_data));
        for (i = 0; i < 7; i++) {
            write_data[0] = i;
            i2c_read(0x30, i, 2, read_data);
            data = read_data[0] << 8 | read_data[1];
            snprintf(mystring + 4 * i, 64, "%04X", data);
        }
        lcd_puts(0, 11, mystring);
#endif

#if 1   /* enable this to see info about IODMA channel 0 (PCM) */
        snprintf(mystring, 64, "DMA: %08X %08X", (int)DMACADDR0, (int)DMACTCNT0);
        lcd_puts(0, 12, mystring);
#endif        
#if 1   /* enable this to see info about IIS */
        snprintf(mystring, 64, "IIS: %08X", (int)I2SSTATUS);
        lcd_puts(0, 13, mystring);
#endif        

        lcd_update();
    }

    //power off
    PDAT1&=~(1<<3);
}

