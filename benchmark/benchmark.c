/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"
#include "benchmark.h"

#ifdef HAL_BENCHMARK_COREMARK
extern void coremark_main(void);
#endif

void benchmark_main(void)
{
    printf("benchmark start:\n");

#ifdef HAL_BENCHMARK_COREMARK
    printf("coremark start:\n");
    coremark_main();
    printf("coremark end.\n");
#endif

    printf("benchmark end.\n");
}
