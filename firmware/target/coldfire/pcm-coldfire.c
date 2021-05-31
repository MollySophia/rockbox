/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Michael Sevakis
 * Copyright (C) 2005 by Linus Nielsen Feltzing
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
#include <stdlib.h>
#include "system.h"
#include "kernel.h"
#include "logf.h"
#include "audio.h"
#include "sound.h"
#if defined(HAVE_SPDIF_REC) || defined(HAVE_SPDIF_OUT)
#include "spdif.h"
#endif
#include "pcm-internal.h"

#define IIS_PLAY_DEFPARM ( (freq_ent[FPARM_CLOCKSEL] << 12) | \
                           (IIS_PLAY & (7 << 8)) | \
                           (4 << 2) )  /* 64 bit clocks / word clock */
#define IIS_FIFO_RESET   (1 << 11)
#define PDIR2_FIFO_RESET (1 << 9)

#if defined(IAUDIO_X5) || defined(IAUDIO_M5) || defined(IAUDIO_M3)
#define IIS_PLAY         IIS1CONFIG
#else
#define IIS_PLAY         IIS2CONFIG
#endif

#ifdef HAVE_SPDIF_OUT
/* EBU TX auto sync, PDIR2 fifo auto sync, IIS1 fifo auto sync */
#define AUDIOGLOB_DEFPARM   ((1 << 10) | (1 << 8) | (1 << 7))
#else
/* PDIR2 fifo auto sync, IIS1 fifo auto sync */
#define AUDIOGLOB_DEFPARM   ((1 << 8) | (1 << 7))
#endif

/** Sample rates **/
#define PLLCR_SET_AUDIO_BITS_DEFPARM \
            ((freq_ent[FPARM_CLSEL] << 28) | (1 << 22))

#define FPARM_CLOCKSEL       0
#define FPARM_CLSEL          1

/* SCLK = Fs * bit clocks per word
 * so SCLK should be Fs * 64
 *
 * CLOCKSEL sets SCLK freq based on Audio CLK
 * 0x0c SCLK = Audio CLK/2   88200 * 64 = 5644800 Hz
 * 0x06 SCLK = Audio CLK/4   44100 * 64 = 2822400 Hz
 * 0x04 SCLK = Audio CLK/8   22050 * 64 = 1411200 Hz
 * 0x02 SCLK = Audio CLK/16  11025 * 64 = 705600 Hz
 *
 * CLSEL sets MCLK1/2 DAC freq based on XTAL freq
 * 0x01 MCLK1/2 = XTAL freq
 * 0x02 MCLK1/2 = XTAL/2 freq
 *
 * Audio CLK can be XTAL freq or XTAL/2 freq  (bit22 in PLLCR)
 * we always set bit22 so Audio CLK is always XTAL freq
 */

#if CONFIG_CPU == MCF5249 && defined(HAVE_UDA1380)
static const unsigned char pcm_freq_parms[HW_NUM_FREQ][2] =
{
HW_HAVE_88_([HW_FREQ_88] = { 0x0c, 0x01 },)
    [HW_FREQ_44] = { 0x06, 0x01 },
    [HW_FREQ_22] = { 0x04, 0x02 },
    [HW_FREQ_11] = { 0x02, 0x02 },
};
#endif

#if CONFIG_CPU == MCF5249 && defined(HAVE_WM8750)
/* We run codec in master mode.
 * Codec can reconstruct all frequencies
 * from single 11.2896 MHz master clock
 */
static const unsigned char pcm_freq_parms[HW_NUM_FREQ][2] =
{
HW_HAVE_88_([HW_FREQ_88] = { 0x00, 0x01 },)
    [HW_FREQ_44] = { 0x00, 0x01 },
    [HW_FREQ_22] = { 0x00, 0x01 },
    [HW_FREQ_11] = { 0x00, 0x01 },
};

#endif

#if (CONFIG_CPU == MCF5250 || CONFIG_CPU == MCF5249) && defined(HAVE_TLV320)
static const unsigned char pcm_freq_parms[HW_NUM_FREQ][2] =
{
HW_HAVE_88_([HW_FREQ_88] = { 0x0c, 0x01 },)
    [HW_FREQ_44] = { 0x06, 0x01 },
    [HW_FREQ_22] = { 0x04, 0x01 },
    [HW_FREQ_11] = { 0x02, 0x02 },
};
#endif

static const unsigned char *freq_ent;

/* Lock status struct for playback and recording */
struct dma_lock
{
    int locked;
    unsigned long state;
};

static void iis_play_reset(void)
{
    or_l(IIS_FIFO_RESET, &IIS_PLAY);
    and_l(~IIS_FIFO_RESET, &IIS_PLAY);
    PDOR3 = 0;
}

static bool is_playback_monitoring(void)
{
    return (IIS_PLAY & (7 << 8)) == (3 << 8);
}

static void iis_play_reset_if_playback(bool if_playback)
{
    int level = set_irq_level(DMA_IRQ_LEVEL);
    if (is_playback_monitoring() == if_playback)
        iis_play_reset();
    restore_irq(level);
}

/* apply audio settings */
/* This clears the reset bit to enable monitoring immediately if monitoring
   recording sources or always if playback is in progress - we might be 
   switching samplerates on the fly */
void pcm_dma_apply_settings(void)
{
    int level = set_irq_level(DMA_IRQ_LEVEL);

    /* remember table entry */
    freq_ent = pcm_freq_parms[pcm_fsel];
 
    /* Reprogramming bits 15-12 requires FIFO to be in a reset
       condition - Users Manual 17-8, Note 11 */
    or_l(IIS_FIFO_RESET, &IIS_PLAY);
    /* Important for TLV320 - this must happen in the correct order
       or starting recording will sound absolutely awful once in
       awhile - audiohw_set_frequency then coldfire_set_pllcr_audio_bits
     */
    IIS_PLAY = IIS_PLAY_DEFPARM | IIS_FIFO_RESET;
    restore_irq(level);

    audiohw_set_frequency(pcm_fsel);
    coldfire_set_pllcr_audio_bits(PLLCR_SET_AUDIO_BITS_DEFPARM);

    level = set_irq_level(DMA_IRQ_LEVEL);

    IIS_PLAY = IIS_PLAY_DEFPARM;

    if ((DCR0 & DMA_EEXT) != 0 && is_playback_monitoring())
        PDOR3 = 0; /* Kick FIFO out of reset by writing to it */

    restore_irq(level);
} /* pcm_dma_apply_settings */

void pcm_play_dma_init(void)
{
    freq_ent = pcm_freq_parms[pcm_fsel];

    AUDIOGLOB = AUDIOGLOB_DEFPARM;
    DIVR0     = 54; /* DMA0 is mapped into vector 54 in system.c */
    and_l(0xffffff00, &DMAROUTE);
    or_l(DMA0_REQ_AUDIO_1, &DMAROUTE);
    DMACONFIG = 1;  /* DMA0Req = PDOR3, DMA1Req = PDIR2 */
    BCR0 = 0; /* No bytes waiting */
    ICR6 = (6 << 2); /* Enable interrupt at level 6, priority 0 */

    /* Setup Coldfire I2S before initializing hardware or changing
       other settings. */
    or_l(IIS_FIFO_RESET, &IIS_PLAY);
    IIS_PLAY = IIS_PLAY_DEFPARM | IIS_FIFO_RESET;
    audio_set_output_source(AUDIO_SRC_PLAYBACK);

    /* Initialize default register values. */
    audiohw_init();

    audio_input_mux(AUDIO_SRC_PLAYBACK, SRCF_PLAYBACK);

    audiohw_set_frequency(pcm_fsel);
    coldfire_set_pllcr_audio_bits(PLLCR_SET_AUDIO_BITS_DEFPARM);

#if defined(HAVE_SPDIF_REC) || defined(HAVE_SPDIF_OUT)
    spdif_init();
#endif
} /* pcm_play_dma_init */

void pcm_play_dma_postinit(void)
{
    audiohw_postinit();
    iis_play_reset();
}

/** DMA **/

/****************************************************************************
 ** Playback DMA transfer
 **/
/* For the locks, DMA interrupt must be disabled when manipulating the lock
   if the handler ever calls these - right now things are arranged so it
   doesn't */
static struct dma_lock dma_play_lock =
{
    .locked = 0,
    .state = (1 << 14)  /* bit 14 is DMA0 */
};

void pcm_play_lock(void)
{
    if (++dma_play_lock.locked == 1)
        coldfire_imr_mod(1 << 14, 1 << 14);
}

void pcm_play_unlock(void)
{
    if (--dma_play_lock.locked == 0)
        coldfire_imr_mod(dma_play_lock.state, 1 << 14);
}

/* Set up the DMA transfer that kicks in when the audio FIFO gets empty */
void pcm_play_dma_start(const void *addr, size_t size)
{
    /* Stop any DMA in progress */
    pcm_play_dma_stop();

    /* Set up DMA transfer  */
    SAR0 = (unsigned long)addr;   /* Source address      */
    DAR0 = (unsigned long)&PDOR3; /* Destination address */
    BCR0 = (unsigned long)size;   /* Bytes to transfer   */

    DCR0 = DMA_INT | DMA_EEXT | DMA_CS | DMA_AA | DMA_SINC |
           DMA_SSIZE(DMA_SIZE_LINE) | DMA_START;

    dma_play_lock.state = (0 << 14);
} /* pcm_play_dma_start */

/* Stops the DMA transfer and interrupt */
void pcm_play_dma_stop(void)
{
    and_l(~(DMA_EEXT | DMA_INT), &DCR0); /* per request and int OFF */
    BCR0 = 0; /* No bytes remaining */
    DSR0 = 1; /* Clear interrupt, errors, stop transfer */

    iis_play_reset_if_playback(true);

    dma_play_lock.state = (1 << 14);
} /* pcm_play_dma_stop */

/* DMA0 Interrupt is called when the DMA has finished transfering a chunk
   from the caller's buffer */
void DMA0(void) __attribute__ ((interrupt_handler, section(".icode")));
void DMA0(void)
{
    unsigned long res = DSR0;

    and_l(~(DMA_EEXT | DMA_INT), &DCR0); /* per request and int OFF */
    DSR0 = 1; /* Clear interrupt and errors */

    if (res & 0x70)
    {
        logf("DMA0 err: %02x", res);
#if 0
        logf("  SAR0: %08x", SAR0);
        logf("  DAR0: %08x", DAR0);
        logf("  BCR0: %08x", BCR0);
        logf("  DCR0: %08x", DCR0);
#endif
    }

    const void *addr;
    size_t size;

    if (pcm_play_dma_complete_callback((res & 0x70) ?
                                       PCM_DMAST_ERR_DMA : PCM_DMAST_OK,
                                       &addr, &size))
    {
        SAR0 = (unsigned long)addr;      /* Source address */
        BCR0 = (unsigned long)size;      /* Bytes to transfer */
        or_l(DMA_EEXT | DMA_INT, &DCR0); /* per request and int ON */

        pcm_play_dma_status_callback(PCM_DMAST_STARTED);
    }
    /* else inished playing */
} /* DMA0 */

const void * pcm_play_dma_get_peak_buffer(int *count)
{
    unsigned long addr, cnt;

    /* Make sure interrupt doesn't change the second value after we read the
     * first value. */
    int level = set_irq_level(DMA_IRQ_LEVEL);
    addr = SAR0;
    cnt = BCR0;
    restore_irq(level);

    *count = (cnt & 0xffffff) >> 2;
    return (void *)((addr + 2) & ~3);
} /* pcm_play_dma_get_peak_buffer */

#ifdef HAVE_RECORDING
/****************************************************************************
 ** Recording DMA transfer
 **/
static struct dma_lock dma_rec_lock =
{
    .locked = 0,
    .state = (1 << 15)  /* bit 15 is DMA1 */
};

/* For the locks, DMA interrupt must be disabled when manipulating the lock
   if the handler ever calls these - right now things are arranged so it
   doesn't */
void pcm_rec_lock(void)
{
    if (++dma_rec_lock.locked == 1)
        coldfire_imr_mod(1 << 15, 1 << 15);
}

void pcm_rec_unlock(void)
{
    if (--dma_rec_lock.locked == 0)
        coldfire_imr_mod(dma_rec_lock.state, 1 << 15);
}

void pcm_rec_dma_start(void *addr, size_t size)
{
    /* Stop any DMA in progress */
    pcm_rec_dma_stop();

    and_l(~PDIR2_FIFO_RESET, &DATAINCONTROL);

    /* Start the DMA transfer.. */
#ifdef HAVE_SPDIF_REC
    /* clear: ebu1cnew, valnogood, symbolerr, parityerr */
    INTERRUPTCLEAR = (1 << 25) | (1 << 24) | (1 << 23) | (1 << 22);
#endif

    SAR1 = (unsigned long)&PDIR2; /* Source address      */
    DAR1 = (unsigned long)addr;   /* Destination address */
    BCR1 = (unsigned long)size;   /* Bytes to transfer   */

    DCR1 = DMA_INT | DMA_EEXT | DMA_CS | DMA_AA | DMA_DINC |
           DMA_DSIZE(DMA_SIZE_LINE) | DMA_START;

    dma_rec_lock.state = (0 << 15);
} /* pcm_rec_dma_start */

void pcm_rec_dma_stop(void)
{
    and_l(~(DMA_EEXT | DMA_INT), &DCR1); /* per request and int OFF */
    DSR1 = 1; /* Clear interrupt, errors, stop transfer */
    BCR1 = 0; /* No bytes received */

    or_l(PDIR2_FIFO_RESET, &DATAINCONTROL);

    iis_play_reset_if_playback(false);

    dma_rec_lock.state = (1 << 15);
} /* pcm_rec_dma_stop */

void pcm_rec_dma_init(void)
{
    DIVR1     = 55; /* DMA1 is mapped into vector 55 in system.c */
    DMACONFIG = 1;  /* DMA0Req = PDOR3, DMA1Req = PDIR2 */
    and_l(0xffff00ff, &DMAROUTE);
    or_l(DMA1_REQ_AUDIO_2, &DMAROUTE);

    pcm_rec_dma_stop();

    /* Enable interrupt at level 6, priority 1 */
    ICR7 = (6 << 2) | (1 << 0);
} /* pcm_init_recording */

void pcm_rec_dma_close(void)
{
    pcm_rec_dma_stop();

    and_l(0xffff00ff, &DMAROUTE);
    ICR7 = 0x00;     /* Disable interrupt */
    dma_rec_lock.state = (0 << 15);
} /* pcm_rec_dma_close */

/* DMA1 Interrupt is called when the DMA has finished transfering a chunk
   into the caller's buffer */
void DMA1(void) __attribute__ ((interrupt_handler, section(".icode")));
void DMA1(void)
{
    unsigned long res = DSR1;
    enum pcm_dma_status status = PCM_DMAST_OK;

    and_l(~(DMA_EEXT | DMA_INT), &DCR1); /* per request and int OFF */
    DSR1 = 1;                            /* Clear interrupt and errors */

    if (res & 0x70)
    {
        status = PCM_DMAST_ERR_DMA;
        logf("DMA1 err: %02x", res);
#if 0
        logf("  SAR1: %08x", SAR1);
        logf("  DAR1: %08x", DAR1);
        logf("  BCR1: %08x", BCR1);
        logf("  DCR1: %08x", DCR1);
#endif
    }
#ifdef HAVE_SPDIF_REC
    else if (DATAINCONTROL == 0xc038 &&
        (INTERRUPTSTAT & ((1 << 23) | (1 << 22))))
    {
        /* reason: symbolerr, parityerr.
         * Ignore valnogood since several sources don't set it properly. */
        /* clear: ebu1cnew, symbolerr, parityerr */
        INTERRUPTCLEAR = (1 << 25) | (1 << 23) | (1 << 22);
        status = PCM_DMAST_ERR_SPDIF;
        logf("spdif err");
    }
#endif

    /* Inform PCM we have more data (or error) */
    void *addr;
    size_t size;

    if (pcm_rec_dma_complete_callback(status, &addr, &size))
    {
        DAR1 = (unsigned long)addr;      /* Destination address */
        BCR1 = (unsigned long)size;      /* Bytes to transfer */
        or_l(DMA_EEXT | DMA_INT, &DCR1); /* per request and int ON */

        pcm_rec_dma_status_callback(PCM_DMAST_STARTED);
    }
} /* DMA1 */

const void * pcm_rec_dma_get_peak_buffer(void)
{
    return (void *)(DAR1 & ~3);
} /* pcm_rec_dma_get_peak_buffer */
#endif
