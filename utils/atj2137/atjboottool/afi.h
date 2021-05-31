/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2017 Amaury Pouly
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
#ifndef __AFI_H__
#define __AFI_H__

#include <stdint.h>

uint32_t afi_checksum(void *ptr, size_t size);
/* Unpack an AFI file: the callback function will be called once for each file in the archive with
 * its name and content. If the callback returns a nonzero value, the function will stop and return
 * that value. Returns 0 on success */
typedef int (*afi_extract_callback_t)(const char *name, uint8_t *buf, size_t size);
int afi_unpack(uint8_t *buf, size_t size, afi_extract_callback_t cb);
/* Check if a file looks like an AFI file */
bool afi_check(uint8_t *buf, size_t size);

#endif /* __AFI_H__ */ 
