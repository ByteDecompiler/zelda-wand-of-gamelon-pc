#include "game.h"

void move_map(void)
{
    long lct[240];
    long *lct_out;
    long addr;
    long i;

    lct_out = lct;
    addr = 0x40000000 | (plane_b_buf0 & 0xffffff);
    addr += map_y * 704;
    addr += map_x;
    for (i = 0; i < 240; ++i) {
        *lct_out++ = addr;
        addr += 704;
    }
    wait_for_line_event();
    flip_world_lcts(lct);
}
