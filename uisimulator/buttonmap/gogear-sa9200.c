/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2009 Mark Arigo
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
        case SDLK_KP4:
        case SDLK_LEFT:
            new_btn = BUTTON_LEFT;
            break;
        case SDLK_KP6:
        case SDLK_RIGHT:
            new_btn = BUTTON_RIGHT;
            break;
        case SDLK_KP8:
        case SDLK_UP:
            new_btn = BUTTON_UP;
            break;
        case SDLK_KP2:
        case SDLK_DOWN:
            new_btn = BUTTON_DOWN;
            break;
        case SDLK_KP1:
            new_btn = BUTTON_MENU;
            break;
        case SDLK_KP5:
        case SDLK_SPACE:
            new_btn = BUTTON_PLAY;
            break;
        case SDLK_KP7:
            new_btn = BUTTON_PREV;
            break;
        case SDLK_KP9:
            new_btn = BUTTON_NEXT;
            break;
        case SDLK_KP_ENTER:
        case SDLK_RETURN:
            new_btn = BUTTON_POWER;
            break;
        case SDLK_PAGEUP:
            new_btn = BUTTON_VOL_UP;
            break;
        case SDLK_PAGEDOWN:
            new_btn = BUTTON_VOL_DOWN;
            break;
    }
    return new_btn;
}

struct button_map bm[] = {
    { SDLK_KP_ENTER,  25, 155, 33, "Power" },
    { SDLK_PAGEUP,   210,  98, 31, "Vol Up" },
    { SDLK_PAGEDOWN, 210, 168, 34, "Vol Down" },
    { SDLK_KP7,       40, 249, 26, "Prev" },
    { SDLK_KP8,      110, 247, 22, "Up" },
    { SDLK_KP9,      183, 249, 31, "Next" },
    { SDLK_KP4,       45, 305, 25, "Left" },
    { SDLK_KP5,      111, 304, 24, "Play" },
    { SDLK_KP6,      183, 304, 21, "Right" },
    { SDLK_KP1,       43, 377, 21, "Menu" },
    { SDLK_KP2,      112, 371, 24, "Down" },
    { 0, 0, 0, 0, "None" }
};
