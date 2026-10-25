#include "game.h"

void fade_up_plane_A(void)
{
    long level;

    for (level = 0; level <= 63; ++level) {
        set_icf(lct_plane_a1, 0, level);
        set_icf(lct_plane_a2, 0, level);
    }
}
