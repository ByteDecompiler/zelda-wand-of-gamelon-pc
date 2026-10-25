#include "game.h"

extern char jump_falling;
extern struct block_info jump_info;
void check_map_while_jump_descent(long dir);

void big_jump_down(void)
{
    zelda_anim_step = 0;
    zelda_idle_timer = 0;
    zelda_dir == 2 ? check_map_while_jump_descent(1) : check_map_while_jump_descent(-1);
    if (jump_falling) {
        jump_step -= 1;
        if (jump_step == -1)
            jump_falling = 0;
    }
    if (!jump_falling) {
        big_jump_active = 0;
        get_sprite_block(zelda_x, zelda_y, &jump_info);
        if (!jump_info.ground && !jump_info.left && !jump_info.right) {
            go_find_hero_action(0);
            anim_frame_idx = 0;
            zelda_moved = 1;
        }
    }
}
