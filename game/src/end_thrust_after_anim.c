#include "game.h"

void end_thrust_after_anim(void)
{
    if (hero_actions[hero_action].id == 7 || hero_actions[hero_action].id == 10
        || hero_actions[hero_action].id == 6) {
        if (anim_frame_idx == 1 && thrust_lunged) {
            if (zelda_dir == 2)
                zelda_sprite_x -= 4;
            else
                zelda_sprite_x += 4;
        }
        end_thrust();
    }
}
