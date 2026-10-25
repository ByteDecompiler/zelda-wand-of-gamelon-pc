#include "game.h"

void set_link_position_and_dir(long dx)
{
    zelda_x += dx;
    zelda_screen_x += dx;
    zelda_moved = 1;
    if (dx < 0) {
        zelda_dir = 6;
        zelda_sprite_x = zelda_screen_x - 9;
    } else {
        zelda_dir = 2;
        zelda_sprite_x = zelda_screen_x - 8;
    }
}
