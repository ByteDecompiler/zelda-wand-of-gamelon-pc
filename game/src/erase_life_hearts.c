#include "game.h"

long heart_x[11];

void erase_life_hearts(long y)
{
    long i;

    for (i = 0; i < max_hearts; ++i)
        erase_16x16(heart_x[i], heart_w, plane_a_mem + y);
}
