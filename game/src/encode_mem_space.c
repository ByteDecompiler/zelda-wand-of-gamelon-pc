#include "game.h"

char mem_space[61];

void encode_mem_space(void)
{
    register char *out;
    long i;

    out = mem_space;
    for (i = 0; i < 32; ++i)
        *out++ = experiences[i];
    for (i = 0; i < 21; ++i)
        *out++ = item_counts[i];
    *out++ = (rubies & 0xff00) >> 8;
    *out++ = rubies & 0xff;
    for (i = 22; i <= 27; ++i)
        *out++ = item_counts[i];
}
