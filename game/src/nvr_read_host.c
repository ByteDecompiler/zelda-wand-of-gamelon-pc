#include "game.h"

long nvr_read_host(struct file_pos *fpos, char *out)
{
    char buf[32];
    long name_len;
    long len;

    if (fpos == 0) {
        errno = 0x3002;
        return -1;
    }
    if (lseek(fpos->path, fpos->host_off, 0) != -1 && (name_len = fpos->info_off - fpos->host_off) && read(fpos->path, buf, name_len)) {
        buf[name_len] = 0;
        len = nvr_unpack_name(buf, out, name_len, nvr_key, 4);
        out[len] = 0;
        return 0;
    }
    *out = 0;
    return -1;
}
