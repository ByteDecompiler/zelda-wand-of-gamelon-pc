#include "game.h"

char attack_state[21];

void check_for_restart(long sprite_no)
{
    if (macros[sprite_no]->lives == 1) {
        macros[sprite_no]->lives -= 1;
        sprite_ptrs[sprite_no] = 0;
    } else {
        if (macros[sprite_no]->lives > 0)
            macros[sprite_no]->lives -= 1;
        attack_state[sprite_no] = 0;
        reinit_sprite(sprite_no);
    }
}
