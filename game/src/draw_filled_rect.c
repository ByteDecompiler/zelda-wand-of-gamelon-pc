#include "game.h"

void draw_filled_rect(long x, long y, long x_end, long y_end, long buf_base, unsigned char color)
{
    register long *dst_ptr;
    register long col;
    register long fill_value;
    register long row;

    dst_ptr = (long *)(buf_base + x + y * 416);
    fill_value = color << 24 | color << 16 | color << 8 | color;
    for (row = y; row <= y_end; ++row) {
        for (col = x; col <= x_end; col += 4)
            *dst_ptr++ = fill_value;
        dst_ptr += (416 - x_end + x) / 4 - 1;
    }
}
