#include "game.h"

void color_font_draw(long x, long y, char *text, long dest_buf, long color)
{
    long saved_target;

    saved_target = draw_target;
    draw_target = dest_buf;
    color_font_draw_glyphs(x, y, text, color);
    draw_target = saved_target;
}
