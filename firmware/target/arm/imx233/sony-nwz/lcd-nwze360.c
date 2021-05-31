/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (c) 2013 by Amaury Pouly
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
#include <string.h>
#include "cpu.h"
#include "system.h"
#include "backlight-target.h"
#include "lcd.h"
#include "lcdif-imx233.h"
#include "clkctrl-imx233.h"
#include "pinctrl-imx233.h"
#include "logf.h"

#ifdef HAVE_LCD_ENABLE
static bool lcd_on;
#endif

static void lcd_write_reg(uint8_t reg, uint8_t data)
{
    imx233_lcdif_wait_ready();
    imx233_lcdif_set_word_length(8);
    imx233_lcdif_pio_send(false, 1, &reg);
    if(reg != 0x22)
        imx233_lcdif_pio_send(true, 1, &data);
}

static void lcd_init_seq(void)
{
    lcd_write_reg(0xea, 0);
    lcd_write_reg(0xeb, 0x20);
    lcd_write_reg(0xec, 0xc);
    lcd_write_reg(0xed, 0xc4);
    lcd_write_reg(0xe8, 0x38);
    lcd_write_reg(0xe9, 0xe);
    lcd_write_reg(0xf1, 1);
    lcd_write_reg(0xf2, 8);
    lcd_write_reg(0x2e, 0x86);
    lcd_write_reg(0x29, 0xff);
    lcd_write_reg(0xe4, 1);
    lcd_write_reg(0xe5, 0x20);
    lcd_write_reg(0xe7, 1);
    lcd_write_reg(0x40, 0);
    lcd_write_reg(0x41, 0);
    lcd_write_reg(0x42, 0);
    lcd_write_reg(0x43, 0x14);
    lcd_write_reg(0x44, 0x14);
    lcd_write_reg(0x45, 0x28);
    lcd_write_reg(0x46, 0x11);
    lcd_write_reg(0x47, 0x57);
    lcd_write_reg(0x48, 5);
    lcd_write_reg(0x49, 0x16);
    lcd_write_reg(0x4a, 0x19);
    lcd_write_reg(0x4b, 0x1a);
    lcd_write_reg(0x4c, 0x1a);
    lcd_write_reg(0x50, 0x17);
    lcd_write_reg(0x51, 0x2b);
    lcd_write_reg(0x52, 0x2b);
    lcd_write_reg(0x53, 0x3f);
    lcd_write_reg(0x54, 0x3f);
    lcd_write_reg(0x55, 0x3f);
    lcd_write_reg(0x56, 0x28);
    lcd_write_reg(0x57, 0x6e);
    lcd_write_reg(0x58, 5);
    lcd_write_reg(0x59, 5);
    lcd_write_reg(0x5a, 6);
    lcd_write_reg(0x5b, 9);
    lcd_write_reg(0x5c, 0x1a);
    lcd_write_reg(0x5d, 0xcc);
    lcd_write_reg(0x1b, 0x1b);
    lcd_write_reg(0x1a, 1);
    lcd_write_reg(0x24, 0x2f); // something special here
    lcd_write_reg(0x25, 0x57); // something special here
    lcd_write_reg(0x23, 0x8a);
    lcd_write_reg(0x2f, 1);
    lcd_write_reg(0x60, 0);
    lcd_write_reg(0x16, 0); // NOTE OF uses 0x8, controls BGR<->RGB
    lcd_write_reg(0x18, 0x36); // something special here
    lcd_write_reg(0x19, 1);
    udelay(5000);
    lcd_write_reg(1, 0);
    lcd_write_reg(0x1f, 0x88);
    udelay(5000);
    lcd_write_reg(0x1f, 0x80);
    udelay(5000);
    lcd_write_reg(0x1f, 0x90);
    udelay(5000);
    lcd_write_reg(0x1f, 0xD0);
    udelay(5000);
    lcd_write_reg(0x17, 6);
    lcd_write_reg(0x37, 0);
    lcd_write_reg(0x28, 0x38);
    udelay(40000);
    lcd_write_reg(0x28, 0x3c);
}

void lcd_init_device(void)
{
    /* the LCD seems to work at 24Mhz, so use the xtal clock with no divider */
    imx233_clkctrl_enable(CLK_PIX, false);
    imx233_clkctrl_set_div(CLK_PIX, 1);
    imx233_clkctrl_set_bypass(CLK_PIX, true); /* use XTAL */
    imx233_clkctrl_enable(CLK_PIX, true);
    imx233_lcdif_init();
    imx233_lcdif_set_lcd_databus_width(8);
    imx233_lcdif_set_timings(1, 1, 1, 1);
    imx233_lcdif_enable_underflow_recover(true);
    imx233_lcdif_setup_system_pins(8);
    imx233_lcdif_set_byte_packing_format(0xf); /* two pixels per 32-bit word */

    // reset device
    imx233_lcdif_reset_lcd(true);
    mdelay(10);
    imx233_lcdif_reset_lcd(false);
    mdelay(10);
    imx233_lcdif_reset_lcd(true);
    mdelay(150);

    lcd_init_seq();
#ifdef HAVE_LCD_ENABLE
    lcd_on = true;
#endif
}

#ifdef HAVE_LCD_ENABLE
bool lcd_active(void)
{
    return lcd_on;
}

static void lcd_enable_seq(bool enable)
{
    if(!enable)
    {
        lcd_write_reg(0x1f, 0xd1);
        lcd_write_reg(1, 0x40);
        lcd_write_reg(1, 0xc0);
        lcd_write_reg(0x19, 1);
    }
    else
    {
        lcd_write_reg(0x19, 0x81);
        udelay(5000);
        lcd_write_reg(1, 0x40);
        udelay(20000);
        lcd_write_reg(1, 0);
        lcd_write_reg(0x1f, 0xd0);
    }
}

void lcd_enable(bool enable)
{
    if(lcd_on == enable)
        return;

    lcd_on = enable;

    if(enable)
        imx233_lcdif_enable(true);
    lcd_enable_seq(enable);
    if(!enable)
        imx233_lcdif_enable(false);
    else
        send_event(LCD_EVENT_ACTIVATION, NULL);
}
#endif

void lcd_update(void)
{
    lcd_update_rect(0, 0, LCD_WIDTH, LCD_HEIGHT);
}

void lcd_update_rect(int x, int y, int w, int h)
{
#ifdef HAVE_LCD_ENABLE
    if(!lcd_on)
        return;
#endif
    /* make sure the rectangle is included in the screen */
    x = MIN(x, LCD_WIDTH);
    y = MIN(y, LCD_HEIGHT);
    w = MIN(w, LCD_WIDTH - x);
    h = MIN(h, LCD_HEIGHT - y);

    imx233_lcdif_wait_ready();
    lcd_write_reg(2, x >> 8);
    lcd_write_reg(3, x & 0xff);
    lcd_write_reg(4, (x + w - 1) >> 8);
    lcd_write_reg(5, (x + w - 1) & 0xff);
    lcd_write_reg(6, y >> 8);
    lcd_write_reg(7, y & 0xff);
    lcd_write_reg(8, (y + h - 1) >> 8);
    lcd_write_reg(9, (y + h - 1) & 0xff);
    lcd_write_reg(0x22, 0);

    imx233_lcdif_wait_ready();
    imx233_lcdif_set_word_length(16);
    /* there are two cases here:
     * - either width = LCD_WIDTH and we can directly memcopy a part of lcd_framebuffer to FRAME
     *   and send it
     * - either width != LCD_WIDTH and we have to build a contiguous copy of the rectangular area
     *   into FRAME before sending it (which is slower and doesn't use the hardware)
     * In all cases, FRAME just acts as a temporary buffer.
     * NOTE It's more interesting to do a copy to FRAME in all cases since in system mode
     * the clock runs at 24MHz which provides barely 10MB/s bandwidth compared to >100MB/s
     * for memcopy operations
     */
    if(w == LCD_WIDTH)
    {
        memcpy((void *)FRAME, FBADDR(x,y), w * h * sizeof(fb_data));
    }
    else
    {
        for(int i = 0; i < h; i++)
            memcpy((fb_data *)FRAME + i * w, FBADDR(x,y + i), w * sizeof(fb_data));
    }
    /* WARNING The LCDIF has a limitation on the vertical count ! In 16-bit packed mode
     * (which we used, ie 16-bit per pixel, 2 pixels per 32-bit words), the v_count
     * field must be a multiple of 2. Furthermore, it seems the lcd controller doesn't
     * really like when both w and h are even, probably because the writes to the GRAM
     * are done on several words and the controller requires dummy writes.
     * The workaround is to always make sure that we send a number of pixels which is
     * a multiple of 4 so that both the lcdif and the controller are happy. If any
     * of w or h is odd, we will send a copy of the first pixels as dummy writes. We will
     * send at most 3 bytes. We then send (w * h + 3) / 4 x 4 bytes.
     */
    if(w % 2 == 1 || h % 2 == 1)
    {
        /* copy three pixel after the last one */
        for(int i = 0; i < 3; i++)
            *((fb_data *)FRAME + w * h + i) = *((fb_data *)FRAME + i);
        /* WARNING we need to update w and h to reflect the pixel count BUT it
         * has no relation to w * h (it can even be 2 * prime). Hopefully, w <= 240 and
         * h <= 320 so w * h <= 76800 and (w * h + 3) / 4 <= 38400 which fits into
         * a 16-bit integer (horizontal count). */
        h = (w * h + 3) / 4;
        w = 4;
    }
    imx233_lcdif_dma_send((void *)FRAME_PHYS_ADDR, w, h);
}

