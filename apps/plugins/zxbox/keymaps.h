#ifndef KEYMAPS_H
#define KEYMAPS_H

#if (CONFIG_KEYPAD == IPOD_4G_PAD) || (CONFIG_KEYPAD == IPOD_3G_PAD) || \
    (CONFIG_KEYPAD == IPOD_1G2G_PAD)

#define ZX_UP        BUTTON_MENU
#define ZX_DOWN        BUTTON_PLAY
#define ZX_SELECT    BUTTON_SELECT
#define ZX_LEFT        BUTTON_LEFT
#define ZX_RIGHT    BUTTON_RIGHT

#elif CONFIG_KEYPAD == IRIVER_H100_PAD || CONFIG_KEYPAD == IRIVER_H300_PAD

#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT  BUTTON_ON
#define ZX_MENU    BUTTON_MODE

#elif CONFIG_KEYPAD == GIGABEAT_PAD

#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT     BUTTON_SELECT
#define ZX_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == GIGABEAT_S_PAD || \
      CONFIG_KEYPAD == SAMSUNG_YPR0_PAD

#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT  BUTTON_SELECT
#define ZX_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD

#define ZX_RIGHT      BUTTON_RIGHT
#define ZX_LEFT    BUTTON_LEFT
#define ZX_UP    BUTTON_UP
#define ZX_DOWN   BUTTON_DOWN
#define ZX_SELECT     BUTTON_SELECT
#define ZX_MENU    BUTTON_PLAY

#elif CONFIG_KEYPAD == IRIVER_H10_PAD
#define ZX_SELECT BUTTON_REW
#define ZX_MENU BUTTON_FF
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_SCROLL_UP
#define ZX_DOWN BUTTON_SCROLL_DOWN

#elif (CONFIG_KEYPAD == SANSA_E200_PAD) || \
      (CONFIG_KEYPAD == SANSA_C200_PAD) || \
      (CONFIG_KEYPAD == SANSA_CLIP_PAD) || \
      (CONFIG_KEYPAD == SANSA_M200_PAD) || \
      (CONFIG_KEYPAD == SANSA_CONNECT_PAD)
#define ZX_SELECT BUTTON_SELECT
#define ZX_MENU BUTTON_POWER
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)
#define ZX_SELECT BUTTON_SELECT
#define ZX_MENU (BUTTON_HOME|BUTTON_REPEAT)
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif CONFIG_KEYPAD == MROBE500_PAD
#define ZX_UP           BUTTON_RC_PLAY
#define ZX_DOWN         BUTTON_RC_DOWN
#define ZX_LEFT         BUTTON_RC_REW
#define ZX_RIGHT        BUTTON_RC_FF
#define ZX_SELECT    BUTTON_RC_MODE
#define ZX_MENU         (BUTTON_POWER | BUTTON_REL)

#elif CONFIG_KEYPAD == MROBE100_PAD
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_SELECT
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#define ZX_UP           BUTTON_RC_VOL_UP
#define ZX_DOWN         BUTTON_RC_VOL_DOWN
#define ZX_LEFT         BUTTON_RC_REW
#define ZX_RIGHT        BUTTON_RC_FF
#define ZX_SELECT       BUTTON_RC_PLAY
#define ZX_MENU         BUTTON_RC_REC

#elif CONFIG_KEYPAD == COWON_D2_PAD
#define ZX_MENU         (BUTTON_MENU|BUTTON_REL)

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT     BUTTON_SELECT
#define ZX_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == CREATIVE_ZENXFI3_PAD
#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_BACK
#define ZX_RIGHT   BUTTON_MENU
#define ZX_SELECT (BUTTON_PLAY|BUTTON_REL)
#define ZX_MENU   (BUTTON_PLAY|BUTTON_REPEAT)

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD

#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_SELECT
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD6330_PAD

#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_PLAY
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_PREV
#define ZX_RIGHT        BUTTON_NEXT
#define ZX_SELECT       BUTTON_RIGHT
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == ONDAVX747_PAD
#define ZX_MENU         (BUTTON_MENU|BUTTON_REL)

#elif CONFIG_KEYPAD == ONDAVX777_PAD
#define ZX_MENU         BUTTON_POWER

#elif (CONFIG_KEYPAD == SAMSUNG_YH820_PAD) || \
      (CONFIG_KEYPAD == SAMSUNG_YH92X_PAD)

#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_PLAY
#define ZX_SELECT2      BUTTON_FFWD
#define ZX_MENU         BUTTON_REW

#elif CONFIG_KEYPAD == PBELL_VIBE500_PAD

#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_PREV
#define ZX_RIGHT        BUTTON_NEXT
#define ZX_SELECT       BUTTON_OK
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == MPIO_HD200_PAD

#define ZX_MENU         (BUTTON_REC|BUTTON_PLAY)
#define ZX_UP           BUTTON_REW
#define ZX_DOWN         BUTTON_FF
#define ZX_SELECT       BUTTON_FUNC
#define ZX_LEFT         BUTTON_VOL_DOWN
#define ZX_RIGHT        BUTTON_VOL_UP

#elif CONFIG_KEYPAD == MPIO_HD300_PAD

#define ZX_MENU         (BUTTON_MENU|BUTTON_REPEAT)
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_SELECT       BUTTON_ENTER
#define ZX_LEFT         BUTTON_REW
#define ZX_RIGHT        BUTTON_FF

#elif CONFIG_KEYPAD == SANSA_FUZEPLUS_PAD

#define ZX_MENU         BUTTON_PLAYPAUSE
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_SELECT       BUTTON_SELECT
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT

#elif (CONFIG_KEYPAD == HM60X_PAD) || \
    (CONFIG_KEYPAD == HM801_PAD)

#define ZX_MENU         BUTTON_POWER
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_SELECT       BUTTON_SELECT
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT

#elif (CONFIG_KEYPAD == SONY_NWZ_PAD)
#define ZX_MENU         BUTTON_POWER
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_PLAY

#elif (CONFIG_KEYPAD == CREATIVE_ZEN_PAD)
#define ZX_MENU         BUTTON_MENU
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_SELECT

#elif CONFIG_KEYPAD == DX50_PAD
#define ZX_MENU         BUTTON_POWER
#define ZX_UP           BUTTON_VOL_UP
#define ZX_DOWN         BUTTON_VOL_DOWN
#define ZX_SELECT       BUTTON_PLAY
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT

#elif CONFIG_KEYPAD == CREATIVE_ZENXFI2_PAD
#define ZX_MENU         BUTTON_MENU

#elif (CONFIG_KEYPAD == AGPTEK_ROCKER_PAD)
#define ZX_MENU         BUTTON_POWER
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_SELECT

#elif CONFIG_KEYPAD == XDUOO_X3_PAD
#define ZX_SELECT       BUTTON_PLAY
#define ZX_MENU         BUTTON_POWER
#define ZX_LEFT         BUTTON_PREV
#define ZX_RIGHT        BUTTON_NEXT
#define ZX_UP           BUTTON_HOME
#define ZX_DOWN         BUTTON_OPTION

#elif CONFIG_KEYPAD == XDUOO_X3II_PAD || CONFIG_KEYPAD == XDUOO_X20_PAD
#define ZX_SELECT       BUTTON_PLAY
#define ZX_MENU         BUTTON_POWER
#define ZX_LEFT         BUTTON_PREV
#define ZX_RIGHT        BUTTON_NEXT
#define ZX_UP           BUTTON_HOME
#define ZX_DOWN         BUTTON_OPTION

#elif CONFIG_KEYPAD == FIIO_M3K_LINUX_PAD
#define ZX_SELECT       BUTTON_PLAY
#define ZX_MENU         BUTTON_POWER
#define ZX_LEFT         BUTTON_PREV
#define ZX_RIGHT        BUTTON_NEXT
#define ZX_UP           BUTTON_HOME
#define ZX_DOWN         BUTTON_OPTION

#elif CONFIG_KEYPAD == IHIFI_770_PAD || CONFIG_KEYPAD == IHIFI_800_PAD
#define ZX_SELECT       BUTTON_PLAY
#define ZX_MENU         BUTTON_POWER
#define ZX_LEFT         BUTTON_HOME
#define ZX_RIGHT        BUTTON_VOL_DOWN
#define ZX_UP           BUTTON_PREV
#define ZX_DOWN         BUTTON_NEXT

#elif CONFIG_KEYPAD == EROSQ_PAD
#define ZX_SELECT       BUTTON_PLAY
#define ZX_MENU         BUTTON_MENU
#define ZX_LEFT         BUTTON_SCROLL_BACK
#define ZX_RIGHT        BUTTON_SCROLL_FWD
#define ZX_UP           BUTTON_PREV
#define ZX_DOWN         BUTTON_NEXT

#elif CONFIG_KEYPAD == FIIO_M3K_PAD
#define ZX_SELECT       BUTTON_SELECT
#define ZX_MENU         BUTTON_MENU
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN

#else
#error Keymap not defined!

#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef ZX_UP
#define ZX_UP           BUTTON_TOPMIDDLE
#endif
#ifndef ZX_DOWN
#define ZX_DOWN         BUTTON_BOTTOMMIDDLE
#endif
#ifndef ZX_LEFT
#define ZX_LEFT         BUTTON_MIDLEFT
#endif
#ifndef ZX_RIGHT
#define ZX_RIGHT        BUTTON_MIDRIGHT
#endif
#ifndef ZX_SELECT
#define ZX_SELECT       BUTTON_CENTER
#endif
#ifndef ZX_MENU
#define ZX_MENU         (BUTTON_TOPLEFT|BUTTON_REL)
#endif
#endif

#endif
