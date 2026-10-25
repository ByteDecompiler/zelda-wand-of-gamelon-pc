#include "game.h"

void clear_buffers(void)
{
    long i;

    clear_416x240(plane_a_buf0 + 32);
    for (i = 0; i < 8; ++i)
        *(long *)(plane_a_buf0 + i * 4) = 0;
    clear_416x240(plane_a_buf1);
    clear_384x240(plane_a_data);
    clear_384x240(plane_b_buf0);
    clear_384x240(plane_b_buf1);
    clear_384x240(plane_b_buf2);
}
