#include "game.h"

void lantern_brightness(void)
{
    if (glove_flash == 4 || glove_flash == 2) {
        set_icf(lct_plane_b1, 1, 63);
        set_icf(lct_plane_b2, 1, 63);
    } else {
        set_icf(lct_plane_b1, 1, lantern_level);
        set_icf(lct_plane_b2, 1, lantern_level);
    }
}
