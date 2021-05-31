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
* Copyright (C) 2009 by Karl Kurbjun
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

#ifndef _PLUGIN_LIB_TOUCHSCREEN_H_
#define _PLUGIN_LIB_TOUCHSCREEN_H_

#include "plugin.h"

#ifdef HAVE_TOUCHSCREEN

/*******************************************************************************
 * Touchbutton
 ******************************************************************************/
struct touchbutton {
    /* Each button has it's own viewport to define colors, drawstyle, location*/
    struct viewport vp;     
    bool repeat;            /* requires the area be held for the action */
    int action;             /* action this button will return */
    bool invisible;         /* Is this an invisible button? */
    char *title;            /* Specify a title */
    fb_data *pixmap;        /* Currently unused, but will allow for a graphic */
};

/* Check: tests if the result of button_get() beloned to a touch button */
int touchbutton_check_button(int button, struct touchbutton *data, int num_buttons);
/* Wait: Wait for input and return the corresponding action */
int touchbutton_get(struct touchbutton *data, int num_buttons);
/* Wait with timeout */
int touchbutton_get_w_tmo(int timeout, struct touchbutton *data, int num_buttons);

/* Draw: Draws all visible buttons */
void touchbutton_draw(struct touchbutton *data, int num_buttons);


/*******************************************************************************
 * Touch mapping
 ******************************************************************************/
struct ts_mapping
{
    int tl_x; /* top left */
    int tl_y;
    int width;
    int height;
};

struct ts_mappings
{
    struct ts_mapping *mappings;
    int amount;
};

unsigned int touchscreen_map(struct ts_mappings *map, int x, int y);

struct ts_raster
{
    int tl_x; /* top left */
    int tl_y;
    int width;
    int height;
    int raster_width;
    int raster_height;
};

struct ts_raster_result
{
    int x;
    int y;
};

unsigned int touchscreen_map_raster(struct ts_raster *map, int x, int y, struct ts_raster_result *result);

struct ts_raster_button_mapping
{
    struct ts_raster *raster;
    bool drag_drop_enable;              /* ... */
    bool double_click_enable;           /* ... */
    bool click_enable;                  /* ... */
    bool move_progress_enable;          /* ... */
    bool two_d_movement_enable;         /* ... */
    struct ts_raster_result two_d_from; /* ... */
    int _prev_x;                        /* Internal: DO NOT MODIFY! */
    int _prev_y;                        /* Internal: DO NOT MODIFY! */
    int _prev_btn_state;                /* Internal: DO NOT MODIFY! */
};

struct ts_raster_button_result
{
    enum{
        TS_ACTION_NONE,
        TS_ACTION_MOVE,
        TS_ACTION_CLICK,
        TS_ACTION_DOUBLE_CLICK,
        TS_ACTION_DRAG_DROP,
        TS_ACTION_TWO_D_MOVEMENT
    } action;
    struct ts_raster_result from;
    struct ts_raster_result to;
};

struct ts_raster_button_result touchscreen_raster_map_button(struct ts_raster_button_mapping *map, int x, int y, int button);

#endif /* HAVE_TOUCHSCREEN */
#endif /* _PLUGIN_LIB_TOUCHSCREEN_H_ */
