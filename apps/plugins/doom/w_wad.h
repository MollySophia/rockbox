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
 *      WAD I/O functions.
 *
 *-----------------------------------------------------------------------------*/


#ifndef __W_WAD__
#define __W_WAD__

#ifdef __GNUG__
#pragma interface
#endif

#include "m_fixed.h"

//
// TYPES
//

typedef struct
{
   char identification[4];                  // Should be "IWAD" or "PWAD".
   int  numlumps;
   int  infotableofs;
} wadinfo_t;

typedef struct
{
   int  filepos;
   int  size;
   char name[8];
} filelump_t;

#ifndef ALL_IN_ONE

// NO_PREDEFINED_LUMPS causes none of the predefined lumps in info.c to be
// included, and removes all extra code which is only there for them
// Saves a little memory normally, lots if any were overridden, and makes
// the executable smaller
#define NO_PREDEFINED_LUMPS

#endif

//
// WADFILE I/O related stuff.
//

// CPhipps - defined enum in wider scope
// Ty 08/29/98 - add source field to identify where this lump came from
enum {
   // CPhipps - define elements in order of 'how new/unusual'
   source_iwad=0,    // iwad file load
   source_pre,       // predefined lump
   source_auto_load, // lump auto-loaded by config file
   source_pwad,      // pwad file load
   source_lmp,       // lmp file load
   source_net        // CPhipps
};
typedef unsigned wad_source_t;

enum {
  ns_global=0,
  ns_sprites,
  ns_flats,
  ns_colormaps
}; /* namespace */

typedef struct
{
   // WARNING: order of some fields important (see info.c).

   char  name[8];
   int   size;
#ifndef NO_PREDEFINED_LUMPS
   const void *data;     // killough 1/31/98: points to predefined lump data
#endif

   // killough 1/31/98: hash table fields, used for ultra-fast hash table lookup
   int index, next;

   // killough 4/17/98: namespace tags, to prevent conflicts between resources
   unsigned namespace;

   int handle;
   int position;
   unsigned int locks; // CPhipps - wad lump locking
   wad_source_t source;
} lumpinfo_t;

// killough 1/31/98: predefined lumps
extern const size_t num_predefined_lumps;
extern const lumpinfo_t predefined_lumps[];

extern void       **lumpcache;
extern lumpinfo_t *lumpinfo;
extern int        numlumps;

// CPhipps - changed wad init
// We _must_ have the wadfiles[] the same as those actually loaded, so there
// is no point having these separate entities. This belongs here.
struct wadfile_info {
   const char* name;
   wad_source_t src;
};

extern struct wadfile_info *wadfiles;

extern size_t numwadfiles; // CPhipps - size of the wadfiles array

void W_Init(void); // CPhipps - uses the above array

// killough 4/17/98: if W_CheckNumForName() called with only
// one argument, pass ns_global as the default namespace

#define W_CheckNumForName(name) (W_CheckNumForName)(name, ns_global)
int     (W_CheckNumForName)(const char* name, int);   // killough 4/17/98
int     W_GetNumForName (const char* name);
int     W_LumpLength (int lump);
void    W_ReadLump (int lump, void *dest);
// CPhipps - modified for 'new' lump locking
void* W_CacheLumpNum (int lump, unsigned short locks);
void    W_UnlockLumpNum(int lump, signed short unlocks);

/* cph - special version to return lump with padding, for sound lumps */
void * W_CacheLumpNumPadded(int lump, size_t len, unsigned char pad);

// CPhipps - convenience macros
#define W_CacheLumpNum(num) (W_CacheLumpNum)((num),1)
#define W_CacheLumpName(name) W_CacheLumpNum (W_GetNumForName(name))

#define W_UnlockLumpNum(num) (W_UnlockLumpNum)((num),1)
#define W_UnlockLumpName(name) W_UnlockLumpNum (W_GetNumForName(name))

char *AddDefaultExtension(char *, const char *);  // killough 1/18/98
void ExtractFileBase(const char *, char *);       // killough
unsigned W_LumpNameHash(const char *s);           // killough 1/31/98

// Function to write all predefined lumps to a PWAD if requested
extern void WritePredefinedLumpWad(const char *filename); // jff 5/6/98

#endif
