#include "game.h"

long check_for_rope_path(long x, long y)
{
    long block_type;
    long i;

    if ((block_type = old_get_sprite_block(x, y)) == 28) {
        rope_out = 2;
        return 1;
    }
    i = x;
    while (block_type == 48 || block_type == 14 || block_type == 31 || block_type == 8 || block_type == 5 || block_type == 25)
        if ((block_type = old_get_sprite_block(--i, y)) == 28) {
            rope_out = 2;
            return 1;
        }
    i = x;
    block_type = old_get_sprite_block(x, y);
    while (block_type == 48 || block_type == 14 || block_type == 31 || block_type == 8 || block_type == 5 || block_type == 25)
        if ((block_type = old_get_sprite_block(++i, y)) == 28) {
            rope_out = 2;
            return 1;
        }
    return 0;
}
