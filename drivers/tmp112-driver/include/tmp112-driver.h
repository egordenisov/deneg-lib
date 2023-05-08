#ifndef __TMP112_DRIVER_H
#define __TMP112_DRIVER_H

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t (i2c_write) (void* phy_i2c, uint8_t* data, uint32_t size);
typedef uint8_t (i2c_read)  (void* phy_i2c, uint8_t* data, uint32_t size);

typedef struct TMP112_CTX {
    i2c_write*  cb_wr;
    i2c_read*   cb_rd;
} tmp112_ctx_t;


typedef enum TMP112_ERROR {
    TMP112_OK = 0,
    TMP112_INVALID_INPUT,
} tmp112_error_t;


uint8_t tmp112_init (tmp112_ctx_t ctx, i2c_write wr, i2c_read rd, void* phy_i2c);

uint8_t tmp112_get_celsius (tmp112_ctx_t ctx, int16_t temp);

uint8_t tmp112_set_celsius_tlow_thigh  (tmp112_ctx_t ctx, int16_t tlow, int16_t thigh);

#ifdef __cplusplus
}
#endif

#endif // __TMP112_DRIVER_H