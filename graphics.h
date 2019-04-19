#pragma once

#include "types.h"
#include "font.h"
inline u8 rgba_red(u32 color) { return color >> 24; }
inline u8 rgba_green(u32 color) { return (color >> 16) & 0xFF; }
inline u8 rgba_blue(u32 color) { return (color >> 8) & 0xFF; }
inline u8 rgba_alpha(u32 color) { return  color & 0xFF; }
inline u32 rgba(u8 red, u8 green, u8 blue) { return (red << 24) | (green << 16) | (blue << 8); }
typedef u32 rgbacolor;


typedef struct Bitmap {
    rgbacolor * pixels;
    u16 width;
    u16 height;
} Bitmap;

inline u32 get_bitmap_index(Bitmap * bitmap, u16 x, u16 y) { return x + (y * bitmap->width); }

inline void pix_set(Bitmap *bitmap, u16 x, u16 y, rgbacolor color) {
    u32 index = get_bitmap_index(bitmap, x, y);
    bitmap->pixels[index] = color;
}
inline void pix_set_rgb(Bitmap *bitmap, u16 x, u16 y, u8 red, u8 green, u8 blue) { pix_set(bitmap, x, y, rgba(red, green, blue)); }

inline void scale_int(Bitmap *pixels, u8 scale, u16 scaled_x, u16 scaled_y, rgbacolor color) {
    for (int sy = 0; sy < scale; sy++) {
        for (int sx = 0; sx < scale; sx++) {
            pix_set(pixels, (scaled_x)*scale + sx, (scaled_y)*scale + sy, color);
        }
    }

}

typedef struct Point {
    u16 x;
    u16 y;
} Point;


typedef struct LCD {
    u8 width; //Number of characters in each row.
    u8 height; //Number of rows.
    char* text; //Lines of text.

} LCD;
inline u32 get_lcd_index(LCD* lcd, u16 x, u16 y) { return x + (y * lcd->width); }

inline void  draw_line(Bitmap *pixels, rgbacolor color, u16 x0, u16 y0, u16 x1, u16 y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        if(pixels->height > y0 && pixels->width > x0)
            pix_set(pixels, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }

}

void render_char(char letter, Bitmap * target, u8 scale, rgbacolor fg, rgbacolor bg, Point *offset_pos);
void render_line(char * text, u16 width, Bitmap * target, u8 scale, rgbacolor fg, rgbacolor bg, Point *offset_pos);
void bitmap_render_lcd(LCD *lcd, Bitmap * target, u8 scale, rgbacolor fg, rgbacolor bg, Point *offset_pos);
void fill_rect(Bitmap * target, Point* from, Point * to, rgbacolor fill_color);