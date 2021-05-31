/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 by Christian Gmeiner
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

#ifndef _TLV320_H_
#define _TLV320_H_

#define AUDIOHW_CAPS    (LIN_GAIN_CAP | MIC_GAIN_CAP)

AUDIOHW_SETTING(VOLUME,     "dB", 0,  1, -74,   6, -20)
#ifdef HAVE_RECORDING
/* Line in   : 0 .. 31 => Volume -34.5 .. +12 dB
 * Mic (left): 0 ..  1 => Volume  +0,     +20 dB */
AUDIOHW_SETTING(LEFT_GAIN,  "dB", 1,  1,   0,  31,  23, (val - 23) * 15)
AUDIOHW_SETTING(RIGHT_GAIN, "dB", 1,  1,   0,  31,  23, (val - 23) * 15)
/* 0 or 20 dB */
AUDIOHW_SETTING(MIC_GAIN,   "dB", 1,  1,   0,   1,   1, val * 200)
#endif /* HAVE_RECORDING */

/*** definitions ***/
#define HEADPHONE_MUTE 0x30 /* 0110000 = -73db */

/* registers */
/* REG_LLIV: Left line input channel volume control */
#define REG_LLIV        0x0
#define LLIV_LRS        (1 << 8)    /* simultaneous volume/mute update  */
#define LLIV_LIM        (1 << 7)    /* Left line input mute             */
#define LLIV_LIV(x)     ((x) & 0x1f)/* Left line input volume control   */

/* REG_RLIV: Right line input channel volume control */
#define REG_RLIV        0x1
#define RLIV_RLS        (1 << 8)    /* simultaneous volume/mute update  */
#define RLIV_RIM        (1 << 7)    /* Right line input mute            */
#define RLIV_RIV(x)     ((x) & 0x1f)/* Right line input volume control  */

/* REG_LHV: Left Channel Headphone Volume Control */
#define REG_LHV         0x2
#define LHV_LRS         (1 << 8)    /* simultaneous volume/mute update */
#define LHV_LZC         (1 << 7)    /* Left-channel zero-cross detect  */
#define LHV_LHV(x)      ((x) & 0x7f)/* Left headphone volume control    */

/* REG_RHV: Right Channel Headphone Volume Control */
#define REG_RHV         0x3
#define RHV_LRS         (1 << 8)    /* simultaneous volume/mute update  */
#define RHV_RZC         (1 << 7)    /* Right-channel zero-cross detect  */
#define RHV_RHV(x)      ((x) & 0x7f)/* Right headphone volume control   */

/* REG_AAP: Analog Audio Path Control */
#define REG_AAP         0x4
#define AAP_DAC         (1 << 4)    /* DAC select           */
#define AAP_BYPASS      (1 << 3)    /* bypass               */
#define AAP_INSEL       (1 << 2)    /* Input select for ADC */
#define AAP_MICM        (1 << 1)    /* Microphone mute      */
#define AAP_MICB        (1 << 0)    /* Microphone boost     */

/* REG_DAP: Digital Audio Path Control */
#define REG_DAP         0x5
#define DAP_DACM        (1 << 3)    /* DAC soft mute                    */
#define DAP_DEEMP_32    (1 << 1)    /* De-emphasis control: 32 kHz      */
#define DAP_DEEMP_44    (2 << 1)    /* De-emphasis control: 44.1 kHz    */
#define DAP_DEEMP_48    (3 << 1)    /* De-emphasis control: 48 kHz      */
#define DAP_ADCHP       (1 << 0)    /* ADC high-pass filter             */

/* REG_PC: Power Down Control */
#define REG_PC          0x6
#define PC_OFF          (1 << 7)    /* Device power     */
#define PC_CLK          (1 << 6)    /* Clock            */
#define PC_OSC          (1 << 5)    /* Oscillator       */
#define PC_OUT          (1 << 4)    /* Outputs          */
#define PC_DAC          (1 << 3)    /* DAC              */
#define PC_ADC          (1 << 2)    /* ADC              */
#define PC_MIC          (1 << 1)    /* Microphone input */
#define PC_LINE         (1 << 0)    /* Line input       */

/* REG_DAIF: Digital Audio Interface Format */
#define REG_DAIF        0x7
#define DAIF_MS         (1 << 6)    /* Master/slave mode                        */
#define DAIF_LRSWAP     (1 << 5)    /* DAC left/right swap                      */
#define DAIF_LRP        (1 << 4)    /* DAC left/right phase                     */
#define DAIF_IWL_16     (0 << 2)    /* Input bit length: 16 bit                 */
#define DAIF_IWL_20     (1 << 2)    /* Input bit length: 20 bit                 */
#define DAIF_IWL_24     (2 << 2)    /* Input bit length: 24 bit                 */
#define DAIF_IWL_32     (3 << 2)    /* Input bit length: 32 bit                 */
#define DAIF_FOR_DSP    (3 << 0)    /* Data format: DSP                         */
#define DAIF_FOR_I2S    (2 << 0)    /* Data format: I2S                         */
#define DAIF_FOR_MSBL   (1 << 0)    /* Data format: MSB first, left aligned     */
#define DAIF_FOR_MSBR   (0 << 0)    /* Data format: MSB first, right aligned    */

/* REG_SRC: Sample Rate Control */
#define REG_SRC         0x8
#define SRC_CLKIN       (1 << 6)    /* Clock input divider                          */
#define SRC_CLKOUT      (1 << 7)    /* Clock output divider                         */
/*#define SRC_SR        ()*/
#define SRC_BOSR        (1 << 1)    /* Base oversampling rate, depends on SRC_USB   */
#define SRC_USB         (1 << 0)    /* Clock mode select                            */

/* REG_DIA: Digital Interface Activation */
#define REG_DIA         0x9
#define DIA_ACT         (1 << 0)    /* Activate interface */

/* REG_RR: Reset Register */
#define REG_RR          0xf
#define RR_RESET        0           /* Reset */

#endif /*_TLV320_H_*/
