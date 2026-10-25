#include "game.h"

void init_sprite_buffer_addresses(void)
{
    long i;
    long base;

    base = sprite_buffer;
    block_ptrs[0] = &jump_info;
    for (i = 0; i < 20; ++i)
        block_ptrs[i + 1] = (struct block_info *)(i * 32 + base);
    base += i * 32;
    macros[0] = (struct macro *)&zelda_dir;
    for (i = 0; i < 20; ++i)
        macros[i + 1] = (struct macro *)(i * 48 + base);
    base += i * 48;
    sprite_pos[0] = &zelda_screen_x;
    for (i = 0; i < 20; ++i)
        sprite_pos[i + 1] = (long *)(i * 16 + base);
    base += i * 16;
    sprite_frames = (long *)base;
}
