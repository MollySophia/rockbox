/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 Maurus Cuelenaere
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


#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <stdbool.h>
#include <windows.h>

#include "MTP_DLL/MTP_DLL.h"

void usage(void)
{
    fprintf(stderr, "usage: sendfirm <local filename>\n");
}

int filesize(char* filename)
{
    FILE* fd;
    int tmp;
    fd = fopen(filename, "r");
    if(fd == NULL)
    {
        fprintf(stderr, "Error while opening %s!\n", filename);
        return -1;
    }
    fseek(fd, 0, SEEK_END);
    tmp = ftell(fd);
    fclose(fd);
    return tmp;
}

void callback(unsigned int progress, unsigned int max)
{
    unsigned int normalized = progress*1000/max;
    printf("Progress: %d.%d%%\r", normalized/10, normalized%10);
    fflush(stdout);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        return 1;
    }
    
    wchar_t *tmp;
    
    tmp = (LPWSTR)malloc(strlen(argv[1])*2+1);
    mbstowcs(tmp, argv[1], strlen(argv[1])*2+1);
    
    wprintf(tmp);
    printf("\n");
    
    fprintf(stdout, "Sending firmware...\n");
    
    if(mtp_sendnk(tmp, filesize(argv[1]), &callback))
        fprintf(stdout, "Firmware sent successfully!\n");
    else
        fprintf(stdout, "Error occured during sending!\n");
        
    free(tmp);

    exit(0);
}
