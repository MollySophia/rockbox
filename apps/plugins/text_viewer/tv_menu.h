/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Gilles Roux
 *               2003 Garrett Derner
 *               2010 Yoshihisa Uchida
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
#ifndef PLUGIN_TEXT_VIEWER_MENU_H
#define PLUGIN_TEXT_VIEWER_MENU_H

enum {
    TV_MENU_RESULT_MOVE_PAGE = 0,
    TV_MENU_RESULT_EXIT_MENU,
    TV_MENU_RESULT_EXIT_PLUGIN,
    TV_MENU_RESULT_ATTACHED_USB,
    TV_MENU_RESULT_ERROR,
};

/*
 * display the setting menu
 *
 * return
 *     the following value returns
 *         TV_MENU_RESULT_MOVE_PAGE     change reading page
 *         TV_MENU_RESULT_EXIT_MENU     menu exit and continue this plugin
 *         TV_MENU_RESULT_EXIT_PLUGIN   request to exit this plugin
 *         TV_MENU_RESULT_ATTACHED_USB  connect USB cable
 */
unsigned tv_display_menu(void);

#endif
