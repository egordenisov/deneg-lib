#include "encoder-driver.h"
#include "string.h"

bool encoder_init (encoder_ctx_t* ctx, encoder_get_time_us time, encoder_get_gpio_state gpioA, bool is_A_active_high, encoder_get_gpio_state gpioB, bool is_B_active_high, bool invert_rot) {
    if (ctx == NULL) {
        return false;
    }

    if (!time || !gpioA || !gpioB) {
        return false;
    }

    memset(ctx, 0, sizeof(encoder_ctx_t));

    ctx->invert_rot           = invert_rot;
    ctx->is_gpioA_active_high = is_A_active_high;
    ctx->is_gpioB_active_high = is_B_active_high;

    ctx->get_time_us = time;
    ctx->get_A_state = gpioA;
    ctx->get_B_state = gpioB;

    ctx->verify_period_us = ENCODER_FIX_STATE_PERIOD_DEFAULT_US;
    ctx->state = 0;

    return true;
}

void encoder_set_cw_cb  (encoder_ctx_t* ctx, encoder_callback cb) {
    ctx->turn_cw_callback = cb;
}

void encoder_set_ccw_cb (encoder_ctx_t* ctx, encoder_callback cb) {
    ctx->turn_ccw_callback = cb;
}

void encoder_custom_timing (encoder_ctx_t* ctx, uint32_t latch_us) {
    ctx->verify_period_us = latch_us;
}

#define ENCODER_GET_STATE(a,b)  ((a)*2+(b))
#define ENCODER_CALL_CALLBACK_IF_EXISTS(x) {if ((x) != NULL) x();}

void encoder_task (encoder_ctx_t* ctx) {
    uint64_t time_now = ctx->get_time_us();
    bool levA = ctx->get_A_state() ^ !ctx->is_gpioA_active_high;
    bool levB = ctx->get_B_state() ^ !ctx->is_gpioB_active_high;

    if (ENCODER_GET_STATE(levA, levB) == ctx->state) {
        if (!ctx->latch_state && (time_now - ctx->timestamp >= ctx->verify_period_us)) {
            ctx->latch_state = true;

            if ((ctx->prev_state == 0) && ((ctx->state == 1) || (ctx->state == 2))) {
                if ((ctx->state == 2) ^ ctx->invert_rot) {
                    ctx->cw_counter++;
                    ENCODER_CALL_CALLBACK_IF_EXISTS(ctx->turn_cw_callback);
                }
                else {
                    ctx->ccw_counter++;
                    ENCODER_CALL_CALLBACK_IF_EXISTS(ctx->turn_ccw_callback);
                }
            }
            ctx->prev_state = ctx->state;
        }
    }
    else {
        ctx->state = ENCODER_GET_STATE(levA, levB);
        ctx->latch_state = false;
        ctx->timestamp = time_now;
    }
}
