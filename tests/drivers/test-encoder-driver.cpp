#include <gtest/gtest.h>
#include "stdbool.h"
#include "stdint.h"

#include "encoder-driver.h"

encoder_ctx_t ctx;

static const uint32_t custom_timing = 49284;
static uint32_t init_time = 76543;

static uint64_t time_us_test = 0;
static uint64_t get_time_us () {
    return time_us_test;
}

static bool gpioA_state = false;
static bool get_gpioA_state () {
    return gpioA_state;
}

static bool gpioB_state = false;
static bool get_gpioB_state () {
    return gpioB_state;
}

static bool test_cw_callback_called = false;
static void test_cw_callback () {
    test_cw_callback_called = true;
}

static bool test_ccw_callback_called = false;
static void test_ccw_callback () {
    test_ccw_callback_called = true;
}

TEST(encoder_init, try_invalid_input)
{
    bool ret = 1;
    ret = encoder_init (NULL, get_time_us, get_gpioA_state, 0, get_gpioB_state, 0, false);
    EXPECT_EQ(ret, 0);

    ret = 1;
    ret = encoder_init (&ctx, NULL, get_gpioA_state, 0, get_gpioB_state, 0, false);
    EXPECT_EQ(ret, 0);

    ret = 1;
    ret = encoder_init (&ctx, get_time_us, NULL, 0, get_gpioB_state, 0, false);
    EXPECT_EQ(ret, 0);

    ret = 1;
    ret = encoder_init (&ctx, get_time_us, get_gpioA_state, 0, NULL, 0, false);
    EXPECT_EQ(ret, 0);
}

TEST(encoder_set_cw_cb, set_cb)
{
    memset(&ctx, 0, sizeof(ctx));
    encoder_set_cw_cb(&ctx, test_cw_callback);
    EXPECT_EQ(ctx.turn_cw_callback, test_cw_callback);
}

TEST(encoder_set_ccw_cb, set_cb)
{
    memset(&ctx, 0, sizeof(ctx));
    encoder_set_ccw_cb(&ctx, test_ccw_callback);
    EXPECT_EQ(ctx.turn_ccw_callback, test_ccw_callback);
}

TEST(encoder_custom_timing, set_timing)
{
    memset(&ctx, 0, sizeof(ctx));
    encoder_custom_timing(&ctx, custom_timing);
    EXPECT_EQ(ctx.verify_period_us, custom_timing);
}

static void emulate_states_sequence (uint8_t* seq, uint32_t len, uint32_t delay) {
    for (uint32_t iter = 0; iter < len; iter++) {
        gpioA_state = !(seq[iter] / 2);
        gpioB_state = !(seq[iter] % 2);
        time_us_test = init_time + iter * delay;
        encoder_task(&ctx);
        time_us_test = init_time + (iter + 1) * delay - 1;
        encoder_task(&ctx);
    }
}

TEST(encoder_task, ccw)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    uint8_t states_ccw[] = {0, 1, 3, 2, 0, 1, 3, 2, 0, 1, 3, 2, 0, 1, 3, 2, 0};
    emulate_states_sequence (states_ccw, sizeof(states_ccw), custom_timing - 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 0);

    emulate_states_sequence (states_ccw, sizeof(states_ccw), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 4);

    emulate_states_sequence (states_ccw, sizeof(states_ccw), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 8);
}

TEST(encoder_task, cw)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    uint8_t states_cw[] = {0, 2, 3, 1, 0, 2, 3, 1, 0, 2, 3, 1, 0, 2, 3, 1, 0};
    emulate_states_sequence (states_cw, sizeof(states_cw), custom_timing - 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 0);

    emulate_states_sequence (states_cw, sizeof(states_cw), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 4);
    EXPECT_EQ (ctx.ccw_counter, 0);

    emulate_states_sequence (states_cw, sizeof(states_cw), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 8);
    EXPECT_EQ (ctx.ccw_counter, 0);
}

TEST(encoder_task, cw_ccw_both)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    uint8_t states[] = {0, 2, 3, 1, 0, 1, 3, 2, 0, 2, 3, 1, 0, 1, 3, 2, 0};
    emulate_states_sequence (states, sizeof(states), custom_timing - 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 0);

    emulate_states_sequence (states, sizeof(states), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 2);
    EXPECT_EQ (ctx.ccw_counter, 2);

    emulate_states_sequence (states, sizeof(states), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 4);
    EXPECT_EQ (ctx.ccw_counter, 4);
}

TEST(encoder_task, cw_cb)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    ctx.turn_cw_callback = test_cw_callback;
    ctx.turn_ccw_callback = test_ccw_callback;

    test_cw_callback_called = false;
    test_ccw_callback_called = false;

    uint8_t states[] = {0, 2, 3, 1, 0};
    emulate_states_sequence (states, sizeof(states), custom_timing - 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 0);
    EXPECT_EQ (test_cw_callback_called, false);
    EXPECT_EQ (test_ccw_callback_called, false);

    emulate_states_sequence (states, sizeof(states), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 1);
    EXPECT_EQ (ctx.ccw_counter, 0);
    EXPECT_EQ (test_cw_callback_called, true);
    EXPECT_EQ (test_ccw_callback_called, false);
}

TEST(encoder_task, ccw_cb)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    ctx.turn_cw_callback = test_cw_callback;
    ctx.turn_ccw_callback = test_ccw_callback;

    test_cw_callback_called = false;
    test_ccw_callback_called = false;

    uint8_t states[] = {0, 1, 3, 2, 0};
    emulate_states_sequence (states, sizeof(states), custom_timing - 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 0);
    EXPECT_EQ (test_cw_callback_called, false);
    EXPECT_EQ (test_ccw_callback_called, false);

    emulate_states_sequence (states, sizeof(states), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 1);
    EXPECT_EQ (test_cw_callback_called, false);
    EXPECT_EQ (test_ccw_callback_called, true);
}

TEST(encoder_task, random_state_false)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    uint8_t states[] = {3, 1, 2, 1, 3};

    emulate_states_sequence (states, sizeof(states), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 0);
}

TEST(encoder_task, random_state_true)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    uint8_t states[] = {0, 1, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 1, 0, 2, 0, 2, 0, 1};
    emulate_states_sequence (states, sizeof(states), custom_timing - 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 0);

    emulate_states_sequence (states, sizeof(states), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 4);
    EXPECT_EQ (ctx.ccw_counter, 5);
}

TEST(encoder_task, long_period)
{
    memset(&ctx, 0, sizeof(ctx));
    ctx.get_A_state = get_gpioA_state;
    ctx.get_B_state = get_gpioB_state;
    ctx.get_time_us = get_time_us;
    ctx.verify_period_us = custom_timing;

    uint8_t states[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0};

    emulate_states_sequence (states, sizeof(states), custom_timing + 1);
    EXPECT_EQ (ctx.cw_counter, 0);
    EXPECT_EQ (ctx.ccw_counter, 1);
}
