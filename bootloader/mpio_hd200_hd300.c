/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010 Marcin Bukat
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

#include <stdlib.h>
#include <stdio.h>
#include "inttypes.h"
#include "string.h"
#include "cpu.h"
#include "system.h"
#include "lcd.h"
#include "../kernel-internal.h"
#include "storage.h"
#include "file_internal.h"
#include "usb.h"
#include "disk.h"
#include "font.h"
#include "adc.h"
#include "backlight.h"
#include "backlight-target.h"
#include "button.h"
#include "panic.h"
#include "power.h"
#include "powermgmt.h"
#include "file.h"

#include "common.h"
#include "rb-loader.h"
#include "loader_strerror.h"
#include "version.h"

#include <stdarg.h>

/* Maximum allowed firmware image size. 10MB is more than enough */
#define MAX_LOADSIZE (10*1024*1024)

#define DRAM_START 0x31000000

#define BOOTMENU_TIMEOUT (10*HZ)
#define BOOTMENU_OPTIONS 3

#define EVENT_NONE 0x00
#define EVENT_ON   0x01
#define EVENT_AC   0x02
#define EVENT_USB  0x04
#define EVENT_RTC  0x08

/* From common.c */
extern int line;
static const char *bootmenu_options[] = {
    "Boot rockbox",
    "Boot MPIO firmware",
    "Shutdown"
};

enum option_t {
    rockbox,
    mpio_firmware,
    shutdown
};

int usb_screen(void)
{
   return 0;
}

/* return true if charger is present */
static inline bool _charger_inserted(void)
{
    return (GPIO1_READ & (1<<14)) ? false : true;
}

/* returns true if end of charge condition is reached */
static inline bool _battery_full(void)
{
    return (GPIO_READ & (1<<30)) ? true : false;
}

#ifdef MPIO_HD300
/* returns true if startup is due to RTC alarm */
static inline bool _rtc_alarm(void)
{
    if ( (GPIO1_READ & (1<<4)) && (GPIO1_READ & (1<<5)) )
        return false;

    return true;
}
#endif

/* Reset the cookie for the crt0 crash check */
static inline void __reset_cookie(void)
{
    asm(" move.l #0,%d0");
    asm(" move.l %d0,0x10017ffc");
}

static void start_rockbox(void)
{
    adc_close();
    asm(" move.w #0x2700,%sr");
    __reset_cookie();
    asm(" move.l %0,%%d0" :: "i"(DRAM_START));
    asm(" movec.l %d0,%vbr");
    asm(" move.l %0,%%sp" :: "m"(*(int *)DRAM_START));
    asm(" move.l %0,%%a0" :: "m"(*(int *)(DRAM_START+4)));
    asm(" jmp (%a0)");
}

static void start_mpio_firmware(void)
{
    asm(" move.w #0x2700,%sr");
    __reset_cookie();
    asm(" movec.l %d0,%vbr");
    asm(" move.l 0,%sp");
    asm(" jmp 8");
}

static void __shutdown(void)
{
    if (_charger_inserted())
    /* if AC power do nothing */
        return;

    /* We need to gracefully spin down the disk to prevent clicks. */
    if (ide_powered())
    {
        /* Make sure ATA has been initialized. */
        storage_init();
        
        /* And put the disk into sleep immediately. */
        storage_sleepnow();
    }

    /* Backlight OFF */
    backlight_hw_off();
    __reset_cookie();

        power_off();
}

/* Print the battery voltage (and a warning message). */
static void check_battery(void)
{

    int battery_voltage, batt_int, batt_frac;
    
    battery_voltage = _battery_voltage();
    batt_int = battery_voltage / 1000;
    batt_frac = (battery_voltage % 1000) / 10;

    printf("Battery: %d.%02dV", batt_int, batt_frac);

    if (battery_voltage <= 3500) 
    {
        printf("WARNING! BATTERY LOW!!");
        sleep(HZ*2);
    }

}


static void lcd_putstring_centered(const char *string)
{
    int w,h;
    font_getstringsize(string, &w, &h, FONT_SYSFIXED);
    lcd_putsxy((LCD_WIDTH-w)/2, (LCD_HEIGHT-h)/2, string);
}

/* This function initializes ATA driver, mounts partitions,
 * loads rockbox image from disk to ram and finally
 * jumps to entry point in ram
 */
static void rb_boot(void)
{
    int rc;

    /* boost to speedup rb image loading */
    cpu_boost(true);

    reset_screen();
    printf("Rockbox boot loader");
    printf("Version %s", rbversion);

    rc = storage_init();
    if(rc)
        error(EATA, rc, true);

    filesystem_init();

    rc = disk_mount_all();
    if (rc <= 0)
        error(EDISK, rc, true);

    printf("Loading firmware");

    rc = load_firmware((unsigned char *)DRAM_START, 
                       BOOTFILE, MAX_LOADSIZE);

    if (rc <= EFILE_EMPTY)
        error(EBOOTFILE, rc, true);

    cpu_boost(false);
    start_rockbox();
}

/* This function prints small bootmenu where
 * you can choose to boot OF, rockbox or just shutdown
 */
static void bootmenu(void)
{
    enum option_t i;
    enum option_t option = rockbox;
    int button;
    const char select[] = "->";
    long start_tick = current_tick;

    /* backbone of menu */
    /* run the loader */
    printf("Rockbox boot loader");
    printf("Ver: %s", rbversion);

    check_battery();

    printf("");
    printf("=========================");

    line += BOOTMENU_OPTIONS+2; /* skip lines */

    printf("=========================");
    printf("");
    printf(" [FF] [REW] to move ");
    printf(" [PLAY] to confirm ");

    /* content of menu and keys handling */
    while (TIME_BEFORE(current_tick,start_tick + BOOTMENU_TIMEOUT))
    {
        /* Draw the menu. */
        line = 6; /* move below header */

        for (i=0;i<BOOTMENU_OPTIONS;i++)
        {
            if (i != option)
                printf("   %s",bootmenu_options[i]);
            else
                printf("%s %s",select,bootmenu_options[i]);
        }

        line = 15;

        printf("Time left: %ds",(BOOTMENU_TIMEOUT - 
                                 (current_tick - start_tick))/HZ);

        lcd_update();

        button = BUTTON_NONE;
        button = button_get_w_tmo(HZ);

        switch (button)
        {
            case BUTTON_REW:
#ifdef MPIO_HD200
            case BUTTON_RC_REW:
#endif
                if (option > rockbox)
                    option--;
                else
                    option = shutdown;
                break;

            case BUTTON_FF:
#ifdef MPIO_HD200
            case BUTTON_RC_FF:
#endif
                if (option < shutdown)
                    option++;
                else
                    option = rockbox;
                break;

            case BUTTON_PLAY:
#ifdef MPIO_HD200
            case BUTTON_RC_PLAY:
            case (BUTTON_PLAY|BUTTON_REC):
#endif
                reset_screen();

                switch (option)
                {
                    case rockbox:
                            rb_boot();
                        break;

                    case mpio_firmware:
                        start_mpio_firmware();
                        break;

                    default:
                        __shutdown();
                        break;
                }
        }
}
/* timeout */
}

void main(void)
{
    /* messages */
    const char usb_connect_msg[] = "Bootloader USB mode";
    const char charging_msg[] = "Charging...";
    const char complete_msg[] = "Charging complete";

    /* helper variable for messages */
    bool blink_toggle = false;

    int button;

    /* hold status variables
     * this two must have different
     * values in the begining
     */
    bool hold = false;
    bool last_hold = true;

    unsigned int event = EVENT_NONE;
    unsigned int last_event = EVENT_NONE;

    /* this is default mode after power_init() */
    bool high_current_charging = true;

    /* setup GPIOs related to power functions */
    power_init();

    system_init();
    kernel_init();

    /* run at 45MHz */
    set_cpu_frequency(CPUFREQ_NORMAL);

    /* IRQs are needed by button driver */
    enable_irq();

    lcd_init();

    /* setup font system */
    font_init();
    lcd_setfont(FONT_SYSFIXED);

     /* buttons reading init */
    adc_init();
    button_init();

    usb_init();
    cpu_idle_mode(true);

    /* lowlevel init only */
    backlight_hw_init();

    /* Handle wakeup event. Possibilities are:
     * RTC alarm (HD300)
     * ON button (PLAY or RC_PLAY on HD200)
     * USB insert
     * AC charger plug
     */
    while(1)
    {
        /* check hold status */
        hold = button_hold();

        /* backlight handling
         * change only on hold toggle */
        if ( hold != last_hold )
        {
            if ( hold )
                backlight_hw_off();
            else
                backlight_hw_on();

            last_hold = hold;
        }

        /* read buttons */
        event = EVENT_NONE;
        button = button_get_w_tmo(HZ);

        if ( (button & BUTTON_PLAY)
#ifdef MPIO_HD200
             || (button & BUTTON_RC_PLAY)
#endif
           )
            event |= EVENT_ON;
 
        if ( usb_detect() == USB_INSERTED )
            event |= EVENT_USB;

        if ( _charger_inserted() )
            event |= EVENT_AC;
#ifdef MPIO_HD300
        if ( _rtc_alarm() )
            event |= EVENT_RTC;
#endif

        reset_screen();
        switch (event)
        {
#ifdef MPIO_HD300
            case EVENT_RTC:
            case (EVENT_RTC | EVENT_ON):
            /* start regardles of buttons state */
                rb_boot();
                break;
#endif
            case EVENT_ON:
            case (EVENT_ON | EVENT_AC):
            /* hold is handled in button driver */
                cpu_idle_mode(false);
                ide_power_enable(true);

                if (button & BUTTON_REC)
                    bootmenu();
                else
                    rb_boot();

                break;

            case EVENT_AC:
                /* AC plug in */
                if (!(last_event & EVENT_AC))
                {
                    /* reset charging circuit */
                    and_l(~(1<<23), &GPIO_ENABLE);
                }

                /* USB unplug */
                if (last_event & EVENT_USB)
                {
                    usb_enable(false);
                    sleep(HZ);
                    ide_power_enable(false);
                    sleep(HZ);
                }
                   
                if(!_battery_full())
                {
                    if (blink_toggle)
                        lcd_putstring_centered(charging_msg);

                    blink_toggle = !blink_toggle;
                }
                else /* end of charge condition */
                {
                    /* put LTC1733 into shutdown mode */
                    or_l((1<<23), &GPIO_ENABLE);

                    if (high_current_charging)
                    {
                        /* switch to low current mode */
                        and_l(~(1<<15), &GPIO_OUT);

                        /* reset charging circuit */
                        and_l(~(1<<23), &GPIO_ENABLE);

                        high_current_charging = false;
                    }
                    else
                    {
                        lcd_putstring_centered(complete_msg);
                    }
                }
                check_battery();
                break;

            case EVENT_USB:
            case (EVENT_USB | EVENT_AC):
                /* AC plug in while in USB mode */
                if (!(last_event & EVENT_AC))
                {
                    /* reset charger circuit */
                    and_l(~(1<<23), &GPIO_ENABLE);
                }

                /* USB plug in */
                if (!(last_event & EVENT_USB))
                {
                    /* init USB */
                    ide_power_enable(true);
                    sleep(HZ/20);
                    ata_enable(false);
                    usb_enable(true);
                }
                
                /* display blinking USB indicator */
                line = 0;

                if (blink_toggle)
                    lcd_putstring_centered(usb_connect_msg);

                check_battery();
                blink_toggle = !blink_toggle;
                storage_spin();
                break;

            default:
                /* USB unplug */
                if (last_event & EVENT_USB)
                {
                    /* disable USB */
                    usb_enable(false);
                    ata_enable(true);
                    sleep(HZ);
                    ide_power_enable(false);
                    sleep(HZ);
                }

                /* spurious wakeup ?*/
                __shutdown();
                break;
        }
        lcd_update();
        last_event = event;
    }

}

/* These functions are present in the firmware library, but we reimplement
   them here because the originals do a lot more than we want */
void screen_dump(void)
{
}
