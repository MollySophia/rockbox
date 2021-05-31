/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 Jonas H�gqvist
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 **************************************************************************/
#include "plugin.h"
#include "lib/pluginlib_actions.h"

/* this set the context to use with PLA */
static const struct button_mapping *plugin_contexts[]
                     = { pla_main_ctx,
#ifdef HAVE_REMOTE_LCD
                         pla_remote_ctx,
#endif
                        };

#define DISPLAY_WIDTH LCD_WIDTH
#define DISPLAY_HEIGHT LCD_HEIGHT
#define RAND_SCALE 5

#ifdef HAVE_REMOTE_LCD
#define REMOTE_WIDTH LCD_REMOTE_WIDTH
#define REMOTE_HEIGHT LCD_REMOTE_HEIGHT
#include "pluginbitmaps/remote_rockboxlogo.h"
#define REMOTE_LOGO_WIDTH BMPWIDTH_remote_rockboxlogo
#define REMOTE_LOGO_HEIGHT BMPHEIGHT_remote_rockboxlogo
#define REMOTE_LOGO remote_rockboxlogo
#endif /* HAVE_REMOTE_LCD */

#define LOGO rockboxlogo
#include "pluginbitmaps/rockboxlogo.h"
#define LOGO_WIDTH BMPWIDTH_rockboxlogo
#define LOGO_HEIGHT BMPHEIGHT_rockboxlogo

/* We use PLA */
#define LP_QUIT              PLA_EXIT
#define LP_QUIT2             PLA_CANCEL
#define LP_DEC_X             PLA_LEFT
#define LP_DEC_X_REPEAT      PLA_LEFT_REPEAT
#define LP_INC_X             PLA_RIGHT
#define LP_INC_X_REPEAT      PLA_RIGHT_REPEAT
#define LP_DEC_Y             PLA_DOWN
#define LP_DEC_Y_REPEAT      PLA_DOWN_REPEAT
#define LP_INC_Y             PLA_UP
#define LP_INC_Y_REPEAT      PLA_UP_REPEAT

enum plugin_status plugin_start(const void* parameter) {
    int button;
    int timer = 10;
    int x = (DISPLAY_WIDTH / 2) - (LOGO_WIDTH / 2);
    int y = (DISPLAY_HEIGHT / 2) - (LOGO_HEIGHT / 2);
    int dx;
    int dy;

    (void)parameter;

    rb->srand(*rb->current_tick);
    dx = rb->rand()%(2*RAND_SCALE+1) - RAND_SCALE;
    dy = rb->rand()%(2*RAND_SCALE+1) - RAND_SCALE;

    while (1) {
        rb->lcd_clear_display();
        rb->lcd_bitmap((const fb_data*)LOGO, x, y, LOGO_WIDTH, LOGO_HEIGHT);
#ifdef REMOTE_LOGO
        rb->lcd_remote_clear_display();
        rb->lcd_remote_bitmap((const fb_remote_data*)REMOTE_LOGO,
                (x * (REMOTE_WIDTH - REMOTE_LOGO_WIDTH)) / (DISPLAY_WIDTH - LOGO_WIDTH),
                (y * (REMOTE_HEIGHT - REMOTE_LOGO_HEIGHT)) / (DISPLAY_HEIGHT - LOGO_HEIGHT),
                REMOTE_LOGO_WIDTH, REMOTE_LOGO_HEIGHT);
#endif
        x += dx;
        if (x < 0) {
            dx = -dx;
            x = 0;
        }
        if (x > DISPLAY_WIDTH - LOGO_WIDTH) {
            dx = -dx;
            x = DISPLAY_WIDTH - LOGO_WIDTH;
        }

        y += dy;
        if (y < 0) {
            dy = -dy;
            y = 0;
        }
        if (y > DISPLAY_HEIGHT - LOGO_HEIGHT) {
            dy = -dy;
            y = DISPLAY_HEIGHT - LOGO_HEIGHT;
        }

        rb->lcd_update();
#ifdef REMOTE_LOGO
        rb->lcd_remote_update();
#endif
        rb->sleep(HZ/timer);



        /*We get button from PLA this way */
        button = pluginlib_getaction(TIMEOUT_NOBLOCK, plugin_contexts,
                               ARRAYLEN(plugin_contexts));

        switch (button) {
            case LP_QUIT:
            case LP_QUIT2:
                return PLUGIN_OK;
            case LP_DEC_X:
            case LP_DEC_X_REPEAT:
                if (dx)
                    dx += (dx < 0) ? 1 : -1;
                break;
            case LP_INC_X:
            case LP_INC_X_REPEAT:
                dx += (dx < 0) ? -1 : 1;
                break;
            case LP_DEC_Y:
            case LP_DEC_Y_REPEAT:
                if (dy)
                    dy += (dy < 0) ? 1 : -1;
                break;
            case LP_INC_Y:
            case LP_INC_Y_REPEAT:
                dy += (dy < 0) ? -1 : 1;
                break;
                
            default:
                if (rb->default_event_handler(button) == SYS_USB_CONNECTED) {
                    return PLUGIN_USB_CONNECTED;
                }
                break;
        }
    }
}
