#include "game.h"

void inc_pf_x(void)
{
    long i;

    pf_x_px += 4;
    pf_x += 1;
    for (i = 0; i < 240; ++i)
        lct_b[i] += 4;
}
