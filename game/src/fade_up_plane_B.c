#include "game.h"

void fade_up_plane_B(void)
{
    long level;

    for (level = 0; level <= 63; ++level) {
        set_icf(lct_plane_b1, 1, level);
        set_icf(lct_plane_b2, 1, level);
    }
}
