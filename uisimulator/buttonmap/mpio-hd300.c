/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010 Michael Leslie
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


#include <SDL.h>
#include "button.h"
#include "buttonmap.h"

int key_to_button(int keyboard_button)
{
    int new_btn = BUTTON_NONE;
    switch (keyboard_button)
    {

        case SDLK_UP:
            new_btn = BUTTON_UP;
            break;
        case SDLK_DOWN:
            new_btn = BUTTON_DOWN;
            break;

        case SDLK_LEFT:
            new_btn = BUTTON_LEFT;
            break;
        case SDLK_RIGHT:
            new_btn = BUTTON_RIGHT;
            break;

        case SDLK_SPACE:
        case SDLK_ESCAPE:
	  new_btn = BUTTON_MENU;
	  break;

        case SDLK_RETURN:
	  new_btn = BUTTON_ENTER;
	  break;

        case SDLK_p:
            new_btn = BUTTON_PLAY;
            break;

        case SDLK_r:
            new_btn = BUTTON_REC;
            break;
    }
    return new_btn;
}

struct button_map bm[] = {
    { SDLK_LEFT,     98, 312, 15, "Rew" },
    { SDLK_RIGHT,   186, 312, 15, "FF" },

    { SDLK_ESCAPE,   90, 364, 40, "Menu" },
    { SDLK_RETURN,  196, 364, 40, "Enter" },

    { SDLK_r,        98, 410, 15, "Rec" },
    { SDLK_p,       186, 410, 15, "Play/Stop" },

    { SDLK_UP,      143, 315, 40, "Up" },
    { SDLK_DOWN,    143, 415, 40, "Down" },

    { SDLK_h,       116,  52, 20, "Hold" },
    { 0, 0, 0, 0, "None" }
};
