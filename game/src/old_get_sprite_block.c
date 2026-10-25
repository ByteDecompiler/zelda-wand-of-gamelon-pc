#include "game.h"

long old_get_sprite_block(long x, long y)
{
    return block_map[x + y * map_width];
}
