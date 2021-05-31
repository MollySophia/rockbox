/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 3.0.0
 * x1000 version: 1.0
 * x1000 authors: Aidan MacDonald
 *
 * Copyright (C) 2015 by the authors
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
#ifndef __HEADERGEN_PCM_H__
#define __HEADERGEN_PCM_H__

#include "macro.h"

#define REG_PCM_CTL             jz_reg(PCM_CTL)
#define JA_PCM_CTL              (0xb0071000 + 0x0)
#define JT_PCM_CTL              JIO_32_RW
#define JN_PCM_CTL              PCM_CTL
#define JI_PCM_CTL              
#define BP_PCM_CTL_ERDMA        9
#define BM_PCM_CTL_ERDMA        0x200
#define BF_PCM_CTL_ERDMA(v)     (((v) & 0x1) << 9)
#define BFM_PCM_CTL_ERDMA(v)    BM_PCM_CTL_ERDMA
#define BF_PCM_CTL_ERDMA_V(e)   BF_PCM_CTL_ERDMA(BV_PCM_CTL_ERDMA__##e)
#define BFM_PCM_CTL_ERDMA_V(v)  BM_PCM_CTL_ERDMA
#define BP_PCM_CTL_ETDMA        8
#define BM_PCM_CTL_ETDMA        0x100
#define BF_PCM_CTL_ETDMA(v)     (((v) & 0x1) << 8)
#define BFM_PCM_CTL_ETDMA(v)    BM_PCM_CTL_ETDMA
#define BF_PCM_CTL_ETDMA_V(e)   BF_PCM_CTL_ETDMA(BV_PCM_CTL_ETDMA__##e)
#define BFM_PCM_CTL_ETDMA_V(v)  BM_PCM_CTL_ETDMA
#define BP_PCM_CTL_LSMP         7
#define BM_PCM_CTL_LSMP         0x80
#define BF_PCM_CTL_LSMP(v)      (((v) & 0x1) << 7)
#define BFM_PCM_CTL_LSMP(v)     BM_PCM_CTL_LSMP
#define BF_PCM_CTL_LSMP_V(e)    BF_PCM_CTL_LSMP(BV_PCM_CTL_LSMP__##e)
#define BFM_PCM_CTL_LSMP_V(v)   BM_PCM_CTL_LSMP
#define BP_PCM_CTL_ERPL         6
#define BM_PCM_CTL_ERPL         0x40
#define BF_PCM_CTL_ERPL(v)      (((v) & 0x1) << 6)
#define BFM_PCM_CTL_ERPL(v)     BM_PCM_CTL_ERPL
#define BF_PCM_CTL_ERPL_V(e)    BF_PCM_CTL_ERPL(BV_PCM_CTL_ERPL__##e)
#define BFM_PCM_CTL_ERPL_V(v)   BM_PCM_CTL_ERPL
#define BP_PCM_CTL_EREC         5
#define BM_PCM_CTL_EREC         0x20
#define BF_PCM_CTL_EREC(v)      (((v) & 0x1) << 5)
#define BFM_PCM_CTL_EREC(v)     BM_PCM_CTL_EREC
#define BF_PCM_CTL_EREC_V(e)    BF_PCM_CTL_EREC(BV_PCM_CTL_EREC__##e)
#define BFM_PCM_CTL_EREC_V(v)   BM_PCM_CTL_EREC
#define BP_PCM_CTL_FLUSH        4
#define BM_PCM_CTL_FLUSH        0x10
#define BF_PCM_CTL_FLUSH(v)     (((v) & 0x1) << 4)
#define BFM_PCM_CTL_FLUSH(v)    BM_PCM_CTL_FLUSH
#define BF_PCM_CTL_FLUSH_V(e)   BF_PCM_CTL_FLUSH(BV_PCM_CTL_FLUSH__##e)
#define BFM_PCM_CTL_FLUSH_V(v)  BM_PCM_CTL_FLUSH
#define BP_PCM_CTL_RST          3
#define BM_PCM_CTL_RST          0x8
#define BF_PCM_CTL_RST(v)       (((v) & 0x1) << 3)
#define BFM_PCM_CTL_RST(v)      BM_PCM_CTL_RST
#define BF_PCM_CTL_RST_V(e)     BF_PCM_CTL_RST(BV_PCM_CTL_RST__##e)
#define BFM_PCM_CTL_RST_V(v)    BM_PCM_CTL_RST
#define BP_PCM_CTL_CLKEN        1
#define BM_PCM_CTL_CLKEN        0x2
#define BF_PCM_CTL_CLKEN(v)     (((v) & 0x1) << 1)
#define BFM_PCM_CTL_CLKEN(v)    BM_PCM_CTL_CLKEN
#define BF_PCM_CTL_CLKEN_V(e)   BF_PCM_CTL_CLKEN(BV_PCM_CTL_CLKEN__##e)
#define BFM_PCM_CTL_CLKEN_V(v)  BM_PCM_CTL_CLKEN
#define BP_PCM_CTL_PCMEN        0
#define BM_PCM_CTL_PCMEN        0x1
#define BF_PCM_CTL_PCMEN(v)     (((v) & 0x1) << 0)
#define BFM_PCM_CTL_PCMEN(v)    BM_PCM_CTL_PCMEN
#define BF_PCM_CTL_PCMEN_V(e)   BF_PCM_CTL_PCMEN(BV_PCM_CTL_PCMEN__##e)
#define BFM_PCM_CTL_PCMEN_V(v)  BM_PCM_CTL_PCMEN

#define REG_PCM_CFG                 jz_reg(PCM_CFG)
#define JA_PCM_CFG                  (0xb0071000 + 0x4)
#define JT_PCM_CFG                  JIO_32_RW
#define JN_PCM_CFG                  PCM_CFG
#define JI_PCM_CFG                  
#define BP_PCM_CFG_SLOT             13
#define BM_PCM_CFG_SLOT             0x6000
#define BF_PCM_CFG_SLOT(v)          (((v) & 0x3) << 13)
#define BFM_PCM_CFG_SLOT(v)         BM_PCM_CFG_SLOT
#define BF_PCM_CFG_SLOT_V(e)        BF_PCM_CFG_SLOT(BV_PCM_CFG_SLOT__##e)
#define BFM_PCM_CFG_SLOT_V(v)       BM_PCM_CFG_SLOT
#define BP_PCM_CFG_RFTH             5
#define BM_PCM_CFG_RFTH             0x1e0
#define BF_PCM_CFG_RFTH(v)          (((v) & 0xf) << 5)
#define BFM_PCM_CFG_RFTH(v)         BM_PCM_CFG_RFTH
#define BF_PCM_CFG_RFTH_V(e)        BF_PCM_CFG_RFTH(BV_PCM_CFG_RFTH__##e)
#define BFM_PCM_CFG_RFTH_V(v)       BM_PCM_CFG_RFTH
#define BP_PCM_CFG_TFTH             1
#define BM_PCM_CFG_TFTH             0x1e
#define BF_PCM_CFG_TFTH(v)          (((v) & 0xf) << 1)
#define BFM_PCM_CFG_TFTH(v)         BM_PCM_CFG_TFTH
#define BF_PCM_CFG_TFTH_V(e)        BF_PCM_CFG_TFTH(BV_PCM_CFG_TFTH__##e)
#define BFM_PCM_CFG_TFTH_V(v)       BM_PCM_CFG_TFTH
#define BP_PCM_CFG_ISS              12
#define BM_PCM_CFG_ISS              0x1000
#define BF_PCM_CFG_ISS(v)           (((v) & 0x1) << 12)
#define BFM_PCM_CFG_ISS(v)          BM_PCM_CFG_ISS
#define BF_PCM_CFG_ISS_V(e)         BF_PCM_CFG_ISS(BV_PCM_CFG_ISS__##e)
#define BFM_PCM_CFG_ISS_V(v)        BM_PCM_CFG_ISS
#define BP_PCM_CFG_OSS              11
#define BM_PCM_CFG_OSS              0x800
#define BF_PCM_CFG_OSS(v)           (((v) & 0x1) << 11)
#define BFM_PCM_CFG_OSS(v)          BM_PCM_CFG_OSS
#define BF_PCM_CFG_OSS_V(e)         BF_PCM_CFG_OSS(BV_PCM_CFG_OSS__##e)
#define BFM_PCM_CFG_OSS_V(v)        BM_PCM_CFG_OSS
#define BP_PCM_CFG_IMSBPOS          10
#define BM_PCM_CFG_IMSBPOS          0x400
#define BF_PCM_CFG_IMSBPOS(v)       (((v) & 0x1) << 10)
#define BFM_PCM_CFG_IMSBPOS(v)      BM_PCM_CFG_IMSBPOS
#define BF_PCM_CFG_IMSBPOS_V(e)     BF_PCM_CFG_IMSBPOS(BV_PCM_CFG_IMSBPOS__##e)
#define BFM_PCM_CFG_IMSBPOS_V(v)    BM_PCM_CFG_IMSBPOS
#define BP_PCM_CFG_OMSBPOS          9
#define BM_PCM_CFG_OMSBPOS          0x200
#define BF_PCM_CFG_OMSBPOS(v)       (((v) & 0x1) << 9)
#define BFM_PCM_CFG_OMSBPOS(v)      BM_PCM_CFG_OMSBPOS
#define BF_PCM_CFG_OMSBPOS_V(e)     BF_PCM_CFG_OMSBPOS(BV_PCM_CFG_OMSBPOS__##e)
#define BFM_PCM_CFG_OMSBPOS_V(v)    BM_PCM_CFG_OMSBPOS
#define BP_PCM_CFG_PCMMOD           0
#define BM_PCM_CFG_PCMMOD           0x1
#define BF_PCM_CFG_PCMMOD(v)        (((v) & 0x1) << 0)
#define BFM_PCM_CFG_PCMMOD(v)       BM_PCM_CFG_PCMMOD
#define BF_PCM_CFG_PCMMOD_V(e)      BF_PCM_CFG_PCMMOD(BV_PCM_CFG_PCMMOD__##e)
#define BFM_PCM_CFG_PCMMOD_V(v)     BM_PCM_CFG_PCMMOD

#define REG_PCM_DP  jz_reg(PCM_DP)
#define JA_PCM_DP   (0xb0071000 + 0x8)
#define JT_PCM_DP   JIO_32_RW
#define JN_PCM_DP   PCM_DP
#define JI_PCM_DP   

#define REG_PCM_INTC            jz_reg(PCM_INTC)
#define JA_PCM_INTC             (0xb0071000 + 0xc)
#define JT_PCM_INTC             JIO_32_RW
#define JN_PCM_INTC             PCM_INTC
#define JI_PCM_INTC             
#define BP_PCM_INTC_ETFS        3
#define BM_PCM_INTC_ETFS        0x8
#define BF_PCM_INTC_ETFS(v)     (((v) & 0x1) << 3)
#define BFM_PCM_INTC_ETFS(v)    BM_PCM_INTC_ETFS
#define BF_PCM_INTC_ETFS_V(e)   BF_PCM_INTC_ETFS(BV_PCM_INTC_ETFS__##e)
#define BFM_PCM_INTC_ETFS_V(v)  BM_PCM_INTC_ETFS
#define BP_PCM_INTC_ETUR        2
#define BM_PCM_INTC_ETUR        0x4
#define BF_PCM_INTC_ETUR(v)     (((v) & 0x1) << 2)
#define BFM_PCM_INTC_ETUR(v)    BM_PCM_INTC_ETUR
#define BF_PCM_INTC_ETUR_V(e)   BF_PCM_INTC_ETUR(BV_PCM_INTC_ETUR__##e)
#define BFM_PCM_INTC_ETUR_V(v)  BM_PCM_INTC_ETUR
#define BP_PCM_INTC_ERFS        1
#define BM_PCM_INTC_ERFS        0x2
#define BF_PCM_INTC_ERFS(v)     (((v) & 0x1) << 1)
#define BFM_PCM_INTC_ERFS(v)    BM_PCM_INTC_ERFS
#define BF_PCM_INTC_ERFS_V(e)   BF_PCM_INTC_ERFS(BV_PCM_INTC_ERFS__##e)
#define BFM_PCM_INTC_ERFS_V(v)  BM_PCM_INTC_ERFS
#define BP_PCM_INTC_EROR        0
#define BM_PCM_INTC_EROR        0x1
#define BF_PCM_INTC_EROR(v)     (((v) & 0x1) << 0)
#define BFM_PCM_INTC_EROR(v)    BM_PCM_INTC_EROR
#define BF_PCM_INTC_EROR_V(e)   BF_PCM_INTC_EROR(BV_PCM_INTC_EROR__##e)
#define BFM_PCM_INTC_EROR_V(v)  BM_PCM_INTC_EROR

#define REG_PCM_INTS            jz_reg(PCM_INTS)
#define JA_PCM_INTS             (0xb0071000 + 0x10)
#define JT_PCM_INTS             JIO_32_RW
#define JN_PCM_INTS             PCM_INTS
#define JI_PCM_INTS             
#define BP_PCM_INTS_TFL         9
#define BM_PCM_INTS_TFL         0x3e00
#define BF_PCM_INTS_TFL(v)      (((v) & 0x1f) << 9)
#define BFM_PCM_INTS_TFL(v)     BM_PCM_INTS_TFL
#define BF_PCM_INTS_TFL_V(e)    BF_PCM_INTS_TFL(BV_PCM_INTS_TFL__##e)
#define BFM_PCM_INTS_TFL_V(v)   BM_PCM_INTS_TFL
#define BP_PCM_INTS_RSTS        14
#define BM_PCM_INTS_RSTS        0x4000
#define BF_PCM_INTS_RSTS(v)     (((v) & 0x1) << 14)
#define BFM_PCM_INTS_RSTS(v)    BM_PCM_INTS_RSTS
#define BF_PCM_INTS_RSTS_V(e)   BF_PCM_INTS_RSTS(BV_PCM_INTS_RSTS__##e)
#define BFM_PCM_INTS_RSTS_V(v)  BM_PCM_INTS_RSTS
#define BP_PCM_INTS_TFS         8
#define BM_PCM_INTS_TFS         0x100
#define BF_PCM_INTS_TFS(v)      (((v) & 0x1) << 8)
#define BFM_PCM_INTS_TFS(v)     BM_PCM_INTS_TFS
#define BF_PCM_INTS_TFS_V(e)    BF_PCM_INTS_TFS(BV_PCM_INTS_TFS__##e)
#define BFM_PCM_INTS_TFS_V(v)   BM_PCM_INTS_TFS
#define BP_PCM_INTS_TUR         7
#define BM_PCM_INTS_TUR         0x80
#define BF_PCM_INTS_TUR(v)      (((v) & 0x1) << 7)
#define BFM_PCM_INTS_TUR(v)     BM_PCM_INTS_TUR
#define BF_PCM_INTS_TUR_V(e)    BF_PCM_INTS_TUR(BV_PCM_INTS_TUR__##e)
#define BFM_PCM_INTS_TUR_V(v)   BM_PCM_INTS_TUR
#define BP_PCM_INTS_RFL         2
#define BM_PCM_INTS_RFL         0x7c
#define BF_PCM_INTS_RFL(v)      (((v) & 0x1f) << 2)
#define BFM_PCM_INTS_RFL(v)     BM_PCM_INTS_RFL
#define BF_PCM_INTS_RFL_V(e)    BF_PCM_INTS_RFL(BV_PCM_INTS_RFL__##e)
#define BFM_PCM_INTS_RFL_V(v)   BM_PCM_INTS_RFL
#define BP_PCM_INTS_RFS         1
#define BM_PCM_INTS_RFS         0x2
#define BF_PCM_INTS_RFS(v)      (((v) & 0x1) << 1)
#define BFM_PCM_INTS_RFS(v)     BM_PCM_INTS_RFS
#define BF_PCM_INTS_RFS_V(e)    BF_PCM_INTS_RFS(BV_PCM_INTS_RFS__##e)
#define BFM_PCM_INTS_RFS_V(v)   BM_PCM_INTS_RFS
#define BP_PCM_INTS_ROR         0
#define BM_PCM_INTS_ROR         0x1
#define BF_PCM_INTS_ROR(v)      (((v) & 0x1) << 0)
#define BFM_PCM_INTS_ROR(v)     BM_PCM_INTS_ROR
#define BF_PCM_INTS_ROR_V(e)    BF_PCM_INTS_ROR(BV_PCM_INTS_ROR__##e)
#define BFM_PCM_INTS_ROR_V(v)   BM_PCM_INTS_ROR

#define REG_PCM_DIV             jz_reg(PCM_DIV)
#define JA_PCM_DIV              (0xb0071000 + 0x14)
#define JT_PCM_DIV              JIO_32_RW
#define JN_PCM_DIV              PCM_DIV
#define JI_PCM_DIV              
#define BP_PCM_DIV_SYNL         11
#define BM_PCM_DIV_SYNL         0x1f800
#define BF_PCM_DIV_SYNL(v)      (((v) & 0x3f) << 11)
#define BFM_PCM_DIV_SYNL(v)     BM_PCM_DIV_SYNL
#define BF_PCM_DIV_SYNL_V(e)    BF_PCM_DIV_SYNL(BV_PCM_DIV_SYNL__##e)
#define BFM_PCM_DIV_SYNL_V(v)   BM_PCM_DIV_SYNL
#define BP_PCM_DIV_SYNDIV       6
#define BM_PCM_DIV_SYNDIV       0x7c0
#define BF_PCM_DIV_SYNDIV(v)    (((v) & 0x1f) << 6)
#define BFM_PCM_DIV_SYNDIV(v)   BM_PCM_DIV_SYNDIV
#define BF_PCM_DIV_SYNDIV_V(e)  BF_PCM_DIV_SYNDIV(BV_PCM_DIV_SYNDIV__##e)
#define BFM_PCM_DIV_SYNDIV_V(v) BM_PCM_DIV_SYNDIV
#define BP_PCM_DIV_CLKDIV       0
#define BM_PCM_DIV_CLKDIV       0x3f
#define BF_PCM_DIV_CLKDIV(v)    (((v) & 0x3f) << 0)
#define BFM_PCM_DIV_CLKDIV(v)   BM_PCM_DIV_CLKDIV
#define BF_PCM_DIV_CLKDIV_V(e)  BF_PCM_DIV_CLKDIV(BV_PCM_DIV_CLKDIV__##e)
#define BFM_PCM_DIV_CLKDIV_V(v) BM_PCM_DIV_CLKDIV

#endif /* __HEADERGEN_PCM_H__*/
