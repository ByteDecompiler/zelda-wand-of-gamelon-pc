#include "game.h"

char *init_error_buffer(long buf_index)
{
    register long *len_ptr;
    register short *short_ptr;
    long off;

    off = buf_index * 30;
    len_ptr = (long *)&error_bufs[off];
    *len_ptr = 27;
    error_bufs[off + 4] = 0;
    short_ptr = (short *)&error_bufs[off + 6];
    *short_ptr = 0;
    short_ptr = (short *)&error_bufs[off + 8];
    *short_ptr = 0;
    error_bufs[off + 26] = 0;
    return &error_bufs[off];
}
