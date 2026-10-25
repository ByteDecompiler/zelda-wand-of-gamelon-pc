#include "game.h"

long nvr_free_mem(long ret_size, char *ret_mem)
{
    long result;

    if ((result = srtmem(ret_size, ret_mem)) == -1) {
    }
    return result;
}
