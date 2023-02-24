#ifndef __DENEG_BUTTON_H
#define __DENEG_BUTTON_H

#include "stdint.h"
#include "stdbool.h"

typedef uint64_t (*deneg_get_time_us)    ();
typedef bool     (*deneg_get_gpio_state) ();

typedef struct
{
    // User area
    deneg_get_time_us       get_time_us;
    deneg_get_gpio_state    get_gpio_state;
    
    bool        gpio_active_high;
    uint32_t    short_push_time_us;
    uint32_t    long_push_time_us;

    // Private area
    uint64_t timestamp;
    bool push;
    bool long_push;
    bool short_push;
    bool latch;

} deneg_button_ctx;

bool deneg_button_init          (deneg_button_ctx* ctx);

bool deneg_button_is_push       (deneg_button_ctx* ctx);
bool deneg_button_is_short_push (deneg_button_ctx* ctx);
bool deneg_button_is_long_push  (deneg_button_ctx* ctx);

void deneg_button_task          (deneg_button_ctx* ctx);

#endif // #ifndef __DENEG_BUTTON_H