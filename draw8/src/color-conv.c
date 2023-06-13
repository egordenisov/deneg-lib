#include "color-conv.h"

inline uint16_t _rgba_to_rgb565(d8_color_t color) {
    uint16_t r = ((uint16_t)color.a) * ((uint16_t)color.r) / 0xFF;
    uint16_t g = ((uint16_t)color.a) * ((uint16_t)color.g) / 0xFF;
    uint16_t b = ((uint16_t)color.a) * ((uint16_t)color.b) / 0xFF;

    uint16_t rgb565 = 0;
    rgb565 |= (b & 0x1F) << 0;
    rgb565 |= (g & 0x3F) << 5;
    rgb565 |= (r & 0x1F) << 11;

    return rgb565;
}

inline uint16_t _rgba_to_monochrome(d8_color_t color) {
    if (color.a == 0) {
        return 0;
    }

    if (color.r == 0 && color.g == 0 && color.b == 0) {
        return 0;
    }

    return UINT16_MAX;
}

uint16_t _rgba_to_val (d8_ctx_t* ctx, d8_color_t color) {
    switch (ctx->color_type) {
        case D8_MONOCHROME: return _rgba_to_monochrome  (color);
        case D8_RGB565:     return _rgba_to_rgb565      (color);
    }
    return 0;
}
