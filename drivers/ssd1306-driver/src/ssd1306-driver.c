#include <string.h>
#include "ssd1306-driver.h"
#include "ssd1306-fonts.h"
#include "ssd1306-regs.h"

#ifndef count_of
#define count_of(a) (sizeof(a)/sizeof((a)[0]))
#endif

void ssd1306_send_cmd(ssd1306_ctx_t* ctx, uint8_t cmd) {
    uint8_t buf[2] = {0x80, cmd};
    ctx->write((ctx->i2c_addr & SSD1306_WRITE_MODE), buf, 2);
}

void ssd1306_send_cmd_list(ssd1306_ctx_t* ctx, uint8_t *buf, uint32_t num) {
    for (uint32_t i = 0; i < num; i++) {
        ssd1306_send_cmd(ctx, buf[i]);
    }
}

void ssd1306_send_buf(ssd1306_ctx_t* ctx, uint8_t* data, uint32_t size) {
    data[0] = 0x40;
    ctx->write((ctx->i2c_addr & SSD1306_WRITE_MODE), data, size + 1);
}

bool ssd1306_init_driver (ssd1306_ctx_t* ctx, uint8_t (*buf) [SSD1306_BUFSIZE], ssd1306_write write) {
    if (ctx == NULL) {
        return false;
    }
    if (buf == NULL) {
        return false;
    }
    if (write == NULL) {
        return false;
    }

    ctx->buf = buf;

    memset(*(ctx->buf), 0, SSD1306_BUFSIZE);

    ctx->write = write;
    ctx->i2c_addr = SSD1306_I2C_ADDR_DEFAULT;
}

bool ssd1306_set_i2c_addr (ssd1306_ctx_t* ctx, uint8_t i2c_addr) {
    ctx->i2c_addr = i2c_addr;
}

bool ssd1306_init (ssd1306_ctx_t* ctx) {
    uint8_t cmds[] = {
        SSD1306_SET_DISP,               // set display off
        /* memory mapping */
        SSD1306_SET_MEM_MODE,           // set memory address mode 0 = horizontal, 1 = vertical, 2 = page
        0x00,                           // horizontal addressing mode
        /* resolution and layout */
        SSD1306_SET_DISP_START_LINE,    // set display start line to 0
        SSD1306_SET_SEG_REMAP | 0x01,   // set segment re-map, column address 127 is mapped to SEG0
        SSD1306_SET_MUX_RATIO,          // set multiplex ratio
        SSD1306_HEIGHT - 1,             // Display height - 1
        SSD1306_SET_COM_OUT_DIR | 0x08, // set COM (common) output scan direction. Scan from bottom up, COM[N-1] to COM0
        SSD1306_SET_DISP_OFFSET,        // set display offset
        0x00,                           // no offset
        SSD1306_SET_COM_PIN_CFG,        // set COM (common) pins hardware configuration. Board specific magic number. 
                                        // 0x02 Works for 128x32, 0x12 Possibly works for 128x64. Other options 0x22, 0x32
#if ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 32))
        0x02,                           
#elif ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 64))
        0x12,
#else
        0x02,
#endif
        /* timing and driving scheme */
        SSD1306_SET_DISP_CLK_DIV,       // set display clock divide ratio
        0x80,                           // div ratio of 1, standard freq
        SSD1306_SET_PRECHARGE,          // set pre-charge period
        0xF1,                           // Vcc internally generated on our board
        SSD1306_SET_VCOM_DESEL,         // set VCOMH deselect level
        0x30,                           // 0.83xVcc
        /* display */
        SSD1306_SET_CONTRAST,           // set contrast control
        0xFF,
        SSD1306_SET_ENTIRE_ON,          // set entire display on to follow RAM content
        SSD1306_SET_NORM_DISP,           // set normal (not inverted) display
        SSD1306_SET_CHARGE_PUMP,        // set charge pump
        0x14,                           // Vcc internally generated on our board
        SSD1306_SET_SCROLL | 0x00,      // deactivate horizontal scrolling if set. This is necessary as memory writes will corrupt if scrolling was enabled
        SSD1306_SET_DISP | 0x01, // turn display on
    };

    ssd1306_send_cmd_list(ctx, cmds, count_of(cmds));
}

static inline int get_font_index(uint8_t ch) {
    if (ch >= 'A' && ch <='Z') {
        return  ch - 'A' + 1;
    }
    else if (ch >= '0' && ch <='9') {
        return  ch - '0' + 27;
    }
    else if (ch == '.') {
        return 37;
    }
    else if (ch == '-') {
        return 38;
    }
    else if (ch = '=') {
        return 39;
    }
    else return  0; // Not got that char so space.
}

static uint8_t reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

bool ssd1306_char (ssd1306_ctx_t* ctx, char c, uint8_t x, uint8_t y) {
    if (x >= SSD1306_WIDTH - 8) {
        return false;
    }
    if (y >= SSD1306_HEIGHT - 8) {
        return false;
    }
    
    uint8_t* symbol = &rp_font[get_font_index(c)*8];

    uint32_t up_page_idx = (y / 8) * 128 + x;
    uint8_t up_mask = 0xFF;
    if (y % 8) {
        up_mask = ~((1 << (y % 8)) - 1);
    }

    uint32_t dn_page_idx = ((y / 8) + 1) * 128 + x;
    uint8_t dn_mask = (1 << (y % 8)) - 1;

    for (uint32_t i = 0; i < 8; i++) {
        (*ctx->buf)[1 + up_page_idx + i] &= ~up_mask;
        (*ctx->buf)[1 + up_page_idx + i] |= ((reverse(symbol[i]) << (y % 8)) & up_mask);

        (*ctx->buf)[1 + dn_page_idx + i] &= ~dn_mask;
        (*ctx->buf)[1 + dn_page_idx + i] |= ((reverse(symbol[i]) >> (8 - (y % 8))) & dn_mask);
    }
}

bool ssd1306_string (ssd1306_ctx_t* ctx, char* s, uint8_t x, uint8_t y) {
    uint8_t len = strlen(s);
    if ((uint32_t)x + len * 8 >= SSD1306_WIDTH) {
        return false;
    }
    if (y >= SSD1306_HEIGHT - 8) {
        return false;
    }
    for (uint32_t i = 0; i < len; i++) {
        ssd1306_char(ctx, s[i], x + i*8, y);
    }
}

bool ssd1306_render (ssd1306_ctx_t* ctx) {
    uint8_t cmds[] = {
        SSD1306_SET_COL_ADDR,
        0,
        SSD1306_WIDTH - 1,
        SSD1306_SET_PAGE_ADDR,
        0,
        SSD1306_NUM_PAGES - 1,
    };
    
    ssd1306_send_cmd_list(ctx, cmds, count_of(cmds));
    ssd1306_send_buf(ctx, *ctx->buf, SSD1306_HEIGHT * SSD1306_WIDTH / 8);
}

