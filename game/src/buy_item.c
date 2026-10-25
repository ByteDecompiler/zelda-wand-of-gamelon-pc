#include "game.h"

long buy_item(long item, long cost, long max)
{
    if (item_counts[item] == max)
        return 3;
    if (rubies < cost)
        return 4;
    item_counts[item] += 3;
    if (item_counts[item] > max)
        item_counts[item] = max;
    rubies -= cost;
    switch (item) {
    case 7:
        add_fake_bombs();
        break;
    case 8:
        add_fake_ropes();
        break;
    case 22:
        add_fake_oil();
        break;
    }
    return 2;
}
