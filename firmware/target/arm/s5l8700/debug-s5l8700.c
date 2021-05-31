/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright © 2008 Rafaël Carré
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

#include <stdbool.h>
#include "config.h"
#include "system.h"
#include "kernel.h"
#include "button.h"
#include "lcd.h"
#include "font.h"
#include "storage.h"
#ifdef IPOD_NANO2G
#include "power.h"
#include "pmu-target.h"
#include "nand-target.h"
#endif
#ifdef HAVE_SERIAL
#include "uart-target.h"
#include "uc870x.h"
#endif

#define DEBUG_CANCEL BUTTON_MENU

/*  Skeleton for adding target specific debug info to the debug menu
 */

#define _DEBUG_PRINTF(a, varargs...) lcd_putsf(0, line++, (a), ##varargs);

extern int lcd_type;
extern uint32_t nand_type[4];

bool dbg_hw_info(void)
{
    int line;
    int i;
#ifdef IPOD_NANO2G
    unsigned int state = 0;
#ifdef UC870X_DEBUG
    const unsigned int max_states=3;
#else
    const unsigned int max_states=2;
#endif
    int nand_bank_count;
    struct storage_info info;
    const struct nand_device_info_type *nand_devicetype[4];
    nand_get_info(&info);
    nand_bank_count = 0;
    for(i=0;i<4;i++)
    {
        nand_devicetype[i] = nand_get_device_type(i);
        if(nand_devicetype[i] != NULL) nand_bank_count++;
    }
#endif

    lcd_clear_display();
    lcd_setfont(FONT_SYSFIXED);

    state=0;
    while(1)
    {
        lcd_clear_display();
        line = 0;

        /* _DEBUG_PRINTF statements can be added here to show debug info */
#ifdef IPOD_NANO2G

        if(state == 0)
        {
            _DEBUG_PRINTF("CPU:");
            _DEBUG_PRINTF("current_tick: %d", (unsigned int)current_tick);
            line++;

            _DEBUG_PRINTF("LCD:");
            _DEBUG_PRINTF("type: %d, %s", lcd_type, lcd_type ? "(7) LDS176" : "(2) ILI9320");
            line++;

            _DEBUG_PRINTF("NAND:");
            _DEBUG_PRINTF("banks: %d",nand_bank_count);

            for(i=0;i<4;i++)
            {
                if(nand_devicetype[i] != NULL)
                {
                    _DEBUG_PRINTF("bank: %d, id: %08X", i, (unsigned int)(*nand_devicetype[i]).id);
                }
            }

            _DEBUG_PRINTF("sectors: %d", info.num_sectors);
            _DEBUG_PRINTF("sector size: %d", info.sector_size);
            _DEBUG_PRINTF("last disk activity: %d", (unsigned int)nand_last_disk_activity());
        }
        else if(state==1)
        {
            _DEBUG_PRINTF("PMU:");
            for(i=0;i<7;i++)
            {
                char *device[] = {"(unknown)", 
                                  "(CLICKWHEEL)", 
                                  "(LCD)",
                                  "(AUDIO)",
                                  "(NAND)",
                                  "(unknown)",
                                  "(ACCESSORY)"};
                _DEBUG_PRINTF("ldo%d %s: %dmV %s",i,
                    pmu_read(0x2e + (i << 1))?" on":"off",
                    900 + pmu_read(0x2d + (i << 1))*100,
                    device[i]);
            }
            _DEBUG_PRINTF("cpu voltage: %dmV",625 + pmu_read(0x1e)*25);
            _DEBUG_PRINTF("memory voltage: %dmV",625 + pmu_read(0x22)*25);
            line++;
            _DEBUG_PRINTF("charging: %s", charging_state() ? "true" : "false");
            _DEBUG_PRINTF("backlight: %s", pmu_read(0x29) ? "on" : "off");
            _DEBUG_PRINTF("brightness value: %d", pmu_read(0x28));
        }
#ifdef UC870X_DEBUG
        else if(state==2)
        {
            extern struct uartc_port ser_port;
            int tx_stat, rx_stat, tx_speed, rx_speed;
            char line_cfg[4];
            int abr_stat;
            uint32_t abr_cnt;
            char *abrstatus[] = {"Idle", "Launched", "Counting", "Abnormal"};

            uartc_port_get_line_info(&ser_port,
                        &tx_stat, &rx_stat, &tx_speed, &rx_speed, line_cfg);
            abr_stat = uartc_port_get_abr_info(&ser_port, &abr_cnt);

            _DEBUG_PRINTF("UART %d:", ser_port.id);
            line++;
            _DEBUG_PRINTF("line: %s", line_cfg);
            _DEBUG_PRINTF("Tx: %s, speed: %d", tx_stat ? "On":"Off", tx_speed);
            _DEBUG_PRINTF("Rx: %s, speed: %d", rx_stat ? "On":"Off", rx_speed);
            _DEBUG_PRINTF("ABR: %s, cnt: %u", abrstatus[abr_stat], abr_cnt);
            line++;
            _DEBUG_PRINTF("n_tx_bytes: %u", ser_port.n_tx_bytes);
            _DEBUG_PRINTF("n_rx_bytes: %u", ser_port.n_rx_bytes);
            _DEBUG_PRINTF("n_ovr_err: %u", ser_port.n_ovr_err);
            _DEBUG_PRINTF("n_parity_err: %u", ser_port.n_parity_err);
            _DEBUG_PRINTF("n_frame_err: %u", ser_port.n_frame_err);
            _DEBUG_PRINTF("n_break_detect: %u", ser_port.n_break_detect);
            _DEBUG_PRINTF("ABR n_abnormal: %u %u",
                        ser_port.n_abnormal0, ser_port.n_abnormal1);
        }
#endif
        else
        {
            state=0;
        }

#else
        _DEBUG_PRINTF("dbg_hw_info");
#endif

        lcd_update(); 
        switch(button_get_w_tmo(HZ/20))
        {
            case BUTTON_SCROLL_BACK:
                if(state!=0) state--;
                break;

            case BUTTON_SCROLL_FWD:
                if(state!=max_states-1)
                {
                    state++;
                }
                break;

            case DEBUG_CANCEL:
            case BUTTON_REL:
                lcd_setfont(FONT_UI);
                return false;
        }
    }

    lcd_setfont(FONT_UI);
    return false;
}

bool dbg_ports(void)
{
    int line;

    lcd_setfont(FONT_SYSFIXED);

    while(1)
    {
        lcd_clear_display();
        line = 0;
        
        _DEBUG_PRINTF("GPIO  0: %08x",(unsigned int)PDAT0);
        _DEBUG_PRINTF("GPIO  1: %08x",(unsigned int)PDAT1);
        _DEBUG_PRINTF("GPIO  2: %08x",(unsigned int)PDAT2);
        _DEBUG_PRINTF("GPIO  3: %08x",(unsigned int)PDAT3);
        _DEBUG_PRINTF("GPIO  4: %08x",(unsigned int)PDAT4);
        _DEBUG_PRINTF("GPIO  5: %08x",(unsigned int)PDAT5);
        _DEBUG_PRINTF("GPIO  6: %08x",(unsigned int)PDAT6);
        _DEBUG_PRINTF("GPIO  7: %08x",(unsigned int)PDAT7);
        _DEBUG_PRINTF("GPIO 10: %08x",(unsigned int)PDAT10);
        _DEBUG_PRINTF("GPIO 11: %08x",(unsigned int)PDAT11);
        _DEBUG_PRINTF("GPIO 13: %08x",(unsigned int)PDAT13);
        _DEBUG_PRINTF("GPIO 14: %08x",(unsigned int)PDAT14);
        _DEBUG_PRINTF("5USEC  : %08x",(unsigned int)FIVE_USEC_TIMER);
        _DEBUG_PRINTF("USEC   : %08x",(unsigned int)USEC_TIMER);
        _DEBUG_PRINTF("USECREG: %08x",(unsigned int)(*(REG32_PTR_T)(0x3C700084)));

        lcd_update();
        if (button_get_w_tmo(HZ/10) == (DEBUG_CANCEL|BUTTON_REL))
            break;
    }
    lcd_setfont(FONT_UI);
    return false;
}

