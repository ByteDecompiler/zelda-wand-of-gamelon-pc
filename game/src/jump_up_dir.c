#include "game.h"

void jump_up_dir(long dx)
{
    long i;

    go_find_hero_action(5);
    if (jump_step < 2)
        i = 0;
    else
        i = 1;
    while (i++ < 2 && jump_rising) {
        get_sprite_block(zelda_x + dx, zelda_y, &jump_info);
        if (dx > 0 && jump_info.left || dx < 0 && jump_info.right) {
            set_link_position(dx);
            jump_rising = 0;
            big_jump_active = 0;
            anim_frame_idx = 1;
        } else if (jump_info_solid) {
            jump_rising = 0;
            jump_falling = 1;
            anim_frame_idx = 1;
        } else
            set_link_position_and_dir(dx);
    }
}
