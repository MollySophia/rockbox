/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \ 
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Dan Everton
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

#ifndef __LCDBITMAP_H__
#define __LCDBITMAP_H__

#include "lcd.h"
#include "SDL.h"

void sim_lcd_init(void);
#if LCD_DEPTH < 8
void sim_lcd_ex_init(unsigned long (*getpixel)(int, int));
void sim_lcd_ex_update_rect(int x, int y, int width, int height);
#endif

#endif /* #ifndef __LCDBITMAP_H__ */

