#include "tmp112-driver.h"
#include "tmp112-regs.h"


uint8_t tmp112_init (tmp112_ctx_t ctx, i2c_write wr, i2c_read rd, void* phy_i2c) {
    ctx->
    if (ctx == NULL) {
        return TMP112_INVALID_INPUT;
    }
    if (wr == NULL) {
        return TMP112_INVALID_INPUT;
    }
    if (rd == NULL) {
        return TMP112_INVALID_INPUT;
    }
    
}

uint8_t tmp112_get_celsius (tmp112_ctx_t ctx, int16_t temp) {

}

uint8_t tmp112_set_celsius_tlow_thigh  (tmp112_ctx_t ctx, int16_t tlow, int16_t thigh) {

}