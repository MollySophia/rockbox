/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Linus Nielsen Feltzing
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
#ifndef RECORDING_H
#define RECORDING_H

#include "config.h"
#include "audio.h"

bool in_recording_screen(void);
bool recording_screen(bool no_source);
char *rec_create_filename(char *buf);
int rec_create_directory(void);
void settings_apply_trigger(void);

/* If true, start recording automatically when recording_sreen() is entered */
extern bool recording_start_automatic;

/* handles device powerup, sets audio source and peakmeter mode */
void rec_set_source(int source, unsigned flags);

/* Initializes a recording_options structure with global settings.
   pass returned data to audio_set_recording_options or 
   rec_set_recording_options */
void rec_init_recording_options(struct audio_recording_options *options);
/* steals mp3 buffer, sets source and then options */
void rec_set_recording_options(struct audio_recording_options *options);

enum recording_command
{
    RECORDING_CMD_STOP,
    RECORDING_CMD_START, /* steal mp3 buffer, create unique filename and
                                                              start recording */
    RECORDING_CMD_START_NEWFILE, /* create unique filename and start recording*/
    RECORDING_CMD_PAUSE,
    RECORDING_CMD_RESUME,
    RECORDING_CMD_STOP_SHUTDOWN /* stop recording and shutdown */
};

/* centralized way to start/stop/... recording */
void rec_command(enum recording_command rec_cmd);

#endif /* RECORDING_H */
