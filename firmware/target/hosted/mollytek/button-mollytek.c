/***************************************************************************
 *             __________               __   ___
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 * Copyright (C) 2017 Marcin Bukat
 * Copyright (C) 2018 Roman Stolyarov
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
#include <poll.h>
//#include <dir.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/input.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "sysfs.h"
#include "button.h"
#include "button-target.h"
#include "panic.h"

#include "kernel.h"
#include "backlight.h"
#include "backlight-target.h"

int input_fd;

static int button_map(int keycode) {
    switch (keycode)
    {
    case KEY_ESC:
        return BUTTON_LEFT;
    
    case KEY_ENTER:
        return BUTTON_RIGHT;

    case KEY_UP:
        return BUTTON_UP;

    case KEY_DOWN:
        return KEY_DOWN;
    
    case KEY_VOLUMEUP:
        return BUTTON_VOL_UP;

    case KEY_VOLUMEDOWN:
        return BUTTON_VOL_DOWN;

    default:
        return 0;
    }
}

void button_init_device(void) {
    input_fd = open("/dev/input/event0", O_RDWR);

    if(input_fd < 0) {
        panicf("Cannot open input device");
    }
}

int button_read_device(void) {
    static int button_bitmap = 0;
    struct input_event event;

    int size = read(input_fd, &event, sizeof(event));
    if(size == (int)sizeof(event)) {
        int keycode = event.code;
        bool press = event.value ? true : false;

        if(press) {
            button_bitmap |= button_map(keycode);
        }
        else {
            button_bitmap &= ~button_map(keycode);
        }
    }

    return button_bitmap;
}

bool headphones_inserted(void) {
    return true; // no hardware detection
}

void button_close_device(void) {
    close(input_fd);
}

