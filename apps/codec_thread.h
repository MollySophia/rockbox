/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 Miika Pekkarinen
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

#ifndef _CODEC_THREAD_H
#define _CODEC_THREAD_H

#include <stdbool.h>
#include "config.h"

/* codec identity */
const char *get_codec_filename(int cod_spec);

/* codec thread */
void codec_thread_init(void) INIT_ATTR;

/* Audio MUST be stopped before requesting callback! */
void codec_thread_do_callback(void (*fn)(void),
                              unsigned int *codec_thread_id);

#ifdef HAVE_PRIORITY_SCHEDULING
int codec_thread_get_priority(void);
int codec_thread_set_priority(int priority);
#endif

/* codec commands - on audio thread only! */
bool codec_load(int hid, int cod_spec);
void codec_go(void);
bool codec_pause(void);
void codec_seek(long time);
void codec_stop(void);
#ifdef HAVE_RECORDING
size_t codec_finish_stream(void);
#endif
void codec_unload(void);
int codec_loaded(void);

/* */

#endif /* _CODEC_THREAD_H */
