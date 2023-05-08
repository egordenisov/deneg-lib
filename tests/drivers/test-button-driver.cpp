#include <gtest/gtest.h>
#include "stdbool.h"
#include "stdint.h"

#include "button-driver.h"

static const uint64_t init_time = 67589729;
static const uint32_t short_min_time = 4500;
static const uint32_t long_min_time = short_min_time * 10;
static button_ctx ctx;

static uint64_t time_us_test = 0;
static uint64_t get_time_us () {
    return time_us_test;
}

static bool gpio_state = false;
static bool get_gpio_state () {
    return gpio_state;
}

static bool test_push_callback_called = false;
static void test_push_callback () {
    test_push_callback_called = true;
}

static bool test_spush_callback_called = false;
static void test_spush_callback () {
    test_spush_callback_called = true;
}

static bool test_lpush_callback_called = false;
static void test_lpush_callback () {
    test_lpush_callback_called = true;
}

static void run_button_task (uint32_t time, bool gpio, bool push, bool short_push, bool long_push) {
    time_us_test  = time;
    gpio_state = gpio;
    button_task(&ctx); // Less time than needed for a short push
    EXPECT_EQ(ctx.push,  push       );
    EXPECT_EQ(ctx.spush, short_push );
    EXPECT_EQ(ctx.lpush, long_push  );
}

TEST(button_init, set_init_values)
{
    time_us_test = init_time;

    ctx.push = 1;
    ctx.spush = 1;
    ctx.lpush = 1;
    ctx.latch_time = 1;

    bool ret = 0;
    ret = button_init (&ctx, get_time_us, get_gpio_state, 0);

    EXPECT_EQ(ret, 1);
    EXPECT_EQ(ctx.push, 0);
    EXPECT_EQ(ctx.latch_time, 0);
    EXPECT_EQ(ctx.lpush, 0);
    EXPECT_EQ(ctx.spush, 0);
}

TEST(button_init, try_invalid_ctx)
{
    bool ret = 1;
    ret = button_init (NULL, get_time_us, get_gpio_state, 0);
    EXPECT_EQ(ret, 0);

    ret = 1;
    ret = button_init (&ctx, NULL, get_gpio_state, 0);
    EXPECT_EQ(ret, 0);

    ret = 1;
    ret = button_init (&ctx, get_time_us, NULL, 0);
    EXPECT_EQ(ret, 0);
}

TEST(button_custom_timing, set_timing)
{
    memset(&ctx, 0, sizeof(ctx));
    button_custom_timing(&ctx, short_min_time, long_min_time);
    EXPECT_EQ(ctx.spush_period_us, short_min_time);
    EXPECT_EQ(ctx.lpush_period_us, long_min_time);
}

TEST(button_set_push_cb, set_push_cb)
{
    memset(&ctx, 0, sizeof(ctx));
    button_set_push_cb(&ctx, test_push_callback);
    EXPECT_EQ(ctx.push_callback, test_push_callback);
}

TEST(button_set_spush_cb, set_spush_cb)
{
    memset(&ctx, 0, sizeof(ctx));
    button_set_spush_cb(&ctx, test_spush_callback);
    EXPECT_EQ(ctx.spush_callback, test_spush_callback);
}

TEST(button_set_lpush_cb, set_lpush_cb)
{
    memset(&ctx, 0, sizeof(ctx));
    button_set_lpush_cb(&ctx, test_lpush_callback);
    EXPECT_EQ(ctx.lpush_callback, test_lpush_callback);
}

TEST(button_task, short_push)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.lpush_period_us = long_min_time;
    ctx.spush_period_us = short_min_time;
    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;
    ctx.push_callback = test_push_callback;
    ctx.spush_callback = test_spush_callback;
    ctx.lpush_callback = test_lpush_callback;

    // Init state
    run_button_task (init_time, false, 0, 0, 0);

    // Less time than needed for a short push
    run_button_task (init_time + short_min_time - 1, false, 0, 0, 0);

    // Releasing the button
    run_button_task (init_time + short_min_time - 1, true, 0, 0, 0);

    // Push again
    run_button_task (init_time + short_min_time, false, 0, 0, 0);

    // Enough time for a short push
    run_button_task (init_time + short_min_time*2 + 1, false, 0, 0, 0);

    // Releasing the button
    test_push_callback_called = false;
    test_spush_callback_called = false;
    test_lpush_callback_called = false;
    run_button_task (init_time + short_min_time*2 + 1, true, 1, 1, 0);
    EXPECT_EQ(test_push_callback_called, true);
    EXPECT_EQ(test_spush_callback_called, true);
    EXPECT_EQ(test_lpush_callback_called, false);
}

TEST(button_task, long_push)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.lpush_period_us = long_min_time;
    ctx.spush_period_us = short_min_time;
    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;
    ctx.push_callback = test_push_callback;
    ctx.spush_callback = test_spush_callback;
    ctx.lpush_callback = test_lpush_callback;

    // Init state
    run_button_task (init_time, false, 0, 0, 0);

    // Less time than needed for a short push
    run_button_task (init_time + long_min_time - 1, false, 0, 0, 0);

    // Releasing the button
    run_button_task (init_time + long_min_time - 1, true, 1, 1, 0);

    ctx.push = false;
    ctx.spush = false;
    ctx.lpush = false;

    // Push again
    run_button_task (init_time + long_min_time, false, 0, 0, 0);

    // Enough time for a short push
    test_push_callback_called = false;
    test_spush_callback_called = false;
    test_lpush_callback_called = false;
    run_button_task (init_time + long_min_time*2 + 1, false, 1, 0, 1);
    EXPECT_EQ(test_push_callback_called, true);
    EXPECT_EQ(test_spush_callback_called, false);
    EXPECT_EQ(test_lpush_callback_called, true);

    // Releasing the button
    run_button_task (init_time + long_min_time*2 + 1, true, 1, 0, 1);
}

TEST(button_task, short_push_repeat)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.lpush_period_us = long_min_time;
    ctx.spush_period_us = short_min_time;
    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;

    // Init state
    run_button_task (init_time, false, 0, 0, 0);

    // Short push
    run_button_task (init_time + short_min_time + 1, false, 0, 0, 0);

    // Release button
    run_button_task (init_time + short_min_time + 1, true, 1, 1, 0);

    ctx.push = 0;
    ctx.spush = 0;
    ctx.lpush = 0;

    // Reset state
    run_button_task (init_time + short_min_time * 2, false, 0, 0, 0);

    // Push again
    run_button_task (init_time + short_min_time * 3 + 1, false, 0, 0, 0);

    // Release button
    run_button_task (init_time + short_min_time * 3 + 1, true, 1, 1, 0);
}

TEST(button_task, long_push_repeat)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.lpush_period_us = long_min_time;
    ctx.spush_period_us = short_min_time;
    ctx.get_time_us = get_time_us;
    ctx.get_gpio_state = get_gpio_state;

    // Init state
    run_button_task (init_time, false, 0, 0, 0);

    // Short push
    run_button_task (init_time + long_min_time + 1, false, 1, 0, 1);

    // Release button
    run_button_task (init_time + long_min_time + 1, true, 1, 0, 1);

    ctx.push = 0;
    ctx.spush = 0;
    ctx.lpush = 0;

    // Reset state
    run_button_task (init_time + long_min_time * 2, false, 0, 0, 0);

    // Push again
    run_button_task (init_time + long_min_time * 3 + 1, false, 1, 0, 1);

    // Release button
    run_button_task (init_time + long_min_time * 3 + 1, true, 1, 0, 1);
}
