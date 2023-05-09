#include "tmp112-driver.h"
#include "tmp112-regs.h"

static float tmp112_reg_to_float (uint16_t reg_value) {
    uint16_t raw = reg_value >> 4; // 4 bits are always zero
    if (raw & (1 << 11)) {
        return -(float)((~raw + 1) & TMP112_UNSIG_RESOLUTION_MASK) / TMP112_LSB_PER_CELSIUS;
    } else {
        return ((float)raw) / TMP112_LSB_PER_CELSIUS;
    }
}

static void tmp112_float_to_reg (float f, uint16_t* reg_value) {
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
    *reg_value = raw << 4;

    return;
}

static tmp112_error_t tmp112_read_reg (tmp112_ctx_t* ctx, uint8_t addr, uint16_t* read_data) {
    uint8_t rc = TMP112_ERROR_UNKNOWN;
    rc = ctx->wr_cb(ctx, &addr, sizeof(addr));
    if (rc != 0) {
        return TMP112_ERROR_I2C_WRITE_ERROR;
    }

    uint8_t buf[2] = {0};
    rc = ctx->rd_cb(ctx, buf, sizeof(buf));
    if (rc != 0) {
        return TMP112_ERROR_I2C_READ_ERROR;
    }

    *read_data = (uint16_t)(buf[0]) * 256 + (uint16_t)(buf[1]);
    return TMP112_OK;
}

static tmp112_error_t tmp112_write_reg (tmp112_ctx_t* ctx, uint8_t addr, uint16_t write_data) {
    uint8_t buf[3] = {0};
    uint8_t rc = TMP112_ERROR_UNKNOWN;

    buf[0] = addr;
    buf[1] = (uint8_t)(write_data >> 8);
    buf[2] = (uint8_t)(write_data & 0xFF);

    rc = ctx->wr_cb(ctx, buf, sizeof(buf));
    if (rc != 0) {
        return TMP112_ERROR_I2C_WRITE_ERROR;
    }

    if (!ctx->verify_write) {
        return TMP112_OK;
    }

    uint16_t read_data = 0;
    rc = tmp112_read_reg(ctx, addr, &read_data);
    if ((rc != TMP112_OK) || (read_data != write_data)) {
        printf("\n\n%d\n\n", read_data);
        printf("\n\n%d\n\n", write_data);
        return TMP112_ERROR_WRITE_VERIFICATION;
    }

    return TMP112_OK;
}

tmp112_error_t tmp112_init (tmp112_ctx_t* ctx, tmp112_i2c_write_t wr, tmp112_i2c_read_t rd, void* phy_i2c) {
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

    uint8_t rc = tmp112_write_reg(ctx, TMP112_REGS_CONFIG, TMP112_REGS_CONFIG_DEFAULT);
    if (rc != TMP112_OK) {
        return rc;
    }

    return TMP112_OK;
}

tmp112_error_t tmp112_get_celsius (tmp112_ctx_t* ctx, float* temp) {
    uint16_t temp_16b = 0;
    tmp112_read_reg(ctx, TMP112_REGS_TEMP, &temp_16b);

    *temp = tmp112_reg_to_float(temp_16b);

    return TMP112_OK;
}

tmp112_error_t tmp112_set_celsius_tlow_thigh  (tmp112_ctx_t* ctx, float tlow, float thigh) {
    if ((tlow < TMP112_MIN_TEMP) || (tlow > TMP112_MAX_TEMP)) {
        return TMP112_ERROR_INVALID_INPUT;
    }
    if ((thigh < TMP112_MIN_TEMP) || (thigh > TMP112_MAX_TEMP)) {
        return TMP112_ERROR_INVALID_INPUT;
    }
    if (tlow >= thigh) {
        return TMP112_ERROR_INVALID_INPUT;
    }

    uint8_t rc = TMP112_ERROR_UNKNOWN;
    uint16_t temp_16b = 0;

    tmp112_float_to_reg(tlow, &temp_16b);
    rc = tmp112_write_reg(ctx, TMP112_REGS_TLOW, temp_16b);
    if (rc != TMP112_OK) {
        return rc;
    }

    tmp112_float_to_reg(thigh, &temp_16b);
    rc = tmp112_write_reg(ctx, TMP112_REGS_THIGH, temp_16b);
    if (rc != TMP112_OK) {
        return rc;
    }

    return TMP112_OK;
}

tmp112_error_t tmp112_set_convertion_rate (tmp112_ctx_t* ctx, tmp112_conv_rate_t cr) {
    tmp112_regs_config_t config_reg;
    tmp112_error_t rc =  tmp112_read_reg(ctx, TMP112_REGS_CONFIG, &(config_reg.word));
    if (rc != TMP112_OK) {
        return rc;
    }

    config_reg.bf.cr = cr;
    rc = tmp112_write_reg(ctx, TMP112_REGS_CONFIG, config_reg.word);
    if (rc != TMP112_OK) {
        return rc;
    }
    return TMP112_OK;
}

tmp112_error_t tmp112_set_fault_queue (tmp112_ctx_t* ctx, tmp112_fault_queue_t fq) {
    tmp112_regs_config_t config_reg;
    tmp112_error_t rc =  tmp112_read_reg(ctx, TMP112_REGS_CONFIG, &(config_reg.word));
    if (rc != TMP112_OK) {
        return rc;
    }

    config_reg.bf.fault = fq;
    rc = tmp112_write_reg(ctx, TMP112_REGS_CONFIG, config_reg.word);
    if (rc != TMP112_OK) {
        return rc;
    }
    return TMP112_OK;
}

