/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Antoine Cellerier <dionoea -at- videolan -dot- org>
 * Copyright (C) 2009 by Andrew Mahone
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

#include "pluginlib_bmp.h"

#include "lcd.h"
#include "file.h"
#include "system.h"

#if defined(HAVE_LCD_COLOR)
#define LE16(x) (htole16(x))&0xff, ((htole16(x))>>8)&0xff
#define LE32(x) (htole32(x))&0xff, ((htole32(x))>>8)&0xff, ((htole32(x))>>16)&0xff, ((htole32(x))>>24)&0xff
/**
 * Save to 24 bit bitmap.
 */
int save_bmp_file( char* filename, struct bitmap *bm )
{
    int line_width = (bm->width*3+3) & ~3;
    int padsize = line_width - bm->width*3;

    const unsigned char header[] =
    {
        0x42, 0x4d, /* signature - 'BM' */
        LE32( bm->height*line_width + 54 + 4*0 ), /* file size in bytes */
        0x00, 0x00, 0x00, 0x00, /* 0, 0 */
        LE32( 54 + 4*0 ), /* offset to bitmap */
        0x28, 0x00, 0x00, 0x00, /* size of this struct (40) */
        LE32( bm->width ), /* bmap width in pixels */
        LE32( bm->height ), /* bmap height in pixels */
        0x01, 0x00, /* num planes - always 1 */
        LE16( 24 ), /* bits per pixel */
        LE32( 0 ), /* compression flag */
        LE32( bm->height*line_width ), /* image size in bytes */
        0xc4, 0x0e, 0x00, 0x00, /* horz resolution */
        0xc4, 0x0e, 0x00, 0x00, /* vert resolution */
        LE32( 0 ), /* 0 -> color table size */
        LE32( 0 ) /* important color count */
    };

    int fh;
    int x,y;
    if( bm->format != FORMAT_NATIVE ) return -1;
    fh = rb->creat( filename , 0666);
    if( fh < 0 ) return -1;

    rb->write( fh, header, sizeof( header ) );
    for( y = bm->height-1; y >= 0; y-- )
    {
        for( x = 0; x <  bm->width; x++ )
        {
            fb_data *d = (fb_data*)( bm->data ) + (x+y*bm->width);
            unsigned char c[] =
            {
                FB_UNPACK_BLUE( *d ),
                FB_UNPACK_GREEN( *d ),
                FB_UNPACK_RED( *d )
            };
            rb->write( fh, c, 3 );
        }
        if(padsize != 0)
        {
            unsigned int c = 0; /* padsize is at most 3. */
            rb->write( fh, &c, padsize );
        }
    }
    rb->close( fh );
    return 1;
}
#endif /* HAVE_LCD_COLOR */

/**
   Very simple image scale from src to dst (nearest neighbour).
   Source and destination dimensions are read from the struct bitmap.
   FIXME: this doesn't work well for LCD_DEPTH<4
*/
void simple_resize_bitmap(struct bitmap *src, struct bitmap *dst)
{
#if defined(LCD_STRIDEFORMAT) && (LCD_STRIDEFORMAT == VERTICAL_STRIDE)
    const int srcw = src->height;
    const int srch = src->width;
    const int dstw = dst->height;
    const int dsth = dst->width;
#else
    const int srcw = src->width;
    const int srch = src->height;
    const int dstw = dst->width;
    const int dsth = dst->height;
#endif
    const fb_data *srcd = (fb_data *)(src->data);
    const fb_data *dstd = (fb_data *)(dst->data);
    const long xrstep = ((srcw-1) << 8) / (dstw-1);
    const long yrstep = ((srch-1) << 8) / (dsth-1);
    fb_data *src_row, *dst_row;
    long xr, yr = 0;
    int src_x, src_y, dst_x, dst_y;
    for (dst_y=0; dst_y < dsth; dst_y++)
    {
        src_y = (yr >> 8);
        src_row = (fb_data *)&srcd[src_y * srcw];
        dst_row = (fb_data *)&dstd[dst_y * dstw];
        for (xr=0,dst_x=0; dst_x < dstw; dst_x++)
        {
            src_x = (xr >> 8);
            dst_row[dst_x] = src_row[src_x];
            xr += xrstep;
        }
        yr += yrstep;
    }
}

#if (LCD_DEPTH < 4)
/**
   Same as simple_resize_bitmap except this is for use with greylib.
*/
void grey_resize_bitmap(struct bitmap *src, struct bitmap *dst)
{
    const int srcw = src->width;
    const int srch = src->height;
    const int dstw = dst->width;
    const int dsth = dst->height;
    const long xrstep = ((srcw-1) << 8) / (dstw-1);
    const long yrstep = ((srch-1) << 8) / (dsth-1);
    unsigned char *srcd = src->data;
    unsigned char *dstd = dst->data;
    unsigned char *src_row, *dst_row;
    long xr, yr = 0;
    int src_x, src_y, dst_x, dst_y;
    for (dst_y=0; dst_y < dsth; dst_y++)
    {
        src_y = (yr >> 8);
        src_row = &srcd[src_y * srcw];
        dst_row = &dstd[dst_y * dstw];
        for (xr=0,dst_x=0; dst_x < dstw; dst_x++)
        {
            src_x = (xr >> 8);
            dst_row[dst_x] = src_row[src_x];
            xr += xrstep;
        }
        yr += yrstep;
    }
}
#endif


#include "wrappers.h"

/* import the core bmp loader */
#include "recorder/bmp.c"
