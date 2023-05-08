#ifndef __TMP112_DRIVER_H
#define __TMP112_DRIVER_H

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t (*i2c_write_t) (void* phy_i2c, uint8_t* data, uint32_t size);
typedef uint8_t (*i2c_read_t)  (void* phy_i2c, uint8_t* data, uint32_t size);

#define TMP112_MIN_TEMP     (-55)
#define TMP112_MAX_TEMP     (128)
#define TMP112_RESOLUTION   (12)

typedef struct TMP112_CTX {
    i2c_write_t  wr_cb;
    i2c_read_t   rd_cb;
    void*      phy_i2c;
} tmp112_ctx_t;


typedef enum TMP112_ERROR {
    TMP112_OK = 0,
    TMP112_UNKNOWN,
    TMP112_INVALID_INPUT,
    TMP112_I2C_WRITE_ERROR,
    TMP112_I2C_READ_ERROR,
} tmp112_error_t;


uint8_t tmp112_init (tmp112_ctx_t* ctx, i2c_write_t wr, i2c_read_t rd, void* phy_i2c);

uint8_t tmp112_get_celsius (tmp112_ctx_t* ctx, float* temp);

uint8_t tmp112_set_celsius_tlow_thigh  (tmp112_ctx_t* ctx, float tlow, float thigh);

#ifdef __cplusplus
}
#endif

#endif // __TMP112_DRIVER_H