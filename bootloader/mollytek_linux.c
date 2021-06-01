/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 *
 * Copyright (C) 2016 by Amaury Pouly
 *               2018 by Marcin Bukat
 *               2018 by Roman Stolyarov
 *
 * Based on Rockbox iriver bootloader by Linus Nielsen Feltzing
 * and the ipodlinux bootloader by Daniel Palffy and Bernard Leach
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

#include "system.h"
#include "lcd.h"
#include "backlight.h"
#include "button-target.h"
#include "button.h"
#include "../kernel/kernel-internal.h"
#include "core_alloc.h"
#include "filesystem-app.h"
#include "lcd.h"
#include "font.h"
#include "power.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdarg.h>
#include "version.h"

enum boot_mode {
    BOOT_ROCKBOX,
    BOOT_TOOLS,
    BOOT_COUNT,
    BOOT_USB,
    BOOT_STOP,
};

static void display_text_center(int y, const char *text) {
    int width;
    lcd_getstringsize(text, &width, NULL);
    lcd_putsxy(LCD_WIDTH / 2 - width / 2, y, text);
}

static void display_text_centerf(int y, const char *format, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, format);

    vsnprintf(buf, sizeof(buf), format, ap);
    display_text_center(y, buf);
}

static int get_inactivity_tmo(void) {
#if defined(HAS_BUTTON_HOLD)
    if(button_hold())
        return 5 * HZ;
    else
#endif
    return 10 * HZ;
}

static enum boot_mode inactivity_action(enum boot_mode cur_selection) {
#if defined(HAS_BUTTON_HOLD)
    if(button_hold())
        return BOOT_STOP;
    else
#endif
    return cur_selection;
}

static enum boot_mode load_boot_mode(enum boot_mode mode) {
    int fd = open("/data/rb_bl_mode.txt", O_RDONLY);
    if(fd >= 0) {
        read(fd, &mode, sizeof(mode));
        close(fd);
    }
    return mode;
}

static void save_boot_mode(enum boot_mode mode) {
    int fd = open("/data/rb_bl_mode.txt", O_RDWR | O_CREAT | O_TRUNC);
    if(fd >= 0) {
        write(fd, &mode, sizeof(mode));
        close(fd);
    }
}

static enum boot_mode get_boot_mode(void) {
    enum boot_mode init_mode = load_boot_mode(BOOT_ROCKBOX);
    enum boot_mode mode = init_mode;
    int last_activity = current_tick;
#if defined(HAS_BUTTON_HOLD)
    bool hold_status = button_hold();
#endif
    while(true) {
        // if(power_input_status() & POWER_INPUT_USB_CHARGER) {
        //     save_boot_mode(mode);
        //     return BOOT_USB;
        // }

        int timeout = last_activity + get_inactivity_tmo();
        if(TIME_AFTER(current_tick, timeout)) {
            save_boot_mode(mode);
            return inactivity_action(mode);
        }

        lcd_clear_display();

#if defined(HAS_BUTTON_HOLD)
        if(button_hold())
        {
            lcd_set_foreground(LCD_RGBPACK(255, 0, 0));
            display_text_center(0, "ON HOLD!");
        }
        else
#endif
        {
            lcd_set_foreground(LCD_RGBPACK(255, 201, 0));
            display_text_center(0, "SELECT PLAYER");
        }
        lcd_set_foreground(LCD_RGBPACK(255, 201, 0));

        const char *desc = (mode == BOOT_ROCKBOX) ? "ROCKBOX" : "TOOLS";
        int desc_height;
        lcd_getstringsize(desc, NULL, &desc_height);
        display_text_center(LCD_HEIGHT - 3*desc_height, desc);

         /* display arrows */
        int arrow_width, arrow_height;
        lcd_getstringsize("<", &arrow_width, &arrow_height);
        int arrow_y = get_icon_y() + ICON_HEIGHT / 2 - arrow_height / 2;
        lcd_putsxy(arrow_width / 2, arrow_y, "<");
        lcd_putsxy(LCD_WIDTH - 3 * arrow_width / 2, arrow_y, ">");

        lcd_set_foreground(LCD_RGBPACK(0, 255, 0));
        display_text_centerf(LCD_HEIGHT - arrow_height * 3 / 2, "timeout in %d sec",
            (timeout - current_tick + HZ - 1) / HZ);

        lcd_update();

        int btn = button_get_w_tmo(HZ / 10);

        if(btn & BUTTON_MAIN)
            last_activity = current_tick;
        
        if(btn & BUTTON_REL)
            continue;
        
        if(btn & BUTTON_REPEAT)
            btn &= ~BUTTON_REPEAT;
        
        if(btn == BUTTON_RIGHT)
            break;
        
        if(btn == BUTTON_UP || btn == BUTTON_VOL_UP)
            mode = (mode + BOOT_COUNT - 1) % BOOT_COUNT;
        if(btn == BUTTON_DOWN || btn == BUTTON_VOL_DOWN)
            mode = (mode + 1) % BOOT_COUNT;
    }

    save_boot_mode(mode);
    return mode;
}

void error_screen(const char *msg) {
    lcd_clear_display();
    lcd_putsf(0, 0, msg);
    lcd_update();
}

int choice_screen(const char *title, bool center, int nr_choices, const char *choices[])
{
    int choice = 0;
    int max_len = 0;
    int h;
    lcd_getstringsize("x", NULL, &h);
    for(int i = 0; i < nr_choices; i++)
    {
        int len = strlen(choices[i]);
        if(len > max_len)
            max_len = len;
    }
    char *buf = malloc(max_len + 10);
    int top_y = 2 * h;
    int nr_lines = (LCD_HEIGHT - top_y) / h;
    while(true)
    {
        /* make sure choice is visible */
        int offset = choice - nr_lines / 2;
        if(offset < 0)
            offset = 0;
        lcd_clear_display();
        /* display top text */
        lcd_set_foreground(LCD_RGBPACK(255, 201, 0));
        display_text_center(0, title);
        int line = 0;
        for(int i = 0; i < nr_choices && line < nr_lines; i++)
        {
            if(i < offset)
                continue;
            if(i == choice)
                lcd_set_foreground(LCD_RGBPACK(255, 0, 0));
            else
                lcd_set_foreground(LCD_RGBPACK(255, 201, 0));
            sprintf(buf, "%s", choices[i]);
            if(center)
                display_text_center(top_y + h * line, buf);
            else
                lcd_putsxy(0, top_y + h * line, buf);
            line++;
        }

        lcd_update();

        /* wait for a key  */
        int btn = button_get_w_tmo(HZ / 10);
        /* ignore release, allow repeat */
        if(btn & BUTTON_REL)
            continue;
        if(btn & BUTTON_REPEAT)
            btn &= ~BUTTON_REPEAT;
        /* play -> stop loop and return mode */
        if(btn == BUTTON_RIGHT)
        {
            free(buf);
            return btn == BUTTON_RIGHT ? choice : -1;
        }
        /* left/right/up/down: change mode */
        if(btn == BUTTON_UP || btn == BUTTON_VOL_UP)
            choice = (choice + nr_choices - 1) % nr_choices;
        if(btn == BUTTON_DOWN || btn == BUTTON_VOL_DOWN)
            choice = (choice + 1) % nr_choices;
    }
}

void run_file(const char *name) {
    char *dirname = "/mnt/sdcard/";
    char *buf = malloc(strlen(dirname) + strlen(name) + 1);
    sprintf(buf, "%s%s", dirname, name);

    lcd_clear_display();
    lcd_set_foreground(LCD_RGBPACK(255, 201, 0));
    lcd_putsf(0, 0, "Running %s", name);
    lcd_update();

    pid_t pid = fork();
    if(pid == 0) {
        execlp("sh", "sh", buf, NULL);
        _exit(42);
    }
    int status;
    waitpid(pid, &status, 0);
    if(WIFEXITED(status)) {
        lcd_set_foreground(LCD_RGBPACK(255, 201, 0));
        lcd_putsf(0, 1, "program returned %d", WEXITSTATUS(status));
    }
    else {
        lcd_set_foreground(LCD_RGBPACK(255, 0, 0));
        lcd_putsf(0, 1, "an error occured: %x", status);
    }
    lcd_set_foreground(LCD_RGBPACK(255, 0, 0));
    lcd_putsf(0, 3, "Press any key or wait");
    lcd_update();
    sleep(HZ);
    while(button_get(false) != =);

    button_get_w_two(4 * HZ);
}

void run_script_menu(void) {
    const char **entries = NULL;
    int nr_entries = 0;
    DIR *dir = opendir("/mnt/sdcard");
    struct dirent *ent;
    while((ent = readdir(dir))) {
        if(ent->d_type != DT_REG)
            continue;
        entries = realloc(entries, (nr_entries + 1) * sizeof(const char *));
        entries[nr_entries++] = strdup(ent->d_name);
    }
    closedir(dir);
    int idx = choice_screen("RUN SCRIPT", false, nr_entries, entries);
    if(idx >= 0)
        run_file(entries[idx]);
    for(int i = 0; i < nr_entries; i++)
        free((char *)entries[i]);
    free(entries);
}

static void adb(int start) {
    pid_t pid = fork();
    if(pid == 0) {
        execlp("/etc/init.d/K90adb", start ? "start" : "stop", NULL);
        _exit(42);
    }
    int status;
    waitpid(pid, &status, 0);
}

static void tools_screen(void)
{
    const char *choices[] = {"ADB start", "ADB stop", "Run script", "Restart"};
    int choice = choice_screen("TOOLS MENU", true, 4, choices);
    if(choice == 0)
    {
        /* run service menu */
        printf("Starting ADB service...\n");
        fflush(stdout);
        adb(1);
    }
    else if(choice == 1)
    {
        printf("Stopping ADB service...\n");
        fflush(stdout);
        adb(0);
    }
    else if(choice == 2)
    {
        run_script_menu();
    }
    else if(choice == 3)
        system_reboot();
}

#if 1
/* open log file */
static int open_log(void)
{
    /* open regular log file */
    int fd = open("/mnt/sdcard/rockbox.log", O_RDWR | O_CREAT | O_APPEND);
    /* get its size */
    struct stat stat;
    if(fstat(fd, &stat) != 0)
        return fd; /* on error, don't do anything */
    /* if file is too large, rename it and start a new log file */
    if(stat.st_size < 1000000)
        return fd;
    close(fd);
    /* move file */
    rename("/mnt/sdcard/rockbox.log", "/mnt/sdcard/rockbox.log.1");
    /* re-open the file, truncate in case the move was unsuccessful */
    return open("/mnt/sdcard/rockbox.log", O_RDWR | O_CREAT | O_APPEND | O_TRUNC);
}
#endif

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
#if 1
    /* redirect stdout and stderr to have error messages logged somewhere on the
     * user partition */
    int fd = open_log();
    if(fd >= 0)
    {
        dup2(fd, fileno(stdout));
        dup2(fd, fileno(stderr));
        close(fd);
    }
    /* print version */
    printf("Rockbox boot loader\n");
    printf("Version: %s\n", rbversion);
    printf("%s\n", MODEL_NAME);
#endif

    system_init();
    core_allocator_init();
    kernel_init();
    paths_init();
    lcd_init();
    font_init();
    button_init();
    backlight_init();
    backlight_set_brightness(DEFAULT_BRIGHTNESS_SETTING);

    /* try to load the extra font we install on the device */
    //int font_id = font_load("/usr/rockbox/fonts/20-Terminus-Bold.fnt");
    //if(font_id >= 0)
    //    lcd_setfont(font_id);

    /* run all tools menu */
    while(true)
    {
        enum boot_mode mode = get_boot_mode();
        if(mode == BOOT_USB)
        {

        }
        else if(mode == BOOT_TOOLS)
        {
            tools_screen();
        }
        else if(mode == BOOT_ROCKBOX)
        {
            fflush(stdout);
#if defined(MOLLYTEK_A0)
            execl("/usr/rockbox/rockbox.a0", "rockbox.a0", NULL);
#endif
            printf("execvp failed: %s\n", strerror(errno));
            /* fallback to OF in case of failure */
            error_screen("Cannot boot Rockbox");
            sleep(5 * HZ);
        }
        else
        {
            printf("suspend\n");
//            nwz_power_suspend();
        }
    }
    /* if we reach this point, everything failed, so return an error so that
     * sysmgrd knows something is wrong */
    return 1;
}