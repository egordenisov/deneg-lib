#include "draw8.h"
#include "color-conv.h"

int _d8_draw_fill (d8_ctx_t* ctx, d8_line_t* diagonal) {
    uint32_t size = (diagonal->x2 - diagonal->x1 + 1) * (diagonal->y2 - diagonal->y1 + 1);

    uint16_t val = _rgba_to_val(ctx, diagonal->color);

    switch (ctx->color_type)
    {
    case D8_MONOCHROME:
        bool px = !!val;
        for (int i = 0; i < size; i++) {
            ctx->buf[i/8] &= ~(1 << (i%8) );
            ctx->buf[i/8] |= px << (i%8);
        }
        break;
    
    case D8_RGB565:
        for (int i = 0; i < size; i++) {
            *( ((uint16_t*)ctx->buf) + i ) = val;
        }
        break;

    default:
        return -D8_COLOR_TYPE;
    }

    ctx->draw(ctx->disp_ctx, diagonal->x1, diagonal->y1, diagonal->x2, diagonal->y2, ctx->buf);
    return D8_OK;
}