/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 Jonathan Gordon
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

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "config.h"
#include "string.h"
#include "lang.h"
#include "action.h"
#include "settings.h"
#include "power.h"
#include "powermgmt.h"
#include "menu.h"
#include "misc.h"
#include "exported_menus.h"
#include "tree.h"
#include "storage.h"
#ifdef HAVE_RECORDING
#include "recording.h"
#endif
#include "yesno.h"
#include "keyboard.h"
#include "screens.h"
#include "plugin.h"
#include "talk.h"
#include "splash.h"
#include "debug_menu.h"
#include "version.h"
#include "time.h"
#include "wps.h"
#include "skin_buffer.h"
#include "disk.h"

static const struct browse_folder_info config = {ROCKBOX_DIR, SHOW_CFG};

/***********************************/
/*    MANAGE SETTINGS MENU        */

static int reset_settings(void)
{
    static const char *lines[]={ID2P(LANG_RESET_ASK)};
    static const char *yes_lines[]={
        ID2P(LANG_SETTINGS),
        ID2P(LANG_RESET_DONE_CLEAR)
    };
    static const char *no_lines[]={
        ID2P(LANG_SETTINGS),
        ID2P(LANG_CANCEL)
    };
    static const struct text_message message={lines, 1};
    static const struct text_message yes_message={yes_lines, 2};
    static const struct text_message no_message={no_lines, 2};

    switch(gui_syncyesno_run(&message, &yes_message, &no_message))
    {
        case YESNO_YES:
            settings_reset();
            settings_save();
            settings_apply(true);
            settings_apply_skins();
            break;
        case YESNO_NO:
            break;
        case YESNO_USB:
            return 1;
    }
    return 0;
}
static int write_settings_file(void* param)
{
    return settings_save_config((intptr_t)param);
}

MENUITEM_FUNCTION(browse_configs, MENU_FUNC_USEPARAM, ID2P(LANG_CUSTOM_CFG),
        browse_folder, (void*)&config, NULL, Icon_NOICON);
MENUITEM_FUNCTION(save_settings_item, MENU_FUNC_USEPARAM, ID2P(LANG_SAVE_SETTINGS),
        write_settings_file, (void*)SETTINGS_SAVE_ALL, NULL, Icon_NOICON);
MENUITEM_FUNCTION(save_theme_item, MENU_FUNC_USEPARAM, ID2P(LANG_SAVE_THEME),
        write_settings_file, (void*)SETTINGS_SAVE_THEME, NULL, Icon_NOICON);
MENUITEM_FUNCTION(save_sound_item, MENU_FUNC_USEPARAM, ID2P(LANG_SAVE_SOUND),
        write_settings_file, (void*)SETTINGS_SAVE_SOUND, NULL, Icon_NOICON);
MENUITEM_FUNCTION(reset_settings_item, 0, ID2P(LANG_RESET),
                  reset_settings, NULL, NULL, Icon_NOICON);

MAKE_MENU(manage_settings, ID2P(LANG_MANAGE_MENU), NULL, Icon_Config,
          &browse_configs, &reset_settings_item,
          &save_settings_item, &save_sound_item, &save_theme_item);
/*    MANAGE SETTINGS MENU        */
/**********************************/

/***********************************/
/*      INFO MENU                  */


static int show_credits(void)
{
    char credits[MAX_PATH] = { '\0' };
    snprintf(credits, MAX_PATH, "%s/credits.rock", VIEWERS_DIR);
    if (plugin_load(credits, NULL) != PLUGIN_OK)
    {
        /* show the rockbox logo and version untill a button is pressed */
        show_logo();
        while (IS_SYSEVENT(get_action(CONTEXT_STD, TIMEOUT_BLOCK)))
            ;
    }
    return 0;
}

#define SIZE_FMT "%s %s"
struct info_data

{
    unsigned long size[NUM_VOLUMES];
    unsigned long free[NUM_VOLUMES];
    unsigned long name[NUM_VOLUMES];
    bool new_data;
};
enum infoscreenorder
{
    INFO_BATTERY = 0,
    INFO_BUFFER,
#ifdef HAVE_RECORDING
    INFO_REC_DIR,
#endif
    INFO_VERSION,
#if CONFIG_RTC
    INFO_DATE,
    INFO_TIME,
#endif
    INFO_DISK1, /* capacity/free on internal */
    INFO_COUNT
};

/* NOTE:  This code is not quite right for
   (HAVE_MULTIVOLUME && !HAVE_MULTIDRIVE), but there are no
   in-tree matches so.. eh.
*/
static int refresh_data(struct info_data *info)
{
    int i = 0;
#ifdef HAVE_MULTIDRIVE
    int drive;
    int max = -1;

    for (i = 0 ; CHECK_VOL(i) ; i++) {
#endif
	volume_size(IF_MV(i,) &info->size[i], &info->free[i]);
#ifdef HAVE_MULTIDRIVE
	drive = volume_drive(i);
	if (drive > 0 || info->size[i] == 0)
	    info->name[i] = LANG_DISK_NAME_MMC;
	else
#endif
	    info->name[i] = LANG_DISK_NAME_INTERNAL;
#ifdef HAVE_MULTIDRIVE
	if (drive > max)
	    max = drive;
	else if (drive < max)
	    break;
    }
#else
    i++;
#endif

    info->new_data = false;
    return i;
}

static const char* info_getname(int selected_item, void *data,
                                char *buffer, size_t buffer_len)
{
    struct info_data *info = (struct info_data*)data;
#if CONFIG_RTC
    struct tm *tm;
#endif
    char s1[32];
    char s2[32];
    int i;

    if (info->new_data)
	refresh_data(info);

    switch (selected_item)
    {
        case INFO_VERSION:
            snprintf(buffer, buffer_len, "%s: %s",
                     str(LANG_VERSION), rbversion);
            break;

#if CONFIG_RTC
        case INFO_TIME:
            tm = get_time();
            if (valid_time(tm))
            {
                snprintf(buffer, buffer_len, "%02d:%02d:%02d %s",
                    global_settings.timeformat == 0 ? tm->tm_hour :
                         ((tm->tm_hour + 11) % 12) + 1,
                         tm->tm_min,
                         tm->tm_sec,
                         global_settings.timeformat == 0 ? "" :
                         tm->tm_hour>11 ? "P" : "A");
            }
            else
            {
                snprintf(buffer, buffer_len, "%s", "--:--:--");
            }
            break;
        case INFO_DATE:
            tm = get_time();
            if (valid_time(tm))
            {
                snprintf(buffer, buffer_len, "%s %d %d",
                    str(LANG_MONTH_JANUARY + tm->tm_mon),
                    tm->tm_mday,
                    tm->tm_year+1900);
            }
            else
            {
                snprintf(buffer, buffer_len, "%s", str(LANG_UNKNOWN));
            }
            break;
#endif

#ifdef HAVE_RECORDING
        case INFO_REC_DIR:
            snprintf(buffer, buffer_len, "%s %s", str(LANG_REC_DIR), global_settings.rec_directory);
            break;
#endif

        case INFO_BUFFER: /* buffer */
        {
            long kib = audio_buffer_size() >> 10; /* to KiB */
            output_dyn_value(s1, sizeof(s1), kib, kibyte_units, 3, true);
            snprintf(buffer, buffer_len, "%s %s", str(LANG_BUFFER_STAT), s1);
        }
        break;
        case INFO_BATTERY: /* battery */
#if CONFIG_CHARGING == CHARGING_SIMPLE
            /* Only know if plugged */
            if (charger_inserted())
                return str(LANG_BATTERY_CHARGE);
            else
#elif CONFIG_CHARGING >= CHARGING_MONITOR
            /* Go by what power management reports */
            if (charging_state())
                return str(LANG_BATTERY_CHARGE);
            else
#endif /* CONFIG_CHARGING = */
            if (battery_level() >= 0)
                snprintf(buffer, buffer_len, str(LANG_BATTERY_TIME),
                         battery_level(), battery_time() / 60, battery_time() % 60);
            else
                return "Battery n/a"; /* translating worth it? */
            break;
        case INFO_DISK1: /* disk usage for internal */
        default:
	    i = selected_item - INFO_DISK1;
	    if (info->size[i]) {
		output_dyn_value(s1, sizeof s1, info->free[i], kibyte_units, 3, true);
		output_dyn_value(s2, sizeof s2, info->size[i], kibyte_units, 3, true);
		snprintf(buffer, buffer_len, "%s %s/%s", str(info->name[i]),
			 s1, s2);
#ifdef HAVE_MULTIVOLUME
	    } else {
		snprintf(buffer, buffer_len, "%s %s", str(info->name[i]),
			 str(LANG_NOT_PRESENT));
#endif
	    }
            break;
    }
    return buffer;
}

static int info_speak_item(int selected_item, void * data)
{
    struct info_data *info = (struct info_data*)data;

#if CONFIG_RTC
    struct tm *tm;
#endif
    int i;

    if (info->new_data)
	refresh_data(info);

    switch (selected_item)
    {
        case INFO_VERSION: /* version */
            talk_id(LANG_VERSION, false);
            talk_spell(rbversion, true);
            break;

#if CONFIG_RTC
        case INFO_TIME:
            tm = get_time();
            talk_id(VOICE_CURRENT_TIME, false);
            if (valid_time(tm))
            {
                talk_time(tm, true);
            }
            else
            {
                talk_id(LANG_UNKNOWN, true);
            }
            break;
        case INFO_DATE:
            tm = get_time();
            if (valid_time(tm))
            {
                talk_date(get_time(), true);
            }
            else
            {
                talk_id(LANG_UNKNOWN, true);
            }
            break;
#endif

#ifdef HAVE_RECORDING
        case INFO_REC_DIR:
            talk_id(LANG_REC_DIR, false);
            if (global_settings.rec_directory && global_settings.rec_directory[0])
            {
                long *pathsep = NULL;
                char rec_directory[MAX_PATHNAME+1];
                char *s;
                strcpy(rec_directory, global_settings.rec_directory);
                s = rec_directory;
                if ((strlen(s) > 1) && (s[strlen(s) - 1] == '/'))
                    s[strlen(s) - 1] = 0;
                while (s)
                {
                    s = strchr(s + 1, '/');
                    if (s)
                        s[0] = 0;
                    talk_dir_or_spell(rec_directory, pathsep, true);
                    if (s)
                        s[0] = '/';
                    pathsep = TALK_IDARRAY(VOICE_CHAR_SLASH);
                }
            }
            break;
#endif

        case INFO_BUFFER: /* buffer */
        {
            talk_id(LANG_BUFFER_STAT, false);
            long kib = audio_buffer_size() >> 10; /* to KiB */
            output_dyn_value(NULL, 0, kib, kibyte_units, 3, true);
            break;
        }
        case INFO_BATTERY: /* battery */
#if CONFIG_CHARGING == CHARGING_SIMPLE
            /* Only know if plugged */
            if (charger_inserted())
            {
                talk_id(LANG_BATTERY_CHARGE, true);
                if (battery_level() >= 0)
                    talk_value(battery_level(), UNIT_PERCENT, true);
            }
            else
#elif CONFIG_CHARGING >= CHARGING_MONITOR
            /* Go by what power management reports */
            if (charging_state())
            {
                talk_id(LANG_BATTERY_CHARGE, true);
                if (battery_level() >= 0)
                    talk_value(battery_level(), UNIT_PERCENT, true);
            }
            else
#endif /* CONFIG_CHARGING = */
            if (battery_level() >= 0)
            {
                int time_left = battery_time();
                talk_id(LANG_BATTERY_TIME, false);
                talk_value(battery_level(), UNIT_PERCENT, true);
                if (time_left >= 0)
                    talk_value(time_left * 60, UNIT_TIME, true);
            }
            else talk_id(VOICE_BLANK, false);
            break;
        case INFO_DISK1: /* disk 1 */
        default:
	    i = selected_item - INFO_DISK1;
	    if (info->size[i]) {
		talk_ids(false, info->name[i], LANG_DISK_FREE_INFO);
		output_dyn_value(NULL, 0, info->free[i], kibyte_units, 3, true);
		talk_id(LANG_DISK_SIZE_INFO, true);
		output_dyn_value(NULL, 0, info->size[i], kibyte_units, 3, true);
#ifdef HAVE_MULTIVOLUME
	    } else {
		talk_id(LANG_NOT_PRESENT, true);
#endif
	    }
	    break;
    }
    return 0;
}

static int info_action_callback(int action, struct gui_synclist *lists)
{
    if (action == ACTION_STD_CANCEL)
        return action;
    else if (action == ACTION_STD_OK
#ifdef HAVE_HOTSWAP
        || action == SYS_FS_CHANGED
#endif
        )
    {
#if (CONFIG_PLATFORM & PLATFORM_NATIVE)
        struct info_data *info = (struct info_data *)lists->data;
        int i;
        info->new_data = true;
        splash(0, ID2P(LANG_SCANNING_DISK));
        for (i = 0; i < NUM_VOLUMES; i++)
            volume_recalc_free(IF_MV(i));
#endif
        gui_synclist_speak_item(lists);
        return ACTION_REDRAW;
    }
#if CONFIG_RTC
    else if (action == ACTION_NONE)
    {
        static int last_redraw = 0;
        if (gui_synclist_item_is_onscreen(lists, 0, INFO_TIME)
            && TIME_AFTER(current_tick, last_redraw + HZ*5))
        {
            last_redraw = current_tick;
            return ACTION_REDRAW;
        }
    }
#endif
    return action;
}

static int show_info(void)
{
    struct info_data data = {.new_data = true };
    struct simplelist_info info;
    int count = INFO_COUNT + refresh_data(&data) - 1;
    simplelist_info_init(&info, str(LANG_ROCKBOX_INFO), count, (void*)&data);
    info.hide_selection = !global_settings.talk_menu;
    if (info.hide_selection)
        info.scroll_all = true;
    info.get_name = info_getname;
    if(global_settings.talk_menu)
         info.get_talk = info_speak_item;
    info.action_callback = info_action_callback;
    return (simplelist_show_list(&info)) ? 1 : 0;
}


MENUITEM_FUNCTION(show_info_item, 0, ID2P(LANG_ROCKBOX_INFO),
                   show_info, NULL, NULL, Icon_NOICON);

#if CONFIG_RTC
int time_screen(void* ignored);
MENUITEM_FUNCTION(timedate_item, MENU_FUNC_CHECK_RETVAL, ID2P(LANG_TIME_MENU),
                    time_screen, NULL,  NULL, Icon_Menu_setting );
#endif

MENUITEM_FUNCTION(show_credits_item, 0, ID2P(LANG_CREDITS),
                   show_credits, NULL, NULL, Icon_NOICON);

MENUITEM_FUNCTION(show_runtime_item, 0, ID2P(LANG_RUNNING_TIME),
                   view_runtime, NULL, NULL, Icon_NOICON);

MENUITEM_FUNCTION(debug_menu_item, 0, ID2P(LANG_DEBUG),
                   debug_menu, NULL, NULL, Icon_NOICON);

MAKE_MENU(info_menu, ID2P(LANG_SYSTEM), 0, Icon_System_menu,
          &show_info_item, &show_credits_item,
          &show_runtime_item, &debug_menu_item);
/*      INFO MENU                  */
/***********************************/

/***********************************/
/*    MAIN MENU                    */

MAKE_MENU(main_menu_, ID2P(LANG_SETTINGS), NULL,
        Icon_Submenu_Entered,
        &sound_settings,
        &playback_settings,
        &settings_menu_item, &theme_menu,
#ifdef HAVE_RECORDING
        &recording_settings,
#endif
#if CONFIG_RTC
        &timedate_item,
#endif
        &manage_settings,
        );
/*    MAIN MENU                    */
/***********************************/
