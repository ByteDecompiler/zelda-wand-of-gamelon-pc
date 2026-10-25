#include "game.h"

void dim_lantern(void)
{
    lantern_pos -= 1;
    if (lantern_pos < 0)
        lantern_pos = 0;
    if (lantern_pos > 31 && lantern_pos <= lantern_size)
        lantern_level = 63;
    else if (lantern_pos == 0)
        lantern_level = 0;
    else if (lantern_pos > lantern_size)
        lantern_level = 63 - (lantern_pos - lantern_size) * 2;
    else
        lantern_level = lantern_pos * 2;
    lantern_level2 = lantern_level;
}
