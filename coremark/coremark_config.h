/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */
#ifndef COREMARK_CONFIG_H_
#define COREMARK_CONFIG_H_

#define COMPILER_FLAGS "-mcpu=cortex-a55"

#define ITERATIONS 0

#define MAIN_HAS_NOARGC 1
#define MAIN_HAS_NORETURN 1

/* rockchip platform definition */
typedef unsigned int size_t;

#define CLOCKS_PER_SEC 1000

#endif /* COREMARK_CONFIG_H_ */
