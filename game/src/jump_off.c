#include "game.h"

void jump_off(void)
{
    jump_wait_timer = 10;
    go_find_hero_action(5);
    zelda_anim_step = 0;
    jump_rising = 1;
    jump_step = 0;
    anim_frame_idx = 0;
    zelda_y -= 1;
    zelda_screen_y -= 1;
    zelda_sprite_y -= 1;
    climbing_blk48 = 0;
    climbing_blk25 = 0;
}
