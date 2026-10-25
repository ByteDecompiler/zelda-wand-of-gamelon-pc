#include "game.h"

struct file_pos *nvr_create(register char *name, char *str_a, char *str_b, long extra, long mode)
{
    long total;
    char buf_a[32];
    char buf_b[40];
    long len_a;
    long len_b;
    register long path;
    register struct file_pos *fpos;

    len_a = strlen(str_a);
    if (len_a > 32)
        len_a = 32;
    len_b = strlen(str_b);
    if (len_b > 40)
        len_b = 40;
    len_a = nvr_pack_name(str_a, buf_a, len_a, nvr_key, 4);
    len_b = nvr_pack_name(str_b, buf_b, len_b, nvr_key, 4);
    total = len_a + 8 + len_b + extra;
    if (nvr_check_name(name))
        path = create(name, 3, 0x5b, total);
    else
        return 0;
    if (path == -1)
        goto fail;
    if ((fpos = (struct file_pos *)nvr_alloc_mem(0x12, mode)) == 0)
        goto close;
    fpos->host_off = 8;
    fpos->info_off = fpos->host_off + len_a;
    fpos->data_off = fpos->info_off + len_b;
    fpos->sum = 0;
    if (write(path, (char *)&fpos->sum, 8) == -1 || write(path, buf_a, len_a) == -1
        || write(path, buf_b, len_b) == -1)
        goto free;
    fpos->path = path;
    fpos->color = mode;
    fpos->pos = fpos->data_off;
    return fpos;
free:
    nvr_free_mem(0x12, (char *)fpos);
close:
    close(path);
fail:
    errno = 0x3004;
    return 0;
}
