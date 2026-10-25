#include "game.h"

void slide(long dx, long wall_flag)
{
    long i;

    for (i = 0; i < 2; ++i) {
        get_sprite_block(zelda_x + dx, zelda_y, &jump_info);
        if (jump_info.floor || jump_info.climb_a || jump_info.climb_b || jump_info.deadly || jump_info.track || *(long *)wall_flag)
            set_link_position(dx);
        else {
            get_sprite_block(zelda_x + dx, zelda_y + 1, &jump_info);
            if (jump_info.floor || jump_info.climb_a || jump_info.climb_b || jump_info.deadly || jump_info.track || *(long *)wall_flag) {
                set_link_position(dx);
                ++zelda_y;
                ++zelda_screen_y;
                ++zelda_sprite_y;
            } else {
                get_sprite_block(zelda_x, zelda_y + 1, &jump_info);
                if (*(long *)wall_flag) {
                    ++zelda_y;
                    ++zelda_screen_y;
                    ++zelda_sprite_y;
                } else {
                    set_link_position(dx);
                    ++zelda_y;
                    ++zelda_screen_y;
                    ++zelda_sprite_y;
                }
            }
        }
        zelda_anim_step = 0;
        if (jump_info.floor || jump_info.track
                || jump_info.climb_a && (old_get_sprite_block(zelda_x, zelda_y + 1) != 48
                                    || old_get_sprite_block(zelda_x, zelda_y - 1) != 48)
                || jump_info.climb_b && (old_get_sprite_block(zelda_x, zelda_y + 1) != 25
                                    || old_get_sprite_block(zelda_x, zelda_y - 1) != 25)) {
            if (!zelda_thrusting) {
                go_find_hero_action(0);
                anim_frame_idx = 0;
            }
            break;
        }
    }
}
