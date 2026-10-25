#include "game.h"

void icon_to_from_save_buffer(char slot, long where, long to)
{
    long left;
    long right;
    long top;
    long bottom;
    long row;
    long col;
    char *src;
    char *dst;

    if (slot < 5) {
        left = ICON_BOX_X;
        right = left + ICON_BOX_W;
        top = slot * 29 + 46;
        bottom = top + 28;
    } else {
        left = ICON_BOX_X2;
        right = left + ICON_BOX_W;
        top = (slot - 5) * 29 + 46;
        bottom = top + 28;
    }
    if (to) {
        dst = (char *)font_glyph_data;
        src = (char *)(where + left + top * 416);
        for (row = top; row <= bottom; ++row) {
            for (col = left; col <= right; ++col)
                *dst++ = *src++;
            src += 0x19f - right + left;
        }
    } else {
        src = (char *)font_glyph_data;
        dst = (char *)(where + left + top * 416);
        for (row = top; row <= bottom; ++row) {
            for (col = left; col <= right; ++col)
                *dst++ = *src++;
            dst += 0x19f - right + left;
        }
    }
}
