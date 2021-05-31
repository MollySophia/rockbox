/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by James Espinoza
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
#ifndef __IMX31L_H__
#define __IMX31L_H__

/* Most(if not all) of these defines are copied from Nand-Boot v4 provided w/ the Imx31 Linux Bsp*/

#define REG8_PTR_T  volatile unsigned char *
#define REG16_PTR_T volatile unsigned short *
#define REG32_PTR_T volatile unsigned long *

/* Place in the section with the framebuffer */
#define TTB_BASE_ADDR (CSD0_BASE_ADDR + (MEMORYSIZE*0x100000) - TTB_SIZE)
#define TTB_SIZE      (0x4000)
#define IRAM_SIZE     (0x4000)
#define TTB_BASE      ((unsigned long *)TTB_BASE_ADDR)
#define FRAME_SIZE    (240*320*2)
/* Rockbox framebuffer address, not retail OS */
#define FRAME_PHYS_ADDR (TTB_BASE_ADDR - FRAME_SIZE)
#define FRAME         ((void *)(FRAME_PHYS_ADDR+0x100000-CSD0_BASE_ADDR))

#define CACHEALIGN_BITS     5
#define CACHEALIGN_SIZE     32
#define NOCACHE_BASE        CSD0_BASE_ADDR

/* USBOTG */
#define USB_QHARRAY_ATTR    __attribute__((section(".qharray"),nocommon,aligned(2048)))
#define USB_NUM_ENDPOINTS   8
#define USB_DEVBSS_ATTR     NOCACHEBSS_ATTR
#define USB_BASE            OTG_BASE_ADDR
#define QHARRAY_SIZE        ((64*USB_NUM_ENDPOINTS*2 + 2047) & (0xffffffff - 2047))
#define QHARRAY_PHYS_ADDR   ((FRAME_PHYS_ADDR - QHARRAY_SIZE) & (0xffffffff - 2047))

/*
 * AIPS 1
 */
#define IRAM_BASE_ADDR          0x1fffc000
#define L2CC_BASE_ADDR          0x30000000
#define AIPS1_BASE_ADDR         0x43F00000
#define AIPS1_CTRL_BASE_ADDR    AIPS1_BASE_ADDR
#define MAX_BASE_ADDR           0x43F04000
#define EVTMON_BASE_ADDR        0x43F08000
#define CLKCTL_BASE_ADDR        0x43F0C000
#define ETB_SLOT4_BASE_ADDR     0x43F10000
#define ETB_SLOT5_BASE_ADDR     0x43F14000
#define ECT_CTIO_BASE_ADDR      0x43F18000
#define I2C1_BASE_ADDR          0x43F80000
#define I2C3_BASE_ADDR          0x43F84000
#define OTG_BASE_ADDR           0x43F88000
#define ATA_BASE_ADDR           0x43F8C000
#define UART1_BASE_ADDR         0x43F90000
#define UART2_BASE_ADDR         0x43F94000
#define I2C2_BASE_ADDR          0x43F98000
#define OWIRE_BASE_ADDR         0x43F9C000
#define SSI1_BASE_ADDR          0x43FA0000
#define CSPI1_BASE_ADDR         0x43FA4000
#define KPP_BASE_ADDR           0x43FA8000
#define IOMUXC_BASE_ADDR        0x43FAC000
#define UART4_BASE_ADDR         0x43FB0000
#define UART5_BASE_ADDR         0x43FB4000
#define ECT_IP1_BASE_ADDR       0x43FB8000
#define ECT_IP2_BASE_ADDR       0x43FBC000
 
/*
 * SPBA
 */
#define SPBA_BASE_ADDR          0x50000000
#define MMC_SDHC1_BASE_ADDR     0x50004000
#define MMC_SDHC2_BASE_ADDR     0x50008000
#define UART3_BASE_ADDR         0x5000C000
#define CSPI2_BASE_ADDR         0x50010000
#define SSI2_BASE_ADDR          0x50014000
#define SIM_BASE_ADDR           0x50018000
#define IIM_BASE_ADDR           0x5001C000
#define ATA_DMA_BASE_ADDR       0x50020000
#define SPBA_CTRL_BASE_ADDR     0x5003C000

/*
 * AIPS 2
 */
#define AIPS2_BASE_ADDR         0x53F00000
#define AIPS2_CTRL_BASE_ADDR    AIPS2_BASE_ADDR
#define CCM_BASE_ADDR           0x53F80000
#define CSPI3_BASE_ADDR         0x53F84000
#define FIRI_BASE_ADDR          0x53F8C000
#define GPT1_BASE_ADDR          0x53F90000
#define EPIT1_BASE_ADDR         0x53F94000
#define EPIT2_BASE_ADDR         0x53F98000
#define GPIO3_BASE_ADDR         0x53FA4000
#define SCC_BASE                0x53FAC000
#define SCM_BASE                0x53FAE000
#define SMN_BASE                0x53FAF000
#define RNGA_BASE_ADDR          0x53FB0000
#define IPU_CTRL_BASE_ADDR      0x53FC0000
#define AUDMUX_BASE             0x53FC4000
#define MPEG4_ENC_BASE          0x53FC8000
#define GPIO1_BASE_ADDR         0x53FCC000
#define GPIO2_BASE_ADDR         0x53FD0000
#define SDMA_BASE_ADDR          0x53FD4000
#define RTC_BASE_ADDR           0x53FD8000
#define WDOG_BASE_ADDR          0x53FDC000
#define PWM_BASE_ADDR           0x53FE0000
#define RTIC_BASE_ADDR          0x53FEC000

#define WDOG1_BASE_ADDR         WDOG_BASE_ADDR
#define CRM_MCU_BASE_ADDR       CCM_BASE_ADDR

/* IIM */
#define IIM_PREV                (*(REG32_PTR_T)(IIM_BASE_ADDR + 0x20))
    #define IIM_PREV_SIG        (0x1f << 3)
    #define IIM_PREV_SIG_IMX31  (0x01 << 3) /* i.MX31 */
#define IIM_SREV                (*(REG32_PTR_T)(IIM_BASE_ADDR + 0x24))
    #define IIM_SREV_SREV       (0xff << 0)
    #define IIM_SREV_1_0        0x00 /* i.MX31/L 1.0, L38W */
    #define IIM_SREV_1_1        0x10 /* i.MX31   1.1, 2L38W */
    #define IIM_SREV_1_1L       0x11 /* i.MX31L  1.1, 2L38W */
    #define IIM_SREV_1_15       0x12 /* i.MX31   1.15, 2L38W/3L38W */
    #define IIM_SREV_1_15L      0x13 /* i.MX31L  1.15, 2L38W/3L38W */
    #define IIM_SREV_1_2        0x14 /* i.MX31   1.2, 3L38W, M45G */
    #define IIM_SREV_1_2L       0x15 /* i.MX31L  1.2, 3L38W, M45G */
    #define IIM_SREV_2_0_1      0x28 /* i.MX31   2.0/2.0.1, M91E */
    #define IIM_SREV_2_0_1L     0x29 /* i.MX31L  2.0/2.0.1, M91E */

/* IOMUXC */
#define IOMUXC_GPR              (*(REG32_PTR_T)(IOMUXC_BASE_ADDR+0x008))

/* SW_MUX_CTL_* */
#define IOMUXC_MUX_OUT              (0x7 << 4)
#define IOMUXC_MUX_OUT_POS          (4)
#define IOMUXC_MUX_IN               (0xf << 0)
#define IOMUXC_MUX_IN_POS           (0)
#define IOMUXC_MUX_MASK             (0x7f)

#define IOMUXC_MUX_OUT_GPIO         (0x0 << IOMUXC_MUX_OUT_POS)
#define IOMUXC_MUX_OUT_FUNCTIONAL   (0x1 << IOMUXC_MUX_OUT_POS)
#define IOMUXC_MUX_OUT_ALT1         (0x2 << IOMUXC_MUX_OUT_POS)
#define IOMUXC_MUX_OUT_ALT2         (0x3 << IOMUXC_MUX_OUT_POS)
#define IOMUXC_MUX_OUT_ALT3         (0x4 << IOMUXC_MUX_OUT_POS)
#define IOMUXC_MUX_OUT_ALT4         (0x5 << IOMUXC_MUX_OUT_POS)
#define IOMUXC_MUX_OUT_ALT5         (0x6 << IOMUXC_MUX_OUT_POS)
#define IOMUXC_MUX_OUT_ALT6         (0x7 << IOMUXC_MUX_OUT_POS)

#define IOMUXC_MUX_IN_NONE          (0x0 << IOMUXC_MUX_IN_POS)
#define IOMUXC_MUX_IN_GPIO          (0x1 << IOMUXC_MUX_IN_POS)
#define IOMUXC_MUX_IN_FUNCTIONAL    (0x2 << IOMUXC_MUX_IN_POS)
#define IOMUXC_MUX_IN_ALT1          (0x4 << IOMUXC_MUX_IN_POS)
#define IOMUXC_MUX_IN_ALT2          (0x8 << IOMUXC_MUX_IN_POS)

/* SW_PAD_CTL_* */
#define IOMUXC_PAD_LOOPBACK             (0x1 << 9) /* Route output to input */
/* Pullup, pulldown and keeper enable */
#define IOMUXC_PAD_PUE_PKE              (0x3 << 7)
#define IOMUXC_PAD_PUE_PKE_DISABLE      (0x0 << 7)
#define IOMUXC_PAD_PUE_PKE_DISABLE_2    (0x1 << 7) /* Same as 0x0 */
#define IOMUXC_PAD_PUE_PKE_KEEPER       (0x2 << 7)
#define IOMUXC_PAD_PUE_PKE_PULLUPDOWN   (0x3 << 7) /* Enb. Pull up or down */
/* Pullup/down resistance */
#define IOMUXC_PAD_PUS                  (0x3 << 5)
#define IOMUXC_PAD_PUS_DOWN_100K        (0x0 << 5)
#define IOMUXC_PAD_PUS_UP_100K          (0x1 << 5)
#if 0 /* Completeness */
#define IOMUXC_PAD_PUS_UP_47K           (0x2 << 5) /* Not in IMX31/L */
#define IOMUXC_PAD_PUS_UP_22K           (0x3 << 5) /* Not in IMX31/L */
#endif
#define IOMUXC_PAD_HYS                  (0x1 << 4) /* Schmitt trigger input */
#define IOMUXC_PAD_ODE                  (0x1 << 3) /* Open drain output 0=CMOS pushpull*/
#define IOMUXC_PAD_DSE                  (0x3 << 1)
#define IOMUXC_PAD_DSE_STD              (0x0 << 1) /* Drive strength */
#define IOMUXC_PAD_DSE_HIGH             (0x1 << 1)
#define IOMUXC_PAD_DSE_MAX              (0x2 << 1)
#define IOMUXC_PAD_DSE_MAX_2            (0x3 << 1) /* Same as 0x2 */
#define IOMUXC_PAD_SRE                  (0x1 << 0) /* Slew rate, 1=fast */

#define IOMUXC_PAD_MASK                 (0x3ff)

/* RNGA */
#define RNGA_CONTROL            (*(REG32_PTR_T)(RNGA_BASE_ADDR+0x00))

#define RNGA_CONTROL_SLEEP                          (1 << 4)

/* IPU */
#define IPU_IPU_CONF                (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x000))
#define IPU_IPU_CHA_BUF0_RDY        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x004))
#define IPU_IPU_CHA_BUF1_RDY        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x008))
#define IPU_IPU_CHA_DB_MODE_SEL     (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x00C))
#define IPU_IPU_CHA_CUR_BUF         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x010))
#define IPU_IPU_FS_PROC_FLOW        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x014))
#define IPU_IPU_FS_DISP_FLOW        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x018))
#define IPU_IPU_TASKS_STAT          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x01C))
#define IPU_IPU_IMA_ADDR            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x020))
#define IPU_IPU_IMA_DATA            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x024))
#define IPU_IPU_INT_CTRL_1          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x028))
#define IPU_IPU_INT_CTRL_2          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x02C))
#define IPU_IPU_INT_CTRL_3          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x030))
#define IPU_IPU_INT_CTRL_4          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x034))
#define IPU_IPU_INT_CTRL_5          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x038))
#define IPU_IPU_INT_STAT_1          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x03C))
#define IPU_IPU_INT_STAT_2          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x040))
#define IPU_IPU_INT_STAT_3          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x044))
#define IPU_IPU_INT_STAT_4          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x048))
#define IPU_IPU_INT_STAT_5          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x04C))
#define IPU_IPU_BRK_CTRL_1          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x050))
#define IPU_IPU_BRK_CTRL_2          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x054))
#define IPU_IPU_BRK_STAT            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x058))
#define IPU_IPU_DIAGB_CTRL          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x060))
#define IPU_CSI_SENS_FRM_SIZE       (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x064))
#define IPU_CSI_ACT_FRM_SIZE        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x068))
#define IPU_CSI_OUT_FRM_CTRL        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x06C))
#define IPU_CSI_TST_CTRL            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x070))
#define IPU_CSI_CCIR_CODE_1         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x074))
#define IPU_CSI_CCIR_CODE_2         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x078))
#define IPU_CSI_CCIR_CODE_3         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x07C))
#define IPU_CSI_FLASH_STROBE_1      (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x080))
#define IPU_CSI_FLASH_STROBE_2      (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x084))
#define IPU_IC_CONF                 (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x088))
#define IPU_IC_PRP_ENC_RSC          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x08C))
#define IPU_IC_PRP_VF_RSC           (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x090))
#define IPU_IC_PP_RSC               (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x094))
#define IPU_IC_CMBP_1               (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x098))
#define IPU_IC_CMBP_2               (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x09C))
#define IPU_PF_CONF                 (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0A0))
#define IPU_IDMAC_CONF              (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0A4))
#define IPU_IDMAC_CHA_EN            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0A8))
#define IPU_IDMAC_CHA_PRI           (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0AC))
#define IPU_IDMAC_CHA_BUSY          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0B0))
#define IPU_SDC_COM_CONF            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0B4))
#define IPU_SDC_GRAPH_WIND_CTRL     (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0B8))
#define IPU_SDC_FG_POS              (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0BC))
#define IPU_SDC_BG_POS              (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0C0))
#define IPU_SDC_CUR_POS             (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0C4))
#define IPU_SDC_CUR_BLINK_PWM_CTRL  (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0C8))
#define IPU_SDC_CUR_MAP             (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0CC))
#define IPU_SDC_HOR_CONF            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0D0))
#define IPU_SDC_VER_CONF            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0D4))
#define IPU_SDC_SHARP_CONF_1        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0D8))
#define IPU_SDC_SHARP_CONF_2        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0DC))
#define IPU_SDC_ADC_CONF            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0E0))
#define IPU_ADC_SYSCHA1_SA          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0E4))
#define IPU_ADC_SYSCHA2_SA          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0E8))
#define IPU_ADC_PRPCHAN_SA          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0EC))
#define IPU_ADC_PPCHAN_SA           (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0F0))
#define IPU_ADC_DISP0_CONF          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0F4))
#define IPU_ADC_DISP0_RD_AP         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0F8))
#define IPU_ADC_DISP0_RDM           (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x0FC))
#define IPU_ADC_DISP0_SS            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x100))
#define IPU_ADC_DISP1_CONF          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x104))
#define IPU_ADC_DISP1_RD_AP         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x108))
#define IPU_ADC_DISP1_RDM           (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x10C))
#define IPU_ADC_DISP2_SS            (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x110))
#define IPU_ADC_DISP2_CONF          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x114))
#define IPU_ADC_DISP2_RD_AP         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x118))
#define IPU_ADC_DISP2_RDM           (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x11C))
#define IPU_ADC_DISP_VSYNC          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x120))
#define IPU_ADC_DISP_IF_CONF        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x124))
#define IPU_ADC_DISP_SIG_POL        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x128))
#define IPU_DI_SER_DISP1_CONF       (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x12C))
#define IPU_DI_SER_DISP2_CONF       (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x130))
#define IPU_DI_HSP_CLK_PER          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x134))
#define IPU_DI_DISP0_TIME_CONF_1    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x138))
#define IPU_DI_DISP0_TIME_CONF_2    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x13C))
#define IPU_DI_DISP0_TIME_CONF_3    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x140))
#define IPU_DI_DISP1_TIME_CONF_1    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x144))
#define IPU_DI_DISP1_TIME_CONF_2    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x148))
#define IPU_DI_DISP1_TIME_CONF_3    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x14C))
#define IPU_DI_DISP2_TIME_CONF_1    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x150))
#define IPU_DI_DISP2_TIME_CONF_2    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x154))
#define IPU_DI_DISP2_TIME_CONF_3    (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x158))
#define IPU_DI_DISP3_TIME_CONF      (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x15C))
#define IPU_DI_DISP0_DB0_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x160))
#define IPU_DI_DISP0_DB1_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x164))
#define IPU_DI_DISP0_DB2_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x168))
#define IPU_DI_DISP0_CB0_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x16C))
#define IPU_DI_DISP0_CB1_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x170))
#define IPU_DI_DISP0_CB2_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x174))
#define IPU_DI_DISP1_DB0_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x178))
#define IPU_DI_DISP1_DB1_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x17C))
#define IPU_DI_DISP1_DB2_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x180))
#define IPU_DI_DISP1_CB0_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x184))
#define IPU_DI_DISP1_CB1_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x188))
#define IPU_DI_DISP1_CB2_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x18C))
#define IPU_DI_DISP2_DB0_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x190))
#define IPU_DI_DISP2_DB1_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x194))
#define IPU_DI_DISP2_DB2_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x198))
#define IPU_DI_DISP2_CB0_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x19C))
#define IPU_DI_DISP2_CB1_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1A0))
#define IPU_DI_DISP2_CB2_MAP        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1A4))
#define IPU_DI_DISP3_B0_MAP         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1A8))
#define IPU_DI_DISP3_B1_MAP         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1AC))
#define IPU_DI_DISP3_B2_MAP         (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1B0))
#define IPU_DI_DISP_ACC_CC          (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1B4))
#define IPU_DI_DISP_LLA_CONF        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1B8))
#define IPU_DI_DISP_LLA_DATA        (*(REG32_PTR_T)(IPU_CTRL_BASE_ADDR+0x1BC))

/* IPU_CONF */
#define IPU_IPU_CONF_PXL_ENDIAN     (1 << 8)
#define IPU_IPU_CONF_DU_EN          (1 << 7)
#define IPU_IPU_CONF_DI_EN          (1 << 6)
#define IPU_IPU_CONF_ADC_EN         (1 << 5)
#define IPU_IPU_CONF_SDC_EN         (1 << 4)
#define IPU_IPU_CONF_PF_EN          (1 << 3)
#define IPU_IPU_CONF_ROT_EN         (1 << 2)
#define IPU_IPU_CONF_IC_EN          (1 << 1)
#define IPU_IPU_CONF_CSI_EN         (1 << 0)

/* ATA */
#define ATA_TIME_OFF            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x00))
#define ATA_TIME_ON             (*(REG8_PTR_T)(ATA_BASE_ADDR+0x01))
    /* PIO */
#define ATA_TIME_1              (*(REG8_PTR_T)(ATA_BASE_ADDR+0x02))
#define ATA_TIME_2W             (*(REG8_PTR_T)(ATA_BASE_ADDR+0x03))
#define ATA_TIME_2R             (*(REG8_PTR_T)(ATA_BASE_ADDR+0x04))
#define ATA_TIME_AX             (*(REG8_PTR_T)(ATA_BASE_ADDR+0x05))
#define ATA_TIME_PIO_RDX        (*(REG8_PTR_T)(ATA_BASE_ADDR+0x06))
#define ATA_TIME_4              (*(REG8_PTR_T)(ATA_BASE_ADDR+0x07))
#define ATA_TIME_9              (*(REG8_PTR_T)(ATA_BASE_ADDR+0x08))
    /* MDMA */
#define ATA_TIME_M              (*(REG8_PTR_T)(ATA_BASE_ADDR+0x09))
#define ATA_TIME_JN             (*(REG8_PTR_T)(ATA_BASE_ADDR+0x0A))
#define ATA_TIME_D              (*(REG8_PTR_T)(ATA_BASE_ADDR+0x0B))
#define ATA_TIME_K              (*(REG8_PTR_T)(ATA_BASE_ADDR+0x0C))
    /* UDMA */
#define ATA_TIME_ACK            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x0D))
#define ATA_TIME_ENV            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x0E))
#define ATA_TIME_RPX            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x0F))
#define ATA_TIME_ZAH            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x10))
#define ATA_TIME_MLIX           (*(REG8_PTR_T)(ATA_BASE_ADDR+0x11))
#define ATA_TIME_DVH            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x12))
#define ATA_TIME_DZFS           (*(REG8_PTR_T)(ATA_BASE_ADDR+0x13))
#define ATA_TIME_DVS            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x14))
#define ATA_TIME_CVH            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x15))
#define ATA_TIME_SS             (*(REG8_PTR_T)(ATA_BASE_ADDR+0x16))
#define ATA_TIME_CYC            (*(REG8_PTR_T)(ATA_BASE_ADDR+0x17))
    /* */
#define ATA_FIFO_DATA_32        (*(REG32_PTR_T)(ATA_BASE_ADDR+0x18))
#define ATA_FIFO_DATA_16        (*(REG16_PTR_T)(ATA_BASE_ADDR+0x1c))
#define ATA_FIFO_FILL           (*(REG8_PTR_T)(ATA_BASE_ADDR+0x20))
    /* Actually ATA_CONTROL but conflicts arise */
#define ATA_INTF_CONTROL        (*(REG8_PTR_T)(ATA_BASE_ADDR+0x24))
#define ATA_INTERRUPT_PENDING   (*(REG8_PTR_T)(ATA_BASE_ADDR+0x28))
#define ATA_INTERRUPT_ENABLE    (*(REG8_PTR_T)(ATA_BASE_ADDR+0x2c))
#define ATA_INTERRUPT_CLEAR     (*(REG8_PTR_T)(ATA_BASE_ADDR+0x30))
#define ATA_FIFO_ALARM          (*(REG8_PTR_T)(ATA_BASE_ADDR+0x34))
#define ATA_DRIVE_DATA          (*(REG16_PTR_T)(ATA_BASE_ADDR+0xA0))
#define ATA_DRIVE_FEATURES      (*(REG8_PTR_T)(ATA_BASE_ADDR+0xA4))
#define ATA_DRIVE_SECTOR_COUNT  (*(REG8_PTR_T)(ATA_BASE_ADDR+0xA8))
#define ATA_DRIVE_SECTOR_NUM    (*(REG8_PTR_T)(ATA_BASE_ADDR+0xAC))
#define ATA_DRIVE_CYL_LOW       (*(REG8_PTR_T)(ATA_BASE_ADDR+0xB0))
#define ATA_DRIVE_CYL_HIGH      (*(REG8_PTR_T)(ATA_BASE_ADDR+0xB4))
#define ATA_DRIVE_CYL_HEAD      (*(REG8_PTR_T)(ATA_BASE_ADDR+0xB8))
#define ATA_DRIVE_STATUS        (*(REG8_PTR_T)(ATA_BASE_ADDR+0xBC)) /* rd */
#define ATA_DRIVE_COMMAND       (*(REG8_PTR_T)(ATA_BASE_ADDR+0xBC)) /* wr */
#define ATA_ALT_DRIVE_STATUS    (*(REG8_PTR_T)(ATA_BASE_ADDR+0xD8)) /* rd */
#define ATA_DRIVE_CONTROL       (*(REG8_PTR_T)(ATA_BASE_ADDR+0xD8)) /* wr */

/* ATA_INTF_CONTROL flags */
#define ATA_FIFO_RST            (1 << 7)
#define ATA_ATA_RST             (1 << 6)
#define ATA_FIFO_TX_EN          (1 << 5)
#define ATA_FIFO_RCV_EN         (1 << 4)
#define ATA_DMA_PENDING         (1 << 3)
#define ATA_DMA_ULTRA_SELECTED  (1 << 2)
#define ATA_DMA_WRITE           (1 << 1)
#define ATA_IORDY_EN            (1 << 0)

/* ATA_INTERRUPT_PENDING, ATA_INTERRUPT_ENABLE, ATA_INTERRUPT_CLEAR flags */
#define ATA_INTRQ1              (1 << 7) /* INTRQ to the DMA */
#define ATA_FIFO_UNDERFLOW      (1 << 6)
#define ATA_FIFO_OVERFLOW       (1 << 5)
#define ATA_CONTROLLER_IDLE     (1 << 4)
#define ATA_INTRQ2              (1 << 3) /* INTRQ to the MCU */

/* EPIT */
#define EPITCR1                 (*(REG32_PTR_T)(EPIT1_BASE_ADDR+0x00))
#define EPITSR1                 (*(REG32_PTR_T)(EPIT1_BASE_ADDR+0x04))
#define EPITLR1                 (*(REG32_PTR_T)(EPIT1_BASE_ADDR+0x08))
#define EPITCMPR1               (*(REG32_PTR_T)(EPIT1_BASE_ADDR+0x0C))
#define EPITCNT1                (*(REG32_PTR_T)(EPIT1_BASE_ADDR+0x10))

#define EPITCR2                 (*(REG32_PTR_T)(EPIT2_BASE_ADDR+0x00))
#define EPITSR2                 (*(REG32_PTR_T)(EPIT2_BASE_ADDR+0x04))
#define EPITLR2                 (*(REG32_PTR_T)(EPIT2_BASE_ADDR+0x08))
#define EPITCMPR2               (*(REG32_PTR_T)(EPIT2_BASE_ADDR+0x0C))
#define EPITCNT2                (*(REG32_PTR_T)(EPIT2_BASE_ADDR+0x10))

#define EPITCR_CLKSRC_OFF               (0 << 24)
#define EPITCR_CLKSRC_IPG_CLK           (1 << 24)
#define EPITCR_CLKSRC_IPG_CLK_HIGHFREQ  (2 << 24)
#define EPITCR_CLKSRC_IPG_CLK_32K       (3 << 24)
#define EPITCR_OM_DISCONNECTED          (0 << 22)
#define EPITCR_OM_TOGGLE                (1 << 22)
#define EPITCR_OM_CLEAR                 (2 << 22)
#define EPITCR_OM_SET                   (3 << 22)
#define EPITCR_STOPEN                   (1 << 21)
#define EPITCR_DOZEN                    (1 << 20)
#define EPITCR_WAITEN                   (1 << 19)
#define EPITCR_DBGEN                    (1 << 18)
#define EPITCR_IOVW                     (1 << 17)
#define EPITCR_SWR                      (1 << 16)
#define EPITCR_PRESCALER                (0xfff << 4) /* Divide by n+1 */
#define EPITCR_PRESCALER_POS            (4)
#define EPITCR_RLD                      (1 << 3)
#define EPITCR_OCIEN                    (1 << 2)
#define EPITCR_ENMOD                    (1 << 1)
#define EPITCR_EN                       (1 << 0)

#define EPITSR_OCIF                     (1 << 0)

/* GPT */
#define GPTCR                   (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x00))
#define GPTPR                   (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x04))
#define GPTSR                   (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x08))
#define GPTIR                   (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x0C))
#define GPTOCR1                 (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x10))
#define GPTOCR2                 (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x14))
#define GPTOCR3                 (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x18))
#define GPTICR1                 (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x1C))
#define GPTICR2                 (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x20))
#define GPTCNT                  (*(REG32_PTR_T)(GPT1_BASE_ADDR+0x24))

/* GPTCR */
#define GPTCR_FO3               (0x1 << 31)
#define GPTCR_FO2               (0x1 << 30)
#define GPTCR_FO1               (0x1 << 29)

#define GPTCR_OM3               (0x7 << 26)
#define GPTCR_OM3_DISCONNECTED  (0x0 << 26)
#define GPTCR_OM3_TOGGLE        (0x1 << 26)
#define GPTCR_OM3_CLEAR         (0x2 << 26)
#define GPTCR_OM3_SET           (0x3 << 26)
#define GPTCR_OM3_SINGLE_COUNT  (0x4 << 26)
    /* 0x5-0x7 same as 0x4 */

#define GPTCR_OM2               (0x7 << 23)
#define GPTCR_OM2_DISCONNECTED  (0x0 << 23)
#define GPTCR_OM2_TOGGLE        (0x1 << 23)
#define GPTCR_OM2_CLEAR         (0x2 << 23)
#define GPTCR_OM2_SET           (0x3 << 23)
#define GPTCR_OM2_SINGLE_COUNT  (0x4 << 23)

    /* 0x5-0x7 same as 0x4 */
#define GPTCR_OM1               (0x7 << 20)
#define GPTCR_OM1_DISCONNECTED  (0x0 << 20)
#define GPTCR_OM1_TOGGLE        (0x1 << 20)
#define GPTCR_OM1_CLEAR         (0x2 << 20)
#define GPTCR_OM1_SET           (0x3 << 20)
#define GPTCR_OM1_SINGLE_COUNT  (0x4 << 20)

    /* 0x5-0x7 same as 0x4 */
#define GPTCR_IM2               (0x3 << 18)
#define GPTCR_IM2_DISABLED      (0x0 << 18)
#define GPTCR_IM2_RISING        (0x1 << 18)
#define GPTCR_IM2_FALLING       (0x2 << 18)
#define GPTCR_IM2_BOTH          (0x3 << 18)

#define GPTCR_IM1               (0x3 << 16)
#define GPTCR_IM1_DISABLED      (0x0 << 16)
#define GPTCR_IM1_RISING        (0x1 << 16)
#define GPTCR_IM1_FALLING       (0x2 << 16)
#define GPTCR_IM1_BOTH          (0x3 << 16)

#define GPTCR_SWR               (0x1 << 15)
#define GPTCR_FRR               (0x1 << 9)

#define GPTCR_CLKSRC            (0x7 << 6)
#define GPTCR_CLKSRC_NONE               (0x0 << 6)
#define GPTCR_CLKSRC_IPG_CLK            (0x1 << 6)
#define GPTCR_CLKSRC_IPG_CLK_HIGHFREQ   (0x2 << 6)
#define GPTCR_CLKSRC_IPG_CLK_32K        (0x4 << 6)
/* Other values not defined */

#define GPTCR_STOPEN            (0x1 << 5)
#define GPTCR_DOZEN             (0x1 << 4)
#define GPTCR_WAITEN            (0x1 << 3)
#define GPTCR_DBGEN             (0x1 << 2)
#define GPTCR_ENMODE            (0x1 << 1)
#define GPTCR_EN                (0x1 << 0)

/* GPTSR */
#define GPTSR_ROV               (0x1 << 5)
#define GPTSR_IF2               (0x1 << 4)
#define GPTSR_IF1               (0x1 << 3)
#define GPTSR_OF3               (0x1 << 2)
#define GPTSR_OF2               (0x1 << 1)
#define GPTSR_OF1               (0x1 << 0)

/* GPTIR */
#define GPTIR_ROV               (0x1 << 5)
#define GPTIR_IF2IE             (0x1 << 4)
#define GPTIR_IF1IE             (0x1 << 3)
#define GPTIR_OF3IE             (0x1 << 2)
#define GPTIR_OF2IE             (0x1 << 1)
#define GPTIR_OF1IE             (0x1 << 0)

/* GPIO */
#define GPIO1_DR                (*(REG32_PTR_T)(GPIO1_BASE_ADDR+0x00))
#define GPIO1_GDIR              (*(REG32_PTR_T)(GPIO1_BASE_ADDR+0x04))
#define GPIO1_PSR               (*(REG32_PTR_T)(GPIO1_BASE_ADDR+0x08))
#define GPIO1_ICR1              (*(REG32_PTR_T)(GPIO1_BASE_ADDR+0x0C))
#define GPIO1_ICR2              (*(REG32_PTR_T)(GPIO1_BASE_ADDR+0x10))
#define GPIO1_IMR               (*(REG32_PTR_T)(GPIO1_BASE_ADDR+0x14))
#define GPIO1_ISR               (*(REG32_PTR_T)(GPIO1_BASE_ADDR+0x18))

#define GPIO2_DR                (*(REG32_PTR_T)(GPIO2_BASE_ADDR+0x00))
#define GPIO2_GDIR              (*(REG32_PTR_T)(GPIO2_BASE_ADDR+0x04))
#define GPIO2_PSR               (*(REG32_PTR_T)(GPIO2_BASE_ADDR+0x08))
#define GPIO2_ICR1              (*(REG32_PTR_T)(GPIO2_BASE_ADDR+0x0C))
#define GPIO2_ICR2              (*(REG32_PTR_T)(GPIO2_BASE_ADDR+0x10))
#define GPIO2_IMR               (*(REG32_PTR_T)(GPIO2_BASE_ADDR+0x14))
#define GPIO2_ISR               (*(REG32_PTR_T)(GPIO2_BASE_ADDR+0x18))

#define GPIO3_DR                (*(REG32_PTR_T)(GPIO3_BASE_ADDR+0x00))
#define GPIO3_GDIR              (*(REG32_PTR_T)(GPIO3_BASE_ADDR+0x04))
#define GPIO3_PSR               (*(REG32_PTR_T)(GPIO3_BASE_ADDR+0x08))
#define GPIO3_ICR1              (*(REG32_PTR_T)(GPIO3_BASE_ADDR+0x0C))
#define GPIO3_ICR2              (*(REG32_PTR_T)(GPIO3_BASE_ADDR+0x10))
#define GPIO3_IMR               (*(REG32_PTR_T)(GPIO3_BASE_ADDR+0x14))
#define GPIO3_ISR               (*(REG32_PTR_T)(GPIO3_BASE_ADDR+0x18))

/* CSPI */
#define CSPI_RXDATA1            (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x00))
#define CSPI_TXDATA1            (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x04))
#define CSPI_CONREG1            (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x08))
#define CSPI_INTREG1            (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x0C))
#define CSPI_DMAREG1            (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x10))
#define CSPI_STATREG1           (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x14))
#define CSPI_PERIODREG1         (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x18))
#define CSPI_TESTREG1           (*(REG32_PTR_T)(CSPI1_BASE_ADDR+0x1C0))

#define CSPI_RXDATA2            (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x00))
#define CSPI_TXDATA2            (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x04))
#define CSPI_CONREG2            (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x08))
#define CSPI_INTREG2            (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x0C))
#define CSPI_DMAREG2            (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x10))
#define CSPI_STATREG2           (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x14))
#define CSPI_PERIODREG2         (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x18))
#define CSPI_TESTREG2           (*(REG32_PTR_T)(CSPI2_BASE_ADDR+0x1C0))

#define CSPI_RXDATA3            (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x00))
#define CSPI_TXDATA3            (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x04))
#define CSPI_CONREG3            (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x08))
#define CSPI_INTREG3            (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x0C))
#define CSPI_DMAREG3            (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x10))
#define CSPI_STATREG3           (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x14))
#define CSPI_PERIODREG3         (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x18))
#define CSPI_TESTREG3           (*(REG32_PTR_T)(CSPI3_BASE_ADDR+0x1C0))

/* CSPI CONREG flags/fields */
#define CSPI_CONREG_CHIP_SELECT_SS0     (0 << 24)
#define CSPI_CONREG_CHIP_SELECT_SS1     (1 << 24)
#define CSPI_CONREG_CHIP_SELECT_SS2     (2 << 24)
#define CSPI_CONREG_CHIP_SELECT_SS3     (3 << 24)
#define CSPI_CONREG_CHIP_SELECT_MASK    (3 << 24)
#define CSPI_CONREG_DRCTL_DONT_CARE     (0 << 20)
#define CSPI_CONREG_DRCTL_TRIG_FALLING  (1 << 20)
#define CSPI_CONREG_DRCTL_TRIG_LOW      (2 << 20)
#define CSPI_CONREG_DRCTL_TRIG_RSV      (3 << 20)
#define CSPI_CONREG_DRCTL_MASK          (3 << 20)
#define CSPI_CONREG_DATA_RATE_DIV_4     (0 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_8     (1 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_16    (2 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_32    (3 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_64    (4 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_128   (5 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_256   (6 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_512   (7 << 16)
#define CSPI_CONREG_DATA_RATE_DIV_MASK  (7 << 16)
#define CSPI_BITCOUNT(n)                ((n) << 8)
#define CSPI_CONREG_SSPOL               (1 << 7)
#define CSPI_CONREG_SSCTL               (1 << 6)
#define CSPI_CONREG_PHA                 (1 << 5)
#define CSPI_CONREG_POL                 (1 << 4)
#define CSPI_CONREG_SMC                 (1 << 3)
#define CSPI_CONREG_XCH                 (1 << 2)
#define CSPI_CONREG_MODE                (1 << 1)
#define CSPI_CONREG_EN                  (1 << 0)

/* CSPI INTREG flags */
#define CSPI_INTREG_TCEN                (1 << 8)
#define CSPI_INTREG_BOEN                (1 << 7)
#define CSPI_INTREG_ROEN                (1 << 6)
#define CSPI_INTREG_RFEN                (1 << 5)
#define CSPI_INTREG_RHEN                (1 << 4)
#define CSPI_INTREG_RREN                (1 << 3)
#define CSPI_INTREG_TFEN                (1 << 2)
#define CSPI_INTREG_THEN                (1 << 1)
#define CSPI_INTREG_TEEN                (1 << 0)

/* CSPI DMAREG flags */
#define CSPI_DMAREG_RFDEN               (1 << 5)
#define CSPI_DMAREG_RHDEN               (1 << 4)
#define CSPI_DMAREG_THDEN               (1 << 1)
#define CSPI_DMAREG_TEDEN               (1 << 0)

/* CSPI STATREG flags */
#define CSPI_STATREG_TC                 (1 << 8) /* w1c */
#define CSPI_STATREG_BO                 (1 << 7) /* w1c */
#define CSPI_STATREG_RO                 (1 << 6)
#define CSPI_STATREG_RF                 (1 << 5)
#define CSPI_STATREG_RH                 (1 << 4)
#define CSPI_STATREG_RR                 (1 << 3)
#define CSPI_STATREG_TF                 (1 << 2)
#define CSPI_STATREG_TH                 (1 << 1)
#define CSPI_STATREG_TE                 (1 << 0)

/* CSPI PERIODREG flags */
#define CSPI_PERIODREG_CSRC             (1 << 15)

/* CSPI TESTREG flags */
#define CSPI_TESTREG_SWAP               (1 << 15)
#define CSPI_TESTREG_LBC                (1 << 14)

/* I2C */
#define I2C_IADR1               (*(REG16_PTR_T)(I2C1_BASE_ADDR+0x0))
#define I2C_IFDR1               (*(REG16_PTR_T)(I2C1_BASE_ADDR+0x4))
#define I2C_I2CR1               (*(REG16_PTR_T)(I2C1_BASE_ADDR+0x8))
#define I2C_I2SR1               (*(REG16_PTR_T)(I2C1_BASE_ADDR+0xC))
#define I2C_I2DR1               (*(REG16_PTR_T)(I2C1_BASE_ADDR+0x10))

#define I2C_IADR2               (*(REG16_PTR_T)(I2C2_BASE_ADDR+0x0))
#define I2C_IFDR2               (*(REG16_PTR_T)(I2C2_BASE_ADDR+0x4))
#define I2C_I2CR2               (*(REG16_PTR_T)(I2C2_BASE_ADDR+0x8))
#define I2C_I2SR2               (*(REG16_PTR_T)(I2C2_BASE_ADDR+0xC))
#define I2C_I2DR2               (*(REG16_PTR_T)(I2C2_BASE_ADDR+0x10))

#define I2C_IADR3               (*(REG16_PTR_T)(I2C3_BASE_ADDR+0x0))
#define I2C_IFDR3               (*(REG16_PTR_T)(I2C3_BASE_ADDR+0x4))
#define I2C_I2CR3               (*(REG16_PTR_T)(I2C3_BASE_ADDR+0x8))
#define I2C_I2SR3               (*(REG16_PTR_T)(I2C3_BASE_ADDR+0xC))
#define I2C_I2DR3               (*(REG16_PTR_T)(I2C3_BASE_ADDR+0x10))

    /* IADR - [7:1] Address */

    /* IFDR */
#define I2C_IFDR_DIV30           0x00
#define I2C_IFDR_DIV32           0x01
#define I2C_IFDR_DIV36           0x02
#define I2C_IFDR_DIV42           0x03
#define I2C_IFDR_DIV48           0x04
#define I2C_IFDR_DIV52           0x05
#define I2C_IFDR_DIV60           0x06
#define I2C_IFDR_DIV72           0x07
#define I2C_IFDR_DIV80           0x08
#define I2C_IFDR_DIV88           0x09
#define I2C_IFDR_DIV104          0x0a
#define I2C_IFDR_DIV128          0x0b
#define I2C_IFDR_DIV144          0x0c
#define I2C_IFDR_DIV160          0x0d
#define I2C_IFDR_DIV192          0x0e
#define I2C_IFDR_DIV240          0x0f
#define I2C_IFDR_DIV288          0x10
#define I2C_IFDR_DIV320          0x11
#define I2C_IFDR_DIV384          0x12
#define I2C_IFDR_DIV480          0x13
#define I2C_IFDR_DIV576          0x14
#define I2C_IFDR_DIV640          0x15
#define I2C_IFDR_DIV768          0x16
#define I2C_IFDR_DIV960          0x17
#define I2C_IFDR_DIV1152         0x18
#define I2C_IFDR_DIV1280         0x19
#define I2C_IFDR_DIV1536         0x1a
#define I2C_IFDR_DIV1920         0x1b
#define I2C_IFDR_DIV2304         0x1c
#define I2C_IFDR_DIV2560         0x1d
#define I2C_IFDR_DIV3072         0x1e
#define I2C_IFDR_DIV3840         0x1f
#define I2C_IFDR_DIV22           0x20
#define I2C_IFDR_DIV24           0x21
#define I2C_IFDR_DIV26           0x22
#define I2C_IFDR_DIV28           0x23
#define I2C_IFDR_DIV32_2         0x24
#define I2C_IFDR_DIV36_2         0x25
#define I2C_IFDR_DIV40           0x26
#define I2C_IFDR_DIV44           0x27
#define I2C_IFDR_DIV48_2         0x28
#define I2C_IFDR_DIV56           0x29
#define I2C_IFDR_DIV64           0x2a
#define I2C_IFDR_DIV72_2         0x2b
#define I2C_IFDR_DIV80_2         0x2c
#define I2C_IFDR_DIV96           0x2d
#define I2C_IFDR_DIV112          0x2e
#define I2C_IFDR_DIV128_2        0x2f
#define I2C_IFDR_DIV160_2        0x30
#define I2C_IFDR_DIV192_2        0x31
#define I2C_IFDR_DIV224          0x32
#define I2C_IFDR_DIV256          0x33
#define I2C_IFDR_DIV320_2        0x34
#define I2C_IFDR_DIV384_2        0x35
#define I2C_IFDR_DIV448          0x36
#define I2C_IFDR_DIV512          0x37
#define I2C_IFDR_DIV640_2        0x38
#define I2C_IFDR_DIV768_2        0x39
#define I2C_IFDR_DIV896          0x3a
#define I2C_IFDR_DIV1024         0x3b
#define I2C_IFDR_DIV1280_2       0x3c
#define I2C_IFDR_DIV1536_2       0x3d
#define I2C_IFDR_DIV1792         0x3e
#define I2C_IFDR_DIV2048         0x3f

    /* I2CR */
#define I2C_I2CR_IEN            (1 << 7)
#define I2C_I2CR_IIEN           (1 << 6)
#define I2C_I2CR_MSTA           (1 << 5)
#define I2C_I2CR_MTX            (1 << 4)
#define I2C_I2CR_TXAK           (1 << 3)
#define I2C_I2CR_RSATA          (1 << 2)

    /* I2SR */
#define I2C_I2SR_ICF            (1 << 7)
#define I2C_I2SR_IAAS           (1 << 6)
#define I2C_I2SR_IBB            (1 << 5)
#define I2C_I2SR_IAL            (1 << 4)
#define I2C_I2SR_SRW            (1 << 2)
#define I2C_I2SR_IIF            (1 << 1)
#define I2C_I2SR_RXAK           (1 << 0)

    /* I2DR - [7:0] Data */

/* AUDMUX */
#define AUDMUX_PTCR1            (*(REG32_PTR_T)(AUDMUX_BASE+0x00))
#define AUDMUX_PDCR1            (*(REG32_PTR_T)(AUDMUX_BASE+0x04))
#define AUDMUX_PTCR2            (*(REG32_PTR_T)(AUDMUX_BASE+0x08))
#define AUDMUX_PDCR2            (*(REG32_PTR_T)(AUDMUX_BASE+0x0C))
#define AUDMUX_PTCR3            (*(REG32_PTR_T)(AUDMUX_BASE+0x10))
#define AUDMUX_PDCR3            (*(REG32_PTR_T)(AUDMUX_BASE+0x14))
#define AUDMUX_PTCR4            (*(REG32_PTR_T)(AUDMUX_BASE+0x18))
#define AUDMUX_PDCR4            (*(REG32_PTR_T)(AUDMUX_BASE+0x1C))
#define AUDMUX_PTCR5            (*(REG32_PTR_T)(AUDMUX_BASE+0x20))
#define AUDMUX_PDCR5            (*(REG32_PTR_T)(AUDMUX_BASE+0x24))
#define AUDMUX_PTCR6            (*(REG32_PTR_T)(AUDMUX_BASE+0x28))
#define AUDMUX_PDCR6            (*(REG32_PTR_T)(AUDMUX_BASE+0x2C))
#define AUDMUX_PTCR7            (*(REG32_PTR_T)(AUDMUX_BASE+0x30))
#define AUDMUX_PDCR7            (*(REG32_PTR_T)(AUDMUX_BASE+0x34))
#define AUDMUX_CNMCR            (*(REG32_PTR_T)(AUDMUX_BASE+0x38))

#define AUDMUX_PTCR_TFS_DIR         (1 << 31)

#define AUDMUX_PTCR_TFSEL           (0xf << 27)
#define AUDMUX_PTCR_TFSEL_TXFS      (0x0 << 27)
#define AUDMUX_PTCR_TFSEL_RXFS      (0x8 << 27)
#define AUDMUX_PTCR_TFSEL_PORT1     (0x0 << 27)
#define AUDMUX_PTCR_TFSEL_PORT2     (0x1 << 27)
#define AUDMUX_PTCR_TFSEL_PORT3     (0x2 << 27)
#define AUDMUX_PTCR_TFSEL_PORT4     (0x3 << 27)
#define AUDMUX_PTCR_TFSEL_PORT5     (0x4 << 27)
#define AUDMUX_PTCR_TFSEL_PORT6     (0x5 << 27)
#define AUDMUX_PTCR_TFSEL_PORT7     (0x6 << 27)

#define AUDMUX_PTCR_TCLKDIR         (1 << 26)

#define AUDMUX_PTCR_TCSEL           (0xf << 22)
#define AUDMUX_PTCR_TCSEL_TXFS      (0x0 << 22)
#define AUDMUX_PTCR_TCSEL_RXFS      (0x8 << 22)
#define AUDMUX_PTCR_TCSEL_PORT1     (0x0 << 22)
#define AUDMUX_PTCR_TCSEL_PORT2     (0x1 << 22)
#define AUDMUX_PTCR_TCSEL_PORT3     (0x2 << 22)
#define AUDMUX_PTCR_TCSEL_PORT4     (0x3 << 22)
#define AUDMUX_PTCR_TCSEL_PORT5     (0x4 << 22)
#define AUDMUX_PTCR_TCSEL_PORT6     (0x5 << 22)
#define AUDMUX_PTCR_TCSEL_PORT7     (0x6 << 22)

#define AUDMUX_PTCR_RFS_DIR         (1 << 21)

#define AUDMUX_PTCR_RFSSEL          (0xf << 17)
#define AUDMUX_PTCR_RFSSEL_TXFS     (0x0 << 17)
#define AUDMUX_PTCR_RFSSEL_RXFS     (0x8 << 17)
#define AUDMUX_PTCR_RFSSEL_PORT1    (0x0 << 17)
#define AUDMUX_PTCR_RFSSEL_PORT2    (0x1 << 17)
#define AUDMUX_PTCR_RFSSEL_PORT3    (0x2 << 17)
#define AUDMUX_PTCR_RFSSEL_PORT4    (0x3 << 17)
#define AUDMUX_PTCR_RFSSEL_PORT5    (0x4 << 17)
#define AUDMUX_PTCR_RFSSEL_PORT6    (0x5 << 17)
#define AUDMUX_PTCR_RFSSEL_PORT7    (0x6 << 17)

#define AUDMUX_PTCR_RCLKDIR         (1 << 16)

#define AUDMUX_PTCR_RCSEL           (0xf << 12)
#define AUDMUX_PTCR_RCSEL_TXFS      (0x0 << 12)
#define AUDMUX_PTCR_RCSEL_RXFS      (0x8 << 12)
#define AUDMUX_PTCR_RCSEL_PORT1     (0x0 << 12)
#define AUDMUX_PTCR_RCSEL_PORT2     (0x1 << 12)
#define AUDMUX_PTCR_RCSEL_PORT3     (0x2 << 12)
#define AUDMUX_PTCR_RCSEL_PORT4     (0x3 << 12)
#define AUDMUX_PTCR_RCSEL_PORT5     (0x4 << 12)
#define AUDMUX_PTCR_RCSEL_PORT6     (0x5 << 12)
#define AUDMUX_PTCR_RCSEL_PORT7     (0x6 << 12)
#define AUDMUX_PTCR_SYN             (1 << 11)

#define AUDMUX_PDCR_RXDSEL          (0x7 << 13)
#define AUDMUX_PDCR_RXDSEL_PORT1    (0 << 13)
#define AUDMUX_PDCR_RXDSEL_PORT2    (1 << 13)
#define AUDMUX_PDCR_RXDSEL_PORT3    (2 << 13)
#define AUDMUX_PDCR_RXDSEL_PORT4    (3 << 13)
#define AUDMUX_PDCR_RXDSEL_PORT5    (4 << 13)
#define AUDMUX_PDCR_RXDSEL_PORT6    (5 << 13)
#define AUDMUX_PDCR_RXDSEL_PORT7    (6 << 13)
#define AUDMUX_PDCR_TXRXEN          (1 << 12)

#define AUDMUX_CNMCR_BEN            (1 << 18)
#define AUDMUX_CNMCR_FSPOL          (1 << 17)
#define AUDMUX_CNMCR_CLKPOL         (1 << 16)

#define AUDMUX_CNMCR_CNTHI          (0xff << 8)
#define AUDMUX_CNMCR_CNTHI_POS      (8)

#define AUDMUX_CNMCR_CNTLOW         (0xff << 0)
#define AUDMUX_CNMCR_CNTLOW_POS     (0)

/* SSI */
#define SSI_STX0_1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x00))
#define SSI_STX1_1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x04))
#define SSI_SRX0_1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x08))
#define SSI_SRX1_1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x0C))
#define SSI_SCR1                (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x10))
#define SSI_SISR1               (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x14))
#define SSI_SIER1               (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x18))
#define SSI_STCR1               (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x1C))
#define SSI_SRCR1               (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x20))
#define SSI_STCCR1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x24))
#define SSI_SRCCR1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x28))
#define SSI_SFCSR1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x2C))
#define SSI_SACNT1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x38))
#define SSI_SACADD1             (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x3C))
#define SSI_SACDAT1             (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x40))
#define SSI_SATAG1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x44))
#define SSI_STMSK1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x48))
#define SSI_SRMSK1              (*(REG32_PTR_T)(SSI1_BASE_ADDR+0x4C))

#define SSI_STX0_2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x00))
#define SSI_STX1_2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x04))
#define SSI_SRX0_2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x08))
#define SSI_SRX1_2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x0C))
#define SSI_SCR2                (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x10))
#define SSI_SISR2               (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x14))
#define SSI_SIER2               (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x18))
#define SSI_STCR2               (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x1C))
#define SSI_SRCR2               (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x20))
#define SSI_STCCR2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x24))
#define SSI_SRCCR2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x28))
#define SSI_SFCSR2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x2C))
#define SSI_SACNT2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x38))
#define SSI_SACADD2             (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x3C))
#define SSI_SACDAT2             (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x40))
#define SSI_SATAG2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x44))
#define SSI_STMSK2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x48))
#define SSI_SRMSK2              (*(REG32_PTR_T)(SSI2_BASE_ADDR+0x4C))

/* SSI SCR */
#define SSI_SCR_CLK_IST         (0x1 << 9)
#define SSI_SCR_TCHN_EN         (0x1 << 8)
#define SSI_SCR_SYS_CLK_EN      (0x1 << 7)

#define SSI_SCR_I2S_MODE        (0x3 << 5)
#define SSI_SCR_I2S_MODE_NORMAL (0x0 << 5)
#define SSI_SCR_I2S_MODE_MASTER (0x1 << 5)
#define SSI_SCR_I2S_MODE_SLAVE  (0x2 << 5)
#define SSI_SCR_I2S_MODE_NOR2   (0x3 << 5)

#define SSI_SCR_SYN             (0x1 << 4)
#define SSI_SCR_NET             (0x1 << 3)
#define SSI_SCR_RE              (0x1 << 2)
#define SSI_SCR_TE              (0x1 << 1)
#define SSI_SCR_SSIEN           (0x1 << 0)

/* SSI SISR */
#define SSI_SISR_CMDAU          (0x1 << 18)
#define SSI_SISR_CMDDU          (0x1 << 17)
#define SSI_SISR_RXT            (0x1 << 16)
#define SSI_SISR_RDR1           (0x1 << 15)
#define SSI_SISR_RDR0           (0x1 << 14)
#define SSI_SISR_TDE1           (0x1 << 13)
#define SSI_SISR_TDE0           (0x1 << 12)
#define SSI_SISR_ROE1           (0x1 << 11)
#define SSI_SISR_ROE0           (0x1 << 10)
#define SSI_SISR_TUE1           (0x1 << 9)
#define SSI_SISR_TUE0           (0x1 << 8)
#define SSI_SISR_TFS            (0x1 << 7)
#define SSI_SISR_RFS            (0x1 << 6)
#define SSI_SISR_TLS            (0x1 << 5)
#define SSI_SISR_RLS            (0x1 << 4)
#define SSI_SISR_RFF1           (0x1 << 3)
#define SSI_SISR_RFF2           (0x1 << 2)
#define SSI_SISR_TFE1           (0x1 << 1)
#define SSI_SISR_TFE0           (0x1 << 0)

/* SSI SIER */
#define SSI_SIER_RDMAE          (0x1 << 22)
#define SSI_SIER_RIE            (0x1 << 21)
#define SSI_SIER_TDMAE          (0x1 << 20)
#define SSI_SIER_TIE            (0x1 << 19)
#define SSI_SIER_CMDAU          (0x1 << 18)
#define SSI_SIER_CMDDU          (0x1 << 17)
#define SSI_SIER_RXT            (0x1 << 16)
#define SSI_SIER_RDR1           (0x1 << 15)
#define SSI_SIER_RDR0           (0x1 << 14)
#define SSI_SIER_TDE1           (0x1 << 13)
#define SSI_SIER_TDE0           (0x1 << 12)
#define SSI_SIER_ROE1           (0x1 << 11)
#define SSI_SIER_ROE0           (0x1 << 10)
#define SSI_SIER_TUE1           (0x1 << 9)
#define SSI_SIER_TUE0           (0x1 << 8)
#define SSI_SIER_TFS            (0x1 << 7)
#define SSI_SIER_RFS            (0x1 << 6)
#define SSI_SIER_TLS            (0x1 << 5)
#define SSI_SIER_RLS            (0x1 << 4)
#define SSI_SIER_RFF1           (0x1 << 3)
#define SSI_SIER_RFF0           (0x1 << 2)
#define SSI_SIER_TFE1           (0x1 << 1)
#define SSI_SIER_TFE0           (0x1 << 0)

/* SSI STCR */
#define SSI_STCR_TXBIT0         (0x1 << 9)
#define SSI_STCR_TFEN1          (0x1 << 8)
#define SSI_STCR_TFEN0          (0x1 << 7)
#define SSI_STCR_TFDIR          (0x1 << 6)
#define SSI_STCR_TXDIR          (0x1 << 5)
#define SSI_STCR_TSHFD          (0x1 << 4)
#define SSI_STCR_TSCKP          (0x1 << 3)
#define SSI_STCR_TFSI           (0x1 << 2)
#define SSI_STCR_TFSL           (0x1 << 1)
#define SSI_STCR_TEFS           (0x1 << 0)

/* SSI SRCR */
#define SSI_SRCR_RXEXT          (0x1 << 10)
#define SSI_SRCR_RXBIT0         (0x1 << 9)
#define SSI_SRCR_RFEN1          (0x1 << 8)
#define SSI_SRCR_RFEN0          (0x1 << 7)
#define SSI_SRCR_RFDIR          (0x1 << 6)
#define SSI_SRCR_RXDIR          (0x1 << 5)
#define SSI_SRCR_RSHFD          (0x1 << 4)
#define SSI_SRCR_RSCKP          (0x1 << 3)
#define SSI_SRCR_RFSI           (0x1 << 2)
#define SSI_SRCR_RFSL           (0x1 << 1)
#define SSI_SRCR_REFS           (0x1 << 0)

/* SSI STCCR/SRCCR */
#define SSI_STRCCR_DIV2         (0x1 << 18)
#define SSI_STRCCR_PSR          (0x1 << 17)

#define SSI_STRCCR_WL           (0xf << 13)
#define SSI_STRCCR_WL8          (0x3 << 13)
#define SSI_STRCCR_WL10         (0x4 << 13)
#define SSI_STRCCR_WL12         (0x5 << 13)
#define SSI_STRCCR_WL16         (0x7 << 13)
#define SSI_STRCCR_WL18         (0x8 << 13)
#define SSI_STRCCR_WL20         (0x9 << 13)
#define SSI_STRCCR_WL22         (0xa << 13)
#define SSI_STRCCR_WL24         (0xb << 13)

#define SSI_STRCCR_DC           (0x1f << 8)
#define SSI_STRCCR_DC_POS       (8)

#define SSI_STRCCR_PM           (0xf << 0)
#define SSI_STRCCR_PM_POS       (0)

/* SSI SFCSR */
#define SSI_SFCSR_RFCNT1        (0xf << 28)
#define SSI_SFCSR_RFCNT1_POS    (28)

#define SSI_SFCSR_TFCNT1        (0xf << 24)
#define SSI_SFCSR_TFCNN1_POS    (24)

#define SSI_SFCSR_RFWM1         (0xf << 20)
#define SSI_SFCSR_RFWM1_POS     (20)

#define SSI_SFCSR_TFWM1         (0xf << 16)
#define SSI_SFCSR_TFWM1_POS     (16)

#define SSI_SFCSR_RFCNT0        (0xf << 12)
#define SSI_SFCSR_RFCNT0_POS    (12)

#define SSI_SFCSR_TFCNT0        (0xf << 8)
#define SSI_SFCSR_TFCNT0_POS    (8)

#define SSI_SFCSR_RFWM0         (0xf << 4)
#define SSI_SFCSR_RFWM0_POS     (4)

#define SSI_SFCSR_TFWM0         (0xf << 0)
#define SSI_SFCSR_TFWM0_POS     (0)

/* SACNT */
#define SSI_SACNT_FRDIV         (0x3f << 5)
#define SSI_SACNT_FRDIV_POS     (5)

#define SSI_SACNT_WR            (0x1 << 4)
#define SSI_SACNT_RD            (0x1 << 3)
#define SSI_SACNT_TIF           (0x1 << 2)
#define SSI_SACNT_FV            (0x1 << 1)
#define SSI_SACNT_AC97EN        (0x1 << 0)

/* RTC */
#define RTC_HOURMIN             (*(REG32_PTR_T)(RTC_BASE_ADDR+0x00))
#define RTC_SECONDS             (*(REG32_PTR_T)(RTC_BASE_ADDR+0x04))
#define RTC_ALRM_HM             (*(REG32_PTR_T)(RTC_BASE_ADDR+0x08))
#define RTC_ALRM_SEC            (*(REG32_PTR_T)(RTC_BASE_ADDR+0x0C))
#define RTC_CTL                 (*(REG32_PTR_T)(RTC_BASE_ADDR+0x10))
#define RTC_ISR                 (*(REG32_PTR_T)(RTC_BASE_ADDR+0x14))
#define RTC_IENR                (*(REG32_PTR_T)(RTC_BASE_ADDR+0x18))
#define RTC_STPWCH              (*(REG32_PTR_T)(RTC_BASE_ADDR+0x1C))
#define RTC_DAYR                (*(REG32_PTR_T)(RTC_BASE_ADDR+0x20))
#define RTC_DAYALARM            (*(REG32_PTR_T)(RTC_BASE_ADDR+0x24))

/* Watchdog */
#define WDOG_WCR                (*(REG16_PTR_T)(WDOG_BASE_ADDR+0x00))
#define WDOG_WSR                (*(REG16_PTR_T)(WDOG_BASE_ADDR+0x02))
#define WDOG_WRSR               (*(REG16_PTR_T)(WDOG_BASE_ADDR+0x04))

#define WDOG_WCR_WT             (0xff << 8)
#define WDOG_WCR_WT_POS         (8)

#define WDOG_WCR_WOE            (0x1 << 6)
#define WDOG_WCR_WDA            (0x1 << 5)
#define WDOG_WCR_SRS            (0x1 << 4)
#define WDOG_WCR_WRE            (0x1 << 3)
#define WDOG_WCR_WDE            (0x1 << 2)
#define WDOG_WCR_WDBG           (0x1 << 1)
#define WDOG_WCR_WDZST          (0x1 << 0)

#define WDOG_WRSR_JRST          (0x1 << 5)
#define WDOG_WRSR_PWR           (0x1 << 4)
#define WDOG_WRSR_EXT           (0x1 << 3)
#define WDOG_WRSR_CMON          (0x1 << 2)
#define WDOG_WRSR_TOUT          (0x1 << 1)
#define WDOG_WRSR_SFTW          (0x1 << 0)

/* Keypad */
#define KPP_KPCR                (*(REG16_PTR_T)(KPP_BASE_ADDR+0x0))
#define KPP_KPSR                (*(REG16_PTR_T)(KPP_BASE_ADDR+0x2))
#define KPP_KDDR                (*(REG16_PTR_T)(KPP_BASE_ADDR+0x4))
#define KPP_KPDR                (*(REG16_PTR_T)(KPP_BASE_ADDR+0x6))

/* KPP_KPSR bits */
#define KPP_KPSR_KRIE           (1 << 9)
#define KPP_KPSR_KDIE           (1 << 8)
#define KPP_KPSR_KRSS           (1 << 3)
#define KPP_KPSR_KDSC           (1 << 2)
#define KPP_KPSR_KPKR           (1 << 1)
#define KPP_KPSR_KPKD           (1 << 0)

/* SDHC */
#define SDHC1_CLOCK_CONTROL     (*(REG32_PTR_T)(MMC_SDHC1_BASE_ADDR+0x00))
#define SDHC2_CLOCK_CONTROL     (*(REG32_PTR_T)(MMC_SDHC2_BASE_ADDR+0x00))

/* SDHC bits */
#define STOP_CLK                (1 << 0)

/* ROMPATCH and AVIC */
#define ROMPATCH_BASE_ADDR      0x60000000

/* Since AVIC vector registers are NOT used, we reserve some for various
 * purposes. Copied from Linux source code. */
#define CHIP_REV_1_0            0x10
#define CHIP_REV_2_0            0x20
#define SYSTEM_REV_ID_REG       (AVIC_BASE_ADDR + AVIC_VEC_1)
#define SYSTEM_REV_ID_MAG       0xF00C

/*
 * NAND, SDRAM, WEIM, M3IF, EMI controllers
 */
#define EXT_MEM_CTRL_BASE       0xB8000000
#define NFC_BASE                EXT_MEM_CTRL_BASE
#define ESDCTL_BASE             0xB8001000
#define WEIM_BASE_ADDR          0xB8002000
#define WEIM_CTRL_CS0           (WEIM_BASE_ADDR+0x00)
#define WEIM_CTRL_CS1           (WEIM_BASE_ADDR+0x10)
#define WEIM_CTRL_CS2           (WEIM_BASE_ADDR+0x20)
#define WEIM_CTRL_CS3           (WEIM_BASE_ADDR+0x30)
#define WEIM_CTRL_CS4           (WEIM_BASE_ADDR+0x40)
#define M3IF_BASE               0xB8003000
#define PCMCIA_CTL_BASE         0xB8004000

/*
 * Memory regions and CS
 */
#define IPU_MEM_BASE_ADDR       0x70000000
#define CSD0_BASE_ADDR          0x80000000
#define CSD1_BASE_ADDR          0x90000000
#define CS0_BASE_ADDR           0xA0000000
#define CS1_BASE_ADDR           0xA8000000
#define CS2_BASE_ADDR           0xB0000000
#define CS3_BASE_ADDR           0xB2000000
#define CS4_BASE_ADDR           0xB4000000
#define CS4_BASE_PSRAM          0xB5000000
#define CS5_BASE_ADDR           0xB6000000
#define PCMCIA_MEM_BASE_ADDR    0xC0000000

#define INTERNAL_ROM_VA         0xF0000000

/*
 * SDRAM
 */
#define RAM_BANK0_BASE          SDRAM_BASE_ADDR

/*
 * IRQ Controller Register Definitions.
 */
#define AVIC_BASE_ADDR          0x68000000
#define AVIC_INTCNTL            (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x00))
#define AVIC_NIMASK             (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x04))
#define AVIC_INTENNUM           (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x08))
#define AVIC_INTDISNUM          (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x0C))
#define AVIC_INTENABLEH         (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x10))
#define AVIC_INTENABLEL         (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x14))
#define AVIC_INTTYPEH           (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x18))
#define AVIC_INTTYPEL           (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x1C))
#define AVIC_NIPRIORITY(n)      (((REG32_PTR_T)(AVIC_BASE_ADDR+0x20))[n])
#define AVIC_NIPRIORITY7        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x20))
#define AVIC_NIPRIORITY6        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x24))
#define AVIC_NIPRIORITY5        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x28))
#define AVIC_NIPRIORITY4        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x2C))
#define AVIC_NIPRIORITY3        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x30))
#define AVIC_NIPRIORITY2        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x34))
#define AVIC_NIPRIORITY1        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x38))
#define AVIC_NIPRIORITY0        (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x3C))
#define AVIC_NIVECSR            (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x40))
#define AVIC_FIVECSR            (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x44))
#define AVIC_INTSRCH            (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x48))
#define AVIC_INTSRCL            (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x4C))
#define AVIC_INTFRCH            (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x50))
#define AVIC_INTFRCL            (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x54))
#define AVIC_NIPNDH             (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x58))
#define AVIC_NIPNDL             (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x5C))
#define AVIC_FIPNDH             (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x60))
#define AVIC_FIPNDL             (*(REG32_PTR_T)(AVIC_BASE_ADDR+0x64))
#define AVIC_VECTOR_BASE_ADDR   (AVIC_BASE_ADDR+0x100)
#define AVIC_VECTOR(n)          (((REG32_PTR_T)VECTOR_BASE_ADDR)[n])

/* The vectors go all the way up to 63. 4 bytes for each */
#define AVIC_INTCNTL_ABFLAG     (1 << 25)
#define AVIC_INTCNTL_ABFEN      (1 << 24)
#define AVIC_INTCNTL_NIDIS      (1 << 22)
#define AVIC_INTCNTL_FIDIS      (1 << 21)
#define AVIC_INTCNTL_NIAD       (1 << 20)
#define AVIC_INTCNTL_FIAD       (1 << 19)
#define AVIC_INTCNTL_NM         (1 << 18)

/* L210 */
#define L2CC_BASE_ADDR                  0x30000000
#define L2_CACHE_LINE_SIZE              32
#define L2_CACHE_CTL_REG                0x100
#define L2_CACHE_AUX_CTL_REG            0x104
#define L2_CACHE_SYNC_REG               0x730
#define L2_CACHE_INV_LINE_REG           0x770
#define L2_CACHE_INV_WAY_REG            0x77C
#define L2_CACHE_CLEAN_LINE_REG         0x7B0
#define L2_CACHE_CLEAN_INV_LINE_REG     0x7F0

#define L2CC_CACHE_SYNC                 (*(REG32_PTR_T)(L2CC_BASE_ADDR+L2_CACHE_SYNC_REG))

/* CCM */
#define CCM_CCMR                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x00))
#define CCM_PDR0                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x04))
#define CCM_PDR1                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x08))
#define CCM_RCSR                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x0C))
#define CCM_MPCTL                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x10))
#define CCM_UPCTL                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x14))
#define CCM_SPCTL                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x18))
#define CCM_COSR                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x1C))
#define CCM_CGR0                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x20))
#define CCM_CGR1                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x24))
#define CCM_CGR2                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x28))
#define CCM_WIMR0                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x2C))
#define CCM_LDC                         (*(REG32_PTR_T)(CCM_BASE_ADDR+0x30))
#define CCM_DCVR0                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x34))
#define CCM_DCVR1                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x38))
#define CCM_DCVR2                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x3C))
#define CCM_DCVR3                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x40))
#define CCM_LTR0                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x44))
#define CCM_LTR1                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x48))
#define CCM_LTR2                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x4C))
#define CCM_LTR3                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x50))
#define CCM_LTBR0                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x54))
#define CCM_LTBR1                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x58))
#define CCM_PMCR0                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x5C))
#define CCM_PMCR1                       (*(REG32_PTR_T)(CCM_BASE_ADDR+0x60))
#define CCM_PDR2                        (*(REG32_PTR_T)(CCM_BASE_ADDR+0x64))

/* CCMR */
#define CCM_CCMR_L2PG                   (0x1 << 29)
#define CCM_CCMR_VSTBY                  (0x1 << 28)
#define CCM_CCMR_WBEN                   (0x1 << 27)
#define CCM_CCMR_FPMF                   (0x1 << 26)
#define CCM_CCMR_CSCS                   (0x1 << 25)
#define CCM_CCMR_PERCS                  (0x1 << 24)

#define CCM_CCMR_SSI2S                  (0x3 << 21)
#define CCM_CCMR_SSI2S_MCU_CLK          (0x0 << 21)
#define CCM_CCMR_SSI2S_USB_CLK          (0x1 << 21)
#define CCM_CCMR_SSI2S_SERIAL_CLK       (0x2 << 21) /* default */

#define CCM_CCMR_SSI1S                  (0x3 << 18)
#define CCM_CCMR_SSI1S_MCU_CLK          (0x0 << 18)
#define CCM_CCMR_SSI1S_USB_CLK          (0x1 << 18)
#define CCM_CCMR_SSI1S_SERIAL_CLK       (0x2 << 18) /* default */

#define CCM_CCMR_RAMW                   (0x3 << 16)
#define CCM_CCMR_RAMW_0ARM_0ALTMS       (0x0 << 16)
#define CCM_CCMR_RAMW_0ARM_1ALTMS       (0x1 << 16) /* Not recommended */
#define CCM_CCMR_RAMW_1ARM_0ALTMS       (0x2 << 16) /* Not recommended */
#define CCM_CCMR_RAMW_1ARM_1ALTMS       (0x3 << 16)

#define CCM_CCMR_LPM                    (0x3 << 14)
#define CCM_CCMR_LPM_WAIT_MODE          (0x0 << 14)
#define CCM_CCMR_LPM_DOZE_MODE          (0x1 << 14)
#define CCM_CCMR_LPM_SRM                (0x2 << 14) /* State retention mode */
#define CCM_CCMR_LPM_DSM                (0x3 << 14) /* Deep sleep mode */

#define CCM_CCMR_FIRS                   (0x3 << 11)
#define CCM_CCMR_FIRS_MCU_CLK           (0x0 << 11)
#define CCM_CCMR_FIRS_USB_CLK           (0x1 << 11)
#define CCM_CCMR_FIRS_SERIAL_CLK        (0x2 << 11)

#define CCM_CCMR_WAMO                   (0x1 << 10)
#define CCM_CCMR_UPE                    (0x1 << 9)
#define CCM_CCMR_SPE                    (0x1 << 8)
#define CCM_CCMR_MDS                    (0x1 << 7)

#define CCM_CCMR_ROMW                   (0x3 << 5)
#define CCM_CCMR_ROMW_0ARM_0ALTMS       (0x0 << 5)
#define CCM_CCMR_ROMW_0ARM_1ALTMS       (0x1 << 5) /* Not recommended */
#define CCM_CCMR_ROMW_1ARM_0ALTMS       (0x2 << 5) /* Not recommended */
#define CCM_CCMR_ROMW_1ARM_1ALTMS       (0x3 << 5)

#define CCM_CCMR_SBYCS                  (0x1 << 4)
#define CCM_CCMR_MPE                    (0x1 << 3)

#define CCM_CCMR_PRCS                   (0x3 << 1)
#define CCM_CCMR_PRCS_FPM               (0x1 << 1)
#define CCM_CCMR_PRCS_CKIH              (0x2 << 1)

#define CCM_CCMR_FPME                   (0x1 << 0)

/* PDR0 */
#define CCM_PDR0_CSI_PODF               (0x1ff << 23)
#define CCM_PDR0_CSI_PODF_POS           (23)

#define CCM_PDR0_PER_PODF               (0x1f << 16)
#define CCM_PDR0_PER_PODF_POS           (16)

#define CCM_PDR0_HSP_PODF               (0x7 << 11)
#define CCM_PDR0_HSP_PODF_POS           (11)

#define CCM_PDR0_NFC_PODF               (0x7 << 8)
#define CCM_PDR0_NFC_PODF_POS           (8)

#define CCM_PDR0_IPG_PODF               (0x3 << 6)
#define CCM_PDR0_IPG_PODF_POS           (6)

#define CCM_PDR0_MAX_PODF               (0x7 << 3)
#define CCM_PDR0_MAX_PODF_POS           (3)

#define CCM_PDR0_MCU_PODF               (0x7 << 0)
#define CCM_PDR0_MCU_PODF_POS           (0)

/* PDR1 */
#define CCM_PDR1_USB_PRDF               (0x3 << 30)
#define CCM_PDR1_USB_PRDF_POS           (30)

#define CCM_PDR1_USB_PODF               (0x7 << 27)
#define CCM_PDR1_USB_PODF_POS           (27)

#define CCM_PDR1_FIRI_PRE_PODF          (0x7 << 24)
#define CCM_PDR1_FIRI_PRE_PODF_POS      (24)

#define CCM_PDR1_FIRI_PODF              (0x3f << 18)
#define CCM_PDR1_FIRI_PODF_POS          (18)

#define CCM_PDR1_SSI2_PRE_PODF          (0x7 << 15)
#define CCM_PDR1_SSI2_PRE_PODF_POS      (15)

#define CCM_PDR1_SSI2_PODF              (0x3f << 9)
#define CCM_PDR1_SSI2_PODF_POS          (9)

#define CCM_PDR1_SSI1_PRE_PODF          (0x7 << 6)
#define CCM_PDR1_SSI1_PRE_PODF_POS      (6)

#define CCM_PDR1_SSI1_PODF              (0x3f << 0)
#define CCM_PDR1_SSI1_PODF_POS          (0)

/* RCSR */
#define CCM_RCSR_NF16B                  (1 << 31)

#define CCM_RCSR_NFMS                   (1 << 30)

#define CCM_RCSR_BTP4                   (1 << 27)
#define CCM_RCSR_BTP3                   (1 << 26)
#define CCM_RCSR_BTP2                   (1 << 25)
#define CCM_RCSR_BTP1                   (1 << 24)
#define CCM_RCSR_BTP0                   (1 << 23)

#define CCM_RCSR_OSCNT                  (0x7f << 16)
#define CCM_RCSR_OSCNT_POS              (16)

#define CCM_RCSR_PERES                  (1 << 15)

#define CCM_RCSR_SDM                    (0x3 << 12)
#define CCM_RCSR_SDM_POS                (12)

#define CCM_RCSR_GPF                    (0x7 << 5)
#define CCM_RCSR_GPF_POS                (5)

#define CCM_RCSR_WFIS                   (1 << 4)

#define CCM_RCSR_REST                   (0x7 << 0)
#define CCM_RCSR_REST_POS               (0)
#define CCM_RCSR_REST_POR_EXT           (0x0)
#define CCM_RCSR_REST_QUALIFIED_EXT     (0x1)
#define CCM_RCSR_REST_WATCHDOG_TMO      (0x2)
/* 0x3 - 0x5: reserved */
#define CCM_RCSR_REST_JTAG              (0x6)
#define CCM_RCSR_REST_ARM11P_GATING     (0x7)

/* MPCTL */
#define CCM_MPCTL_BRM                   (1 << 31)
#define CCM_MPCTL_PD                    (0xf << 26)
#define CCM_MPCTL_PD_POS                (26)
#define CCM_MPCTL_MFD                   (0x3ff << 16)
#define CCM_MPCTL_MFD_POS               (16)
#define CCM_MPCTL_MFI                   (0xf << 10)
#define CCM_MPCTL_MFI_POS               (10)
#define CCM_MPCTL_MFN                   (0x3ff << 0)
#define CCM_MPCTL_MFN_POS               (0)

/* UPCTL */
#define CCM_UPCTL_BRM                   (1 << 31)
#define CCM_UPCTL_PD                    (0xf << 26)
#define CCM_UPCTL_PD_POS                (26)
#define CCM_UPCTL_MFD                   (0x3ff << 16)
#define CCM_UPCTL_MFD_POS               (16)
#define CCM_UPCTL_MFI                   (0xf << 10)
#define CCM_UPCTL_MFI_POS               (10)
#define CCM_UPCTL_MFN                   (0x3ff << 0)
#define CCM_UPCTL_MFN_POS               (0)

/* SPCTL */
#define CCM_SPCTL_BRM                   (1 << 31)
#define CCM_SPCTL_PD                    (0xf << 26)
#define CCM_SPCTL_PD_POS                (26)
#define CCM_SPCTL_MFD                   (0x3ff << 16)
#define CCM_SPCTL_MFD_POS               (16)
#define CCM_SPCTL_MFI                   (0xf << 10)
#define CCM_SPCTL_MFI_POS               (10)
#define CCM_SPCTL_MFN                   (0x3ff << 0)
#define CCM_SPCTL_MFN_POS               (0)

/* COSR */
#define CCM_COSR_CLKOEN                 (1 << 9)
#define CCM_COSR_CLKOUTDIV              (0x7 << 6)
#define CCM_COSR_CLKOUTDIV_POS          (6)
#define CCM_COSR_CLKOSEL                (0xf << 0)
#define CCM_COSR_CLKOSEL_POS            (0)
#define CCM_COSR_CLKOSEL_MPL_DPDGCK_CLK         (0x0)
#define CCM_COSR_CLKOSEL_IPG_CLK_CCM            (0x1)
#define CCM_COSR_CLKOSEL_UPL_DPDGCK_CLK         (0x2)
#define CCM_COSR_CLKOSEL_PLL_REF_CLK            (0x3)
#define CCM_COSR_CLKOSEL_FPM_CKIL512_CLK        (0x4)
#define CCM_COSR_CLKOSEL_IPG_CLK_AHB_ARM        (0x5)
#define CCM_COSR_CLKOSEL_IPG_CLK_ARM            (0x6)
#define CCM_COSR_CLKOSEL_SPL_DPDGCK_CLK         (0x7)
#define CCM_COSR_CLKOSEL_CKIH                   (0x8)
#define CCM_COSR_CLKOSEL_IPG_CLK_AHB_EMI_CLK    (0x9)
#define CCM_COSR_CLKOSEL_IPG_CLK_IPU_HSP        (0x9)
#define CCM_COSR_CLKOSEL_IPG_CLK_NFC_20M        (0xa)
#define CCM_COSR_CLKOSEL_IPG_CLK_PERCLK_UART1   (0xb)
#define CCM_COSR_CLKOSEL_IPG_REF_CIR1           (0xc) /* ref_cir_gateload */
#define CCM_COSR_CLKOSEL_IPG_REF_CIR2           (0xc) /* ref_cir_intrcload */
#define CCM_COSR_CLKOSEL_IPG_REF_CIR3           (0xc) /* ref_cir_path */

/* CGR0 */
/* CGR1 */
/* CGR2 */
/* Handled in ccm-imx31.h and ccm-imx31.c */


#define CCM_WIMR0_GPIO3                 (1 << 0)
#define CCM_WIMR0_GPIO2                 (1 << 1)
#define CCM_WIMR0_GPIO1                 (1 << 2)
#define CCM_WIMR0_PCMCIA                (1 << 3)
#define CCM_WIMR0_WDT                   (1 << 4)
#define CCM_WIMR0_USB_OTG               (1 << 5)
#define CCM_WIMR0_IPI_INT_UH2           (1 << 6)
#define CCM_WIMR0_IPI_INT_UH1           (1 << 7)
#define CCM_WIMR0_IPI_INT_UART5_ANDED   (1 << 8)
#define CCM_WIMR0_IPI_INT_UART4_ANDED   (1 << 9)
#define CCM_WIMR0_IPI_INT_UART3_ANDED   (1 << 10)
#define CCM_WIMR0_IPI_INT_UART2_ANDED   (1 << 11)
#define CCM_WIMR0_IPI_INT_UART1_ANDED   (1 << 12)
#define CCM_WIMR0_IPI_INT_SIM_DATA_IRQ  (1 << 13)
#define CCM_WIMR0_IPI_INT_SDHC2         (1 << 14)
#define CCM_WIMR0_IPI_INT_SDHC1         (1 << 15)
#define CCM_WIMR0_IPI_INT_RTC           (1 << 16)
#define CCM_WIMR0_IPI_INT_PWM           (1 << 17)
#define CCM_WIMR0_IPI_INT_KPP           (1 << 18)
#define CCM_WIMR0_IPI_INT_IIM           (1 << 19)
#define CCM_WIMR0_IPI_INT_GPT           (1 << 20)
#define CCM_WIMR0_IPI_INT_FIR           (1 << 21)
#define CCM_WIMR0_IPI_INT_EPIT2         (1 << 22)
#define CCM_WIMR0_IPI_INT_EPIT1         (1 << 23)
#define CCM_WIMR0_IPI_INT_CSPI2         (1 << 24)
#define CCM_WIMR0_IPI_INT_CSPI1         (1 << 25)
#define CCM_WIMR0_IPI_INT_POWER_FAIL    (1 << 26)
#define CCM_WIMR0_IPI_INT_CSPI3         (1 << 27)
#define CCM_WIMR0_RESERVED28            (1 << 28)
#define CCM_WIMR0_RESERVED29            (1 << 29)
#define CCM_WIMR0_RESERVED30            (1 << 30)
#define CCM_WIMR0_RESERVED31            (1 << 31)

/* LDC */
/* 32 bits specify value */

/* DCVR0-DCVR3 */
#define CCM_DCVR_ULV                    (0x3ff << 22) /* Upper limit */
#define CCM_DCVR_ULV_POS                (22)
#define CCM_DCVR_LLV                    (0x3ff << 12) /* Lower limit */
#define CCM_DCVR_LLV_POS                (12)
#define CCM_DCVR_ELV                    (0x3ff <<  2) /* Emergency limit */
#define CCM_DCVR_ELV_POS                (2)


/* LTR0 */
#define CCM_LTR0_UPTHR                  (0x3f << 22)
#define CCM_LTR0_UPTHR_POS              (22)
#define CCM_LTR0_DNTHR                  (0x3f << 16)
#define CCM_LTR0_DNTHR_POS              (16)
/* for div_3_clk */
#define CCM_LTR0_DIV3CK                 (0x3 << 1)
#define CCM_LTR0_DIV3CK_POS             (1)
#define CCM_LTR0_DIV3CK_2048            (0x0 << 1) /* 1/2048 ARM clock */
#define CCM_LTR0_DIV3CK_8192            (0x1 << 1) /* 1/8192 ARM clock */
#define CCM_LTR0_DIV3CK_32768           (0x2 << 1) /* 1/32768 ARM clock */
#define CCM_LTR0_DIV3CK_131072          (0x3 << 1) /* 1/131072 ARM clock */

/* LTR1 */
#define CCM_LTR1_LTBRSH                 (1 << 23)
#define CCM_LTR1_LTBRSR                 (1 << 22)
#define CCM_LTR1_DNCNT                  (0xff << 14)
#define CCM_LTR1_DNCNT_POS              (14)
#define CCM_LTR1_UPCNT                  (0xff << 6)
#define CCM_LTR1_UPCNT_POS              (6)
#define CCM_LTR1_PNCTHR                 (0x3f << 0)
#define CCM_LTR1_PNCTHR_POS             (0)

/* LTR2 */
#define CCM_LTR2_EMAC                   (0x1ff)
#define CCM_LTR2_EMAC_POS               (0)

/* PMCR0 */
#define CCM_PMCR0_DFSUP_MCUPLL          (1 << 31)
#define CCM_PMCR0_DFSUP_MCUPLL_POS      (31)
#define CCM_PMCR0_DFSUP_POST_DIVIDERS   (1 << 30)
#define CCM_PMCR0_DVSUP                 (0x3 << 28)
#define CCM_PMCR0_DVSUP_POS             (28)
#define CCM_PMCR0_UDSC                  (1 << 27)
#define CCM_PMCR0_VSCNT                 (0x7 << 24)
#define CCM_PMCR0_VSCNT_POS             (24)
#define CCM_PMCR0_DVFEV                 (1 << 23)
#define CCM_PMCR0_DVFIS                 (1 << 22)
#define CCM_PMCR0_LBMI                  (1 << 21)
#define CCM_PMCR0_LBFL                  (1 << 20)
#define CCM_PMCR0_LBCF                  (0x3 << 18)
#define CCM_PMCR0_LBCF_4                (0x0 << 18)
#define CCM_PMCR0_LBCF_8                (0x1 << 18)
#define CCM_PMCR0_LBCF_12               (0x2 << 18)
#define CCM_PMCR0_LBCF_16               (0x3 << 18)
#define CCM_PMCR0_PTVIS                 (1 << 17)
#define CCM_PMCR0_UPDTEN                (1 << 16)
#define CCM_PMCR0_FSVAIM                (1 << 15)
#define CCM_PMCR0_FSVAI                 (0x3 << 13)
#define CCM_PMCR0_FSVAI_NO_INT          (0x0 << 13)
#define CCM_PMCR0_FSVAI_INCREASE        (0x1 << 13)
#define CCM_PMCR0_FSVAI_DECREASE        (0x2 << 13)
#define CCM_PMCR0_FSVAI_INCREASE_NOW    (0x3 << 13)
#define CCM_PMCR0_FSVAI_POS             (13)
#define CCM_PMCR0_DPVCR                 (1 << 12)
#define CCM_PMCR0_DPVV                  (1 << 11)
#define CCM_PMCR0_WFIM                  (1 << 10)
#define CCM_PMCR0_DRCE3                 (1 << 9)
#define CCM_PMCR0_DRCE2                 (1 << 8)
#define CCM_PMCR0_DRCE1                 (1 << 7)
#define CCM_PMCR0_DRCE0                 (1 << 6)
#define CCM_PMCR0_DCR                   (1 << 5) /* 512 vs 256 count */
#define CCM_PMCR0_DVFEN                 (1 << 4)
#define CCM_PMCR0_PTVAIM                (1 << 3)
#define CCM_PMCR0_PTVAI                 (0x3 << 1)
#define CCM_PMCR0_PTVAI_NO_INT          (0x0 << 1)
#define CCM_PMCR0_PTVAI_DECREASE        (0x1 << 1)
#define CCM_PMCR0_PTVAI_INCREASE        (0x2 << 1)
#define CCM_PMCR0_PTVAI_INCREASE_NOW    (0x3 << 1)
#define CCM_PMCR0_DPTEN                 (1 << 0)


/* PMCR1 */
#define CCM_PMCR1_DVGP_POS              (0)
#define CCM_PMCR1_DVGP_MASK             (0xf << 0)

/* IC revision 2.0 or greater ONLY! */
#define CCM_PMCR1_EMIRQ_EN              (1 << 8)
#define CCM_PMCR1_PLLRDIS               (1 << 7) /* No PLL reset on switch */


/* WEIM - CS0 */
#define CSCRU                           0x00
#define CSCRL                           0x04
#define CSCRA                           0x08

/* ESDCTL */
#define ESDCTL_ESDCTL0                  0x00
#define ESDCTL_ESDCFG0                  0x04
#define ESDCTL_ESDCTL1                  0x08
#define ESDCTL_ESDCFG1                  0x0C
#define ESDCTL_ESDMISC                  0x10

/* More UART 1 Register defines */
#define URXD1                           (*(REG32_PTR_T)(UART1_BASE_ADDR+0x00))
#define UTXD1                           (*(REG32_PTR_T)(UART1_BASE_ADDR+0x40))
#define UCR1_1                          (*(REG32_PTR_T)(UART1_BASE_ADDR+0x80))
#define UCR2_1                          (*(REG32_PTR_T)(UART1_BASE_ADDR+0x84))
#define UCR3_1                          (*(REG32_PTR_T)(UART1_BASE_ADDR+0x88))
#define UCR4_1                          (*(REG32_PTR_T)(UART1_BASE_ADDR+0x8C))
#define UFCR1                           (*(REG32_PTR_T)(UART1_BASE_ADDR+0x90))
#define USR1_1                          (*(REG32_PTR_T)(UART1_BASE_ADDR+0x94))
#define USR2_1                          (*(REG32_PTR_T)(UART1_BASE_ADDR+0x98))
#define UTS1                            (*(REG32_PTR_T)(UART1_BASE_ADDR+0xB4))

#define UCR1_2                          (*(REG32_PTR_T)(UART2_BASE_ADDR+0x80))
#define UCR1_3                          (*(REG32_PTR_T)(UART3_BASE_ADDR+0x80))
#define UCR1_4                          (*(REG32_PTR_T)(UART4_BASE_ADDR+0x80))
#define UCR1_5                          (*(REG32_PTR_T)(UART5_BASE_ADDR+0x80))

/*
 * UART Control Register 0 Bit Fields.
 */
#define EUARTUCR1_ADEN      (1 << 15)   // Auto detect interrupt
#define EUARTUCR1_ADBR      (1 << 14)   // Auto detect baud rate
#define EUARTUCR1_TRDYEN    (1 << 13)   // Transmitter ready interrupt enable
#define EUARTUCR1_IDEN      (1 << 12)   // Idle condition interrupt
#define EUARTUCR1_RRDYEN    (1 << 9)    // Recv ready interrupt enable
#define EUARTUCR1_RDMAEN    (1 << 8)    // Recv ready DMA enable
#define EUARTUCR1_IREN      (1 << 7)    // Infrared interface enable
#define EUARTUCR1_TXMPTYEN  (1 << 6)    // Transimitter empt  interrupt enable
#define EUARTUCR1_RTSDEN    (1 << 5)    // RTS delta interrupt enable
#define EUARTUCR1_SNDBRK    (1 << 4)    // Send break
#define EUARTUCR1_TDMAEN    (1 << 3)    // Transmitter ready DMA enable
#define EUARTUCR1_DOZE      (1 << 1)    // Doze
#define EUARTUCR1_UARTEN    (1 << 0)    // UART enabled
#define EUARTUCR2_ESCI      (1 << 15)   // Escape seq interrupt enable
#define EUARTUCR2_IRTS      (1 << 14)   // Ignore RTS pin
#define EUARTUCR2_CTSC      (1 << 13)   // CTS pin control
#define EUARTUCR2_CTS       (1 << 12)   // Clear to send
#define EUARTUCR2_ESCEN     (1 << 11)   // Escape enable
#define EUARTUCR2_PREN      (1 << 8)    // Parity enable
#define EUARTUCR2_PROE      (1 << 7)    // Parity odd/even
#define EUARTUCR2_STPB      (1 << 6)    // Stop
#define EUARTUCR2_WS        (1 << 5)    // Word size
#define EUARTUCR2_RTSEN     (1 << 4)    // Request to send interrupt enable
#define EUARTUCR2_ATEN      (1 << 3)    // Aging timer enable
#define EUARTUCR2_TXEN      (1 << 2)    // Transmitter enabled
#define EUARTUCR2_RXEN      (1 << 1)    // Receiver enabled
#define EUARTUCR2_SRST_     (1 << 0)    // SW reset
#define EUARTUCR3_PARERREN  (1 << 12)   // Parity enable
#define EUARTUCR3_FRAERREN  (1 << 11)   // Frame error interrupt enable
#define EUARTUCR3_ADNIMP    (1 << 7)    // Autobaud detection not improved
#define EUARTUCR3_RXDSEN    (1 << 6)    // Receive status interrupt  enable
#define EUARTUCR3_AIRINTEN  (1 << 5)    // Async IR wake interrupt enable
#define EUARTUCR3_AWAKEN    (1 << 4)    // Async wake interrupt enable
#define EUARTUCR3_RXDMUXSEL (1 << 2)    // RXD muxed input selected
#define EUARTUCR3_INVT      (1 << 1)    // Inverted Infrared transmission
#define EUARTUCR3_ACIEN     (1 << 0)    // Autobaud counter interrupt  enable
#define EUARTUCR4_CTSTL_32  (32 << 10)  // CTS trigger level (32 chars)
#define EUARTUCR4_INVR      (1 << 9)    // Inverted infrared reception
#define EUARTUCR4_ENIRI     (1 << 8)    // Serial infrared interrupt enable
#define EUARTUCR4_WKEN      (1 << 7)    // Wake interrupt enable
#define EUARTUCR4_IRSC      (1 << 5)    // IR special case
#define EUARTUCR4_LPBYP     (1 << 4)    // Low power bypass
#define EUARTUCR4_TCEN      (1 << 3)    // Transmit complete interrupt  enable
#define EUARTUCR4_BKEN      (1 << 2)    // Break condition interrupt enable
#define EUARTUCR4_OREN      (1 << 1)    // Receiver overrun interrupt enable
#define EUARTUCR4_DREN      (1 << 0)    // Recv data ready interrupt enable
#define EUARTUFCR_RXTL_SHF  0           // Receiver trigger level shift
#define EUARTUFCR_RFDIV_1   (5 << 7)    // Reference freq divider (div> 1)
#define EUARTUFCR_RFDIV_2   (4 << 7)    // Reference freq divider (div> 2)
#define EUARTUFCR_RFDIV_3   (3 << 7)    // Reference freq  divider (div 3)
#define EUARTUFCR_RFDIV_4   (2 << 7)    // Reference freq divider (div 4)
#define EUARTUFCR_RFDIV_5   (1 << 7)    // Reference freq divider (div 5)
#define EUARTUFCR_RFDIV_6   (0 << 7)    // Reference freq divider (div 6)
#define EUARTUFCR_RFDIV_7   (6 << 7)    // Reference freq divider (div 7)
#define EUARTUFCR_TXTL_SHF  10          // Transmitter trigger level shift
#define EUARTUSR1_PARITYERR (1 << 15)   // Parity error interrupt flag
#define EUARTUSR1_RTSS      (1 << 14)   // RTS pin status
#define EUARTUSR1_TRDY      (1 << 13)   // Transmitter ready interrupt/dma flag
#define EUARTUSR1_RTSD      (1 << 12)   // RTS delta
#define EUARTUSR1_ESCF      (1 << 11)   // Escape seq interrupt flag
#define EUARTUSR1_FRAMERR   (1 << 10)   // Frame error interrupt flag
#define EUARTUSR1_RRDY      (1 << 9)    // Receiver ready  interrupt/dma flag
#define EUARTUSR1_AGTIM     (1 << 8)    // Aging timeout interrupt status
#define EUARTUSR1_RXDS      (1 << 6)    // Receiver idle interrupt flag
#define EUARTUSR1_AIRINT    (1 << 5)    // Async IR wake interrupt flag
#define EUARTUSR1_AWAKE     (1 << 4)    // Aysnc wake interrupt flag
#define EUARTUSR2_ADET      (1 << 15)   // Auto baud rate detect  complete
#define EUARTUSR2_TXFE      (1 << 14)   // Transmit buffer FIFO empty
#define EUARTUSR2_IDLE      (1 << 12)   // Idle condition
#define EUARTUSR2_ACST      (1 << 11)   // Autobaud counter stopped
#define EUARTUSR2_IRINT     (1 << 8)    // Serial infrared interrupt flag
#define EUARTUSR2_WAKE      (1 << 7)    // Wake
#define EUARTUSR2_RTSF      (1 << 4)    // RTS edge interrupt flag
#define EUARTUSR2_TXDC      (1 << 3)    // Transmitter complete
#define EUARTUSR2_BRCD      (1 << 2)    // Break condition
#define EUARTUSR2_ORE       (1 << 1)    // Overrun error
#define EUARTUSR2_RDR       (1 << 0)    // Recv data ready
#define EUARTUTS_FRCPERR    (1 << 13)   // Force parity error
#define EUARTUTS_LOOP       (1 << 12)   // Loop tx and rx
#define EUARTUTS_TXEMPTY    (1 << 6)    // TxFIFO empty
#define EUARTUTS_RXEMPTY    (1 << 5)    // RxFIFO empty
#define EUARTUTS_TXFULL     (1 << 4)    // TxFIFO full
#define EUARTUTS_RXFULL     (1 << 3)    // RxFIFO full
#define EUARTUTS_SOFTRST    (1 << 0)    // Software reset

/* SDMA */
#define SDMA_MC0PTR         (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x000))
#define SDMA_INTR           (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x004))
#define SDMA_STOP_STAT      (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x008))
#define SDMA_HSTART         (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x00C))
#define SDMA_EVTOVR         (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x010))
#define SDMA_DSPOVR         (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x014))
#define SDMA_HOSTOVR        (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x018))
#define SDMA_EVTPEND        (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x01C))
#define SDMA_DSPENBL        (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x020))
#define SDMA_RESET          (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x024))
#define SDMA_EVTERR         (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x028))
#define SDMA_INTRMSK        (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x02C))
#define SDMA_PSW            (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x030))
#define SDMA_EVTERRDBG      (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x034))
#define SDMA_CONFIG         (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x038))
#define SDMA_ONCE_ENB       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x040))
#define SDMA_ONCE_DATA      (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x044))
#define SDMA_ONCE_INSTR     (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x048))
#define SDMA_ONCE_STAT      (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x04C))
#define SDMA_ONCE_CMD       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x050))
#define SDMA_EVT_MIRROR     (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x054))
#define SDMA_ILLINSTADDR    (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x058))
#define SDMA_CHN0ADDR       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x05C))
#define SDMA_ONCE_RTB       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x060))
#define SDMA_XTRIG_CONF1    (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x070))
#define SDMA_XTRIG_CONF2    (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x074))

/* SDMA_CHNENBL: 0x080 - 0x0FC */
#define SDMA_CHNENBL(n)     (((REG32_PTR_T)(SDMA_BASE_ADDR + 0x080))[n]) /* 0..31 */

/* SDMA_CHNPRI: 0x100 - 0x17C */
#define SDMA_CHNPRI(n)      (((REG32_PTR_T)(SDMA_BASE_ADDR + 0x100))[n]) /* 0..31 */

#define SDMA_ONCE_COUNT     (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x200))
#define SDMA_ONCE_ECTL      (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x204))
#define SDMA_ONCE_EAA       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x208))
#define SDMA_ONCE_EAB       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x20C))
#define SDMA_ONCE_EAM       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x210))
#define SDMA_ONCE_ED        (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x214))
#define SDMA_ONCE_EDM       (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x218))
#define SDMA_ONCE_PCMATCH   (*(REG32_PTR_T)(SDMA_BASE_ADDR + 0x21C))

/* SDMA_RESET */
#define SDMA_RESET_RESCHED  (0x1 << 1)
#define SDMA_RESET_RESET    (0x1 << 0)

/* SDMA_PSW */
#define SDMA_PSW_NCP        (0x7 << 13)
#define SDMA_PSW_NCR        (0x1f << 8)
#define SDMA_PSW_CCP        (0x7 << 5)
#define SDMA_PSW_CCR        (0x1f << 0)

/* SDMA_CONFIG */
#define SDMA_CONFIG_DSPDMA  (0x1 << 12)
#define SDMA_CONFIG_RTDOBS  (0x1 << 11)
#define SDMA_CONFIG_ACR     (0x1 << 4)
#define SDMA_CONFIG_CSM     (0x3 << 0)
    #define SDMA_CONFIG_CSM_STATIC            (0x0 << 0)
    #define SDMA_CONFIG_CSM_DYNAMIC_LOW_POWER (0x1 << 0)
    #define SDMA_CONFIG_CSM_DYNAMIC_NO_LOOP   (0x2 << 0)
    #define SDMA_CONFIG_CSM_DYNAMIC           (0x3 << 0)

/* SDMA_ONCE_ENB */
#define SDMA_ONCE_ENB_ENB   (0x1 << 0)

/* SDMA_ONCE_STAT */
#define SDMA_ONCE_STAT_PST  (0xf << 12)
    #define SDMA_ONCE_STAT_PST_PROGRAM                  (0x0 << 12)
    #define SDMA_ONCE_STAT_PST_DATA                     (0x1 << 12)
    #define SDMA_ONCE_STAT_PST_CHANGE_OF_FLOW           (0x2 << 12)
    #define SDMA_ONCE_STAT_PST_CHANGE_OF_FLOW_IN_LOOP   (0x3 << 12)
    #define SDMA_ONCE_STAT_PST_DEBUG                    (0x4 << 12)
    #define SDMA_ONCE_STAT_PST_FUNCTIONAL_UNIT          (0x5 << 12)
    #define SDMA_ONCE_STAT_PST_SLEEP                    (0x6 << 12)
    #define SDMA_ONCE_STAT_PST_SAVE                     (0x7 << 12)
    #define SDMA_ONCE_STAT_PST_PROGRAM_IN_SLEEP         (0x8 << 12)
    #define SDMA_ONCE_STAT_PST_DATA_IN_SLEEP            (0x9 << 12)
    #define SDMA_ONCE_STAT_PST_CHANGE_OF_FLOW_IN_SLEEP  (0xa << 12)
    #define SDMA_ONCE_STAT_PST_CHANGE_OF_FLOW_IN_LOOP_IN_SLEEP (0xb << 12)
    #define SDMA_ONCE_STAT_PST_DEBUG_IN_SLEEP           (0xc << 12)
    #define SDMA_ONCE_STAT_PST_FUNCTIONAL_UNIT_IN_SLEEP (0xd << 12)
    #define SDMA_ONCE_STAT_PST_SLEEP_AFTER_RESET        (0xe << 12)
    #define SDMA_ONCE_STAT_PST_RESTORE                  (0xf << 12)
#define SDMA_ONCE_STAT_RCV  (0x1 << 11)
#define SDMA_ONCE_STAT_EDR  (0x1 << 10)
#define SDMA_ONCE_STAT_ODR  (0x1 << 9)
#define SDMA_ONCE_STAT_SWB  (0x1 << 8)
#define SDMA_ONCE_STAT_MST  (0x1 << 7)
#define SDMA_ONCE_STAT_ECDR (0x7 << 0)
    #define SDMA_ONCE_STAT_ECDR_MATCHED_ADDRA_COND (0x1 << 0)
    #define SDMA_ONCE_STAT_ECDR_MATCHED_ADDRB_COND (0x1 << 1)
    #define SDMA_ONCE_STAT_ECDR_MATCHED_DATA_COND  (0x1 << 2)

/* SDMA_ONCE_CMD */
#define SDMA_ONCE_CMD_RSTATUS       0x0
#define SDMA_ONCE_CMD_DMOV          0x1
#define SDMA_ONCE_CMD_EXEC_ONCE     0x2
#define SDMA_ONCE_CMD_RUN_CORE      0x3
#define SDMA_ONCE_CMD_EXEC_CORE     0x4
#define SDMA_ONCE_CMD_DEBUG_RQST    0x5
#define SDMA_ONCE_CMD_RBUFFER       0x6
/* 7-15 reserved */

/* SDMA_CHN0ADDR */
#define SDMA_CHN0ADDR_SMSZ          (0x1 << 14)
/* 13:0 = 0x0050 by default (std. boot code) */

/* SDMA_EVT_MIRROR */
#define SDMA_EVT_MIRROR_EVENTS(n)   (0x1 << (n))

/* SDMA_XTRIG_CONF1 */
#define SDMA_XTRIG_CONF1_CNF3       (0x1 << 30)
#define SDMA_XTRIG_CONF1_NUM3       (0x1f << 24)
#define SDMA_XTRIG_CONF1_CNF2       (0x1 << 22)
#define SDMA_XTRIG_CONF1_NUM2       (0x1f << 16)
#define SDMA_XTRIG_CONF1_CNF1       (0x1 << 14)
#define SDMA_XTRIG_CONF1_NUM1       (0x1f << 8)
#define SDMA_XTRIG_CONF1_CNF0       (0x1 << 6)
#define SDMA_XTRIG_CONF1_NUM0       (0x1f << 0)

/* SDMA_XTRIG_CONF2 */
#define SDMA_XTRIG_CONF2_CNF7       (0x1 << 30)
#define SDMA_XTRIG_CONF2_NUM7       (0x1f << 24)
#define SDMA_XTRIG_CONF2_CNF6       (0x1 << 22)
#define SDMA_XTRIG_CONF2_NUM6       (0x1f << 16)
#define SDMA_XTRIG_CONF2_CNF5       (0x1 << 14)
#define SDMA_XTRIG_CONF2_NUM5       (0x1f << 8)
#define SDMA_XTRIG_CONF2_CNF4       (0x1 << 6)
#define SDMA_XTRIG_CONF2_NUM4       (0x1f << 0)

/* SDMA_CHNENBL(n) */
#define SDMA_CHNENBL_ENBL(n)        (0x1 << (n))


#define L2CC_ENABLED

/* Assuming 26MHz input clock */
/*                         PD          MFD           MFI          MFN */
#define MPCTL_PARAM_208  ((1 << 26) + (0   << 16) + (8  << 10) + (0 << 0))
#define MPCTL_PARAM_399  ((0 << 26) + (51  << 16) + (7  << 10) + (35 << 0))
#define MPCTL_PARAM_532  ((0 << 26) + (51  << 16) + (10 << 10) + (12 << 0))

/* UPCTL                   PD             MFD              MFI          MFN */
#define UPCTL_PARAM_288  (((1-1) << 26) + ((13-1) << 16) + (5  << 10) + (7  << 0))
#define UPCTL_PARAM_240  (((2-1) << 26) + ((13-1) << 16) + (9  << 10) + (3  << 0))

/* PDR0 */
#define PDR0_208_104_52     0xFF870D48  /* ARM=208MHz, HCLK=104MHz, IPG=52MHz */
#define PDR0_399_66_66      0xFF872B28  /* ARM=399MHz, HCLK=IPG=66.5MHz */
#define PDR0_399_133_66     0xFF871650  /* ARM=399MHz, HCLK=133MHz, IPG=66.5MHz */
#define PDR0_532_133_66     0xFF871E58  /* ARM=532MHz, HCLK=133MHz, IPG=66MHz */
#define PDR0_665_83_66      0xFF873D78  /* ARM=532MHz, HCLK=133MHz, IPG=66MHz */
#define PDR0_665_133_66     0xFF872660  /* ARM=532MHz, HCLK=133MHz, IPG=66MHz */

#define PBC_BASE            CS4_BASE_ADDR    /* Peripheral Bus Controller */

#define PBC_BSTAT2                  0x2
#define PBC_BCTRL1                  0x4
#define PBC_BCTRL1_CLR              0x6
#define PBC_BCTRL2                  0x8
#define PBC_BCTRL2_CLR              0xA
#define PBC_BCTRL3                  0xC
#define PBC_BCTRL3_CLR              0xE
#define PBC_BCTRL4                  0x10
#define PBC_BCTRL4_CLR              0x12
#define PBC_BSTAT1                  0x14
#define MX31EVB_CS_LAN_BASE         (CS4_BASE_ADDR + 0x00020000 +  0x300)
#define MX31EVB_CS_UART_BASE        (CS4_BASE_ADDR + 0x00010000)

#define REDBOOT_IMAGE_SIZE              0x40000
 
#define SDRAM_WORKAROUND_FULL_PAGE
 
#define ARMHIPG_208_52_52         /* ARM: 208MHz, HCLK=IPG=52MHz*/
#define ARMHIPG_52_52_52          /* ARM: 52MHz, HCLK=IPG=52MHz*/
#define ARMHIPG_399_66_66
#define ARMHIPG_399_133_66
 
/* MX31 EVB SDRAM is from 0x80000000, 64M */
#define SDRAM_BASE_ADDR                 CSD0_BASE_ADDR
#define SDRAM_SIZE                      0x04000000

#define UART_WIDTH_32         /* internal UART is 32bit access only */
#define EXT_UART_x16

#define UART_WIDTH_32         /* internal UART is 32bit access only */

#define FLASH_BURST_MODE_ENABLE 1
#define SDRAM_COMPARE_CONST1    0x55555555
#define SDRAM_COMPARE_CONST2    0xAAAAAAAA
#define UART_FIFO_CTRL          0x881
#define TIMEOUT                 1000

/* Timer frequency */
/* timer is based on ipg_clk */
#define TIMER_FREQ (66000000)

#endif /* __IMX31L_H__ */
