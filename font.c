#include "sdl_pong.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

static unsigned char ttf_buffer[1<<25];

static FILE *
init_font(char *filename) {
  FILE *f = fopen(filename, "rb");
  fread(ttf_buffer, 1, 1<<25, f);

  return f;
}

static void
font_draw_text(sdl_buffer *backbuffer, int x, int y, unsigned char *text) {
  stbtt_fontinfo font;
  unsigned char *bitmap;
  stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

  int s = 30;

  float scale = stbtt_ScaleForPixelHeight(&font, s);
  int ascent = 0;

  stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
  int baseline = ascent*scale;

  int x_cursor = x;
  while (*text) {
    int advance, lsb, x0, y0, x1, y1;

    stbtt_GetCodepointHMetrics(&font, *text, &advance, &lsb);
    stbtt_GetCodepointBitmapBox(&font, *text, scale, scale, &x0, &y0, &x1, &y1);

    unsigned char *pixel = get_pixel(x_cursor+x0, y+baseline+y0);
    stbtt_MakeCodepointBitmap(&font,
                              pixel,
                              backbuffer->bytes_per_pixel*(x1-x0),
                              backbuffer->bytes_per_pixel*(y1-y0),
                              backbuffer->texture_width*backbuffer->bytes_per_pixel,
                              2*scale, scale,
                              *text);

    x_cursor += (advance*scale);
    if (*(text+1))
      x_cursor += (scale*stbtt_GetCodepointKernAdvance(&font, *text, *(text+1)));

    text++;
  }
}
