#include "game.h"

void set_link_position(long dx)
{
    zelda_x += dx;
    zelda_screen_x += dx;
    zelda_sprite_x += dx;
    zelda_moved = 1;
}
