#ifndef __XDUOOLINUX_CODEC__
#define __XDUOOLINUX_CODEC__

#define AUDIOHW_CAPS (LINEOUT_CAP | FILTER_ROLL_OFF_CAP)
AUDIOHW_SETTING(VOLUME, "dB", 1, 5, -102*10,  0, -30*10)
AUDIOHW_SETTING(FILTER_ROLL_OFF, "", 0, 1, 0, 4, 0)
#endif

// We want this, but the codec takes over a second to unmute!
//#define AUDIOHW_MUTE_ON_STOP

#if defined(XDUOO_X3II)
/* The AK4490 glitches when switching sample rates */
#define AUDIOHW_MUTE_ON_SRATE_CHANGE
#endif

#if defined(XDUOO_X20)
//#define AUDIOHW_NEEDS_INITIAL_UNMUTE
#endif

void audiohw_mute(int mute);
void xduoo_set_output(int ps);
int xduoo_get_outputs(void);
