#include "game.h"

long use_rubies(long cost)
{
    if (rubies < cost) {
        not_enough_rubies = 1;
        return 0;
    }
    rubies -= cost;
    return 1;
}
