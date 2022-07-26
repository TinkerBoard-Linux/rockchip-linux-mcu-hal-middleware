/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */
#ifndef RPMSG_CONFIG_H_
#define RPMSG_CONFIG_H_

#define RL_BUFFER_PAYLOAD_SIZE (496U)
#define RL_BUFFER_COUNT (64U)

#define RL_MAX_INSTANCE_NUM (12U)
#define RL_PLATFORM_HIGHEST_LINK_ID     (0xFFU)
#define RL_PLATFORM_IRQ_BASE            RPMSG_01_IRQn

/*
 * env_bm isr count
 * 4bit for master and 4bit for remote. each link_id has 2 virtqueue.
 * TODO: Modify here to reduce memory usage
 */
#define ISR_COUNT (0x1E0U)

#endif /* RPMSG_CONFIG_H_ */
