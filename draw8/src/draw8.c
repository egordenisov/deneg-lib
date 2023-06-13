#include "draw8.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#define RETURN_IF_NULL_PTR(x) { if( (x) == NULL ) {return -D8_NULL_PTR;} }

int d8_init (d8_ctx_t* ctx, d8_init_t* init_struct) {
    RETURN_IF_NULL_PTR(ctx);
    RETURN_IF_NULL_PTR(init_struct);
    RETURN_IF_NULL_PTR(init_struct->draw);
    RETURN_IF_NULL_PTR(init_struct->buf);

    uint16_t h = init_struct->height;
    uint16_t w = init_struct->width;
    uint16_t c = (uint16_t)init_struct->color_type;

    uint32_t required_buf_len = ceil(h * w * c / 8);

    if (init_struct->buf_len < required_buf_len){
        return -D8_BUF_SIZE;
    }

    memset(ctx, 0, sizeof(d8_ctx_t));
    memset(init_struct->buf, 0, required_buf_len);

    ctx->h          = h;
    ctx->w          = w;
    ctx->color_type = init_struct->color_type;
    ctx->buf        = init_struct->buf;
    ctx->buf_len    = init_struct->buf_len;
    ctx->draw       = init_struct->draw;
    ctx->disp_ctx   = init_struct->disp_ctx;

    return D8_OK;
}

int d8_draw_fill (d8_ctx_t* ctx, d8_line_t* diagonal) {
    RETURN_IF_NULL_PTR(ctx);
    RETURN_IF_NULL_PTR(diagonal);

    if ((diagonal->x2 < diagonal->x1) || (diagonal->y2 < diagonal->y1)) {
        return -D8_ORDER_ERR;
    }
    if (diagonal->x2 >= ctx->w) {
        return -D8_WIDTH_LIMIT;
    }
    if (diagonal->y2 >= ctx->h) {
        return -D8_HEIGHT_LIMIT;
    }

    return _d8_draw_fill(ctx, diagonal);
}

int d8_text_box_create (d8_ctx_t* ctx, d8_text_box_ctx_t* text_box_ctx, d8_text_box_init_t* text_box_init) {
    RETURN_IF_NULL_PTR(ctx);
    RETURN_IF_NULL_PTR(text_box_ctx);
    RETURN_IF_NULL_PTR(text_box_init);
    RETURN_IF_NULL_PTR(text_box_init->text_buf);

    memset(text_box_ctx, 0, sizeof(d8_text_box_ctx_t));

    text_box_ctx->font = text_box_init->custom_font ? text_box_init->custom_font : &(ctx->default_font);

    uint16_t sym_w = text_box_ctx->font->sym_width;
    uint16_t sym_h = text_box_ctx->font->sym_height;

    uint16_t box_w = text_box_init->width_in_symbols;
    uint16_t box_h = text_box_init->height_in_symbols;

    uint16_t x1 = text_box_init->x1;
    uint16_t y1 = text_box_init->y1;

    if (sym_w * box_w + x1 >= ctx->w) {
        return -D8_WIDTH_LIMIT;
    }

    if (sym_h * box_h + y1 >= ctx->h) {
        return -D8_HEIGHT_LIMIT;
    }

    text_box_ctx->x1                = x1;
    text_box_ctx->y1                = y1;
    text_box_ctx->height_in_symbols = box_h;
    text_box_ctx->width_in_symbols  = box_w;
    text_box_ctx->text_buf          = text_box_init->text_buf;
    text_box_ctx->font_color        = text_box_init->font_color;
    text_box_ctx->background_color  = text_box_init->background_color;

    d8_line_t line = {
        .x1 = x1,
        .x2 = x1 + sym_w * box_w,
        .y1 = y1,
        .y2 = y1 + sym_h * box_h,
        .color = text_box_ctx->background_color,
    };

    return _d8_draw_fill(ctx, &line);
}



int d8_draw_char (d8_ctx_t* ctx, char c, uint16_t x1, uint16_t y1, d8_color_t background, d8_color_t font) {

}

int d8_text_box_write (d8_ctx_t* ctx, d8_text_box_ctx_t* text_box_ctx, const char* format, ...) {
    RETURN_IF_NULL_PTR(ctx);
    RETURN_IF_NULL_PTR(text_box_ctx);
    RETURN_IF_NULL_PTR(format);

    va_list ap;
    sprintf(ctx->buf_str, format, ap);
    
    if (strlen(ctx->buf_str) > text_box_ctx->width_in_symbols) {
        return -D8_WIDTH_LIMIT;
    }

    for (int32_t i = text_box_ctx->current_height; i > 0; i--) {
        strcpy(text_box_ctx->text_buf[i], text_box_ctx->text_buf[i - 1]);
    }
    strcpy(text_box_ctx->text_buf[0], ctx->buf_str);

    if (text_box_ctx->current_height < text_box_ctx->height_in_symbols) {
        text_box_ctx->current_height++;
    }
    
    for (int32_t i = 0; i < text_box_ctx->current_height; i++) {
        int rc = d8_draw_string();
        if (rc != D8_OK) {
            return rc;
        }
    }

    return D8_OK;
}

int d8_text_box_clear (d8_ctx_t* ctx, d8_text_box_ctx_t* text_box_ctx) {
    RETURN_IF_NULL_PTR(ctx);
    RETURN_IF_NULL_PTR(text_box_ctx);

    text_box_ctx->current_height = 0;

    d8_line_t line = {
        .x1 = text_box_ctx->x1,
        .x2 = text_box_ctx->x1 + text_box_ctx->font->sym_width * text_box_ctx->width_in_symbols,
        .y1 = text_box_ctx->y1,
        .y2 = text_box_ctx->y1 + text_box_ctx->font->sym_height * text_box_ctx->height_in_symbols,
        .color = text_box_ctx->background_color,
    };

    return _d8_draw_fill(ctx, &line);
}