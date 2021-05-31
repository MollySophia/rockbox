#include "rockmacros.h"

#include "defs.h"
#include "hw.h"
#include "cpu-gb.h"
#include "regs.h"
#include "lcd-gb.h"
#include "fb.h"
#include "lcdc.h"


#define C (cpu.lcdc)


/*
 * stat_trigger updates the STAT interrupt line to reflect whether any
 * of the conditions set to be tested (by bits 3-6 of R_STAT) are met.
 * This function should be called whenever any of the following occur:
 * 1) LY or LYC changes.
 * 2) A state transition affects the low 2 bits of R_STAT (see below).
 * 3) The program writes to the upper bits of R_STAT.
 * stat_trigger also updates bit 2 of R_STAT to reflect whether LY=LYC.
 */

void stat_trigger(void)
{
    static const int condbits[4] = { 0x08, 0x10, 0x20, 0x00 };
    int flag = 0;

    if (R_LY == R_LYC)
    {
        R_STAT |= 0x04;
        if (R_STAT & 0x40) flag = IF_STAT;
    }
    else R_STAT &= ~0x04;

    if (R_STAT & condbits[R_STAT&3]) flag = IF_STAT;

    if (!(R_LCDC & 0x80)) flag = 0;
    
    hw_interrupt(flag, IF_STAT);
}

/*
 * stat_change is called when a transition results in a change to the
 * LCD STAT condition (the low 2 bits of R_STAT).  It raises or lowers
 * the VBLANK interrupt line appropriately and calls stat_trigger to
 * update the STAT interrupt line.
 */

static void stat_change(unsigned int stat)
{
    R_STAT = (R_STAT & 0x7C) | stat;

    if (stat != 1) hw_interrupt(0, IF_VBLANK);
    /* hw_interrupt((stat == 1) ? IF_VBLANK : 0, IF_VBLANK); */
    stat_trigger();
}


void lcdc_change(byte b)
{
    byte old = R_LCDC;
    R_LCDC = b;
    if ((R_LCDC ^ old) & 0x80) /* lcd on/off change */
    {
        R_LY = 0;
        stat_change(2);
        C = 40;
        lcd_begin();
    }
}


void lcdc_trans(void)
{
    if (!(R_LCDC & 0x80))
    {
        while (C <= 0)
        {
            switch ((byte)(R_STAT & 3))
            {
            case 0:
            case 1:
                stat_change(2);
                C += 40;
                break;
            case 2:
                stat_change(3);
                C += 86;
                break;
            case 3:
                stat_change(0);
                if (hw.hdma & 0x80)
                    hw_hdma();
                else
                    C += 102;
                break;
            }
            return;
        }
    }
    while (C <= 0)
    {
        switch ((byte)(R_STAT & 3))
        {
        case 1:
            if (!(hw.ilines & IF_VBLANK))
            {
                C += 218;
                hw_interrupt(IF_VBLANK, IF_VBLANK);
                break;
            }
            if (R_LY == 0)
            {
                lcd_begin();
                stat_change(2);
                C += 40;
                break;
            }
            else if (R_LY < 152)
                C += 228;
            else if (R_LY == 152)
                C += 28;
            else
            {
                R_LY = -1;
                C += 200;
            }
            R_LY++;
            stat_trigger();
            break;
        case 2:
            if (fb.enabled)
                lcd_refreshline();
            stat_change(3);
            C += 86;
            break;
        case 3:
            stat_change(0);
            if (hw.hdma & 0x80)
                hw_hdma();
            /* FIXME -- how much of the hblank does hdma use?? */
            /* else */
            C += 102;
            break;
        case 0:
            if (++R_LY >= 144)
            {
                if (cpu.halt)
                {
                    hw_interrupt(IF_VBLANK, IF_VBLANK);
                    C += 228;
                }
                else C += 10;
                stat_change(1);
                break;
            }
            stat_change(2);
            C += 40;
            break;
        }
    }
}







