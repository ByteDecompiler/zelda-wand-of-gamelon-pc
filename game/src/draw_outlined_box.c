#include "game.h"

void draw_outlined_box(long where, long x1, long y1, long x2, long y2, long fill, long border)
{
    long saved;

    saved = draw_target;
    draw_target = where;
    draw_line(x1, y1, x2, y1, border);
    draw_line(x2, y1, x2, y2, border);
    draw_line(x2, y2, x1, y2, border);
    draw_line(x1, y2, x1, y1, border);
    ++x1;
    --x2;
    ++y1;
    --y2;
    draw_line(x1, y1, x2, y1, border);
    draw_line(x2, y1, x2, y2, border);
    draw_line(x2, y2, x1, y2, border);
    draw_line(x1, y2, x1, y1, border);
    ++x1;
    --x2;
    ++y1;
    --y2;
    for (; y1 <= y2; ++y1)
        draw_line(x1, y1, x2, y1, fill);
    draw_target = saved;
}
