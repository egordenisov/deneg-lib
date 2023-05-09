#include <gtest/gtest.h>
#include "stdbool.h"
#include "stdint.h"

#include "tmp112-driver.h"

tmp112_ctx_t tmp112_ctx;

typedef struct {
    float temp;
    uint8_t b1;
    uint8_t b2;
} vals_t;

static uint8_t val_ptr = 0;
static vals_t vals[] = {
    {.temp = 127.9375, .b1 = 0x7F, .b2 = 0xF0},
    {.temp = 100,      .b1 = 0x64, .b2 = 0x00},
    {.temp = 80,       .b1 = 0x50, .b2 = 0x00},
    {.temp = 75,       .b1 = 0x4B, .b2 = 0x00},
    {.temp = 50,       .b1 = 0x32, .b2 = 0x00},
    {.temp = 25,       .b1 = 0x19, .b2 = 0x00},
    {.temp = 0.25,     .b1 = 0x00, .b2 = 0x40},
    {.temp = 0.0,      .b1 = 0x00, .b2 = 0x00},
    {.temp = -0.25,    .b1 = 0xFF, .b2 = 0xC0},
    {.temp = -25,      .b1 = 0xE7, .b2 = 0x00},
    {.temp = -55,      .b1 = 0xC9, .b2 = 0x00},
};

static uint8_t buf_prev_reg_b1;
static uint8_t buf_prev_reg_b2;

static uint8_t buf_reg_b1;
static uint8_t buf_reg_b2;
static uint8_t buf_reg_addr;

static uint8_t i2c_write (void* i2c, uint8_t* data, uint32_t size) {
    buf_reg_addr = data[0];
    if (size == 1) {
        return TMP112_OK;
    }

    if (size == 3) {
        buf_prev_reg_b1 = buf_reg_b1;
        buf_prev_reg_b2 = buf_reg_b2;
        buf_reg_b1 = data[1];
        buf_reg_b2 = data[2];
        return TMP112_OK;
    }
    return -1;
}

static uint8_t i2c_read (void* i2c, uint8_t* data, uint32_t size) {
    if (size != 2) {
        return -1;
    }
    data[0] = buf_reg_b1;
    data[1] = buf_reg_b2;

    if (buf_reg_addr == 0x00) {
        data[0] = vals[val_ptr].b1;
        data[1] = vals[val_ptr].b2;
    }
    return TMP112_OK;
}
static uint32_t phy_i2c_ctx = 32;

TEST(tmp112_init, set_init_values)
{
    tmp112_ctx.phy_i2c = NULL;
    tmp112_ctx.wr_cb = NULL;
    tmp112_ctx.rd_cb = NULL;

    tmp112_error_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    
    EXPECT_EQ(rc, TMP112_OK);
    EXPECT_EQ(tmp112_ctx.phy_i2c, &phy_i2c_ctx);
    EXPECT_EQ(tmp112_ctx.wr_cb, i2c_write);
    EXPECT_EQ(tmp112_ctx.rd_cb, i2c_read);
};

TEST(tmp112_init, set_zero_values)
{
    tmp112_error_t rc = TMP112_OK;
    rc = tmp112_init(NULL, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_ERROR_INVALID_INPUT);

    rc = TMP112_OK;
    rc = tmp112_init(&tmp112_ctx, NULL, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_ERROR_INVALID_INPUT);

    rc = TMP112_OK;
    rc = tmp112_init(&tmp112_ctx, i2c_write, NULL, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_ERROR_INVALID_INPUT);

    rc = TMP112_OK;
    rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, NULL);
    EXPECT_EQ(rc, TMP112_OK);
};

static float check_temp() {
    float temp = -255;

    tmp112_error_t rc = tmp112_get_celsius(&tmp112_ctx, &temp);
    EXPECT_EQ(rc, TMP112_OK);

    return temp;
}

TEST(tmp112_get_celsius, get_values)
{
    tmp112_error_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_OK);

    for (uint32_t i = 0; i < (sizeof(vals)/sizeof(vals_t)); i++) {
        val_ptr = i;
        EXPECT_FLOAT_EQ(check_temp(), vals[val_ptr].temp);
    }
};

static void check_set_tlow_thigh() {
    tmp112_error_t rc = tmp112_set_celsius_tlow_thigh(&tmp112_ctx, vals[val_ptr+1].temp, vals[val_ptr].temp);
    EXPECT_EQ(rc, TMP112_OK);

    return;
}

TEST(tmp112_set_celsius_tlow_thigh, set_values)
{
    tmp112_error_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_OK);

    for (uint32_t i = 0; i < (sizeof(vals)/sizeof(vals_t) - 1); i++) {
        val_ptr = i;
        buf_reg_b1 = 0;
        buf_reg_b2 = 255;
        check_set_tlow_thigh();

        EXPECT_EQ(vals[val_ptr].b1, buf_reg_b1);
        EXPECT_EQ(vals[val_ptr].b2, buf_reg_b2);

        EXPECT_EQ(vals[val_ptr+1].b1, buf_prev_reg_b1);
        EXPECT_EQ(vals[val_ptr+1].b2, buf_prev_reg_b2);
    }
};

TEST(tmp112_set_convertion_rate, execute)
{
    tmp112_error_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_OK);

    rc = tmp112_set_convertion_rate(&tmp112_ctx, TMP112_CONV_RATE_1Hz);
    EXPECT_EQ(rc, TMP112_OK);
}

TEST(tmp112_set_fault_queue, execute)
{
    tmp112_error_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_OK);

    rc = tmp112_set_fault_queue(&tmp112_ctx, TMP112_FAUTLT_QUEUE_2);
    EXPECT_EQ(rc, TMP112_OK);
}