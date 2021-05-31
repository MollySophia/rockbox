/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *      Lookup tables.
 *      Do not try to look them up :-).
 *      In the order of appearance:
 *
 *      int finetangent[4096]   - Tangens LUT.
 *       Should work with BAM fairly well (12 of 16bit,
 *      effectively, by shifting).
 *
 *      int finesine[10240]             - Sine lookup.
 *       Guess what, serves as cosine, too.
 *       Remarkable thing is, how to use BAMs with this?
 *
 *      int tantoangle[2049]    - ArcTan LUT,
 *        maps tan(angle) to angle fast. Gotta search.
 *
 *-----------------------------------------------------------------------------*/

#ifndef __TABLES__
#define __TABLES__

#include "m_fixed.h"
#include "rockmacros.h"

#define FINEANGLES              8192
#define FINEMASK                (FINEANGLES-1)

// 0x100000000 to 0x2000
#define ANGLETOFINESHIFT        19

#ifndef ALL_IN_ONE

// TABLES_AS_LUMPS causes the tables to be loaded from a wad lump, and the normal
// data to be stored as a predefined lump.
// Only really useful for dumping the trig tables or with NO_PREDEFINED_LUMPS
#define TABLES_AS_LUMPS
#endif

// Binary Angle Measument, BAM.
#define ANG45   0x20000000
#define ANG90   0x40000000
#define ANG180  0x80000000
#define ANG270  0xc0000000

#define SLOPERANGE 2048
#define SLOPEBITS    11
#define DBITS      (FRACBITS-SLOPEBITS)

typedef unsigned angle_t;

// Load trig tables if needed
#ifdef TABLES_AS_LUMPS
void R_LoadTrigTables(void);
#define TRIG_CONST
#else
#define TRIG_CONST const
#endif

// Effective size is 10240.
extern TRIG_CONST fixed_t *finesine;

// Re-use data, is just PI/2 phase shift.
extern TRIG_CONST fixed_t *finecosine;

// Effective size is 4096.
extern TRIG_CONST fixed_t *finetangent;

// Effective size is 2049;
// The +1 size is to handle the case when x==y without additional checking.

extern TRIG_CONST angle_t *tantoangle;

// Utility function, called by R_PointToAngle.
int SlopeDiv(unsigned num, unsigned den);

#undef TRIG_CONST
#endif
