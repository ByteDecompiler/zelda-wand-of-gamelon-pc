#include "game.h"

void main_init(long sprite_no, struct macro_def *def)
{
    (&damage_taken)[sprite_no] = 0;
    sprite_pos[sprite_no][2] = def->x;
    sprite_pos[sprite_no][3] = def->y;
    macros[sprite_no]->step_timer = macros[sprite_no]->step_delay;
    macros[sprite_no]->alt_active = 0;
    (&anim_frame_idx)[sprite_no] = 0;
    macros[sprite_no]->animate = 1;
    switch (def->type) {
    case 1:
        init_type_1(sprite_no, def);
        break;
    case 2:
        init_type_2(sprite_no, def);
        break;
    case 3:
        init_type_3(sprite_no, def);
        break;
    case 4:
        init_type_4(sprite_no, def);
        break;
    case 5:
        init_type_5(sprite_no, def);
        break;
    case 7:
        init_type_7(sprite_no, def);
        break;
    case 8:
        init_type_8(sprite_no, def);
        break;
    }
}
