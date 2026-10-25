#include "game.h"

long init_sprite_macros(void)
{
    long i;
    register long *hdr;
    register long *macro_offsets;
    char *base;

    for (i = 0; i < 50; ++i)
        macro_ptrs[i] = 0;
    hdr = (long *)macro_data;
    macro_offsets = (long *)macro_data + 4;
    base = macro_data + hdr[2];
    for (i = 0; i < hdr[3]; ++i)
        macro_ptrs[i] = (long)(base + *macro_offsets++ * 2);
    return 0;
}
