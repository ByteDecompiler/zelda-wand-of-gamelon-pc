#include "game.h"

long randomize(void)
{
    register long result;

    result = ((random_seed = (random_seed * 415821 + 1) % 65536) >> 2) % 65536;
}
