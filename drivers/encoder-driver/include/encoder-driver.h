#ifndef __ENCODER_DRIVER_H
#define __ENCODER_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/** \file encoder-driver.h
 *  
 * Driver for reading encoder rotations and calling callbacks
 *
 */

//------------------------------------------------------------------
// Includes
//------------------------------------------------------------------

// Standard Headers
#include <stdint.h>
#include <stdbool.h>

// Function typedefs
typedef uint64_t (*encoder_get_time_us)      ();
typedef bool     (*encoder_get_gpio_state)   ();
typedef void     (*encoder_callback)         ();

// Default minimum time to latch state in us
#define ENCODER_FIX_STATE_PERIOD_DEFAULT_US ( 50 )

typedef struct {
    //
    // You can read and reset fields cw_counter and ccw_counter
    //

    // Clockwise counter incremented each step
    uint32_t cw_counter;

    // Counterclockwise counter incremented each step
    uint32_t ccw_counter;

    //
    // Private. Don't change this fields
    //
    encoder_get_time_us     get_time_us;
    encoder_get_gpio_state  get_A_state;
    encoder_get_gpio_state  get_B_state;
    encoder_callback        turn_cw_callback;
    encoder_callback        turn_ccw_callback;

    bool            is_gpioA_active_high;
    bool            is_gpioB_active_high;
    bool            invert_rot;
    uint32_t        verify_period_us;

    uint64_t timestamp;
    uint8_t state;
    uint8_t prev_state;
    bool latch_state;

} encoder_ctx_t;

/**
 * @brief Initialise encoder driver function should be called to use the driver
 * 
 * 
 * @param ctx poiner to the context (a user-created structure of the encoder_ctx_t type)
 * @param time pointer to function for get system time in us
 * @param gpioA pointer to get state of A switch in encoder
 * @param is_A_active_high set true if gpioA returns active_high
 * @param gpioB pointer to get state of B switch in encoder
 * @param is_B_active_high set true if gpioB returns active_high
 * @param invert_rot set true to invert cw and ccw
 * @return true if init ok
 * @return false if input params invalid
 */
bool encoder_init (encoder_ctx_t* ctx, encoder_get_time_us time, encoder_get_gpio_state gpioA, bool is_A_active_high, encoder_get_gpio_state gpioB, bool is_B_active_high, bool invert_rot);

/**
 * @brief Task function should be called in main loop
 * 
 * @param ctx poiner to the context
 */
void encoder_task (encoder_ctx_t* ctx);

/**
 * @brief Set callback for clockwise rotation
 * 
 * @param ctx poiner to the context
 * @param cb pointer to a custom callback
 */
void encoder_set_cw_cb  (encoder_ctx_t* ctx, encoder_callback cb);

/**
 * @brief Set callback for counterclockwise rotation
 * 
 * @param ctx poiner to the context
 * @param cb pointer to a custom callback
 */
void encoder_set_ccw_cb (encoder_ctx_t* ctx, encoder_callback cb);

/**
 * @brief Set custom timing instead of default value
 * 
 * @param ctx poiner to the context
 * @param latch_us minimum time to latch state in us
 */
void encoder_custom_timing (encoder_ctx_t* ctx, uint32_t latch_us);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __ENCODER_DRIVER_H