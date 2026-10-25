#include "game.h"

void fade_out_plane_A(void)
{
    long level;

    for (level = 63; level >= 0; level -= 1) {
        set_icf(lct_plane_a1, 0, level);
        set_icf(lct_plane_a2, 0, level);
    }
}
