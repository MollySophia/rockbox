/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 by Michiel van der Kolk 
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
#include "searchengine.h"
#include "parser.h"
#include "token.h"
#include "dbinterface.h"



void *audio_bufferbase;
void *audio_bufferpointer;
size_t audio_buffer_free;
int w, h, y;

void *my_malloc(size_t size)
{
    void *alloc;

    if (!audio_bufferbase)
    {
        audio_bufferbase = audio_bufferpointer
            = rb->plugin_get_audio_buffer(&audio_buffer_free);
        audio_bufferpointer+=3;
        audio_bufferpointer=(void *)(((long)audio_bufferpointer)&~3);
        audio_buffer_free-=audio_bufferpointer-audio_bufferbase;
    }
    if (size + 4 > audio_buffer_free)
        return 0;
    alloc = audio_bufferpointer;
    audio_bufferpointer +=(size+3)&~3; // alignment
    audio_buffer_free -= (size+3)&~3;
    return alloc;
}

void setmallocpos(void *pointer) 
{
    audio_bufferpointer = pointer;
    audio_buffer_free = audio_bufferpointer - audio_bufferbase;
}

/* this is the plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
    unsigned char *result,buf[500];
    int parsefd,hits;

    audio_bufferbase=audio_bufferpointer=0;
    audio_buffer_free=0;

    /* now go ahead and have fun! */
    PUTS("SearchEngine v0.1");
    parsefd=rb->open(parameter,O_RDONLY);
    if(parsefd<0) {
        rb->splash(2*HZ,"Unable to open search tokenstream");
        return PLUGIN_ERROR;    
    }
    result=parse(parsefd);
    rb->snprintf(buf,250,"Retval: 0x%x",result);
    PUTS(buf);
    rb->close(parsefd);
    hits=0;
    if(result!=0) {
        int fd=rb->open("/search.m3u", O_WRONLY|O_CREAT|O_TRUNC, 0666);
        int i;
        for(i=0;i<rb->tagdbheader->filecount;i++)
            if(result[i]) {
                hits++;
                rb->fdprintf(fd,"%s\n",getfilename(i));
            }
        rb->close(fd);
    }
    rb->snprintf(buf,250,"Hits: %d",hits);
    rb->splash(HZ*3,buf);
    if (result!=0) {
        /* Return PLUGIN_USB_CONNECTED to force a file-tree refresh */
        return PLUGIN_USB_CONNECTED;
    }
    return PLUGIN_OK;
}
