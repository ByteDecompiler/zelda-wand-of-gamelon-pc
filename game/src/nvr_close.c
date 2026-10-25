#include "game.h"

long nvr_close(register struct file_pos *nvr_file)
{
    if (nvr_file == 0) {
        errno = 0x3002;
        return -1;
    }
    nvr_file->sum = (nvr_file->sum & 0xc000) | (nvr_checksum(nvr_file) & 0x3fff);
    if (lseek(nvr_file->path, 0, 0) != -1) {
        write(nvr_file->path, (char *)&nvr_file->sum, 2);
        if (close(nvr_file->path) != -1)
            return 0;
    }
    return -1;
}
