#include "game.h"

void init_sprites(void)
{
    long i;

    for (i = 0; i < 21; ++i) {
        sprite_ptrs[i] = 0;
        sprite_drawn[i] = 0;
        sprite_asleep[i] = 0;
        sprite_draw_xs[i] = 0;
        sprite_xs[i] = 0;
        sprite_draw_ys[i] = 0;
        sprite_ys[i] = 0;
        sprite_gone[i] = 0;
    }
}
