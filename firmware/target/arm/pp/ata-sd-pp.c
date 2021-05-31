/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Daniel Ankers
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
#include "config.h" /* for HAVE_MULTIDRIVE */
#include "sdmmc.h"
#include "gcc_extensions.h"
#ifdef HAVE_HOTSWAP
#include "sd-pp-target.h"
#endif
#include "system.h"
#include <string.h>
#include "led.h"
#include "cpu.h"
#include "storage.h"
#include "fs_defines.h"

#define BLOCKS_PER_BANK 0x7a7800

/* Comparing documentations of various MMC/SD controllers revealed, */
/* that this controller seems to be a mix of PXA27x, PXA255 and */
/* some PP specific stuff. The register and bit definitions are */
/* taken from the 'PXA27x Developers Manual', as it appears to be */
/* the closest match. Known differences and obscurities are commented.*/

#define MMC_STRPCL      (*(volatile unsigned int *)(0x70008200))
#define MMC_STAT        (*(volatile unsigned int *)(0x70008204))
#define MMC_CLKRT       (*(volatile unsigned int *)(0x70008208))
#define MMC_SPI         (*(volatile unsigned int *)(0x7000820c))
#define MMC_CMDAT       (*(volatile unsigned int *)(0x70008210))
#define MMC_RESTO       (*(volatile unsigned int *)(0x70008214))
#define MMC_RDTO        (*(volatile unsigned int *)(0x70008218))
#define MMC_BLKLEN      (*(volatile unsigned int *)(0x7000821c))
#define MMC_NUMBLK      (*(volatile unsigned int *)(0x70008220))
#define MMC_I_MASK      (*(volatile unsigned int *)(0x70008224))
#define MMC_CMD         (*(volatile unsigned int *)(0x70008228))
#define MMC_ARGH        (*(volatile unsigned int *)(0x7000822c))
#define MMC_ARGL        (*(volatile unsigned int *)(0x70008230))
#define MMC_RES         (*(volatile unsigned int *)(0x70008234))

/* PXA255/27x have separate RX/TX FIFOs with 32x8 bit */
/* PP502x has a combined Data FIFO with 16x16 bit */
#define MMC_DATA_FIFO   (*(volatile unsigned int *)(0x70008280))

/* PP specific registers, no other controller seem to have such. */
#define MMC_SD_STATE    (*(volatile unsigned int *)(0x70008238))
#define MMC_INIT_1      (*(volatile unsigned int *)(0x70008240))
#define MMC_INIT_2      (*(volatile unsigned int *)(0x70008244))

/* MMC_STAT bits */
#define STAT_SDIO_SUSPEND_ACK (1 << 16)
#define STAT_SDIO_INT         (1 << 15)
#define STAT_RD_STALLED       (1 << 14)
#define STAT_END_CMD_RES      (1 << 13)
#define STAT_PRG_DONE         (1 << 12)
#define STAT_DATA_TRAN_DONE   (1 << 11)
#define STAT_SPI_WR_ERR       (1 << 10)
#define STAT_FLASH_ERR        (1 << 9)
#define STAT_CLK_EN           (1 << 8)
#define STAT_RECV_FIFO_FULL   (1 << 7)  /* taken from PXA255 */
#define STAT_XMIT_FIFO_EMPTY  (1 << 6)  /* taken from PXA255 */
#define STAT_RES_CRC_ERR      (1 << 5)
#define STAT_DAT_ERR_TOKEN    (1 << 4)
#define STAT_CRC_RD_ERR       (1 << 3)
#define STAT_CRC_WR_ERR       (1 << 2)
#define STAT_TIME_OUT_RES     (1 << 1)
#define STAT_TIME_OUT_READ    (1)
#define STAT_ERROR_BITS       (0x3f)
 
/* MMC_CMDAT bits */
/* Some of the bits used by the OF don't make much sense with these */
/* definitions. So they're probably different between PXA and PP502x */
/* Bits 0-5 appear to match though. */
#define CMDAT_SDIO_RESUME  (1 << 13)
#define CMDAT_SDIO_SUSPEND (1 << 12)
#define CMDAT_SDIO_INT_EN  (1 << 11)
#define CMDAT_STOP_TRAN    (1 << 10)
#define CMDAT_SD_4DAT      (1 << 8)
#define CMDAT_DMA_EN       (1 << 7)
#define CMDAT_INIT         (1 << 6)
#define CMDAT_BUSY         (1 << 5)
#define CMDAT_STRM_BLK     (1 << 4)
#define CMDAT_WR_RD        (1 << 3)
#define CMDAT_DATA_EN      (1 << 2)
#define CMDAT_RES_TYPE3    (3)
#define CMDAT_RES_TYPE2    (2)
#define CMDAT_RES_TYPE1    (1)
 
/* MMC_I_MASK bits */
/* PP502x apparently only has bits 0-3 */
#define I_MASK_SDIO_SUSPEND_ACK  (1 << 12)
#define I_MASK_SDIO_INT          (1 << 11)
#define I_MASK_RD_STALLED        (1 << 10)
#define I_MASK_RES_ERR           (1 << 9)
#define I_MASK_DAT_ERR           (1 << 8)
#define I_MASK_TINT              (1 << 7)
#define I_MASK_TXFIFO_WR_REQ     (1 << 6)
#define I_MASK_RXFIFO_RD_REQ     (1 << 5)
#define I_MASK_CLK_IS_OFF        (1 << 4)
#define I_MASK_STOP_CMD          (1 << 3)
#define I_MASK_END_CMD_RES       (1 << 2)
#define I_MASK_PRG_DONE          (1 << 1)
#define I_MASK_DATA_TRAN_DONE    (1 << 0)

#define FIFO_LEN        16          /* FIFO is 16 words deep */

#define EC_OK                    0
#define EC_FAILED                1
#define EC_NOCARD                2
#define EC_WAIT_STATE_FAILED     3
#define EC_CHECK_TIMEOUT_FAILED  4
#define EC_POWER_UP              5
#define EC_READ_TIMEOUT          6
#define EC_WRITE_TIMEOUT         7
#define EC_TRAN_SEL_BANK         8
#define EC_TRAN_READ_ENTRY       9
#define EC_TRAN_READ_EXIT       10
#define EC_TRAN_WRITE_ENTRY     11
#define EC_TRAN_WRITE_EXIT      12
#define EC_FIFO_SEL_BANK_EMPTY  13
#define EC_FIFO_SEL_BANK_DONE   14
#define EC_FIFO_ENA_BANK_EMPTY  15
#define EC_FIFO_READ_FULL       16
#define EC_FIFO_WR_EMPTY        17
#define EC_FIFO_WR_DONE         18
#define EC_COMMAND              19
#define NUM_EC                  20

/* for compatibility */
static long last_disk_activity = -1;

static long next_yield = 0;
#define MIN_YIELD_PERIOD 1000

static tCardInfo card_info[2];
static tCardInfo *currcard = NULL; /* current active card */

struct sd_card_status
{
    int retry;
    int retry_max;
};

static struct sd_card_status sd_status[NUM_DRIVES] =
{
    { 0, 1  },
#ifdef HAVE_MULTIDRIVE
    { 0, 10 }
#endif
};

static struct mutex sd_mtx SHAREDBSS_ATTR;

#ifdef HAVE_HOTSWAP
static int sd_first_drive = 0;
#endif

/* Private Functions */

static unsigned int check_time[NUM_EC];

static inline void enable_controller(bool on)
{
    if(on)
    {
        DEV_EN |= DEV_ATA; /* Enable controller */
    }
    else
    {
        DEV_EN &= ~DEV_ATA; /* Disable controller */
    }
}

static inline bool sd_check_timeout(long timeout, int id)
{
    return !TIME_AFTER(USEC_TIMER, check_time[id] + timeout);
}

static bool sd_poll_status(unsigned int trigger, long timeout)
{
    long t = USEC_TIMER;

    while ((MMC_STAT & trigger) == 0)
    {
        long time = USEC_TIMER;

        if (TIME_AFTER(time, next_yield))
        {
            long ty = USEC_TIMER;
            yield();
            timeout += USEC_TIMER - ty;
            next_yield = ty + MIN_YIELD_PERIOD;
        }

        if (TIME_AFTER(time, t + timeout))
            return false;
    }

    return true;
}

static int sd_command(unsigned int cmd, unsigned long arg1,
                      unsigned long *response, unsigned int cmdat)
{
    int i, words; /* Number of 16 bit words to read from MMC_RES */
    unsigned int data[9];

    MMC_CMD = cmd;
    MMC_ARGH = (unsigned int)((arg1 & 0xffff0000) >> 16);
    MMC_ARGL = (unsigned int)((arg1 & 0xffff));
    MMC_CMDAT  = cmdat;

    if (!sd_poll_status(STAT_END_CMD_RES, 100000))
        return -EC_COMMAND;

    if ((MMC_STAT & STAT_ERROR_BITS) != 0)
        /* Error sending command */
        return -EC_COMMAND - (MMC_STAT & STAT_ERROR_BITS)*100;

    if (cmd == SD_GO_IDLE_STATE)
        return 0; /* no response here */

    words = (cmdat == CMDAT_RES_TYPE2) ? 9 : 3;

    for (i = 0; i < words; i++) /* MMC_RES is read MSB first */
        data[i] = MMC_RES; /* Read most significant 16-bit word */

    if (response == NULL)
    {
        /* response discarded */
    }
    else if (cmdat == CMDAT_RES_TYPE2)
    {
        /* Response type 2 has the following structure:
         * [135:135] Start Bit - '0'
         * [134:134] Transmission bit - '0'
         * [133:128] Reserved - '111111'
         * [127:001] CID or CSD register including internal CRC7
         * [000:000] End Bit - '1'
         */
        response[3] = (data[0]<<24) + (data[1]<<8) + (data[2]>>8);
        response[2] = (data[2]<<24) + (data[3]<<8) + (data[4]>>8);
        response[1] = (data[4]<<24) + (data[5]<<8) + (data[6]>>8);
        response[0] = (data[6]<<24) + (data[7]<<8) + (data[8]>>8);
    }
    else
    {
        /* Response types 1, 1b, 3, 6, 7 have the following structure:
         * Types 4 and 5 are not supported.
         *
         *     [47] Start bit - '0'
         *     [46] Transmission bit - '0'
         *  [45:40] R1, R1b, R6, R7: Command index
         *          R3: Reserved - '111111'
         *   [39:8] R1, R1b: Card Status
         *          R3: OCR Register
         *          R6: [31:16] RCA
         *              [15: 0] Card Status Bits 23, 22, 19, 12:0
         *                     [23] COM_CRC_ERROR
         *                     [22] ILLEGAL_COMMAND
         *                     [19] ERROR
         *                   [12:9] CURRENT_STATE
         *                      [8] READY_FOR_DATA
         *                    [7:6]
         *                      [5] SD_APP_CMD
         *                      [4]
         *                      [3] AKE_SEQ_ERROR
         *                      [2] Reserved
         *                    [1:0] Reserved for test mode
         *          R7: [19:16] Voltage accepted
         *              [15:8]  echo-back of check pattern
         *    [7:1] R1, R1b: CRC7
         *          R3: Reserved - '1111111'
         *      [0] End Bit - '1'
         */
        response[0] = (data[0]<<24) + (data[1]<<8) + (data[2]>>8);
    }

    return 0;
}

static int sd_wait_for_state(unsigned int state, int id)
{
    unsigned long response = 0;
    unsigned int timeout = 0x80000;

    check_time[id] = USEC_TIMER;

    while (1)
    {
        int ret = sd_command(SD_SEND_STATUS, currcard->rca, &response, CMDAT_RES_TYPE1);
        long us;

        if (ret < 0)
            return ret*100 - id;

        if (((response >> 9) & 0xf) == state)
        {
            MMC_SD_STATE = state;
            return 0;
        }

        if (!sd_check_timeout(timeout, id))
            return -EC_WAIT_STATE_FAILED*100 - id;

        us = USEC_TIMER;
        if (TIME_AFTER(us, next_yield))
        {
            yield();
            timeout += USEC_TIMER - us;
            next_yield = us + MIN_YIELD_PERIOD;
        }
    }
}


static inline bool card_detect_target(void)
{
#ifdef HAVE_HOTSWAP
#ifdef SANSA_E200
    return (GPIOA_INPUT_VAL & 0x80) == 0; /* low active */
#elif defined SANSA_C200
    return (GPIOL_INPUT_VAL & 0x08) != 0; /* high active */
#endif
#else
    return false;
#endif
}


static inline void copy_read_sectors_fast(unsigned char **buf)
{
    /* Copy one chunk of 16 words using best method for start alignment */
    switch ( (intptr_t)*buf & 3 )
    {
    case 0:
        asm volatile (
            "ldmia  %[data], { r2-r9 }          \r\n"
            "orr    r2, r2, r3, lsl #16         \r\n"
            "orr    r4, r4, r5, lsl #16         \r\n"
            "orr    r6, r6, r7, lsl #16         \r\n"
            "orr    r8, r8, r9, lsl #16         \r\n"
            "stmia  %[buf]!, { r2, r4, r6, r8 } \r\n"
            "ldmia  %[data], { r2-r9 }          \r\n"
            "orr    r2, r2, r3, lsl #16         \r\n"
            "orr    r4, r4, r5, lsl #16         \r\n"
            "orr    r6, r6, r7, lsl #16         \r\n"
            "orr    r8, r8, r9, lsl #16         \r\n"
            "stmia  %[buf]!, { r2, r4, r6, r8 } \r\n"
            : [buf]"+&r"(*buf)
            : [data]"r"(&MMC_DATA_FIFO)
            : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9"
        );
        break;
    case 1:
        asm volatile (
            "ldmia  %[data], { r2-r9 }          \r\n"
            "orr    r3, r2, r3, lsl #16         \r\n"
            "strb   r3, [%[buf]], #1            \r\n"
            "mov    r3, r3, lsr #8              \r\n"
            "strh   r3, [%[buf]], #2            \r\n"
            "mov    r3, r3, lsr #16             \r\n"
            "orr    r3, r3, r4, lsl #8          \r\n"
            "orr    r3, r3, r5, lsl #24         \r\n"
            "mov    r5, r5, lsr #8              \r\n"
            "orr    r5, r5, r6, lsl #8          \r\n"
            "orr    r5, r5, r7, lsl #24         \r\n"
            "mov    r7, r7, lsr #8              \r\n"
            "orr    r7, r7, r8, lsl #8          \r\n"
            "orr    r7, r7, r9, lsl #24         \r\n"
            "mov    r2, r9, lsr #8              \r\n"
            "stmia  %[buf]!, { r3, r5, r7 }     \r\n"
            "ldmia  %[data], { r3-r10 }         \r\n"
            "orr    r2, r2, r3, lsl #8          \r\n"
            "orr    r2, r2, r4, lsl #24         \r\n"
            "mov    r4, r4, lsr #8              \r\n"
            "orr    r4, r4, r5, lsl #8          \r\n"
            "orr    r4, r4, r6, lsl #24         \r\n"
            "mov    r6, r6, lsr #8              \r\n"
            "orr    r6, r6, r7, lsl #8          \r\n"
            "orr    r6, r6, r8, lsl #24         \r\n"
            "mov    r8, r8, lsr #8              \r\n"
            "orr    r8, r8, r9, lsl #8          \r\n"
            "orr    r8, r8, r10, lsl #24        \r\n"
            "mov    r10, r10, lsr #8            \r\n"
            "stmia  %[buf]!, { r2, r4, r6, r8 } \r\n"
            "strb   r10, [%[buf]], #1           \r\n"
            : [buf]"+&r"(*buf)
            : [data]"r"(&MMC_DATA_FIFO)
            : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10"
        );
        break;
    case 2:
        asm volatile (
            "ldmia  %[data], { r2-r9 }          \r\n"
            "strh   r2, [%[buf]], #2            \r\n"
            "orr    r3, r3, r4, lsl #16         \r\n"
            "orr    r5, r5, r6, lsl #16         \r\n"
            "orr    r7, r7, r8, lsl #16         \r\n"
            "stmia  %[buf]!, { r3, r5, r7 }     \r\n"
            "ldmia  %[data], { r2-r8, r10 }     \r\n"
            "orr    r2, r9, r2, lsl #16         \r\n"
            "orr    r3, r3, r4, lsl #16         \r\n"
            "orr    r5, r5, r6, lsl #16         \r\n"
            "orr    r7, r7, r8, lsl #16         \r\n"
            "stmia  %[buf]!, { r2, r3, r5, r7 } \r\n"
            "strh   r10, [%[buf]], #2           \r\n"
            : [buf]"+&r"(*buf)
            : [data]"r"(&MMC_DATA_FIFO)
            : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10"
        );
        break;
    case 3:
        asm volatile (
            "ldmia  %[data], { r2-r9 }          \r\n"
            "orr    r3, r2, r3, lsl #16         \r\n"
            "strb   r3, [%[buf]], #1            \r\n"
            "mov    r3, r3, lsr #8              \r\n"
            "orr    r3, r3, r4, lsl #24         \r\n"
            "mov    r4, r4, lsr #8              \r\n"
            "orr    r5, r4, r5, lsl #8          \r\n"
            "orr    r5, r5, r6, lsl #24         \r\n"
            "mov    r6, r6, lsr #8              \r\n"
            "orr    r7, r6, r7, lsl #8          \r\n"
            "orr    r7, r7, r8, lsl #24         \r\n"
            "mov    r8, r8, lsr #8              \r\n"
            "orr    r2, r8, r9, lsl #8          \r\n"
            "stmia  %[buf]!, { r3, r5, r7 }     \r\n"
            "ldmia  %[data], { r3-r10 }         \r\n"
            "orr    r2, r2, r3, lsl #24         \r\n"
            "mov    r3, r3, lsr #8              \r\n"
            "orr    r4, r3, r4, lsl #8          \r\n"
            "orr    r4, r4, r5, lsl #24         \r\n"
            "mov    r5, r5, lsr #8              \r\n"
            "orr    r6, r5, r6, lsl #8          \r\n"
            "orr    r6, r6, r7, lsl #24         \r\n"
            "mov    r7, r7, lsr #8              \r\n"
            "orr    r8, r7, r8, lsl #8          \r\n"
            "orr    r8, r8, r9, lsl #24         \r\n"
            "mov    r9, r9, lsr #8              \r\n"
            "orr    r10, r9, r10, lsl #8        \r\n"
            "stmia  %[buf]!, { r2, r4, r6, r8 } \r\n"
            "strh   r10, [%[buf]], #2           \r\n"
            "mov    r10, r10, lsr #16           \r\n"
            "strb   r10, [%[buf]], #1           \r\n"
            : [buf]"+&r"(*buf)
            : [data]"r"(&MMC_DATA_FIFO)
            : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10"
        );
        break;
    }
}

static inline void copy_read_sectors_slow(unsigned char** buf)
{
    int cnt = FIFO_LEN;
    int t;

    /* Copy one chunk of 16 words */
    asm volatile (
    "1:                                     \r\n"
        "ldrh   %[t], [%[data]]             \r\n"
        "strb   %[t], [%[buf]], #1          \r\n"
        "mov    %[t], %[t], lsr #8          \r\n"
        "strb   %[t], [%[buf]], #1          \r\n"
        "subs   %[cnt], %[cnt], #1          \r\n"
        "bgt    1b                          \r\n"
        : [cnt]"+&r"(cnt), [buf]"+&r"(*buf),
          [t]"=&r"(t)
        : [data]"r"(&MMC_DATA_FIFO)
    );
}

/* Writes have to be kept slow for now */
static inline void copy_write_sectors(const unsigned char** buf)
{
    int cnt = FIFO_LEN - 1;
    unsigned t;
    long time;

    time = USEC_TIMER + 3;
    if  (((intptr_t)*buf & 3) == 0)
    {
        asm volatile (
            "ldmia  %[buf]!, { r3, r5, r7, r9 } \r\n"
            "mov    r4, r3, lsr #16             \r\n" 
            "mov    r6, r5, lsr #16             \r\n" 
            "mov    r8, r7, lsr #16             \r\n" 
            "mov    r10, r9, lsr #16            \r\n" 
            "stmia  %[data], { r3-r10 }         \r\n"
            "ldmia  %[buf]!, { r3, r5, r7, r9 } \r\n"
            "mov    r4, r3, lsr #16             \r\n" 
            "mov    r6, r5, lsr #16             \r\n" 
            "mov    r8, r7, lsr #16             \r\n" 
            "mov    %[t], r9, lsr #16           \r\n" 
            "stmia  %[data], { r3-r9 }          \r\n"
            : [buf]"+&r"(*buf), [t]"=&r"(t) 
            : [data]"r"(&MMC_DATA_FIFO)
            : "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10"
        );
    }
    else
    {
        do
        {
            t  = *(*buf)++;
            t |= *(*buf)++ << 8;
            MMC_DATA_FIFO = t;
        } while (--cnt > 0); /* tail loop is faster */
        t  = *(*buf)++;
        t |= *(*buf)++ << 8;
    }
    /* Don't write the last word before at least 3 usec have elapsed since FIFO_EMPTY */
    /* This prevents the 'two bytes inserted' bug. */

    while (!TIME_AFTER(USEC_TIMER, time));
    MMC_DATA_FIFO = t;
}

static int sd_select_bank(unsigned char bank)
{
    unsigned char card_data[FIFO_LEN*2];// FIFO_LEN words=FIFO_LEN*2 bytes
    const unsigned char* write_buf;
    int i, ret;

    memset(card_data, 0, sizeof card_data);

    ret = sd_wait_for_state(SD_TRAN, EC_TRAN_SEL_BANK);
    if (ret < 0)
        return ret;

    MMC_BLKLEN = 512;
    MMC_NUMBLK = 1;

    ret = sd_command(35, 0, NULL, /* CMD35 is vendor specific */
                     0x1c00 | CMDAT_WR_RD | CMDAT_DATA_EN | CMDAT_RES_TYPE1);
    if (ret < 0)
        return ret;

    MMC_SD_STATE = SD_PRG;

    card_data[0] = bank;

    /* Write the card data */
    for (i = 0; i < SD_BLOCK_SIZE/2; i += FIFO_LEN)
    {
        write_buf = card_data;
        /* Wait for the FIFO to empty */
        if (sd_poll_status(STAT_XMIT_FIFO_EMPTY, 10000))
        {
            copy_write_sectors(&write_buf); /* Copy one chunk of 16 words */
            /* clear buffer: only the first chunk contains interesting data (bank), the remaining is zero filling */
            memset(card_data, 0, sizeof card_data);
            continue;
        }

        return -EC_FIFO_SEL_BANK_EMPTY;
    }

    if (!sd_poll_status(STAT_PRG_DONE, 10000))
        return -EC_FIFO_SEL_BANK_DONE;

    currcard->current_bank = bank;

    return 0;
}

static void sd_card_mux(int card_no)
{
/* Set the current card mux */
#if defined(SANSA_E200)
    if (card_no == 0)
    {
        GPO32_VAL |= 0x4;

        GPIO_CLEAR_BITWISE(GPIOA_ENABLE, 0x7a);
        GPIO_CLEAR_BITWISE(GPIOA_OUTPUT_EN, 0x7a);
        GPIO_SET_BITWISE(GPIOD_ENABLE,  0x1f);
        GPIO_SET_BITWISE(GPIOD_OUTPUT_VAL, 0x1f);
        GPIO_SET_BITWISE(GPIOD_OUTPUT_EN,  0x1f);

        outl((inl(0x70000014) & ~(0x3ffff)) | 0x255aa, 0x70000014);
    }
    else
    {
        GPO32_VAL &= ~0x4;

        GPIO_CLEAR_BITWISE(GPIOD_ENABLE, 0x1f);
        GPIO_CLEAR_BITWISE(GPIOD_OUTPUT_EN, 0x1f);
        GPIO_SET_BITWISE(GPIOA_ENABLE, 0x7a);
        GPIO_SET_BITWISE(GPIOA_OUTPUT_VAL, 0x7a);
        GPIO_SET_BITWISE( GPIOA_OUTPUT_EN, 0x7a);

        outl(inl(0x70000014) & ~(0x3ffff), 0x70000014);
    }
#elif defined(SANSA_C200)
    if (card_no == 0)
    {
        GPO32_VAL |= 0x4;

        GPIO_CLEAR_BITWISE(GPIOD_ENABLE, 0x1f);
        GPIO_CLEAR_BITWISE(GPIOD_OUTPUT_EN, 0x1f);
        GPIO_SET_BITWISE(GPIOA_ENABLE, 0x7a);
        GPIO_SET_BITWISE(GPIOA_OUTPUT_VAL, 0x7a);
        GPIO_SET_BITWISE( GPIOA_OUTPUT_EN, 0x7a);

        outl(inl(0x70000014) & ~(0x3ffff), 0x70000014);
    }
    else
    {
        GPO32_VAL &= ~0x4;

        GPIO_CLEAR_BITWISE(GPIOA_ENABLE, 0x7a);
        GPIO_CLEAR_BITWISE(GPIOA_OUTPUT_EN, 0x7a);
        GPIO_SET_BITWISE(GPIOD_ENABLE,  0x1f);
        GPIO_SET_BITWISE(GPIOD_OUTPUT_VAL, 0x1f);
        GPIO_SET_BITWISE(GPIOD_OUTPUT_EN,  0x1f);

        outl((inl(0x70000014) & ~(0x3ffff)) | 0x255aa, 0x70000014);
    }
#elif defined(PHILIPS_SA9200)
    /* only 1 "card" (no external memory card) */
    (void)card_no;

    GPIO_SET_BITWISE(GPIOH_ENABLE, 0x80);
    GPIO_SET_BITWISE(GPIOH_OUTPUT_EN, 0x80);

    outl(0x255aa, 0x70000014);

    GPIO_CLEAR_BITWISE(GPIOA_ENABLE, 0x04);
    GPIO_CLEAR_BITWISE(GPIOA_OUTPUT_EN, 0x04);

    GPIO_CLEAR_BITWISE(GPIOA_ENABLE, 0x7a);
    GPIO_CLEAR_BITWISE(GPIOA_OUTPUT_EN, 0x7a);

    GPIO_SET_BITWISE(GPIOH_OUTPUT_VAL, 0x80);
    GPIO_SET_BITWISE(GPIOH_OUTPUT_EN, 0x80);
#endif
}

static void sd_init_device(int card_no)
{
/* SD Protocol registers */
#ifdef HAVE_HOTSWAP
    unsigned long response = 0;
#endif
    unsigned int  i;
    unsigned char carddata[512];
    unsigned char *dataptr;
    unsigned long temp_reg[4];
    int ret;

/* Enable and initialise controller */
    MMC_CLKRT = 6;  /* switch to lowest clock rate */

/* Initialise card data as blank */
    memset(currcard, 0, sizeof(*currcard));

/* Switch card mux to card to initialize */
    sd_card_mux(card_no);

/* Init NAND */
    MMC_INIT_1 |=  (1 << 15);
    MMC_INIT_2 |=  (1 << 15);
    MMC_INIT_2 &= ~(3 << 12);
    MMC_INIT_2 |=  (1 << 13);
    MMC_INIT_1 &= ~(3 << 12);
    MMC_INIT_1 |=  (1 << 13);

    DEV_EN |= DEV_ATA; /* Enable controller */
    DEV_RS |= DEV_ATA; /* Reset controller */
    DEV_RS &=~DEV_ATA; /* Clear Reset */

    MMC_SD_STATE = SD_TRAN;

    MMC_I_MASK = 0xf;  /* disable interrupts */

    ret = sd_command(SD_GO_IDLE_STATE, 0, NULL, 0x100);
    if (ret < 0)
        goto card_init_error;

    check_time[EC_POWER_UP] = USEC_TIMER;

#ifdef HAVE_HOTSWAP
    /* Check for SDHC:
       - non-SDHC cards simply ignore SD_SEND_IF_COND (CMD8) and we get error -219,
         which we can just ignore and assume we're dealing with standard SD.
       - SDHC cards echo back the argument into the response. This is how we
         tell if the card is SDHC.
     */
    ret = sd_command(SD_SEND_IF_COND,0x1aa, &response,
                     CMDAT_DATA_EN | CMDAT_RES_TYPE3);
    if ( (ret < 0) && (ret!=-219) )
            goto card_init_error;
#endif

    while ((currcard->ocr & (1 << 31)) == 0) /* until card is powered up */
    {
        ret = sd_command(SD_APP_CMD, currcard->rca, NULL, CMDAT_RES_TYPE1);
        if (ret < 0)
            goto card_init_error;

#ifdef HAVE_HOTSWAP
        if(response == 0x1aa)
        {
            /* SDHC */
            ret = sd_command(SD_APP_OP_COND, (1<<30)|0x100000,
                             &currcard->ocr, CMDAT_RES_TYPE3);
        }
        else
#endif /* HAVE_HOTSWAP */
        {
            /* SD Standard */
            ret = sd_command(SD_APP_OP_COND, 0x100000, &currcard->ocr,
                             CMDAT_RES_TYPE3);
        }

        if (ret < 0)
            goto card_init_error;

        if (!sd_check_timeout(5000000, EC_POWER_UP))
        {
            ret = -EC_POWER_UP;
            goto card_init_error;
        }
    }

    ret = sd_command(SD_ALL_SEND_CID, 0, temp_reg, CMDAT_RES_TYPE2);
    if (ret < 0)
        goto card_init_error;

    for(i=0; i<4; i++)
        currcard->cid[i] = temp_reg[3-i];

    ret = sd_command(SD_SEND_RELATIVE_ADDR, 0, &currcard->rca, CMDAT_RES_TYPE1);
    if (ret < 0)
        goto card_init_error;

    ret = sd_command(SD_SEND_CSD, currcard->rca, temp_reg, CMDAT_RES_TYPE2);
    if (ret < 0)
        goto card_init_error;

    for(i=0; i<4; i++)
        currcard->csd[i] = temp_reg[3-i];

    sd_parse_csd(currcard);
    
    MMC_CLKRT = 0;  /* switch to highest clock rate */

    ret = sd_command(SD_SELECT_CARD, currcard->rca, NULL,
                     0x80 | CMDAT_RES_TYPE1);
    if (ret < 0)
        goto card_init_error;

    ret = sd_command(SD_APP_CMD, currcard->rca, NULL, CMDAT_RES_TYPE1);
    if (ret < 0)
        goto card_init_error;

    ret = sd_command(SD_SET_BUS_WIDTH, currcard->rca | 2, NULL,
                     CMDAT_RES_TYPE1); /* 4 bit */
    if (ret < 0)
        goto card_init_error;

    ret = sd_command(SD_SET_BLOCKLEN, currcard->blocksize, NULL,
                     CMDAT_RES_TYPE1);
    if (ret < 0)
        goto card_init_error;

    MMC_BLKLEN = currcard->blocksize;

    /* If this card is >4GB & not SDHC, then we need to enable bank switching */
    if( (currcard->numblocks >= BLOCKS_PER_BANK) &&
        ((currcard->ocr & (1<<30)) == 0) )
    {
        MMC_SD_STATE = SD_TRAN;
        MMC_NUMBLK = 1;

        ret = sd_command(SD_SWITCH_FUNC, 0x80ffffef, NULL,
                         0x1c00 | CMDAT_DATA_EN | CMDAT_RES_TYPE1);
        if (ret < 0)
            goto card_init_error;

        /* Read 512 bytes from the card.
        The first 512 bits contain the status information
        TODO: Do something useful with this! */
        dataptr = carddata;
        for (i = 0; i < SD_BLOCK_SIZE/2; i += FIFO_LEN)
        {
            /* Wait for the FIFO to be full */
            if (sd_poll_status(STAT_RECV_FIFO_FULL, 100000))
            {
                copy_read_sectors_slow(&dataptr);
                continue;
            }

            ret = -EC_FIFO_ENA_BANK_EMPTY;
            goto card_init_error;
        }
    }

    currcard->initialized = 1;
    return;

    /* Card failed to initialize so disable it */
card_init_error:
    currcard->initialized = ret;
}

/* lock must already be aquired */
static void sd_select_device(int card_no)
{
    currcard = &card_info[card_no];

    if (card_no == 0)
    {
        /* Main card always gets a chance */
        sd_status[0].retry = 0;
    }

    if (currcard->initialized > 0)
    {
        /* This card is already initialized - switch to it */
        sd_card_mux(card_no);
        return;
    }

    if (currcard->initialized == 0)
    {
        /* Card needs (re)init */
        sd_init_device(card_no);
    }
}

/* API Functions */

int sd_read_sectors(IF_MD(int drive,) unsigned long start, int incount,
                     void* inbuf)
{
#ifndef HAVE_MULTIDRIVE
    const int drive = 0;
#endif
    int ret;
    unsigned char *buf, *buf_end;
    unsigned int bank;
    
    /* TODO: Add DMA support. */

    mutex_lock(&sd_mtx);
    enable_controller(true);
    led(true);

sd_read_retry:
    if (drive != 0 && !card_detect_target())
    {
        /* no external sd-card inserted */
        ret = -EC_NOCARD;
        goto sd_read_error;
    }

    sd_select_device(drive);

    if (currcard->initialized < 0)
    {
        ret = currcard->initialized;
        goto sd_read_error;
    }

    last_disk_activity = current_tick;

    /* Only switch banks with non-SDHC cards */
    if((currcard->ocr & (1<<30))==0)
    {
        bank = start / BLOCKS_PER_BANK;

        if (currcard->current_bank != bank)
        {
            ret = sd_select_bank(bank);
            if (ret < 0)
                goto sd_read_error;
        }
    
        start -= bank * BLOCKS_PER_BANK;
    }

    ret = sd_wait_for_state(SD_TRAN, EC_TRAN_READ_ENTRY);
    if (ret < 0)
        goto sd_read_error;

    MMC_NUMBLK = incount;

#ifdef HAVE_HOTSWAP
    if(currcard->ocr & (1<<30) )
    {
        /* SDHC */
        ret = sd_command(SD_READ_MULTIPLE_BLOCK, start, NULL,
                         0x1c00 | CMDAT_BUSY | CMDAT_DATA_EN | CMDAT_RES_TYPE1);
    }
    else
#endif
    {
        ret = sd_command(SD_READ_MULTIPLE_BLOCK, start * SD_BLOCK_SIZE, NULL,
                         0x1c00 | CMDAT_BUSY | CMDAT_DATA_EN | CMDAT_RES_TYPE1);
    }
    if (ret < 0)
        goto sd_read_error;

    /* TODO: Don't assume SD_BLOCK_SIZE == SECTOR_SIZE */

    buf_end = (unsigned char *)inbuf + incount * currcard->blocksize;
    for (buf = inbuf; buf < buf_end;)
    {
        /* Wait for the FIFO to be full */
        if (sd_poll_status(STAT_RECV_FIFO_FULL, 0x80000))
        {
            copy_read_sectors_fast(&buf); /* Copy one chunk of 16 words */
            /* TODO: Switch bank if necessary */
            continue;
        }

        ret = -EC_FIFO_READ_FULL;
        goto sd_read_error;
    }

    last_disk_activity = current_tick;

    ret = sd_command(SD_STOP_TRANSMISSION, 0, NULL, CMDAT_RES_TYPE1);
    if (ret < 0)
        goto sd_read_error;

    ret = sd_wait_for_state(SD_TRAN, EC_TRAN_READ_EXIT);
    if (ret < 0)
        goto sd_read_error;

    while (1)
    {
        led(false);
        enable_controller(false);
        mutex_unlock(&sd_mtx);

        return ret;

sd_read_error:
        if (sd_status[drive].retry < sd_status[drive].retry_max
            && ret != -EC_NOCARD)
        {
            sd_status[drive].retry++;
            currcard->initialized = 0;
            goto sd_read_retry;
        }
    }
}

int sd_write_sectors(IF_MD(int drive,) unsigned long start, int count,
                      const void* outbuf)
{
/* Write support is not finished yet */
/* TODO: The standard suggests using ACMD23 prior to writing multiple blocks
   to improve performance */
#ifndef HAVE_MULTIDRIVE
    const int drive = 0;
#endif
    int ret;
    const unsigned char *buf, *buf_end;
    unsigned int bank;

    mutex_lock(&sd_mtx);
    enable_controller(true);
    led(true);

sd_write_retry:
    if (drive != 0 && !card_detect_target())
    {
        /* no external sd-card inserted */
        ret = -EC_NOCARD;
        goto sd_write_error;
    }

    sd_select_device(drive);

    if (currcard->initialized < 0)
    {
        ret = currcard->initialized;
        goto sd_write_error;
    }

    /* Only switch banks with non-SDHC cards */
    if((currcard->ocr & (1<<30))==0)
    {
        bank = start / BLOCKS_PER_BANK;

        if (currcard->current_bank != bank)
        {
            ret = sd_select_bank(bank);
            if (ret < 0)
                goto sd_write_error;
        }
    
        start -= bank * BLOCKS_PER_BANK;
    }

    check_time[EC_WRITE_TIMEOUT] = USEC_TIMER;

    ret = sd_wait_for_state(SD_TRAN, EC_TRAN_WRITE_ENTRY);
    if (ret < 0)
        goto sd_write_error;

    MMC_NUMBLK = count;

#ifdef HAVE_HOTSWAP
    if(currcard->ocr & (1<<30) )
    {
        /* SDHC */
        ret = sd_command(SD_WRITE_MULTIPLE_BLOCK, start, NULL,
                         CMDAT_WR_RD | CMDAT_DATA_EN | CMDAT_RES_TYPE1);
    }
    else
#endif
    {
        ret = sd_command(SD_WRITE_MULTIPLE_BLOCK, start*SD_BLOCK_SIZE, NULL,
                         CMDAT_WR_RD | CMDAT_DATA_EN | CMDAT_RES_TYPE1);
    }
    if (ret < 0)
        goto sd_write_error;

    buf_end = outbuf + count * currcard->blocksize - 2*FIFO_LEN;

    for (buf = outbuf; buf <= buf_end;)
    {
        if (buf == buf_end)
        {
            /* Set MMC_SD_STATE to SD_PRG for the last buffer fill */
            MMC_SD_STATE = SD_PRG;
        }

        copy_write_sectors(&buf); /* Copy one chunk of 16 words */
        /* TODO: Switch bank if necessary */

        /* Wait for the FIFO to empty */
        if (!sd_poll_status(STAT_XMIT_FIFO_EMPTY, 0x80000))
        {
            ret = -EC_FIFO_WR_EMPTY;
            goto sd_write_error;
        }
    }

    last_disk_activity = current_tick;

    if (!sd_poll_status(STAT_PRG_DONE, 0x80000))
    {
        ret = -EC_FIFO_WR_DONE;
        goto sd_write_error;
    }

    ret = sd_command(SD_STOP_TRANSMISSION, 0, NULL, CMDAT_RES_TYPE1);
    if (ret < 0)
        goto sd_write_error;

    ret = sd_wait_for_state(SD_TRAN, EC_TRAN_WRITE_EXIT);
    if (ret < 0)
        goto sd_write_error;

    while (1)
    {
        led(false);
        enable_controller(false);
        mutex_unlock(&sd_mtx);

        return ret;

sd_write_error:
        if (sd_status[drive].retry < sd_status[drive].retry_max
            && ret != -EC_NOCARD)
        {
            sd_status[drive].retry++;
            currcard->initialized = 0;
            goto sd_write_retry;
        }
    }
}

void sd_enable(bool on)
{
    mutex_lock(&sd_mtx);
    enable_controller(on);
    mutex_unlock(&sd_mtx);
}

int sd_init(void)
{
    int ret = 0;

    mutex_init(&sd_mtx);

    led(false);

    /* init controller */
#if defined(PHILIPS_SA9200)
    GPIOA_ENABLE = 0x00;
    GPIO_SET_BITWISE(GPIOD_ENABLE, 0x01);
#else
    outl(inl(0x70000088) & ~(0x4), 0x70000088);
    outl(inl(0x7000008c) & ~(0x4), 0x7000008c);
    GPO32_ENABLE |= 0x4;

    GPIO_SET_BITWISE(GPIOG_ENABLE, (0x3 << 5));
    GPIO_SET_BITWISE(GPIOG_OUTPUT_EN, (0x3 << 5));
    GPIO_SET_BITWISE(GPIOG_OUTPUT_VAL, (0x3 << 5));
#endif

#ifdef HAVE_HOTSWAP
    /* enable card detection port - mask interrupt first */
#ifdef SANSA_E200
    GPIO_CLEAR_BITWISE(GPIOA_INT_EN, 0x80);

    GPIO_CLEAR_BITWISE(GPIOA_OUTPUT_EN, 0x80);
    GPIO_SET_BITWISE(GPIOA_ENABLE, 0x80);
#elif defined SANSA_C200
    GPIO_CLEAR_BITWISE(GPIOL_INT_EN, 0x08);

    GPIO_CLEAR_BITWISE(GPIOL_OUTPUT_EN, 0x08);
    GPIO_SET_BITWISE(GPIOL_ENABLE, 0x08);
#endif
#endif
    sd_select_device(0);

    if (currcard->initialized < 0)
        ret = currcard->initialized;

    /* enable interupt for the mSD card */
    sleep(HZ/10);
#ifdef HAVE_HOTSWAP
#ifdef SANSA_E200
    CPU_INT_EN = HI_MASK;
    CPU_HI_INT_EN = GPIO0_MASK;

    GPIOA_INT_LEV = (0x80 << 8) | (~GPIOA_INPUT_VAL & 0x80);

    GPIOA_INT_CLR = 0x80;

    /* enable the card detect interrupt */
    GPIO_SET_BITWISE(GPIOA_INT_EN, 0x80);
#elif defined SANSA_C200
    CPU_INT_EN = HI_MASK;
    CPU_HI_INT_EN = GPIO2_MASK;

    GPIOL_INT_LEV = (0x08 << 8) | (~GPIOL_INPUT_VAL & 0x08);

    GPIOL_INT_CLR = 0x08;

    /* enable the card detect interrupt */
    GPIO_SET_BITWISE(GPIOL_INT_EN, 0x08);
#endif
#endif /* HAVE_HOTSWAP */

    mutex_unlock(&sd_mtx);

    return ret;
}

tCardInfo *card_get_info_target(int card_no)
{
    return &card_info[card_no];
}

#ifdef HAVE_HOTSWAP
static int sd1_oneshot_callback(struct timeout *tmo)
{
    /* This is called only if the state was stable for 300ms - check state
     * and post appropriate event. */
    queue_broadcast(card_detect_target() ? SYS_HOTSWAP_INSERTED :
                                           SYS_HOTSWAP_EXTRACTED,
                    sd_first_drive+1);
    return 0;
    (void)tmo;
}

/* called on insertion/removal interrupt */
void microsd_int(void)
{
    static struct timeout sd1_oneshot;

#ifdef SANSA_E200
    GPIO_CLEAR_BITWISE(GPIOA_INT_EN, 0x80);
    GPIOA_INT_LEV = (0x80 << 8) | (~GPIOA_INPUT_VAL & 0x80);
    GPIOA_INT_CLR = 0x80;
    GPIO_SET_BITWISE(GPIOA_INT_EN, 0x80);

#elif defined SANSA_C200
    GPIO_CLEAR_BITWISE(GPIOL_INT_EN, 0x08);
    GPIOL_INT_LEV = (0x08 << 8) | (~GPIOL_INPUT_VAL & 0x08);
    GPIOL_INT_CLR = 0x08;
    GPIO_SET_BITWISE(GPIOL_INT_EN, 0x08);
#endif
    timeout_register(&sd1_oneshot, sd1_oneshot_callback, (3*HZ/10), 0);
}
#endif /* HAVE_HOTSWAP */

long sd_last_disk_activity(void)
{
    return last_disk_activity;
}

#ifdef HAVE_HOTSWAP
bool sd_removable(IF_MD_NONVOID(int drive))
{
#ifndef HAVE_MULTIDRIVE
    const int drive=0;
#endif
    return (drive==1);
}

bool sd_present(IF_MD_NONVOID(int drive))
{
#ifndef HAVE_MULTIDRIVE
    const int drive=0;
#endif
    if(drive==0)
    {
        return true;
    }
    else
    {
        return card_detect_target();
    }
}
#endif

#ifdef CONFIG_STORAGE_MULTI
int sd_num_drives(int first_drive)
{
#ifdef HAVE_HOTSWAP
    /* Store which logical drive number(s) we have been assigned */
    sd_first_drive = first_drive;
#else
    (void)first_drive;
#endif
    
#ifdef HAVE_MULTIDRIVE
    return 2;
#else
    return 1;
#endif
}
#endif

int sd_event(long id, intptr_t data)
{
    int rc = 0;

    switch (id)
    {
#ifdef HAVE_HOTSWAP
    case SYS_HOTSWAP_INSERTED:
    case SYS_HOTSWAP_EXTRACTED:
        mutex_lock(&sd_mtx); /* lock-out card activity */

        /* Force card init for new card, re-init for re-inserted one or
         * clear if the last attempt to init failed with an error. */
        card_info[data].initialized = 0;
        sd_status[data].retry = 0;

        /* Access is now safe */
        mutex_unlock(&sd_mtx);
        break;
#endif /* HAVE_HOTSWAP */

    case Q_STORAGE_TICK:
        /* never let a timer wrap confuse us */
        next_yield = USEC_TIMER;
    default:
        rc = storage_event_default_handler(id, data, last_disk_activity,
                                           STORAGE_SD);
        break;
    }

    return rc;
}
