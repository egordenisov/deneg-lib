#ifndef __TMP112_DRIVER_H
#define __TMP112_DRIVER_H

/*
* TMP112 temperature sensor driver
* supports only basic functions
*/

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t (*tmp112_i2c_write_t) (void* phy_i2c, uint8_t* data, uint32_t size);
typedef uint8_t (*tmp112_i2c_read_t)  (void* phy_i2c, uint8_t* data, uint32_t size);

#define TMP112_MIN_TEMP     (-55)
#define TMP112_MAX_TEMP     (128)
#define TMP112_RESOLUTION   (12)

typedef struct TMP112_CTX {
    tmp112_i2c_write_t  wr_cb;
    tmp112_i2c_read_t   rd_cb;
    void*               phy_i2c;
    
    bool                verify_write;
} tmp112_ctx_t;


typedef enum TMP112_ERROR {
    TMP112_OK = 0,
    TMP112_ERROR_UNKNOWN,
    TMP112_ERROR_INVALID_INPUT,
    TMP112_ERROR_I2C_WRITE_ERROR,
    TMP112_ERROR_I2C_READ_ERROR,
    TMP112_ERROR_WRITE_VERIFICATION,
} tmp112_error_t;

typedef enum {
    TMP112_FAUTLT_QUEUE_1 = 0x00, // Default
    TMP112_FAUTLT_QUEUE_2 = 0x01,
    TMP112_FAUTLT_QUEUE_4 = 0x02,
    TMP112_FAUTLT_QUEUE_6 = 0x03,
} tmp112_fault_queue_t;

typedef enum {
    TMP112_CONV_RATE_025Hz       = 0x00,
    TMP112_CONV_RATE_1Hz         = 0x01,
    TMP112_CONV_RATE_4Hz         = 0x02, // Default
    TMP112_CONV_RATE_8Hz         = 0x03,
} tmp112_conv_rate_t;

/**
 * @brief Call this function before using driver
 * 
 * @param ctx pointer to a user-created context structure
 * @param wr pointer to a user-created i2c write function
 * @param rd pointer to a user-created i2c read function
 * @param phy_i2c user context for i2c functions (NULL if not used)
 * @return tmp112_error_t enum
 */
tmp112_error_t tmp112_init (tmp112_ctx_t* ctx, tmp112_i2c_write_t wr, tmp112_i2c_read_t rd, void* phy_i2c);

/**
 * @brief TMP112 temperature reading
 * 
 * @param ctx pointer to context
 * @param temp temperature returns in degree Celsius
 * @return tmp112_error_t enum
 */
tmp112_error_t tmp112_get_celsius (tmp112_ctx_t* ctx, float* temp);

/**
 * @brief Setting threshold temperatures t low and t high. low = +75 and high = +80 - default
 * 
 * @param ctx pointer to context
 * @param tlow t low in Celsius (from -55 to 128)
 * @param thigh t high in Celsius (from -55 to 128)
 * @return tmp112_error_t enum
 */
tmp112_error_t tmp112_set_celsius_tlow_thigh (tmp112_ctx_t* ctx, float tlow, float thigh);

/**
 * @brief Setting convertion rate (0.25, 1, 4 or 8 Hz). 4 Hz - default
 * 
 * @param ctx pointer to context
 * @param cr tmp112_conv_rate_t enum
 * @return tmp112_error_t enum
 */
tmp112_error_t tmp112_set_convertion_rate (tmp112_ctx_t* ctx, tmp112_conv_rate_t cr);

/**
 * @brief Setting fault queue len (1, 2, 4 or 6). 1 - default
 * 
 * @param ctx pointer to context
 * @param fq tmp112_fault_queue_t enum
 * @return tmp112_error_t enum
 */
tmp112_error_t tmp112_set_fault_queue (tmp112_ctx_t* ctx, tmp112_fault_queue_t fq);

#ifdef __cplusplus
}
#endif

#endif // __TMP112_DRIVER_H