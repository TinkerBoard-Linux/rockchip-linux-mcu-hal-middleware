/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */
#include <stdio.h>
#include <string.h>

#include "rpmsg_platform.h"
#include "rpmsg_env.h"

#include "hal_base.h"

#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
#error "This RPMsg-Lite port requires RL_USE_ENVIRONMENT_CONTEXT set to 0"
#endif

static int32_t isr_counter = 0;
static int32_t disable_counter = 0;
static int32_t first_notify = 0;
static void *platform_lock;
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
static LOCK_STATIC_CONTEXT platform_lock_static_ctxt;
#endif

#ifdef RL_PLATFORM_USING_MBOX
/* master core uses RL_MBOX_A2B and remote core uses RL_MBOX_B2A */
#define RL_MBOX_B2A 0
#define RL_MBOX_A2B 1

static struct MBOX_REG *rl_pMBox = MBOX0;
static int32_t register_count = 0;

static void rpmsg_mbox_isr(void)
{
    HAL_MBOX_IrqHandler(MBOX_BB_IRQn, rl_pMBox);
}

static void rpmsg_remote_cb(struct MBOX_CMD_DAT *msg, void *args)
{
    uint32_t link_id;
    struct MBOX_CMD_DAT rx_msg = *msg;

    if (rx_msg.DATA != RL_RPMSG_MAGIC)
        printf("rpmsg remote: mailbox data error!\n");
    link_id = rx_msg.CMD & 0xFFU;

    if (first_notify == 0)
    {
        env_isr(RL_GET_VQ_ID(link_id, 0));
        first_notify++;
    } else {
        env_isr(RL_GET_VQ_ID(link_id, 1));
    }
}

static struct MBOX_CLIENT mbox_clr[MBOX_CHAN_CNT] =
{
    { "mbox-clr0", MBOX_BB_IRQn, rpmsg_remote_cb, (void *)MBOX_CH_0 },
    { "mbox-clr1", MBOX_BB_IRQn, rpmsg_remote_cb, (void *)MBOX_CH_1 },
    { "mbox-clr2", MBOX_BB_IRQn, rpmsg_remote_cb, (void *)MBOX_CH_2 },
    { "mbox-clr3", MBOX_BB_IRQn, rpmsg_remote_cb, (void *)MBOX_CH_3 },
};
#endif

static void platform_global_isr_disable(void)
{
}

static void platform_global_isr_enable(void)
{
}

int32_t platform_init_interrupt(uint32_t vector_id, void *isr_data)
{
#ifdef RL_PLATFORM_USING_MBOX
    uint32_t ch_id;
    int ret = 0;
    struct MBOX_CLIENT *mbox_cl[MBOX_CHAN_CNT];

    /* MCU use MBOX CH3 */
    ch_id = 3;
#endif

    /* Register ISR to environment layer */
    env_register_isr(vector_id, isr_data);

    env_lock_mutex(platform_lock);

    RL_ASSERT(0 <= isr_counter);
    if (isr_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
#ifdef RL_PLATFORM_USING_MBOX
        HAL_NVIC_SetIRQHandler(MBOX_BB_IRQn, rpmsg_mbox_isr);
        if (register_count % 2 == 0)
        {
            HAL_MBOX_Init(rl_pMBox, RL_MBOX_B2A);
            mbox_cl[ch_id] = &mbox_clr[ch_id];
            ret = HAL_MBOX_RegisterClient(rl_pMBox, ch_id, mbox_cl[ch_id]);
            if (ret) {
                printf("mbox remote client register failed, ret=%d\n", ret);
            }
        }
        register_count++;
#endif
    }
    isr_counter++;

    env_unlock_mutex(platform_lock);

    return 0;
}

int32_t platform_deinit_interrupt(uint32_t vector_id)
{
    env_lock_mutex(platform_lock);

    RL_ASSERT(0 < isr_counter);
    isr_counter--;
    if (isr_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
    }

    /* Unregister ISR from environment layer */
    env_unregister_isr(vector_id);

    env_unlock_mutex(platform_lock);

    return 0;
}

void platform_notify(uint32_t vector_id)
{
#ifdef RL_PLATFORM_USING_MBOX
    uint32_t ch_id;
    uint32_t link_id;
    struct MBOX_CMD_DAT tx_msg;

    link_id = RL_GET_LINK_ID(vector_id);
    tx_msg.CMD = link_id & 0xFFU;
    tx_msg.DATA = RL_RPMSG_MAGIC;

    /* MCU use MBOX CH3 */
    ch_id = 3;
#endif

    env_lock_mutex(platform_lock);

#ifdef RL_PLATFORM_USING_MBOX
    HAL_MBOX_SendMsg(rl_pMBox, ch_id, &tx_msg);
#endif

    env_unlock_mutex(platform_lock);
}

/**
 * platform_time_delay
 *
 * @param num_msec Delay time in ms.
 *
 */
void platform_time_delay(uint32_t num_msec)
{
    HAL_DelayMs(num_msec);
}

/**
 * platform_in_isr
 *
 * Return whether CPU is processing IRQ
 *
 * @return True for IRQ, false otherwise.
 *
 */
int32_t platform_in_isr(void)
{
    return 0;
}

/**
 * platform_interrupt_enable
 *
 * Enable peripheral-related interrupt
 *
 * @param vector_id Virtual vector ID that needs to be converted to IRQ number
 *
 * @return vector_id Return value is never checked.
 *
 */
int32_t platform_interrupt_enable(uint32_t vector_id)
{
    RL_ASSERT(0 < disable_counter);

    platform_global_isr_disable();
    disable_counter--;
    if (disable_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
#ifdef RL_PLATFORM_USING_MBOX
        HAL_NVIC_EnableIRQ(MBOX_BB_IRQn);
#endif
    }
    platform_global_isr_enable();
    return ((int32_t)vector_id);
}

/**
 * platform_interrupt_disable
 *
 * Disable peripheral-related interrupt.
 *
 * @param vector_id Virtual vector ID that needs to be converted to IRQ number
 *
 * @return vector_id Return value is never checked.
 *
 */
int32_t platform_interrupt_disable(uint32_t vector_id)
{
    RL_ASSERT(0 <= disable_counter);

    platform_global_isr_disable();
    if (disable_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
#ifdef RL_PLATFORM_USING_MBOX
        HAL_NVIC_DisableIRQ(MBOX_BB_IRQn);
#endif
    }
    disable_counter++;
    platform_global_isr_enable();
    return ((int32_t)vector_id);
}

/**
 * platform_map_mem_region
 *
 * Dummy implementation
 *
 */
void platform_map_mem_region(uint32_t vrt_addr, uint32_t phy_addr, uint32_t size, uint32_t flags)
{
}

/**
 * platform_cache_all_flush_invalidate
 *
 * Dummy implementation
 *
 */
void platform_cache_all_flush_invalidate(void)
{
}

/**
 * platform_cache_disable
 *
 * Dummy implementation
 *
 */
void platform_cache_disable(void)
{
}

/**
 * platform_vatopa
 *
 * Dummy implementation
 *
 */
uint32_t platform_vatopa(void *addr)
{
    return ((uint32_t)(char *)addr);
}

/**
 * platform_patova
 *
 * Dummy implementation
 *
 */
void *platform_patova(uint32_t addr)
{
    return ((void *)(char *)addr);
}

/**
 * platform_init
 *
 * platform/environment init
 */
int32_t platform_init(void)
{
    /* Create lock used in multi-instanced RPMsg */
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    if (0 != env_create_mutex(&platform_lock, 1, &platform_lock_static_ctxt))
#else
    if (0 != env_create_mutex(&platform_lock, 1))
#endif
    {
        return -1;
    }

    return 0;
}

/**
 * platform_deinit
 *
 * platform/environment deinit process
 */
int32_t platform_deinit(void)
{
    /* Delete lock used in multi-instanced RPMsg */
    env_delete_mutex(platform_lock);
    platform_lock = ((void *)0);
    return 0;
}
