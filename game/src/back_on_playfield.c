#include "game.h"

void back_on_playfield(void)
{
    long level;

    reinit_lct_A();
    reinit_lct_B();
    if (dark_room) {
        dim_lantern();
        for (level = 0; level <= 63; ++level) {
            set_icf(lct_plane_a1, 0, level);
            set_icf(lct_plane_a2, 0, level);
            if (level <= lantern_level) {
                set_icf(lct_plane_b1, 1, level);
                set_icf(lct_plane_b2, 1, level);
            }
        }
    } else
        fade_up_plane_AB();
}
