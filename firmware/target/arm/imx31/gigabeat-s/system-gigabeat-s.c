/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 by James Espinoza
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

#include "kernel.h"
#include "system.h"
#include "gcc_extensions.h"
#include "panic.h"
#include "avic-imx31.h"
#include "gpio-imx31.h"
#include "mmu-imx31.h"
#include "system-target.h"
#include "powermgmt.h"
#include "lcd.h"
#include "serial-imx31.h"
#include "debug.h"
#include "ccm-imx31.h"
#include "mc13783.h"
#include "dvfs_dptc-imx31.h"

static unsigned long product_rev;
static unsigned long system_rev;

/** IC revision info routines **/
unsigned int iim_system_rev(void)
{
    return system_rev & IIM_SREV_SREV;
}

unsigned int iim_prod_rev(void)
{
    return product_rev;
}

static void INIT_ATTR iim_init(void)
{
    /* Initialize the IC revision info (required by SDMA) */
    ccm_module_clock_gating(CG_IIM, CGM_ON_RUN_WAIT);
    product_rev = IIM_PREV;
    system_rev = IIM_SREV;
}

/** Watchdog timer routines **/

/* Initialize the watchdog timer */
void watchdog_init(unsigned int half_seconds)
{
    uint16_t wcr = ((half_seconds << WDOG_WCR_WT_POS) & WDOG_WCR_WT) |
                   WDOG_WCR_WOE |       /* WDOG output enabled */
                   WDOG_WCR_WDA |       /* WDOG assertion - no effect */
                   WDOG_WCR_SRS |       /* System reset - no effect */
                   WDOG_WCR_WRE;        /* Generate a WDOG signal */

    ccm_module_clock_gating(CG_WDOG, CGM_ON_RUN_WAIT);

    WDOG_WCR = wcr;
    WDOG_WSR = 0x5555;
    WDOG_WCR = wcr | WDOG_WCR_WDE; /* Enable timer - hardware does
                                      not allow a disable now */
    WDOG_WSR = 0xaaaa;
}

/* Service the watchdog timer */
void watchdog_service(void)
{
    WDOG_WSR = 0x5555;
    WDOG_WSR = 0xaaaa;
}


/** uevent APIs **/

static uevent_cb_type ucallback = NULL; /* uevent callback */

static void cancel_uevent(void)
{
    GPTSR = GPTSR_OF1;
    GPTIR &= ~GPTIR_OF1IE;
    ucallback = NULL;
}

static void __attribute__((interrupt("IRQ"))) GPT_HANDLER(void)
{
    uevent_cb_type cb = ucallback;
    cancel_uevent();
    cb();
}

void uevent(unsigned long usecs, uevent_cb_type callback)
{
    if (!callback || ucallback)
        return; /* Is busy */

    unsigned long status = disable_interrupt_save(IRQ_FIQ_STATUS);

    ucallback = callback;

    for (int i = 0; i < 1; i++)
    {
        unsigned long utime = GPTCNT;
        unsigned long time = utime + usecs + 1;

        GPTOCR1 = time;
        GPTSR = GPTSR_OF1;
        GPTIR |= GPTIR_OF1IE;

        if (TIME_BEFORE(GPTCNT, time))
            break; /* Didn't miss it */
    }

    restore_interrupt(status);
}

void uevent_cancel(void)
{
    unsigned long status = disable_interrupt_save(IRQ_FIQ_STATUS);

    if (ucallback)
        cancel_uevent();

    restore_interrupt(status);
}


/** GPT timer routines - basis for udelay/uevent **/

/* Start the general-purpose timer (1MHz) */
void gpt_start(void)
{
    ccm_module_clock_gating(CG_GPT, CGM_ON_RUN_WAIT);
    unsigned int ipg_mhz = ccm_get_ipg_clk() / 1000000;

    GPTCR &= ~GPTCR_EN; /* Disable counter */
    GPTCR |= GPTCR_SWR; /* Reset module */
    while (GPTCR & GPTCR_SWR);
    /* No output
     * No capture
     * Enable in wait and run mode
     * Freerun mode (count to 0xFFFFFFFF and roll-over to 0x00000000)
     */
    GPTCR = GPTCR_FRR | GPTCR_WAITEN | GPTCR_CLKSRC_IPG_CLK;
    GPTPR = ipg_mhz - 1;
    GPTSR = GPTSR_OF1;
    GPTCR |= GPTCR_EN;

    avic_enable_int(INT_GPT, INT_TYPE_IRQ, INT_PRIO_GPT, GPT_HANDLER);
}

/* Stop the general-purpose timer */
void gpt_stop(void)
{
    unsigned long status = disable_interrupt_save(IRQ_FIQ_STATUS);
    avic_disable_int(INT_GPT);
    cancel_uevent();
    GPTCR &= ~GPTCR_EN;
    restore_interrupt(status);
}

int system_memory_guard(int newmode)
{
    (void)newmode;
    return 0;
}

void system_halt(void)
{
    disable_interrupt(IRQ_FIQ_STATUS);
    avic_set_ni_level(AVIC_NIL_DISABLE);
    while (1)
        core_idle();
}

void system_reboot(void)
{
    /* Multi-context so no SPI available (WDT?)  */
    system_halt();
}

void system_exception_wait(void)
{
    /* Called in many contexts so button reading may be a chore */
    system_halt();
}

void INIT_ATTR system_init(void)
{
    static const enum IMX31_CG_LIST disable_clocks[] INITDATA_ATTR =
    {
        /* CGR0 */
        CG_SD_MMC1,
        CG_SD_MMC2,
        CG_IIM,
        CG_SDMA,
        CG_CSPI3,
        CG_RNG,
        CG_UART1,
        CG_UART2,
        CG_SSI1,
        CG_I2C1,
        CG_I2C2,
        CG_I2C3,

        /* CGR1 */
        CG_HANTRO,
        CG_MEMSTICK1,
        CG_MEMSTICK2,
        CG_CSI,
        CG_RTC,
        CG_WDOG,
        CG_PWM,
        CG_SIM,
        CG_ECT,
        CG_USBOTG,
        CG_KPP,
        CG_UART3,
        CG_UART4,
        CG_UART5,
        CG_1_WIRE,

        /* CGR2 */
        CG_SSI2,
        CG_CSPI1,
        CG_CSPI2,
        CG_GACC,
        CG_RTIC,
        CG_FIR
    };

    unsigned int i;

    /* Initialize frequency with current */
    cpu_frequency = ccm_get_mcu_clk();

    /* MCR WFI enables wait mode (CCM_CCMR_LPM_WAIT_MODE = 0) */
    bitclr32(&CCM_CCMR, CCM_CCMR_LPM);

    iim_init();

    bitset32(&SDHC1_CLOCK_CONTROL, STOP_CLK);
    bitset32(&SDHC2_CLOCK_CONTROL, STOP_CLK);
    bitset32(&RNGA_CONTROL, RNGA_CONTROL_SLEEP);
    bitclr32(&UCR1_1, EUARTUCR1_UARTEN);
    bitclr32(&UCR1_2, EUARTUCR1_UARTEN);
    bitclr32(&UCR1_3, EUARTUCR1_UARTEN);
    bitclr32(&UCR1_4, EUARTUCR1_UARTEN);
    bitclr32(&UCR1_5, EUARTUCR1_UARTEN);

    for (i = 0; i < ARRAYLEN(disable_clocks); i++)
        ccm_module_clock_gating(disable_clocks[i], CGM_OFF);

    avic_init();
    gpt_start();
    gpio_init();
}

void system_prepare_fw_start(void)
{
    dvfs_stop();
    dptc_stop();
    mc13783_close();
    tick_stop();
    disable_interrupt(IRQ_FIQ_STATUS);
    avic_set_ni_level(AVIC_NIL_DISABLE);
}


#ifndef BOOTLOADER
void rolo_restart_firmware(const unsigned char *source, unsigned char *dest,
                           int length) NORETURN_ATTR;

void NORETURN_ATTR
rolo_restart(const unsigned char *source, unsigned char *dest, int length)
{
    /* Some housekeeping tasks must be performed for a safe changeover */
    charging_algorithm_close();
    system_prepare_fw_start();

    /* Copying routine where new image is run */
    rolo_restart_firmware(source, dest, length);
}
#endif /* BOOTLOADER */


void dumpregs(void)
{
    asm volatile ("mov %0,r0\n\t"
                  "mov %1,r1\n\t"
                  "mov %2,r2\n\t"
                  "mov %3,r3":
                  "=r"(regs.r0),"=r"(regs.r1),
                  "=r"(regs.r2),"=r"(regs.r3):);

    asm volatile ("mov %0,r4\n\t"
                  "mov %1,r5\n\t"
                  "mov %2,r6\n\t"
                  "mov %3,r7":
                  "=r"(regs.r4),"=r"(regs.r5),
                  "=r"(regs.r6),"=r"(regs.r7):);

    asm volatile ("mov %0,r8\n\t"
                  "mov %1,r9\n\t"
                  "mov %2,r10\n\t"
                  "mov %3,r12":
                  "=r"(regs.r8),"=r"(regs.r9),
                  "=r"(regs.r10),"=r"(regs.r11):);

    asm volatile ("mov %0,r12\n\t"
                   "mov %1,sp\n\t"
                   "mov %2,lr\n\t"
                   "mov %3,pc\n"
                   "sub %3,%3,#8":
                   "=r"(regs.r12),"=r"(regs.sp),
                   "=r"(regs.lr),"=r"(regs.pc):);
#ifdef HAVE_SERIAL
    dprintf("Register Dump :\n");
    dprintf("R0=0x%x\tR1=0x%x\tR2=0x%x\tR3=0x%x\n",regs.r0,regs.r1,regs.r2,regs.r3);
    dprintf("R4=0x%x\tR5=0x%x\tR6=0x%x\tR7=0x%x\n",regs.r4,regs.r5,regs.r6,regs.r7);
    dprintf("R8=0x%x\tR9=0x%x\tR10=0x%x\tR11=0x%x\n",regs.r8,regs.r9,regs.r10,regs.r11);
    dprintf("R12=0x%x\tSP=0x%x\tLR=0x%x\tPC=0x%x\n",regs.r12,regs.sp,regs.lr,regs.pc);
    //dprintf("CPSR=0x%x\t\n",regs.cpsr);
#endif
    DEBUGF("Register Dump :\n");
    DEBUGF("R0=0x%x\tR1=0x%x\tR2=0x%x\tR3=0x%x\n",regs.r0,regs.r1,regs.r2,regs.r3);
    DEBUGF("R4=0x%x\tR5=0x%x\tR6=0x%x\tR7=0x%x\n",regs.r4,regs.r5,regs.r6,regs.r7);
    DEBUGF("R8=0x%x\tR9=0x%x\tR10=0x%x\tR11=0x%x\n",regs.r8,regs.r9,regs.r10,regs.r11);
    DEBUGF("R12=0x%x\tSP=0x%x\tLR=0x%x\tPC=0x%x\n",regs.r12,regs.sp,regs.lr,regs.pc);
    //DEBUGF("CPSR=0x%x\t\n",regs.cpsr);

 }

#ifdef HAVE_ADJUSTABLE_CPU_FREQ

void set_cpu_frequency(long frequency)
{
    (void)freqency;
}

#endif
