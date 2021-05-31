/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Linus Nielsen Feltzing
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
#include "ata.h"
#include "usb.h"
#include "usb_core.h"
#include "usb_drv.h"
#include "mc13783.h"
#include "ccm-imx31.h"
#include "avic-imx31.h"
#include "power-gigabeat-s.h"
#include <string.h>

static int usb_status = USB_EXTRACTED;
static bool bootloader_install_mode = false;

static void enable_transceiver(bool enable)
{
    if (enable)
    {
        if (GPIO1_DR & (1 << 30))
        {
            bitclr32(&GPIO3_DR, (1 << 16)); /* Reset ISP1504 */
            sleep(HZ/100);
            bitset32(&GPIO3_DR, (1 << 16));
            sleep(HZ/10);
            bitclr32(&GPIO1_DR, (1 << 30)); /* Select ISP1504 */
        }
    }
    else
    {
        bitset32(&GPIO1_DR, (1 << 30)); /* Deselect ISP1504 */
    }
}

/* Read the immediate state of the cable from the PMIC */
bool usb_plugged(void)
{
    return mc13783_read(MC13783_INTERRUPT_SENSE0) & MC13783_USB4V4S;
}

/* Helper to update the USB cable status */
static void update_usb_status(bool sense)
{
    int status = sense ? USB_INSERTED : USB_EXTRACTED;
    usb_status = status;
    /* Notify power that USB charging is potentially available */
    charger_usb_detect_event(status);
    usb_status_event(status);
}

/* Detect presence of USB bus - called from PMIC ISR */
void MC13783_EVENT_CB_USB(void)
{
    /* Read the associated sense value */
    update_usb_status(mc13783_event_sense());
}

int usb_detect(void)
{
    return usb_status;
}

void usb_init_device(void)
{
    /* Do one-time inits */
    usb_drv_startup();

    /* Initially poll */
    update_usb_status(usb_plugged());

    /* Enable PMIC event */
    mc13783_enable_event(MC13783_INT_ID_USB, true);
}

void usb_enable(bool on)
{
    /* Module clock should be on since since this could be called with
     * OFF initially and writing module registers would hardlock otherwise. */
    ccm_module_clock_gating(CG_USBOTG, CGM_ON_RUN_WAIT);
    enable_transceiver(true);

    if (on)
    {
        usb_core_init();
    }
    else
    {
        usb_core_exit();
        enable_transceiver(false);
        ccm_module_clock_gating(CG_USBOTG, CGM_OFF);
    }
}

void usb_attach(void)
{
    bootloader_install_mode = false;

    if (usb_core_driver_enabled(USB_DRIVER_MASS_STORAGE))
    {
        /* Check if this will be bootloader install mode, exposing the
         * boot partition instead of the data partition */
        bootloader_install_mode =
            (button_status() & USB_BL_INSTALL_MODE_BTN) != 0;
    }
}

static void __attribute__((interrupt("IRQ"))) USB_OTG_HANDLER(void)
{
    usb_drv_int(); /* Call driver handler */
}

void usb_drv_int_enable(bool enable)
{
    if (enable)
    {
        avic_enable_int(INT_USB_OTG, INT_TYPE_IRQ, INT_PRIO_DEFAULT,
                        USB_OTG_HANDLER);
    }
    else
    {
        avic_disable_int(INT_USB_OTG);
    }
}

/* Called when reading the MBR */
void usb_fix_mbr(unsigned char *mbr)
{
    unsigned char* p = mbr + 0x1be;

    /* Windows ignores the partition flags and mounts the first partition it
       sees when the device reports itself as removable. Swap the partitions
       so the data partition appears to be partition 0. Mark the boot
       partition 0 as hidden and make it partition 1.

       Update 2018-01-14: Windows shows it if exists, so long as it can mount
       it. Resort to not exposing entries that we want hidden.
     */

   /* The Gigabeat S factory partition table contains invalid values for the
       "active" flag in the MBR.  This prevents at least the Linux kernel
       from accepting the partition table, so we fix it on-the-fly. */
    p[0x00] &= 0x80;
    p[0x10] &= 0x80;
    p[0x20] &= 0x80;
    p[0x30] &= 0x80;

    if (bootloader_install_mode)
    {
        memset(&p[0x10], 0x00, 0x30);      /* Hide non-boot partitions */
    }
    else
    {
        memmove(&p[0x00], &p[0x10], 0x30); /* Hide boot partition */
        memset(&p[0x30], 0x00, 0x10);
    }
}
