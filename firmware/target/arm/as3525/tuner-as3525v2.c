/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Multi-tuner detection module to select between the Si4700 and the RDA5802
 *
 * Copyright (C) 2010 Bertrik Sikken
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
#include "config.h"
#include "tuner.h"

#ifdef CONFIG_TUNER_MULTI
int tuner_detect_type(void)
{
#if (CONFIG_TUNER & SI4700)
    if (si4700_detect()) {
        return SI4700;
    }
#endif
#if (CONFIG_TUNER & RDA5802)
    if (rda5802_detect()) {
        return RDA5802;
    }
#endif
    return RDA5802; /* returning 0 freezes the player : FS#11791 */
}
#endif
