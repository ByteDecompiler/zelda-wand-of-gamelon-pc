#include "game.h"

void font_draw(long x, long y, char *text, long color)
{
    long saved_target;

    saved_target = draw_target;
    draw_target = color;
    font_draw_glyphs(x, y, text);
    draw_target = saved_target;
}
