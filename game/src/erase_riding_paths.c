#include "game.h"

void erase_riding_paths(void)
{
    long i;

    for (i = 0; i < 21; ++i)
        if (i && sprite_ptrs[i]) {
            if (macros[i]->kind == 5)
                restore_arkidai_path(i);
            else if (macros[i]->kind == 7) {
                restore_vert_blocks(sprite_pos[i][2] - 2, sprite_pos[i][3] - 13, 14, &((char *)path_blocks)[i * 18]);
                restore_vert_blocks(sprite_pos[i][2] + 2, sprite_pos[i][3] - 13, 14, &((char *)path_blocks_r)[i * 18]);
            } else if (macros[i]->kind == 8)
                restore_sprite_blocks(sprite_pos[i][2] - 9, sprite_pos[i][3] - 1, 18, &((char *)path_blocks)[i * 18]);
        }
}
