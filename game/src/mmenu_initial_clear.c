#include "game.h"

void mmenu_initial_clear(void)
{
    long saved;
    long y;

    clear_416x240(plane_a_buf0);
    saved = draw_target;
    draw_target = plane_a_buf0;
    for (y = 0x15; y <= 0xd4; ++y)
        draw_line(0x35, y, 0x146, y, 0x14);
    draw_target = saved;
}
