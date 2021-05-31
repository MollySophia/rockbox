/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (c) 2008 by Michael Sevakis
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
#ifndef _MC13783_H_
#define _MC13783_H_

#include "spi-imx31.h"

enum mc13783_regs_enum
{
    MC13783_INTERRUPT_STATUS0            =  0,
    MC13783_INTERRUPT_MASK0              =  1,
    MC13783_INTERRUPT_SENSE0             =  2,
    MC13783_INTERRUPT_STATUS1            =  3,
    MC13783_INTERRUPT_MASK1              =  4,
    MC13783_INTERRUPT_SENSE1             =  5,
    MC13783_POWER_UP_MODE_SENSE          =  6,
    MC13783_IDENTIFICATION               =  7,
    MC13783_SEMAPHORE                    =  8,
    MC13783_ARBITRATION_PERIPHERAL_AUDIO =  9,
    MC13783_ARBITRATION_SWITCHERS        = 10,
    MC13783_ARBITRATION_REGULATORS0      = 11,
    MC13783_ARBITRATION_REGULATORS1      = 12,
    MC13783_POWER_CONTROL0               = 13,
    MC13783_POWER_CONTROL1               = 14,
    MC13783_POWER_CONTROL2               = 15,
    MC13783_REGEN_ASSIGNMENT             = 16,
    MC13783_CONTROL_SPARE                = 17, /* x */
    MC13783_MEMORYA                      = 18,
    MC13783_MEMORYB                      = 19,
    MC13783_RTC_TIME                     = 20,
    MC13783_RTC_ALARM                    = 21,
    MC13783_RTC_DAY                      = 22,
    MC13783_RTC_DAY_ALARM                = 23,
    MC13783_SWITCHERS0                   = 24,
    MC13783_SWITCHERS1                   = 25,
    MC13783_SWITCHERS2                   = 26,
    MC13783_SWITCHERS3                   = 27,
    MC13783_SWITCHERS4                   = 28,
    MC13783_SWITCHERS5                   = 29,
    MC13783_REGULATOR_SETTING0           = 30,
    MC13783_REGULATOR_SETTING1           = 31,
    MC13783_REGULATOR_MODE0              = 32,
    MC13783_REGULATOR_MODE1              = 33,
    MC13783_POWER_MISCELLANEOUS          = 34,
    MC13783_POWER_SPARE                  = 35, /* x */
    MC13783_AUDIO_RX0                    = 36,
    MC13783_AUDIO_RX1                    = 37,
    MC13783_AUDIO_TX                     = 38,
    MC13783_SSI_NETWORK                  = 39,
    MC13783_AUDIO_CODEC                  = 40,
    MC13783_AUDIO_STEREO_DAC             = 41,
    MC13783_AUDIO_SPARE                  = 42, /* x */
    MC13783_ADC0                         = 43,
    MC13783_ADC1                         = 44,
    MC13783_ADC2                         = 45,
    MC13783_ADC3                         = 46,
    MC13783_ADC4                         = 47,
    MC13783_CHARGER                      = 48,
    MC13783_USB0                         = 49,
    MC13783_CHARGER_USB1                 = 50,
    MC13783_LED_CONTROL0                 = 51,
    MC13783_LED_CONTROL1                 = 52,
    MC13783_LED_CONTROL2                 = 53,
    MC13783_LED_CONTROL3                 = 54,
    MC13783_LED_CONTROL4                 = 55,
    MC13783_LED_CONTROL5                 = 56,
    MC13783_SPARE                        = 57, /* x */
    MC13783_TRIM0                        = 58, /* x */
    MC13783_TRIM1                        = 59, /* x */
    MC13783_TEST0                        = 60, /* x */
    MC13783_TEST1                        = 61, /* x */
    MC13783_TEST2                        = 62, /* x */
    MC13783_TEST3                        = 63, /* x */
    MC13783_NUM_REGS                     = 64,
};
/* x = unused/reserved/not implemented */

/* INTERRUPT_STATUS0 (0) */
#define MC13783_ADCDONEI                (0x1 << 0)
#define MC13783_ADCBISDONEI             (0x1 << 1)
#define MC13783_TSI                     (0x1 << 2)
#define MC13783_WHIGHI                  (0x1 << 3)
#define MC13783_WLOWI                   (0x1 << 4)
#define MC13783_CHGDETI                 (0x1 << 6)
#define MC13783_CHGOVI                  (0x1 << 7)
#define MC13783_CHGREVI                 (0x1 << 8)
#define MC13783_CHGSHORTI               (0x1 << 9)
#define MC13783_CCCVI                   (0x1 << 10)
#define MC13783_CHGCURRI                (0x1 << 11)
#define MC13783_BPONII                  (0x1 << 12)
#define MC13783_LOBATLI                 (0x1 << 13)
#define MC13783_LOBATHI                 (0x1 << 14)
#define MC13783_UDPI                    (0x1 << 15)
#define MC13783_USBI                    (0x1 << 16)
#define MC13783_IDFLOATI                (0x1 << 19)
#define MC13783_SE1I                    (0x1 << 21)
#define MC13783_CKDETI                  (0x1 << 22)
#define MC13783_UDMI                    (0x1 << 23)

/* INTERRUPT_MASK0 (1) */
#define MC13783_ADCDONEM                (0x1 << 0)
#define MC13783_ADCBISDONEM             (0x1 << 1)
#define MC13783_TSM                     (0x1 << 2)
#define MC13783_WHIGHM                  (0x1 << 3)
#define MC13783_WLOWM                   (0x1 << 4)
#define MC13783_CHGDETM                 (0x1 << 6)
#define MC13783_CHGOVM                  (0x1 << 7)
#define MC13783_CHGREVM                 (0x1 << 8)
#define MC13783_CHGSHORTM               (0x1 << 9)
#define MC13783_CCCVM                   (0x1 << 10)
#define MC13783_CHGCURRM                (0x1 << 11)
#define MC13783_BPONIM                  (0x1 << 12)
#define MC13783_LOBATLM                 (0x1 << 13)
#define MC13783_LOBATHM                 (0x1 << 14)
#define MC13783_UDPM                    (0x1 << 15)
#define MC13783_USBM                    (0x1 << 16)
#define MC13783_IDFLOATM                (0x1 << 19)
#define MC13783_SE1M                    (0x1 << 21)
#define MC13783_CKDETM                  (0x1 << 22)
#define MC13783_UDMM                    (0x1 << 23)

/* INTERRUPT_SENSE0 (2) */
#define MC13783_CHGDETS                 (0x1 << 6)
#define MC13783_CHGOVS                  (0x1 << 7)
#define MC13783_CHGREVS                 (0x1 << 8)
#define MC13783_CHGSHORTS               (0x1 << 9)
#define MC13783_CCCVS                   (0x1 << 10)
#define MC13783_CHGCURRS                (0x1 << 11)
#define MC13783_BPONIS                  (0x1 << 12)
#define MC13783_LOBATLS                 (0x1 << 13)
#define MC13783_LOBATHS                 (0x1 << 14)
#define MC13783_UDPS                    (0x1 << 15)
#define MC13783_USB4V4S                 (0x1 << 16)
#define MC13783_USB2V0S                 (0x1 << 17)
#define MC13783_USB0V8S                 (0x1 << 18)
#define MC13783_IDFLOATS                (0x1 << 19)
#define MC13783_SE1S                    (0x1 << 21)
#define MC13783_CKDETS                  (0x1 << 22)
#define MC13783_UDMS                    (0x1 << 23)

/* INTERRUPT_STATUS1 (3) */
#define MC13783_1HZI                    (0x1 << 0)
#define MC13783_TODAI                   (0x1 << 1)
#define MC13783_ONOFD1I                 (0x1 << 3)  /* ON1B */
#define MC13783_ONOFD2I                 (0x1 << 4)  /* ON2B */
#define MC13783_ONOFD3I                 (0x1 << 5)  /* ON3B */
#define MC13783_SYSRSTI                 (0x1 << 6)
#define MC13783_RTCRSTI                 (0x1 << 7)
#define MC13783_PCII                    (0x1 << 8)
#define MC13783_WARMI                   (0x1 << 9)
#define MC13783_MEMHLDI                 (0x1 << 10)
#define MC13783_PWRRDYI                 (0x1 << 11)
#define MC13783_THWARNLI                (0x1 << 12)
#define MC13783_THWARNHI                (0x1 << 13)
#define MC13783_CLKI                    (0x1 << 14)
#define MC13783_SEMAFI                  (0x1 << 15)
#define MC13783_MC2BI                   (0x1 << 17)
#define MC13783_HSDETI                  (0x1 << 18)
#define MC13783_HSLI                    (0x1 << 19)
#define MC13783_ALSPTHI                 (0x1 << 20)
#define MC13783_AHSSHORTI               (0x1 << 21)

/* INTERRUPT_MASK1 (4) */
#define MC13783_1HZM                    (0x1 << 0)
#define MC13783_TODAM                   (0x1 << 1)
#define MC13783_ONOFD1M                 (0x1 << 3)  /* ON1B */
#define MC13783_ONOFD2M                 (0x1 << 4)  /* ON2B */
#define MC13783_ONOFD3M                 (0x1 << 5)  /* ON3B */
#define MC13783_SYSRSTM                 (0x1 << 6)
#define MC13783_RTCRSTM                 (0x1 << 7)
#define MC13783_PCIM                    (0x1 << 8)
#define MC13783_WARMM                   (0x1 << 9)
#define MC13783_MEMHLDM                 (0x1 << 10)
#define MC13783_PWRRDYM                 (0x1 << 11)
#define MC13783_THWARNLM                (0x1 << 12)
#define MC13783_THWARNHM                (0x1 << 13)
#define MC13783_CLKM                    (0x1 << 14)
#define MC13783_SEMAFM                  (0x1 << 15)
#define MC13783_MC2BM                   (0x1 << 17)
#define MC13783_HSDETM                  (0x1 << 18)
#define MC13783_HSLM                    (0x1 << 19)
#define MC13783_ALSPTHM                 (0x1 << 20)
#define MC13783_AHSSHORTM               (0x1 << 21)

/* INTERRUPT_SENSE1 (5) */
#define MC13783_ONOFD1S                 (0x1 << 3)  /* ON1B */
#define MC13783_ONOFD2S                 (0x1 << 4)  /* ON2B */
#define MC13783_ONOFD3S                 (0x1 << 5)  /* ON3B */
#define MC13783_PWRRDYS                 (0x1 << 11)
#define MC13783_THWARNLS                (0x1 << 12)
#define MC13783_THWARNHS                (0x1 << 13)
#define MC13783_CLKS                    (0x1 << 14)
#define MC13783_MC2BS                   (0x1 << 17)
#define MC13783_HSDETS                  (0x1 << 18)
#define MC13783_HSLS                    (0x1 << 19)
#define MC13783_ALSPTHS                 (0x1 << 20)
#define MC13783_AHSSHORTS               (0x1 << 21)

/* POWER_UP_MODE_SENSE (6) */
#define MC13783_ICTESTS                 (0x1 << 0)
#define MC13783_CLKSELS                 (0x1 << 1)
#define MC13783_PUMS1S                  (0x3 << 2)
    #define MC13783_PUMS1S_LOW          (0x0 << 2)
    #define MC13783_PUMS1S_OPEN         (0x1 << 2)
    #define MC13783_PUMS1S_HIGH         (0x2 << 2)
#define MC13783_PUMS2S                  (0x3 << 4)
    #define MC13783_PUMS2S_LOW          (0x0 << 4)
    #define MC13783_PUMS2S_OPEN         (0x1 << 4)
    #define MC13783_PUMS2S_HIGH         (0x2 << 4)
#define MC13783_PUMS3S                  (0x3 << 6)
    #define MC13783_PUMS3S_LOW          (0x0 << 6)
    #define MC13783_PUMS3S_OPEN         (0x1 << 6)
    #define MC13783_PUMS3S_HIGH         (0x2 << 6)
#define MC13783_CHRGMOD0S               (0x3 << 8)
    #define MC13783_CHRGMOD0S_LOW       (0x0 << 8)
    #define MC13783_CHRGMOD0S_OPEN      (0x1 << 8)
    #define MC13783_CHRGMOD0S_HIGH      (0x3 << 8)
#define MC13783_CHRGMOD1S               (0x3 << 10)
    #define MC13783_CHRGMOD1S_LOW       (0x0 << 10)
    #define MC13783_CHRGMOD1S_OPEN      (0x1 << 10)
    #define MC13783_CHRGMOD1S_HIGH      (0x3 << 10)
#define MC13783_UMODS                   (0x3 << 12)
    #define MC13783_UMODS_LOW_UMODS1_LOW        (0x0 << 12)
    #define MC13783_UMODS_OPEN_UMODS1_LOW       (0x1 << 12)
    #define MC13783_UMODS_DONTCARE_UMODS1_HIGH  (0x2 << 12)
    #define MC13783_UMODS_HIGH_UMODS1_LOW       (0x3 << 12)
#define MC13783_USBENS                  (0x1 << 14)
#define MC13783_SW1ABS                  (0x1 << 15)
#define MC13783_SW2ABS                  (0x1 << 16)

/* IDENTIFICATION (7) */
#define MC13783_REVISION                (0x1f << 0)
#define MC13783_REVISION_POS            (0)
#define MC13783_ICID                    (0x7 << 6)
#define MC13783_ICID_POS                (6)
#define MC13783_FIN                     (0x3 << 9)
#define MC13783_FIN_POS                 (9)
#define MC13783_FAB                     (0x3 << 12)
#define MC13783_FAB_POS                 (12)

/* SEMAPHORE (8) */
#define MC13783_SEMCTRLA                (0x1 << 0)
#define MC13783_SEMCTRLB                (0x1 << 2)
#define MC13783_SEMWRTA                 (0xf << 4)
#define MC13783_SEMWRTA_POS             (4)
#define MC13783_SEMWRTB                 (0x3f << 8)
#define MC13783_SEMWRTB_POS             (8)
#define MC13783_SEMRDA                  (0xf << 14)
#define MC13783_SEMRDA_POS              (14)
#define MC13783_SEMRDB                  (0x3f << 18)
#define MC13783_SEMRDB_POS              (18)

/* ARBITRATION_PERIPHERAL_AUDIO (9) */
#define MC13783_AUDIOTXSEL              (0x3 << 0)
    #define MC13783_AUDIOTXSEL_PRI_SPI  (0x0 << 0)
    #define MC13783_AUDIOTXSEL_SEC_SPI  (0x1 << 0)
    #define MC13783_AUDIOTXSEL_OR_SPI   (0x2 << 0)
    #define MC13783_AUDIOTXSEL_AND_SPI  (0x3 << 0)
#define MC13783_TXGAINSEL               (0x1 << 2)
#define MC13783_AUDIORXSEL              (0x3 << 3)
    #define MC13783_AUDIORXSEL_PRI_SPI  (0x0 << 3)
    #define MC13783_AUDIORXSEL_SEC_SPI  (0x1 << 3)
    #define MC13783_AUDIORXSEL_OR_SPI   (0x2 << 3)
    #define MC13783_AUDIORXSEL_AND_SPI  (0x3 << 3)
#define MC13783_RXGAINSEL               (0x1 << 5)
#define MC13783_AUDIOCDCSEL             (0x1 << 6)
#define MC13783_AUDIOSTDCSEL            (0x1 << 7)
#define MC13783_BIASSEL                 (0x3 << 8)
    #define MC13783_BIASSEL_PRI_SPI     (0x0 << 8)
    #define MC13783_BIASSEL_SEC_SPI     (0x1 << 8)
    #define MC13783_BIASSEL_OR_SPI      (0x2 << 8)
    #define MC13783_BIASSEL_AND_SPI     (0x3 << 8)
#define MC13783_RTCSEL                  (0x1 << 11)
#define MC13783_ADCSEL                  (0x3 << 12)
    #define MC13783_ADCSEL_PRI1_SEC1    (0x0 << 12)
    #define MC13783_ADCSEL_PRI2_SEC0    (0x1 << 12)
    #define MC13783_ADCSEL_PRI0_SEC2    (0x2 << 12)
    /* 0x3 = same as 0x0 */
#define MC13783_USBSEL                  (0x1 << 14)
#define MC13783_CHRGSEL                 (0x1 << 15)
#define MC13783_BLLEDSEL                (0x1 << 16)
#define MC13783_TCLEDSEL                (0x1 << 17)
#define MC13783_ADAPTSEL                (0x1 << 18)

/* ARBITRATION_SWITCHERS (10) */
#define MC13783_SW1ASTBYAND             (0x1 << 0)
#define MC13783_SW1BSTBYAND             (0x1 << 1)
#define MC13783_SW2ASTBYAND             (0x1 << 2)
#define MC13783_SW2BSTBYAND             (0x1 << 3)
#define MC13783_SW3SEL0                 (0x1 << 4)
#define MC13783_SW1ABDVS                (0x1 << 5)
#define MC13783_SW2ABDVS                (0x1 << 6)
#define MC13783_SW1ASEL                 (0x1 << 7)
#define MC13783_SW1BSEL                 (0x1 << 8)
#define MC13783_SW2ASEL                 (0x1 << 9)
#define MC13783_SW2BSEL                 (0x1 << 10)
#define MC13783_PLLSEL                  (0x1 << 12)
#define MC13783_PWGT1SEL                (0x1 << 14)
#define MC13783_PWGT2SEL                (0x1 << 15)

/* ARBITRATION_REGULATORS0 (11) */
#define MC13783_VAUDIOSEL               (0x3 << 0)
    #define MC13783_VAUDIOSEL_PRI       (0x0 << 0)
#define MC13783_VIOHISEL                (0x3 << 2)
    #define MC13783_VIOHISEL_PRI        (0x0 << 2)
#define MC13783_VIOLOSEL                (0x3 << 4)
    #define MC13783_VIOLOSEL_PRI        (0x0 << 4)
#define MC13783_VDIGSEL                 (0x3 << 6)
    #define MC13783_VDIGSEL_PRI         (0x0 << 6)
#define MC13783_VGENSEL                 (0x3 << 8)
    #define MC13783_VGENSEL_PRI         (0x0 << 8)
#define MC13783_VRFDIGSEL               (0x3 << 10)
    #define MC13783_VRFDIGSEL_PRI       (0x0 << 10)
#define MC13783_VRFREFSEL               (0x3 << 12)
    #define MC13783_VRFREFSEL_PRI       (0x0 << 12)
#define MC13783_VRFCPSEL                (0x3 << 14)
    #define MC13783_VRFCPSEL_PRI        (0x0 << 14)
#define MC13783_VSIMSEL                 (0x3 << 16)
    #define MC13783_VSIMSEL_PRI         (0x0 << 16)
#define MC13783_VESIMSEL                (0x3 << 18)
    #define MC13783_VESIMSEL_PRI        (0x0 << 18)
#define MC13783_VCAMSEL                 (0x3 << 20)
    #define MC13783_VCAMSEL_PRI         (0x0 << 20)
#define MC13783_VRFBGSEL                (0x3 << 22)
    #define MC13783_VRFBGSEL_PRI        (0x0 << 22)

/* ARBITRATION_REGULATORS1 (12) */
#define MC13783_VVIBSEL                 (0x3 << 0)
    #define MC13783_VVIBSEL_PRI         (0x0 << 0)
#define MC13783_VRF1SEL                 (0x3 << 2)
    #define MC13783_VRF1SEL_PRI         (0x0 << 2)
#define MC13783_VRF2SEL                 (0x3 << 4)
    #define MC13783_VRF2SEL_PRI         (0x0 << 4)
#define MC13783_VMMC1SEL                (0x3 << 6)
    #define MC13783_VMMC1SEL_PRI        (0x0 << 6)
#define MC13783_VMMC2SEL                (0x3 << 8)
    #define MC13783_VMMC2SEL_PRI        (0x0 << 8)
#define MC13783_GPO1SEL                 (0x3 << 14)
    #define MC13783_GPO1SEL_PRI         (0x0 << 14)
    #define MC13783_GPO1SEL_BOTH        (0x1 << 14)
    #define MC13783_GPO1SEL_AND         (0x3 << 14)
#define MC13783_GPO2SEL                 (0x3 << 16)
    #define MC13783_GPO2SEL_PRI         (0x0 << 16)
    #define MC13783_GPO2SEL_BOTH        (0x1 << 16)
    #define MC13783_GPO2SEL_AND         (0x3 << 16)
#define MC13783_GPO3SEL                 (0x3 << 18)
    #define MC13783_GPO3SEL_PRI         (0x0 << 18)
    #define MC13783_GPO3SEL_BOTH        (0x1 << 18)
    #define MC13783_GPO3SEL_AND         (0x3 << 18)
#define MC13783_GPO4SEL                 (0x3 << 20)
    #define MC13783_GPO4SEL_PRI         (0x0 << 20)
    #define MC13783_GPO4SEL_BOTH        (0x1 << 20)
    #define MC13783_GPO4SEL_AND         (0x3 << 20)

/* POWER_CONTROL0 (13) */
#define MC13783_PCEN                    (0x1 << 0)
#define MC13783_PCCOUNTEN               (0x1 << 1)
#define MC13783_WARMEN                  (0x1 << 2)
#define MC13783_USEROFFSPI              (0x1 << 3)
#define MC13783_USEROFFPC               (0x1 << 4)
#define MC13783_USEROFFCLK              (0x1 << 5)
#define MC13783_CLK32KMCUEN             (0x1 << 6)
#define MC13783_VBKUP2AUTOMH            (0x1 << 7)
#define MC13783_VBKUP1EN                (0x1 << 8)
#define MC13783_VBKUPAUTO               (0x1 << 9)
#define MC13783_VBKUP1                  (0x3 << 10)
    #define MC13783_VBKUP1_1_0V         (0x0 << 10)
    #define MC13783_VBKUP1_1_2V         (0x1 << 10)
    #define MC13783_VBKUP1_1_575V       (0x2 << 10)
    #define MC13783_VBKUP1_1_8V         (0x3 << 10)
#define MC13783_VBKUP2EN                (0x1 << 12)
#define MC13783_VBKUP2AUTO              (0x1 << 13)
#define MC13783_VBKUP2                  (0x3 << 14)
    #define MC13783_VBKUP2_1_0V         (0x0 << 14)
    #define MC13783_VBKUP2_1_2V         (0x1 << 14)
    #define MC13783_VBKUP2_1_5V         (0x2 << 14)
    #define MC13783_VBKUP2_1_8V         (0x3 << 14)
#define MC13783_BPDET                   (0x3 << 16)
    /* 00: UVDET 2.6, LOBATL UVDET+0.2, LOBATH UVDET+0.4 BPON 3.2 */
    #define MC13783_BPDET_2_4           (0x0 << 16)
    /* 01: UVDET 2.6, LOBATL UVDET+0.3, LOBATH UVDET+0.5 BPON 3.2 */
    #define MC13783_BPDET_3_5           (0x1 << 16)
    /* 10: UVDET 2.6, LOBATL UVDET+0.4, LOBATH UVDET+0.7 BPON 3.2 */
    #define MC13783_BPDET_4_7           (0x2 << 16)
    /* 11: UVDET 2.6, LOBATL UVDET+0.5, LOBATH UVDET+0.8 BPON 3.2 */
    #define MC13783_BPDET_5_8           (0x3 << 16)
#define MC13783_EOLSEL                  (0x1 << 18)
#define MC13783_BATTDETEN               (0x1 << 19)
#define MC13783_VCOIN                   (0x7 << 20)
    #define MC13783_VCOIN_2_50V         (0x0 << 20)
    #define MC13783_VCOIN_2_70V         (0x1 << 20)
    #define MC13783_VCOIN_2_80V         (0x2 << 20)
    #define MC13783_VCOIN_2_90V         (0x3 << 20)
    #define MC13783_VCOIN_3_00V         (0x4 << 20)
    #define MC13783_VCOIN_3_10V         (0x5 << 20)
    #define MC13783_VCOIN_3_20V         (0x6 << 20)
    #define MC13783_VCOIN_3_30V         (0x7 << 20)
#define MC13783_COINCHEN                (0x1 << 23)

/* POWER_CONTROL1 (14) */
    /* Up to 8 seconds */
#define MC13783_PCT                     (0xff << 0)
#define MC13783_PCT_POS                 (0)
#define MC13783_PCCOUNT                 (0xf << 8)
#define MC13783_PCCOUNT_POS             (8)
#define MC13783_PCMAXCNT                (0xf << 12)
#define MC13783_PCMAXCNT_POS            (12)
    /* Up to 8 minutes with MEMALLON=0, <> 0 + MEMALLON=1: infinite */
#define MC13783_MEMTMR                  (0xf << 16)
#define MC13783_MEMTMR_POS              (16)
#define MC13783_MEMALLON                (0x1 << 20)

/* POWER_CONTROL2 (15) */
#define MC13783_RESTARTEN               (0x1 << 0)
#define MC13783_ON1BRSTEN               (0x1 << 1)
#define MC13783_ON2BRSTEN               (0x1 << 2)
#define MC13783_ON3BTSTEN               (0x1 << 3)
#define MC13783_ON1BDBNC                (0x3 << 4)
    #define MC13783_ON1BDBNC_0MS        (0x0 << 4)
    #define MC13783_ON1BDBNC_30MS       (0x1 << 4)
    #define MC13783_ON1BDBNC_150MS      (0x2 << 4)
    #define MC13783_ON1BDBNC_750MS      (0x3 << 4)
#define MC13783_ON2BDBNC                (0x3 << 6)
    #define MC13783_ON2BDBNC_0MS        (0x0 << 6)
    #define MC13783_ON2BDBNC_30MS       (0x1 << 6)
    #define MC13783_ON2BDBNC_150MS      (0x2 << 6)
    #define MC13783_ON2BDBNC_750MS      (0x3 << 6)
#define MC13783_ON3BDBNC                (0x3 << 8)
    #define MC13783_ON3BDBNC_0MS        (0x0 << 8)
    #define MC13783_ON3BDBNC_30MS       (0x1 << 8)
    #define MC13783_ON3BDBNC_150MS      (0x2 << 8)
    #define MC13783_ON3BDBNC_750MS      (0x3 << 8)
#define MC13783_STANDBYPRIINV           (0x1 << 10)
#define MC13783_STANDBYSECINV           (0x1 << 11)

/* REGEN_ASSIGNMENT (16) */
#define MC13783_VAUDIOREGEN             (0x1 << 0)
#define MC13783_VIOHIREGEN              (0x1 << 1)
#define MC13783_VIOLOREGEN              (0x1 << 2)
#define MC13783_VDIGREGEN               (0x1 << 3)
#define MC13783_VGENREGEN               (0x1 << 4)
#define MC13783_VRFDIGREGEN             (0x1 << 5)
#define MC13783_VRFREFREGEN             (0x1 << 6)
#define MC13783_VRFCPREGEN              (0x1 << 7)
#define MC13783_VCAMREGEN               (0x1 << 8)
#define MC13783_VRFBGREGEN              (0x1 << 9)
#define MC13783_VRF1REGEN               (0x1 << 10)
#define MC13783_VRF2REGEN               (0x1 << 11)
#define MC13783_VMMC1REGEN              (0x1 << 12)
#define MC13783_VMMC2REGEN              (0x1 << 13)
#define MC13783_GPO1REGEN               (0x1 << 16)
#define MC13783_GPO2REGEN               (0x1 << 17)
#define MC13783_GPO3REGEN               (0x1 << 18)
#define MC13783_GPO4REGEN               (0x1 << 19)
#define MC13783_REGENINV                (0x1 << 20)
#define MC13783_VESIMESIMEN             (0x1 << 21)
#define MC13783_VMMC1ESIMEN             (0x1 << 22)
#define MC13783_VMMC2ESIMEN             (0x1 << 23)

/* MEMORYA (18) */
#define MC13783_MEMORYA_MASK            (0xffffff)

/* MEMORYB (19) */
#define MC13783_MEMORYB_MASK            (0xffffff)

/* RTC_TIME (20) */
#define MC13783_RTC_TIME_MASK           (0x1ffff)

/* RTC_ALARM (21) */
#define MC13783_RTC_ALARM_MASK          (0x1ffff)

/* RTC_DAY (22) */
#define MC13783_RTC_DAY_MASK            (0x7fff)

/* RTC_DAY_ALARM (23) */
#define MC13783_RTC_DAY_ALARM_MASK      (0x7fff)

/* SWITCHERS0 (24) */
#define MC13783_SW1A                    (0x3f << 0)
#define MC13783_SW1A_POS                (0)
#define MC13783_SW1ADVS                 (0x3f << 6)
#define MC13783_SW1ADVS_POS             (6)
#define MC13783_SW1ASTBY                (0x3f << 12)
#define MC13783_SW1ASTBY_POS            (12)

/* SWITCHERS1 (25) */
#define MC13783_SW1B                    (0x3f << 0)
#define MC13783_SW1B_POS                (0)
#define MC13783_SW1BDVS                 (0x3f << 6)
#define MC13783_SW1BDVS_POS             (6)
#define MC13783_SW1BSTBY                (0x3f << 12)
#define MC13783_SW1BSTBY_POS            (12)

/* SWITCHERS2 (26) */
#define MC13783_SW2A                    (0x3f << 0)
#define MC13783_SW2A_POS                (0)
#define MC13783_SW2ADVS                 (0x3f << 6)
#define MC13783_SW2ADVS_POS             (6)
#define MC13783_SW2ASTBY                (0x3f << 12)
#define MC13783_SW2ASTBY_POS            (12)

/* SWITCHERS3 (27) */
#define MC13783_SW2B                    (0x3f << 0)
#define MC13783_SW2B_POS                (0)
#define MC13783_SW2BDVS                 (0x3f << 6)
#define MC13783_SW2BDVS_POS             (6)
#define MC13783_SW2BSTBY                (0x3f << 12)
#define MC13783_SW2BSTBY_POS            (12)

/* Switcher Voltages (SWITCHERS0-SWITCHERS3) */
/* Switcher 1 and 2 */
#define MC13783_SW_0_900                0x00 /* 0.900 V */
#define MC13783_SW_0_925                0x01 /* 0.925 V */
#define MC13783_SW_0_950                0x02 /* 0.950 V */
#define MC13783_SW_0_975                0x03 /* 0.975 V */
#define MC13783_SW_1_000                0x04 /* 1.000 V */
#define MC13783_SW_1_025                0x05 /* 1.025 V */
#define MC13783_SW_1_050                0x06 /* 1.050 V */
#define MC13783_SW_1_075                0x07 /* 1.075 V */
#define MC13783_SW_1_100                0x08 /* 1.100 V */
#define MC13783_SW_1_125                0x09 /* 1.125 V */
#define MC13783_SW_1_150                0x0a /* 1.150 V */
#define MC13783_SW_1_175                0x0b /* 1.175 V */
#define MC13783_SW_1_200                0x0c /* 1.200 V */
#define MC13783_SW_1_225                0x0d /* 1.225 V */
#define MC13783_SW_1_250                0x0e /* 1.250 V */
#define MC13783_SW_1_275                0x0f /* 1.275 V */
#define MC13783_SW_1_300                0x10 /* 1.300 V */
#define MC13783_SW_1_325                0x11 /* 1.325 V */
#define MC13783_SW_1_350                0x12 /* 1.350 V */
#define MC13783_SW_1_375                0x13 /* 1.375 V */
#define MC13783_SW_1_400                0x14 /* 1.400 V */
#define MC13783_SW_1_425                0x15 /* 1.425 V */
#define MC13783_SW_1_450                0x16 /* 1.450 V */
#define MC13783_SW_1_475                0x17 /* 1.475 V */
#define MC13783_SW_1_500                0x18 /* 1.500 V */
#define MC13783_SW_1_525                0x19 /* 1.525 V */
#define MC13783_SW_1_550                0x1a /* 1.550 V */
#define MC13783_SW_1_575                0x1b /* 1.575 V */
#define MC13783_SW_1_600                0x1c /* 1.600 V */
#define MC13783_SW_1_625                0x1d /* 1.625 V */
#define MC13783_SW_1_650                0x1e /* 1.650 V */
#define MC13783_SW_1_675                0x1f /* 1.675 V */
#define MC13783_SW_1_700                0x20 /* 0x20 - 0x23 = 1.700V */
#define MC13783_SW_1_800                0x24 /* 0x24 - 0x27 = 1.800V */
/* Switcher 1 */
#define MC13783_SW1_1_850               0x28 /* 0x29 - 0x2b = 1.850V */
/* Switcher 2 */
#define MC13783_SW2_1_900               0x28 /* 0x29 - 0x2b = 1.900V */
/* Switcher 1 and 2 */
#define MC13783_SW_2_000                0x2c /* 0x2c - 0x2f = 2.000V */
#define MC13783_SW_2_100                0x30 /* 0x30 - 0x33 = 2.100V */
#define MC13783_SW_2_200                0x34 /* 0x34 - 0x3f = 2.200V */


/* SWITCHERS4 (28) */
#define MC13783_SW1AMODE                (0x3 << 0)
    #define MC13783_SW1AMODE_OFF        (0x0 << 0)
    #define MC13783_SW1AMODE_PWM        (0x1 << 0)
    #define MC13783_SW1AMODE_PWM_SKIP   (0x2 << 0)
    #define MC13783_SW1AMODE_PFM        (0x3 << 0)
#define MC13783_SW1ASTBYMODE            (0x3 << 2)
    #define MC13783_SW1ASTBYMODE_OFF      (0x0 << 2)
    #define MC13783_SW1ASTBYMODE_PWM      (0x1 << 2)
    #define MC13783_SW1ASTBYMODE_PWM_SKIP (0x2 << 2)
    #define MC13783_SW1ASTBYMODE_PFM      (0x3 << 2)
#define MC13783_SW1ADVSSPEED            (0x3 << 6)
    /* 25mV every ... */
    #define MC13783_SW1ADVSSPEED_4US_NO_PWR_RDY (0x0 << 6)
    #define MC13783_SW1ADVSSPEED_4US            (0x1 << 6)
    #define MC13783_SW1ADVSSPEED_8US            (0x2 << 6)
    #define MC13783_SW1ADVSSPEED_16US           (0x3 << 6)
#define MC13783_SW1APANIC               (0x1 << 8)
#define MC13783_SW1ASFST                (0x1 << 9)
#define MC13783_SW1BMODE                (0x3 << 10)
    #define MC13783_SW1BMODE_OFF        (0x0 << 10)
    #define MC13783_SW1BMODE_PWM        (0x1 << 10)
    #define MC13783_SW1BMODE_PWM_SKIP   (0x2 << 10)
    #define MC13783_SW1BMODE_PFM        (0x3 << 10)
#define MC13783_SW1BSTBYMODE            (0x3 << 12)
    #define MC13783_SW1BSTBYMODE_OFF      (0x0 << 12)
    #define MC13783_SW1BSTBYMODE_PWM      (0x1 << 12)
    #define MC13783_SW1BSTBYMODE_PWM_SKIP (0x2 << 12)
    #define MC13783_SW1BSTBYMODE_PFM      (0x3 << 12)
#define MC13783_SW1BDVSSPEED            (0x3 << 14)
    /* 25mV every ... */
    #define MC13783_SW1BDVSSPEED_4US_NO_PWR_RDY (0x0 << 14)
    #define MC13783_SW1BDVSSPEED_4US            (0x1 << 14)
    #define MC13783_SW1BDVSSPEED_8US            (0x2 << 14)
    #define MC13783_SW1BDVSSPEED_16US           (0x3 << 14)
#define MC13783_SW1BPANIC               (0x1 << 16)
#define MC13783_SW1BSFST                (0x1 << 17)
#define MC13783_PLLEN                   (0x1 << 18)
#define MC13783_PLLX                    (0x7 << 19)
    #define MC13783_PLLX_28             (0x0 << 19)
    #define MC13783_PLLX_29             (0x1 << 19)
    #define MC13783_PLLX_30             (0x2 << 19)
    #define MC13783_PLLX_31             (0x3 << 19)
    #define MC13783_PLLX_32             (0x4 << 19)
    #define MC13783_PLLX_33             (0x5 << 19)
    #define MC13783_PLLX_34             (0x6 << 19)
    #define MC13783_PLLX_35             (0x7 << 19)

/* SWITCHERS5 (29) */
#define MC13783_SW2AMODE                (0x3 << 0)
    #define MC13783_SW2AMODE_OFF        (0x0 << 0)
    #define MC13783_SW2AMODE_PWM        (0x1 << 0)
    #define MC13783_SW2AMODE_PWM_SKIP   (0x2 << 0)
    #define MC13783_SW2AMODE_PFM        (0x3 << 0)
#define MC13783_SW2ASTBYMODE            (0x3 << 2)
    #define MC13783_SW2ASTBYMODE_OFF      (0x0 << 2)
    #define MC13783_SW2ASTBYMODE_PWM      (0x1 << 2)
    #define MC13783_SW2ASTBYMODE_PWM_SKIP (0x2 << 2)
    #define MC13783_SW2ASTBYMODE_PFM      (0x3 << 2)
#define MC13783_SW2ADVSSPEED            (0x3 << 6)
    #define MC13783_SW2ADVSSPEED_4US_NO_PWR_RDY (0x0 << 6)
    #define MC13783_SW2ADVSSPEED_4US            (0x1 << 6)
    #define MC13783_SW2ADVSSPEED_8US            (0x2 << 6)
    #define MC13783_SW2ADVSSPEED_16US           (0x3 << 6)
#define MC13783_SW2APANIC               (0x1 << 8)
#define MC13783_SW2ASFST                (0x1 << 9)
#define MC13783_SW2BMODE                (0x3 << 10)
    #define MC13783_SW2BMODE_OFF        (0x0 << 10)
    #define MC13783_SW2BMODE_PWM        (0x1 << 10)
    #define MC13783_SW2BMODE_PWM_SKIP   (0x2 << 10)
    #define MC13783_SW2BMODE_PFM        (0x3 << 10)
#define MC13783_SW2BSTBYMODE            (0x3 << 12)
    #define MC13783_SW2BSTBYMODE_OFF      (0x0 << 12)
    #define MC13783_SW2BSTBYMODE_PWM      (0x1 << 12)
    #define MC13783_SW2BSTBYMODE_PWM_SKIP (0x2 << 12)
    #define MC13783_SW2BSTBYMODE_PFM      (0x3 << 12)
#define MC13783_SW2BDVSSPEED            (0x3 << 14)
    #define MC13783_SW2BDVSSPEED_4US_NO_PWR_RDY (0x0 << 14)
    #define MC13783_SW2BDVSSPEED_4US            (0x1 << 14)
    #define MC13783_SW2BDVSSPEED_8US            (0x2 << 14)
    #define MC13783_SW2BDVSSPEED_16US           (0x3 << 14)
#define MC13783_SW2BPANIC               (0x1 << 16)
#define MC13783_SW2BSFST                (0x1 << 17)
#define MC13783_SW3                     (0x3 << 18)
    #define MC13783_SW3_5_0V            (0x0 << 18)
    /* 0x1...0x2 same as 0x0 */
    #define MC13783_SW3_5_5V            (0x3 << 18)
#define MC13783_SW3EN                   (0x1 << 20)
#define MC13783_SW3STBY                 (0x1 << 21)
#define MC13783_SW3MODE                 (0x1 << 22)

/* REGULATOR_SETTING0 (30) */
#define MC13783_VIOLO                   (0x3 << 2)
    #define MC13783_VIOLO_1_20V         (0x0 << 2)
    #define MC13783_VIOLO_1_30V         (0x1 << 2)
    #define MC13783_VIOLO_1_50V         (0x2 << 2)
    #define MC13783_VIOLO_1_80V         (0x3 << 2)
#define MC13783_VDIG                    (0x3 << 4)
    #define MC13783_VDIG_1_20V          (0x0 << 4)
    #define MC13783_VDIG_1_30V          (0x1 << 4)
    #define MC13783_VDIG_1_50V          (0x2 << 4)
    #define MC13783_VDIG_1_80V          (0x3 << 4)
#define MC13783_VGEN                    (0x7 << 6)
    #define MC13783_VGEN_1_20V          (0x0 << 6)
    #define MC13783_VGEN_1_30V          (0x1 << 6)
    #define MC13783_VGEN_1_50V          (0x2 << 6)
    #define MC13783_VGEN_1_80V          (0x3 << 6)
    #define MC13783_VGEN_1_10V          (0x4 << 6)
    #define MC13783_VGEN_2_00V          (0x5 << 6)
    #define MC13783_VGEN_2_775V         (0x6 << 6)
    #define MC13783_VGEN_2_40V          (0x7 << 6)
#define MC13783_VRFDIG                  (0x3 << 9)
    #define MC13783_VREFDIG_1_20V       (0x0 << 9)
    #define MC13783_VREFDIG_1_30V       (0x1 << 9)
    #define MC13783_VREFDIG_1_80V       (0x2 << 9)
    #define MC13783_VREFDIG_1_875V      (0x3 << 9)
#define MC13783_VRFREF                  (0x3 << 11)
    #define MC13783_VRFREF_2_475V       (0x0 << 11)
    #define MC13783_VRFREF_2_600V       (0x1 << 11)
    #define MC13783_VRFREF_2_700V       (0x2 << 11)
    #define MC13783_VRFREF_2_775V       (0x3 << 11)
#define MC13783_VRFCP                   (0x1 << 13)
#define MC13783_VSIM                    (0x1 << 14)
#define MC13783_VESIM                   (0x1 << 15)
#define MC13783_VCAM                    (0x7 << 16)
    #define MC13783_VCAM_1_50V          (0x0 << 16)
    #define MC13783_VCAM_1_80V          (0x1 << 16)
    #define MC13783_VCAM_2_50V          (0x2 << 16)
    #define MC13783_VCAM_2_55V          (0x3 << 16)
    #define MC13783_VCAM_2_60V          (0x4 << 16)
    #define MC13783_VCAM_2_75V          (0x5 << 16)
    #define MC13783_VCAM_2_80V          (0x6 << 16)
    #define MC13783_VCAM_3_00V          (0x7 << 16)

/* REGULATOR_SETTING1 (31) */
#define MC13783_VVIB                    (0x3 << 0)
    #define MC13783_VVIB_1_30V          (0x0 << 0)
    #define MC13783_VVIB_1_80V          (0x1 << 0)
    #define MC13783_VVIB_2_00V          (0x2 << 0)
    #define MC13783_VVIB_3_00V          (0x3 << 0)
#define MC13783_VRF1                    (0x3 << 2)
    #define MC13783_VRF1_1_500V         (0x0 << 2)
    #define MC13783_VRF1_1_875V         (0x1 << 2)
    #define MC13783_VRF1_2_700V         (0x2 << 2)
    #define MC13783_VRF1_2_775V         (0x3 << 2)
#define MC13783_VRF2                    (0x3 << 4)
    #define MC13783_VRF2_1_500V         (0x0 << 4)
    #define MC13783_VRF2_1_875V         (0x1 << 4)
    #define MC13783_VRF2_2_700V         (0x2 << 4)
    #define MC13783_VRF2_2_775V         (0x3 << 4)
#define MC13783_VMMC1                   (0x7 << 6)
    #define MC13783_VMMC1_1_60V         (0x0 << 6)
    #define MC13783_VMMC1_1_80V         (0x1 << 6)
    #define MC13783_VMMC1_2_00V         (0x2 << 6)
    #define MC13783_VMMC1_2_60V         (0x3 << 6)
    #define MC13783_VMMC1_2_70V         (0x4 << 6)
    #define MC13783_VMMC1_2_80V         (0x5 << 6)
    #define MC13783_VMMC1_2_90V         (0x6 << 6)
    #define MC13783_VMMC1_3_00V         (0x7 << 6)
#define MC13783_VMMC2                   (0x7 << 9)
    #define MC13783_VMMC2_1_60V         (0x0 << 9)
    #define MC13783_VMMC2_1_80V         (0x1 << 9)
    #define MC13783_VMMC2_2_00V         (0x2 << 9)
    #define MC13783_VMMC2_2_60V         (0x3 << 9)
    #define MC13783_VMMC2_2_70V         (0x4 << 9)
    #define MC13783_VMMC2_2_80V         (0x5 << 9)
    #define MC13783_VMMC2_2_90V         (0x6 << 9)
    #define MC13783_VMMC2_3_00V         (0x7 << 9)

/* REGULATOR_MODE0 (32) */
#define MC13783_VAUDIOEN                (0x1 << 0)
#define MC13783_VAUDIOSTBY              (0x1 << 1)
#define MC13783_VAUDIOMODE              (0x1 << 2)
#define MC13783_VIOHIEN                 (0x1 << 3)
#define MC13783_VIOHISTBY               (0x1 << 4)
#define MC13783_VIOHIMODE               (0x1 << 5)
#define MC13783_VIOLOEN                 (0x1 << 6)
#define MC13783_VIOLOSTBY               (0x1 << 7)
#define MC13783_VIOLOMODE               (0x1 << 8)
#define MC13783_VDIGEN                  (0x1 << 9)
#define MC13783_VDIGSTBY                (0x1 << 10)
#define MC13783_VDIGMODE                (0x1 << 11)
#define MC13783_VGENEN                  (0x1 << 12)
#define MC13783_VGENSTBY                (0x1 << 13)
#define MC13783_VGENMODE                (0x1 << 14)
#define MC13783_VRFDIGEN                (0x1 << 15)
#define MC13783_VRFDIGSTBY              (0x1 << 16)
#define MC13783_VRFDIGMODE              (0x1 << 17)
#define MC13783_VRFREFEN                (0x1 << 18)
#define MC13783_VRFREFSTBY              (0x1 << 19)
#define MC13783_VRFREFMODE              (0x1 << 20)
#define MC13783_VRFCPEN                 (0x1 << 21)
#define MC13783_VRFCPSTBY               (0x1 << 22)
#define MC13783_VRFCPMODE               (0x1 << 23)

/* REGULATOR_MODE1 (33) */
#define MC13783_VSIMEN                  (0x1 << 0)
#define MC13783_VSIMSTBY                (0x1 << 1)
#define MC13783_VSIMMODE                (0x1 << 2)
#define MC13783_VESIMEN                 (0x1 << 3)
#define MC13783_VESIMSTBY               (0x1 << 4)
#define MC13783_VESIMMODE               (0x1 << 5)
#define MC13783_VCAMEN                  (0x1 << 6)
#define MC13783_VCAMSTBY                (0x1 << 7)
#define MC13783_VCAMMODE                (0x1 << 8)
#define MC13783_VRFBGEN                 (0x1 << 9)
#define MC13783_VRFBGSTBY               (0x1 << 10)
#define MC13783_VVIBEN                  (0x1 << 11)
#define MC13783_VRF1EN                  (0x1 << 12)
#define MC13783_VRF1STBY                (0x1 << 13)
#define MC13783_VRF1MODE                (0x1 << 14)
#define MC13783_VRF2EN                  (0x1 << 15)
#define MC13783_VRF2STBY                (0x1 << 16)
#define MC13783_VRF2MODE                (0x1 << 17)
#define MC13783_VMMC1EN                 (0x1 << 18)
#define MC13783_VMMC1STBY               (0x1 << 19)
#define MC13783_VMMC1MODE               (0x1 << 20)
#define MC13783_VMMC2EN                 (0x1 << 21)
#define MC13783_VMMC2STBY               (0x1 << 22)
#define MC13783_VMMC2MODE               (0x1 << 23)

/* POWER_MISCELLANEOUS (34) */
#define MC13783_GPO1EN                  (0x1 << 6)
#define MC13783_GPO1STBY                (0x1 << 7)
#define MC13783_GPO2EN                  (0x1 << 8)
#define MC13783_GPO2STBY                (0x1 << 9)
#define MC13783_GPO3EN                  (0x1 << 10)
#define MC13783_GPO3STBY                (0x1 << 11)
#define MC13783_GPO4EN                  (0x1 << 12)
#define MC13783_GPO4STBY                (0x1 << 13)
#define MC13783_VIBPINCTRL              (0x1 << 14)
#define MC13783_PWGT1SPIEN              (0x1 << 15)
#define MC13783_PWGT2SPIEN              (0x1 << 16)

/* AUDIO_RX0 (36) */
#define MC13783_VAUDIOON                (0x1 << 0)
#define MC13783_BIASEN                  (0x1 << 1)
#define MC13783_BIASSPEED               (0x1 << 2)
#define MC13783_ASPEN                   (0x1 << 3)
#define MC13783_ASPSEL                  (0x1 << 4)
#define MC13783_ALSPEN                  (0x1 << 5)
#define MC13783_ALSPREF                 (0x1 << 6)
#define MC13783_ALSPSEL                 (0x1 << 7)
#define MC13783_LSPLEN                  (0x1 << 8)
#define MC13783_AHSREN                  (0x1 << 9)
#define MC13783_AHSLEN                  (0x1 << 10)
#define MC13783_AHSSEL                  (0x1 << 11)
#define MC13783_HSPGDIS                 (0x1 << 12)
#define MC13783_HSDETEN                 (0x1 << 13)
#define MC13783_HSDETAUTOB              (0x1 << 14)
#define MC13783_ARXOUTREN               (0x1 << 15)
#define MC13783_ARXOUTLEN               (0x1 << 16)
#define MC13783_ARXOUTSEL               (0x1 << 17)
#define MC13783_CDCOUTEN                (0x1 << 18)
#define MC13783_HSLDETEN                (0x1 << 19)
#define MC13783_ADDCDC                  (0x1 << 21)
#define MC13783_ADDSTDC                 (0x1 << 22)
#define MC13783_ADDRXIN                 (0x1 << 23)

/* AUDIO_RX1 (37) */
#define MC13783_PGARXEN                 (0x1 << 0)
    /* <=0010=-33dB...1101=0dB...1111=+6dB in 3dB steps */
#define MC13783_PGARX                   (0xf << 1)
#define MC13783_PGARX_POS               (1)
#define MC13783_PGASTEN                 (0x1 << 5)
    /* <=0010=-33dB...1101=0dB...1111=+6dB in 3dB steps */
#define MC13783_PGAST                   (0xf << 6)
#define MC13783_PGAST_POS               (6)
#define MC13783_ARXINEN                 (0x1 << 10)
#define MC13783_ARXIN                   (0x1 << 11)
    /* <=0010=-33dB...1101=0dB...1111=+6dB in 3dB steps */
#define MC13783_PGARXIN                 (0xf << 12)
#define MC13783_PGARXIN_POS             (12)
#define MC13783_MONO                    (0x3 << 16)
    #define MC13783_MONO_LR_INDEPENDENT (0x0 << 16)
    #define MC13783_MONO_ST_OPPOSITE    (0x1 << 16)
    #define MC13783_MONO_ST_TO_MONO     (0x2 << 16)
    #define MC13783_MONO_MONO_OPPOSITE  (0x3 << 16)
    /* 000=-21dB...3dB steps...111=0dB: left or right */
#define MC13783_BAL                     (0x7 << 18)
#define MC13783_BAL_POS                 (18)
#define MC13783_BALLR                   (0x1 << 21)

/* AUDIO_TX (38) */
#define MC13783_MC1BEN                  (0x1 << 0)
#define MC13783_MC2BEN                  (0x1 << 1)
#define MC13783_MC2BDETDBNC             (0x1 << 2)
#define MC13783_MC2BDETEN               (0x1 << 3)
#define MC13783_AMC1REN                 (0x1 << 5)
#define MC13783_AMC1RITOV               (0x1 << 6)
#define MC13783_AMC1LEN                 (0x1 << 7)
#define MC13783_AMC1LITOV               (0x1 << 8)
#define MC13783_AMC2EN                  (0x1 << 9)
#define MC13783_AMC2ITOV                (0x1 << 10)
#define MC13783_ATXINEN                 (0x1 << 11)
#define MC13783_ATXOUTEN                (0x1 << 12)
#define MC13783_RXINREC                 (0x1 << 13)
    /* 00000=-8dB...01000=0dB...11111=+23dB */
#define MC13783_PGATXR                  (0x1f << 14)
#define MC13783_PGATXR_POS              (14)
    /* 00000=-8dB...01000=0dB...11111=+23dB */
#define MC13783_PGATXL                  (0x1f << 19)
#define MC13783_PGATXL_POS              (19)

/* SSI_NETWORK (39) */
#define MC13783_CDCTXRXSLOT             (0x3 << 2)
    #define MC13783_CDCTXRXSLOT_TS0     (0x0 << 2)
    #define MC13783_CDCTXRXSLOT_TS1     (0x1 << 2)
    #define MC13783_CDCTXRXSLOT_TS2     (0x2 << 2)
    #define MC13783_CDCTXRXSLOT_TS3     (0x3 << 2)
#define MC13783_CDCTXSECSLOT            (0x3 << 4)
    #define MC13783_CDCTXSECSLOT_TS0    (0x0 << 4)
    #define MC13783_CDCTXSECSLOT_TS1    (0x1 << 4)
    #define MC13783_CDCTXSECSLOT_TS2    (0x2 << 4)
    #define MC13783_CDCTXSECSLOT_TS3    (0x3 << 4)
#define MC13783_CDCRXSECSLOT            (0x3 << 6)
    #define MC13783_CDCRXSECSLOT_TS0    (0x0 << 6)
    #define MC13783_CDCRXSECSLOT_TS1    (0x1 << 6)
    #define MC13783_CDCRXSECSLOT_TS2    (0x2 << 6)
    #define MC13783_CDCRXSECSLOT_TS3    (0x3 << 6)
    /* -inf, -0dB, -6dB, -12dB */
#define MC13783_CDCRXSECGAIN            (0x3 << 8)
#define MC13783_CDCRXSECGAIN_POS        (8)
#define MC13783_CDCSUMGAIN              (0x1 << 10)
#define MC13783_CDCFSDLY                (0x1 << 11)
#define MC13783_STDCSLOTS               (0x3 << 12)
    #define MC13783_STDCSLOTS_8         (0x0 << 12)
    #define MC13783_STDCSLOTS_LR6       (0x1 << 12)
    #define MC13783_STDCSLOTS_LR2       (0x2 << 12)
    #define MC13783_STDCSLOTS_LR        (0x3 << 12)
#define MC13783_STDCRXSLOT              (0x3 << 14)
    #define MC13783_STDCRXSLOT_TS0_TS1  (0x0 << 14)
    #define MC13783_STDCRXSLOT_TS2_TS3  (0x1 << 14)
    #define MC13783_STDCRXSLOT_TS4_TS5  (0x2 << 14)
    #define MC13783_STDCRXSLOT_TS6_TS7  (0x3 << 14)
#define MC13783_STDCRXSECSLOT           (0x3 << 16)
    #define MC13783_STDCRXSECSLOT_TS0_TS1 (0x0 << 16)
    #define MC13783_STDCRXSECSLOT_TS2_TS3 (0x1 << 16)
    #define MC13783_STDCRXSECSLOT_TS4_TS5 (0x2 << 16)
    #define MC13783_STDCRXSECSLOT_TS6_TS7 (0x3 << 16)
    /* -inf, -0dB, -6dB, -12dB */
#define MC13783_STDCRXSECGAIN           (0x3 << 18)
#define MC13783_STDCRXSECGAIN_POS       (18)
#define MC13783_STDSUMGAIN              (0x1 << 20)

/* AUDIO_CODEC (40) */
#define MC13783_CDCSSISEL               (0x1 << 0)
#define MC13783_CDCCLKSEL               (0x1 << 1)
#define MC13783_CDCSM                   (0x1 << 2)
#define MC13783_CDCBCLINV               (0x1 << 3)
#define MC13783_CDCFSINV                (0x1 << 4)
#define MC13783_CDCFS                   (0x3 << 5)
    #define MC13783_CDCFS_NET           (0x1 << 5)
    #define MC13783_CDCFS_I2S           (0x2 << 5)
#define MC13783_CDCCLK                  (0x7 << 7)
#define MC13783_CDCFS8K16K              (0x1 << 10)
#define MC13783_CDCEN                   (0x1 << 11)
#define MC13783_CDCCLKEN                (0x1 << 12)
#define MC13783_CDCTS                   (0x1 << 13)
#define MC13783_CDCDITH                 (0x1 << 14)
#define MC13783_CDCRESET                (0x1 << 15)
#define MC13783_CDCBYP                  (0x1 << 16)
#define MC13783_CDCALM                  (0x1 << 17)
#define MC13783_CDCDLM                  (0x1 << 18)
#define MC13783_AUDIHPF                 (0x1 << 19)
#define MC13783_AUDOHPF                 (0x1 << 20)

/* AUDIO_STEREO_DAC (41) */
#define MC13783_STDCSSISEL              (0x1 << 0)
#define MC13783_STDCCLKSEL              (0x1 << 1)
#define MC13783_STDCSM                  (0x1 << 2)
#define MC13783_STDCBCLINV              (0x1 << 3)
#define MC13783_STDCFSINV               (0x1 << 4)
#define MC13783_STDCFS                  (0x3 << 5)
    #define MC13783_STDCFS_NORMAL       (0x0 << 5)
    #define MC13783_STDCFS_NET          (0x1 << 5)
    #define MC13783_STDCFS_I2S          (0x2 << 5)
#define MC13783_STDCCLK                 (0x7 << 7)
    /* Master */
    #define MC13783_STDCCLK_13_0MHZ     (0x0 << 7)
    #define MC13783_STDCCLK_15_36MHZ    (0x1 << 7)
    #define MC13783_STDCCLK_16_8MHZ     (0x2 << 7)
    #define MC13783_STDCCLK_26_0MHZ     (0x4 << 7)
    #define MC13783_STDCCLK_12_0MHZ     (0x5 << 7)
    #define MC13783_STDCCLK_3_6864MHZ   (0x6 << 7)
    #define MC13783_STDCCLK_33_6MHZ     (0x7 << 7)
    /* Slave */
    #define MC13783_STDCCLK_CLIMCL      (0x5 << 7)
    #define MC13783_STDCCLK_FS          (0x6 << 7)
    #define MC13783_STDCCLK_BCL         (0x7 << 7)
#define MC13783_STDCFSDLYB              (0x1 << 10)
#define MC13783_STDCEN                  (0x1 << 11)
#define MC13783_STDCCLKEN               (0x1 << 12)
#define MC13783_STDCRESET               (0x1 << 15)
#define MC13783_SPDIF                   (0x1 << 16)
#define MC13783_SR                      (0xf << 17)
    #define MC13783_SR_8000             (0x0 << 17)
    #define MC13783_SR_11025            (0x1 << 17)
    #define MC13783_SR_12000            (0x2 << 17)
    #define MC13783_SR_16000            (0x3 << 17)
    #define MC13783_SR_22050            (0x4 << 17)
    #define MC13783_SR_24000            (0x5 << 17)
    #define MC13783_SR_32000            (0x6 << 17)
    #define MC13783_SR_44100            (0x7 << 17)
    #define MC13783_SR_48000            (0x8 << 17)
    #define MC13783_SR_64000            (0x9 << 17)
    #define MC13783_SR_96000            (0xa << 17)

/* ADC0 (43) */
#define MC13783_LICELLCON               (0x1 << 0)
#define MC13783_CHRGICON                (0x1 << 1)
#define MC13783_BATICON                 (0x1 << 2)
#define MC13783_RTHEN                   (0x1 << 3)
#define MC13783_DTHEN                   (0x1 << 4)
#define MC13783_UIDEN                   (0x1 << 5)
#define MC13783_ADOUTEN                 (0x1 << 6)
#define MC13783_ADOUTPER                (0x1 << 7)
#define MC13783_ADREFEN                 (0x1 << 10)
#define MC13783_ADREFMODE               (0x1 << 11)
#define MC13783_TSMOD                   (0x7 << 12)
    #define MC13783_TSMOD_INACTIVE      (0x0 << 12)
    #define MC13783_TSMOD_INTERRUPT     (0x1 << 12)
    #define MC13783_TSMOD_RESISTIVE     (0x2 << 12)
    #define MC13783_TSMOD_POSITION      (0x3 << 12)
    /* 0x4 - 0x7 = Inactive (same as 0x0) */
#define MC13783_CHRGRAWDIV              (0x1 << 15)
#define MC13783_ADINC1                  (0x1 << 16)
#define MC13783_ADINC2                  (0x1 << 17)
#define MC13783_WCOMP                   (0x1 << 18)
#define MC13783_ADCBIS0_ACCESS          (0x1 << 23)

/* ADC1 (44) */
#define MC13783_ADEN                    (0x1 << 0)
#define MC13783_RAND                    (0x1 << 1)
#define MC13783_ADSEL                   (0x1 << 3)
#define MC13783_TRIGMASK                (0x1 << 4)
#define MC13783_ADA1                    (0x7 << 5)
#define MC13783_ADA1_POS                (5)
#define MC13783_ADA2                    (0x7 << 8)
#define MC13783_ADA2_POS                (8)
#define MC13783_ATO                     (0xff << 11)
#define MC13783_ATO_POS                 (11)
#define MC13783_ATOX                    (0x1 << 19)
#define MC13783_ASC                     (0x1 << 20)
#define MC13783_ADTRIGIGN               (0x1 << 21)
#define MC13783_ADONESHOT               (0x1 << 22)
#define MC13783_ADCBIS1_ACCESS          (0x1 << 23)

/* ADC2 (45) */
#define MC13783_ADD1                    (0x3ff << 2)
#define MC13783_ADD1_POS                (2)
#define MC13783_ADD2                    (0x3ff << 14)
#define MC13783_ADD2_POS                (14)

/* ADC3 (46) */
#define MC13783_WHIGH                   (0x3f << 0)
#define MC13783_WHIGH_POS               (0)
#define MC13783_ICID                    (0x7 << 6)
#define MC13783_ICID_POS                (6)
#define MC13783_WLOW                    (0x3f << 9)
#define MC13783_WLOW_POS                (9)
#define MC13783_ADCBIS2_ACCESS          (0x1 << 23)

/* ADC4 (47) */
#define MC13783_ADCBIS1                 (0x3ff << 2)
#define MC13783_ADCBIS1_POS             (2)
#define MC13783_ADCBIS2                 (0x3ff << 14)
#define MC13783_ADCBIS2_POS             (14)

/* CHARGER (48) */
#define MC13783_VCHRG                   (0x7 << 0)
    #define MC13783_VCHRG_4_050V        (0x0 << 0)
    #define MC13783_VCHRG_4_375V        (0x1 << 0)
    #define MC13783_VCHRG_4_150V        (0x2 << 0)
    #define MC13783_VCHRG_4_200V        (0x3 << 0)
    #define MC13783_VCHRG_4_250V        (0x4 << 0)
    #define MC13783_VCHRG_4_300V        (0x5 << 0)
    #define MC13783_VCHRG_3_800V        (0x6 << 0)
    #define MC13783_VCHRG_4_500V        (0x7 << 0)
#define MC13783_ICHRG                   (0xf << 3) /*  Min  Nom  Max */
    #define MC13783_ICHRG_POS           (3)
    #define MC13783_ICHRG_0MA           (0x0 << 3) /*    0    0    0 */
    #define MC13783_ICHRG_70MA          (0x1 << 3) /*   55   70   85 */
    #define MC13783_ICHRG_177MA         (0x2 << 3) /*  161  177  195 */
    #define MC13783_ICHRG_266MA         (0x3 << 3) /*  242  266  293 */
    #define MC13783_ICHRG_355MA         (0x4 << 3) /*  322  355  390 */
    #define MC13783_ICHRG_443MA         (0x5 << 3) /*  403  443  488 */
    #define MC13783_ICHRG_532MA         (0x6 << 3) /*  484  532  585 */
    #define MC13783_ICHRG_621MA         (0x7 << 3) /*  564  621  683 */
    #define MC13783_ICHRG_709MA         (0x8 << 3) /*  645  709  780 */
    #define MC13783_ICHRG_798MA         (0x9 << 3) /*  725  798  878 */
    #define MC13783_ICHRG_886MA         (0xa << 3) /*  806  886  975 */
    #define MC13783_ICHRG_975MA         (0xb << 3) /*  886  975 1073 */
    #define MC13783_ICHRG_1064MA        (0xc << 3) /*  967 1064 1170 */
    #define MC13783_ICHRG_1152MA        (0xd << 3) /* 1048 1152 1268 */
    #define MC13783_ICHRG_1596MA        (0xe << 3) /* 1450 1596 1755 */
    #define MC13783_ICHRG_FULLY_ON      (0xf << 3) /* Disallow HW FET turn on */
#define MC13783_ICHRGTR                 (0x7 << 7) /*  Min  Nom  Max */
    #define MC13783_ICHRGTR_POS         (7)
    #define MC13783_ICHRGTR_0MA         (0x0 << 7) /*    0    0    0 */
    #define MC13783_ICHRGTR_9MA         (0x1 << 7) /*    6    9   12 */
    #define MC13783_ICHRGTR_20MA        (0x2 << 7) /*   14   20   26 */
    #define MC13783_ICHRGTR_36MA        (0x3 << 7) /*   25   36   47 */
    #define MC13783_ICHRGTR_42MA        (0x4 << 7) /*   29   42   55 */
    #define MC13783_ICHRGTR_50MA        (0x5 << 7) /*   35   50   65 */
    #define MC13783_ICHRGTR_59MA        (0x6 << 7) /*   41   59   77 */
    #define MC13783_ICHRGTR_68MA        (0x7 << 7) /*   50   68   86 */
#define MC13783_FETOVRD                 (0x1 << 10)
#define MC13783_FETCTRL                 (0x1 << 11)
#define MC13783_RVRSMODE                (0x1 << 13)
#define MC13783_OVCTRL                  (0x3 << 15)
    #define MC13783_OVCTRL_5_83V        (0x0 << 15) /* Not for separate! */
    #define MC13783_OVCTRL_6_90V        (0x1 << 15)
    #define MC13783_OVCTRL_9_80V        (0x2 << 15)
    #define MC13783_OVCTRL_19_6V        (0x3 << 15)
#define MC13783_UCHEN                   (0x1 << 17)
#define MC13783_CHRGLEDEN               (0x1 << 18)
#define MC13783_CHRGRAWPDEN             (0x1 << 19)

/* USB0 (49) */
#define MC13783_FSENB                   (0x1 << 0)
#define MC13783_USBSUSPEND              (0x1 << 1)
#define MC13783_USBPU                   (0x1 << 2)
#define MC13783_UDPPD                   (0x1 << 3)
#define MC13783_UDMPD                   (0x1 << 4)
#define MC13783_DP150KPU                (0x1 << 5)
#define MC13783_VBUS70KPDENB            (0x1 << 6)
#define MC13783_VBUSPULSETMR            (0x7 << 7)
    #define MC13783_VBUSPULSETMR_NA     (0x0 << 7)
    #define MC13783_VBUSPULSETMR_10MS   (0x1 << 7)
    #define MC13783_VBUSPULSETMR_20MS   (0x2 << 7)
    #define MC13783_VBUSPULSETMR_30MS   (0x3 << 7)
    #define MC13783_VBUSPULSETMR_40MS   (0x4 << 7)
    #define MC13783_VBUSPULSETMR_50MS   (0x5 << 7)
    #define MC13783_VBUSPULSETMR_60MS   (0x6 << 7)
    #define MC13783_VBUSPULSETMR_INF    (0x7 << 7)
#define MC13783_DLPSRP                  (0x1 << 10)
#define MC13783_SE0CONN                 (0x1 << 11)
#define MC13783_USBXCVREN               (0x1 << 12)
#define MC13783_CONMODE                 (0x7 << 14)
    #define MC13783_CONMODE_USB         (0x0 << 14)
    #define MC13783_CONMODE_RS232       (0x1 << 14) /* and 0x2 */
    #define MC13783_CONMODE_CEA_936_A   (0x4 << 14) /* and 0x5...0x7 */
#define MC13783_DATSE0                  (0x1 << 17)
#define MC13783_BIDIR                   (0x1 << 18)
#define MC13783_USBCNTRL                (0x1 << 19)
#define MC13783_IDPD                    (0x1 << 20)
#define MC13783_IDPULSE                 (0x1 << 21)
#define MC13783_IDPUCNTRL               (0x1 << 22)
#define MC13783_DMPULSE                 (0x1 << 23)

/* CHARGER_USB1 (50) */
#define MC13783_USBIN                   (0x3 << 0)
    #define MC13783_USBIN_BOOST_VINBUS  (0x0 << 0)
    #define MC13783_USBIN_VBUS          (0x1 << 0) /* and 0x3 */
    #define MC13783_USBIN_BP            (0x2 << 0) /* VINVIB */
#define MC13783_VUSB                    (0x1 << 2) /* 0=3.2V, 1=3.3V */
#define MC13783_VUSBEN                  (0x1 << 3)
#define MC13783_VBUSEN                  (0x1 << 5)
#define MC13783_RSPOL                   (0x1 << 6)
#define MC13783_RSTRI                   (0x1 << 7)
#define MC13783_ID100KPU                (0x1 << 8)

/* LED_CONTROL0 (51) */
#define MC13783_LEDEN                   (0x1 << 0)
#define MC13783_LEDMDRAMPUP             (0x1 << 1)
#define MC13783_LEDADRAMPUP             (0x1 << 2)
#define MC13783_LEDKDRAMPUP             (0x1 << 3)
#define MC13783_LEDMDRAMPDOWN           (0x1 << 4)
#define MC13783_LEDADRAMPDOWN           (0x1 << 5)
#define MC13783_LEDKDRAMPDOWN           (0x1 << 6)
#define MC13783_TRIODEMD                (0x1 << 7)
#define MC13783_TRIODEAD                (0x1 << 8)
#define MC13783_TRIODEKD                (0x1 << 9)
#define MC13783_BOOSTEN                 (0x1 << 10)
#define MC13783_ABMODE                  (0x7 << 11)
    #define MC13783_ABMODE_ADAPTIVE_BOOST_DISABLED  (0x0 << 11)
    #define MC13783_ABMODE_MONCH_LEDMD1             (0x1 << 11)
    #define MC13783_ABMODE_MONCH_LEDMD12            (0x2 << 11)
    #define MC13783_ABMODE_MONCH_LEDMD123           (0x3 << 11)
    #define MC13783_ABMODE_MONCH_LEDMD1234          (0x4 << 11)
    #define MC13783_ABMODE_MONCH_LEDMD1234_LEADAD1  (0x5 << 11)
    #define MC13783_ABMODE_MONCH_LEDMD1234_LEADAD12 (0x6 << 11)
    #define MC13783_ABMODE_MONCH_LEDMD1_LEDAD_ACT   (0x7 << 11)
#define MC13783_ABREF                   (0x3 << 14)
    #define MC13783_ABREF_200MV         (0x0 << 14)
    #define MC13783_ABREF_400MV         (0x1 << 14)
    #define MC13783_ABREF_600MV         (0x2 << 14)
    #define MC13783_ABREF_800MV         (0x3 << 14)
#define MC13783_FLPATTRN                (0xf << 17)
    #define MC13783_FLPATTRN_POS        (17)
#define MC13783_FLBANK1                 (0x1 << 21)
#define MC13783_FLBANK2                 (0x1 << 22)
#define MC13783_FLBANK3                 (0x1 << 23)

/* LED_CONTROL1 (52) */
#define MC13783_LEDR1RAMPUP             (0x1 << 0)
#define MC13783_LEDG1RAMPUP             (0x1 << 1)
#define MC13783_LEDB1RAMPUP             (0x1 << 2)
#define MC13783_LEDR1RAMPDOWN           (0x1 << 3)
#define MC13783_LEDG1RAMPDOWN           (0x1 << 4)
#define MC13783_LEDB1RAMPDOWN           (0x1 << 5)
#define MC13783_LEDR2RAMPUP             (0x1 << 6)
#define MC13783_LEDG2RAMPUP             (0x1 << 7)
#define MC13783_LEDB2RAMPUP             (0x1 << 8)
#define MC13783_LEDR2RAMPDOWN           (0x1 << 9)
#define MC13783_LEDG2RAMPDOWN           (0x1 << 10)
#define MC13783_LEDB2RAMPDOWN           (0x1 << 11)
#define MC13783_LEDR3RAMPUP             (0x1 << 12)
#define MC13783_LEDG3RAMPUP             (0x1 << 13)
#define MC13783_LEDB3RAMPUP             (0x1 << 14)
#define MC13783_LEDR3RAMPDOWN           (0x1 << 15)
#define MC13783_LEDG3RAMPDOWN           (0x1 << 16)
#define MC13783_LEDB3RAMPDOWN           (0x1 << 17)
#define MC13783_TC1HALF                 (0x1 << 18)
#define MC13783_SLEWLIMTC               (0x1 << 23)

/* LED_CONTROL2 (53) */
#define MC13783_LEDMD                   (0x7 << 0)
#define MC13783_LEDMD_POS               (0)
#define MC13783_LEDAD                   (0x7 << 3)
#define MC13783_LEDAD_POS               (3)
#define MC13783_LEDKP                   (0x7 << 6)
#define MC13783_LEDKP_POS               (6)
#define MC13783_LEDMDDC                 (0xf << 9)
#define MC13783_LEDMDDC_POS             (9)
#define MC13783_LEDADDC                 (0xf << 13)
#define MC13783_LEDADDC_POS             (13)
#define MC13783_LEDKPDC                 (0xf << 17)
#define MC13783_LEDKPDC_POS             (17)
#define MC13783_BLPERIOD                (0x1 << 21)
#define MC13783_BLPERIOD_POS            (21)
#define MC13783_SLEWLIMBL               (0x1 << 23)

/* LED_CONTROL3 (54) */
#define MC13783_LEDR1                   (0x3 << 0)
#define MC13783_LEDR1_POS               (0)
#define MC13783_LEDG1                   (0x3 << 2)
#define MC13783_LEDG1_POS               (2)
#define MC13783_LEDB1                   (0x3 << 4)
#define MC13783_LEDB1_POS               (4)
#define MC13783_LEDR1DC                 (0x1f << 6)
#define MC13783_LEDR1DC_POS             (6)
#define MC13783_LEDG1DC                 (0x1f << 11)
#define MC13783_LEDG1DC_POS             (11)
#define MC13783_LEDB1DC                 (0x1f << 16)
#define MC13783_LEDB1DC_POS             (16)
#define MC13783_TC1PERIOD               (0x3 << 21)
#define MC13783_TC1PERIOD_POS           (21)
#define MC13783_TC1TRIODE               (0x1 << 23)

/* LED_CONTROL4 (55) */
#define MC13783_LEDR2                   (0x3 << 0)
#define MC13783_LEDR2_POS               (0)
#define MC13783_LEDG2                   (0x3 << 2)
#define MC13783_LEDG2_POS               (2)
#define MC13783_LEDB2                   (0x3 << 4)
#define MC13783_LEDB2_POS               (4)
#define MC13783_LEDR2DC                 (0x1f << 6)
#define MC13783_LEDR2DC_POS             (6)
#define MC13783_LEDG2DC                 (0x1f << 11)
#define MC13783_LEDG2DC_POS             (11)
#define MC13783_LEDB2DC                 (0x1f << 16)
#define MC13783_LEDB2DC_POS             (16)
#define MC13783_TC2PERIOD               (0x3 << 21)
#define MC13783_TC2PERIOD_POS           (21)
#define MC13783_TC2TRIODE               (0x1 << 23)

/* LED_CONTROL5 (56) */
#define MC13783_LEDR3                   (0x3 << 0)
#define MC13783_LEDR3_POS               (0)
#define MC13783_LEDG3                   (0x3 << 2)
#define MC13783_LEDG3_POS               (2)
#define MC13783_LEDB3                   (0x3 << 4)
#define MC13783_LEDB3_POS               (4)
#define MC13783_LEDR3DC                 (0x1f << 6)
#define MC13783_LEDR3DC_POS             (6)
#define MC13783_LEDG3DC                 (0x1f << 11)
#define MC13783_LEDG3DC_POS             (11)
#define MC13783_LEDB3DC                 (0x1f << 16)
#define MC13783_LEDB3DC_POS             (16)
#define MC13783_TC3PERIOD               (0x3 << 21)
#define MC13783_TC3PERIOD_POS           (21)
#define MC13783_TC3TRIODE               (0x1 << 23)

void mc13783_init(void);
void mc13783_close(void);
uint32_t mc13783_set(unsigned address, uint32_t bits);
uint32_t mc13783_clear(unsigned address, uint32_t bits);
uint32_t mc13783_read(unsigned address);
int mc13783_write(unsigned address, uint32_t data);
uint32_t mc13783_write_masked(unsigned address, uint32_t data, uint32_t mask);
/* buffer must be available as packet workspace */
int mc13783_read_regs(const unsigned char *regs, uint32_t *buffer, int count);
/* buffer must be available as packet workspace */
int mc13783_write_regs(const unsigned char *regs, uint32_t *buffer, int count);
/* buffer must be available as packet workspace */
bool mc13783_read_async(struct spi_transfer_desc *xfer,
                        const unsigned char *regs, uint32_t *buffer,
                        int count, spi_transfer_cb_fn_type callback);
/* buffer must be available as packet workspace */
bool mc13783_write_async(struct spi_transfer_desc *xfer,
                         const unsigned char *regs, uint32_t *buffer,
                         int count, spi_transfer_cb_fn_type callback);

#define MC13783_DATA_ERROR UINT32_MAX

/* Statically-registered interrupt enable/disable */
enum mc13783_int_ids
{
    /* *STATUS0/MASK0/SENSE0 */
    MC13783_INT_ID_ADCDONE    =  0,
    MC13783_INT_ID_ADCBISDONE =  1,
    MC13783_INT_ID_TS         =  2,
    MC13783_INT_ID_WHIGH      =  3,
    MC13783_INT_ID_WLOW       =  4,
    MC13783_INT_ID_CHGDET     =  6,
    MC13783_INT_ID_CHGOV      =  7,
    MC13783_INT_ID_CHGREV     =  8,
    MC13783_INT_ID_CHGSHORT   =  9,
    MC13783_INT_ID_CCCV       = 10,
    MC13783_INT_ID_CHGCURR    = 11,
    MC13783_INT_ID_BPON       = 12,
    MC13783_INT_ID_LOBATL     = 13,
    MC13783_INT_ID_LOBATH     = 14,
    MC13783_INT_ID_UDP        = 15,
    MC13783_INT_ID_USB        = 16,
    MC13783_INT_ID_IDFLOAT    = 19,
    MC13783_INT_ID_SE1        = 21,
    MC13783_INT_ID_CKDET      = 22,
    MC13783_INT_ID_UDM        = 23,
     /* *STATUS1/MASK1/SENSE1 */
    MC13783_INT_ID_1HZ        =  0 + 32,
    MC13783_INT_ID_TODA       =  1 + 32,
    MC13783_INT_ID_ONOFD1     =  3 + 32,  /* ON1B */
    MC13783_INT_ID_ONOFD2     =  4 + 32,  /* ON2B */
    MC13783_INT_ID_ONOFD3     =  5 + 32,  /* ON3B */
    MC13783_INT_ID_SYSRST     =  6 + 32,
    MC13783_INT_ID_RTCRST     =  7 + 32,
    MC13783_INT_ID_PCI        =  8 + 32,
    MC13783_INT_ID_WARM       =  9 + 32,
    MC13783_INT_ID_MEMHLD     = 10 + 32,
    MC13783_INT_ID_PWRRDY     = 11 + 32,
    MC13783_INT_ID_THWARNL    = 12 + 32,
    MC13783_INT_ID_THWARNH    = 13 + 32,
    MC13783_INT_ID_CLK        = 14 + 32,
    MC13783_INT_ID_SEMAF      = 15 + 32,
    MC13783_INT_ID_MC2B       = 17 + 32,
    MC13783_INT_ID_HSDET      = 18 + 32,
    MC13783_INT_ID_HSL        = 19 + 32,
    MC13783_INT_ID_ALSPTH     = 20 + 32,
    MC13783_INT_ID_AHSSHORT   = 21 + 32,
};

#ifdef DEFINE_MC13783_VECTOR_TABLE

struct mc13783_event
{
    uint32_t id    : 8;     /* MC13783_INT_ID_x */
    uint32_t sense : 24;    /* MC13783_xS */
    void (*callback)(void); /* MC13783_EVENT_CB_x */
};

/* Declares vector table. Order-implied priority */
#define MC13783_EVENT_VECTOR_TBL_START() \
    static FORCE_INLINE uintptr_t __mc13783_event_vector_tbl(int __what) \
    {                                                                    \
        static const struct mc13783_event __tbl[] = {

#define MC13783_EVENT_VECTOR_CB(__name)   void MC13783_EVENT_CB_##__name(void)

#define MC13783_EVENT_VECTOR(__name, __sense) \
            { .id       = MC13783_INT_ID_##__name,                 \
              .sense    = (__sense),                               \
              .callback = (MC13783_EVENT_CB_##__name) },

#define MC13783_EVENT_VECTOR_TBL_END() \
        };                                              \
        switch (__what)                                 \
        {                                               \
            default: return (uintptr_t)__tbl;           \
            case  1: return (uintptr_t)ARRAYLEN(__tbl); \
        }                                               \
    }

#define mc13783_event_vector_tbl \
     ((const struct mc13783_event *)__mc13783_event_vector_tbl(0))

#define mc13783_event_vector_tbl_len \
     ((unsigned int)__mc13783_event_vector_tbl(1))

#endif /* DEFINE_MC13783_VECTOR_TABLE */

void mc13783_enable_event(enum mc13783_int_ids id, bool enable);

/* Read the sense bit(s) if configured - valid only within the respective
   event handler */
uint32_t mc13783_event_sense(void);

#endif /* _MC13783_H_ */
