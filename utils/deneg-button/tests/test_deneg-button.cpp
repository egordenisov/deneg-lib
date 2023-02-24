#include <gtest/gtest.h>
#include "stdbool.h"
#include "stdint.h"

extern "C" {
    #include "../include/deneg-button.h"
}

static const uint64_t init_time = 67589729;
static const uint32_t short_min_time = 4500;
static const uint32_t long_min_time = short_min_time * 10;
static deneg_button_ctx ctx;

static uint64_t time_us_test = 0;
static uint64_t get_time_us () {
    return time_us_test;
}

static bool gpio_state = false;
static bool get_gpio_state () {
    return gpio_state;
}

TEST(deneg_button_init, set_init_values)
{
    time_us_test = init_time;

    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;
    ctx.push = 1;
    ctx.short_push = 1;
    ctx.long_push = 1;
    ctx.latch = 1;

    bool ret = 0;
    ret = deneg_button_init (&ctx);

    EXPECT_EQ(ret, 1);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.latch, 0);
    EXPECT_EQ(ctx.long_push, 0);
    EXPECT_EQ(ctx.short_push, 0);
}

TEST(deneg_button_init, try_invalid_ctx)
{
    bool ret = 1;
    ret = deneg_button_init (NULL);
    EXPECT_EQ(ret, 0);

    ctx.get_gpio_state = NULL;
    ctx.get_time_us = get_time_us;
    ret = 1;

    ret = deneg_button_init (&ctx);
    EXPECT_EQ(ret, 0);

    ctx.get_gpio_state = get_gpio_state;
    ctx.get_time_us = NULL;
    ret = 1;

    ret = deneg_button_init (&ctx);
    EXPECT_EQ(ret, 0);
}

TEST(deneg_button_task, short_push)
{
    ctx.push = 0;
    ctx.latch = 0;
    ctx.long_push = 0;
    ctx.short_push = 0;
    ctx.long_push_time_us = long_min_time;
    ctx.short_push_time_us = short_min_time;
    ctx.gpio_active_high = false;

    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;
    ctx.timestamp = init_time;
    time_us_test  = init_time;
    gpio_state = 0;
    deneg_button_task (&ctx);
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + short_min_time - 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Less time than needed for a short push
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + short_min_time + 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Enough time for a short push
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 1);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + short_min_time * 2;
    gpio_state = 1;
    deneg_button_task (&ctx); // Releasing the button
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 1);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + short_min_time * 5;
    gpio_state = 1;
    deneg_button_task (&ctx); // After releasing the button
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 1);
    EXPECT_EQ(ctx.long_push,  0);
}

TEST(deneg_button_task, long_push)
{
    ctx.push = 0;
    ctx.latch = 0;
    ctx.long_push = 0;
    ctx.short_push = 0;
    ctx.long_push_time_us = long_min_time;
    ctx.short_push_time_us = short_min_time;
    ctx.gpio_active_high = false;

    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;
    ctx.timestamp = init_time;
    time_us_test  = init_time;
    gpio_state = 0;
    deneg_button_task (&ctx);
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + long_min_time - 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Less time than needed for a long push
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 1);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + long_min_time + 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Enough time for a long push
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  1);

    time_us_test  = init_time + long_min_time * 2;
    gpio_state = 1;
    deneg_button_task (&ctx); // Releasing the button
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  1);

    time_us_test  = init_time + long_min_time * 5;
    gpio_state = 1;
    deneg_button_task (&ctx); // After releasing the button
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  1);
}

TEST(deneg_button_task, short_push_repeat)
{
    ctx.push = 0;
    ctx.latch = 0;
    ctx.long_push = 0;
    ctx.short_push = 0;
    ctx.long_push_time_us = long_min_time;
    ctx.short_push_time_us = short_min_time;
    ctx.gpio_active_high = false;

    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;
    ctx.timestamp = init_time;
    time_us_test  = init_time;
    gpio_state = 0;
    deneg_button_task (&ctx);
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + short_min_time + 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Short push
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 1);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + short_min_time + 1;
    gpio_state = 1;
    deneg_button_task (&ctx); // Release button
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 1);
    EXPECT_EQ(ctx.long_push,  0);

    ctx.push = 0;
    ctx.short_push = 0;
    ctx.long_push = 0;
    time_us_test = init_time + short_min_time * 2;
    gpio_state = 1;
    deneg_button_task (&ctx); // Reset state
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test = init_time + short_min_time * 2;
    gpio_state = 0;
    deneg_button_task (&ctx); // Push
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + short_min_time * 3 + 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Short push
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 1);
    EXPECT_EQ(ctx.long_push,  0);
}

TEST(deneg_button_task, long_push_repeat)
{
    ctx.push = 0;
    ctx.latch = 0;
    ctx.long_push = 0;
    ctx.short_push = 0;
    ctx.long_push_time_us = long_min_time;
    ctx.short_push_time_us = short_min_time;
    ctx.gpio_active_high = false;

    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;
    ctx.timestamp = init_time;
    time_us_test  = init_time;
    gpio_state = 0;
    deneg_button_task (&ctx);
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + long_min_time + 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Long push
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  1);

    time_us_test  = init_time + long_min_time + 1;
    gpio_state = 1;
    deneg_button_task (&ctx); // Release button
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  1);

    ctx.push = 0;
    ctx.short_push = 0;
    ctx.long_push = 0;
    time_us_test = init_time + long_min_time * 2;
    gpio_state = 1;
    deneg_button_task (&ctx); // Reset state
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test = init_time + long_min_time * 2;
    gpio_state = 0;
    deneg_button_task (&ctx); // Push
    EXPECT_EQ(ctx.push,       0);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  0);

    time_us_test  = init_time + long_min_time * 3 + 1;
    gpio_state = 0;
    deneg_button_task (&ctx); // Long push
    EXPECT_EQ(ctx.push,       1);
    EXPECT_EQ(ctx.short_push, 0);
    EXPECT_EQ(ctx.long_push,  1);
}

TEST(deneg_button_is_push, is_push_test)
{
    ctx.push = 1;
    ctx.short_push = 1;
    ctx.long_push = 1;
    bool ret = 0;

    ret = deneg_button_is_push(&ctx);
    EXPECT_EQ(ret, 1);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.long_push, 0);
    EXPECT_EQ(ctx.short_push, 0);

    ret = deneg_button_is_push(&ctx);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.long_push, 0);
    EXPECT_EQ(ctx.short_push, 0);

    ctx.push = 1;
    ret = deneg_button_is_push(&ctx);
    EXPECT_EQ(ret, 1);
}

TEST(deneg_button_is_short_push, is_push_test)
{
    ctx.push = 1;
    ctx.short_push = 1;
    ctx.long_push = 1;
    bool ret = 0;

    ret = deneg_button_is_short_push(&ctx);
    EXPECT_EQ(ret, 1);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.long_push, 0);
    EXPECT_EQ(ctx.short_push, 0);

    ret = deneg_button_is_short_push(&ctx);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.long_push, 0);
    EXPECT_EQ(ctx.short_push, 0);

    ctx.short_push = 1;
    ret = deneg_button_is_short_push(&ctx);
    EXPECT_EQ(ret, 1);
}

TEST(deneg_button_is_long_push, is_push_test)
{
    ctx.push = 1;
    ctx.short_push = 1;
    ctx.long_push = 1;
    bool ret = 0;

    ret = deneg_button_is_long_push(&ctx);
    EXPECT_EQ(ret, 1);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.long_push, 0);
    EXPECT_EQ(ctx.short_push, 0);

    ret = deneg_button_is_long_push(&ctx);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.long_push, 0);
    EXPECT_EQ(ctx.short_push, 0);

    ctx.long_push = 1;
    ret = deneg_button_is_long_push(&ctx);
    EXPECT_EQ(ret, 1);
}