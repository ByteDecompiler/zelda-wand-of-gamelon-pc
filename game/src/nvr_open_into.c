#include "game.h"

struct file_pos *nvr_open_into(char *name, char *want, long mode, struct file_pos *file_rec)
{
    char buf[32];
    long path;

    if (nvr_check_name(name)) {
        if ((path = open(name, 3)) == -1)
            return 0;
    } else {
        errno = 0x3005;
        return 0;
    }
    if (file_rec != 0) {
        file_rec->path = path;
        file_rec->color = mode;
        if (read(path, (char *)&file_rec->sum, 8) != -1) {
            if (strcmp(want, nvr_magic) != 0 && (nvr_read_host(file_rec, buf) == -1 || strcmp(want, buf) != 0)) {
                errno = 0x3001;
                close(path);
                nvr_free_mem(0x12, (char *)file_rec);
                return 0;
            }
            if ((file_rec->sum & 0x3fff) == (nvr_checksum(file_rec) & 0x3fff)) {
                file_rec->pos = file_rec->data_off;
                return file_rec;
            }
        }
    }
    close(path);
    return 0;
}
