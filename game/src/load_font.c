#include "game.h"

void load_font(long data)
{
    font_height = 27;
    font_width = 0x1a0;
    font_bitmap_a = data;
    font_bitmap_b = data + 0xc0;
    font_bitmap_c = data + 0x180;
}
