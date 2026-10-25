#include "game.h"

void kill_sprite(long sprite_no)
{
    sprite_ptrs[sprite_no] = 0;
    sprite_killed[sprite_no] = 1;
    if (sprite_no == 0)
        hero_killed = 1;
}
