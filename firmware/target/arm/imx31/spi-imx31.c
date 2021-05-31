/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (c) 2007 Will Robertson
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
#include "spi-imx31.h"
#include "avic-imx31.h"
#include "ccm-imx31.h"
#include "debug.h"
#include "kernel.h"

/* Forward interrupt handler declarations */
#if (SPI_MODULE_MASK & USE_CSPI1_MODULE)
static __attribute__((interrupt("IRQ"))) void CSPI1_HANDLER(void);
#endif
#if (SPI_MODULE_MASK & USE_CSPI2_MODULE)
static __attribute__((interrupt("IRQ"))) void CSPI2_HANDLER(void);
#endif
#if (SPI_MODULE_MASK & USE_CSPI3_MODULE)
static __attribute__((interrupt("IRQ"))) void CSPI3_HANDLER(void);
#endif

#define RXDATA      (0x000 / sizeof (unsigned long)) /* 000h */
#define TXDATA      (0x004 / sizeof (unsigned long)) /* 004h */
#define CONREG      (0x008 / sizeof (unsigned long)) /* 008h */
#define INTREG      (0x00c / sizeof (unsigned long)) /* 00Ch */
#define DMAREG      (0x010 / sizeof (unsigned long)) /* 010h */
#define STATREG     (0x014 / sizeof (unsigned long)) /* 014h */
#define PERIODREG   (0x01c / sizeof (unsigned long)) /* 018h */
#define TESTREG     (0x1c0 / sizeof (unsigned long)) /* 1C0h */

/* State data associatated with each CSPI module */
static struct spi_module_desc
{
    volatile unsigned long * const base; /* CSPI module address */
    struct spi_transfer_desc *head;      /* Running job */
    struct spi_transfer_desc *tail;      /* Most recent job added */
    const struct spi_node *last_node;    /* Last node used for module */
    void (* const handler)(void);        /* Interrupt handler */
    int rxcount;                         /* Independent copy of txcount */
    int8_t enable;                       /* Enable count */
    int8_t byte_size;                    /* Size of transfers in bytes */
    const int8_t cg;                     /* Clock-gating value */
    const int8_t ints;                   /* AVIC vector number */
} spi_descs[SPI_NUM_CSPI] =
/* Init non-zero members */
{
#if (SPI_MODULE_MASK & USE_CSPI1_MODULE)
    {
        .base    = (unsigned long *)CSPI1_BASE_ADDR,
        .cg      = CG_CSPI1,
        .ints    = INT_CSPI1,
        .handler = CSPI1_HANDLER,
    },
#endif
#if (SPI_MODULE_MASK & USE_CSPI2_MODULE)
    {
        .base    = (unsigned long *)CSPI2_BASE_ADDR,
        .cg      = CG_CSPI2,
        .ints    = INT_CSPI2,
        .handler = CSPI2_HANDLER,
    },
#endif
#if (SPI_MODULE_MASK & USE_CSPI3_MODULE)
    {
        .base    = (unsigned long *)CSPI3_BASE_ADDR,
        .cg      = CG_CSPI3,
        .ints    = INT_CSPI3,
        .handler = CSPI3_HANDLER,
    },
#endif
};

/* Reset the module */
static void spi_reset(struct spi_module_desc * const desc)
{
    /* Reset by leaving it disabled */
    desc->base[CONREG] &= ~CSPI_CONREG_EN;
}

/* Write the context for the node and remember it to avoid unneeded reconfigure */
static bool spi_set_context(struct spi_module_desc *desc,
                            struct spi_transfer_desc *xfer)
{
    const struct spi_node * const node = xfer->node; 
    volatile unsigned long * const base = desc->base;

    if (desc->enable == 0)
        return false;

    if (node == desc->last_node)
        return true;

    /* Errata says CSPI should be disabled when writing PERIODREG. */
    base[CONREG] &= ~CSPI_CONREG_EN;

    /* Switch the module's node */
    desc->last_node = node;
    desc->byte_size = (((node->conreg >> 8) & 0x1f) + 1 + 7) / 8 - 1;

    /* Set the wait-states */
    base[PERIODREG] = node->periodreg & 0xffff;

    /* Keep reserved and start bits cleared. Keep enabled bit. */
    base[CONREG] =
        (node->conreg & ~(0xfcc8e000 | CSPI_CONREG_XCH | CSPI_CONREG_SMC));

    return true;
}


/* Fill the TX fifo. Returns the number of remaining words. */
static int tx_fill_fifo(struct spi_module_desc * const desc,
                        volatile unsigned long * const base,
                        struct spi_transfer_desc * const xfer)
{
    int count = xfer->count;
    int size = desc->byte_size;

    while ((base[STATREG] & CSPI_STATREG_TF) == 0)
    {
        uint32_t word = 0;

        switch (size & 3)
        {
        case 3:
            word  = *(unsigned char *)(xfer->txbuf + 3) << 24;
        case 2:
            word |= *(unsigned char *)(xfer->txbuf + 2) << 16;
        case 1:
            word |= *(unsigned char *)(xfer->txbuf + 1) << 8;
        case 0:
            word |= *(unsigned char *)(xfer->txbuf + 0);
        }

        xfer->txbuf += size + 1; /* Increment buffer */

        base[TXDATA] = word;    /* Write to FIFO */

        if (--count == 0)
            break;
    }

    xfer->count = count;

    return count;
}

/* Start a transfer on the SPI */
static bool start_transfer(struct spi_module_desc * const desc,
                           struct spi_transfer_desc * const xfer)
{
    if (!spi_set_context(desc, xfer))
    {
        xfer->count = -1;
        return false;
    }

    volatile unsigned long * const base = desc->base;

    base[CONREG] |= CSPI_CONREG_EN; /* Enable module */

    desc->rxcount = xfer->count;

    unsigned long intreg;

    intreg = (xfer->count < 8) ?
        CSPI_INTREG_TCEN : /* Trans. complete: TX will run out in prefill */
        CSPI_INTREG_THEN;  /* INT when TX half-empty */

    intreg |= (xfer->count < 4) ?
        CSPI_INTREG_RREN : /* Must grab data on every word */
        CSPI_INTREG_RHEN;  /* Enough data to wait for half-full */

    tx_fill_fifo(desc, base, xfer);

    base[STATREG] = CSPI_STATREG_TC; /* Ack 'complete' */
    base[INTREG]  = intreg;          /* Enable interrupts */
    base[CONREG] |= CSPI_CONREG_XCH; /* Begin transfer */

    return true;
}

/* Common code for interrupt handlers */
static void spi_interrupt(struct spi_module_desc * const desc)
{
    volatile unsigned long * const base = desc->base;
    unsigned long intreg = base[INTREG];
    struct spi_transfer_desc *xfer = desc->head;
    int inc = desc->byte_size + 1;

    /* Data received - empty out RXFIFO */
    while ((base[STATREG] & CSPI_STATREG_RR) != 0)
    {
        uint32_t word = base[RXDATA];

        if (desc->rxcount <= 0)
            continue;

        if (xfer->rxbuf != NULL)
        {
            /* There is a receive buffer */
            switch (desc->byte_size & 3)
            {
            case 3:
                *(unsigned char *)(xfer->rxbuf + 3) = word >> 24;
            case 2:
                *(unsigned char *)(xfer->rxbuf + 2) = word >> 16;
            case 1:
                *(unsigned char *)(xfer->rxbuf + 1) = word >> 8;
            case 0:
                *(unsigned char *)(xfer->rxbuf + 0) = word;
            }

            xfer->rxbuf += inc;
        }

        if (--desc->rxcount < 4)
        {
            if (desc->rxcount == 0)
            {
                /* No more to receive - stop RX interrupts */
                intreg &= ~(CSPI_INTREG_RHEN | CSPI_INTREG_RREN);
                base[INTREG] = intreg;
            }
            else if (intreg & CSPI_INTREG_RHEN)
            {
                /* < 4 words expected - switch to RX ready */
                intreg &= ~CSPI_INTREG_RHEN;
                intreg |= CSPI_INTREG_RREN;
                base[INTREG] = intreg;
            }
        }
    }

    if (xfer->count > 0)
    {
        /* Data to transmit - fill TXFIFO or write until exhausted. */
        int remaining = tx_fill_fifo(desc, base, xfer);

        /* If transfer completed because TXFIFO ran out of data, resume it or
           else it will not finish. */
        if (!(base[CONREG] & CSPI_CONREG_XCH))
        {
            base[STATREG] = CSPI_STATREG_TC;
            base[CONREG] |= CSPI_CONREG_XCH;
        }

        if (remaining > 0)
            return; /* Still more after this */

        /* Out of data - stop TX interrupts, enable TC interrupt. */
        intreg &= ~CSPI_INTREG_THEN;
        intreg |= CSPI_INTREG_TCEN;
        base[INTREG] = intreg;
    }

    if ((intreg & CSPI_INTREG_TCEN) && (base[STATREG] & CSPI_STATREG_TC))
    {
        /* Outbound transfer is complete. */
        intreg &= ~CSPI_INTREG_TCEN;
        base[INTREG] = intreg;
    }

    if (intreg != 0)
        return;

    /* All interrupts are masked; we're done with current transfer. */
    for (;;)
    {
        struct spi_transfer_desc *next = xfer->next;
        spi_transfer_cb_fn_type callback = xfer->callback;
        xfer->next = NULL;

        if (next == xfer)
        {
            /* Last job on queue */
            desc->head = NULL;

            if (callback != NULL)
                callback(xfer);

            /* Callback may have restarted transfers. */
            if (desc->head == NULL)
                base[CONREG] &= ~CSPI_CONREG_EN; /* Disable module */
        }
        else
        {
            /* Queue next job. */
            desc->head = next;

            if (callback != NULL)
                callback(xfer);

            if (!start_transfer(desc, next))
            {
                xfer = next;
                continue; /* Failed: try next */
            }
        }

        break;
    }
}

/* Interrupt handlers for each CSPI module */
#if (SPI_MODULE_MASK & USE_CSPI1_MODULE)
static __attribute__((interrupt("IRQ"))) void CSPI1_HANDLER(void)
{
    spi_interrupt(&spi_descs[CSPI1_NUM]);
}
#endif

#if (SPI_MODULE_MASK & USE_CSPI2_MODULE)
static __attribute__((interrupt("IRQ"))) void CSPI2_HANDLER(void)
{
    spi_interrupt(&spi_descs[CSPI2_NUM]);
}
#endif

#if (SPI_MODULE_MASK & USE_CSPI3_MODULE)
static __attribute__((interrupt("IRQ"))) void CSPI3_HANDLER(void)
{
    spi_interrupt(&spi_descs[CSPI3_NUM]);
}
#endif

/* Initialize the SPI driver */
void INIT_ATTR spi_init(void)
{
    for (int i = 0; i < SPI_NUM_CSPI; i++)
    {
        struct spi_module_desc * const desc = &spi_descs[i];
        ccm_module_clock_gating(desc->cg, CGM_ON_RUN_WAIT);
        spi_reset(desc);
        ccm_module_clock_gating(desc->cg, CGM_OFF);
    }
}

/* Enable or disable the node - modules will be switch on/off accordingly. */
void spi_enable_node(const struct spi_node *node, bool enable)
{
    struct spi_module_desc * const desc = &spi_descs[node->num];

    if (enable)
    {
        if (++desc->enable == 1)
        {
            /* Enable clock-gating register */
            ccm_module_clock_gating(desc->cg, CGM_ON_RUN_WAIT);
            /* Reset */
            spi_reset(desc);
            desc->last_node = NULL;
            /* Enable interrupt at controller level */
            avic_enable_int(desc->ints, INT_TYPE_IRQ, INT_PRIO_DEFAULT,
                            desc->handler);
        }
    }
    else
    {
        if (desc->enable > 0 && --desc->enable == 0)
        {
            /* Last enable for this module */
            /* Wait for outstanding transactions */
            while (*(void ** volatile)&desc->head != NULL);

            /* Disable interrupt at controller level */
            avic_disable_int(desc->ints);

            /* Disable interface */
            desc->base[CONREG] &= ~CSPI_CONREG_EN;

            /* Disable interface clock */
            ccm_module_clock_gating(desc->cg, CGM_OFF);
        }
    }
}

/* Send and/or receive data on the specified node */
bool spi_transfer(struct spi_transfer_desc *xfer)
{
    if (xfer->count == 0)
        return true;  /* No data? No problem. */

    if (xfer->count < 0 || xfer->next != NULL || xfer->node == NULL)
    {
        /* Can't pass a busy descriptor, requires a node and negative size
         * is invalid to pass. */
        return false;
    }

    bool retval = true;
    unsigned long cpsr = disable_irq_save();
    struct spi_module_desc * const desc = &spi_descs[xfer->node->num];

    if (desc->head == NULL)
    {
        /* No transfers in progress; start interface. */
        retval = start_transfer(desc, xfer);

        if (retval)
        {
            /* Start ok: actually put it in the queue. */
            desc->head = xfer;
            desc->tail = xfer;
            xfer->next = xfer; /* First, self-reference terminate */
        }
    }
    else
    {
        /* Already running: simply add to end and the final INT on the
         * running transfer will pick it up. */
        desc->tail->next = xfer; /* Add to tail */
        desc->tail       = xfer; /* New tail */
        xfer->next       = xfer; /* Self-reference terminate */
    }

    restore_irq(cpsr);

    return retval;
}

/* Returns 'true' if the descriptor is not busy */
bool spi_transfer_complete(const struct spi_transfer_desc *xfer)
{
    return xfer->next == NULL;
}
