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

static uint8_t setted_temp_b1;
static uint8_t setted_temp_b2;

static uint8_t i2c_write (void* i2c, uint8_t* data, uint32_t size) {
    if (size == 3) {
        if ((data[0] == 0x02) || (data[0] == 0x03)){
            setted_temp_b1 = data[1];
            setted_temp_b2 = data[2];
        }
    }

    return 0;
}

static uint8_t i2c_read (void* i2c, uint8_t* data, uint32_t size) {
    if (size != 2){
        return -1;
    }

    data[0] = vals[val_ptr].b1;
    data[1] = vals[val_ptr].b2;
    return 0;
}
static uint32_t phy_i2c_ctx = 32;

TEST(tmp112_init, set_init_values)
{
    tmp112_ctx.phy_i2c = NULL;
    tmp112_ctx.wr_cb = NULL;
    tmp112_ctx.rd_cb = NULL;

    uint8_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    
    EXPECT_EQ(rc, TMP112_OK);
    EXPECT_EQ(tmp112_ctx.phy_i2c, &phy_i2c_ctx);
    EXPECT_EQ(tmp112_ctx.wr_cb, i2c_write);
    EXPECT_EQ(tmp112_ctx.rd_cb, i2c_read);
};

TEST(tmp112_init, set_zero_values)
{
    uint8_t rc = 0;
    rc = tmp112_init(NULL, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_ERROR_INVALID_INPUT);

    rc = 0;
    rc = tmp112_init(&tmp112_ctx, NULL, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_ERROR_INVALID_INPUT);

    rc = 0;
    rc = tmp112_init(&tmp112_ctx, i2c_write, NULL, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_ERROR_INVALID_INPUT);

    rc = 0;
    rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, NULL);
    EXPECT_EQ(rc, TMP112_OK);
};

static float check_temp() {
    float temp = -255;

    uint8_t rc = tmp112_get_celsius(&tmp112_ctx, &temp);
    EXPECT_EQ(rc, TMP112_OK);

    return temp;
}

TEST(tmp112_get_celsius, get_values)
{
    uint8_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_OK);

    for (uint32_t i = 0; i < (sizeof(vals)/sizeof(vals_t)); i++) {
        val_ptr = i;
        EXPECT_FLOAT_EQ(check_temp(), vals[val_ptr].temp);
    }
};

static void check_set_tlow_thigh() {
    uint8_t rc = tmp112_set_celsius_tlow_thigh(&tmp112_ctx, vals[val_ptr].temp, vals[val_ptr].temp);
    EXPECT_EQ(rc, TMP112_OK);

    return;
}

TEST(tmp112_set_celsius_tlow_thigh, set_values)
{
    uint8_t rc = tmp112_init(&tmp112_ctx, i2c_write, i2c_read, &phy_i2c_ctx);
    EXPECT_EQ(rc, TMP112_OK);

    for (uint32_t i = 0; i < (sizeof(vals)/sizeof(vals_t)); i++) {
        val_ptr = i;
        setted_temp_b1 = 0;
        setted_temp_b2 = 255;
        check_set_tlow_thigh();

        EXPECT_EQ(vals[val_ptr].b1, setted_temp_b1);
        EXPECT_EQ(vals[val_ptr].b2, setted_temp_b2);
    }
};
