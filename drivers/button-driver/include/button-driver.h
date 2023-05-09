#ifndef __BUTTON_DRIVER_H
#define __BUTTON_DRIVER_H

/*
* Button driver
* Support short and long pushes
* 
*/

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t (*button_get_time_us_t)      ();
typedef bool     (*button_get_gpio_state_t)   (void* phy_gpio);
typedef void     (*button_callback_t)         ();

typedef struct {
    button_get_time_us_t     get_time_us;
    button_get_gpio_state_t  get_gpio_state;
    void*                    phy_gpio;
    button_callback_t        push_callback;
    button_callback_t        spush_callback;
    button_callback_t        lpush_callback;

    bool            is_gpio_active_high;
    uint32_t        spush_period_us;
    uint32_t        lpush_period_us;

    uint64_t timestamp;
    bool push;
    bool lpush;
    bool spush;
    bool latch_time;
    bool latch_long;
} button_ctx_t;

/**
 * @brief Call this function before using driver
 * 
 * @param ctx pointer to a user-created context structure
 * @param time pointer to a user-created [get system time in us] function
 * @param gpio pointer to a user-created [get gpio state] function. false = button pushed
 * @param phy_gpio user context for gpio state function (NULL if not used)
 */
bool button_init (button_ctx_t* ctx, button_get_time_us_t time, button_get_gpio_state_t gpio, void* phy_gpio);

/**
 * @brief Call this function regularly for checking pin state
 * 
 * @param ctx pointer to context
 */
void button_task (button_ctx_t* ctx);

bool button_set_custom_timings (button_ctx_t* ctx, uint32_t short_period, uint32_t long_period);

void button_set_push_cb  (button_ctx_t* ctx, button_callback_t cb);
void button_set_spush_cb (button_ctx_t* ctx, button_callback_t cb);
void button_set_lpush_cb (button_ctx_t* ctx, button_callback_t cb);

#define BUTTON_SPUSH_PERIOD_DEFAULT_US ( 100*1000)
#define BUTTON_LPUSH_PERIOD_DEFAULT_US (1500*1000)

#ifdef __cplusplus
}
#endif

#endif // #ifndef __BUTTON_DRIVER_H