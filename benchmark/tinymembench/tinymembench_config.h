/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */
#ifndef TINYMEMBENCH_CONFIG_H_
#define TINYMEMBENCH_CONFIG_H_

#define TINYMEMBENCH_SIZE (4 * 1024 * 1024)

#define ALIGN_PADDING 0x100000

/*
 * 32 byte cacheline size:
 * 1.Rockchip MCU Subsystem ARM Cortex-M0
 *
 * 64 byte cacheline size:
 * 1.ARM Cortex-A55
 */
#define CACHE_LINE_SIZE 64

#define TINYMEMBENCH_CLOCKS_PER_SEC 1000

#endif /* TINYMEMBENCH_CONFIG_H_ */
