/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 3.0.0
 * stmp3700 version: 2.4.0
 * stmp3700 authors: Amaury Pouly
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
#ifndef __HEADERGEN_STMP3700_DRI_H__
#define __HEADERGEN_STMP3700_DRI_H__

#define HW_DRI_CTRL                                     HW(DRI_CTRL)
#define HWA_DRI_CTRL                                    (0x80074000 + 0x0)
#define HWT_DRI_CTRL                                    HWIO_32_RW
#define HWN_DRI_CTRL                                    DRI_CTRL
#define HWI_DRI_CTRL                                    
#define HW_DRI_CTRL_SET                                 HW(DRI_CTRL_SET)
#define HWA_DRI_CTRL_SET                                (HWA_DRI_CTRL + 0x4)
#define HWT_DRI_CTRL_SET                                HWIO_32_WO
#define HWN_DRI_CTRL_SET                                DRI_CTRL
#define HWI_DRI_CTRL_SET                                
#define HW_DRI_CTRL_CLR                                 HW(DRI_CTRL_CLR)
#define HWA_DRI_CTRL_CLR                                (HWA_DRI_CTRL + 0x8)
#define HWT_DRI_CTRL_CLR                                HWIO_32_WO
#define HWN_DRI_CTRL_CLR                                DRI_CTRL
#define HWI_DRI_CTRL_CLR                                
#define HW_DRI_CTRL_TOG                                 HW(DRI_CTRL_TOG)
#define HWA_DRI_CTRL_TOG                                (HWA_DRI_CTRL + 0xc)
#define HWT_DRI_CTRL_TOG                                HWIO_32_WO
#define HWN_DRI_CTRL_TOG                                DRI_CTRL
#define HWI_DRI_CTRL_TOG                                
#define BP_DRI_CTRL_SFTRST                              31
#define BM_DRI_CTRL_SFTRST                              0x80000000
#define BV_DRI_CTRL_SFTRST__RUN                         0x0
#define BV_DRI_CTRL_SFTRST__RESET                       0x1
#define BF_DRI_CTRL_SFTRST(v)                           (((v) & 0x1) << 31)
#define BFM_DRI_CTRL_SFTRST(v)                          BM_DRI_CTRL_SFTRST
#define BF_DRI_CTRL_SFTRST_V(e)                         BF_DRI_CTRL_SFTRST(BV_DRI_CTRL_SFTRST__##e)
#define BFM_DRI_CTRL_SFTRST_V(v)                        BM_DRI_CTRL_SFTRST
#define BP_DRI_CTRL_CLKGATE                             30
#define BM_DRI_CTRL_CLKGATE                             0x40000000
#define BV_DRI_CTRL_CLKGATE__RUN                        0x0
#define BV_DRI_CTRL_CLKGATE__NO_CLKS                    0x1
#define BF_DRI_CTRL_CLKGATE(v)                          (((v) & 0x1) << 30)
#define BFM_DRI_CTRL_CLKGATE(v)                         BM_DRI_CTRL_CLKGATE
#define BF_DRI_CTRL_CLKGATE_V(e)                        BF_DRI_CTRL_CLKGATE(BV_DRI_CTRL_CLKGATE__##e)
#define BFM_DRI_CTRL_CLKGATE_V(v)                       BM_DRI_CTRL_CLKGATE
#define BP_DRI_CTRL_ENABLE_INPUTS                       29
#define BM_DRI_CTRL_ENABLE_INPUTS                       0x20000000
#define BV_DRI_CTRL_ENABLE_INPUTS__ANALOG_LINE_IN       0x0
#define BV_DRI_CTRL_ENABLE_INPUTS__DRI_DIGITAL_IN       0x1
#define BF_DRI_CTRL_ENABLE_INPUTS(v)                    (((v) & 0x1) << 29)
#define BFM_DRI_CTRL_ENABLE_INPUTS(v)                   BM_DRI_CTRL_ENABLE_INPUTS
#define BF_DRI_CTRL_ENABLE_INPUTS_V(e)                  BF_DRI_CTRL_ENABLE_INPUTS(BV_DRI_CTRL_ENABLE_INPUTS__##e)
#define BFM_DRI_CTRL_ENABLE_INPUTS_V(v)                 BM_DRI_CTRL_ENABLE_INPUTS
#define BP_DRI_CTRL_STOP_ON_OFLOW_ERROR                 26
#define BM_DRI_CTRL_STOP_ON_OFLOW_ERROR                 0x4000000
#define BV_DRI_CTRL_STOP_ON_OFLOW_ERROR__IGNORE         0x0
#define BV_DRI_CTRL_STOP_ON_OFLOW_ERROR__STOP           0x1
#define BF_DRI_CTRL_STOP_ON_OFLOW_ERROR(v)              (((v) & 0x1) << 26)
#define BFM_DRI_CTRL_STOP_ON_OFLOW_ERROR(v)             BM_DRI_CTRL_STOP_ON_OFLOW_ERROR
#define BF_DRI_CTRL_STOP_ON_OFLOW_ERROR_V(e)            BF_DRI_CTRL_STOP_ON_OFLOW_ERROR(BV_DRI_CTRL_STOP_ON_OFLOW_ERROR__##e)
#define BFM_DRI_CTRL_STOP_ON_OFLOW_ERROR_V(v)           BM_DRI_CTRL_STOP_ON_OFLOW_ERROR
#define BP_DRI_CTRL_STOP_ON_PILOT_ERROR                 25
#define BM_DRI_CTRL_STOP_ON_PILOT_ERROR                 0x2000000
#define BV_DRI_CTRL_STOP_ON_PILOT_ERROR__IGNORE         0x0
#define BV_DRI_CTRL_STOP_ON_PILOT_ERROR__STOP           0x1
#define BF_DRI_CTRL_STOP_ON_PILOT_ERROR(v)              (((v) & 0x1) << 25)
#define BFM_DRI_CTRL_STOP_ON_PILOT_ERROR(v)             BM_DRI_CTRL_STOP_ON_PILOT_ERROR
#define BF_DRI_CTRL_STOP_ON_PILOT_ERROR_V(e)            BF_DRI_CTRL_STOP_ON_PILOT_ERROR(BV_DRI_CTRL_STOP_ON_PILOT_ERROR__##e)
#define BFM_DRI_CTRL_STOP_ON_PILOT_ERROR_V(v)           BM_DRI_CTRL_STOP_ON_PILOT_ERROR
#define BP_DRI_CTRL_DMA_DELAY_COUNT                     16
#define BM_DRI_CTRL_DMA_DELAY_COUNT                     0x1f0000
#define BF_DRI_CTRL_DMA_DELAY_COUNT(v)                  (((v) & 0x1f) << 16)
#define BFM_DRI_CTRL_DMA_DELAY_COUNT(v)                 BM_DRI_CTRL_DMA_DELAY_COUNT
#define BF_DRI_CTRL_DMA_DELAY_COUNT_V(e)                BF_DRI_CTRL_DMA_DELAY_COUNT(BV_DRI_CTRL_DMA_DELAY_COUNT__##e)
#define BFM_DRI_CTRL_DMA_DELAY_COUNT_V(v)               BM_DRI_CTRL_DMA_DELAY_COUNT
#define BP_DRI_CTRL_REACQUIRE_PHASE                     15
#define BM_DRI_CTRL_REACQUIRE_PHASE                     0x8000
#define BV_DRI_CTRL_REACQUIRE_PHASE__NORMAL             0x0
#define BV_DRI_CTRL_REACQUIRE_PHASE__NEW_PHASE          0x1
#define BF_DRI_CTRL_REACQUIRE_PHASE(v)                  (((v) & 0x1) << 15)
#define BFM_DRI_CTRL_REACQUIRE_PHASE(v)                 BM_DRI_CTRL_REACQUIRE_PHASE
#define BF_DRI_CTRL_REACQUIRE_PHASE_V(e)                BF_DRI_CTRL_REACQUIRE_PHASE(BV_DRI_CTRL_REACQUIRE_PHASE__##e)
#define BFM_DRI_CTRL_REACQUIRE_PHASE_V(v)               BM_DRI_CTRL_REACQUIRE_PHASE
#define BP_DRI_CTRL_OVERFLOW_IRQ_EN                     11
#define BM_DRI_CTRL_OVERFLOW_IRQ_EN                     0x800
#define BV_DRI_CTRL_OVERFLOW_IRQ_EN__DISABLED           0x0
#define BV_DRI_CTRL_OVERFLOW_IRQ_EN__ENABLED            0x1
#define BF_DRI_CTRL_OVERFLOW_IRQ_EN(v)                  (((v) & 0x1) << 11)
#define BFM_DRI_CTRL_OVERFLOW_IRQ_EN(v)                 BM_DRI_CTRL_OVERFLOW_IRQ_EN
#define BF_DRI_CTRL_OVERFLOW_IRQ_EN_V(e)                BF_DRI_CTRL_OVERFLOW_IRQ_EN(BV_DRI_CTRL_OVERFLOW_IRQ_EN__##e)
#define BFM_DRI_CTRL_OVERFLOW_IRQ_EN_V(v)               BM_DRI_CTRL_OVERFLOW_IRQ_EN
#define BP_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN              10
#define BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN              0x400
#define BV_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN__DISABLED    0x0
#define BV_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN__ENABLED     0x1
#define BF_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN(v)           (((v) & 0x1) << 10)
#define BFM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN(v)          BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN
#define BF_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN_V(e)         BF_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN(BV_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN__##e)
#define BFM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN_V(v)        BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN
#define BP_DRI_CTRL_ATTENTION_IRQ_EN                    9
#define BM_DRI_CTRL_ATTENTION_IRQ_EN                    0x200
#define BV_DRI_CTRL_ATTENTION_IRQ_EN__DISABLED          0x0
#define BV_DRI_CTRL_ATTENTION_IRQ_EN__ENABLED           0x1
#define BF_DRI_CTRL_ATTENTION_IRQ_EN(v)                 (((v) & 0x1) << 9)
#define BFM_DRI_CTRL_ATTENTION_IRQ_EN(v)                BM_DRI_CTRL_ATTENTION_IRQ_EN
#define BF_DRI_CTRL_ATTENTION_IRQ_EN_V(e)               BF_DRI_CTRL_ATTENTION_IRQ_EN(BV_DRI_CTRL_ATTENTION_IRQ_EN__##e)
#define BFM_DRI_CTRL_ATTENTION_IRQ_EN_V(v)              BM_DRI_CTRL_ATTENTION_IRQ_EN
#define BP_DRI_CTRL_OVERFLOW_IRQ                        3
#define BM_DRI_CTRL_OVERFLOW_IRQ                        0x8
#define BV_DRI_CTRL_OVERFLOW_IRQ__NO_REQUEST            0x0
#define BV_DRI_CTRL_OVERFLOW_IRQ__REQUEST               0x1
#define BF_DRI_CTRL_OVERFLOW_IRQ(v)                     (((v) & 0x1) << 3)
#define BFM_DRI_CTRL_OVERFLOW_IRQ(v)                    BM_DRI_CTRL_OVERFLOW_IRQ
#define BF_DRI_CTRL_OVERFLOW_IRQ_V(e)                   BF_DRI_CTRL_OVERFLOW_IRQ(BV_DRI_CTRL_OVERFLOW_IRQ__##e)
#define BFM_DRI_CTRL_OVERFLOW_IRQ_V(v)                  BM_DRI_CTRL_OVERFLOW_IRQ
#define BP_DRI_CTRL_PILOT_SYNC_LOSS_IRQ                 2
#define BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ                 0x4
#define BV_DRI_CTRL_PILOT_SYNC_LOSS_IRQ__NO_REQUEST     0x0
#define BV_DRI_CTRL_PILOT_SYNC_LOSS_IRQ__REQUEST        0x1
#define BF_DRI_CTRL_PILOT_SYNC_LOSS_IRQ(v)              (((v) & 0x1) << 2)
#define BFM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ(v)             BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ
#define BF_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_V(e)            BF_DRI_CTRL_PILOT_SYNC_LOSS_IRQ(BV_DRI_CTRL_PILOT_SYNC_LOSS_IRQ__##e)
#define BFM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_V(v)           BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ
#define BP_DRI_CTRL_ATTENTION_IRQ                       1
#define BM_DRI_CTRL_ATTENTION_IRQ                       0x2
#define BV_DRI_CTRL_ATTENTION_IRQ__NO_REQUEST           0x0
#define BV_DRI_CTRL_ATTENTION_IRQ__REQUEST              0x1
#define BF_DRI_CTRL_ATTENTION_IRQ(v)                    (((v) & 0x1) << 1)
#define BFM_DRI_CTRL_ATTENTION_IRQ(v)                   BM_DRI_CTRL_ATTENTION_IRQ
#define BF_DRI_CTRL_ATTENTION_IRQ_V(e)                  BF_DRI_CTRL_ATTENTION_IRQ(BV_DRI_CTRL_ATTENTION_IRQ__##e)
#define BFM_DRI_CTRL_ATTENTION_IRQ_V(v)                 BM_DRI_CTRL_ATTENTION_IRQ
#define BP_DRI_CTRL_RUN                                 0
#define BM_DRI_CTRL_RUN                                 0x1
#define BV_DRI_CTRL_RUN__HALT                           0x0
#define BV_DRI_CTRL_RUN__RUN                            0x1
#define BF_DRI_CTRL_RUN(v)                              (((v) & 0x1) << 0)
#define BFM_DRI_CTRL_RUN(v)                             BM_DRI_CTRL_RUN
#define BF_DRI_CTRL_RUN_V(e)                            BF_DRI_CTRL_RUN(BV_DRI_CTRL_RUN__##e)
#define BFM_DRI_CTRL_RUN_V(v)                           BM_DRI_CTRL_RUN

#define HW_DRI_TIMING                               HW(DRI_TIMING)
#define HWA_DRI_TIMING                              (0x80074000 + 0x10)
#define HWT_DRI_TIMING                              HWIO_32_RW
#define HWN_DRI_TIMING                              DRI_TIMING
#define HWI_DRI_TIMING                              
#define BP_DRI_TIMING_PILOT_REP_RATE                16
#define BM_DRI_TIMING_PILOT_REP_RATE                0xf0000
#define BF_DRI_TIMING_PILOT_REP_RATE(v)             (((v) & 0xf) << 16)
#define BFM_DRI_TIMING_PILOT_REP_RATE(v)            BM_DRI_TIMING_PILOT_REP_RATE
#define BF_DRI_TIMING_PILOT_REP_RATE_V(e)           BF_DRI_TIMING_PILOT_REP_RATE(BV_DRI_TIMING_PILOT_REP_RATE__##e)
#define BFM_DRI_TIMING_PILOT_REP_RATE_V(v)          BM_DRI_TIMING_PILOT_REP_RATE
#define BP_DRI_TIMING_GAP_DETECTION_INTERVAL        0
#define BM_DRI_TIMING_GAP_DETECTION_INTERVAL        0xff
#define BF_DRI_TIMING_GAP_DETECTION_INTERVAL(v)     (((v) & 0xff) << 0)
#define BFM_DRI_TIMING_GAP_DETECTION_INTERVAL(v)    BM_DRI_TIMING_GAP_DETECTION_INTERVAL
#define BF_DRI_TIMING_GAP_DETECTION_INTERVAL_V(e)   BF_DRI_TIMING_GAP_DETECTION_INTERVAL(BV_DRI_TIMING_GAP_DETECTION_INTERVAL__##e)
#define BFM_DRI_TIMING_GAP_DETECTION_INTERVAL_V(v)  BM_DRI_TIMING_GAP_DETECTION_INTERVAL

#define HW_DRI_STAT                                         HW(DRI_STAT)
#define HWA_DRI_STAT                                        (0x80074000 + 0x20)
#define HWT_DRI_STAT                                        HWIO_32_RW
#define HWN_DRI_STAT                                        DRI_STAT
#define HWI_DRI_STAT                                        
#define BP_DRI_STAT_DRI_PRESENT                             31
#define BM_DRI_STAT_DRI_PRESENT                             0x80000000
#define BV_DRI_STAT_DRI_PRESENT__UNAVAILABLE                0x0
#define BV_DRI_STAT_DRI_PRESENT__AVAILABLE                  0x1
#define BF_DRI_STAT_DRI_PRESENT(v)                          (((v) & 0x1) << 31)
#define BFM_DRI_STAT_DRI_PRESENT(v)                         BM_DRI_STAT_DRI_PRESENT
#define BF_DRI_STAT_DRI_PRESENT_V(e)                        BF_DRI_STAT_DRI_PRESENT(BV_DRI_STAT_DRI_PRESENT__##e)
#define BFM_DRI_STAT_DRI_PRESENT_V(v)                       BM_DRI_STAT_DRI_PRESENT
#define BP_DRI_STAT_PILOT_PHASE                             16
#define BM_DRI_STAT_PILOT_PHASE                             0xf0000
#define BF_DRI_STAT_PILOT_PHASE(v)                          (((v) & 0xf) << 16)
#define BFM_DRI_STAT_PILOT_PHASE(v)                         BM_DRI_STAT_PILOT_PHASE
#define BF_DRI_STAT_PILOT_PHASE_V(e)                        BF_DRI_STAT_PILOT_PHASE(BV_DRI_STAT_PILOT_PHASE__##e)
#define BFM_DRI_STAT_PILOT_PHASE_V(v)                       BM_DRI_STAT_PILOT_PHASE
#define BP_DRI_STAT_OVERFLOW_IRQ_SUMMARY                    3
#define BM_DRI_STAT_OVERFLOW_IRQ_SUMMARY                    0x8
#define BV_DRI_STAT_OVERFLOW_IRQ_SUMMARY__NO_REQUEST        0x0
#define BV_DRI_STAT_OVERFLOW_IRQ_SUMMARY__REQUEST           0x1
#define BF_DRI_STAT_OVERFLOW_IRQ_SUMMARY(v)                 (((v) & 0x1) << 3)
#define BFM_DRI_STAT_OVERFLOW_IRQ_SUMMARY(v)                BM_DRI_STAT_OVERFLOW_IRQ_SUMMARY
#define BF_DRI_STAT_OVERFLOW_IRQ_SUMMARY_V(e)               BF_DRI_STAT_OVERFLOW_IRQ_SUMMARY(BV_DRI_STAT_OVERFLOW_IRQ_SUMMARY__##e)
#define BFM_DRI_STAT_OVERFLOW_IRQ_SUMMARY_V(v)              BM_DRI_STAT_OVERFLOW_IRQ_SUMMARY
#define BP_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY             2
#define BM_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY             0x4
#define BV_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY__NO_REQUEST 0x0
#define BV_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY__REQUEST    0x1
#define BF_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY(v)          (((v) & 0x1) << 2)
#define BFM_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY(v)         BM_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY
#define BF_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY_V(e)        BF_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY(BV_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY__##e)
#define BFM_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY_V(v)       BM_DRI_STAT_PILOT_SYNC_LOSS_IRQ_SUMMARY
#define BP_DRI_STAT_ATTENTION_IRQ_SUMMARY                   1
#define BM_DRI_STAT_ATTENTION_IRQ_SUMMARY                   0x2
#define BV_DRI_STAT_ATTENTION_IRQ_SUMMARY__NO_REQUEST       0x0
#define BV_DRI_STAT_ATTENTION_IRQ_SUMMARY__REQUEST          0x1
#define BF_DRI_STAT_ATTENTION_IRQ_SUMMARY(v)                (((v) & 0x1) << 1)
#define BFM_DRI_STAT_ATTENTION_IRQ_SUMMARY(v)               BM_DRI_STAT_ATTENTION_IRQ_SUMMARY
#define BF_DRI_STAT_ATTENTION_IRQ_SUMMARY_V(e)              BF_DRI_STAT_ATTENTION_IRQ_SUMMARY(BV_DRI_STAT_ATTENTION_IRQ_SUMMARY__##e)
#define BFM_DRI_STAT_ATTENTION_IRQ_SUMMARY_V(v)             BM_DRI_STAT_ATTENTION_IRQ_SUMMARY

#define HW_DRI_DATA             HW(DRI_DATA)
#define HWA_DRI_DATA            (0x80074000 + 0x30)
#define HWT_DRI_DATA            HWIO_32_RW
#define HWN_DRI_DATA            DRI_DATA
#define HWI_DRI_DATA            
#define BP_DRI_DATA_DATA        0
#define BM_DRI_DATA_DATA        0xffffffff
#define BF_DRI_DATA_DATA(v)     (((v) & 0xffffffff) << 0)
#define BFM_DRI_DATA_DATA(v)    BM_DRI_DATA_DATA
#define BF_DRI_DATA_DATA_V(e)   BF_DRI_DATA_DATA(BV_DRI_DATA_DATA__##e)
#define BFM_DRI_DATA_DATA_V(v)  BM_DRI_DATA_DATA

#define HW_DRI_DEBUG0                               HW(DRI_DEBUG0)
#define HWA_DRI_DEBUG0                              (0x80074000 + 0x40)
#define HWT_DRI_DEBUG0                              HWIO_32_RW
#define HWN_DRI_DEBUG0                              DRI_DEBUG0
#define HWI_DRI_DEBUG0                              
#define HW_DRI_DEBUG0_SET                           HW(DRI_DEBUG0_SET)
#define HWA_DRI_DEBUG0_SET                          (HWA_DRI_DEBUG0 + 0x4)
#define HWT_DRI_DEBUG0_SET                          HWIO_32_WO
#define HWN_DRI_DEBUG0_SET                          DRI_DEBUG0
#define HWI_DRI_DEBUG0_SET                          
#define HW_DRI_DEBUG0_CLR                           HW(DRI_DEBUG0_CLR)
#define HWA_DRI_DEBUG0_CLR                          (HWA_DRI_DEBUG0 + 0x8)
#define HWT_DRI_DEBUG0_CLR                          HWIO_32_WO
#define HWN_DRI_DEBUG0_CLR                          DRI_DEBUG0
#define HWI_DRI_DEBUG0_CLR                          
#define HW_DRI_DEBUG0_TOG                           HW(DRI_DEBUG0_TOG)
#define HWA_DRI_DEBUG0_TOG                          (HWA_DRI_DEBUG0 + 0xc)
#define HWT_DRI_DEBUG0_TOG                          HWIO_32_WO
#define HWN_DRI_DEBUG0_TOG                          DRI_DEBUG0
#define HWI_DRI_DEBUG0_TOG                          
#define BP_DRI_DEBUG0_DMAREQ                        31
#define BM_DRI_DEBUG0_DMAREQ                        0x80000000
#define BF_DRI_DEBUG0_DMAREQ(v)                     (((v) & 0x1) << 31)
#define BFM_DRI_DEBUG0_DMAREQ(v)                    BM_DRI_DEBUG0_DMAREQ
#define BF_DRI_DEBUG0_DMAREQ_V(e)                   BF_DRI_DEBUG0_DMAREQ(BV_DRI_DEBUG0_DMAREQ__##e)
#define BFM_DRI_DEBUG0_DMAREQ_V(v)                  BM_DRI_DEBUG0_DMAREQ
#define BP_DRI_DEBUG0_DMACMDKICK                    30
#define BM_DRI_DEBUG0_DMACMDKICK                    0x40000000
#define BF_DRI_DEBUG0_DMACMDKICK(v)                 (((v) & 0x1) << 30)
#define BFM_DRI_DEBUG0_DMACMDKICK(v)                BM_DRI_DEBUG0_DMACMDKICK
#define BF_DRI_DEBUG0_DMACMDKICK_V(e)               BF_DRI_DEBUG0_DMACMDKICK(BV_DRI_DEBUG0_DMACMDKICK__##e)
#define BFM_DRI_DEBUG0_DMACMDKICK_V(v)              BM_DRI_DEBUG0_DMACMDKICK
#define BP_DRI_DEBUG0_DRI_CLK_INPUT                 29
#define BM_DRI_DEBUG0_DRI_CLK_INPUT                 0x20000000
#define BF_DRI_DEBUG0_DRI_CLK_INPUT(v)              (((v) & 0x1) << 29)
#define BFM_DRI_DEBUG0_DRI_CLK_INPUT(v)             BM_DRI_DEBUG0_DRI_CLK_INPUT
#define BF_DRI_DEBUG0_DRI_CLK_INPUT_V(e)            BF_DRI_DEBUG0_DRI_CLK_INPUT(BV_DRI_DEBUG0_DRI_CLK_INPUT__##e)
#define BFM_DRI_DEBUG0_DRI_CLK_INPUT_V(v)           BM_DRI_DEBUG0_DRI_CLK_INPUT
#define BP_DRI_DEBUG0_DRI_DATA_INPUT                28
#define BM_DRI_DEBUG0_DRI_DATA_INPUT                0x10000000
#define BF_DRI_DEBUG0_DRI_DATA_INPUT(v)             (((v) & 0x1) << 28)
#define BFM_DRI_DEBUG0_DRI_DATA_INPUT(v)            BM_DRI_DEBUG0_DRI_DATA_INPUT
#define BF_DRI_DEBUG0_DRI_DATA_INPUT_V(e)           BF_DRI_DEBUG0_DRI_DATA_INPUT(BV_DRI_DEBUG0_DRI_DATA_INPUT__##e)
#define BFM_DRI_DEBUG0_DRI_DATA_INPUT_V(v)          BM_DRI_DEBUG0_DRI_DATA_INPUT
#define BP_DRI_DEBUG0_TEST_MODE                     27
#define BM_DRI_DEBUG0_TEST_MODE                     0x8000000
#define BF_DRI_DEBUG0_TEST_MODE(v)                  (((v) & 0x1) << 27)
#define BFM_DRI_DEBUG0_TEST_MODE(v)                 BM_DRI_DEBUG0_TEST_MODE
#define BF_DRI_DEBUG0_TEST_MODE_V(e)                BF_DRI_DEBUG0_TEST_MODE(BV_DRI_DEBUG0_TEST_MODE__##e)
#define BFM_DRI_DEBUG0_TEST_MODE_V(v)               BM_DRI_DEBUG0_TEST_MODE
#define BP_DRI_DEBUG0_PILOT_REP_RATE                26
#define BM_DRI_DEBUG0_PILOT_REP_RATE                0x4000000
#define BV_DRI_DEBUG0_PILOT_REP_RATE__8_AT_4MHZ     0x0
#define BV_DRI_DEBUG0_PILOT_REP_RATE__12_AT_6MHZ    0x1
#define BF_DRI_DEBUG0_PILOT_REP_RATE(v)             (((v) & 0x1) << 26)
#define BFM_DRI_DEBUG0_PILOT_REP_RATE(v)            BM_DRI_DEBUG0_PILOT_REP_RATE
#define BF_DRI_DEBUG0_PILOT_REP_RATE_V(e)           BF_DRI_DEBUG0_PILOT_REP_RATE(BV_DRI_DEBUG0_PILOT_REP_RATE__##e)
#define BFM_DRI_DEBUG0_PILOT_REP_RATE_V(v)          BM_DRI_DEBUG0_PILOT_REP_RATE
#define BP_DRI_DEBUG0_SPARE                         18
#define BM_DRI_DEBUG0_SPARE                         0x3fc0000
#define BF_DRI_DEBUG0_SPARE(v)                      (((v) & 0xff) << 18)
#define BFM_DRI_DEBUG0_SPARE(v)                     BM_DRI_DEBUG0_SPARE
#define BF_DRI_DEBUG0_SPARE_V(e)                    BF_DRI_DEBUG0_SPARE(BV_DRI_DEBUG0_SPARE__##e)
#define BFM_DRI_DEBUG0_SPARE_V(v)                   BM_DRI_DEBUG0_SPARE
#define BP_DRI_DEBUG0_FRAME                         0
#define BM_DRI_DEBUG0_FRAME                         0x3ffff
#define BF_DRI_DEBUG0_FRAME(v)                      (((v) & 0x3ffff) << 0)
#define BFM_DRI_DEBUG0_FRAME(v)                     BM_DRI_DEBUG0_FRAME
#define BF_DRI_DEBUG0_FRAME_V(e)                    BF_DRI_DEBUG0_FRAME(BV_DRI_DEBUG0_FRAME__##e)
#define BFM_DRI_DEBUG0_FRAME_V(v)                   BM_DRI_DEBUG0_FRAME

#define HW_DRI_DEBUG1                               HW(DRI_DEBUG1)
#define HWA_DRI_DEBUG1                              (0x80074000 + 0x50)
#define HWT_DRI_DEBUG1                              HWIO_32_RW
#define HWN_DRI_DEBUG1                              DRI_DEBUG1
#define HWI_DRI_DEBUG1                              
#define HW_DRI_DEBUG1_SET                           HW(DRI_DEBUG1_SET)
#define HWA_DRI_DEBUG1_SET                          (HWA_DRI_DEBUG1 + 0x4)
#define HWT_DRI_DEBUG1_SET                          HWIO_32_WO
#define HWN_DRI_DEBUG1_SET                          DRI_DEBUG1
#define HWI_DRI_DEBUG1_SET                          
#define HW_DRI_DEBUG1_CLR                           HW(DRI_DEBUG1_CLR)
#define HWA_DRI_DEBUG1_CLR                          (HWA_DRI_DEBUG1 + 0x8)
#define HWT_DRI_DEBUG1_CLR                          HWIO_32_WO
#define HWN_DRI_DEBUG1_CLR                          DRI_DEBUG1
#define HWI_DRI_DEBUG1_CLR                          
#define HW_DRI_DEBUG1_TOG                           HW(DRI_DEBUG1_TOG)
#define HWA_DRI_DEBUG1_TOG                          (HWA_DRI_DEBUG1 + 0xc)
#define HWT_DRI_DEBUG1_TOG                          HWIO_32_WO
#define HWN_DRI_DEBUG1_TOG                          DRI_DEBUG1
#define HWI_DRI_DEBUG1_TOG                          
#define BP_DRI_DEBUG1_INVERT_PILOT                  31
#define BM_DRI_DEBUG1_INVERT_PILOT                  0x80000000
#define BV_DRI_DEBUG1_INVERT_PILOT__NORMAL          0x0
#define BV_DRI_DEBUG1_INVERT_PILOT__INVERTED        0x1
#define BF_DRI_DEBUG1_INVERT_PILOT(v)               (((v) & 0x1) << 31)
#define BFM_DRI_DEBUG1_INVERT_PILOT(v)              BM_DRI_DEBUG1_INVERT_PILOT
#define BF_DRI_DEBUG1_INVERT_PILOT_V(e)             BF_DRI_DEBUG1_INVERT_PILOT(BV_DRI_DEBUG1_INVERT_PILOT__##e)
#define BFM_DRI_DEBUG1_INVERT_PILOT_V(v)            BM_DRI_DEBUG1_INVERT_PILOT
#define BP_DRI_DEBUG1_INVERT_ATTENTION              30
#define BM_DRI_DEBUG1_INVERT_ATTENTION              0x40000000
#define BV_DRI_DEBUG1_INVERT_ATTENTION__NORMAL      0x0
#define BV_DRI_DEBUG1_INVERT_ATTENTION__INVERTED    0x1
#define BF_DRI_DEBUG1_INVERT_ATTENTION(v)           (((v) & 0x1) << 30)
#define BFM_DRI_DEBUG1_INVERT_ATTENTION(v)          BM_DRI_DEBUG1_INVERT_ATTENTION
#define BF_DRI_DEBUG1_INVERT_ATTENTION_V(e)         BF_DRI_DEBUG1_INVERT_ATTENTION(BV_DRI_DEBUG1_INVERT_ATTENTION__##e)
#define BFM_DRI_DEBUG1_INVERT_ATTENTION_V(v)        BM_DRI_DEBUG1_INVERT_ATTENTION
#define BP_DRI_DEBUG1_INVERT_DRI_DATA               29
#define BM_DRI_DEBUG1_INVERT_DRI_DATA               0x20000000
#define BV_DRI_DEBUG1_INVERT_DRI_DATA__NORMAL       0x0
#define BV_DRI_DEBUG1_INVERT_DRI_DATA__INVERTED     0x1
#define BF_DRI_DEBUG1_INVERT_DRI_DATA(v)            (((v) & 0x1) << 29)
#define BFM_DRI_DEBUG1_INVERT_DRI_DATA(v)           BM_DRI_DEBUG1_INVERT_DRI_DATA
#define BF_DRI_DEBUG1_INVERT_DRI_DATA_V(e)          BF_DRI_DEBUG1_INVERT_DRI_DATA(BV_DRI_DEBUG1_INVERT_DRI_DATA__##e)
#define BFM_DRI_DEBUG1_INVERT_DRI_DATA_V(v)         BM_DRI_DEBUG1_INVERT_DRI_DATA
#define BP_DRI_DEBUG1_INVERT_DRI_CLOCK              28
#define BM_DRI_DEBUG1_INVERT_DRI_CLOCK              0x10000000
#define BV_DRI_DEBUG1_INVERT_DRI_CLOCK__NORMAL      0x0
#define BV_DRI_DEBUG1_INVERT_DRI_CLOCK__INVERTED    0x1
#define BF_DRI_DEBUG1_INVERT_DRI_CLOCK(v)           (((v) & 0x1) << 28)
#define BFM_DRI_DEBUG1_INVERT_DRI_CLOCK(v)          BM_DRI_DEBUG1_INVERT_DRI_CLOCK
#define BF_DRI_DEBUG1_INVERT_DRI_CLOCK_V(e)         BF_DRI_DEBUG1_INVERT_DRI_CLOCK(BV_DRI_DEBUG1_INVERT_DRI_CLOCK__##e)
#define BFM_DRI_DEBUG1_INVERT_DRI_CLOCK_V(v)        BM_DRI_DEBUG1_INVERT_DRI_CLOCK
#define BP_DRI_DEBUG1_REVERSE_FRAME                 27
#define BM_DRI_DEBUG1_REVERSE_FRAME                 0x8000000
#define BV_DRI_DEBUG1_REVERSE_FRAME__NORMAL         0x0
#define BV_DRI_DEBUG1_REVERSE_FRAME__REVERSED       0x1
#define BF_DRI_DEBUG1_REVERSE_FRAME(v)              (((v) & 0x1) << 27)
#define BFM_DRI_DEBUG1_REVERSE_FRAME(v)             BM_DRI_DEBUG1_REVERSE_FRAME
#define BF_DRI_DEBUG1_REVERSE_FRAME_V(e)            BF_DRI_DEBUG1_REVERSE_FRAME(BV_DRI_DEBUG1_REVERSE_FRAME__##e)
#define BFM_DRI_DEBUG1_REVERSE_FRAME_V(v)           BM_DRI_DEBUG1_REVERSE_FRAME
#define BP_DRI_DEBUG1_SWIZZLED_FRAME                0
#define BM_DRI_DEBUG1_SWIZZLED_FRAME                0x3ffff
#define BF_DRI_DEBUG1_SWIZZLED_FRAME(v)             (((v) & 0x3ffff) << 0)
#define BFM_DRI_DEBUG1_SWIZZLED_FRAME(v)            BM_DRI_DEBUG1_SWIZZLED_FRAME
#define BF_DRI_DEBUG1_SWIZZLED_FRAME_V(e)           BF_DRI_DEBUG1_SWIZZLED_FRAME(BV_DRI_DEBUG1_SWIZZLED_FRAME__##e)
#define BFM_DRI_DEBUG1_SWIZZLED_FRAME_V(v)          BM_DRI_DEBUG1_SWIZZLED_FRAME

#define HW_DRI_VERSION              HW(DRI_VERSION)
#define HWA_DRI_VERSION             (0x80074000 + 0x60)
#define HWT_DRI_VERSION             HWIO_32_RW
#define HWN_DRI_VERSION             DRI_VERSION
#define HWI_DRI_VERSION             
#define BP_DRI_VERSION_MAJOR        24
#define BM_DRI_VERSION_MAJOR        0xff000000
#define BF_DRI_VERSION_MAJOR(v)     (((v) & 0xff) << 24)
#define BFM_DRI_VERSION_MAJOR(v)    BM_DRI_VERSION_MAJOR
#define BF_DRI_VERSION_MAJOR_V(e)   BF_DRI_VERSION_MAJOR(BV_DRI_VERSION_MAJOR__##e)
#define BFM_DRI_VERSION_MAJOR_V(v)  BM_DRI_VERSION_MAJOR
#define BP_DRI_VERSION_MINOR        16
#define BM_DRI_VERSION_MINOR        0xff0000
#define BF_DRI_VERSION_MINOR(v)     (((v) & 0xff) << 16)
#define BFM_DRI_VERSION_MINOR(v)    BM_DRI_VERSION_MINOR
#define BF_DRI_VERSION_MINOR_V(e)   BF_DRI_VERSION_MINOR(BV_DRI_VERSION_MINOR__##e)
#define BFM_DRI_VERSION_MINOR_V(v)  BM_DRI_VERSION_MINOR
#define BP_DRI_VERSION_STEP         0
#define BM_DRI_VERSION_STEP         0xffff
#define BF_DRI_VERSION_STEP(v)      (((v) & 0xffff) << 0)
#define BFM_DRI_VERSION_STEP(v)     BM_DRI_VERSION_STEP
#define BF_DRI_VERSION_STEP_V(e)    BF_DRI_VERSION_STEP(BV_DRI_VERSION_STEP__##e)
#define BFM_DRI_VERSION_STEP_V(v)   BM_DRI_VERSION_STEP

#endif /* __HEADERGEN_STMP3700_DRI_H__*/
