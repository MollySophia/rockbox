/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by Catalin Patulea
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

#include "audio.h"
#include "registers.h"

void audiohw_init(void)
{
    /* Configure McBSP */
    SPCR10 = 0; /* Receiver reset */
    SPCR20 = 3 << 4; /* Rate gen disabled, RINT=XSYNCERR, TX disabled for now */
    PCR0 = 1 << 1; /* Serial port pins, external frame sync, external clock,
                                           frame sync FSX is active-high,
                                           TX data sampled on falling clock */
    XCR10 = 0x00a0; /* 1 word per frame, 32 bits per word */
    XCR20 = 0; /* Single-phase, unexpected frame pulse restarts xfer,
                                        0-bit data delay */
}

void audiohw_start(void)
{
    /* Trigger first XEVT0 */
    SPCR20 |= 1;
}

void audiohw_stop(void)
{
    /* Reset the transmitter */
    SPCR20&=0xFFFE;
}

