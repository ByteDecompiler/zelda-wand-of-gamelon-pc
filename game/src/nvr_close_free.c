#include "game.h"

long nvr_close_free(register struct file_pos *file_rec)
{
    if (nvr_close(file_rec) == 0) {
        nvr_free_mem(0x12, (char *)file_rec);
        return 0;
    }
    return -1;
}
