#include "game.h"

void init_bomb(long x, long y)
{
    add_to_playfield_tools(1, 6, x, y, 6, 32);
    bomb_placed = 1;
}
