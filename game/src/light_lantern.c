#include "game.h"

void light_lantern(void)
{
    if (item_counts[2]) {
        lantern_size = 0x500;
        lantern_pos = 0x520 - lantern_pos;
    } else if (item_counts[1]) {
        lantern_size = 0x280;
        lantern_pos = 0x2a0 - lantern_pos;
    } else {
        lantern_size = 0x140;
        lantern_pos = 0x160 - lantern_pos;
    }
}
