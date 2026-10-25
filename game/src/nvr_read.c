#include "game.h"

long nvr_read(struct file_pos *fpos, char *buf, long nbytes)
{
    long got;

    if (fpos == 0) {
        errno = 0x3002;
        return -1;
    }
    if (lseek(fpos->path, fpos->pos, 0) != -1) {
        got = read(fpos->path, buf, nbytes);
        fpos->pos += got;
        return got;
    }
    errno = 0x3003;
    return -1;
}
