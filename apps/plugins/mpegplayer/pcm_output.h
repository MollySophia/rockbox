/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * PCM output buffer declarations
 *
 * Copyright (c) 2007 Michael Sevakis
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
#ifndef PCM_OUTPUT_H
#define PCM_OUTPUT_H

#define PCM_HDR_SIZE        (sizeof (struct pcm_frame_header))
struct pcm_frame_header   /* Header added to pcm data every time a decoded
                             audio frame is sent out */
{
    uint32_t size;        /* size of this frame - including header */
    uint32_t time;        /* timestamp for this frame in audio ticks */
    unsigned char data[]; /* open array of audio data */
} ALIGNED_ATTR(4);

bool pcm_output_init(void);
void pcm_output_exit(void);
void pcm_output_flush(void);
void pcm_output_set_clock(uint32_t time);
uint32_t pcm_output_get_clock(void);
uint32_t pcm_output_get_ticks(uint32_t *start);
void pcm_output_play_pause(bool play);
void pcm_output_stop(void);
void pcm_output_drain(void);
void * pcm_output_get_buffer(ssize_t *size);
bool pcm_output_commit_data(ssize_t size, uint32_t timestamp);
bool pcm_output_empty(void);

#endif /* PCM_OUTPUT_H */
