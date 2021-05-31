/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * mpegplayer audio thread implementation
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
#include "plugin.h"
#include "mpegplayer.h"
#include "codecs/libmad/bit.h"
#include "codecs/libmad/mad.h"

/** Audio stream and thread **/
struct pts_queue_slot;
struct audio_thread_data
{
    struct queue_event ev;  /* Our event queue to receive commands */
    int state;              /* Thread state */
    int status;             /* Media status (STREAM_PLAYING, etc.) */
    int mad_errors;         /* A count of the errors in each frame */
    unsigned samplerate;    /* Current stream sample rate */
    int nchannels;          /* Number of audio channels */
    struct dsp_config *dsp; /* The DSP we're using */
    struct dsp_buffer src;  /* Current audio data for DSP processing */
};

/* The audio thread is stolen from the core codec thread */
static struct event_queue audio_str_queue SHAREDBSS_ATTR;
static struct queue_sender_list audio_str_queue_send SHAREDBSS_ATTR;
struct stream audio_str IBSS_ATTR;

/* libmad related definitions */
static struct mad_stream stream IBSS_ATTR;
static struct mad_frame  frame IBSS_ATTR;
static struct mad_synth  synth IBSS_ATTR;

/*sbsample buffer for mad_frame*/
mad_fixed_t sbsample[2][36][32];

/* 2567 bytes */
static unsigned char mad_main_data[MAD_BUFFER_MDLEN];

/* There isn't enough room for this in IRAM on PortalPlayer, but there
   is for Coldfire. */

/* 4608 bytes */
#if defined(CPU_COLDFIRE) || defined(CPU_S5L870X)
static mad_fixed_t mad_frame_overlap[2][32][18] IBSS_ATTR;
#else
static mad_fixed_t mad_frame_overlap[2][32][18];
#endif

/** A queue for saving needed information about MPEG audio packets **/
#define AUDIODESC_QUEUE_LEN  (1 << 5) /* 32 should be way more than sufficient -
                                         if not, the case is handled */
#define AUDIODESC_QUEUE_MASK (AUDIODESC_QUEUE_LEN-1)
struct audio_frame_desc
{
    uint32_t time;  /* Time stamp for packet in audio ticks       */
    ssize_t  size;  /* Number of unprocessed bytes left in packet */
};

 /* This starts out wr == rd but will never be emptied to zero during
    streaming again in order to support initializing the first packet's
    timestamp without a special case */
struct
{
    /* Compressed audio data */
    uint8_t *start;  /* Start of encoded audio buffer */
    uint8_t *ptr;    /* Pointer to next encoded audio data */
    ssize_t used;    /* Number of bytes in MPEG audio buffer */
    /* Compressed audio data descriptors */
    unsigned read, write;
    struct audio_frame_desc *curr; /* Current slot */
    struct audio_frame_desc descs[AUDIODESC_QUEUE_LEN];
} audio_queue;

static inline int audiodesc_queue_count(void)
{
    return audio_queue.write - audio_queue.read;
}

static inline bool audiodesc_queue_full(void)
{
    return audio_queue.used >= MPA_MAX_FRAME_SIZE + MAD_BUFFER_GUARD ||
            audiodesc_queue_count() >= AUDIODESC_QUEUE_LEN;
}

/* Increments the queue tail postion - should be used to preincrement */
static inline void audiodesc_queue_add_tail(void)
{
    if (audiodesc_queue_full())
    {
        DEBUGF("audiodesc_queue_add_tail: audiodesc queue full!\n");
        return;
    }

    audio_queue.write++;
}

/* Increments the queue head position - leaves one slot as current */
static inline bool audiodesc_queue_remove_head(void)
{
    if (audio_queue.write == audio_queue.read)
        return false;

    audio_queue.read++;
    return true;
}

/* Returns the "tail" at the index just behind the write index */
static inline struct audio_frame_desc * audiodesc_queue_tail(void)
{
    return &audio_queue.descs[(audio_queue.write - 1) & AUDIODESC_QUEUE_MASK];
}

/* Returns a pointer to the current head */
static inline struct audio_frame_desc * audiodesc_queue_head(void)
{
    return &audio_queue.descs[audio_queue.read & AUDIODESC_QUEUE_MASK];
}

/* Resets the pts queue - call when starting and seeking */
static void audio_queue_reset(void)
{
    audio_queue.ptr = audio_queue.start;
    audio_queue.used = 0;
    audio_queue.read = 0;
    audio_queue.write = 0;
    rb->memset(audio_queue.descs, 0, sizeof (audio_queue.descs));
    audio_queue.curr = audiodesc_queue_head();
}

static void audio_queue_advance_pos(ssize_t len)
{
    audio_queue.ptr        += len;
    audio_queue.used       -= len;
    audio_queue.curr->size -= len;
}

static int audio_buffer(struct stream *str, enum stream_parse_mode type)
{
    int ret = STREAM_OK;

    /* Carry any overshoot to the next size since we're technically
       -size bytes into it already. If size is negative an audio
       frame was split across packets. Old has to be saved before
       moving the head. */
    if (audio_queue.curr->size <= 0 && audiodesc_queue_remove_head())
    {
        struct audio_frame_desc *old = audio_queue.curr;
        audio_queue.curr = audiodesc_queue_head();
        audio_queue.curr->size += old->size;
        old->size = 0;
    }

    /* Add packets to compressed audio buffer until it's full or the
     * timestamp queue is full - whichever happens first */
    while (!audiodesc_queue_full())
    {
        ret = parser_get_next_data(str, type);
        struct audio_frame_desc *curr;
        ssize_t len;

        if (ret != STREAM_OK)
            break;

        /* Get data from next audio packet */
        len = str->curr_packet_end - str->curr_packet;

        if (str->pkt_flags & PKT_HAS_TS)
        {
            audiodesc_queue_add_tail();
            curr = audiodesc_queue_tail();
            curr->time = TS_TO_TICKS(str->pts);
            /* pts->size should have been zeroed when slot was
               freed */
        }
        else
        {
            /* Add to the one just behind the tail - this may be
             * the head or the previouly added tail - whether or
             * not we'll ever reach this is quite in question
             * since audio always seems to have every packet
             * timestamped */
            curr = audiodesc_queue_tail();
        }

        curr->size += len;

        /* Slide any remainder over to beginning */
        if (audio_queue.ptr > audio_queue.start && audio_queue.used > 0)
        {
            rb->memmove(audio_queue.start, audio_queue.ptr,
                        audio_queue.used);
        }

        /* Splice this packet onto any remainder */
        rb->memcpy(audio_queue.start + audio_queue.used,
                   str->curr_packet, len);

        audio_queue.used += len;
        audio_queue.ptr = audio_queue.start;

        rb->yield();
    }

    return ret;
}

/* Initialise libmad */
static void init_mad(void)
{
    /* init the sbsample buffer */
    frame.sbsample_prev = &sbsample;
    frame.sbsample = &sbsample;

    /* We do this so libmad doesn't try to call codec_calloc(). This needs to
     * be called before mad_stream_init(), mad_frame_inti() and 
     * mad_synth_init(). */
    frame.overlap = &mad_frame_overlap;
    stream.main_data = &mad_main_data;

    /* Call mad initialization. Those will zero the arrays frame.overlap,
     * frame.sbsample and frame.sbsample_prev. Therefore there is no need to 
     * zero them here. */
    mad_stream_init(&stream);
    mad_frame_init(&frame);
    mad_synth_init(&synth);
}

/* Sync audio stream to a particular frame - see main decoder loop for
 * detailed remarks */
static int audio_sync(struct audio_thread_data *td,
                      struct str_sync_data *sd)
{
    int retval = STREAM_MATCH;
    uint32_t sdtime = TS_TO_TICKS(clip_time(&audio_str, sd->time));
    uint32_t time;
    uint32_t duration = 0;
    struct stream *str;
    struct stream tmp_str;
    struct mad_header header;
    struct mad_stream stream;

    if (td->ev.id == STREAM_SYNC)
    {
        /* Actually syncing for playback - use real stream */
        time = 0;
        str = &audio_str;
    }
    else
    {
        /* Probing - use temp stream */
        time = INVALID_TIMESTAMP;
        str = &tmp_str;
        str->id = audio_str.id;
    }        

    str->hdr.pos = sd->sk.pos;
    str->hdr.limit = sd->sk.pos + sd->sk.len;

    mad_stream_init(&stream);
    mad_header_init(&header);

    while (1)
    {
        if (audio_buffer(str, STREAM_PM_RANDOM_ACCESS) == STREAM_DATA_END)
        {
            DEBUGF("audio_sync:STR_DATA_END\n  aqu:%ld swl:%ld swr:%ld\n",
                    (long)audio_queue.used, str->hdr.win_left, str->hdr.win_right);
            if (audio_queue.used <= MAD_BUFFER_GUARD)
                goto sync_data_end;
        }

        stream.error = 0;
        mad_stream_buffer(&stream, audio_queue.ptr, audio_queue.used);

        if (stream.sync && mad_stream_sync(&stream) < 0)
        {
            DEBUGF(" audio: mad_stream_sync failed\n");
            audio_queue_advance_pos(MAX(audio_queue.curr->size - 1, 1));
            continue;
        }

        stream.sync = 0;

        if (mad_header_decode(&header, &stream) < 0)
        {
            DEBUGF(" audio: mad_header_decode failed:%s\n",
                   mad_stream_errorstr(&stream));
            audio_queue_advance_pos(1);
            continue;
        }

        duration = 32*MAD_NSBSAMPLES(&header);
        time = audio_queue.curr->time;

        DEBUGF(" audio: ft:%u t:%u fe:%u nsamp:%u sampr:%u\n",
               (unsigned)TICKS_TO_TS(time), (unsigned)sd->time,
               (unsigned)TICKS_TO_TS(time + duration),
               (unsigned)duration, header.samplerate);

        audio_queue_advance_pos(stream.this_frame - audio_queue.ptr);

        if (time <= sdtime && sdtime < time + duration)
        {
            DEBUGF(" audio: ft<=t<fe\n");
            retval = STREAM_PERFECT_MATCH;
            break;
        }
        else if (time > sdtime)
        {
            DEBUGF(" audio: ft>t\n");
            break;
        }

        audio_queue_advance_pos(stream.next_frame - audio_queue.ptr);
        audio_queue.curr->time += duration;

        rb->yield();
    }

sync_data_end:
    if (td->ev.id == STREAM_FIND_END_TIME)
    {
        if (time != INVALID_TIMESTAMP)
        {
            time = TICKS_TO_TS(time);
            duration = TICKS_TO_TS(duration);
            sd->time = time + duration;
            retval = STREAM_PERFECT_MATCH;
        }
        else
        {
            retval = STREAM_NOT_FOUND;
        }
    }

    DEBUGF(" audio header: 0x%02X%02X%02X%02X\n",
           (unsigned)audio_queue.ptr[0], (unsigned)audio_queue.ptr[1],
           (unsigned)audio_queue.ptr[2], (unsigned)audio_queue.ptr[3]);

    return retval;
    (void)td;
}

static void audio_thread_msg(struct audio_thread_data *td)
{
    while (1)
    {
        intptr_t reply = 0;

        switch (td->ev.id)
        {
        case STREAM_PLAY:
            td->status = STREAM_PLAYING;

            switch (td->state)
            {
            case TSTATE_INIT:
                td->state = TSTATE_DECODE;
            case TSTATE_DECODE:
            case TSTATE_RENDER_WAIT:
                break;

            case TSTATE_EOS:
                /* At end of stream - no playback possible so fire the
                 * completion event */
                stream_generate_event(&audio_str, STREAM_EV_COMPLETE, 0);
                break;
            }

            break;

        case STREAM_PAUSE:
            td->status = STREAM_PAUSED;
            reply = td->state != TSTATE_EOS;
            break;

        case STREAM_STOP:
            if (td->state == TSTATE_DATA)
                stream_clear_notify(&audio_str, DISK_BUF_DATA_NOTIFY);

            td->status = STREAM_STOPPED;
            td->state = TSTATE_EOS;

            reply = true;
            break;            

        case STREAM_RESET:
            if (td->state == TSTATE_DATA)
                stream_clear_notify(&audio_str, DISK_BUF_DATA_NOTIFY);

            td->status = STREAM_STOPPED;
            td->state = TSTATE_INIT;
            td->samplerate = 0;
            td->nchannels = 0;

            init_mad();
            td->mad_errors = 0;

            audio_queue_reset();

            reply = true;
            break;

        case STREAM_NEEDS_SYNC:
            reply = true; /* Audio always needs to */
            break;

        case STREAM_SYNC:
        case STREAM_FIND_END_TIME:
            if (td->state != TSTATE_INIT)
                break;

            reply = audio_sync(td, (struct str_sync_data *)td->ev.data);
            break;

        case DISK_BUF_DATA_NOTIFY:
            /* Our bun is done */
            if (td->state != TSTATE_DATA)
                break;

            td->state = TSTATE_DECODE;
            str_data_notify_received(&audio_str);
            break;

        case STREAM_QUIT:
            /* Time to go - make thread exit */
            td->state = TSTATE_EOS;
            return;
        }

        str_reply_msg(&audio_str, reply);

        if (td->status == STREAM_PLAYING)
        {
            switch (td->state)
            {
            case TSTATE_DECODE:
            case TSTATE_RENDER_WAIT:
                /* These return when in playing state */
                return;
            }
        }

        str_get_msg(&audio_str, &td->ev);
    }
}

static void audio_thread(void)
{
    struct audio_thread_data td;
#ifdef HAVE_PRIORITY_SCHEDULING
    /* Up the priority since the core DSP over-yields internally */
    int old_priority = rb->thread_set_priority(rb->thread_self(),
                                               PRIORITY_PLAYBACK-4);
#endif

    rb->memset(&td, 0, sizeof (td));
    td.status = STREAM_STOPPED;
    td.state = TSTATE_EOS;

    /* We need this here to init the EMAC for Coldfire targets */
    init_mad();

    td.dsp = rb->dsp_get_config(CODEC_IDX_AUDIO);
    rb->dsp_configure(td.dsp, DSP_SET_OUT_FREQUENCY, CLOCK_RATE);
#ifdef HAVE_PITCHCONTROL
    rb->sound_set_pitch(PITCH_SPEED_100);
    rb->dsp_set_timestretch(PITCH_SPEED_100);
#endif
    rb->dsp_configure(td.dsp, DSP_RESET, 0);
    rb->dsp_configure(td.dsp, DSP_FLUSH, 0);
    rb->dsp_configure(td.dsp, DSP_SET_SAMPLE_DEPTH, MAD_F_FRACBITS);

    goto message_wait;

    /* This is the decoding loop. */
    while (1)
    {
        td.state = TSTATE_DECODE;

        /* Check for any pending messages and process them */
        if (str_have_msg(&audio_str))
        {
        message_wait:
            /* Wait for a message to be queued */
            str_get_msg(&audio_str, &td.ev);

        message_process:
            /* Process a message already dequeued */
            audio_thread_msg(&td);

            switch (td.state)
            {
            /* These states are the only ones that should return */
            case TSTATE_DECODE:          goto audio_decode;
            case TSTATE_RENDER_WAIT:     goto render_wait;
            /* Anything else is interpreted as an exit */
            default:
            {
#ifdef HAVE_PRIORITY_SCHEDULING
                rb->thread_set_priority(rb->thread_self(), old_priority);
#endif
                return;
                }
            }
        }

    audio_decode:

        /** Buffering **/
        switch (audio_buffer(&audio_str, STREAM_PM_STREAMING))
        {
        case STREAM_DATA_NOT_READY:
        {
            td.state = TSTATE_DATA;
            goto message_wait;
            } /* STREAM_DATA_NOT_READY: */

        case STREAM_DATA_END:
        {
            if (audio_queue.used > MAD_BUFFER_GUARD)
                break; /* Still have frames to decode */

            /* Used up remainder of compressed audio buffer. Wait for
             * samples on PCM buffer to finish playing. */
            audio_queue_reset();

            while (1)
            {
                if (pcm_output_empty())
                {
                    td.state = TSTATE_EOS;
                    stream_generate_event(&audio_str, STREAM_EV_COMPLETE, 0);
                    break;
                }

                pcm_output_drain();
                str_get_msg_w_tmo(&audio_str, &td.ev, 1);

                if (td.ev.id != SYS_TIMEOUT)
                    break;
            }

            goto message_wait;
            } /* STREAM_DATA_END: */
        }

        /** Decoding **/
        mad_stream_buffer(&stream, audio_queue.ptr, audio_queue.used);

        int mad_stat = mad_frame_decode(&frame, &stream);

        ssize_t len = stream.next_frame - audio_queue.ptr;

        if (mad_stat != 0)
        {
            DEBUGF("audio: Stream error: %s\n",
                   mad_stream_errorstr(&stream));

            /* If something's goofed - try to perform resync by moving
             * at least one byte at a time */
            audio_queue_advance_pos(MAX(len, 1));

            if (stream.error == MAD_ERROR_BUFLEN)
            {
                /* This makes the codec support partially corrupted files */
                if (++td.mad_errors <= MPA_MAX_FRAME_SIZE)
                {
                    stream.error = 0;
                    rb->yield();
                    continue;
                }
                DEBUGF("audio: Too many errors\n");
            }
            else if (MAD_RECOVERABLE(stream.error))
            {
                /* libmad says it can recover - just keep on decoding */
                rb->yield();
                continue;
            }
            else
            {
                /* Some other unrecoverable error */
                DEBUGF("audio: Unrecoverable error\n");
            }

            /* This is too hard - bail out */
            td.state = TSTATE_EOS;
            td.status = STREAM_ERROR;
            stream_generate_event(&audio_str, STREAM_EV_COMPLETE, 0);

            goto message_wait;
        }

        /* Adjust sizes by the frame size */
        audio_queue_advance_pos(len);
        td.mad_errors = 0; /* Clear errors */

        /* Generate the pcm samples */
        mad_synth_frame(&synth, &frame);

        /** Output **/
        if (frame.header.samplerate != td.samplerate)
        {
            td.samplerate = frame.header.samplerate;
            rb->dsp_configure(td.dsp, DSP_SET_FREQUENCY,
                              td.samplerate);
        }

        if (MAD_NCHANNELS(&frame.header) != td.nchannels)
        {
            td.nchannels = MAD_NCHANNELS(&frame.header);
            rb->dsp_configure(td.dsp, DSP_SET_STEREO_MODE,
                              td.nchannels == 1 ?
                                STEREO_MONO : STEREO_NONINTERLEAVED);
        }

        td.src.remcount  = synth.pcm.length;
        td.src.pin[0]    = synth.pcm.samples[0];
        td.src.pin[1]    = synth.pcm.samples[1];
        td.src.proc_mask = 0;

        td.state  = TSTATE_RENDER_WAIT;

        /* Add a frame of audio to the pcm buffer. Maximum is 1152 samples. */
    render_wait:
        rb->yield();

        while (1)
        {
            struct dsp_buffer dst;
            dst.remcount = 0;
            dst.bufcount = MAX(td.src.remcount, 1024);

            ssize_t size = dst.bufcount * 2 * sizeof(int16_t);

            /* Wait for required amount of free buffer space */
            while ((dst.p16out = pcm_output_get_buffer(&size)) == NULL)
            {
                /* Wait one frame */
                int timeout = dst.bufcount*HZ / td.samplerate;
                str_get_msg_w_tmo(&audio_str, &td.ev, MAX(timeout, 1));
                if (td.ev.id != SYS_TIMEOUT)
                    goto message_process;
            }

            dst.bufcount = size / (2 * sizeof (int16_t));
            rb->dsp_process(td.dsp, &td.src, &dst);

            if (dst.remcount > 0)
            {
                /* Make this data available to DMA */
                pcm_output_commit_data(dst.remcount * 2 * sizeof(int16_t),
                                       audio_queue.curr->time);

                /* As long as we're on this timestamp, the time is just
                   incremented by the number of samples */
                audio_queue.curr->time += dst.remcount;
            }
            else if (td.src.remcount <= 0)
            {
                break;
            }
        }
    } /* end decoding loop */
}

/* Initializes the audio thread resources and starts the thread */
bool audio_thread_init(void)
{
    /* Initialise the encoded audio buffer and its descriptors */
    audio_queue.start = mpeg_malloc(AUDIOBUF_ALLOC_SIZE,
                                    MPEG_ALLOC_AUDIOBUF);
    if (audio_queue.start == NULL)
        return false;

    /* Start the audio thread */
    audio_str.hdr.q = &audio_str_queue;
    rb->queue_init(audio_str.hdr.q, false);

    /* We steal the codec thread for audio */
    rb->codec_thread_do_callback(audio_thread, &audio_str.thread);

    rb->queue_enable_queue_send(audio_str.hdr.q, &audio_str_queue_send,
                                audio_str.thread);

    /* Wait for thread to initialize */
    str_send_msg(&audio_str, STREAM_NULL, 0);

    return true;
}

/* Stops the audio thread */
void audio_thread_exit(void)
{
    if (audio_str.thread != 0)
    {
        str_post_msg(&audio_str, STREAM_QUIT, 0);
        rb->codec_thread_do_callback(NULL, NULL);
        audio_str.thread = 0;
    }
}
