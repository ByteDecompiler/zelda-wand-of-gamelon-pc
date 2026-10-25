#include "game.h"

void init_random(void)
{
    long time, date, tick;
    short day;

    sysdate(0, &time, &date, &day, &tick);
    random_seed = time / 1000 * 1000 + 421;
}
