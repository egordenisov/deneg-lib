#include "text.h"
#include "draw8.h"

int _d8_draw_char (d8_ctx_t* ctx, char c, uint16_t x1, uint16_t y1, d8_color_t background, d8_color_t font, d8_font_t* custom_font) {
    
    d8_font_t* font = custom_font ? custom_font : &(ctx->default_font);
    for (int32_t i = 0; i < font->sym_height; i++) {
        for (int32_t j = 0; j < font->sym_width; j+=8) {
            
        }
    }

    return D8_OK;
}