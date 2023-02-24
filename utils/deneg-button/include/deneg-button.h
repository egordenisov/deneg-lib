#ifndef __DENEG_BUTTON_H
#define __DENEG_BUTTON_H

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t (*button_get_time_us)      ();
typedef bool     (*button_get_gpio_state)   ();
typedef void     (*button_callback)         ();

typedef struct {
    button_get_time_us     get_time_us;
    button_get_gpio_state  get_gpio_state;
    button_callback        push_callback;
    button_callback        spush_callback;
    button_callback        lpush_callback;

    bool            is_gpio_active_high;
    uint32_t        spush_period_us;
    uint32_t        lpush_period_us;

    uint64_t timestamp;
    bool push;
    bool lpush;
    bool spush;
    bool latch_time;
    bool latch_long;
} button_ctx;

bool button_init (button_ctx* ctx, button_get_time_us time, button_get_gpio_state gpio, bool active_high);

bool button_custom_timing (button_ctx* ctx, uint32_t short_period, uint32_t long_period);

void button_set_push_cb  (button_ctx* ctx, button_callback cb);
void button_set_spush_cb (button_ctx* ctx, button_callback cb);
void button_set_lpush_cb (button_ctx* ctx, button_callback cb);

void button_task         (button_ctx* ctx);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __DENEG_BUTTON_H