/*
 * This config file is for the Sansa M200 V4 series
 */

/* For Rolo and boot loader */
#define MODEL_NUMBER 42

#define MODEL_NAME   "Sandisk Sansa m200 v4 series"

/* Enable FAT16 support */
#define HAVE_FAT16SUPPORT

#define HW_SAMPR_CAPS       SAMPR_CAP_ALL_96

/* define this if you have recording possibility */
#define HAVE_RECORDING

#define REC_SAMPR_CAPS      (SAMPR_CAP_48 | SAMPR_CAP_44 | SAMPR_CAP_32 | \
                             SAMPR_CAP_24 | SAMPR_CAP_22 | SAMPR_CAP_16 | \
                             SAMPR_CAP_12 | SAMPR_CAP_11 | SAMPR_CAP_8)

/* because the samplerates don't match at each point, we must be able to
 * tell PCM which set of rates to use. not needed if recording rates are
 * a simple subset of playback rates and are equal values. */
#define CONFIG_SAMPR_TYPES

/* Define bitmask of input sources - recordable bitmask can be defined
   explicitly if different */
#define INPUT_SRC_CAPS (SRC_CAP_MIC | SRC_CAP_FMRADIO)




/* define this if you can flip your LCD */
#define HAVE_LCD_FLIP

/* define this if you can invert the colours on your LCD */
#define HAVE_LCD_INVERT

/* define this if you have access to the quickscreen */
//#define HAVE_QUICKSCREEN

/* define this if you would like tagcache to build on this target */
//#define HAVE_TAGCACHE

/* define this if you have a flash memory storage */
#define HAVE_FLASH_STORAGE

/* define this if the flash memory uses the SecureDigital Memory Card protocol */
#define CONFIG_STORAGE STORAGE_SD

/* LCD dimensions */
#define LCD_WIDTH  128
#define LCD_HEIGHT 64
/* sqrt(128^2 + 64^2) / 1.8 = 79.5 */
#define LCD_DPI 80
#define LCD_DEPTH  1

#define LCD_PIXELFORMAT VERTICAL_PACKING

/* Display colours, for screenshots and sim (0xRRGGBB) */
#define LCD_DARKCOLOR       0x000000
#define LCD_BRIGHTCOLOR     0x5e6854
#define LCD_BL_DARKCOLOR    0x000000
#define LCD_BL_BRIGHTCOLOR  0x3ca0e6

/* define this to indicate your device's keypad */
#define CONFIG_KEYPAD SANSA_M200_PAD

/* Define this to enable morse code input */
#define HAVE_MORSE_INPUT

/* define this if you have a real-time clock */
#ifndef BOOTLOADER
#define CONFIG_RTC RTC_AS3514
#endif

/* define this if you have RTC RAM available for settings */
//#define HAVE_RTC_RAM

/* Define this if you have a software controlled poweroff */
#define HAVE_SW_POWEROFF

/* The number of bytes reserved for loadable codecs */
#define CODEC_SIZE 0x48000 /* in IRAM */

/* The number of bytes reserved for loadable plugins */
#define PLUGIN_BUFFER_SIZE 0x10000

#define AB_REPEAT_ENABLE

#define CONFIG_TUNER TEA5767
#define CONFIG_TUNER_XTAL 32768




/* Define this if you have the TLV320 audio codec */
/*#define HAVE_TLV320*/

/* TLV320 has no tone controls, so we use the software ones */
#define HAVE_SW_TONE_CONTROLS

/* We're working on the assumption that the AS3525 has something
   similar to the AS3514 for audio codec etc */
#define HAVE_AS3514

/* Define this for LCD backlight available */
#define HAVE_BACKLIGHT

/* Define how much SD sectors are reserved for OF */
#define AMS_OF_SIZE 0x5000

#define BATTERY_CAPACITY_DEFAULT 1500 /* default battery capacity */
#define BATTERY_CAPACITY_MIN 1500 /* min. capacity selectable */
#define BATTERY_CAPACITY_MAX 3200 /* max. capacity selectable */
#define BATTERY_CAPACITY_INC 50   /* capacity increment */
#define BATTERY_TYPES_COUNT  1    /* only one type */

#define CONFIG_BATTERY_MEASURE VOLTAGE_MEASURE

/* define this if the unit should not shut down on low battery. */
#define NO_LOW_BATTERY_SHUTDOWN

/* Define this if you have an AMS AS3525 */
#define CONFIG_CPU AS3525

/* Define this if you want to use the AS3525 i2c interface */
#define CONFIG_I2C I2C_AS3525

/* Define this to the CPU frequency */
#define CPU_FREQ      248000000

/* Define this if you have adjustable CPU frequency */
#define HAVE_ADJUSTABLE_CPU_FREQ

/* Offset ( in the firmware file's header ) to the file CRC */
#define FIRMWARE_OFFSET_FILE_CRC 0

/* Offset ( in the firmware file's header ) to the real data */
#define FIRMWARE_OFFSET_FILE_DATA 8

#define CONFIG_USBOTG USBOTG_AS3525

/* enable these for the experimental usb stack */
#define HAVE_USBSTACK
//#define USB_HANDLED_BY_OF
#define USB_VENDOR_ID 0x0781
#define USB_PRODUCT_ID 0x7431
#define HAVE_USB_HID_MOUSE
#define HAVE_BOOTLOADER_USB_MODE

#define CONFIG_LCD LCD_SSD1815

#define BOOTFILE_EXT "sansa"
#define BOOTFILE "rockbox." BOOTFILE_EXT
#define BOOTDIR "/.rockbox"

#define IRAM_LCDFRAMEBUFFER IBSS_ATTR /* put the lcd frame buffer in IRAM */

/* Define this if a programmable hotkey is mapped */
#define HAVE_HOTKEY
