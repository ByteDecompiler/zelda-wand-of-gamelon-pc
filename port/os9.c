#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "pc.h"

extern long game_errno;

enum { P_FREE, P_VIDEO, P_AUDIO, P_CD, P_RTR, P_NVR };
#define MAX_PATHS 32
static struct path {
    int kind;
    char name[64];
    u32 first, size;
    u32 pos;
    FILE *f;
} paths[MAX_PATHS];

#define E_PNNF 216
#define E_BPNUM 201

static const char *basename_of(const char *name)
{
    const char *b = strrchr(name, '/');
    return b ? b + 1 : name;
}

static char *alloc_string(const char *s)
{
    char *p = mem_alloc((u32)strlen(s) + 1, 0);
    strcpy(p, s);
    return p;
}

static int new_path(int kind, const char *name)
{
    for (int i = 3; i < MAX_PATHS; i++)
        if (paths[i].kind == P_FREE) {
            memset(&paths[i], 0, sizeof paths[i]);
            paths[i].kind = kind;
            snprintf(paths[i].name, sizeof paths[i].name, "%s", name);
            return i;
        }
    return -1;
}

struct path *path_get(long n)
{
    return n >= 0 && n < MAX_PATHS && paths[n].kind != P_FREE ? &paths[n] : NULL;
}

int path_rtr(long n, u32 *first, u32 *size, u32 *pos)
{
    struct path *p = path_get(n);
    if (!p || p->kind != P_RTR)
        return 0;
    *first = p->first;
    *size = p->size;
    *pos = p->pos;
    return 1;
}

static void save_path(const char *name, char *out, int n)
{
    CreateDirectoryA("saves", NULL);
    snprintf(out, n, "saves\\%s", basename_of(name));
}

long port_open(char *name, long mode)
{
    (void)mode;
    const char *b = basename_of(name);
    int n;
    if (strcmp(name, "/video") == 0 || strcmp(name, "/vid") == 0)
        return new_path(P_VIDEO, name);
    if (strcmp(name, "/audio") == 0 || strcmp(name, "/aud") == 0)
        return new_path(P_AUDIO, name);
    if (strcmp(name, "/cd") == 0 || strcmp(name, "/cd@") == 0 || strcmp(name, "cd@") == 0)
        return new_path(P_CD, name);
    if (strncmp(name, "/nvr/", 5) == 0) {
        char file[MAX_PATH];
        save_path(name, file, sizeof file);
        FILE *f = fopen(file, (mode & 2) ? "r+b" : "rb");
        if (!f) {
            game_errno = E_PNNF;
            return -1;
        }
        n = new_path(P_NVR, name);
        paths[n].f = f;
        return n;
    }
    if (strcmp(name, "/nvr") == 0 || strcmp(name, "/nvr@") == 0)
        return new_path(P_NVR, name);
    u32 first, size;
    if (disc_file(b, &first, &size)) {
        n = new_path(P_RTR, name);
        paths[n].first = first;
        paths[n].size = size;
        return n;
    }
    LOG("open: no %s\n", name);
    game_errno = E_PNNF;
    return -1;
}

long port_create(char *name, long mode, long attr, long size)
{
    (void)mode; (void)attr; (void)size;
    char file[MAX_PATH];
    save_path(name, file, sizeof file);
    FILE *f = fopen(file, "w+b");
    if (!f) {
        game_errno = E_PNNF;
        return -1;
    }
    int n = new_path(P_NVR, name);
    paths[n].f = f;
    return n;
}

long port_unlink(char *name)
{
    char file[MAX_PATH];
    save_path(name, file, sizeof file);
    if (remove(file) != 0) {
        game_errno = E_PNNF;
        return -1;
    }
    return 0;
}

long port_close(long n)
{
    struct path *p = path_get(n);
    if (!p) {
        game_errno = E_BPNUM;
        return -1;
    }
    if (p->f)
        fclose(p->f);
    p->kind = P_FREE;
    return 0;
}

long port_read(long n, char *buf, long count)
{
    struct path *p = path_get(n);
    if (!p) {
        game_errno = E_BPNUM;
        return -1;
    }
    if (p->kind == P_NVR && p->f)
        return (long)fread(buf, 1, count, p->f);
    if (p->kind == P_RTR) {
        u8 raw[SECTOR_RAW];
        long done = 0;
        while (done < count && p->pos < p->size) {
            u32 sec = p->pos / 2048, off = p->pos % 2048;
            if (!disc_read(p->first + sec, raw))
                break;
            long n2 = 2048 - off;
            if (n2 > count - done)
                n2 = count - done;
            memcpy(buf + done, raw + 24 + off, n2);
            done += n2;
            p->pos += n2;
        }
        return done;
    }
    return 0;
}

long port_write(long n, char *buf, long count)
{
    struct path *p = path_get(n);
    if (!p) {
        game_errno = E_BPNUM;
        return -1;
    }
    if (p->kind == P_NVR && p->f) {
        long w = (long)fwrite(buf, 1, count, p->f);
        fflush(p->f);
        return w;
    }
    return count;
}

long port_lseek(long n, long pos, long mode)
{
    struct path *p = path_get(n);
    if (!p) {
        game_errno = E_BPNUM;
        return -1;
    }
    if (p->kind == P_NVR && p->f) {
        fseek(p->f, pos, mode == 1 ? SEEK_CUR : mode == 2 ? SEEK_END : SEEK_SET);
        return ftell(p->f);
    }
    if (mode == 1)
        pos += p->pos;
    else if (mode == 2)
        pos += p->size;
    p->pos = (u32)pos;
    return pos;
}

long getstat_free_space(long n)
{
    (void)n;
    return 7000;
}

long port_strlen(char *s) { return (long)strlen(s); }
long port_strcmp(char *a, char *b) { return strcmp(a, b); }
void port_strcpy(char *d, char *s) { strcpy(d, s); }
void port_memset(char *buf, long v, long n) { memset(buf, (int)v, n); }
void port_memmove(char *d, char *s, long n) { memmove(d, s, n); }

long port_findstr(long pos, char *string, char *pattern)
{
    if (pos < 1)
        pos = 1;
    if ((size_t)pos > strlen(string) + 1)
        return 0;
    char *at = strstr(string + pos - 1, pattern);
    return at ? (long)(at - string) + 1 : 0;
}

char *port_srqcmem(long size, long color) { return mem_alloc((u32)size, (int)color); }
void port_free(char *p) { mem_free(p); }
long port_srtmem(long size, char *p) { (void)size; mem_free(p); return 0; }

char *logo_module(void);
long logo_play(void);

char *port_modcload(char *name, long mode, long color) { (void)name; (void)mode; (void)color; return logo_module(); }
char *port_modlink(char *name, long mode) { (void)name; (void)mode; return logo_module(); }
long port_munlink(char *m) { (void)m; return 0; }
long port_os9exec(char *a, long b, long *c, long d, long e, long f, long g) { return logo_play(); }
long port_os9fork(void) { return -1; }
void port_wait(long *status) { if (status) *status = 0; }

long port_getpid(void) { return 2; }
void port_kill(long pid, long sig) { (void)pid; signal_send((int)sig); }
void port_exit(long code) { ExitProcess((UINT)code); }

void port_sysdate(long format, long *t, long *date, short *day, long *tick)
{
    (void)format;
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    if (t)
        *t = tm->tm_hour << 16 | tm->tm_min << 8 | tm->tm_sec;
    if (date)
        *date = (tm->tm_year + 1900) << 16 | (tm->tm_mon + 1) << 8 | tm->tm_mday;
    if (day)
        *day = (short)tm->tm_wday;
    if (tick)
        *tick = (long)(GetTickCount() % 100);
}

static struct { int used; int sig; double period, left; } alarms[8];

long port_alm_cycle(long sig, long when)
{
    double period = (when & 0x80000000) ? (when & 0x7fffffff) / 256.0 : when / 100.0;
    for (int i = 0; i < 8; i++)
        if (!alarms[i].used) {
            alarms[i].used = 1;
            alarms[i].sig = (int)sig;
            alarms[i].period = alarms[i].left = period > 0 ? period : 1 / 60.0;
            return i + 1;
        }
    return -1;
}

void port_alm_delete(long id)
{
    if (id >= 1 && id <= 8)
        alarms[id - 1].used = 0;
}

void os9_tick(double dt)
{
    for (int i = 0; i < 8; i++)
        if (alarms[i].used) {
            alarms[i].left -= dt;
            while (alarms[i].left <= 0) {
                signal_send(alarms[i].sig);
                alarms[i].left += alarms[i].period;
            }
        }
}

char *port_csd_devname(long type, long n)
{
    (void)n;
    switch (type) {
    case 2: return alloc_string("/audio");
    case 3: return alloc_string("/video");
    default: return alloc_string("/cd");
    }
}

char *port_csd_devparam(char *name)
{
    (void)name;
    return alloc_string("");
}

void cd_setstat_39(long path) { (void)path; }

const char *path_name(long n)
{
    struct path *p = path_get(n);
    return p ? p->name : "";
}

void state_add(void *p, u32 size);

void os9_state(void)
{
    state_add(paths, sizeof paths);
    state_add(alarms, sizeof alarms);
}

void os9_after_load(void)
{
    for (int i = 0; i < MAX_PATHS; i++)
        if (paths[i].kind == P_NVR) {
            paths[i].f = NULL;
            paths[i].kind = P_FREE;
        }
}
