# SPDX-License-Identifier: BSD-3-Clause */

# Copyright (c) 2022 Rockchip Electronics Co., Ltd.

COREMARK_PATH=$(HAL_PATH)/middleware/coremark

INCLUDES += \
-I"$(COREMARK_PATH)" \
-I"$(COREMARK_PATH)/barebones" \

SRC_DIRS += \
    $(COREMARK_PATH) \
    $(COREMARK_PATH)/barebones \
