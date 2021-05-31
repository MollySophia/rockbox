/*
 * This config file is for toshiba Gigabeat S
 */

#define TOSHIBA_GIGABEAT_S 1

#define MODEL_NAME "Toshiba Gigabeat S"

/* System source clock frequencies (Hz) */
#define CONFIG_CKIL_FREQ    32768       /* RTC Crystal, Tuner */
#define CONFIG_CKIH_FREQ    27000000    /* PLL Reference */

/* For Rolo and boot loader */
#define MODEL_NUMBER 21

/* define this if you use an ATA controller */
#define CONFIG_STORAGE STORAGE_ATA

/* For the Gigabeat S, we mount the second partition */
#define CONFIG_DEFAULT_PARTNUM 1

/*define this if the ATA controller and method of USB access support LBA48 */
#define HAVE_LBA48




/* define this if you have a colour LCD */
#define HAVE_LCD_COLOR

/* define this if you want album art for this target */
#define HAVE_ALBUMART

/* define this to enable bitmap scaling */
#define HAVE_BMP_SCALING

/* define this to enable JPEG decoding */
#define HAVE_JPEG

/* define this if you have access to the quickscreen */
#define HAVE_QUICKSCREEN

/* define this if you would like tagcache to build on this target */
#define HAVE_TAGCACHE

/* define this if the target has volume keys which can be used in the lists */
#define HAVE_VOLUME_IN_LIST

/* LCD dimensions */
#define LCD_WIDTH  240
#define LCD_HEIGHT 320
/* sqrt(240^2 + 320^2) / 2.4 = 166.7 */
#define LCD_DPI 167
#define LCD_DEPTH  16   /* 65k colours */
#define LCD_PIXELFORMAT RGB565 /* rgb565 */

#define CONFIG_KEYPAD GIGABEAT_S_PAD

/* Define this to enable morse code input */
#define HAVE_MORSE_INPUT




/* define this if you have a real-time clock */
#define CONFIG_RTC RTC_MC13783

/* Define if the device can wake from an RTC alarm */
#define HAVE_RTC_ALARM

/* Define this for LCD backlight available */
#define HAVE_BACKLIGHT

/* define this if you have a disk storage, i.e. something
   that needs spinups and can cause skips when shaked */
#define HAVE_DISK_STORAGE

/* Define this if you have a software controlled poweroff */
#define HAVE_SW_POWEROFF

/* The number of bytes reserved for loadable codecs */
#define CODEC_SIZE 0x100000

/* The number of bytes reserved for loadable plugins */
#define PLUGIN_BUFFER_SIZE 0x80000

#define AB_REPEAT_ENABLE

/* Define this if you have the WM8978 audio codec */
#define HAVE_WM8978

/* Define bitmask of input sources - recordable bitmask can be defined
   explicitly if different */
#define INPUT_SRC_CAPS SRC_CAP_FMRADIO

/* define the bitmask of hardware sample rates */
#define HW_SAMPR_CAPS (SAMPR_CAP_48 | SAMPR_CAP_44 | SAMPR_CAP_32 | \
                       SAMPR_CAP_24 | SAMPR_CAP_22 | SAMPR_CAP_16 | \
                       SAMPR_CAP_12 | SAMPR_CAP_11 | SAMPR_CAP_8)

/* define the bitmask of recording sample rates */
#define REC_SAMPR_CAPS HW_SAMPR_CAPS /* Same as playback */

/* define default recording levels */
#define DEFAULT_REC_LEFT_GAIN 0
#define DEFAULT_REC_RIGHT_GAIN 0

/* Define this if you have recording capability */
#define HAVE_RECORDING

/* Define this if your LCD can be put to sleep. */
#define HAVE_LCD_SLEEP
/* We don't use a setting but a fixed delay after the backlight has
 * turned off */
#define LCD_SLEEP_TIMEOUT (2*HZ)

/* Define this if your LCD can be enabled/disabled */
#define HAVE_LCD_ENABLE

#ifndef BOOTLOADER
/* Define this if you have a SI4700 fm radio tuner */
#define CONFIG_TUNER SI4700
#define HAVE_RDS_CAP
#define CONFIG_RDS (RDS_CFG_ISR | RDS_CFG_PROCESS)

/* define this if you can flip your LCD */
#define HAVE_LCD_FLIP

/* define this if you can invert the colours on your LCD */
#define HAVE_LCD_INVERT

/* Define this if your LCD can set contrast */
#define HAVE_LCD_CONTRAST

/* Main LCD contrast range and defaults */
#define MIN_CONTRAST_SETTING        0
#define MAX_CONTRAST_SETTING        63
#define DEFAULT_CONTRAST_SETTING    47 /* Match boot contrast */

/* Define this for LCD backlight brightness available */
#define HAVE_BACKLIGHT_BRIGHTNESS

/* Main LCD backlight brightness range and defaults */
#define MIN_BRIGHTNESS_SETTING          1
#define MAX_BRIGHTNESS_SETTING          24
#define DEFAULT_BRIGHTNESS_SETTING      12

/* Implementation-defined fading type with bool settings */
#define CONFIG_BACKLIGHT_FADING BACKLIGHT_FADING_TARGET

#define HAVE_HEADPHONE_DETECTION
#endif /* BOOTLOADER */

#define CONFIG_CPU IMX31L

#define HAVE_FPU

/* Define this if you want to use imx31l's i2c interface */
#define CONFIG_I2C I2C_IMX31L

/* Define the bitmask of modules used */
#define SPI_MODULE_MASK (USE_CSPI2_MODULE | USE_CSPI3_MODULE)
#define I2C_MODULE_MASK (USE_I2C1_MODULE | USE_I2C2_MODULE)
#define GPIO_EVENT_MASK (USE_GPIO1_EVENTS)

/* Define this if target has an additional number of threads specific to it */
#define TARGET_EXTRA_THREADS 1

/* Type of mobile power - check this out */
#define BATTERY_CAPACITY_DEFAULT 700 /* default battery capacity */
#define BATTERY_CAPACITY_MIN     700 /* min. capacity selectable */
#define BATTERY_CAPACITY_MAX    1200 /* max. capacity selectable */
#define BATTERY_CAPACITY_INC      25 /* capacity increment */
#define BATTERY_TYPES_COUNT        1 /* only one type */

#define CONFIG_BATTERY_MEASURE VOLTAGE_MEASURE

/* TODO: have a proper status displayed in the bootloader and have it
 * work! */
/* Charging implemented in a target-specific algorithm */
#define CONFIG_CHARGING CHARGING_TARGET

#define HAVE_USB_CHARGING_ENABLE

/* define this if the hardware can be powered off while charging */
/* We don't charge while powered down so maybe implement a
   finish-charging-and-then-poweroff mode */
#define HAVE_POWEROFF_WHILE_CHARGING

/* The size of the flash ROM */
#define FLASH_SIZE 0x200000

/* Define this to the CPU frequency */
#define CPU_FREQ 264000000 /* Set by retailOS loader */

#define FREQ cpu_frequency

/* Button that exposures boot partition rather than data during session */
#define USB_BL_INSTALL_MODE_BTN     BUTTON_VOL_DOWN

/* define this if the unit can be powered or charged via USB */
#define HAVE_USB_POWER
#define USBPOWER_BUTTON             BUTTON_MENU

#ifndef BOOTLOADER
#define USBPOWER_BTN_IGNORE         USB_BL_INSTALL_MODE_BTN
#else
/* Disable charging-only mode detection in bootloader */
#define USBPOWER_BTN_IGNORE         (BUTTON_MAIN | BUTTON_REMOTE)
#endif

/* define this if the unit has a battery switch or battery can be removed
 * when running */
#define HAVE_BATTERY_SWITCH

/* USB On-the-go */
#define CONFIG_USBOTG USBOTG_ARC

/* enable these for the usb stack */
#define HAVE_USBSTACK
/* usb stack and driver settings */
#define USB_PORTSCX_PHY_TYPE PORTSCX_PTS_ULPI
#define USB_VENDOR_ID 0x0930
#define USB_PRODUCT_ID 0x0010
#define HAVE_USB_HID_MOUSE
#ifdef BOOTLOADER
/* enable bootloader USB mode - ONLY define when also BOOTLOADER */
#define HAVE_BOOTLOADER_USB_MODE
#endif

/* Define this if you have ATA power-off control */
#define HAVE_ATA_POWER_OFF

/* Define this to add support for ATA DMA */
#define HAVE_ATA_DMA

#define CONFIG_LCD LCD_GIGABEATS

/* Offset ( in the firmware file's header ) to the file CRC */
#define FIRMWARE_OFFSET_FILE_CRC 0

/* Offset ( in the firmware file's header ) to the real data */
#define FIRMWARE_OFFSET_FILE_DATA 8

//#define HAVE_SERIAL
#define HAVE_VOLUME_IN_LIST

/*Remove Comments from UART_INT to enable the UART interrupts,*/
/*otherwise iterrupts will be disabled. For now we will test */
/*UART state by polling the registers, and if necessary update this */
/*method by using the interrupts instead*/
//#define UART_INT

/* Define this if you have adjustable CPU frequency */
/* #define HAVE_ADJUSTABLE_CPU_FREQ */

#define HAVE_PCM_DMA_ADDRESS
#define HAVE_PCM_REC_DMA_ADDRESS

#define BOOTFILE_EXT "gigabeat"
#define BOOTFILE "rockbox." BOOTFILE_EXT
#define BOOTDIR "/.rockbox"

/* Define this if a programmable hotkey is mapped */
#define HAVE_HOTKEY
