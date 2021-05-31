/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id$
*
* New greyscale framework
* Scrolling routines
*
* This is a generic framework to display 129 shades of grey on low-depth
* bitmap LCDs (Archos b&w, Iriver & Ipod 4-grey) within plugins.
*
* Copyright (C) 2008 Jens Arnold
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
#include "grey.h"

extern struct viewport _grey_default_vp;

/*** Scrolling ***/

/* Scroll left */
void grey_scroll_left(int count)
{
    struct viewport *vp = &_grey_default_vp;
    unsigned char *data, *data_end;
    int length, blank;

    if ((unsigned)count >= (unsigned)_grey_info.width)
    {
        grey_clear_display();
        return;
    }

    data = _grey_info.buffer;
    data_end = data + _GREY_MULUQ(_grey_info.width, _grey_info.height);
    length = _grey_info.width - count;
    blank = (vp->drawmode & DRMODE_INVERSEVID) ? vp->fg_pattern : vp->bg_pattern;

    do
    {
        rb->memmove(data, data + count, length);
        data += length;
        rb->memset(data, blank, count);
        data += count;
    }
    while (data < data_end);
}

/* Scroll right */
void grey_scroll_right(int count)
{
    struct viewport *vp = &_grey_default_vp;
    unsigned char *data, *data_end;
    int length, blank;

    if ((unsigned)count >= (unsigned)_grey_info.width)
    {
        grey_clear_display();
        return;
    }

    data = _grey_info.buffer;
    data_end = data + _GREY_MULUQ(_grey_info.width, _grey_info.height);
    length = _grey_info.width - count;
    blank = (vp->drawmode & DRMODE_INVERSEVID) ? vp->fg_pattern : vp->bg_pattern;

    do
    {
        rb->memmove(data + count, data, length);
        rb->memset(data, blank, count);
        data += _grey_info.width;
    }
    while (data < data_end);
}

/* Scroll up */
void grey_scroll_up(int count)
{
    struct viewport *vp = &_grey_default_vp;
    long shift, length;
    int blank;

    if ((unsigned)count >= (unsigned)_grey_info.height)
    {
        grey_clear_display();
        return;
    }

    shift = _GREY_MULUQ(_grey_info.width, count);
    length = _GREY_MULUQ(_grey_info.width, _grey_info.height - count);
    blank = (vp->drawmode & DRMODE_INVERSEVID) ? vp->fg_pattern : vp->bg_pattern;

    rb->memmove(_grey_info.buffer, _grey_info.buffer + shift,
                           length);
    rb->memset(_grey_info.buffer + length, blank, shift);
}

/* Scroll down */
void grey_scroll_down(int count)
{
    struct viewport *vp = &_grey_default_vp;
    long shift, length;
    int blank;

    if ((unsigned)count >= (unsigned)_grey_info.height)
    {
        grey_clear_display();
        return;
    }

    shift = _GREY_MULUQ(_grey_info.width, count);
    length = _GREY_MULUQ(_grey_info.width, _grey_info.height - count);
    blank = (vp->drawmode & DRMODE_INVERSEVID) ? vp->fg_pattern : vp->bg_pattern;

    rb->memmove(_grey_info.buffer + shift, _grey_info.buffer,
                           length);
    rb->memset(_grey_info.buffer, blank, shift);
}

/*** Unbuffered scrolling functions ***/

/* Scroll left */
void grey_ub_scroll_left(int count)
{
    struct viewport *vp = &_grey_default_vp;
    unsigned char *data, *data_end;
    int blank, length;

    if ((unsigned)count >= (unsigned)_grey_info.width)
    {
        grey_ub_clear_display();
        return;
    }

    data = _grey_info.values;
    data_end = data + _GREY_MULUQ(_grey_info.width, _grey_info.height);
    length = (_grey_info.width - count) << _GREY_BSHIFT;
    count <<= _GREY_BSHIFT;
    blank = _grey_info.gvalue[(vp->drawmode & DRMODE_INVERSEVID) ?
                              vp->fg_pattern : vp->bg_pattern];
    do
    {
        rb->memmove(data, data + count, length);
        data += length;
        rb->memset(data, blank, count);
        data += count;
    }
    while (data < data_end);
#ifdef SIMULATOR
    rb->sim_lcd_ex_update_rect(_grey_info.x, _grey_info.y,
                                          _grey_info.width, _grey_info.height);
#endif
}

/* Scroll right */
void grey_ub_scroll_right(int count)
{
    struct viewport *vp = &_grey_default_vp;
    unsigned char *data, *data_end;
    int blank, length;

    if ((unsigned)count >= (unsigned)_grey_info.width)
    {
        grey_ub_clear_display();
        return;
    }

    data = _grey_info.values;
    data_end = data + _GREY_MULUQ(_grey_info.width, _grey_info.height);
    length = (_grey_info.width - count) << _GREY_BSHIFT;
    count <<= _GREY_BSHIFT;
    blank = _grey_info.gvalue[(vp->drawmode & DRMODE_INVERSEVID) ?
                              vp->fg_pattern : vp->bg_pattern];
    do
    {
        rb->memmove(data + count, data, length);
        rb->memset(data, blank, count);
        data += _grey_info.width << _GREY_BSHIFT;
    }
    while (data < data_end);
#ifdef SIMULATOR
    rb->sim_lcd_ex_update_rect(_grey_info.x, _grey_info.y,
                                          _grey_info.width, _grey_info.height);
#endif
}

/* Scroll up */
void grey_ub_scroll_up(int count)
{
    struct viewport *vp = &_grey_default_vp;
    unsigned char *dst, *end, *src;
    int blank;

    if ((unsigned)count >= (unsigned)_grey_info.height)
    {
        grey_ub_clear_display();
        return;
    }

    dst   = _grey_info.values;
    end   = dst + _GREY_MULUQ(_grey_info.height, _grey_info.width);
    blank = _grey_info.gvalue[(vp->drawmode & DRMODE_INVERSEVID) ?
                              vp->fg_pattern : vp->bg_pattern];

#if (LCD_PIXELFORMAT == VERTICAL_PACKING) \
 || (LCD_PIXELFORMAT == VERTICAL_INTERLEAVED)
    if (count & _GREY_BMASK)
    {
        /* Scrolling by fractional blocks - move pixel wise. */
        unsigned char *line_end;
        int ys, yd;

        for (ys = count, yd = 0; ys < _grey_info.height; ys++, yd++)
        {
            dst = _grey_info.values
                + _GREY_MULUQ(_grey_info.width, yd & ~_GREY_BMASK)
                + (~yd & _GREY_BMASK);
            src = _grey_info.values
                + _GREY_MULUQ(_grey_info.width, ys & ~_GREY_BMASK)
                + (~ys & _GREY_BMASK);
            line_end = dst + _grey_info.width * _GREY_BSIZE;

            do
            {
                *dst = *src;
                dst += _GREY_BSIZE;
                src += _GREY_BSIZE;
            }
            while (dst < line_end);
        }
        for (; yd & _GREY_BMASK; yd++)   /* Fill remainder of current block. */
        {
            dst = _grey_info.values
                + _GREY_MULUQ(_grey_info.width, yd & ~_GREY_BMASK)
                + (~yd & _GREY_BMASK);
            line_end = dst + _grey_info.width * _GREY_BSIZE;

            do
            {
                *dst = blank;
                dst += _GREY_BSIZE;
            }
            while (dst < line_end);
        }
    }
    else
#endif
    {
        int blen = _GREY_MULUQ(_grey_info.height - count, _grey_info.width);

        src = dst + _GREY_MULUQ(count, _grey_info.width);
        rb->memmove(dst, src, blen);
        dst += blen;
    }
    rb->memset(dst, blank, end - dst); /* Fill remainder at once. */
#ifdef SIMULATOR
    rb->sim_lcd_ex_update_rect(_grey_info.x, _grey_info.y,
                                          _grey_info.width, _grey_info.height);
#endif
}

/* Scroll down */
void grey_ub_scroll_down(int count)
{
    struct viewport *vp = &_grey_default_vp;
    unsigned char *start, *dst;
    int blank;

    if ((unsigned)count >= (unsigned)_grey_info.height)
    {
        grey_ub_clear_display();
        return;
    }

    start = _grey_info.values;
    dst   = start + _GREY_MULUQ(_grey_info.height, _grey_info.width);
    blank = _grey_info.gvalue[(vp->drawmode & DRMODE_INVERSEVID) ?
                              vp->fg_pattern : vp->bg_pattern];

#if (LCD_PIXELFORMAT == VERTICAL_PACKING) \
 || (LCD_PIXELFORMAT == VERTICAL_INTERLEAVED)
    if (count & _GREY_BMASK)
    {
        /* Scrolling by fractional blocks - move pixel wise. */
        unsigned char *src, *line_end;
        int ys, yd;

        yd = _grey_info.height - 1;
        for (ys = yd - count; ys >= 0; ys--, yd--)
        {
            dst = _grey_info.values
                + _GREY_MULUQ(_grey_info.width, yd & ~_GREY_BMASK)
                + (~yd & _GREY_BMASK);
            src = _grey_info.values
                + _GREY_MULUQ(_grey_info.width, ys & ~_GREY_BMASK)
                + (~ys & _GREY_BMASK);
            line_end = dst + _grey_info.width * _GREY_BSIZE;

            do
            {
                *dst = *src;
                dst += _GREY_BSIZE;
                src += _GREY_BSIZE;
            }
            while (dst < line_end);
        }
        for (; ~yd & _GREY_BMASK; yd--)  /* Fill remainder of current block. */
        {
            dst = _grey_info.values
                + _GREY_MULUQ(_grey_info.width, yd & ~_GREY_BMASK)
                + (~yd & _GREY_BMASK);
            line_end = dst + _grey_info.width * _GREY_BSIZE;

            do
            {
                line_end -= _GREY_BSIZE;
                *line_end = blank;
            }
            while (dst < line_end);
        }
        /* Top pixel in a block has the highest address, but dst must point
         * to the lowest address in that block for the subsequent fill. */
        dst -= _GREY_BMASK;
    }
    else
#endif
    {
        int blen = _GREY_MULUQ(_grey_info.height - count, _grey_info.width);

        dst -= blen;
        rb->memmove(dst, start, blen);
    }
    rb->memset(start, blank, dst - start);
    /* Fill remainder at once. */
#ifdef SIMULATOR
    rb->sim_lcd_ex_update_rect(_grey_info.x, _grey_info.y,
                                          _grey_info.width, _grey_info.height);
#endif
}
