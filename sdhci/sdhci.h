/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 *
 */
#ifndef __SDHCI_SDHCI_H__
#define __SDHCI_SDHCI_H__

#include "mmc.h"

#define word32				*(volatile unsigned int *)(long)
/*
 * Controller registers
 */

#define SDHCI_DMA_ADDRESS	0x00

#define SDHCI_BLOCK_SIZE	0x04
#define  SDHCI_MAKE_BLKSZ(dma, blksz) (((dma & 0x7) << 12) | (blksz & 0xFFF))

#define SDHCI_BLOCK_COUNT	0x06

#define SDHCI_ARGUMENT		0x08

#define SDHCI_TRANSFER_MODE	0x0C
#define  SDHCI_TRNS_DMA		0x01
#define  SDHCI_TRNS_BLK_CNT_EN	0x02
#define  SDHCI_TRNS_ACMD12	0x04
#define  SDHCI_TRNS_READ	0x10
#define  SDHCI_TRNS_MULTI	0x20

#define SDHCI_COMMAND		0x0E
#define  SDHCI_CMD_RESP_MASK	0x03
#define  SDHCI_CMD_CRC		0x08
#define  SDHCI_CMD_INDEX	0x10
#define  SDHCI_CMD_DATA		0x20
#define  SDHCI_CMD_ABORTCMD	0xC0

#define  SDHCI_CMD_RESP_NONE	0x00
#define  SDHCI_CMD_RESP_LONG	0x01
#define  SDHCI_CMD_RESP_SHORT	0x02
#define  SDHCI_CMD_RESP_SHORT_BUSY 0x03

#define SDHCI_MAKE_CMD(c, f) (((c & 0xff) << 8) | (f & 0xff))
#define SDHCI_GET_CMD(c) ((c>>8) & 0x3f)

#define SDHCI_RESPONSE		0x10

#define SDHCI_BUFFER		0x20

#define SDHCI_PRESENT_STATE	0x24
#define  SDHCI_CMD_INHIBIT	0x00000001
#define  SDHCI_DATA_INHIBIT	0x00000002
#define  SDHCI_DOING_WRITE	0x00000100
#define  SDHCI_DOING_READ	0x00000200
#define  SDHCI_SPACE_AVAILABLE	0x00000400
#define  SDHCI_DATA_AVAILABLE	0x00000800
#define  SDHCI_CARD_PRESENT	0x00010000
#define  SDHCI_CARD_STATE_STABLE	0x00020000
#define  SDHCI_CARD_DETECT_PIN_LEVEL	0x00040000
#define  SDHCI_WRITE_PROTECT	0x00080000

#define SDHCI_HOST_CONTROL	0x28
#define  SDHCI_CTRL_LED		0x01
#define  SDHCI_CTRL_4BITBUS	0x02
#define  SDHCI_CTRL_HISPD	0x04
#define  SDHCI_CTRL_DMA_MASK	0x18
#define   SDHCI_CTRL_SDMA	0x00
#define   SDHCI_CTRL_ADMA1	0x08
#define   SDHCI_CTRL_ADMA32	0x10
#define   SDHCI_CTRL_ADMA64	0x18
#define  SDHCI_CTRL_8BITBUS	0x20
#define  SDHCI_CTRL_CD_TEST_INS	0x40
#define  SDHCI_CTRL_CD_TEST	0x80

#define SDHCI_POWER_CONTROL	0x29
#define  SDHCI_POWER_ON		0x01
#define  SDHCI_POWER_180	0x0A
#define  SDHCI_POWER_300	0x0C
#define  SDHCI_POWER_330	0x0E

#define SDHCI_BLOCK_GAP_CONTROL	0x2A

#define SDHCI_WAKE_UP_CONTROL	0x2B
#define  SDHCI_WAKE_ON_INT	0x01
#define  SDHCI_WAKE_ON_INSERT	0x02
#define  SDHCI_WAKE_ON_REMOVE	0x04

#define SDHCI_CLOCK_CONTROL	0x2C
#define  SDHCI_DIVIDER_SHIFT	8
#define  SDHCI_DIVIDER_HI_SHIFT	6
#define  SDHCI_DIV_MASK	0xFF
#define  SDHCI_DIV_MASK_LEN	8
#define  SDHCI_DIV_HI_MASK	0x300
#define  SDHCI_CLOCK_CARD_EN	0x0004
#define  SDHCI_CLOCK_INT_STABLE	0x0002
#define  SDHCI_CLOCK_INT_EN	0x0001

#define SDHCI_TIMEOUT_CONTROL	0x2E

#define SDHCI_SOFTWARE_RESET	0x2F
#define  SDHCI_RESET_ALL	0x01
#define  SDHCI_RESET_CMD	0x02
#define  SDHCI_RESET_DATA	0x04

#define SDHCI_INT_STATUS	0x30
#define SDHCI_INT_ENABLE	0x34
#define SDHCI_SIGNAL_ENABLE	0x38
#define  SDHCI_INT_RESPONSE	0x00000001
#define  SDHCI_INT_DATA_END	0x00000002
#define  SDHCI_INT_DMA_END	0x00000008
#define  SDHCI_INT_SPACE_AVAIL	0x00000010
#define  SDHCI_INT_DATA_AVAIL	0x00000020
#define  SDHCI_INT_CARD_INSERT	0x00000040
#define  SDHCI_INT_CARD_REMOVE	0x00000080
#define  SDHCI_INT_CARD_INT	0x00000100
#define  SDHCI_INT_ERROR	0x00008000
#define  SDHCI_INT_TIMEOUT	0x00010000
#define  SDHCI_INT_CRC		0x00020000
#define  SDHCI_INT_END_BIT	0x00040000
#define  SDHCI_INT_INDEX	0x00080000
#define  SDHCI_INT_DATA_TIMEOUT	0x00100000
#define  SDHCI_INT_DATA_CRC	0x00200000
#define  SDHCI_INT_DATA_END_BIT	0x00400000
#define  SDHCI_INT_BUS_POWER	0x00800000
#define  SDHCI_INT_ACMD12ERR	0x01000000
#define  SDHCI_INT_ADMA_ERROR	0x02000000

#define  SDHCI_INT_NORMAL_MASK	0x00007FFF
#define  SDHCI_INT_ERROR_MASK	0xFFFF8000

#define  SDHCI_INT_CMD_MASK	(SDHCI_INT_RESPONSE | SDHCI_INT_TIMEOUT | \
				 SDHCI_INT_CRC | SDHCI_INT_END_BIT | SDHCI_INT_INDEX)
#define  SDHCI_INT_DATA_MASK	(SDHCI_INT_DATA_END | SDHCI_INT_DMA_END | \
				 SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL | \
				 SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_DATA_CRC | \
				 SDHCI_INT_DATA_END_BIT | SDHCI_INT_ADMA_ERROR)
#define SDHCI_INT_ALL_MASK	((unsigned int)-1)

#define SDHCI_ACMD12_ERR	0x3C

#define SDHCI_HOST_CONTROL2             0x3E
#define  SDHCI_CTRL_UHS_MASK            0x0007
#define   SDHCI_CTRL_UHS_SDR12		    0x0000
#define   SDHCI_CTRL_UHS_SDR25		    0x0001
#define   SDHCI_CTRL_UHS_SDR50		    0x0002
#define   SDHCI_CTRL_UHS_SDR104		    0x0003
#define   SDHCI_CTRL_UHS_DDR50		    0x0004
#define   SDHCI_CTRL_HS400		        0x0007 /* Non-standard only for RK3588*/

#define  SDHCI_CTRL_VDD_180             0x0008
#define  SDHCI_CTRL_DRV_TYPE_MASK       0x0030
#define   SDHCI_CTRL_DRV_TYPE_B         0x0000
#define   SDHCI_CTRL_DRV_TYPE_A         0x0010
#define   SDHCI_CTRL_DRV_TYPE_C         0x0020
#define   SDHCI_CTRL_DRV_TYPE_D         0x0030
#define  SDHCI_CTRL_EXEC_TUNING         0x0040
#define  SDHCI_CTRL_TUNED_CLK           0x0080
#define  SDHCI_CTRL_PRESET_VAL_ENABLE   0x8000

#define SDHCI_CAPABILITIES	0x40
#define  SDHCI_TIMEOUT_CLK_MASK	0x0000003F
#define  SDHCI_TIMEOUT_CLK_SHIFT 0
#define  SDHCI_TIMEOUT_CLK_UNIT	0x00000080
#define  SDHCI_CLOCK_BASE_MASK	0x00003F00
#define  SDHCI_CLOCK_V3_BASE_MASK	0x0000FF00
#define  SDHCI_CLOCK_BASE_SHIFT	8
#define  SDHCI_MAX_BLOCK_MASK	0x00030000
#define  SDHCI_MAX_BLOCK_SHIFT  16
#define  SDHCI_CAN_DO_8BIT	0x00040000
#define  SDHCI_CAN_DO_ADMA2	0x00080000
#define  SDHCI_CAN_DO_ADMA1	0x00100000
#define  SDHCI_CAN_DO_HISPD	0x00200000
#define  SDHCI_CAN_DO_SDMA	0x00400000
#define  SDHCI_CAN_VDD_330	0x01000000
#define  SDHCI_CAN_VDD_300	0x02000000
#define  SDHCI_CAN_VDD_180	0x04000000
#define  SDHCI_CAN_64BIT	0x10000000

#define SDHCI_CAPABILITIES_1	0x44

#define SDHCI_MAX_CURRENT	0x48

/* 4C-4F reserved for more max current */

#define SDHCI_SET_ACMD12_ERROR	0x50
#define SDHCI_SET_INT_ERROR	0x52

#define SDHCI_ADMA_ERROR	0x54

/* 55-57 reserved */

#define SDHCI_ADMA_ADDRESS	0x58

/* 60-FB reserved */

#define SDHCI_SLOT_INT_STATUS	0xFC

#define SDHCI_HOST_VERSION	0xFE
#define  SDHCI_VENDOR_VER_MASK	0xFF00
#define  SDHCI_VENDOR_VER_SHIFT	8
#define  SDHCI_SPEC_VER_MASK	0x00FF
#define  SDHCI_SPEC_VER_SHIFT	0
#define   SDHCI_SPEC_100	0
#define   SDHCI_SPEC_200	1
#define   SDHCI_SPEC_300	2

/*
 * End of controller registers.
 */

#define SDHCI_MAX_DIV_SPEC_200	256
#define SDHCI_MAX_DIV_SPEC_300	2046

/*
* platform_infos
*/
#define SDHCI_PLATFORM_REMOVABLE	(1 << 0)
#define SDHCI_PLATFORM_NO_EMMC_HS200	(1 << 1)
#define SDHCI_PLATFORM_EMMC_1V8_POWER	(1 << 2)
#define SDHCI_PLATFORM_NO_CLK_BASE	(1 << 3)
/*
 * quirks
 */
#define SDHCI_QUIRK_32BIT_DMA_ADDR	(1 << 0)
#define SDHCI_QUIRK_REG32_RW		(1 << 1)
#define SDHCI_QUIRK_BROKEN_R1B		(1 << 2)
#define SDHCI_QUIRK_NO_HISPD_BIT	(1 << 3)
#define SDHCI_QUIRK_BROKEN_VOLTAGE	(1 << 4)
#define SDHCI_QUIRK_NO_CD		(1 << 5)
#define SDHCI_QUIRK_WAIT_SEND_CMD	(1 << 6)
#define SDHCI_QUIRK_NO_SIMULT_VDD_AND_POWER (1 << 7)
#define SDHCI_QUIRK_NO_EMMC_HS200	(1 << 8)
#define SDHCI_QUIRK_EMMC_1V8_POWER	(1 << 9)
#define SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN (1 << 10)

/*
 * Host SDMA buffer boundary. Valid values from 4K to 512K in powers of 2.
 */
#define SDHCI_DEFAULT_BOUNDARY_SIZE	(512 * 1024)
#define SDHCI_DEFAULT_BOUNDARY_ARG	(7)

/* ADMA packet descriptor */
typedef struct {
	u16     attributes;
	u16     length;
	u32     addr;
} SdhciAdma;

typedef struct {
	u16     attributes;
	u16     length;
	u32     addr;
	u32     addr_hi;
} SdhciAdma64;

#define SDHCI_MAX_PER_DESCRIPTOR 0x10000

/* ADMA descriptor attributes */
#define SDHCI_ADMA_VALID (1 << 0)
#define SDHCI_ADMA_END (1 << 1)
#define SDHCI_ADMA_INT (1 << 2)
#define SDHCI_ACT_NOP (0 << 4)
#define SDHCI_ACT_TRAN (2 << 4)
#define SDHCI_ACT_LINK (3 << 4)

typedef struct sdhci_host SdhciHost;

struct sdhci_host {
	MmcCtrlr mmc_ctrlr;
	char *name;
	void *ioaddr;

	int initialized;
	unsigned quirks;
	unsigned host_caps;
	unsigned version;
	unsigned clock; /* current, min and max interface clocks */
	unsigned clock_f_min;
	unsigned clock_f_max;
	unsigned clock_base; /* controller base clock */
	int removable;
	unsigned voltages;
	unsigned int        tuning_done;    /* Condition flag set when CMD19 succeeds */
	uint32_t timing;

	/*
	 * Dynamically allocated array of ADMA descriptors to use for data
	 * transfers
	 */
	SdhciAdma *adma_descs;
	SdhciAdma64 *adma64_descs;
	/* select 32bit or 64bit ADMA operations */
	unsigned dma64;

	/* Number of ADMA descriptors currently in the array. */
	int adma_desc_count;

	int (*attach)(SdhciHost *host);
	void (*set_control_reg)(SdhciHost *host);

	unsigned int (*set_clock)(unsigned int clock);
};

extern SdhciHost emmc_host;

static inline void sdhci_writel(SdhciHost *host, u32 val, int reg)
{
	//writel(val, host->ioaddr + reg);
    *(volatile unsigned int *)(host->ioaddr + reg) = val; 
}

static inline void sdhci_writew(SdhciHost *host, u16 val, int reg)
{
	//writew(val, host->ioaddr + reg);
    *(volatile unsigned short *)(host->ioaddr + reg) = val; 
}

static inline void sdhci_writeb(SdhciHost *host, u8 val, int reg)
{
	//writeb(val, host->ioaddr + reg);
    *(volatile unsigned char *)(host->ioaddr + reg) = val; 
}
static inline u32 sdhci_readl(SdhciHost *host, int reg)
{
	//return readl(host->ioaddr + reg);
	return *(volatile unsigned int *)(host->ioaddr + reg);
	
}

static inline u16 sdhci_readw(SdhciHost *host, int reg)
{
	//return readw(host->ioaddr + reg);
	return *(volatile unsigned short *)(host->ioaddr + reg);
}

static inline u8 sdhci_readb(SdhciHost *host, int reg)
{
	//return readb(host->ioaddr + reg);
	return *(volatile unsigned char *)(host->ioaddr + reg);
}

static inline SdhciHost *mmc_get_host(void)
{
    return &emmc_host;
}

void add_sdhci(SdhciHost *host);
void sdhci_reset(SdhciHost *host, u8 mask);

#endif