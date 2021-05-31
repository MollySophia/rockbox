/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id$
*
* Additional LCD routines not present in the rockbox core
* Drawing functions
*
* Copyright (C) 2005 Jens Arnold
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

#include "xlcd.h"

/* sort the given coordinates by increasing x value */
static void sort_points_by_increasing_x(int* x1, int* y1,
                                        int* x2, int* y2,
                                        int* x3, int* y3)
{
    int x, y;
    if (*x1 > *x3)
    {
        if (*x2 < *x3)       /* x2 < x3 < x1 */
        {
            x = *x1; *x1 = *x2; *x2 = *x3; *x3 = x;
            y = *y1; *y1 = *y2; *y2 = *y3; *y3 = y;
        }
        else if (*x2 > *x1)  /* x3 < x1 < x2 */
        {
            x = *x1; *x1 = *x3; *x3 = *x2; *x2 = x;
            y = *y1; *y1 = *y3; *y3 = *y2; *y2 = y;
        }
        else               /* x3 <= x2 <= x1 */
        {
            x = *x1; *x1 = *x3; *x3 = x;
            y = *y1; *y1 = *y3; *y3 = y;
        }
    }
    else
    {
        if (*x2 < *x1)       /* x2 < x1 <= x3 */
        {
            x = *x1; *x1 = *x2; *x2 = x;
            y = *y1; *y1 = *y2; *y2 = y;
        }
        else if (*x2 > *x3)  /* x1 <= x3 < x2 */
        {
            x = *x2; *x2 = *x3; *x3 = x;
            y = *y2; *y2 = *y3; *y3 = y;
        }
        /* else already sorted */
    }
}

#define sort_points_by_increasing_y(x1, y1, x2, y2, x3, y3) \
    sort_points_by_increasing_x(y1, x1, y2, x2, y3, x3)

/* draw a filled triangle, using horizontal lines for speed */
static void xlcd_filltriangle_horizontal(struct screen* display,
                                         int x1, int y1,
                                         int x2, int y2,
                                         int x3, int y3)
{
    long fp_x1, fp_x2, fp_dx1, fp_dx2;
    int y;
    sort_points_by_increasing_y(&x1, &y1, &x2, &y2, &x3, &y3);

    if (y1 < y3)  /* draw */
    {
        fp_dx1 = ((x3 - x1) << 16) / (y3 - y1);
        fp_x1  = (x1 << 16) + (1<<15) + (fp_dx1 >> 1);

        if (y1 < y2)  /* first part */
        {
            fp_dx2 = ((x2 - x1) << 16) / (y2 - y1);
            fp_x2  = (x1 << 16) + (1<<15) + (fp_dx2 >> 1);
            for (y = y1; y < y2; y++)
            {
                display->hline(fp_x1 >> 16, fp_x2 >> 16, y);
                fp_x1 += fp_dx1;
                fp_x2 += fp_dx2;
            }
        }
        if (y2 < y3)  /* second part */
        {
            fp_dx2 = ((x3 - x2) << 16) / (y3 - y2);
            fp_x2 = (x2 << 16) + (1<<15) + (fp_dx2 >> 1);
            for (y = y2; y < y3; y++)
            {
                display->hline(fp_x1 >> 16, fp_x2 >> 16, y);
                fp_x1 += fp_dx1;
                fp_x2 += fp_dx2;
            }
        }
    }
}

/* draw a filled triangle, using vertical lines for speed */
static void xlcd_filltriangle_vertical(struct screen* display,
                                       int x1, int y1,
                                       int x2, int y2,
                                       int x3, int y3)
{
    long fp_y1, fp_y2, fp_dy1, fp_dy2;
    int x;
    sort_points_by_increasing_x(&x1, &y1, &x2, &y2, &x3, &y3);

    if (x1 < x3)  /* draw */
    {
        fp_dy1 = ((y3 - y1) << 16) / (x3 - x1);
        fp_y1  = (y1 << 16) + (1<<15) + (fp_dy1 >> 1);

        if (x1 < x2)  /* first part */
        {
            fp_dy2 = ((y2 - y1) << 16) / (x2 - x1);
            fp_y2  = (y1 << 16) + (1<<15) + (fp_dy2 >> 1);
            for (x = x1; x < x2; x++)
            {
                display->vline(x, fp_y1 >> 16, fp_y2 >> 16);
                fp_y1 += fp_dy1;
                fp_y2 += fp_dy2;
            }
        }
        if (x2 < x3)  /* second part */
        {
            fp_dy2 = ((y3 - y2) << 16) / (x3 - x2);
            fp_y2 = (y2 << 16) + (1<<15) + (fp_dy2 >> 1);
            for (x = x2; x < x3; x++)
            {
                display->vline(x, fp_y1 >> 16, fp_y2 >> 16);
                fp_y1 += fp_dy1;
                fp_y2 += fp_dy2;
            }
        }
    }
}

void xlcd_filltriangle(int x1, int y1,
                       int x2, int y2,
                       int x3, int y3)
{
    /* default is main screen */
    xlcd_filltriangle_screen(rb->screens[SCREEN_MAIN],
                             x1, y1, x2, y2, x3, y3);
}

void xlcd_filltriangle_screen(struct screen* display,
                              int x1, int y1,
                              int x2, int y2,
                              int x3, int y3)
{
    if(display->pixel_format==HORIZONTAL_PACKING || display->depth>=8)
        xlcd_filltriangle_horizontal(display, x1, y1, x2, y2, x3, y3);
    else
        xlcd_filltriangle_vertical(display, x1, y1, x2, y2, x3, y3);
}

static void xlcd_fillcircle_horizontal(struct screen* display,
                                       int cx, int cy, int radius)
{
    int d = 3 - (radius * 2);
    int x = 0;
    int y = radius;
    while(x <= y)
    {
        display->hline(cx - x, cx + x, cy + y);
        display->hline(cx - x, cx + x, cy - y);
        display->hline(cx - y, cx + y, cy + x);
        display->hline(cx - y, cx + y, cy - x);
        if(d < 0)
        {
            d += (x * 4) + 6;
        }
        else
        {
            d += ((x - y) * 4) + 10;
            --y;
        }
        ++x;
    }
}

static void xlcd_fillcircle_vertical(struct screen* display,
                                     int cx, int cy, int radius)
{
    int d = 3 - (radius * 2);
    int x = 0;
    int y = radius;
    while(x <= y)
    {
        display->vline(cx + x, cy + y, cy - y);
        display->vline(cx - x, cy + y, cy - y);
        display->vline(cy + x, cx + y, cx - y);
        display->vline(cy - x, cx + y, cx - y);
        if(d < 0)
        {
            d += (x * 4) + 6;
        }
        else
        {
            d += ((x - y) * 4) + 10;
            --y;
        }
        ++x;
    }
}

void xlcd_fillcircle_screen(struct screen* display,
                            int cx, int cy, int radius)
{
    if(display->pixel_format == HORIZONTAL_PACKING || display->depth >= 8)
        xlcd_fillcircle_horizontal(display, cx, cy, radius);
    else
        xlcd_fillcircle_vertical(display, cx, cy, radius);
}

void xlcd_fillcircle(int cx, int cy, int radius)
{
    xlcd_fillcircle_screen(rb->screens[SCREEN_MAIN], cx, cy, radius);
}

void xlcd_drawcircle_screen(struct screen* display,
                                   int cx, int cy, int radius)
{
    int d = 3 - (radius * 2);
    int x = 0;
    int y = radius;
    while(x <= y)
    {
        display->drawpixel(cx + x, cy + y);
        display->drawpixel(cx - x, cy + y);
        display->drawpixel(cx + x, cy - y);
        display->drawpixel(cx - x, cy - y);
        display->drawpixel(cx + y, cy + x);
        display->drawpixel(cx - y, cy + x);
        display->drawpixel(cx + y, cy - x);
        display->drawpixel(cx - y, cy - x);
        if(d < 0)
        {
            d += (x * 4) + 6;
        }
        else
        {
            d += ((x - y) * 4) + 10;
            --y;
        }
        ++x;
    }
}

void xlcd_drawcircle(int cx, int cy, int radius)
{
    /* default is main screen */
    xlcd_drawcircle_screen(rb->screens[SCREEN_MAIN], cx, cy, radius);
}


#if LCD_DEPTH >= 8 && LCD_DEPTH <= 16

#ifdef HAVE_LCD_COLOR
static const fb_data graylut[256] = {
#if LCD_PIXELFORMAT == RGB565
    0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0821, 0x0821, 0x0841,
    0x0841, 0x0841, 0x0841, 0x0861, 0x0861, 0x1062, 0x1062, 0x1082,
    0x1082, 0x1082, 0x1082, 0x10a2, 0x10a2, 0x18a3, 0x18a3, 0x18c3,
    0x18c3, 0x18c3, 0x18c3, 0x18e3, 0x18e3, 0x20e4, 0x20e4, 0x2104,
    0x2104, 0x2104, 0x2104, 0x2124, 0x2124, 0x2124, 0x2925, 0x2945,
    0x2945, 0x2945, 0x2945, 0x2965, 0x2965, 0x2965, 0x3166, 0x3186,
    0x3186, 0x3186, 0x3186, 0x31a6, 0x31a6, 0x31a6, 0x39a7, 0x39c7,
    0x39c7, 0x39c7, 0x39c7, 0x39e7, 0x39e7, 0x39e7, 0x41e8, 0x4208,
    0x4208, 0x4208, 0x4208, 0x4228, 0x4228, 0x4228, 0x4a29, 0x4a49,
    0x4a49, 0x4a49, 0x4a49, 0x4a69, 0x4a69, 0x4a69, 0x4a69, 0x528a,
    0x528a, 0x528a, 0x528a, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x5aab,
    0x5acb, 0x5acb, 0x5acb, 0x5acb, 0x5aeb, 0x5aeb, 0x5aeb, 0x62ec,
    0x630c, 0x630c, 0x630c, 0x630c, 0x632c, 0x632c, 0x632c, 0x6b2d,
    0x6b4d, 0x6b4d, 0x6b4d, 0x6b4d, 0x6b6d, 0x6b6d, 0x6b6d, 0x6b6d,
    0x738e, 0x738e, 0x738e, 0x738e, 0x73ae, 0x73ae, 0x73ae, 0x73ae,
    0x7bcf, 0x7bcf, 0x7bcf, 0x7bcf, 0x7bef, 0x7bef, 0x7bef, 0x7bef,
    0x8410, 0x8410, 0x8410, 0x8410, 0x8430, 0x8430, 0x8430, 0x8430,
    0x8c51, 0x8c51, 0x8c51, 0x8c51, 0x8c71, 0x8c71, 0x8c71, 0x8c71,
    0x9492, 0x9492, 0x9492, 0x9492, 0x94b2, 0x94b2, 0x94b2, 0x94b2,
    0x94d2, 0x9cd3, 0x9cd3, 0x9cd3, 0x9cf3, 0x9cf3, 0x9cf3, 0x9cf3,
    0x9d13, 0xa514, 0xa514, 0xa514, 0xa534, 0xa534, 0xa534, 0xa534,
    0xa554, 0xad55, 0xad55, 0xad55, 0xad55, 0xad75, 0xad75, 0xad75,
    0xad75, 0xb596, 0xb596, 0xb596, 0xb596, 0xb5b6, 0xb5b6, 0xb5b6,
    0xb5b6, 0xb5d6, 0xbdd7, 0xbdd7, 0xbdd7, 0xbdf7, 0xbdf7, 0xbdf7,
    0xbdf7, 0xbe17, 0xc618, 0xc618, 0xc618, 0xc638, 0xc638, 0xc638,
    0xc638, 0xc658, 0xce59, 0xce59, 0xce59, 0xce79, 0xce79, 0xce79,
    0xce79, 0xce99, 0xd69a, 0xd69a, 0xd69a, 0xd6ba, 0xd6ba, 0xd6ba,
    0xd6ba, 0xd6da, 0xd6da, 0xdedb, 0xdedb, 0xdefb, 0xdefb, 0xdefb,
    0xdefb, 0xdf1b, 0xdf1b, 0xe71c, 0xe71c, 0xe73c, 0xe73c, 0xe73c,
    0xe73c, 0xe75c, 0xe75c, 0xef5d, 0xef5d, 0xef7d, 0xef7d, 0xef7d,
    0xef7d, 0xef9d, 0xef9d, 0xf79e, 0xf79e, 0xf7be, 0xf7be, 0xf7be,
    0xf7be, 0xf7de, 0xf7de, 0xffdf, 0xffdf, 0xffff, 0xffff, 0xffff
#elif LCD_PIXELFORMAT == RGB565SWAPPED
    0x0000, 0x0000, 0x0000, 0x2000, 0x2000, 0x2108, 0x2108, 0x4108,
    0x4108, 0x4108, 0x4108, 0x6108, 0x6108, 0x6210, 0x6210, 0x8210,
    0x8210, 0x8210, 0x8210, 0xa210, 0xa210, 0xa318, 0xa318, 0xc318,
    0xc318, 0xc318, 0xc318, 0xe318, 0xe318, 0xe420, 0xe420, 0x0421,
    0x0421, 0x0421, 0x0421, 0x2421, 0x2421, 0x2421, 0x2529, 0x4529,
    0x4529, 0x4529, 0x4529, 0x6529, 0x6529, 0x6529, 0x6631, 0x8631,
    0x8631, 0x8631, 0x8631, 0xa631, 0xa631, 0xa631, 0xa739, 0xc739,
    0xc739, 0xc739, 0xc739, 0xe739, 0xe739, 0xe739, 0xe841, 0x0842,
    0x0842, 0x0842, 0x0842, 0x2842, 0x2842, 0x2842, 0x294a, 0x494a,
    0x494a, 0x494a, 0x494a, 0x694a, 0x694a, 0x694a, 0x694a, 0x8a52,
    0x8a52, 0x8a52, 0x8a52, 0xaa52, 0xaa52, 0xaa52, 0xaa52, 0xab5a,
    0xcb5a, 0xcb5a, 0xcb5a, 0xcb5a, 0xeb5a, 0xeb5a, 0xeb5a, 0xec62,
    0x0c63, 0x0c63, 0x0c63, 0x0c63, 0x2c63, 0x2c63, 0x2c63, 0x2d6b,
    0x4d6b, 0x4d6b, 0x4d6b, 0x4d6b, 0x6d6b, 0x6d6b, 0x6d6b, 0x6d6b,
    0x8e73, 0x8e73, 0x8e73, 0x8e73, 0xae73, 0xae73, 0xae73, 0xae73,
    0xcf7b, 0xcf7b, 0xcf7b, 0xcf7b, 0xef7b, 0xef7b, 0xef7b, 0xef7b,
    0x1084, 0x1084, 0x1084, 0x1084, 0x3084, 0x3084, 0x3084, 0x3084,
    0x518c, 0x518c, 0x518c, 0x518c, 0x718c, 0x718c, 0x718c, 0x718c,
    0x9294, 0x9294, 0x9294, 0x9294, 0xb294, 0xb294, 0xb294, 0xb294,
    0xd294, 0xd39c, 0xd39c, 0xd39c, 0xf39c, 0xf39c, 0xf39c, 0xf39c,
    0x139d, 0x14a5, 0x14a5, 0x14a5, 0x34a5, 0x34a5, 0x34a5, 0x34a5,
    0x54a5, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x75ad, 0x75ad, 0x75ad,
    0x75ad, 0x96b5, 0x96b5, 0x96b5, 0x96b5, 0xb6b5, 0xb6b5, 0xb6b5,
    0xb6b5, 0xd6b5, 0xd7bd, 0xd7bd, 0xd7bd, 0xf7bd, 0xf7bd, 0xf7bd,
    0xf7bd, 0x17be, 0x18c6, 0x18c6, 0x18c6, 0x38c6, 0x38c6, 0x38c6,
    0x38c6, 0x58c6, 0x59ce, 0x59ce, 0x59ce, 0x79ce, 0x79ce, 0x79ce,
    0x79ce, 0x99ce, 0x9ad6, 0x9ad6, 0x9ad6, 0xbad6, 0xbad6, 0xbad6,
    0xbad6, 0xdad6, 0xdad6, 0xdbde, 0xdbde, 0xfbde, 0xfbde, 0xfbde,
    0xfbde, 0x1bdf, 0x1bdf, 0x1ce7, 0x1ce7, 0x3ce7, 0x3ce7, 0x3ce7,
    0x3ce7, 0x5ce7, 0x5ce7, 0x5def, 0x5def, 0x7def, 0x7def, 0x7def,
    0x7def, 0x9def, 0x9def, 0x9ef7, 0x9ef7, 0xbef7, 0xbef7, 0xbef7,
    0xbef7, 0xdef7, 0xdef7, 0xdfff, 0xdfff, 0xffff, 0xffff, 0xffff
#endif /* LCD_PIXELFORMAT */
};
#endif /* HAVE_LCD_COLOR */

/* unused functions, enable when needed */
#if 0
/* Draw a partial greyscale bitmap, canonical 8 bit format */
void xlcd_gray_bitmap_part(const unsigned char *src, int src_x, int src_y,
                           int stride, int x, int y, int width, int height)
{
    size_t dst_stride;
    fb_data *lcd_fb = get_framebuffer(NULL, &dst_stride);

    const unsigned char *src_end;
    fb_data *dst;

    /* nothing to draw? */
    if ((width <= 0) || (height <= 0) || (x >= LCD_WIDTH) || (y >= LCD_HEIGHT)
        || (x + width <= 0) || (y + height <= 0))
        return;

    /* clipping */
    if (x < 0)
    {
        width += x;
        src_x -= x;
        x = 0;
    }
    if (y < 0)
    {
        height += y;
        src_y -= y;
        y = 0;
    }
    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;

    src    += stride * src_y + src_x; /* move starting point */
    src_end = src + stride * height;
    dst     = lcd_fb + dst_stride * y + x;

    do
    {
        const unsigned char *src_row = src;
        const unsigned char *row_end = src_row + width;
        fb_data *dst_row = dst;

#ifdef HAVE_LCD_COLOR
        do
#if LCD_DEPTH == 16
            *dst_row++ = graylut[*src_row++];
#else
            /* untested change because this function is completely unused */
            *dst_row->r = *dst_row->g = *dst_row->b = *src_row++;
            dst_row++;
#endif
        while (src_row < row_end);
#endif

        src +=  stride;
        dst += dst_stride;
    }
    while (src < src_end);
}

/* Draw a full greyscale bitmap, canonical 8 bit format */
void xlcd_gray_bitmap(const unsigned char *src, int x, int y, int width,
                      int height)
{
    xlcd_gray_bitmap_part(src, 0, 0, width, x, y, width, height);
}
#endif

#ifdef HAVE_LCD_COLOR
/* Draw a partial colour bitmap, canonical 24 bit RGB format */
void xlcd_color_bitmap_part(const unsigned char *src, int src_x, int src_y,
                            int stride, int x, int y, int width, int height)
{
    size_t dst_stride;
    fb_data *lcd_fb = get_framebuffer(NULL, &dst_stride);

    const unsigned char *src_end;
    fb_data *dst;

    /* nothing to draw? */
    if ((width <= 0) || (height <= 0) || (x >= LCD_WIDTH) || (y >= LCD_HEIGHT)
        || (x + width <= 0) || (y + height <= 0))
        return;

    /* clipping */
    if (x < 0)
    {
        width += x;
        src_x -= x;
        x = 0;
    }
    if (y < 0)
    {
        height += y;
        src_y -= y;
        y = 0;
    }
    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;

    src    += 3 * (stride * src_y + src_x); /* move starting point */
    src_end = src + 3 * stride * height;
    dst     = lcd_fb + dst_stride * y + x;

    do
    {
        const unsigned char *src_row = src;
        const unsigned char *row_end = src_row + 3 * width;
        fb_data *dst_row = dst;

        do
        {   /* only RGB565 and RGB565SWAPPED so far */
            unsigned red   = 31 * (*src_row++) + 127;
            unsigned green = 63 * (*src_row++) + 127;
            unsigned blue  = 31 * (*src_row++) + 127;

            red   = (red + (red >> 8)) >> 8;     /* approx red /= 255: */
            green = (green + (green >> 8)) >> 8; /* approx green /= 255: */
            blue  = (blue + (blue >> 8)) >> 8;   /* approx blue /= 255: */

#if LCD_PIXELFORMAT == RGB565
            *dst_row++ = (red << 11) | (green << 5) | blue;
#elif LCD_PIXELFORMAT == RGB565SWAPPED
            *dst_row++ = swap16((red << 11) | (green << 5) | blue);
#endif
        }
        while (src_row < row_end);

        src +=  3 * stride;
        dst += dst_stride;
    }
    while (src < src_end);
}

/* Draw a full colour bitmap, canonical 24 bit RGB format */
void xlcd_color_bitmap(const unsigned char *src, int x, int y, int width,
                       int height)
{
    xlcd_color_bitmap_part(src, 0, 0, width, x, y, width, height);
}
#endif /* HAVE_LCD_COLOR */

#endif /* LCD_DEPTH >= 8 */
