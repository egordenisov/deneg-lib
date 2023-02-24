#include <stdio.h>
#include "../include/deneg-button.h"

bool deneg_button_init (deneg_button_ctx* ctx) {
    if (ctx == NULL) {
        return false;
    }
    if (ctx->get_gpio_state == NULL) {
        return false;
    }
    if (ctx->get_time_us == NULL) {
        return false;
    }

    ctx->push = false;
    ctx->latch = false;
    ctx->long_push = false;
    ctx->short_push = false;

    ctx->timestamp = 0;

    return true;
}

bool deneg_button_is_push (deneg_button_ctx* ctx) {
    bool buf = ctx->push;
    ctx->push = false;
    ctx->long_push = false;
    ctx->short_push = false;

    return buf;
}

bool deneg_button_is_short_push (deneg_button_ctx* ctx) {
    bool buf = ctx->short_push;
    ctx->push = false;
    ctx->long_push = false;
    ctx->short_push = false;

    return buf;
}

bool deneg_button_is_long_push (deneg_button_ctx* ctx) {
    bool buf = ctx->long_push;
    ctx->push = false;
    ctx->long_push = false;
    ctx->short_push = false;

    return buf;
}

void deneg_button_task (deneg_button_ctx* ctx) {
    uint64_t time_now = ctx->get_time_us();

    if (ctx->get_gpio_state() == ctx->gpio_active_high) { // If pushed
        if (!ctx->latch) {
            ctx->timestamp = time_now;
            ctx->latch = true;
        }

        if (time_now - ctx->timestamp > ctx->short_push_time_us) {
            ctx->push = true;

            if (time_now - ctx->timestamp > ctx->long_push_time_us) {
                ctx->short_push = false;
                ctx->long_push = true;
            }
            else {
                ctx->short_push = true;
                ctx->long_push = false;
            }
        }
    }
    else { // If no push
        ctx->latch = false;
    }
}
