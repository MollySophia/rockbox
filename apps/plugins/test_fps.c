/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 Peter D'Hoye
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
#include "lib/helper.h"
#include "lib/grey.h"
#include "lib/pluginlib_touchscreen.h"
#include "lib/pluginlib_exit.h"
#include "lib/pluginlib_actions.h"

/* this set the context to use with PLA */
static const struct button_mapping *plugin_contexts[] = { pla_main_ctx };


#define FPS_QUIT   PLA_EXIT
#define FPS_QUIT2  PLA_CANCEL

#define DURATION (2*HZ) /* longer duration gives more precise results */



/* Screen logging */
static int line;
static int max_line;
#ifdef HAVE_REMOTE_LCD
static int remote_line;
static int remote_max_line;
#endif

static void log_init(void)
{
    int h;

    rb->lcd_getstringsize("A", NULL, &h);
    max_line = LCD_HEIGHT / h;
    line = 0;
    rb->lcd_clear_display();
    rb->lcd_update();
#ifdef HAVE_REMOTE_LCD
    rb->lcd_remote_getstringsize("A", NULL, &h);
    remote_max_line = LCD_REMOTE_HEIGHT / h;
    remote_line = 0;
    rb->lcd_remote_clear_display();
    rb->lcd_remote_update();
#endif
}

static void log_text(char *text)
{
    rb->lcd_puts(0, line, text);
    if (++line >= max_line)
        line = 0;
    rb->lcd_update();
#ifdef HAVE_REMOTE_LCD
    rb->lcd_remote_puts(0, remote_line, text);
    if (++remote_line >= remote_max_line)
        remote_line = 0;
    rb->lcd_remote_update();
#endif
}

static int calc_tenth_fps(int framecount, long ticks)
{
    return (10*HZ) * framecount / ticks;
}

static void time_main_update(void)
{
    char str[32];     /* text buffer */
    long time_start;  /* start tickcount */
    long time_end;    /* end tickcount */
    int frame_count;
    int fps;

    const int part14_x = LCD_WIDTH/4;   /* x-offset for 1/4 update test */
    const int part14_w = LCD_WIDTH/2;   /* x-size for 1/4 update test */
    const int part14_y = LCD_HEIGHT/4;  /* y-offset for 1/4 update test */
    const int part14_h = LCD_HEIGHT/2;  /* y-size for 1/4 update test */

    log_text("Main LCD Update");

    /* Test 1: full LCD update */
    frame_count = 0;
    rb->sleep(0); /* sync to tick */
    time_start = *rb->current_tick;
    while((time_end = *rb->current_tick) - time_start < DURATION)
    {
        rb->lcd_update();
        frame_count++;
    }
    fps = calc_tenth_fps(frame_count, time_end - time_start);
    rb->snprintf(str, sizeof(str), "1/1: %d.%d fps", fps / 10, fps % 10);
    log_text(str);

    /* Test 2: quarter LCD update */
    frame_count = 0;
    rb->sleep(0); /* sync to tick */
    time_start = *rb->current_tick;
    while((time_end = *rb->current_tick) - time_start < DURATION)
    {
        rb->lcd_update_rect(part14_x, part14_y, part14_w, part14_h);
        frame_count++;
    }
    fps = calc_tenth_fps(frame_count, time_end - time_start);
    rb->snprintf(str, sizeof(str), "1/4: %d.%d fps", fps / 10, fps % 10);
    log_text(str);
}

#if defined(HAVE_LCD_COLOR) && (MEMORYSIZE > 2)

#if LCD_WIDTH >= LCD_HEIGHT
#define YUV_WIDTH LCD_WIDTH
#define YUV_HEIGHT LCD_HEIGHT
#else /* Assume the screen is rotated on portrait LCDs */
#define YUV_WIDTH LCD_HEIGHT
#define YUV_HEIGHT LCD_WIDTH
#endif

static unsigned char ydata[YUV_HEIGHT][YUV_WIDTH];
static unsigned char udata[YUV_HEIGHT/2][YUV_WIDTH/2];
static unsigned char vdata[YUV_HEIGHT/2][YUV_WIDTH/2];

static unsigned char * const yuvbuf[3] = {
    (void*)ydata,
    (void*)udata,
    (void*)vdata
};

static void make_gradient_rect(int width, int height)
{
    unsigned char vline[YUV_WIDTH/2];
    int x, y;

    width /= 2;
    height /= 2;

    for (x = 0; x < width; x++)
        vline[x] = (x << 8) / width;
    for (y = 0; y < height; y++)
    {
        rb->memset(udata[y], (y << 8) / height, width);
        rb->memcpy(vdata[y], vline, width);
    }
}

static void time_main_yuv(void)
{
    char str[32];     /* text buffer */
    long time_start;  /* start tickcount */
    long time_end;    /* end tickcount */
    int frame_count;
    int fps;

    const int part14_x = YUV_WIDTH/4;   /* x-offset for 1/4 update test */
    const int part14_w = YUV_WIDTH/2;   /* x-size for 1/4 update test */
    const int part14_y = YUV_HEIGHT/4;  /* y-offset for 1/4 update test */
    const int part14_h = YUV_HEIGHT/2;  /* y-size for 1/4 update test */
    
    log_text("Main LCD YUV");

    rb->memset(ydata, 128, sizeof(ydata)); /* medium grey */

    /* Test 1: full LCD update */
    make_gradient_rect(YUV_WIDTH, YUV_HEIGHT);

    frame_count = 0;
    rb->sleep(0); /* sync to tick */
    time_start = *rb->current_tick;
    while((time_end = *rb->current_tick) - time_start < DURATION)
    {
        rb->lcd_blit_yuv(yuvbuf, 0, 0, YUV_WIDTH,
                         0, 0, YUV_WIDTH, YUV_HEIGHT);
        frame_count++;
    }
    fps = calc_tenth_fps(frame_count, time_end - time_start);
    rb->snprintf(str, sizeof(str), "1/1: %d.%d fps", fps / 10, fps % 10);
    log_text(str);

    /* Test 2: quarter LCD update */
    make_gradient_rect(YUV_WIDTH/2, YUV_HEIGHT/2);

    frame_count = 0;
    rb->sleep(0); /* sync to tick */
    time_start = *rb->current_tick;
    while((time_end = *rb->current_tick) - time_start < DURATION)
    {
        rb->lcd_blit_yuv(yuvbuf, 0, 0, YUV_WIDTH,
                         part14_x, part14_y, part14_w, part14_h);
        frame_count++;
    }
    fps = calc_tenth_fps(frame_count, time_end - time_start);
    rb->snprintf(str, sizeof(str), "1/4: %d.%d fps", fps / 10, fps % 10);
    log_text(str);
}
#endif

#ifdef HAVE_REMOTE_LCD
static void time_remote_update(void)
{
    char str[32];     /* text buffer */
    long time_start;  /* start tickcount */
    long time_end;    /* end tickcount */
    int frame_count;
    int fps;

    const int part14_x = LCD_REMOTE_WIDTH/4;   /* x-offset for 1/4 update test */
    const int part14_w = LCD_REMOTE_WIDTH/2;   /* x-size for 1/4 update test */
    const int part14_y = LCD_REMOTE_HEIGHT/4;  /* y-offset for 1/4 update test */
    const int part14_h = LCD_REMOTE_HEIGHT/2;  /* y-size for 1/4 update test */

    log_text("Remote LCD Update");

    /* Test 1: full LCD update */
    frame_count = 0;
    rb->sleep(0); /* sync to tick */
    time_start = *rb->current_tick;
    while((time_end = *rb->current_tick) - time_start < DURATION)
    {
        rb->lcd_remote_update();
        frame_count++;
    }
    fps = calc_tenth_fps(frame_count, time_end - time_start);
    rb->snprintf(str, sizeof(str), "1/1: %d.%d fps", fps / 10, fps % 10);
    log_text(str);

    /* Test 2: quarter LCD update */
    frame_count = 0;
    rb->sleep(0); /* sync to tick */
    time_start = *rb->current_tick;
    while((time_end = *rb->current_tick) - time_start < DURATION)
    {
        rb->lcd_remote_update_rect(part14_x, part14_y, part14_w, part14_h);
        frame_count++;
    }
    fps = calc_tenth_fps(frame_count, time_end - time_start);
    rb->snprintf(str, sizeof(str), "1/4: %d.%d fps", fps / 10, fps % 10);
    log_text(str);
}
#endif

#if LCD_DEPTH < 4

GREY_INFO_STRUCT_IRAM
static unsigned char greydata[LCD_HEIGHT][LCD_WIDTH];

static void make_grey_rect(int width, int height)
{
    unsigned char vline[LCD_WIDTH];
    int x, y;

    for (x = 0; x < width; x++)
        vline[x] = (x << 8) / width;
    for (y = 0; y < height; y++)
        rb->memcpy(greydata[y], vline, width);
}

static void time_greyscale(void)
{
    char str[32];     /* text buffer */
    long time_start;  /* start tickcount */
    long time_end;    /* end tickcount */
    long time_1, time_2;
    int frames_1, frames_2;
    int fps, load;
    size_t gbuf_size;
    unsigned char *gbuf = (unsigned char *) rb->plugin_get_buffer(&gbuf_size);

#if NUM_CORES > 1
    int i;
    for (i = 0; i < NUM_CORES; i++)
    {
        rb->snprintf(str, sizeof(str), "Greyscale (%s)",
                     (i > 0) ? "COP" : "CPU");
        log_text(str);
#else
    const int i = 0;
    log_text("Greyscale library");
    {
#endif

        if (!grey_init(gbuf, gbuf_size, (i > 0) ? GREY_ON_COP : 0,
                       LCD_WIDTH, LCD_HEIGHT, NULL))
        {
            log_text("greylib: out of memory.");
            return;
        }
        make_grey_rect(LCD_WIDTH, LCD_HEIGHT);

        /* Test 1 - greyscale overlay not yet enabled */
        frames_1 = 0;
        rb->sleep(0); /* sync to tick */
        time_start = *rb->current_tick;
        while((time_end = *rb->current_tick) - time_start < DURATION)
        {
            grey_ub_gray_bitmap(greydata[0], 0, 0, LCD_WIDTH, LCD_HEIGHT);
            frames_1++;
        }
        time_1 = time_end - time_start;
    
        /* Test 2 - greyscale overlay enabled */
        grey_show(true);
        frames_2 = 0;
        rb->sleep(0); /* sync to tick */
        time_start = *rb->current_tick;
        while((time_end = *rb->current_tick) - time_start < DURATION)
        {
            grey_ub_gray_bitmap(greydata[0], 0, 0, LCD_WIDTH, LCD_HEIGHT);
            frames_2++;
        }
        time_2 = time_end - time_start;

        grey_release();
        fps = calc_tenth_fps(frames_2, time_2);
        load = 100 - (100 * frames_2 * time_1) / (frames_1 * time_2);
        rb->snprintf(str, sizeof(str), "1/1: %d.%d fps", fps / 10, fps % 10);
        log_text(str);

        if (load > 0 && load < 100)
        {
            rb->snprintf(str, sizeof(str), "CPU load: %d%%", load);
            log_text(str);
        }
        else
            log_text("CPU load err (boost?)");
    }
}
#endif

void plugin_quit(void)
{
#ifdef HAVE_TOUCHSCREEN
    static struct touchbutton button[] = {{
            .action = ACTION_STD_OK,
            .title = "OK",
            /* .vp runtime initialized, rest false/NULL */
    }};
    struct viewport *vp = &button[0].vp;
    struct screen *lcd = rb->screens[SCREEN_MAIN];
    rb->viewport_set_defaults(vp, SCREEN_MAIN);
    const int border = 10;
    const int height = 50;

    lcd->set_viewport(vp);
    /* button matches the bottom center in the grid */
    vp->x = lcd->lcdwidth/3;
    vp->width = lcd->lcdwidth/3;
    vp->height = height;
    vp->y = lcd->lcdheight - height - border;

    touchbutton_draw(button, ARRAYLEN(button));
    lcd->update_viewport();
    if (rb->touchscreen_get_mode() == TOUCHSCREEN_POINT)
    {
        while(touchbutton_get(button, ARRAYLEN(button)) != ACTION_STD_OK);
    }
    else
#endif
        while (1)
        {
            int btn = pluginlib_getaction(TIMEOUT_BLOCK, plugin_contexts,
                                                   ARRAYLEN(plugin_contexts));
            exit_on_usb(btn);
            if ((btn == FPS_QUIT) || (btn == FPS_QUIT2))
                break;
        }
}

/* plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
#if (CONFIG_PLATFORM & PLATFORM_NATIVE)
    char str[32];
    int cpu_freq;
#endif

    /* standard stuff */
    (void)parameter;
    
#ifdef HAVE_TOUCHSCREEN
    rb->touchscreen_set_mode(rb->global_settings->touch_mode);
#endif

    log_init();
#if (CONFIG_PLATFORM & PLATFORM_NATIVE)
    cpu_freq = *rb->cpu_frequency; /* remember CPU frequency */
#endif
#ifdef HAVE_BACKLIGHT
    backlight_ignore_timeout();
#endif
    time_main_update();
    rb->sleep(HZ);
#if defined(HAVE_LCD_COLOR) && (MEMORYSIZE > 2)
    time_main_yuv();
#endif
#if LCD_DEPTH < 4
    time_greyscale();
#endif
#ifdef HAVE_REMOTE_LCD
    time_remote_update();
#endif

#if (CONFIG_PLATFORM & PLATFORM_NATIVE)
    if (*rb->cpu_frequency != cpu_freq)
        rb->snprintf(str, sizeof(str), "CPU clock changed!");
    else
        rb->snprintf(str, sizeof(str), "CPU: %d MHz",
                     (cpu_freq + 500000) / 1000000);
    log_text(str);
#endif
#ifdef HAVE_BACKLIGHT
    backlight_use_settings();
#endif
    /* wait until user closes plugin */
    plugin_quit();

    return PLUGIN_OK;
}
