#include "game.h"

char *nvr_alloc_mem(long mem_size, long mem_color)
{
    char *mem_ptr;

    if ((mem_ptr = srqcmem(mem_size, mem_color)) == (char *)-1)
        return 0;
    return mem_ptr;
}
