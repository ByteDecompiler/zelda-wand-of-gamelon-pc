#include "game.h"

long init_sword_thrust(void)
{
    if (zelda_thrusting || climbing_blk48 || climbing_blk25 || rope_out)
        return 0;
    zelda_idle_timer = 0;
    attack_damage[0] = 1;
    zelda_anim_step = 0;
    zelda_thrusting = 1;
    thrust_lunge = 1;
    thrust_lunged = 0;
    if (zelda_riding)
        thrust_lunge = 0;
    get_sprite_block(zelda_x, zelda_y, &jump_info);
    if (jump_rising || jump_falling || jump_info_ground || jump_info.left || jump_info.right) {
        go_find_hero_action(7);
        thrust_lunge = 0;
    } else if (zelda_crouching)
        go_find_hero_action(10);
    else
        go_find_hero_action(6);
    anim_frame_idx = 0;
    return 1;
}
