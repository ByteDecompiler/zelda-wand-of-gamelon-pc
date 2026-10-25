#include "game.h"

void kill_sprite_noflash(long sprite_idx)
{
    sprite_ptrs[sprite_idx] = 0;
    sprite_gone[sprite_idx] = 1;
}
