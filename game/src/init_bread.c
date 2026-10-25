#include "game.h"

void init_bread(long x, long y)
{
    add_to_playfield_tools(1, 18, x, y, 18, 0xf0);
    bread_timer = 0xf0;
    bread_x = x;
    bread_y = y;
}
