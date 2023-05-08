#include "tmp112-driver.h"
#include "tmp112-regs.h"

static inline uint8_t tmp112_write_byte (tmp112_ctx_t* ctx, uint8_t data) {
    return ctx->wr_cb(ctx->phy_i2c, &data, sizeof(uint8_t));
}

static float tmp112_bytes_to_float (uint8_t b1, uint8_t b2) {
    uint16_t raw = (b1 * 256 + b2) >> 4; // 4 bits are always zero 
    if (raw & (1 << 11)) {
        return -(float)((~raw + 1) & TMP112_UNSIG_RESOLUTION_MASK) / TMP112_LSB_PER_CELSIUS;
    } else {
        return ((float)raw) / TMP112_LSB_PER_CELSIUS;
    }
}

static void tmp112_float_to_bytes (float f, uint8_t* bytes) {
    if ((f > TMP112_MAX_TEMP) || (f < TMP112_MIN_TEMP)) {
        return;
    }
    uint16_t raw = 0;
    if (f >= 0) {
        raw = ((uint16_t)(f * TMP112_LSB_PER_CELSIUS)) & TMP112_UNSIG_RESOLUTION_MASK;
    } else {
        raw = (~((uint16_t)(-f * TMP112_LSB_PER_CELSIUS) - 1) & TMP112_UNSIG_RESOLUTION_MASK);
        raw |= (1 << TMP112_UNSIG_RESOLUTION);
    }
    raw = raw << 4;
    bytes[0] = raw >> 8;
    bytes[1] = raw & 0xFF;

    return;
}

static uint8_t tmp112_write_2byte_reg (tmp112_ctx_t* ctx, uint8_t addr, uint8_t* write_data) {
    uint8_t buf[3] = {0};
    uint8_t rc = TMP112_ERROR_UNKNOWN;

    buf[0] = addr;
    buf[1] = write_data[0];
    buf[2] = write_data[1];

    rc = ctx->wr_cb(ctx, buf, sizeof(buf));
    if (rc != 0) {
        return TMP112_ERROR_I2C_WRITE_ERROR;
    }
    return TMP112_OK;
}

static uint8_t tmp112_read_2byte_reg (tmp112_ctx_t* ctx, uint8_t addr, uint8_t* read_data) {
    uint8_t rc = TMP112_ERROR_UNKNOWN;
    rc = tmp112_write_byte(ctx, addr);
    if (rc != 0) {
        return TMP112_ERROR_I2C_WRITE_ERROR;
    }
    
    rc = ctx->rd_cb(ctx, read_data, sizeof(int16_t));
    if (rc != 0) {
        return TMP112_ERROR_I2C_READ_ERROR;
    }
    return TMP112_OK;
}

uint8_t tmp112_init (tmp112_ctx_t* ctx, tmp112_i2c_write_t wr, tmp112_i2c_read_t rd, void* phy_i2c) {
    if (ctx == NULL) {
        return TMP112_ERROR_INVALID_INPUT;
    }
    if (wr == NULL) {
        return TMP112_ERROR_INVALID_INPUT;
    }
    if (rd == NULL) {
        return TMP112_ERROR_INVALID_INPUT;
    }

    ctx->wr_cb = wr;
    ctx->rd_cb = rd;
    ctx->phy_i2c = phy_i2c;
    ctx->verify_write = true;

    return TMP112_OK;
}

uint8_t tmp112_get_celsius (tmp112_ctx_t* ctx, float* temp) {
    uint8_t t_in_bytes[2] = {0};
    tmp112_read_2byte_reg(ctx, TMP112_REGS_TEMP, t_in_bytes);

    *temp = tmp112_bytes_to_float(t_in_bytes[0], t_in_bytes[1]);

    return TMP112_OK;
}

uint8_t tmp112_set_celsius_tlow_thigh  (tmp112_ctx_t* ctx, float tlow, float thigh) {
    if ((tlow < TMP112_MIN_TEMP) || (tlow > TMP112_MAX_TEMP)) {
        return TMP112_ERROR_INVALID_INPUT;
    }
    if ((thigh < TMP112_MIN_TEMP) || (thigh > TMP112_MAX_TEMP)) {
        return TMP112_ERROR_INVALID_INPUT;
    }

    uint8_t rc = TMP112_ERROR_UNKNOWN;

    uint8_t t_in_bytes[2] = {0};
    tmp112_float_to_bytes(tlow, t_in_bytes);
    rc = tmp112_write_2byte_reg(ctx, TMP112_REGS_TLOW, t_in_bytes);
    if (rc != TMP112_OK) {
        return rc;
    }

    tmp112_float_to_bytes(thigh, t_in_bytes);
    rc = tmp112_write_2byte_reg(ctx, TMP112_REGS_THIGH, t_in_bytes);
    if (rc != TMP112_OK) {
        return rc;
    }

    return TMP112_OK;
}
