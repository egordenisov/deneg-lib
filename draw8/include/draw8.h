#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef D8_MAX_STR_LEN
#define D8_MAX_STR_LEN 128
#endif

typedef enum {
    D8_MONOCHROME   = 1,
    D8_RGB565       = 16,
} d8_color_type_t;

enum {
    D8_OK = 0,
    D8_NULL_PTR,
    D8_BUF_SIZE,
    D8_WIDTH_LIMIT,
    D8_HEIGHT_LIMIT,
    D8_ORDER_ERR,
    D8_COLOR_TYPE,
};

/**
 * x1y1->->->->->->->->
 * ->->->->->->->->->->
 * ->->->->->->->->->->
 * ->->->->->->->->x2y2
 */
typedef void (*d8_draw_raw) (void* disp_ctx, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t* data);

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} d8_color_t;

typedef struct {
    uint16_t w;
    uint16_t h;
    d8_color_type_t color_type;
    d8_draw_raw draw;

    uint8_t* buf;
    uint32_t buf_len;
    void* disp_ctx;


    d8_font_t default_font;

    char buf_str[D8_MAX_STR_LEN];
} d8_ctx_t;

typedef struct {
    uint16_t sym_width;
    uint16_t sym_height;
    uint16_t sym_count;
    uint8_t* font;
} d8_font_t;

typedef struct {
    uint16_t    x1;
    uint16_t    y1;
    uint16_t    x2;
    uint16_t    y2;
    d8_color_t  color;
} d8_line_t;

typedef struct {
    uint16_t width_in_symbols;
    uint16_t height_in_symbols;
    uint16_t x1;
    uint16_t y1;
    char** text_buf;
    d8_font_t* font;
    d8_color_t font_color;
    d8_color_t background_color;

    uint16_t current_height;
} d8_text_box_ctx_t;

typedef struct {
    uint16_t width_in_symbols;
    uint16_t height_in_symbols;
    uint16_t x1;
    uint16_t y1;
    char** text_buf;
    d8_font_t* custom_font;
    d8_color_t font_color;
    d8_color_t background_color;
} d8_text_box_init_t;

typedef struct {
    uint16_t        width;
    uint16_t        height;
    d8_color_type_t color_type;
    uint8_t*        buf;
    uint32_t        buf_len;
    d8_draw_raw     draw;
    void*           disp_ctx;
} d8_init_t;


int d8_init (d8_ctx_t* ctx, d8_init_t* init_struct);

int d8_draw_line (d8_ctx_t* ctx, d8_line_t* line);

int d8_draw_fill (d8_ctx_t* ctx, d8_line_t* diagonal);

int d8_text_box_create (d8_ctx_t* ctx, d8_text_box_ctx_t* text_box_ctx, d8_text_box_init_t* text_box_init);
int d8_text_box_write (d8_ctx_t* ctx, d8_text_box_ctx_t* text_box_ctx, const char* format, ...);
int d8_text_box_clear (d8_ctx_t* ctx, d8_text_box_ctx_t* text_box_ctx);

#ifdef __cplusplus
}
#endif
