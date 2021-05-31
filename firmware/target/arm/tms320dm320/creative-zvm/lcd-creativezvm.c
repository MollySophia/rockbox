/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id$
*
* Copyright (C) 2008 by Maurus Cuelenaere
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
#include <sys/types.h>
#include "config.h"

#include "hwcompat.h"
#include "kernel.h"
#include "lcd.h"
#include "system.h"
#include "string-extra.h" /* memset16() */
#include "cpu.h"
#include "spi.h"
#include "lcd-target.h"
#include "ltv350qv.h"

/* Power and display status */
extern bool lcd_on;  /* lcd-memframe.c */
static bool direct_fb_access = false; /* Does the DM320 has direct access to the FB? */

/* Copies a rectangle from one framebuffer to another. Can be used in
   single transfer mode with width = num pixels, and height = 1 which
   allows a full-width rectangle to be copied more efficiently. */
extern void lcd_copy_buffer_rect(fb_data *dst, const fb_data *src,
                                 int width, int height);

int lcd_default_contrast(void)
{
    return 0x1f;
}

void lcd_set_contrast(int val)
{
    /* find S6F2002 controller datasheet first */
    (void)val;
}

void lcd_set_invert_display(bool yesno) {
  (void) yesno;
  /* TODO: */
}

void lcd_set_flip(bool yesno) {
  (void) yesno;
  /* TODO: */
}

static void enable_venc(bool enable)
{
    if(enable)
    {
        /* Set OSD clock */
        IO_CLK_MOD1 &= ~(CLK_MOD1_VENC | CLK_MOD1_OSD); /* disable OSD clock and VENC clock */
        IO_CLK_O2DIV = 3;

        IO_CLK_OSEL &= ~CLK_OSEL_O2SEL(0xF); /* reset 'General purpose clock output (GIO26, GIO34)' and */
        IO_CLK_OSEL |= CLK_OSEL_O2SEL(4);    /* set to 'PLLIN clock' */

        IO_CLK_SEL1 |= (CLK_SEL1_OSD | CLK_SEL1_VENC(7)); /* set to 'GP clock output 2 (GIO26, GIO34)' and turn on 'VENC clock' */
        IO_CLK_MOD1 |= (CLK_MOD1_VENC | CLK_MOD1_OSD);    /* enable OSD clock and VENC clock */
    }
    else
    {
        /* Disable video encoder */
        IO_VID_ENC_VMOD &= ~VENC_VMOD_VENC;
        /* Disable clock for power saving */
        IO_CLK_MOD1 &= ~(CLK_MOD1_VENC | CLK_MOD1_OSD);
    }
}

/* LTV250QV panel functions */
static void lcd_write_reg(unsigned char reg, unsigned short val)
{
    unsigned char block[3] = {LTV_OPC_INDEX, 0, reg | 0xFF};
    spi_block_transfer(SPI_target_LTV250QV, block, sizeof(block), NULL, 0);
    block[0] = LTV_OPC_DATA;
    block[1] = val >> 8;
    block[2] = val & 0xFF;
    spi_block_transfer(SPI_target_LTV250QV, block, sizeof(block), NULL, 0);
}

static void sleep_ms(unsigned int ms)
{
    sleep(ms*HZ/1000);
}

static void lcd_display_on(bool reset)
{  
    /* Enable main power */
    IO_GIO_BITSET2 |= (1 << 3);
    
    /* power on sequence as per the ZVM firmware */
    sleep_ms(250);
    IO_GIO_BITSET1 = (1 << 13);
    sleep_ms(5);
    IO_GIO_BITSET2 = (1 << 5);
    IO_GIO_BITSET2 = (1 << 8);
    sleep_ms(1);
    
    /*Init SPI here... */
    sleep_ms(32);
    
    IO_GIO_BITSET2 = (1 << 0);
    sleep_ms(5);
    IO_GIO_BITSET2 = (1 << 7);
    sleep_ms(5);
    IO_GIO_BITSET2 = (1 << 4);
    sleep_ms(5);
    IO_GIO_BITCLR2 = (1 << 8);
    /*TODO: figure out what OF does after this... */
    IO_GIO_BITSET2 = (1 << 8);
    sleep_ms(1);
    
    lcd_write_reg(LTV_IFCTL,     LTV_NL(29));
    lcd_write_reg(LTV_DATACTL,   0);
    lcd_write_reg(LTV_ENTRY_MODE,0);
    lcd_write_reg(LTV_GATECTL1,  0);
    lcd_write_reg(LTV_GATECTL2,  (LTV_NW_INV_1LINE | LTV_FHN | LTV_FTI(2) | LTV_FWI(3)));
    lcd_write_reg(LTV_VBP,       0);
    lcd_write_reg(LTV_HBP,       0);
    lcd_write_reg(LTV_SOTCTL,    0);
    lcd_write_reg(LTV_PWRCTL1,   0);
    lcd_write_reg(LTV_PWRCTL2,   0);
    lcd_write_reg(LTV_GAMMA(0),  0);
    lcd_write_reg(LTV_GAMMA(1),  0);
    lcd_write_reg(LTV_GAMMA(2),  0);
    lcd_write_reg(LTV_GAMMA(3),  0);
    lcd_write_reg(LTV_GAMMA(4),  0);
    lcd_write_reg(LTV_GAMMA(5),  0);
    lcd_write_reg(LTV_GAMMA(6),  0);
    lcd_write_reg(LTV_GAMMA(7),  0);
    lcd_write_reg(LTV_GAMMA(8),  0);
    lcd_write_reg(LTV_GAMMA(9),  0);
    sleep_ms(10);
    
    lcd_write_reg(LTV_PWRCTL1,   (LTV_VCOM_DISABLE | LTV_DRIVE_CURRENT(5) | LTV_SUPPLY_CURRENT(5)));
    lcd_write_reg(LTV_PWRCTL2,   0);
    sleep_ms(40);
    
    lcd_write_reg(LTV_PWRCTL2,   LTV_VCOML_ENABLE);
    sleep_ms(40);
    
    lcd_write_reg(LTV_IFCTL,     (LTV_NMD | LTV_NL(29)));
    lcd_write_reg(LTV_DATACTL,   (LTV_DS_SAME | LTV_CHS_480 | LTV_DF_RGB | LTV_RGB_BGR));
    lcd_write_reg(LTV_ENTRY_MODE,(LTV_VSPL_ACTIVE_LOW | LTV_HSPL_ACTIVE_LOW | LTV_DPL_SAMPLE_RISING | LTV_EPL_ACTIVE_LOW | LTV_SS_RIGHT_TO_LEFT));
    lcd_write_reg(LTV_GATECTL1,  LTV_CLW(1));
    lcd_write_reg(LTV_GATECTL2,  (LTV_NW_INV_1LINE | LTV_DSC | LTV_FTI(3) | LTV_FWI(3)));
    lcd_write_reg(LTV_VBP,       0x5);
    lcd_write_reg(LTV_HBP,       0x1B);
    lcd_write_reg(LTV_SOTCTL,    LTV_SDT(2));
    lcd_write_reg(LTV_GAMMA(0),  0x203);
    lcd_write_reg(LTV_GAMMA(1),  0x302);
    lcd_write_reg(LTV_GAMMA(2),  0xC08);
    lcd_write_reg(LTV_GAMMA(3),  0xC08);
    lcd_write_reg(LTV_GAMMA(4),  0x707);
    lcd_write_reg(LTV_GAMMA(5),  0x707);
    lcd_write_reg(LTV_GAMMA(6),  0x104);
    lcd_write_reg(LTV_GAMMA(7),  0x306);
    lcd_write_reg(LTV_GAMMA(8),  0);
    lcd_write_reg(LTV_GAMMA(9),  0);
    sleep_ms(60);
    
    lcd_write_reg(LTV_PWRCTL1,   (LTV_VCOMOUT_ENABLE | LTV_POWER_ON | LTV_DRIVE_CURRENT(5) | LTV_SUPPLY_CURRENT(5)));
    lcd_write_reg(LTV_PWRCTL2,   (LTV_VCOML_VOLTAGE(17) | LTV_VCOMH_VOLTAGE(26))); /* VCOML=0,0625V VCOMH=1,21875V */
    sleep_ms(10);

    if(!reset)
    {
        enable_venc(true);
        /* Re-enable video encoder */
        IO_VID_ENC_VMOD |= VENC_VMOD_VENC;
    }
    /* tell that we're on now */
    lcd_on = true;
}

#ifdef HAVE_LCD_ENABLE
static void lcd_display_off(void)
{
    /* LQV shutdown sequence */
    lcd_write_reg(LTV_PWRCTL1,  (LTV_VCOMOUT_ENABLE | LTV_DRIVE_CURRENT(5) | LTV_SUPPLY_CURRENT(5)));
    sleep_ms(20);
    
    lcd_write_reg(LTV_PWRCTL1,  (LTV_DRIVE_CURRENT(5) | LTV_SUPPLY_CURRENT(5)));
    lcd_write_reg(LTV_GATECTL2, (LTV_NW_INV_1LINE | LTV_FTI(3) | LTV_FWI(3)));
    lcd_write_reg(LTV_PWRCTL2,  0);
    sleep_ms(20);
    
    lcd_write_reg(LTV_PWRCTL1,  0);
    sleep_ms(10);
    unsigned char temp[1];
    temp[0] = 0;
    spi_block_transfer(SPI_target_LTV250QV, temp, sizeof(temp), NULL, 0);
    
    IO_GIO_BITCLR2 = (1 << 4);
    sleep_ms(5);
    IO_GIO_BITCLR2 = (1 << 7);
    sleep_ms(5);
    IO_GIO_BITCLR2 = (1 << 0);
    sleep_ms(2);
    IO_GIO_BITCLR2 = (1 << 8);
    IO_GIO_BITCLR2 = (1 << 5);
    
    /* Disable main power */
    IO_GIO_BITCLR2 |= (1 << 3);
    
    enable_venc(false);
    
    lcd_on = false;
}

void lcd_enable(bool on)
{
/* Disabled until properly working */
return;
    if (on == lcd_on)
        return;

    if (on)
    {
        lcd_display_on(false);  /* Turn on display */
        lcd_update();      /* Resync display */
        send_event(LCD_EVENT_ACTIVATION, NULL);
    }
    else
    {
        lcd_display_off();  /* Turn off display */
    }
}
#endif

void lcd_set_direct_fb(bool yes)
{
    unsigned int addr;
    direct_fb_access = yes;
    if(yes)
        addr = ((unsigned int)FBADDR(0,0)-CONFIG_SDRAM_START) / 32;
    else
        addr = ((unsigned int)FRAME-CONFIG_SDRAM_START) / 32;
    IO_OSD_OSDWINADH = addr >> 16;
    IO_OSD_OSDWIN0ADL = addr & 0xFFFF;
}

bool lcd_get_direct_fb(void)
{
    return direct_fb_access;
}

static bool _lcd_enabled(void)
{
    /* Needed to detect if VENC/LCD already is initialized... */
    if(IO_VID_ENC_VDCTL & VENC_VDCTL_VCLKE)
        return true;
    else if(!(IO_VID_ENC_VDCTL & VENC_VDCTL_YCDC))
        return true;
    else if(IO_CLK_MOD1 & CLK_MOD1_VENC)
        return true;
    else if(IO_CLK_MOD1 & CLK_MOD1_OSD)
        return true;
    else
        return false;
}

void lcd_init_device(void)
{    
    if(!_lcd_enabled())
    {
        lcd_display_on(true);
        
        enable_venc(true);
        
        /* Set LCD values in Video Encoder */
        IO_VID_ENC_VMOD &= 0x8800; /* Clear all values */
        IO_VID_ENC_VMOD |= (VENC_VMOD_DACPD | VENC_VMOD_VMD | VENC_VMOD_ITLC | VENC_VMOD_VDMD(2)); /* set mode to RGB666 parallel 16 bit */
        IO_VID_ENC_VDCTL &= 0x8FE8; /* Clear all values */
        IO_VID_ENC_VDCTL |= (VENC_VDCTL_VCLKP | VENC_VDCTL_DOMD(2)),
        IO_VID_ENC_VDPRO = VENC_VDPRO_PFLTR;
        IO_VID_ENC_SYNCTL &= 0xE000; /* Clear all values */
        IO_VID_ENC_SYNCTL |= (VENC_SYNCTL_VPL | VENC_SYNCTL_HPL);
        IO_VID_ENC_HSDLY = 0;
        IO_VID_ENC_HSPLS = 0x12;
        IO_VID_ENC_HSTART = 0x1B;
        IO_VID_ENC_HVALID = 0x140;
        IO_VID_ENC_HINT = 0x168;
        IO_VID_ENC_VSDLY = 0;
        IO_VID_ENC_VSPLS = 3;
        IO_VID_ENC_VSTART = 5;
        IO_VID_ENC_VVALID = 0xF0;
        IO_VID_ENC_VINT = 0x118;
        IO_VID_ENC_RGBCTL &= 0x088; /* Clear all values */
        IO_VID_ENC_RGBCTL |= VENC_RGBCTL_DFLTR;
        IO_VID_ENC_RGBCLP = VENC_RGBCLP_UCLIP(0xFF);
        IO_VID_ENC_LCDOUT &= 0xFE00; /* Clear all values */
        IO_VID_ENC_LCDOUT |= (VENC_LCDOUT_OEE | VENC_LCDOUT_FIDS);
        IO_VID_ENC_DCLKCTL &= 0xC0C0; /* Clear all values */
        IO_VID_ENC_DCLKCTL |= VENC_DCLKCTL_DCKEC;
        IO_VID_ENC_DCLKPTN0 = 1;
        DM320_REG(0x0864) = 0; /* ???? */
    }
    else
        lcd_on = true;

    /* Based on lcd-mr500.c from Catalin Patulea */
    /* Clear the Frame */
    memset16(FRAME, 0x0000, LCD_WIDTH*LCD_HEIGHT);
    
    IO_OSD_MODE = 0x00ff;
    IO_OSD_VIDWINMD = 0x0002;
    IO_OSD_OSDWINMD0 = 0x2001;
    IO_OSD_OSDWINMD1 = 0x0002;
    IO_OSD_ATRMD = 0x0000;
    IO_OSD_RECTCUR = 0x0000;

    unsigned int addr;
    IO_OSD_OSDWIN0OFST = (LCD_WIDTH*16) / 256;
    addr = ((unsigned int)FRAME-CONFIG_SDRAM_START) / 32;
    IO_OSD_OSDWINADH = addr >> 16;
    IO_OSD_OSDWIN0ADL = addr & 0xFFFF;

#ifndef ZEN_VISION
    IO_OSD_BASEPX = 26;
    IO_OSD_BASEPY = 5;
#else
    IO_OSD_BASEPX = 80;
    IO_OSD_BASEPY = 0;
#endif

    IO_OSD_OSDWIN0XP = 0;
    IO_OSD_OSDWIN0YP = 0;
    IO_OSD_OSDWIN0XL = LCD_WIDTH;
    IO_OSD_OSDWIN0YL = LCD_HEIGHT;

    IO_VID_ENC_VDCTL |= VENC_VDCTL_VCLKE; /* Enable VCLK */
    IO_VID_ENC_VMOD |= VENC_VMOD_VENC; /* Enable video encoder */
    IO_VID_ENC_SYNCTL |= VENC_SYNCTL_SYE; /* Enable sync output */
    IO_VID_ENC_VDCTL &= ~VENC_VDCTL_DOMD(3); /* Normal digital data output */
}


/*** Update functions ***/

/* Update a fraction of the display. */
void lcd_update_rect(int x, int y, int width, int height)
{
    register fb_data *dst, *src;

    if (!lcd_on || direct_fb_access)
        return;

    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x; /* Clip right */
    if (x < 0)
        width += x, x = 0; /* Clip left */
    if (width <= 0)
    return; /* nothing left to do */

    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y; /* Clip bottom */
    if (y < 0)
        height += y, y = 0; /* Clip top */
    if (height <= 0)
        return; /* nothing left to do */

#if CONFIG_ORIENTATION == SCREEN_PORTRAIT
    dst = (fb_data *)FRAME + LCD_WIDTH*y + x;
    src = FBADDR(x,y);

    /* Copy part of the Rockbox framebuffer to the second framebuffer */
    if (width < LCD_WIDTH)
    {
        /* Not full width - do line-by-line */
        lcd_copy_buffer_rect(dst, src, width, height);
    }
    else
    {
        /* Full width - copy as one line */
        lcd_copy_buffer_rect(dst, src, LCD_WIDTH*height, 1);
    }
#else
    src = FBADDR(x,y);
    
    register int xc, yc;
    register fb_data *start=FRAME + LCD_HEIGHT*(LCD_WIDTH-x-1) + y + 1;

    for(yc=0;yc<height;yc++)
    {
        dst=start+yc;
        for(xc=0; xc<width; xc++)
        {
            *dst=*src++;
            dst-=LCD_HEIGHT;
        }
        src+=x;
    }
#endif
}

/* Update the display.
This must be called after all other LCD functions that change the display. */
void lcd_update(void)
{
    if (!lcd_on || direct_fb_access)
        return;
#if CONFIG_ORIENTATION == SCREEN_PORTRAIT
    lcd_copy_buffer_rect((fb_data *)FRAME, FBADDR(0,0),
                         LCD_WIDTH*LCD_HEIGHT, 1);
#else
    lcd_update_rect(0, 0, LCD_WIDTH, LCD_HEIGHT);
#endif
}
