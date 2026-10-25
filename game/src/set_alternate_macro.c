#include "game.h"

char alt_repeat[21];
unsigned char alt_frame_base[21];

long set_alternate_macro(long sprite_idx, long which, long repeat, long state, long speed)
{
    if (!repeat)
        return 1;
    if (!(alt_macro_lists[sprite_idx] = (unsigned short *)macro_ptrs[which - 1]))
        return 1;
    alt_index[sprite_idx] = -1;
    alt_repeat[sprite_idx] = repeat;
    if (state == 10)
        state = macros[sprite_idx]->state;
    if (state == 2)
        alt_frame_base[sprite_idx] = 0;
    else if (state == 6)
        alt_frame_base[sprite_idx] = 3;
    macros[sprite_idx]->alt_active = 1;
    if (speed < 1)
        speed = 1;
    if (speed > 8)
        speed = 8;
    macros[sprite_idx]->alt_step_delay = speed;
    macros[sprite_idx]->step_timer = speed;
    return 0;
}
