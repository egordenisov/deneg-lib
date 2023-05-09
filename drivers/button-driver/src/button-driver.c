#include <string.h>
#include "button-driver.h"

bool button_init (button_ctx_t* ctx, button_get_time_us_t time, button_get_gpio_state_t gpio, void* phy_gpio) {
    if (ctx == NULL) {
        return false;
    }
    if (!time || !gpio) {
        return false;
    }
    memset(ctx, 0, sizeof(button_ctx_t));

    ctx->is_gpio_active_high = false;
    ctx->get_gpio_state = gpio;
    ctx->get_time_us = time;
    ctx->phy_gpio = phy_gpio;

    ctx->spush_period_us = BUTTON_SPUSH_PERIOD_DEFAULT_US;
    ctx->lpush_period_us = BUTTON_LPUSH_PERIOD_DEFAULT_US;

    return true;
}

bool button_set_custom_timings (button_ctx_t* ctx, uint32_t short_period, uint32_t long_period) {
    if (long_period < short_period) {
        return false;
    }
    ctx->spush_period_us = short_period;
    ctx->lpush_period_us = long_period;
}

void button_set_push_cb  (button_ctx_t* ctx, button_callback_t cb) {
    ctx->push_callback = cb;
}

void button_set_spush_cb (button_ctx_t* ctx, button_callback_t cb) {
    ctx->spush_callback = cb;
}

void button_set_lpush_cb (button_ctx_t* ctx, button_callback_t cb) {
    ctx->lpush_callback = cb;
}

#define CALL_CALLBACK_IF_NOT_NULL(x) {if ((x) != NULL) x();}

void button_task (button_ctx_t* ctx) {
    uint64_t time_now = ctx->get_time_us();
    if (ctx->get_gpio_state(ctx->phy_gpio) == ctx->is_gpio_active_high) { // If pushed
        if (!ctx->latch_time) {
            ctx->timestamp = time_now;
            ctx->latch_time = true;
        }
        if (!ctx->latch_long && (time_now - ctx->timestamp >= ctx->lpush_period_us)) {
            ctx->latch_long = true;

            ctx->push = true;
            ctx->lpush = true;
            ctx->spush = false;
            CALL_CALLBACK_IF_NOT_NULL (ctx->push_callback);
            CALL_CALLBACK_IF_NOT_NULL (ctx->lpush_callback);
        }
    }
    else { // If no push
        if (ctx->latch_time && !ctx->latch_long && (time_now - ctx->timestamp >= ctx->spush_period_us)) {
            ctx->push = true;
            ctx->lpush = false;
            ctx->spush = true;
            CALL_CALLBACK_IF_NOT_NULL (ctx->push_callback);
            CALL_CALLBACK_IF_NOT_NULL (ctx->spush_callback);
        }
        ctx->latch_time = false;
        ctx->latch_long = false;
    }
}
