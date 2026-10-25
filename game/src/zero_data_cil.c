#include "game.h"

void zero_data_cil(void)
{
    long i;

    for (i = 0; i < 32; ++i)
        data_cil[i] = 0;
}
