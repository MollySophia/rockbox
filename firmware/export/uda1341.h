/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2009 by Bob Cousins
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

#ifndef _UDA1341_H
#define _UDA1341_H

#define AUDIOHW_CAPS    (BASS_CAP | TREBLE_CAP | MIC_GAIN_CAP | LIN_GAIN_CAP\
                         | MONO_VOL_CAP)

AUDIOHW_SETTING(VOLUME,     "dB", 0,  1, -61,   0, -25)
AUDIOHW_SETTING(BASS,       "dB", 0,  2,   0,  24,   0)
AUDIOHW_SETTING(TREBLE,     "dB", 0,  2,   0,   6,   0)
#ifdef HAVE_RECORDING
AUDIOHW_SETTING(LEFT_GAIN,  "dB", 1,  1,-128,  96,   0)
AUDIOHW_SETTING(RIGHT_GAIN, "dB", 1,  1,-128,  96,   0)
AUDIOHW_SETTING(MIC_GAIN,   "dB", 1,  1,-128, 108,  16)
#endif /* HAVE_RECORDING */

/* These are logical register numbers for driver */ 
enum uda_register {
    UDA_REG_ID_STATUS_0,
    UDA_REG_ID_STATUS_1,
    UDA_REG_ID_CTRL0,
    UDA_REG_ID_CTRL1,
    UDA_REG_ID_CTRL2,
    UDA_REG_ID_EXT_0,
    UDA_REG_ID_EXT_1,
    UDA_REG_ID_EXT_2,
    UDA_REG_ID_EXT_4,
    UDA_REG_ID_EXT_5,
    UDA_REG_ID_EXT_6,
    NUM_REG_ID
};


/* Address byte */
#define UDA1341_ADDR            0x14
#define UDA_REG_DATA0           0x00
#define UDA_REG_DATA1           0x01
#define UDA_REG_STATUS          0x02

/* STATUS */
#define UDA_STATUS_0            (0 << 7)
#define UDA_STATUS_1            (1 << 7)

#define UDA_RESET               (1 << 6)
#define UDA_SYSCLK_512FS        (0 << 4)
#define UDA_SYSCLK_384FS        (1 << 4)
#define UDA_SYSCLK_256FS        (2 << 4)
#define I2S_IFMT_IIS            (0 << 1)
#define I2S_IFMT_LSB16          (1 << 1)
#define I2S_IFMT_LSB18          (2 << 1)
#define I2S_IFMT_LSB20          (3 << 1)
#define I2S_IFMT_MSB            (4 << 1)
#define I2S_IFMT_LSB16_OFMT_MSB (5 << 1)
#define I2S_IFMT_LSB18_OFMT_MSB (6 << 1)
#define I2S_IFMT_LSB20_OFMT_MSB (7 << 1)
#define UDA_DC_FILTER           (1 << 0)

#define UDA_OUTPUT_GAIN         (1 << 6)
#define UDA_INPUT_GAIN          (1 << 5)
#define UDA_ADC_INVERT          (1 << 4)
#define UDA_DAC_INVERT          (1 << 3)
#define UDA_DOUBLE_SPEED        (1 << 2)
#define UDA_POWER_ADC_ON        (1 << 1)
#define UDA_POWER_DAC_ON        (1 << 0)

/* DATA0 */
#define UDA_DATA_CTRL0          (0 << 6)    /* volume */
#define UDA_DATA_CTRL1          (1 << 6)    /* bass, treble */
#define UDA_DATA_CTRL2          (2 << 6)    /* peak det pos, de-emp, mute */
#define UDA_DATA_EXT_ADDR       (6 << 5)
#define UDA_DATA_EXT_DATA       (7 << 5)

#define UDA_VOLUME(x)           ((x) << 0)  /* 1=0dB, 61=-60dB */

#define UDA_BASS_BOOST(x)       ((x) << 2)  /* see datasheet */
#define UDA_BASS_BOOST_MASK     0x0F  
#define UDA_TREBLE(x)           ((x) << 0)  /* see datasheet */
#define UDA_TREBLE_MASK         0x03  

#define UDA_PEAK_DETECT_POS_BEFORE  (0 << 5)
#define UDA_PEAK_DETECT_POS_AFTER   (1 << 5)
#define UDA_DE_EMPHASIS_NONE    (0 << 3)
#define UDA_DE_EMPHASIS_32      (1 << 3)
#define UDA_DE_EMPHASIS_44_1    (2 << 3)
#define UDA_DE_EMPHASIS_48      (3 << 3)
#define UDA_MUTE_ON                 (1 << 2)
#define UDA_MUTE_OFF                (0 << 2)
#define UDA_MODE_SWITCH_FLAT    (0 << 0)
#define UDA_MODE_SWITCH_MIN     (1 << 0)
#define UDA_MODE_SWITCH_MAX     (3 << 0)

#define UDA_EXT_0               (0 << 5)    /* Mixer Gain Chan 1 */
#define UDA_EXT_1               (1 << 5)    /* Mixer Gain Chan 2 */
#define UDA_EXT_2               (2 << 5)    /* Mic sens and mixer mode */
#define UDA_EXT_4               (4 << 5)    /* AGC, Input amp gain */
#define UDA_EXT_5               (5 << 5)    /* Input amp gain */
#define UDA_EXT_6               (6 << 5)    /* AGC settings */

/* TODO: DATA0 extended registers */

/* DATA1: see datasheet */

#endif /* _UDA_1341_H */
