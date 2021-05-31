/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2021 Aidan MacDonald
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

#include "pwm-x1000.h"
#include "clk-x1000.h"
#include "gpio-x1000.h"
#include "system.h"
#include "kernel.h"
#include "x1000/tcu.h"

struct pwm_gpio_data {
    int port;
    unsigned pin;
    int func;
};

struct pwm_state {
    struct pwm_gpio_data gpio;
    int period_ns;
    int duty_ns;
    int full_ticks;
    int half_ticks;
    int prescaler;
};

static struct pwm_state pwm_state[] = {
    {{GPIO_C, 1 << 25, GPIO_DEVICE(0)}, -1, -1, -1, -1, -1},
    {{GPIO_C, 1 << 26, GPIO_DEVICE(1)}, -1, -1, -1, -1, -1},
    {{GPIO_C, 1 << 27, GPIO_DEVICE(1)}, -1, -1, -1, -1, -1},
    {{GPIO_B, 1 <<  6, GPIO_DEVICE(2)}, -1, -1, -1, -1, -1},
    {{GPIO_C, 1 << 24, GPIO_DEVICE(0)}, -1, -1, -1, -1, -1},
};

void pwm_init(int chn)
{
    /* clear cached state */
    struct pwm_state* st = &pwm_state[chn];
    st->period_ns = -1;
    st->duty_ns = -1;
    st->full_ticks = -1;
    st->prescaler = -1;
    st->prescaler = -1;

    /* clear GPIO and disable timer */
    gpio_config(st->gpio.port, st->gpio.pin, GPIO_OUTPUT(0));
    jz_clr(TCU_STOP, 1 << chn);
    jz_clr(TCU_ENABLE, 1 << chn);
    jz_set(TCU_STOP, 1 << chn);
}

void pwm_set_period(int chn, int period_ns, int duty_ns)
{
    struct pwm_state* st = &pwm_state[chn];
    unsigned long long tmp;
    int full_ticks = st->full_ticks;
    int half_ticks = st->half_ticks;
    int prescaler = st->prescaler;

    if(period_ns != st->period_ns) {
        /* calculate full tick period and prescaler */
        tmp = clk_get(X1000_CLK_PCLK) / 1000000;
        tmp *= period_ns;
        tmp /= 1000;

        prescaler = 0;
        while(tmp > 0xffff && prescaler < 5) {
            tmp /= 4;
            prescaler += 1;
        }

        full_ticks = (tmp > 0xffff) ? 0xffff : tmp;
        st->period_ns = period_ns;
    }

    if(duty_ns != st->duty_ns) {
        /* calculate half tick value */
        tmp = full_ticks;
        tmp *= duty_ns;
        tmp /= period_ns;

        half_ticks = (tmp > 0xffff) ? 0xffff : tmp;
        if(half_ticks >= full_ticks)
            half_ticks = full_ticks - 1;
        st->duty_ns = duty_ns;
    }

    /* need to clear STOP bit to access timer unit registers */
    int was_stopped = !!(jz_read(TCU_STOP) & (1 << chn));
    if(was_stopped)
        jz_clr(TCU_STOP, 1 << chn);

    /* check if timer is currently running */
    int was_enabled = !!(jz_read(TCU_ENABLE) & (1 << chn));
    int enabled = was_enabled;

    if(prescaler != st->prescaler) {
        /* must disable timer to change these settings */
        if(was_enabled) {
            jz_clr(TCU_ENABLE, 1 << chn);
            enabled = 0;
        }

        jz_overwritef(TCU_CTRL(chn), SHUTDOWN_V(GRACEFUL), INIT_LVL(0),
                      PWM_EN(1), PRESCALE(prescaler), SOURCE_V(PCLK));
        REG_TCU_COUNT(chn) = 0;
        st->prescaler = prescaler;
    }

    if(full_ticks != st->full_ticks || half_ticks != st->half_ticks) {
        if(enabled) {
            /* avoid changing PWM settings in the middle of a cycle */
            unsigned cmp = REG_TCU_CMP_FULL(chn) - 1;
            long deadline = current_tick + 3;
            while(REG_TCU_COUNT(chn) < cmp
               && TIME_BEFORE(current_tick, deadline));
        }

        /* these can be changed while the timer is running */
        REG_TCU_CMP_FULL(chn) = full_ticks;
        REG_TCU_CMP_HALF(chn) = full_ticks - half_ticks;
        st->full_ticks = full_ticks;
        st->half_ticks = half_ticks;
    }

    /* restore the enable/stop state */
    if(was_enabled && !enabled)
        jz_set(TCU_ENABLE, 1 << chn);
    if(was_stopped)
        jz_set(TCU_STOP, 1 << chn);
}

void pwm_enable(int chn)
{
    /* Start timer */
    jz_clr(TCU_STOP, 1 << chn);
    jz_set(TCU_ENABLE, 1 << chn);

    /* Configure GPIO function */
    struct pwm_state* st = &pwm_state[chn];
    gpio_config(st->gpio.port, st->gpio.pin, st->gpio.func);
}

void pwm_disable(int chn)
{
    /* Set GPIO to output 0 */
    struct pwm_state* st = &pwm_state[chn];
    gpio_config(st->gpio.port, st->gpio.pin, GPIO_OUTPUT(0));

    /* Stop timer */
    jz_clr(TCU_ENABLE, 1 << chn);
    jz_set(TCU_STOP, 1 << chn);
}
