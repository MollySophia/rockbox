/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008 by Maurus Cuelenaere
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
#include "cpu.h"
#include "nand.h"
#include "nand_id.h"
#include "system.h"
#include "panic.h"
#include "kernel.h"
#include "storage.h"
#include "string.h"
/*#define LOGF_ENABLE*/
#include "logf.h"

//#define USE_DMA
//#define USE_ECC

/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0         0
#define NAND_CMD_READ1         1
#define NAND_CMD_RNDOUT        5
#define NAND_CMD_PAGEPROG      0x10
#define NAND_CMD_READOOB       0x50
#define NAND_CMD_ERASE1        0x60
#define NAND_CMD_STATUS        0x70
#define NAND_CMD_STATUS_MULTI  0x71
#define NAND_CMD_SEQIN         0x80
#define NAND_CMD_RNDIN         0x85
#define NAND_CMD_READID        0x90
#define NAND_CMD_ERASE2        0xd0
#define NAND_CMD_RESET         0xff

/* Extended commands for large page devices */
#define NAND_CMD_READSTART     0x30
#define NAND_CMD_RNDOUTSTART   0xE0
#define NAND_CMD_CACHEDPROG    0x15

/* Status bits */
#define NAND_STATUS_FAIL       0x01
#define NAND_STATUS_FAIL_N1    0x02
#define NAND_STATUS_TRUE_READY 0x20
#define NAND_STATUS_READY      0x40
#define NAND_STATUS_WP         0x80

/*
 * NAND parameter struct
 */
struct nand_param
{
    unsigned int bus_width;        /* data bus width: 8-bit/16-bit */
    unsigned int row_cycle;        /* row address cycles: 2/3 */
    unsigned int page_size;        /* page size in bytes: 512/2048/4096 */
    unsigned int oob_size;         /* oob size in bytes: 16/64/128 */
    unsigned int page_per_block;   /* pages per block: 32/64/128 */
};

/*
 * jz4740_nand.c
 *
 * NAND read routine for JZ4740
 *
 * Copyright (c) 2005-2008 Ingenic Semiconductor Inc.
 *
 */

static volatile unsigned long nand_address;
#define NAND_DATAPORT      (nand_address)
#define NAND_ADDRPORT      (nand_address+0x10000)
#define NAND_COMMPORT      (nand_address+0x08000)

#define ECC_BLOCK          512
#define ECC_POS            6
#define PAR_SIZE           9

#define __nand_cmd(n)      (REG8(NAND_COMMPORT) = (n))
#define __nand_addr(n)     (REG8(NAND_ADDRPORT) = (n))
#define __nand_data8()     (REG8(NAND_DATAPORT))
#define __nand_data16()    (REG16(NAND_DATAPORT))

#define __nand_ecc_rs_encoding() \
    (REG_EMC_NFECR = EMC_NFECR_ECCE | EMC_NFECR_ERST | EMC_NFECR_RS | EMC_NFECR_RS_ENCODING)
#define __nand_ecc_rs_decoding() \
    (REG_EMC_NFECR = EMC_NFECR_ECCE | EMC_NFECR_ERST | EMC_NFECR_RS | EMC_NFECR_RS_DECODING)
#define __nand_ecc_disable()     (REG_EMC_NFECR &= ~EMC_NFECR_ECCE)
#define __nand_ecc_encode_sync() while (!(REG_EMC_NFINTS & EMC_NFINTS_ENCF))
#define __nand_ecc_decode_sync() while (!(REG_EMC_NFINTS & EMC_NFINTS_DECF))

/*--------------------------------------------------------------*/

static struct nand_info* chip_info = NULL;
static struct nand_info* banks[4];
static unsigned int nr_banks = 1;
static unsigned long bank_size;
static struct nand_param internal_param;
static struct mutex nand_mtx;
#ifdef USE_DMA
static struct mutex nand_dma_mtx;
static struct semaphore nand_dma_complete;
#endif
static unsigned char temp_page[4096]; /* Max page size */

static inline void jz_nand_wait_ready(void)
{
    register unsigned int timeout = 1000;
    while ((REG_GPIO_PXPIN(2) & 0x40000000) && timeout--);
    while (!(REG_GPIO_PXPIN(2) & 0x40000000));
}

#ifndef USE_DMA
static inline void jz_nand_read_buf16(void *buf, int count)
{
    register int i;
    register unsigned short *p = (unsigned short *)buf;

    for (i = 0; i < count; i += 2)
        *p++ = __nand_data16();
}

static inline void jz_nand_read_buf8(void *buf, int count)
{
    register int i;
    register unsigned char *p = (unsigned char *)buf;

    for (i = 0; i < count; i++)
        *p++ = __nand_data8();
}
#else
static void jz_nand_write_dma(void *source, unsigned int len, int bw)
{
    mutex_lock(&nand_dma_mtx);

    if(((unsigned int)source < 0xa0000000) && len)
         commit_discard_dcache_range(source, len);

    dma_enable();

    REG_DMAC_DCCSR(DMA_NAND_CHANNEL) = DMAC_DCCSR_NDES;
    REG_DMAC_DSAR(DMA_NAND_CHANNEL) = PHYSADDR((unsigned long)source);
    REG_DMAC_DTAR(DMA_NAND_CHANNEL) = PHYSADDR((unsigned long)NAND_DATAPORT);
    REG_DMAC_DTCR(DMA_NAND_CHANNEL) = len / 16;
    REG_DMAC_DRSR(DMA_NAND_CHANNEL) = DMAC_DRSR_RS_AUTO;
    REG_DMAC_DCMD(DMA_NAND_CHANNEL) = (DMAC_DCMD_SAI| DMAC_DCMD_DAI | DMAC_DCMD_SWDH_32 | DMAC_DCMD_DS_16BYTE |
                                       (bw == 8 ? DMAC_DCMD_DWDH_8 : DMAC_DCMD_DWDH_16));

    REG_DMAC_DCCSR(DMA_NAND_CHANNEL) |= DMAC_DCCSR_EN; /* Enable DMA channel */
#if 1
    while( REG_DMAC_DTCR(DMA_NAND_CHANNEL) )
        yield();
#else
    REG_DMAC_DCMD(DMA_NAND_CHANNEL) |= DMAC_DCMD_TIE;  /* Enable DMA interrupt */
    semaphore_wait(&nand_dma_complete, TIMEOUT_BLOCK);
#endif

    REG_DMAC_DCCSR(DMA_NAND_CHANNEL) &= ~DMAC_DCCSR_EN; /* Disable DMA channel */

    dma_disable();

    mutex_unlock(&nand_dma_mtx);
}

static void jz_nand_read_dma(void *target, unsigned int len, int bw)
{
    mutex_lock(&nand_dma_mtx);

    if(((unsigned int)target < 0xa0000000) && len)
        discard_dcache_range(target, len);

    dma_enable();

    REG_DMAC_DCCSR(DMA_NAND_CHANNEL) = DMAC_DCCSR_NDES ;
    REG_DMAC_DSAR(DMA_NAND_CHANNEL) = PHYSADDR((unsigned long)NAND_DATAPORT);
    REG_DMAC_DTAR(DMA_NAND_CHANNEL) = PHYSADDR((unsigned long)target);
    REG_DMAC_DTCR(DMA_NAND_CHANNEL) = len / 4;
    REG_DMAC_DRSR(DMA_NAND_CHANNEL) = DMAC_DRSR_RS_AUTO;
    REG_DMAC_DCMD(DMA_NAND_CHANNEL) = (DMAC_DCMD_SAI| DMAC_DCMD_DAI | DMAC_DCMD_DWDH_32 | DMAC_DCMD_DS_32BIT |
                                       (bw == 8 ? DMAC_DCMD_SWDH_8 : DMAC_DCMD_SWDH_16));
    REG_DMAC_DCCSR(DMA_NAND_CHANNEL) |= DMAC_DCCSR_EN; /* Enable DMA channel */
#if 1
    while( REG_DMAC_DTCR(DMA_NAND_CHANNEL) )
        yield();
#else
    REG_DMAC_DCMD(DMA_NAND_CHANNEL) |= DMAC_DCMD_TIE;  /* Enable DMA interrupt */
    semaphore_wait(&nand_dma_complete, TIMEOUT_BLOCK);
#endif

    //REG_DMAC_DCCSR(DMA_NAND_CHANNEL) &= ~DMAC_DCCSR_EN; /* Disable DMA channel */

    dma_disable();

    mutex_unlock(&nand_dma_mtx);
}

void DMA_CALLBACK(DMA_NAND_CHANNEL)(void)
{
    if (REG_DMAC_DCCSR(DMA_NAND_CHANNEL) & DMAC_DCCSR_HLT)
        REG_DMAC_DCCSR(DMA_NAND_CHANNEL) &= ~DMAC_DCCSR_HLT;

    if (REG_DMAC_DCCSR(DMA_NAND_CHANNEL) & DMAC_DCCSR_AR)
        REG_DMAC_DCCSR(DMA_NAND_CHANNEL) &= ~DMAC_DCCSR_AR;

    if (REG_DMAC_DCCSR(DMA_NAND_CHANNEL) & DMAC_DCCSR_CT)
        REG_DMAC_DCCSR(DMA_NAND_CHANNEL) &= ~DMAC_DCCSR_CT;

    if (REG_DMAC_DCCSR(DMA_NAND_CHANNEL) & DMAC_DCCSR_TT)
        REG_DMAC_DCCSR(DMA_NAND_CHANNEL) &= ~DMAC_DCCSR_TT;

    semaphore_release(&nand_dma_complete);
}
#endif /* USE_DMA */

static inline void jz_nand_read_buf(void *buf, int count, int bw)
{
#ifdef USE_DMA
    if (bw == 8)
        jz_nand_read_dma(buf, count, 8);
    else
        jz_nand_read_dma(buf, count, 16);
#else
    if (bw == 8)
        jz_nand_read_buf8(buf, count);
    else
        jz_nand_read_buf16(buf, count);
#endif
}

#ifdef USE_ECC
/*
 * Correct 1~9-bit errors in 512-bytes data
 */
static void jz_rs_correct(unsigned char *dat, int idx, int mask)
{
    int i, j;
    unsigned short d, d1, dm;

    i = (idx * 9) >> 3;
    j = (idx * 9) & 0x7;

    i = (j == 0) ? (i - 1) : i;
    j = (j == 0) ? 7 : (j - 1);

    if (i > 512)
        return;

    if (i == 512)
        d = dat[i - 1];
    else
        d = (dat[i] << 8) | dat[i - 1];

    d1 = (d >> j) & 0x1ff;
    d1 ^= mask;

    dm = ~(0x1ff << j);
    d = (d & dm) | (d1 << j);

    dat[i - 1] = d & 0xff;
    if (i < 512)
        dat[i] = (d >> 8) & 0xff;
}
#endif

/*
 * Read oob
 */
static int jz_nand_read_oob(unsigned long page_addr, unsigned char *buf, int size)
{
    struct nand_param *nandp = &internal_param;
    int page_size, row_cycle, bus_width;
    int col_addr;

    page_size = nandp->page_size;
    row_cycle = nandp->row_cycle;
    bus_width = nandp->bus_width;

    if (page_size >= 2048)
        col_addr = page_size;
    else
        col_addr = 0;

    if (page_size >= 2048)
        /* Send READ0 command */
        __nand_cmd(NAND_CMD_READ0);
    else
        /* Send READOOB command */
        __nand_cmd(NAND_CMD_READOOB);

    /* Send column address */
    __nand_addr(col_addr & 0xff);
    if (page_size >= 2048)
        __nand_addr((col_addr >> 8) & 0xff);

    /* Send page address */
    __nand_addr(page_addr & 0xff);
    __nand_addr((page_addr >> 8) & 0xff);
    if (row_cycle == 3)
        __nand_addr((page_addr >> 16) & 0xff);

    /* Send READSTART command for 2048 ps NAND */
    if (page_size >= 2048)
        __nand_cmd(NAND_CMD_READSTART);

    /* Wait for device ready */
    jz_nand_wait_ready();

    /* Read oob data */
    jz_nand_read_buf(buf, size, bus_width);

    return 0;
}


/*
 * nand_read_page()
 *
 * Input:
 *
 *    block - block number: 0, 1, 2, ...
 *    page - page number within a block: 0, 1, 2, ...
 *    dst - pointer to target buffer
 */
static int jz_nand_read_page(unsigned long page_addr, unsigned char *dst)
{
    struct nand_param *nandp = &internal_param;
    int page_size, oob_size;
    int row_cycle, bus_width, ecc_count;
    int i;
#ifdef USE_ECC
    int j;
#endif
    unsigned char *data_buf;
    unsigned char oob_buf[nandp->oob_size];

    if(nand_address == 0)
        return -1;

    page_size = nandp->page_size;
    oob_size = nandp->oob_size;
    row_cycle = nandp->row_cycle;
    bus_width = nandp->bus_width;

    /*
     * Read oob data
     */
    jz_nand_read_oob(page_addr, oob_buf, oob_size);

    /*
     * Read page data
     */

    /* Send READ0 command */
    __nand_cmd(NAND_CMD_READ0);

    /* Send column address */
    __nand_addr(0);
    if (page_size >= 2048)
        __nand_addr(0);

    /* Send page address */
    __nand_addr(page_addr & 0xff);
    __nand_addr((page_addr >> 8) & 0xff);
    if (row_cycle >= 3)
        __nand_addr((page_addr >> 16) & 0xff);

    /* Send READSTART command for 2048 ps NAND */
    if (page_size >= 2048)
        __nand_cmd(NAND_CMD_READSTART);

    /* Wait for device ready */
    jz_nand_wait_ready();

    /* Read page data */
    data_buf = dst;

    ecc_count = page_size / ECC_BLOCK;

    for (i = 0; i < ecc_count; i++)
    {
#ifdef USE_ECC
        volatile unsigned char *paraddr = (volatile unsigned char *)EMC_NFPAR0;
        unsigned int stat;

        /* Enable RS decoding */
        REG_EMC_NFINTS = 0x0;
        __nand_ecc_rs_decoding();
#endif

        /* Read data */
        jz_nand_read_buf((void *)data_buf, ECC_BLOCK, bus_width);

#ifdef USE_ECC
        /* Set PAR values */
        for (j = 0; j < PAR_SIZE; j++)
            *paraddr++ = oob_buf[ECC_POS + i*PAR_SIZE + j];

        /* Set PRDY */
        REG_EMC_NFECR |= EMC_NFECR_PRDY;

        /* Wait for completion */
        __nand_ecc_decode_sync();

        /* Disable decoding */
        __nand_ecc_disable();

        /* Check result of decoding */
        stat = REG_EMC_NFINTS;
        if (stat & EMC_NFINTS_ERR)
        {
            /* Error occurred */
            if (stat & EMC_NFINTS_UNCOR)
            {
                /* Uncorrectable error occurred */
                logf("Uncorrectable ECC error at NAND page address 0x%lx", page_addr);
                return -1;
            }
            else
            {
                unsigned int errcnt, index, mask;

                errcnt = (stat & EMC_NFINTS_ERRCNT_MASK) >> EMC_NFINTS_ERRCNT_BIT;
                switch (errcnt)
                {
                case 4:
                    index = (REG_EMC_NFERR3 & EMC_NFERR_INDEX_MASK) >> EMC_NFERR_INDEX_BIT;
                    mask = (REG_EMC_NFERR3 & EMC_NFERR_MASK_MASK) >> EMC_NFERR_MASK_BIT;
                    jz_rs_correct(data_buf, index, mask);
                case 3:
                    index = (REG_EMC_NFERR2 & EMC_NFERR_INDEX_MASK) >> EMC_NFERR_INDEX_BIT;
                    mask = (REG_EMC_NFERR2 & EMC_NFERR_MASK_MASK) >> EMC_NFERR_MASK_BIT;
                    jz_rs_correct(data_buf, index, mask);
                case 2:
                    index = (REG_EMC_NFERR1 & EMC_NFERR_INDEX_MASK) >> EMC_NFERR_INDEX_BIT;
                    mask = (REG_EMC_NFERR1 & EMC_NFERR_MASK_MASK) >> EMC_NFERR_MASK_BIT;
                    jz_rs_correct(data_buf, index, mask);
                case 1:
                    index = (REG_EMC_NFERR0 & EMC_NFERR_INDEX_MASK) >> EMC_NFERR_INDEX_BIT;
                    mask = (REG_EMC_NFERR0 & EMC_NFERR_MASK_MASK) >> EMC_NFERR_MASK_BIT;
                    jz_rs_correct(data_buf, index, mask);
                    break;
                default:
                    break;
                }
            }
        }
#endif

        data_buf += ECC_BLOCK;
    }

    return 0;
}

/*
 * Disable NAND controller
 */
static void jz_nand_disable(void)
{
    REG_EMC_NFCSR &= ~(EMC_NFCSR_NFCE1 | EMC_NFCSR_NFCE2 |
                       EMC_NFCSR_NFCE3 | EMC_NFCSR_NFCE4 |
                       EMC_NFCSR_NFE1 | EMC_NFCSR_NFE2   |
                       EMC_NFCSR_NFE3 | EMC_NFCSR_NFE4   );
}

/*
 * Enable NAND controller
 */
static void jz_nand_enable(void)
{
#if 0
    /* OF RE */
    REG_GPIO_PXFUNS(1) = 0x1E018000; // __gpio_as_func0() start
    REG_GPIO_PXSELC(1) = 0x1E018000; // __gpio_as_func0() end

    REG_GPIO_PXFUNS(2) = 0x3000<<16; // __gpio_as_func0() start
    REG_GPIO_PXSELC(2) = 0x3000<<16; // __gpio_as_func0() end

    REG_GPIO_PXFUNC(2) = 0x4000<<16; // __gpio_port_as_input() start
    REG_GPIO_PXSELC(2) = 0x4000<<16;
    REG_GPIO_PXDIRC(2) = 0x4000<<16; // __gpio_port_as_input() end
    REG_GPIO_PXPES(2) = 0x4000<<16; // __gpio_disable_pull()

    REG_GPIO_PXFUNS(1) = 0x40<<16; // __gpio_as_func0() start
    REG_GPIO_PXSELC(1) = 0x40<<16; // __gpio_as_func0() end

    REG_EMC_SMCR1 = (REG_EMC_SMCR1 & 0xFF) | 0x4621200;
    REG_EMC_SMCR2 = (REG_EMC_SMCR2 & 0xFF) | 0x4621200;
    REG_EMC_SMCR3 = (REG_EMC_SMCR3 & 0xFF) | 0x4621200;
    REG_EMC_SMCR4 = (REG_EMC_SMCR4 & 0xFF) | 0x4621200;

    REG_EMC_SMCR1 = REG_EMC_SMCR2 = REG_EMC_SMCR3 = REG_EMC_SMCR4 = 0x6621200;
#else
    REG_EMC_SMCR1 = REG_EMC_SMCR2 = REG_EMC_SMCR3 = REG_EMC_SMCR4 = 0x04444400;
#endif
}

static void jz_nand_select(int bank)
{
    REG_EMC_NFCSR |= (EMC_NFCSR_NFE(bank+1) | EMC_NFCSR_NFCE(bank+1));

    switch(bank)
    {
        case 0:
            nand_address = 0xB8000000;
            break;
        case 1:
            nand_address = 0xB4000000;
            break;
        case 2:
            nand_address = 0xAC000000;
            break;
        case 3:
            nand_address = 0xA8000000;
            break;
    }
}

static void jz_nand_deselect(int bank)
{
    REG_EMC_NFCSR &= ~(EMC_NFCSR_NFE(bank+1) | EMC_NFCSR_NFCE(bank+1));
    nand_address = 0;
}

static int jz_nand_init(void)
{
    unsigned char cData[5];
    int i;

    jz_nand_enable();

    for(i=0; i<4; i++)
    {
        jz_nand_select(i);

        __nand_cmd(NAND_CMD_READID);
        __nand_addr(NAND_CMD_READ0);
        cData[0] = __nand_data8();
        cData[1] = __nand_data8();
        cData[2] = __nand_data8();
        cData[3] = __nand_data8();
        cData[4] = __nand_data8();

        jz_nand_deselect(i);

        logf("NAND chip %d: 0x%x 0x%x 0x%x 0x%x 0x%x", i+1, cData[0], cData[1],
                                                  cData[2], cData[3], cData[4]);

        banks[i] = nand_identify(cData);

        if(banks[i] != NULL)
            nr_banks++;

        if(i == 0 && banks[i] == NULL)
        {
            panicf("Unknown NAND flash chip: 0x%x 0x%x 0x%x 0x%x 0x%x", cData[0],
                                          cData[1], cData[2], cData[3], cData[4]);
            return -1; /* panicf() doesn't return though */
        }
    }
    chip_info = banks[0];

    internal_param.bus_width = 8;
    internal_param.row_cycle = chip_info->row_cycles;
    internal_param.page_size = chip_info->page_size;
    internal_param.oob_size = chip_info->spare_size;
    internal_param.page_per_block = chip_info->pages_per_block;

    bank_size = chip_info->page_size * chip_info->blocks_per_bank / 512 * chip_info->pages_per_block;

    jz_nand_disable();

    return 0;
}

int nand_init(void)
{
    int res = 0;
    static bool inited = false;

    if(!inited)
    {
        res = jz_nand_init();
        mutex_init(&nand_mtx);
#ifdef USE_DMA
        mutex_init(&nand_dma_mtx);
        semaphore_init(&nand_dma_complete, 1, 0);
        system_enable_irq(DMA_IRQ(DMA_NAND_CHANNEL));
#endif

        inited = true;
    }

    return res;
}

static inline int read_sector(unsigned long start, unsigned int count,
                               void* buf, unsigned int chip_size)
{
    register int ret;

    if(UNLIKELY(start % chip_size == 0 && count == chip_size))
        ret = jz_nand_read_page(start / chip_size, buf);
    else
    {
        ret = jz_nand_read_page(start / chip_size, temp_page);
        memcpy(buf, temp_page + (start % chip_size), count);
    }

    return ret;
}

int nand_read_sectors(IF_MV(int drive,) unsigned long start, int count, void* buf)
{
#ifdef HAVE_MULTIVOLUME
    (void)drive;
#endif
    int ret = 0;
    unsigned int i, _count, chip_size = chip_info->page_size;
    unsigned long _start;

    logf("start");
    mutex_lock(&nand_mtx);

    _start = start << 9;
    _count = count << 9;

    if(_count <= chip_size)
    {
        jz_nand_select(start / bank_size);
        ret = read_sector(_start, _count, buf, chip_size);
        jz_nand_deselect(start / bank_size);
    }
    else
    {
        for(i=0; i<_count && ret==0; i+=chip_size)
        {
            jz_nand_select((start+(i>>9)) / bank_size);

            ret = read_sector(_start+i, (_count-i < chip_size ?
                                         _count-i : chip_size),
                               buf+i, chip_size);

            jz_nand_deselect((start+(i>>9)) / bank_size);
        }
    }

    mutex_unlock(&nand_mtx);

    logf("nand_read_sectors(%ld, %d, 0x%x): %d", start, count, (int)buf, ret);

    return ret;
}

/* TODO */
int nand_write_sectors(IF_MV(int drive,) unsigned long start, int count, const void* buf)
{
    (void)start;
    (void)count;
    (void)buf;
#ifdef HAVE_MULTIVOLUME
    (void)drive;
#endif

    return -1;
}

#ifdef HAVE_STORAGE_FLUSH
int nand_flush(void)
{
    return 0;
}
#endif

void nand_spindown(int seconds)
{
    /* null */
    (void)seconds;
}

void nand_sleep(void)
{
    /* null */
}

void nand_spin(void)
{
    /* null */
}

void nand_enable(bool on)
{
    /* null - flash controller is enabled/disabled as needed. */
    (void)on;
}

/* TODO */
long nand_last_disk_activity(void)
{
    return 0;
}

int nand_spinup_time(void)
{
    return 0;
}

void nand_sleepnow(void)
{
}

#ifdef STORAGE_GET_INFO
void nand_get_info(IF_MV(int drive,) struct storage_info *info)
{
#ifdef HAVE_MULTIVOLUME
    (void)drive;
#endif

    /* firmware version */
    info->revision="0.00";

    info->vendor="Rockbox";
    info->product="NAND Storage";

    /* blocks count */
    info->num_sectors = bank_size * nr_banks;
    info->sector_size = 512;
}
#endif

#ifdef CONFIG_STORAGE_MULTI
int nand_num_drives(int first_drive)
{
    /* We don't care which logical drive number(s) we have been assigned */
    (void)first_drive;

    return 1;
}
#endif
