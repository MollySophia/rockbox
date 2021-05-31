/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by Michael Sevakis
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

#include "config.h"
#include "system.h"
#include "kernel.h"
#include "string.h"
#include "button.h"
#include "lcd.h"
#include "font.h"
#include "mc13783.h"
#include "adc.h"
#include "ccm-imx31.h"
#include "dvfs_dptc-imx31.h"
#include <stdio.h>

#define DEBUG_CANCEL  BUTTON_BACK

bool dbg_hw_info(void)
{
    int line;
    unsigned int pllref;
    unsigned int mcu_pllfreq, ser_pllfreq, usb_pllfreq;
    uint32_t mpctl, spctl, upctl;
    unsigned int freq;
    uint32_t regval;

    extern volatile unsigned int dvfs_nr_dn, dvfs_nr_up,
                                 dvfs_nr_pnc, dvfs_nr_no;
    extern volatile unsigned int dptc_nr_dn, dptc_nr_up,
                                 dptc_nr_pnc, dptc_nr_no;

    lcd_clear_display();
    lcd_setfont(FONT_SYSFIXED);

    while (1)
    {
        line = 0;
        lcd_putsf(0, line++, "Sys Rev Code: 0x%02X", iim_system_rev());
        line++;

        mpctl = CCM_MPCTL;
        spctl = CCM_SPCTL;
        upctl = CCM_UPCTL;

        pllref = ccm_get_pll_ref_clk_rate();

        mcu_pllfreq = ccm_calc_pll_rate(pllref, mpctl);
        ser_pllfreq = ccm_calc_pll_rate(pllref, spctl);
        usb_pllfreq = ccm_calc_pll_rate(pllref, upctl);

        lcd_putsf(0, line++, "pll_ref_clk: %u", pllref);
        line++;

        /* MCU clock domain */
        lcd_putsf(0, line++, "MPCTL: %08lX", mpctl);

        lcd_putsf(0, line++, " mpl_dpdgck_clk: %u", mcu_pllfreq);
        line++;

        regval = CCM_PDR0;
        lcd_putsf(0, line++, "  PDR0: %08lX", regval);

        freq = mcu_pllfreq / (((regval & 0x7) + 1));
        lcd_putsf(0, line++, "   mcu_clk:      %u", freq);

        freq = mcu_pllfreq / (((regval >> 11) & 0x7) + 1);
        lcd_putsf(0, line++, "   hsp_clk:      %u", freq);

        freq = mcu_pllfreq / (((regval >> 3) & 0x7) + 1);
        lcd_putsf(0, line++, "   hclk_clk:     %u", freq);

        lcd_putsf(0, line++, "   ipg_clk:      %u",
            freq / (unsigned)(((regval >> 6) & 0x3) + 1));

        lcd_putsf(0, line++, "   nfc_clk:      %u",
            freq / (unsigned)(((regval >> 8) & 0x7) + 1));

        line++;

        /* Serial clock domain */
        lcd_putsf(0, line++, "SPCTL: %08lX", spctl);
        lcd_putsf(0, line++, " spl_dpdgck_clk: %u", ser_pllfreq);

        line++;

        /* USB clock domain */
        lcd_putsf(0, line++, "UPCTL: %08lX", upctl);

        lcd_putsf(0, line++, " upl_dpdgck_clk: %u", usb_pllfreq);

        regval = CCM_PDR1;
        lcd_putsf(0, line++, "  PDR1: %08lX", regval);

        freq = usb_pllfreq /
            ((((regval >> 30) & 0x3) + 1) * (((regval >> 27) & 0x7) + 1));
        lcd_putsf(0, line++, "   usb_clk:       %u", freq);

        freq = usb_pllfreq / (((CCM_PDR0 >> 16) & 0x1f) + 1);
        lcd_putsf(0, line++, "   ipg_per_baud:  %u", freq);

        line++;
        lcd_putsf(0, line++, "PMCR0: %08lX", CCM_PMCR0);

        line++;
        lcd_putsf(0, line++, "cpu_frequency: %ld Hz", cpu_frequency);

        lcd_putsf(0, line++, "dvfs_level: %u", dvfs_get_level());
        lcd_putsf(0, line++, "dvfs d|u|p|n: %u %u %u %u",
                  dvfs_nr_dn, dvfs_nr_up, dvfs_nr_pnc, dvfs_nr_no);
        regval = dvfs_dptc_get_voltage();
        lcd_putsf(0, line++, "cpu_voltage: %d.%03d V", regval / 1000,
                  regval % 1000);

        lcd_putsf(0, line++, "dptc_wp: %u", dptc_get_wp());
        lcd_putsf(0, line++, "dptc d|u|p|n: %u %u %u %u",
                  dptc_nr_dn, dptc_nr_up, dptc_nr_pnc, dptc_nr_no);
        lcd_putsf(0, line++, "DVCR0,1: %08lX %08lX", CCM_DCVR0, CCM_DCVR1);
        lcd_putsf(0, line++, "DVCR2,3: %08lX %08lX", CCM_DCVR2, CCM_DCVR3);
        lcd_putsf(0, line++, "SWITCHERS0: %08lX", mc13783_read(MC13783_SWITCHERS0));
        lcd_putsf(0, line++, "SWITCHERS1: %08lX", mc13783_read(MC13783_SWITCHERS1));
          
        lcd_update();

        if (button_get_w_tmo(HZ/10) == (DEBUG_CANCEL|BUTTON_REL))
            return false;
    }
}

bool dbg_ports(void)
{
    int line;
    int i;

    static const char pmic_regset[] =
    {
        MC13783_INTERRUPT_STATUS0,
        MC13783_INTERRUPT_SENSE0,
        MC13783_INTERRUPT_STATUS1,
        MC13783_INTERRUPT_SENSE1,
        MC13783_CHARGER,
        MC13783_RTC_TIME,
        MC13783_RTC_ALARM,
        MC13783_RTC_DAY,
        MC13783_RTC_DAY_ALARM,
    };

    static const char *pmic_regnames[ARRAYLEN(pmic_regset)] =
    {
        "Int Stat0 ",
        "Int Sense0",
        "Int Stat1 ",
        "Int Sense1",
        "Charger   ",
        "RTC Time  ",
        "RTC Alarm ",
        "RTC Day   ",
        "RTC Day Al",
    };

    uint32_t pmic_regs[ARRAYLEN(pmic_regset)];

    lcd_clear_display();
    lcd_setfont(FONT_SYSFIXED);

    while(1)
    {
        line = 0;
        lcd_puts(0, line++, "[Ports and Registers]");
        line++;

        /* GPIO1 */
        lcd_putsf(0, line++, "GPIO1: DR:   %08lx GDIR: %08lx", GPIO1_DR, GPIO1_GDIR);

        lcd_putsf(0, line++, "       PSR:  %08lx ICR1: %08lx", GPIO1_PSR, GPIO1_ICR1);

        lcd_putsf(0, line++, "       ICR2: %08lx IMR:  %08lx", GPIO1_ICR2, GPIO1_IMR);

        lcd_putsf(0, line++, "       ISR:  %08lx",             GPIO1_ISR);
        line++;

        /* GPIO2 */
        lcd_putsf(0, line++, "GPIO2: DR:   %08lx GDIR: %08lx", GPIO2_DR, GPIO2_GDIR);

        lcd_putsf(0, line++, "       PSR:  %08lx ICR1: %08lx", GPIO2_PSR, GPIO2_ICR1);

        lcd_putsf(0, line++, "       ICR2: %08lx IMR:  %08lx", GPIO2_ICR2, GPIO2_IMR);

        lcd_putsf(0, line++, "       ISR:  %08lx",             GPIO2_ISR);
        line++;

        /* GPIO3 */
        lcd_putsf(0, line++, "GPIO3: DR:   %08lx GDIR: %08lx", GPIO3_DR, GPIO3_GDIR);

        lcd_putsf(0, line++, "       PSR:  %08lx ICR1: %08lx", GPIO3_PSR, GPIO3_ICR1);

        lcd_putsf(0, line++, "       ICR2: %08lx IMR:  %08lx", GPIO3_ICR2, GPIO3_IMR);

        lcd_putsf(0, line++, "       ISR:  %08lx",             GPIO3_ISR);
        line++;

        lcd_puts(0, line++, "PMIC Registers");
        line++;

        mc13783_read_regs(pmic_regset, pmic_regs, ARRAYLEN(pmic_regs));

        for (i = 0; i < (int)ARRAYLEN(pmic_regs); i++)
        {
            lcd_putsf(0, line++, "%s: %08lx", pmic_regnames[i], pmic_regs[i]);
        }

        line++;

        lcd_puts(0, line++, "ADC");
        line++;

        for (i = 0; i < NUM_ADC_CHANNELS; i += 4)
        {
            lcd_putsf(0, line++,
                     "CH%02d:%04u CH%02d:%04u CH%02d:%04u CH%02d:%04u",
                     i+0, adc_read(i+0),
                     i+1, adc_read(i+1),
                     i+2, adc_read(i+2),
                     i+3, adc_read(i+3));
        }

        lcd_update();
        if (button_get_w_tmo(HZ/10) == (DEBUG_CANCEL|BUTTON_REL))
            return false;
    }
}


bool __dbg_dvfs_dptc(void)
{
    int ltwlevel;
    unsigned long ltdetect;
    int dvfs_wp, dvfs_mask;
    bool dptc_on;
    int i;
    char buf[32];
    unsigned long ltw[4];
    bool ltwassert[4];

    lcd_setfont(FONT_SYSFIXED);

    dvfs_mask = dvfs_level_mask();

    dvfs_wp = dvfs_enabled() ? -1 : (int)dvfs_get_level();
    dptc_on = dptc_enabled();
    dvfs_get_gp_sense(&ltwlevel, &ltdetect);

    while (1)
    {
        int line = 0;

        int button = button_get_w_tmo(HZ/10);

        lcd_clear_display();

        if (dvfs_wp < 0)
            strcpy(buf, "Auto");
        else
            snprintf(buf, sizeof(buf), "%d", dvfs_wp);

        lcd_puts(0, line, "[DVFS/DPTC]");
        line += 2;
        lcd_putsf(0, line, "CPU freq. point (Up/Dn) : %s", buf);
        line += 2;
        lcd_putsf(0, line, "DPTC volt. scale (Play) : %s",
                  dptc_on ? "Enabled" : "Disabled");
        line += 2;
        lcd_putsf(0, line, "GP load level (Vol +/-) : %d", ltwlevel);
        line += 2;
        lcd_puts(0, line, "----------------------------------------");
        line += 2;
        lcd_putsf(0, line++, "Frequency: %dHz", cpu_frequency);
        i = dvfs_dptc_get_voltage();
        lcd_putsf(0, line++, "Voltage  : %d.%03d V", i / 1000, i % 1000);

        for (i = 0; i <= 3; i++)
        {
            ltw[i] = dvfs_get_lt_weight(i + DVFS_LT_SIG_DVGP0);
            ltwassert[i] = dvfs_get_gp_bit(i + DVFS_DVGP_0);
        }

        lcd_putsf(0, line++, "GPW (3-0): %lu%lu%lu%lu %c%c%c%c",
                  ltw[3], ltw[2], ltw[1], ltw[0],
                  ltwassert[3] ? 'y' : 'n',
                  ltwassert[2] ? 'y' : 'n',
                  ltwassert[1] ? 'y' : 'n',
                  ltwassert[0] ? 'y' : 'n');

        line += 2;
        lcd_puts(8, line, "(Press SELECT to revert)");

        switch (button)
        {
        case DEBUG_CANCEL|BUTTON_REL:
            return false;

        /* CPU frequency */
        case BUTTON_UP:
            if (++dvfs_wp >= DVFS_NUM_LEVELS)
            {
                /* Going back to automatic */
                dvfs_wp = -1;
                dvfs_start();
            }
            else
            {
                if (dvfs_wp == 0)
                {
                    /* Going to manual setting */
                    dvfs_stop();
                }

                /* Skip gaps in mask */
                while (((1 << dvfs_wp) & dvfs_mask) == 0) dvfs_wp++;
                dvfs_set_level(dvfs_wp);
            }

            break;

        case BUTTON_DOWN:
            if (--dvfs_wp == -1)
            {
                /* Going back to automatic */
                dvfs_start();
            }
            else
            {
                if (dvfs_wp <= -2)
                {
                    /* Going to manual setting */
                    dvfs_stop();
                    dvfs_wp = DVFS_NUM_LEVELS - 1;
                }

                /* Skip gaps in mask */
                while (((1 << dvfs_wp) & dvfs_mask) == 0) dvfs_wp--;
                dvfs_set_level(dvfs_wp);
            }
            break;

        /* GP Load tracking */
        case BUTTON_VOL_UP:
            if (ltwlevel < 28)
                dvfs_set_gp_sense(++ltwlevel, ltdetect);
            break;

        case BUTTON_VOL_DOWN:
            if (ltwlevel > 0)
                dvfs_set_gp_sense(--ltwlevel, ltdetect);
            break;

        case BUTTON_PLAY:
            dptc_on = !dptc_enabled();
            dptc_on ? dptc_start() : dptc_stop();
            break;

        case BUTTON_SELECT:
            dvfs_start();
            dptc_start();
            dvfs_set_gp_sense(-1, 0);

            dvfs_wp = dvfs_enabled() ? -1 : (int)dvfs_get_level();
            dptc_on = dptc_enabled();
            dvfs_get_gp_sense(&ltwlevel, &ltdetect);
            break;
        }

        lcd_update();
        yield();
    }
}
