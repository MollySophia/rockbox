/*
 * This config file is for the Creative ZEN Mozaic
 */
#define IMX233_SUBTARGET    3700
#define IMX233_PACKAGE      IMX233_BGA169
#define IMX233_PARTITIONS   IMX233_CREATIVE

/* For Rolo and boot loader */
#define MODEL_NUMBER 87
#define MODEL_NAME   "Creative ZEN Mozaic"

#define HW_SAMPR_CAPS       SAMPR_CAP_ALL_192

/* define this if you have recording possibility */
#define HAVE_RECORDING

#define REC_SAMPR_CAPS      SAMPR_CAP_ALL_96

/* Default recording levels */
#define DEFAULT_REC_MIC_GAIN    23
#define DEFAULT_REC_LEFT_GAIN   23
#define DEFAULT_REC_RIGHT_GAIN  23

/* Define bitmask of input sources - recordable bitmask can be defined
   explicitly if different */
#define INPUT_SRC_CAPS (SRC_CAP_MIC | SRC_CAP_FMRADIO)



/* define this if you have a colour LCD */
#define HAVE_LCD_COLOR

#ifndef BOOTLOADER
#define HAVE_ALBUMART

/* define this to enable bitmap scaling */
#define HAVE_BMP_SCALING

/* define this to enable JPEG decoding */
#define HAVE_JPEG

/* Define this if a programmable hotkey is mapped */
#define HAVE_HOTKEY

/* define this if you have access to the quickscreen */
#define HAVE_QUICKSCREEN

/* define this if you would like tagcache to build on this target */
#define HAVE_TAGCACHE

/* define this if you have LCD enable function */
//#define HAVE_LCD_ENABLE

/* Define this if your LCD can be put to sleep. HAVE_LCD_ENABLE
   should be defined as well.
#define HAVE_LCD_SLEEP
#define HAVE_LCD_SLEEP_SETTING
*/

/* define this if you can flip your LCD
#define HAVE_LCD_FLIP
*/

/* define this if you can invert the colours on your LCD
#define HAVE_LCD_INVERT
*/

/* define this if you have a real-time clock */
#define CONFIG_RTC RTC_IMX233

/* define this if you have a real-time clock with alarm facilities */
#define HAVE_RTC_ALARM

#endif /* !BOOTLOADER */

/* define this if you have an i.MX23 codec */
#define HAVE_IMX233_CODEC

/* Define this if you have a speaker */
#define HAVE_SPEAKER

#define CONFIG_TUNER TEA5760

/* There is no hardware tone control */
#define HAVE_SW_TONE_CONTROLS

#define CONFIG_KEYPAD CREATIVE_ZEN_PAD
#define HAVE_HEADPHONE_DETECTION

/* Define this to enable morse code input */
#define HAVE_MORSE_INPUT




/* LCD dimensions */
#define LCD_WIDTH  128
#define LCD_HEIGHT 160
/* sqrt(128^2 + 160^2) / 1.8 = 113.8 */
#define LCD_DPI 114
#define LCD_DEPTH  16   /* 65536 colours */
#define LCD_PIXELFORMAT RGB565 /* rgb565 */

/* Define this if you have a software controlled poweroff */
#define HAVE_SW_POWEROFF

/* Some devices seem to be FAT16 formatted */
#define HAVE_FAT16SUPPORT

/* The number of bytes reserved for loadable codecs */
#define CODEC_SIZE 0x100000

/* The number of bytes reserved for loadable plugins */
#define PLUGIN_BUFFER_SIZE 0x80000

#define AB_REPEAT_ENABLE

/* Define this for LCD backlight available */
#define HAVE_BACKLIGHT
#define HAVE_BACKLIGHT_BRIGHTNESS

/* Main LCD backlight brightness range and defaults */
#define MIN_BRIGHTNESS_SETTING      0
#define MAX_BRIGHTNESS_SETTING      16
#define DEFAULT_BRIGHTNESS_SETTING  10

/* Which backlight fading type? */
#define CONFIG_BACKLIGHT_FADING BACKLIGHT_FADING_SW_SETTING

/* define this if you have a flash memory storage */
#define HAVE_FLASH_STORAGE

/* define this if the flash memory uses the SecureDigital Memory Card protocol */
#define CONFIG_STORAGE STORAGE_SD

/* todo */
#define BATTERY_CAPACITY_DEFAULT 550    /* default battery capacity */
#define BATTERY_CAPACITY_MIN 550        /* min. capacity selectable */
#define BATTERY_CAPACITY_MAX 550        /* max. capacity selectable */
#define BATTERY_CAPACITY_INC 0          /* capacity increment */
#define BATTERY_TYPES_COUNT  1          /* only one type */

#define CONFIG_BATTERY_MEASURE VOLTAGE_MEASURE

/* Charging implemented in a target-specific algorithm */
//#warning FIXME change this to CHARGING_TARGET when powermgmt is ready
#define CONFIG_CHARGING CHARGING_TARGET

/* define this if the unit can be powered or charged via USB */
#define HAVE_USB_POWER

/* Define this if you have an IMX233*/
#define CONFIG_CPU IMX233

/* Define this if you want to use the IMX233 i2c interface */
#define CONFIG_I2C I2C_IMX233

/* define current usage levels (based on battery bench) */
#define CURRENT_NORMAL     35
#define CURRENT_BACKLIGHT  30
#define CURRENT_RECORD     CURRENT_NORMAL

/* maximum charging current */
#define CURRENT_MAX_CHG   200

/* Define this to the CPU frequency */
#define CPU_FREQ      454000000

/* Type of LCD */
#define CONFIG_LCD LCD_CREATIVEZENMOZAIC

/* Offset ( in the firmware file's header ) to the file CRC and data. These are
   only used when loading the old format rockbox.e200 file */
#define FIRMWARE_OFFSET_FILE_CRC    0x0
#define FIRMWARE_OFFSET_FILE_DATA   0x8

/* USB On-the-go */
#define CONFIG_USBOTG USBOTG_ARC

/* enable these for the experimental usb stack */
#define HAVE_USBSTACK
//#define USB_HANDLED_BY_OF
#define USB_VENDOR_ID 0x041e
#define USB_PRODUCT_ID 0x4133
#define HAVE_USB_HID_MOUSE
#define HAVE_BOOTLOADER_USB_MODE

/* Define this if you have adjustable CPU frequency */
//#define HAVE_ADJUSTABLE_CPU_FREQ

/* Virtual LED (icon) */
#define CONFIG_LED LED_VIRTUAL

#define BOOTFILE_EXT    "creative"
#define BOOTFILE        "rockbox." BOOTFILE_EXT
#define BOOTDIR "/.rockbox"

#define INCLUDE_TIMEOUT_API
