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

    uint32_t required_buf_len = ceil(h * w * (c / 8) / D8_BUF_ONE_FOURTH);
    if (init_struct->buf_len < required_buf_len){
        return -D8_BUF_SIZE;
    }

    memset(ctx, 0, sizeof(d8_ctx_t));
    memset(init_struct->buf, 0, required_buf_len);

    uint32_t full_buf_len = ceil(h * w * (c / 8) / D8_BUF_FULL_SIZE);

    ctx->h          = h;
    ctx->w          = w;
    ctx->color_type = init_struct->color_type;
    
    ctx->buf        = init_struct->buf;
    ctx->buf_len    = init_struct->buf_len;
    ctx->buf_type   = (init_struct->buf_len < full_buf_len) ? D8_BUF_ONE_FOURTH : D8_BUF_FULL_SIZE;
    
    ctx->draw       = init_struct->draw;
    ctx->disp_ctx   = init_struct->disp_ctx;

    return D8_OK;
}
