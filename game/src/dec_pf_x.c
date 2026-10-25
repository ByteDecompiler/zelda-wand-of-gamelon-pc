#include "game.h"

void dec_pf_x(void)
{
    long i;

    pf_x -= 1;
    pf_x_px -= 4;
    for (i = 0; i < 240; ++i)
        lct_b[i] -= 4;
}
