#include "draw8.h"
#include "color-conv.h"
#include <math.h>

int _d8_draw_fill (d8_ctx_t* ctx, d8_line_t* diagonal) {
    uint32_t size = (diagonal->x2 - diagonal->x1 + 1) * (diagonal->y2 - diagonal->y1 + 1);

    uint16_t val = _rgba_to_val(ctx, diagonal->color);

    for (int i = 0; i <= size / ctx->buf_len; i++) {
        uint32_t step_size = (i+1)*ctx->buf_len >= size ? 
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
    }

    return D8_OK;
}

int _d8_draw_frame (d8_ctx_t* ctx, d8_line_t* diagonal, uint16_t frame_widht) {
    int rc = D8_OK;
    {
        d8_line_t left_vertical = {
            .x1 = diagonal->x1,
            .x2 = diagonal->x1 + frame_widht,
            .y1 = diagonal->y1,
            .y2 = diagonal->y2,
            .color = diagonal->color,
        };
        rc = _d8_draw_fill(ctx, &left_vertical);
        if (rc != D8_OK) {
            return rc;
        }
    }
    {
        d8_line_t top_horizontal = {
            .x1 = diagonal->x1,
            .x2 = diagonal->x2,
            .y1 = diagonal->y1,
            .y2 = diagonal->y1 + frame_widht,
            .color = diagonal->color,
        };
        rc = _d8_draw_fill(ctx, &top_horizontal);
        if (rc != D8_OK) {
            return rc;
        }
    }
    {
        d8_line_t right_vertical = {
            .x1 = diagonal->x2 - frame_widht,
            .x2 = diagonal->x2,
            .y1 = diagonal->y1,
            .y2 = diagonal->y2,
            .color = diagonal->color,
        };
        rc = _d8_draw_fill(ctx, &right_vertical);
        if (rc != D8_OK) {
            return rc;
        }
    }
    {
        d8_line_t bottom_horizontal = {
            .x1 = diagonal->x1,
            .x2 = diagonal->x2,
            .y1 = diagonal->y2 - frame_widht,
            .y2 = diagonal->y2,
            .color = diagonal->color,
        };
        rc = _d8_draw_fill(ctx, &bottom_horizontal);
        if (rc != D8_OK) {
            return rc;
        }
    }

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

