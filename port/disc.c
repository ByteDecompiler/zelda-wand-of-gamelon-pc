#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"

#define MAX_FILES 64
static struct image {
    FILE *f;
    long long sectors;
    struct { char name[32]; u32 first, size; } files[MAX_FILES];
    int file_count;
} images[2];
static struct image *cur = &images[0];

#define disc (cur->f)
#define disc_sectors (cur->sectors)
#define files (cur->files)
#define file_count (cur->file_count)

void disc_select(int which)
{
    cur = &images[which];
}

int disc_read(u32 lba, u8 *raw)
{
    if (!disc || lba >= disc_sectors)
        return 0;
    _fseeki64(disc, (long long)lba * SECTOR_RAW, SEEK_SET);
    return fread(raw, 1, SECTOR_RAW, disc) == SECTOR_RAW;
}

void disc_parse(const u8 *raw, struct sector *s)
{
    s->file = raw[16];
    s->channel = raw[17];
    s->submode = raw[18];
    s->coding = raw[19];
    s->form2 = (s->submode & 0x20) != 0;
    s->eor = (s->submode & 0x01) != 0;
    s->video = (s->submode & 0x02) != 0;
    s->audio = (s->submode & 0x04) != 0;
    s->data = (s->submode & 0x08) != 0;
    s->eof = (s->submode & 0x80) != 0;
    s->payload = raw + 24;
    s->size = s->form2 ? 2324 : 2048;
}

int disc_open(const char *path)
{
    u8 raw[SECTOR_RAW];
    if (disc)
        fclose(disc);
    file_count = 0;
    disc = fopen(path, "rb");
    if (!disc)
        return 0;
    _fseeki64(disc, 0, SEEK_END);
    disc_sectors = _ftelli64(disc) / SECTOR_RAW;
    if (!disc_read(16, raw) || memcmp(raw + 24 + 1, "CD-I ", 5) != 0)
        return 0;
    u32 path_table = be32(raw + 24 + 148);
    if (!disc_read(path_table, raw))
        return 0;
    u32 root = be32(raw + 24 + 2);
    if (!disc_read(root, raw))
        return 0;
    const u8 *d = raw + 24;
    for (int k = 0; k < 2048 && d[k] && file_count < MAX_FILES; k += d[k]) {
        int len = d[k + 32];
        if (len == 1 && (d[k + 33] == 0 || d[k + 33] == 1))
            continue;
        if (len > 31)
            len = 31;
        memcpy(files[file_count].name, d + k + 33, len);
        files[file_count].name[len] = 0;
        files[file_count].first = be32(d + k + 6);
        files[file_count].size = be32(d + k + 14);
        file_count++;
    }
    return file_count > 0;
}

int disc_file(const char *name, u32 *first, u32 *size)
{
    for (int i = 0; i < file_count; i++)
        if (strcmp(files[i].name, name) == 0) {
            *first = files[i].first;
            *size = files[i].size;
            return 1;
        }
    return 0;
}

u8 *disc_load_file(const char *name, u32 *size)
{
    u32 first, n;
    u8 raw[SECTOR_RAW];
    if (!disc_file(name, &first, &n))
        return NULL;
    u8 *buf = malloc(n + 2048);
    for (u32 i = 0; i * 2048 < n; i++) {
        if (!disc_read(first + i, raw))
            break;
        memcpy(buf + i * 2048, raw + 24, 2048);
    }
    *size = n;
    return buf;
}

int disc_find(const char *program, char *path, int n)
{
    static const char *const dirs[] = {".", "disc"};
    u32 first, size;
    for (int d = 0; d < 2; d++) {
        char pattern[MAX_PATH];
        WIN32_FIND_DATAA fd;
        snprintf(pattern, sizeof pattern, "%s\\*.bin", dirs[d]);
        HANDLE h = FindFirstFileA(pattern, &fd);
        if (h == INVALID_HANDLE_VALUE)
            continue;
        do {
            snprintf(path, n, "%s\\%s", dirs[d], fd.cFileName);
            if (disc_open(path) && disc_file(program, &first, &size)) {
                FindClose(h);
                return 1;
            }
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    return 0;
}
