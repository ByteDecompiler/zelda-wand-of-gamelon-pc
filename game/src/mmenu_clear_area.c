#include "game.h"

void mmenu_clear_area(void)
{
    long saved;
    long y;

    saved = draw_target;
    draw_target = plane_a_buf0;
    for (y = font_height + 25; y <= 0xd2; ++y)
        draw_line(0x42, y, 0x13e, y, 0x14);
    draw_target = saved;
}
