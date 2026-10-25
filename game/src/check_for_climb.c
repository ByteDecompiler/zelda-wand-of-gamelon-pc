#include "game.h"

void check_for_climb(long dy)
{
    get_sprite_block(zelda_x, zelda_y, &jump_info);
    if (jump_info.climb_a && old_get_sprite_block(zelda_x, zelda_y + dy) == 48) {
        climbing_blk48 = 1;
        return;
    }
    if (jump_info.climb_b && old_get_sprite_block(zelda_x, zelda_y + dy) == 25) {
        climbing_blk25 = 1;
        return;
    }
    if (dy == 1 && jump_info.f14 || dy == -1 && jump_info.f12) {
        while (!jump_info.climb_a && !jump_info.climb_b) {
            ++zelda_x;
            ++zelda_screen_x;
            ++zelda_sprite_x;
            get_sprite_block(zelda_x, zelda_y, &jump_info);
        }
        if (jump_info.climb_a)
            climbing_blk48 = 1;
        if (jump_info.climb_b)
            climbing_blk25 = 1;
    } else if (dy == 1 && jump_info.f15 || dy == -1 && jump_info.f13) {
        while (!jump_info.climb_a && !jump_info.climb_b) {
            --zelda_x;
            --zelda_screen_x;
            --zelda_sprite_x;
            get_sprite_block(zelda_x, zelda_y, &jump_info);
        }
        if (jump_info.climb_a)
            climbing_blk48 = 1;
        if (jump_info.climb_b)
            climbing_blk25 = 1;
    }
}
