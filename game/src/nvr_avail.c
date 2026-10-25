#include "game.h"

long nvr_avail(void)
{
    long path, result;

    path = open(nvr_name, 0x81);
    result = getstat_free_space(path);
    close(path);
    return result;
}
