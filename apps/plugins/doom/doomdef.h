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
 *  Internally used data structures for virtually everything,
 *   key definitions, lots of other stuff.
 *
 *-----------------------------------------------------------------------------*/

#ifndef __DOOMDEF__
#define __DOOMDEF__

#include "rockmacros.h"

// killough 4/25/98: Make gcc extensions mean nothing on other compilers
#ifndef __GNUC__
#define __attribute__(x)
#endif

//
// Global parameters/defines.
//
// DOOM version
enum { DVERSION =  110 };

// Game mode handling - identify IWAD version
//  to handle IWAD dependend animations etc.
enum {
   shareware,    // DOOM 1 shareware, E1, M9
   registered,   // DOOM 1 registered, E3, M27
   commercial,   // DOOM 2 retail, E1 M34  (DOOM 2 german edition not handled)
   retail,       // DOOM 1 retail, E4, M36
   indetermined  // Well, no IWAD found.
};
typedef unsigned GameMode_t;

// Mission packs - might be useful for TC stuff?
enum {
   doom,         // DOOM 1
   doom2,        // DOOM 2
   pack_tnt,     // TNT mission pack
   pack_plut,    // Plutonia pack
   none
};
typedef unsigned GameMission_t;

// Identify language to use, software localization.
enum {
   english,
   french,
   german,
   unknown
};
typedef unsigned Language_t;

//
// For resize of screen, at start of game.
//

#define BASE_WIDTH  320

// It is educational but futile to change this
//  scaling e.g. to 2. Drawing of status bar,
//  menues etc. is tied to the scale implied
//  by the graphics.

#define INV_ASPECT_RATIO 0.625 // 0.75, ideally

// killough 2/8/98: MAX versions for maximum screen sizes
// allows us to avoid the overhead of dynamic allocation
// when multiple screen sizes are supported

#if(LCD_HEIGHT>LCD_WIDTH)
extern int rotate_screen;
// proff 08/17/98: Changed for high-res
#define MAX_SCREENWIDTH  LCD_HEIGHT
#define MAX_SCREENHEIGHT LCD_HEIGHT
extern int SCREENWIDTH;
extern int SCREENHEIGHT;
#else
// proff 08/17/98: Changed for high-res
#define MAX_SCREENWIDTH  LCD_WIDTH
#define MAX_SCREENHEIGHT LCD_HEIGHT
#define SCREENWIDTH  LCD_WIDTH
#define SCREENHEIGHT LCD_HEIGHT
#endif

// The maximum number of players, multiplayer/networking.
#define MAXPLAYERS       4

// phares 5/14/98:
// DOOM Editor Numbers (aka doomednum in mobj_t)

#define DEN_PLAYER5 4001
#define DEN_PLAYER6 4002
#define DEN_PLAYER7 4003
#define DEN_PLAYER8 4004

// State updates, number of tics / second.
#define TICRATE          35

// The current state of the game: whether we are playing, gazing
// at the intermission screen, the game final animation, or a demo.

enum {
   GS_LEVEL,
   GS_INTERMISSION,
   GS_FINALE,
   GS_DEMOSCREEN
};
typedef unsigned gamestate_t;

//
// Difficulty/skill settings/filters.
//

// Skill flags.
#define  MTF_EASY    1
#define  MTF_NORMAL  2
#define  MTF_HARD    4
// Deaf monsters/do not react to sound.
#define  MTF_AMBUSH  8

/* killough 11/98 */
#define MTF_NOTSINGLE          16
#define MTF_NOTDM              32
#define MTF_NOTCOOP            64
#define MTF_FRIEND            128
#define MTF_RESERVED          256

enum {
   sk_none=-1, //jff 3/24/98 create unpicked skill setting
   sk_baby=0,
   sk_easy,
   sk_medium,
   sk_hard,
   sk_nightmare
};
typedef int skill_t;

//
// Key cards.
//

enum {
   it_bluecard,
   it_yellowcard,
   it_redcard,
   it_blueskull,
   it_yellowskull,
   it_redskull,
   NUMCARDS
};
typedef unsigned card_t;

// The defined weapons, including a marker
// indicating user has not changed weapon.
enum {
   wp_fist,
   wp_pistol,
   wp_shotgun,
   wp_chaingun,
   wp_missile,
   wp_plasma,
   wp_bfg,
   wp_chainsaw,
   wp_supershotgun,

   NUMWEAPONS,
   wp_nochange              // No pending weapon change.
};
typedef unsigned weapontype_t;

// Ammunition types defined.
enum {
   am_clip,    // Pistol / chaingun ammo.
   am_shell,   // Shotgun / double barreled shotgun.
   am_cell,    // Plasma rifle, BFG.
   am_misl,    // Missile launcher.
   NUMAMMO,
   am_noammo   // Unlimited for chainsaw / fist.
};

typedef unsigned ammotype_t;

// Power up artifacts.
enum {
   pw_invulnerability,
   pw_strength,
   pw_invisibility,
   pw_ironfeet,
   pw_allmap,
   pw_infrared,
   NUMPOWERS
};
typedef unsigned powertype_t;

// Power up durations (how many seconds till expiration).
enum {
   INVULNTICS  = (30*TICRATE),
   INVISTICS   = (60*TICRATE),
   INFRATICS   = (120*TICRATE),
   IRONTICS    = (60*TICRATE)
};
typedef unsigned powerduration_t;

//
// DOOM keyboard definition.
// This is the stuff configured by Setup.Exe.
// Most key data are simple ascii (uppercased).
//
#define KEY_RIGHTARROW     0xae
#define KEY_LEFTARROW      0xac
#define KEY_UPARROW        0xad
#define KEY_DOWNARROW      0xaf
#define KEY_ESCAPE         0x1B
#define KEY_ENTER          0x60     // Changed due to button reconfig - 0x0D
#define KEY_TAB            0x61     // Changed due to button reconfig - 0x09
#define KEY_F1          (0x80+0x3b)
#define KEY_F2          (0x80+0x3c)
#define KEY_F3          (0x80+0x3d)
#define KEY_F4          (0x80+0x3e)
#define KEY_F5          (0x80+0x3f)
#define KEY_F6          (0x80+0x40)
#define KEY_F7          (0x80+0x41)
#define KEY_F8          (0x80+0x42)
#define KEY_F9          (0x80+0x43)
#define KEY_F10         (0x80+0x44)
#define KEY_F11         (0x80+0x57)
#define KEY_F12         (0x80+0x58)
#define KEY_BACKSPACE      0x7F
#define KEY_PAUSE          0xff
#define KEY_EQUALS         0x3d
#define KEY_MINUS          0x2d
#define KEY_RSHIFT      (0x80+0x36)
#define KEY_RCTRL       (0x80+0x1d)
#define KEY_RALT        (0x80+0x38)
#define KEY_LALT         KEY_RALT
#define KEY_CAPSLOCK   0xba                                        // phares

// phares 3/2/98:
#define KEY_INSERT     0xd2
#define KEY_HOME       0xc7
#define KEY_PAGEUP     0xc9
#define KEY_PAGEDOWN   0xd1
#define KEY_DEL        0xc8
#define KEY_END        0xcf
#define KEY_SCROLLLOCK 0xc6
#define KEY_SPACEBAR   0x20
// phares 3/2/98

#define KEY_NUMLOCK    0xC5                 // killough 3/6/98

// cph - Add the numeric keypad keys, as suggested by krose 4/22/99:
// The way numbers are assigned to keys is a mess, but it's too late to
// change that easily. At least these additions are don neatly.
// Codes 0x100-0x200 are reserved for number pad

#define KEY_KEYPAD0      (0x100 + '0')
#define KEY_KEYPAD1      (0x100 + '1')
#define KEY_KEYPAD2      (0x100 + '2')
#define KEY_KEYPAD3      (0x100 + '3')
#define KEY_KEYPAD4      (0x100 + '4')
#define KEY_KEYPAD5      (0x100 + '5')
#define KEY_KEYPAD6      (0x100 + '6')
#define KEY_KEYPAD7      (0x100 + '7')
#define KEY_KEYPAD8      (0x100 + '8')
#define KEY_KEYPAD9      (0x100 + '9')
#define KEY_KEYPADENTER  (0x100 + KEY_ENTER)
#define KEY_KEYPADDIVIDE (0x100 + '/')
#define KEY_KEYPADMULTIPLY (0x100 + '*')
#define KEY_KEYPADMINUS  (0x100 + '-')
#define KEY_KEYPADPLUS   (0x100 + '+')
#define KEY_KEYPADPERIOD (0x100 + '.')

// phares 4/19/98:
// Defines Setup Screen groups that config variables appear in.
// Used when resetting the defaults for every item in a Setup group.

enum {
   ss_none,
   ss_keys,
   ss_weap,
   ss_stat,
   ss_auto,
   ss_enem,
   ss_mess,
   ss_chat,
   ss_gen,       /* killough 10/98 */
   ss_comp,      /* killough 10/98 */
   ss_max
};
typedef unsigned ss_types;

// phares 3/20/98:
//
// Player friction is variable, based on controlling
// linedefs. More friction can create mud, sludge,
// magnetized floors, etc. Less friction can create ice.

#define MORE_FRICTION_MOMENTUM 15000       // mud factor based on momentum
#define ORIG_FRICTION          0xE800      // original value
#define ORIG_FRICTION_FACTOR   2048        // original value

#endif          // __DOOMDEF__
