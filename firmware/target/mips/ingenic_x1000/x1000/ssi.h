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
#ifndef __HEADERGEN_SSI_H__
#define __HEADERGEN_SSI_H__

#include "macro.h"

#define REG_SSI_DR  jz_reg(SSI_DR)
#define JA_SSI_DR   (0xb0043000 + 0x0)
#define JT_SSI_DR   JIO_32_RW
#define JN_SSI_DR   SSI_DR
#define JI_SSI_DR   

#define REG_SSI_CR0                 jz_reg(SSI_CR0)
#define JA_SSI_CR0                  (0xb0043000 + 0x4)
#define JT_SSI_CR0                  JIO_32_RW
#define JN_SSI_CR0                  SSI_CR0
#define JI_SSI_CR0                  
#define BP_SSI_CR0_TENDIAN          18
#define BM_SSI_CR0_TENDIAN          0xc0000
#define BF_SSI_CR0_TENDIAN(v)       (((v) & 0x3) << 18)
#define BFM_SSI_CR0_TENDIAN(v)      BM_SSI_CR0_TENDIAN
#define BF_SSI_CR0_TENDIAN_V(e)     BF_SSI_CR0_TENDIAN(BV_SSI_CR0_TENDIAN__##e)
#define BFM_SSI_CR0_TENDIAN_V(v)    BM_SSI_CR0_TENDIAN
#define BP_SSI_CR0_RENDIAN          16
#define BM_SSI_CR0_RENDIAN          0x30000
#define BF_SSI_CR0_RENDIAN(v)       (((v) & 0x3) << 16)
#define BFM_SSI_CR0_RENDIAN(v)      BM_SSI_CR0_RENDIAN
#define BF_SSI_CR0_RENDIAN_V(e)     BF_SSI_CR0_RENDIAN(BV_SSI_CR0_RENDIAN__##e)
#define BFM_SSI_CR0_RENDIAN_V(v)    BM_SSI_CR0_RENDIAN
#define BP_SSI_CR0_SSIE             15
#define BM_SSI_CR0_SSIE             0x8000
#define BF_SSI_CR0_SSIE(v)          (((v) & 0x1) << 15)
#define BFM_SSI_CR0_SSIE(v)         BM_SSI_CR0_SSIE
#define BF_SSI_CR0_SSIE_V(e)        BF_SSI_CR0_SSIE(BV_SSI_CR0_SSIE__##e)
#define BFM_SSI_CR0_SSIE_V(v)       BM_SSI_CR0_SSIE
#define BP_SSI_CR0_TIE              14
#define BM_SSI_CR0_TIE              0x4000
#define BF_SSI_CR0_TIE(v)           (((v) & 0x1) << 14)
#define BFM_SSI_CR0_TIE(v)          BM_SSI_CR0_TIE
#define BF_SSI_CR0_TIE_V(e)         BF_SSI_CR0_TIE(BV_SSI_CR0_TIE__##e)
#define BFM_SSI_CR0_TIE_V(v)        BM_SSI_CR0_TIE
#define BP_SSI_CR0_RIE              13
#define BM_SSI_CR0_RIE              0x2000
#define BF_SSI_CR0_RIE(v)           (((v) & 0x1) << 13)
#define BFM_SSI_CR0_RIE(v)          BM_SSI_CR0_RIE
#define BF_SSI_CR0_RIE_V(e)         BF_SSI_CR0_RIE(BV_SSI_CR0_RIE__##e)
#define BFM_SSI_CR0_RIE_V(v)        BM_SSI_CR0_RIE
#define BP_SSI_CR0_TEIE             12
#define BM_SSI_CR0_TEIE             0x1000
#define BF_SSI_CR0_TEIE(v)          (((v) & 0x1) << 12)
#define BFM_SSI_CR0_TEIE(v)         BM_SSI_CR0_TEIE
#define BF_SSI_CR0_TEIE_V(e)        BF_SSI_CR0_TEIE(BV_SSI_CR0_TEIE__##e)
#define BFM_SSI_CR0_TEIE_V(v)       BM_SSI_CR0_TEIE
#define BP_SSI_CR0_REIE             11
#define BM_SSI_CR0_REIE             0x800
#define BF_SSI_CR0_REIE(v)          (((v) & 0x1) << 11)
#define BFM_SSI_CR0_REIE(v)         BM_SSI_CR0_REIE
#define BF_SSI_CR0_REIE_V(e)        BF_SSI_CR0_REIE(BV_SSI_CR0_REIE__##e)
#define BFM_SSI_CR0_REIE_V(v)       BM_SSI_CR0_REIE
#define BP_SSI_CR0_LOOP             10
#define BM_SSI_CR0_LOOP             0x400
#define BF_SSI_CR0_LOOP(v)          (((v) & 0x1) << 10)
#define BFM_SSI_CR0_LOOP(v)         BM_SSI_CR0_LOOP
#define BF_SSI_CR0_LOOP_V(e)        BF_SSI_CR0_LOOP(BV_SSI_CR0_LOOP__##e)
#define BFM_SSI_CR0_LOOP_V(v)       BM_SSI_CR0_LOOP
#define BP_SSI_CR0_RFINE            9
#define BM_SSI_CR0_RFINE            0x200
#define BF_SSI_CR0_RFINE(v)         (((v) & 0x1) << 9)
#define BFM_SSI_CR0_RFINE(v)        BM_SSI_CR0_RFINE
#define BF_SSI_CR0_RFINE_V(e)       BF_SSI_CR0_RFINE(BV_SSI_CR0_RFINE__##e)
#define BFM_SSI_CR0_RFINE_V(v)      BM_SSI_CR0_RFINE
#define BP_SSI_CR0_RFINC            8
#define BM_SSI_CR0_RFINC            0x100
#define BF_SSI_CR0_RFINC(v)         (((v) & 0x1) << 8)
#define BFM_SSI_CR0_RFINC(v)        BM_SSI_CR0_RFINC
#define BF_SSI_CR0_RFINC_V(e)       BF_SSI_CR0_RFINC(BV_SSI_CR0_RFINC__##e)
#define BFM_SSI_CR0_RFINC_V(v)      BM_SSI_CR0_RFINC
#define BP_SSI_CR0_EACLRUN          7
#define BM_SSI_CR0_EACLRUN          0x80
#define BF_SSI_CR0_EACLRUN(v)       (((v) & 0x1) << 7)
#define BFM_SSI_CR0_EACLRUN(v)      BM_SSI_CR0_EACLRUN
#define BF_SSI_CR0_EACLRUN_V(e)     BF_SSI_CR0_EACLRUN(BV_SSI_CR0_EACLRUN__##e)
#define BFM_SSI_CR0_EACLRUN_V(v)    BM_SSI_CR0_EACLRUN
#define BP_SSI_CR0_FSEL             6
#define BM_SSI_CR0_FSEL             0x40
#define BF_SSI_CR0_FSEL(v)          (((v) & 0x1) << 6)
#define BFM_SSI_CR0_FSEL(v)         BM_SSI_CR0_FSEL
#define BF_SSI_CR0_FSEL_V(e)        BF_SSI_CR0_FSEL(BV_SSI_CR0_FSEL__##e)
#define BFM_SSI_CR0_FSEL_V(v)       BM_SSI_CR0_FSEL
#define BP_SSI_CR0_VRCNT            4
#define BM_SSI_CR0_VRCNT            0x10
#define BF_SSI_CR0_VRCNT(v)         (((v) & 0x1) << 4)
#define BFM_SSI_CR0_VRCNT(v)        BM_SSI_CR0_VRCNT
#define BF_SSI_CR0_VRCNT_V(e)       BF_SSI_CR0_VRCNT(BV_SSI_CR0_VRCNT__##e)
#define BFM_SSI_CR0_VRCNT_V(v)      BM_SSI_CR0_VRCNT
#define BP_SSI_CR0_TFMODE           3
#define BM_SSI_CR0_TFMODE           0x8
#define BF_SSI_CR0_TFMODE(v)        (((v) & 0x1) << 3)
#define BFM_SSI_CR0_TFMODE(v)       BM_SSI_CR0_TFMODE
#define BF_SSI_CR0_TFMODE_V(e)      BF_SSI_CR0_TFMODE(BV_SSI_CR0_TFMODE__##e)
#define BFM_SSI_CR0_TFMODE_V(v)     BM_SSI_CR0_TFMODE
#define BP_SSI_CR0_TFLUSH           2
#define BM_SSI_CR0_TFLUSH           0x4
#define BF_SSI_CR0_TFLUSH(v)        (((v) & 0x1) << 2)
#define BFM_SSI_CR0_TFLUSH(v)       BM_SSI_CR0_TFLUSH
#define BF_SSI_CR0_TFLUSH_V(e)      BF_SSI_CR0_TFLUSH(BV_SSI_CR0_TFLUSH__##e)
#define BFM_SSI_CR0_TFLUSH_V(v)     BM_SSI_CR0_TFLUSH
#define BP_SSI_CR0_RFLUSH           1
#define BM_SSI_CR0_RFLUSH           0x2
#define BF_SSI_CR0_RFLUSH(v)        (((v) & 0x1) << 1)
#define BFM_SSI_CR0_RFLUSH(v)       BM_SSI_CR0_RFLUSH
#define BF_SSI_CR0_RFLUSH_V(e)      BF_SSI_CR0_RFLUSH(BV_SSI_CR0_RFLUSH__##e)
#define BFM_SSI_CR0_RFLUSH_V(v)     BM_SSI_CR0_RFLUSH
#define BP_SSI_CR0_DISREV           0
#define BM_SSI_CR0_DISREV           0x1
#define BF_SSI_CR0_DISREV(v)        (((v) & 0x1) << 0)
#define BFM_SSI_CR0_DISREV(v)       BM_SSI_CR0_DISREV
#define BF_SSI_CR0_DISREV_V(e)      BF_SSI_CR0_DISREV(BV_SSI_CR0_DISREV__##e)
#define BFM_SSI_CR0_DISREV_V(v)     BM_SSI_CR0_DISREV

#define REG_SSI_CR1             jz_reg(SSI_CR1)
#define JA_SSI_CR1              (0xb0043000 + 0x8)
#define JT_SSI_CR1              JIO_32_RW
#define JN_SSI_CR1              SSI_CR1
#define JI_SSI_CR1              
#define BP_SSI_CR1_FRMHL        30
#define BM_SSI_CR1_FRMHL        0xc0000000
#define BF_SSI_CR1_FRMHL(v)     (((v) & 0x3) << 30)
#define BFM_SSI_CR1_FRMHL(v)    BM_SSI_CR1_FRMHL
#define BF_SSI_CR1_FRMHL_V(e)   BF_SSI_CR1_FRMHL(BV_SSI_CR1_FRMHL__##e)
#define BFM_SSI_CR1_FRMHL_V(v)  BM_SSI_CR1_FRMHL
#define BP_SSI_CR1_TFVCK        28
#define BM_SSI_CR1_TFVCK        0x30000000
#define BF_SSI_CR1_TFVCK(v)     (((v) & 0x3) << 28)
#define BFM_SSI_CR1_TFVCK(v)    BM_SSI_CR1_TFVCK
#define BF_SSI_CR1_TFVCK_V(e)   BF_SSI_CR1_TFVCK(BV_SSI_CR1_TFVCK__##e)
#define BFM_SSI_CR1_TFVCK_V(v)  BM_SSI_CR1_TFVCK
#define BP_SSI_CR1_TCKFI        26
#define BM_SSI_CR1_TCKFI        0xc000000
#define BF_SSI_CR1_TCKFI(v)     (((v) & 0x3) << 26)
#define BFM_SSI_CR1_TCKFI(v)    BM_SSI_CR1_TCKFI
#define BF_SSI_CR1_TCKFI_V(e)   BF_SSI_CR1_TCKFI(BV_SSI_CR1_TCKFI__##e)
#define BFM_SSI_CR1_TCKFI_V(v)  BM_SSI_CR1_TCKFI
#define BP_SSI_CR1_FMAT         20
#define BM_SSI_CR1_FMAT         0x300000
#define BF_SSI_CR1_FMAT(v)      (((v) & 0x3) << 20)
#define BFM_SSI_CR1_FMAT(v)     BM_SSI_CR1_FMAT
#define BF_SSI_CR1_FMAT_V(e)    BF_SSI_CR1_FMAT(BV_SSI_CR1_FMAT__##e)
#define BFM_SSI_CR1_FMAT_V(v)   BM_SSI_CR1_FMAT
#define BP_SSI_CR1_TTRG         16
#define BM_SSI_CR1_TTRG         0xf0000
#define BF_SSI_CR1_TTRG(v)      (((v) & 0xf) << 16)
#define BFM_SSI_CR1_TTRG(v)     BM_SSI_CR1_TTRG
#define BF_SSI_CR1_TTRG_V(e)    BF_SSI_CR1_TTRG(BV_SSI_CR1_TTRG__##e)
#define BFM_SSI_CR1_TTRG_V(v)   BM_SSI_CR1_TTRG
#define BP_SSI_CR1_MCOM         12
#define BM_SSI_CR1_MCOM         0xf000
#define BF_SSI_CR1_MCOM(v)      (((v) & 0xf) << 12)
#define BFM_SSI_CR1_MCOM(v)     BM_SSI_CR1_MCOM
#define BF_SSI_CR1_MCOM_V(e)    BF_SSI_CR1_MCOM(BV_SSI_CR1_MCOM__##e)
#define BFM_SSI_CR1_MCOM_V(v)   BM_SSI_CR1_MCOM
#define BP_SSI_CR1_RTRG         8
#define BM_SSI_CR1_RTRG         0xf00
#define BF_SSI_CR1_RTRG(v)      (((v) & 0xf) << 8)
#define BFM_SSI_CR1_RTRG(v)     BM_SSI_CR1_RTRG
#define BF_SSI_CR1_RTRG_V(e)    BF_SSI_CR1_RTRG(BV_SSI_CR1_RTRG__##e)
#define BFM_SSI_CR1_RTRG_V(v)   BM_SSI_CR1_RTRG
#define BP_SSI_CR1_FLEN         3
#define BM_SSI_CR1_FLEN         0xf8
#define BF_SSI_CR1_FLEN(v)      (((v) & 0x1f) << 3)
#define BFM_SSI_CR1_FLEN(v)     BM_SSI_CR1_FLEN
#define BF_SSI_CR1_FLEN_V(e)    BF_SSI_CR1_FLEN(BV_SSI_CR1_FLEN__##e)
#define BFM_SSI_CR1_FLEN_V(v)   BM_SSI_CR1_FLEN
#define BP_SSI_CR1_ITFRM        24
#define BM_SSI_CR1_ITFRM        0x1000000
#define BF_SSI_CR1_ITFRM(v)     (((v) & 0x1) << 24)
#define BFM_SSI_CR1_ITFRM(v)    BM_SSI_CR1_ITFRM
#define BF_SSI_CR1_ITFRM_V(e)   BF_SSI_CR1_ITFRM(BV_SSI_CR1_ITFRM__##e)
#define BFM_SSI_CR1_ITFRM_V(v)  BM_SSI_CR1_ITFRM
#define BP_SSI_CR1_UNFIN        23
#define BM_SSI_CR1_UNFIN        0x800000
#define BF_SSI_CR1_UNFIN(v)     (((v) & 0x1) << 23)
#define BFM_SSI_CR1_UNFIN(v)    BM_SSI_CR1_UNFIN
#define BF_SSI_CR1_UNFIN_V(e)   BF_SSI_CR1_UNFIN(BV_SSI_CR1_UNFIN__##e)
#define BFM_SSI_CR1_UNFIN_V(v)  BM_SSI_CR1_UNFIN
#define BP_SSI_CR1_PHA          1
#define BM_SSI_CR1_PHA          0x2
#define BF_SSI_CR1_PHA(v)       (((v) & 0x1) << 1)
#define BFM_SSI_CR1_PHA(v)      BM_SSI_CR1_PHA
#define BF_SSI_CR1_PHA_V(e)     BF_SSI_CR1_PHA(BV_SSI_CR1_PHA__##e)
#define BFM_SSI_CR1_PHA_V(v)    BM_SSI_CR1_PHA
#define BP_SSI_CR1_POL          0
#define BM_SSI_CR1_POL          0x1
#define BF_SSI_CR1_POL(v)       (((v) & 0x1) << 0)
#define BFM_SSI_CR1_POL(v)      BM_SSI_CR1_POL
#define BF_SSI_CR1_POL_V(e)     BF_SSI_CR1_POL(BV_SSI_CR1_POL__##e)
#define BFM_SSI_CR1_POL_V(v)    BM_SSI_CR1_POL

#define REG_SSI_SR                  jz_reg(SSI_SR)
#define JA_SSI_SR                   (0xb0043000 + 0xc)
#define JT_SSI_SR                   JIO_32_RW
#define JN_SSI_SR                   SSI_SR
#define JI_SSI_SR                   
#define BP_SSI_SR_TFIFO_NUM         16
#define BM_SSI_SR_TFIFO_NUM         0x1ff0000
#define BF_SSI_SR_TFIFO_NUM(v)      (((v) & 0x1ff) << 16)
#define BFM_SSI_SR_TFIFO_NUM(v)     BM_SSI_SR_TFIFO_NUM
#define BF_SSI_SR_TFIFO_NUM_V(e)    BF_SSI_SR_TFIFO_NUM(BV_SSI_SR_TFIFO_NUM__##e)
#define BFM_SSI_SR_TFIFO_NUM_V(v)   BM_SSI_SR_TFIFO_NUM
#define BP_SSI_SR_RFIFO_NUM         8
#define BM_SSI_SR_RFIFO_NUM         0xff00
#define BF_SSI_SR_RFIFO_NUM(v)      (((v) & 0xff) << 8)
#define BFM_SSI_SR_RFIFO_NUM(v)     BM_SSI_SR_RFIFO_NUM
#define BF_SSI_SR_RFIFO_NUM_V(e)    BF_SSI_SR_RFIFO_NUM(BV_SSI_SR_RFIFO_NUM__##e)
#define BFM_SSI_SR_RFIFO_NUM_V(v)   BM_SSI_SR_RFIFO_NUM
#define BP_SSI_SR_END               7
#define BM_SSI_SR_END               0x80
#define BF_SSI_SR_END(v)            (((v) & 0x1) << 7)
#define BFM_SSI_SR_END(v)           BM_SSI_SR_END
#define BF_SSI_SR_END_V(e)          BF_SSI_SR_END(BV_SSI_SR_END__##e)
#define BFM_SSI_SR_END_V(v)         BM_SSI_SR_END
#define BP_SSI_SR_BUSY              6
#define BM_SSI_SR_BUSY              0x40
#define BF_SSI_SR_BUSY(v)           (((v) & 0x1) << 6)
#define BFM_SSI_SR_BUSY(v)          BM_SSI_SR_BUSY
#define BF_SSI_SR_BUSY_V(e)         BF_SSI_SR_BUSY(BV_SSI_SR_BUSY__##e)
#define BFM_SSI_SR_BUSY_V(v)        BM_SSI_SR_BUSY
#define BP_SSI_SR_TFF               5
#define BM_SSI_SR_TFF               0x20
#define BF_SSI_SR_TFF(v)            (((v) & 0x1) << 5)
#define BFM_SSI_SR_TFF(v)           BM_SSI_SR_TFF
#define BF_SSI_SR_TFF_V(e)          BF_SSI_SR_TFF(BV_SSI_SR_TFF__##e)
#define BFM_SSI_SR_TFF_V(v)         BM_SSI_SR_TFF
#define BP_SSI_SR_RFE               4
#define BM_SSI_SR_RFE               0x10
#define BF_SSI_SR_RFE(v)            (((v) & 0x1) << 4)
#define BFM_SSI_SR_RFE(v)           BM_SSI_SR_RFE
#define BF_SSI_SR_RFE_V(e)          BF_SSI_SR_RFE(BV_SSI_SR_RFE__##e)
#define BFM_SSI_SR_RFE_V(v)         BM_SSI_SR_RFE
#define BP_SSI_SR_TFHE              3
#define BM_SSI_SR_TFHE              0x8
#define BF_SSI_SR_TFHE(v)           (((v) & 0x1) << 3)
#define BFM_SSI_SR_TFHE(v)          BM_SSI_SR_TFHE
#define BF_SSI_SR_TFHE_V(e)         BF_SSI_SR_TFHE(BV_SSI_SR_TFHE__##e)
#define BFM_SSI_SR_TFHE_V(v)        BM_SSI_SR_TFHE
#define BP_SSI_SR_RFHF              2
#define BM_SSI_SR_RFHF              0x4
#define BF_SSI_SR_RFHF(v)           (((v) & 0x1) << 2)
#define BFM_SSI_SR_RFHF(v)          BM_SSI_SR_RFHF
#define BF_SSI_SR_RFHF_V(e)         BF_SSI_SR_RFHF(BV_SSI_SR_RFHF__##e)
#define BFM_SSI_SR_RFHF_V(v)        BM_SSI_SR_RFHF
#define BP_SSI_SR_UNDR              1
#define BM_SSI_SR_UNDR              0x2
#define BF_SSI_SR_UNDR(v)           (((v) & 0x1) << 1)
#define BFM_SSI_SR_UNDR(v)          BM_SSI_SR_UNDR
#define BF_SSI_SR_UNDR_V(e)         BF_SSI_SR_UNDR(BV_SSI_SR_UNDR__##e)
#define BFM_SSI_SR_UNDR_V(v)        BM_SSI_SR_UNDR
#define BP_SSI_SR_OVER              0
#define BM_SSI_SR_OVER              0x1
#define BF_SSI_SR_OVER(v)           (((v) & 0x1) << 0)
#define BFM_SSI_SR_OVER(v)          BM_SSI_SR_OVER
#define BF_SSI_SR_OVER_V(e)         BF_SSI_SR_OVER(BV_SSI_SR_OVER__##e)
#define BFM_SSI_SR_OVER_V(v)        BM_SSI_SR_OVER

#define REG_SSI_ITR             jz_reg(SSI_ITR)
#define JA_SSI_ITR              (0xb0043000 + 0x10)
#define JT_SSI_ITR              JIO_32_RW
#define JN_SSI_ITR              SSI_ITR
#define JI_SSI_ITR              
#define BP_SSI_ITR_IVLTM        0
#define BM_SSI_ITR_IVLTM        0x7fff
#define BF_SSI_ITR_IVLTM(v)     (((v) & 0x7fff) << 0)
#define BFM_SSI_ITR_IVLTM(v)    BM_SSI_ITR_IVLTM
#define BF_SSI_ITR_IVLTM_V(e)   BF_SSI_ITR_IVLTM(BV_SSI_ITR_IVLTM__##e)
#define BFM_SSI_ITR_IVLTM_V(v)  BM_SSI_ITR_IVLTM
#define BP_SSI_ITR_CNTCLK       15
#define BM_SSI_ITR_CNTCLK       0x8000
#define BF_SSI_ITR_CNTCLK(v)    (((v) & 0x1) << 15)
#define BFM_SSI_ITR_CNTCLK(v)   BM_SSI_ITR_CNTCLK
#define BF_SSI_ITR_CNTCLK_V(e)  BF_SSI_ITR_CNTCLK(BV_SSI_ITR_CNTCLK__##e)
#define BFM_SSI_ITR_CNTCLK_V(v) BM_SSI_ITR_CNTCLK

#define REG_SSI_ICR jz_reg(SSI_ICR)
#define JA_SSI_ICR  (0xb0043000 + 0x14)
#define JT_SSI_ICR  JIO_32_RW
#define JN_SSI_ICR  SSI_ICR
#define JI_SSI_ICR  

#define REG_SSI_GR  jz_reg(SSI_GR)
#define JA_SSI_GR   (0xb0043000 + 0x18)
#define JT_SSI_GR   JIO_32_RW
#define JN_SSI_GR   SSI_GR
#define JI_SSI_GR   

#define REG_SSI_RCNT    jz_reg(SSI_RCNT)
#define JA_SSI_RCNT     (0xb0043000 + 0x1c)
#define JT_SSI_RCNT     JIO_32_RW
#define JN_SSI_RCNT     SSI_RCNT
#define JI_SSI_RCNT     

#endif /* __HEADERGEN_SSI_H__*/
