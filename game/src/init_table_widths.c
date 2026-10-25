#include "game.h"

void init_table_widths(void)
{
    long i;

    for (i = 0; i < 240; ++i)
        line_table[i] = i * 416;
}
