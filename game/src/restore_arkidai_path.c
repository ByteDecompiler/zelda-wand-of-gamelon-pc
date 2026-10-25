#include "game.h"

void restore_arkidai_path(long sprite_no)
{
    macros[sprite_no]->state == 2
        ? restore_sprite_blocks(sprite_pos[sprite_no][2] - 8, sprite_pos[sprite_no][3] - 1, 12, &((char *)path_blocks)[sprite_no * 18])
        : restore_sprite_blocks(sprite_pos[sprite_no][2] - 3, sprite_pos[sprite_no][3] - 1, 12, &((char *)path_blocks)[sprite_no * 18]);
}
