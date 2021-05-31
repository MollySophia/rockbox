/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 by Mark Arigo
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
#include <sys/types.h> /* off_t */

#include "config.h"
#include "cpu.h"
#include "lcd.h"
#include "kernel.h"
#include "system.h"
#ifdef HAVE_LCD_SHUTDOWN
#include "backlight-target.h"  /* included for backlight_hw_off() prototype */
#endif

/* Display status */
static unsigned lcd_yuv_options SHAREDBSS_ATTR = 0;

#if defined(HAVE_LCD_ENABLE) || defined(HAVE_LCD_SLEEP)
static bool is_lcd_enabled = true;
#endif

/* NOTE: the LCD is rotated 90 degrees */

/* LCD command set for Samsung S6B33B2 */

#define R_NOP                  0x00
#define R_OSCILLATION_MODE     0x02
#define R_DRIVER_OUTPUT_MODE   0x10
#define R_DCDC_SET             0x20
#define R_BIAS_SET             0x22
#define R_DCDC_CLOCK_DIV       0x24
#define R_DCDC_AMP_ONOFF       0x26
#define R_TEMP_COMPENSATION    0x28
#define R_CONTRAST_CONTROL1    0x2a
#define R_CONTRAST_CONTROL2    0x2b
#define R_STANDBY_OFF          0x2c
#define R_STANDBY_ON           0x2d
#define R_DDRAM_BURST_OFF      0x2e
#define R_DDRAM_BURST_ON       0x2f
#define R_ADDRESSING_MODE      0x30
#define R_ROW_VECTOR_MODE      0x32
#define R_N_LINE_INVERSION     0x34
#define R_FRAME_FREQ_CONTROL   0x36
#define R_RED_PALETTE          0x38
#define R_GREEN_PALETTE        0x3a
#define R_BLUE_PALETTE         0x3c
#define R_ENTRY_MODE           0x40
#define R_X_ADDR_AREA          0x42
#define R_Y_ADDR_AREA          0x43
#define R_RAM_SKIP_AREA        0x45
#define R_DISPLAY_OFF          0x50
#define R_DISPLAY_ON           0x51
#define R_SPEC_DISPLAY_PATTERN 0x53
#define R_PARTIAL_DISPLAY_MODE 0x55
#define R_PARTIAL_START_LINE   0x56
#define R_PARTIAL_END_LINE     0x57
#define R_AREA_SCROLL_MODE     0x59
#define R_SCROLL_START_LINE    0x5a
#define R_DATA_FORMAT_SELECT   0x60


/* wait for LCD */
static inline void lcd_wait_write(void)
{
    while (LCD1_CONTROL & LCD1_BUSY_MASK);
}

/* send LCD data */
static void lcd_send_data(unsigned data)
{
    lcd_wait_write();
    LCD1_DATA = data >> 8;
    lcd_wait_write();
    LCD1_DATA = data & 0xff;
}

/* send LCD command */
static void lcd_send_command(unsigned cmd)
{
    lcd_wait_write();
    LCD1_CMD = cmd;
}

/* LCD init */
void lcd_init_device(void)
{
#if 0
    /* This is the init sequence from the yh820 OF bootloader */
    unsigned long tmp;

    DEV_INIT1 &= ~0x3000;
    tmp = DEV_INIT1;
    DEV_INIT1 = tmp;
    DEV_INIT2 &= ~0x400;
    
    LCD1_CONTROL &= ~0x4;
    udelay(15);
    LCD1_CONTROL |= 0x4;

    LCD1_CONTROL = 0x680;
    LCD1_CONTROL = 0x684;

    LCD1_CONTROL |= 0x1;
    udelay(200);

    lcd_send_command(R_STANDBY_OFF);
    udelay(100000);

    lcd_send_command(R_DISPLAY_OFF);
    udelay(10000);

    lcd_send_command(R_OSCILLATION_MODE);
    lcd_send_command(0x01);
    udelay(30000);

    lcd_send_command(R_DCDC_SET);
    lcd_send_command(0x01);
    udelay(30000);

    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x01);
    udelay(30000);

    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x09);
    udelay(30000);

    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x0b);
    udelay(30000);

    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x0f);
    udelay(30000);

    lcd_send_command(R_DCDC_CLOCK_DIV);
    lcd_send_command(0x03);
    udelay(10000);

    lcd_send_command(R_CONTRAST_CONTROL1);
    lcd_send_command(0x89);
    udelay(10000);

    lcd_send_command(R_TEMP_COMPENSATION);
    lcd_send_command(0x01);
    udelay(10000);

    lcd_send_command(R_ADDRESSING_MODE);
    lcd_send_command(0x19);
    udelay(10000);

    lcd_send_command(R_ROW_VECTOR_MODE);
    lcd_send_command(0x00);
    udelay(10000);

    lcd_send_command(R_N_LINE_INVERSION);
    lcd_send_command(0x8b);
    udelay(10000);

    lcd_send_command(R_ENTRY_MODE);
    lcd_send_command(0x01);

    lcd_send_command(R_DRIVER_OUTPUT_MODE);
    lcd_send_command(0x34);
    udelay(10000);

    lcd_send_command(R_X_ADDR_AREA);         /* vertical dimensions */
    lcd_send_command(0x00);                  /* y1 */
    lcd_send_command(LCD_HEIGHT - 1);        /* y2 */
    
    lcd_send_command(R_Y_ADDR_AREA);         /* horizontal dimensions */
    lcd_send_command(0x04);                  /* x1 + 4 */
    lcd_send_command(LCD_WIDTH - 1 + 4);     /* x2 + 4 */
    udelay(100);

    lcd_send_command(R_BIAS_SET);
    lcd_send_command(0x01);

    lcd_send_command(R_DDRAM_BURST_OFF);
    udelay(100);

    lcd_send_command(R_DISPLAY_ON);
    udelay(30000);
#endif
}

/*** hardware configuration ***/
int lcd_default_contrast(void)
{
    return DEFAULT_CONTRAST_SETTING;
}

void lcd_set_contrast(int val)
{
    lcd_send_command(R_CONTRAST_CONTROL1);
    lcd_send_command(val);
}

void lcd_set_invert_display(bool yesno)
{
    lcd_send_command(R_SPEC_DISPLAY_PATTERN);
    lcd_send_command(yesno);
}

#if defined(HAVE_LCD_ENABLE)
void lcd_enable(bool yesno)
{
    if (yesno == is_lcd_enabled)
        return;

    if ((is_lcd_enabled = yesno))
    {
        lcd_send_command(R_STANDBY_OFF);
        lcd_send_command(R_DISPLAY_ON);
        send_event(LCD_EVENT_ACTIVATION, NULL);
    }
    else
    {
        lcd_send_command(R_STANDBY_ON);
    }
}
#endif

#ifdef HAVE_LCD_SLEEP
void lcd_sleep(void)
{
    if (is_lcd_enabled)
    {
        is_lcd_enabled = false;
        lcd_send_command(R_STANDBY_ON);
    }
}

void lcd_awake(void)
{
    if (!is_lcd_enabled)
    {
        is_lcd_enabled = true;
        lcd_send_command(R_STANDBY_OFF);
        lcd_send_command(R_DISPLAY_ON);
        send_event(LCD_EVENT_ACTIVATION, NULL);
    }
}
#endif

#if defined(HAVE_LCD_ENABLE) || defined(HAVE_LCD_SLEEP)
bool lcd_active(void)
{
    return is_lcd_enabled;
}
#endif

#if defined(HAVE_LCD_SHUTDOWN)
void lcd_shutdown(void)
{
    backlight_hw_off();
#ifndef HAVE_LCD_ENABLE
    /* already done by backlight_hw_off() */
    lcd_send_command(R_STANDBY_ON);
#endif
}
#endif

#ifdef HAVE_LCD_FLIP
/* turn the display upside down (call lcd_update() afterwards) */
/* Note: since the lcd is rotated, this will flip horiz instead of vert */
void lcd_set_flip(bool yesno)
{
    lcd_send_command(R_DRIVER_OUTPUT_MODE);
    lcd_send_command(yesno ? 0x30 : 0x34);
}
#endif


/*** update functions ***/

void lcd_yuv_set_options(unsigned options)
{
    lcd_yuv_options = options;
}

/* Line write helper function for lcd_yuv_blit. Write two lines of yuv420. */
extern void lcd_write_yuv420_lines(unsigned char const * const src[3],
                                   int width,
                                   int stride);
extern void lcd_write_yuv420_lines_odither(unsigned char const * const src[3],
                                           int width,
                                           int stride,
                                           int x_screen, /* To align dither pattern */
                                           int y_screen);
/* Performance function to blit a YUV bitmap directly to the LCD */
void lcd_blit_yuv(unsigned char * const src[3],
                  int src_x, int src_y, int stride,
                  int x, int y, int width, int height)
{
    unsigned char const * yuv_src[3];
    off_t z;

    /* Sorry, but width and height must be >= 2 or else */
    width &= ~1;
    height >>= 1;
    
    z = stride*src_y;
    yuv_src[0] = src[0] + z + src_x;
    yuv_src[1] = src[1] + (z >> 2) + (src_x >> 1);
    yuv_src[2] = src[2] + (yuv_src[1] - src[1]);

    lcd_send_command(R_ENTRY_MODE);
    lcd_send_command(0x03);

    lcd_send_command(R_Y_ADDR_AREA);
    lcd_send_command(x + 4);
    lcd_send_command(x + width - 1 + 4);

    if (lcd_yuv_options & LCD_YUV_DITHER)
    {
        do
        {
            lcd_send_command(R_X_ADDR_AREA);
            lcd_send_command(y);
            lcd_send_command(y + 1);

            lcd_write_yuv420_lines_odither(yuv_src, width, stride, x, y);
            yuv_src[0] += stride << 1; /* Skip down two luma lines */
            yuv_src[1] += stride >> 1; /* Skip down one chroma line */
            yuv_src[2] += stride >> 1;
            y += 2;
        }
         while (--height > 0);
    }
    else
    {
        do
        {
            lcd_send_command(R_X_ADDR_AREA);
            lcd_send_command(y);
            lcd_send_command(y + 1);

            lcd_write_yuv420_lines(yuv_src, width, stride);
            yuv_src[0] += stride << 1; /* Skip down two luma lines */
            yuv_src[1] += stride >> 1; /* Skip down one chroma line */
            yuv_src[2] += stride >> 1;
            y += 2;
        }
         while (--height > 0);
    }
}

/* Update the display.
   This must be called after all other LCD functions that change the display. */
void lcd_update(void)
{
    lcd_update_rect(0, 0, LCD_WIDTH, LCD_HEIGHT);
}

/* Update a fraction of the display. */
void lcd_update_rect(int x, int y, int width, int height)
{
    const fb_data *addr;

#if defined(HAVE_LCD_ENABLE) || defined(HAVE_LCD_SLEEP)
    if (!is_lcd_enabled)
        return;
#endif
    
    if (x + width >= LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height >= LCD_HEIGHT)
        height = LCD_HEIGHT - y;
        
    if ((width <= 0) || (height <= 0))
        return; /* Nothing left to do. */

    addr = FBADDR(x,y);

    if (width <= 1) {                    
        lcd_send_command(R_ENTRY_MODE);  /* The X end address must be larger */
        lcd_send_command(0x03);          /* that the X start address, so we */
        lcd_send_command(R_Y_ADDR_AREA); /* switch to vertical mode for */
        lcd_send_command(x + 4);         /* single column updates and set */
        lcd_send_command(x + 1 + 4);     /* the window width to 2 */
    } else {
        lcd_send_command(R_ENTRY_MODE);
        lcd_send_command(0x01);
        lcd_send_command(R_Y_ADDR_AREA);
        lcd_send_command(x + 4);
        lcd_send_command(x + width - 1 + 4);
    }

    lcd_send_command(R_X_ADDR_AREA);
    lcd_send_command(y);
    lcd_send_command(y + height - 1);

    do {
        int w = width;
        do {
            lcd_send_data(*addr++);
        } while (--w > 0);
        addr += LCD_WIDTH - width;
    } while (--height > 0);
}
