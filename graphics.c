#include "graphics.h"

//Point zero_point = { 0,0 };
const u8 xsize = 5, ysize = 7;
void render_char(char letter, Bitmap * target, u8 scale, rgbacolor fg, rgbacolor bg, Point *offset_pos) {

    u8 * font = font_5_7_ascii(letter);
    for (u8 y = 0; y < ysize; y++) {
        for (u8 x = 0; x < xsize; x++) {

            rgbacolor clr = font[x] & 0x80 >> (7 - y) ? fg : bg;

            scale_int(target, scale, offset_pos->x + x, offset_pos->y + y, clr);
        }
        //Fill spacing between letter
        scale_int(target, scale, offset_pos->x + 5, offset_pos->y + y, bg);


    }
}

void render_line(char * text, u16 width, Bitmap * target, u8 scale, rgbacolor fg, rgbacolor bg, Point *offset_pos) {
    for (u8 j = 0; j < width; j++) {
        char c = text[j];
        // u8 * font = font_5_7_ascii(c);

        render_char(c, target, scale, fg, bg, offset_pos);
        //Fill spacing between
        for (int x = 0; x < xsize + 1; x++) { scale_int(target, scale, offset_pos->x + x, offset_pos->y + 7, bg); }

        offset_pos->x += 6;
    }
}

void bitmap_render_lcd(LCD *lcd, Bitmap * target, u8 scale, rgbacolor fg, rgbacolor bg, Point *offset_pos) {


    Point op = { offset_pos->x, offset_pos->y };
    for (u8 i = 0; i < lcd->height; i++) {
        render_line(&lcd->text[get_lcd_index(lcd, 0, i)], lcd->width, target, scale, fg, bg, &op);

        op.x = offset_pos->x;
        op.y += 8;
    }
}

void fill_rect(Bitmap * target, Point * from, Point * to, rgbacolor fill_color) {
    if (to->x > target->width) to->x = target->width;
    if (to->y > target->height) to->y = target->height;
    for (u16 sy = from->y; sy < to->y; sy++) {
        for (u16 sx = from->x; sx < to->x; sx++) {
            pix_set(target, sx, sy, fill_color);
        }
    }
}
