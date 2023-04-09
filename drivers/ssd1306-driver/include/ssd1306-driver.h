#ifndef __SSD1306_DRIVER_H
#define __SSD1306_DRIVER_H

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SSD1306_WIDTH               128
#define SSD1306_HEIGHT              32

#if SSD1306_WIDTH!=128
    #error "Supports only width=128"
#endif

#if SSD1306_HEIGHT!=32
    #error "Supports only height=32"
#endif

typedef uint32_t (*ssd1306_write) (uint8_t addr, const uint8_t* data, uint32_t size);

#define SSD1306_BUFSIZE (SSD1306_WIDTH*SSD1306_HEIGHT/8 + 1)

typedef struct SSD1306_CTX {
    uint8_t (*buf) [SSD1306_BUFSIZE];

    uint8_t i2c_addr;

    ssd1306_write write;

} ssd1306_ctx_t;


bool ssd1306_init_driver (ssd1306_ctx_t* ctx, uint8_t (*buf) [SSD1306_BUFSIZE], ssd1306_write write);
bool ssd1306_init (ssd1306_ctx_t* ctx);

bool ssd1306_char (ssd1306_ctx_t* ctx, char c, uint8_t x, uint8_t y);
bool ssd1306_string (ssd1306_ctx_t* ctx, char* s, uint8_t x, uint8_t y);
bool ssd1306_render (ssd1306_ctx_t* ctx);

/**
 * @brief Set custom i2c address instead of default 0x3C addr
 * If use call this function BEFORE ssd1306_init
 * @param ctx
 * @param i2c_addr
 */
bool ssd1306_set_i2c_addr (ssd1306_ctx_t* ctx, uint8_t i2c_addr);

#define SSD1306_I2C_ADDR_DEFAULT    (0x3C)

#define SSD1306_WRITE_MODE          (0xFE)
#define SSD1306_READ_MODE           (0xFF)

#ifdef __cplusplus
}
#endif

#endif // #ifndef __SSD1306_DRIVER_H