#include "game.h"

long use_item(long item)
{
    if (item_counts[item] <= 0)
        return 0;
    item_counts[item] -= 1;
    return 1;
}
