#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"

#define NOP 0x10000000u
#define COLS 8

struct lct {
    int used, plane, lines;
    u32 *ins;
    int *link_lct, *link_line;
};
struct fct {
    int used, plane, length;
    u32 *ins;
    int link_lct, link_line;
};

#define MAX_LCT 32
#define MAX_FCT 8
static struct lct lcts[MAX_LCT];
static struct fct fcts[MAX_FCT];
static int exec_fct[2] = {-1, -1};
static CRITICAL_SECTION video_lock;
static volatile int line_signal = -1;

static u32 clut[256];
static int clut_bank[2];
static u32 icm, tcr, plane_order, backdrop;
static u32 trans_color[2], mask_color[2], dyuv_start[2];
static int weight[2] = {63, 63};
static int file_type[2];

static int cursor_on, cursor_x, cursor_y, cursor_color = 7, cursor_hx, cursor_hy;
static u16 cursor_rows[16];

void state_add(void *p, u32 size);

void video_state(void)
{
#define S(x) state_add((void *)&(x), sizeof(x))
    S(lcts); S(fcts); S(exec_fct); S(line_signal); S(clut); S(clut_bank); S(icm); S(tcr); S(plane_order);
    S(backdrop); S(trans_color); S(mask_color); S(dyuv_start); S(weight); S(file_type);
    S(cursor_on); S(cursor_x); S(cursor_y); S(cursor_color); S(cursor_hx); S(cursor_hy); S(cursor_rows);
#undef S
}

void video_init(void)
{
    InitializeCriticalSection(&video_lock);
}

static void lock(void) { port_enter(); EnterCriticalSection(&video_lock); }
static void unlock(void) { LeaveCriticalSection(&video_lock); port_leave(); }

long dc_crfct(long path, long plane, long length, long attr)
{
    (void)path; (void)attr;
    lock();
    int id = -1;
    for (int i = 1; i < MAX_FCT; i++)
        if (!fcts[i].used) {
            id = i;
            break;
        }
    if (id > 0) {
        struct fct *f = &fcts[id];
        f->used = 1;
        f->plane = (int)plane;
        f->length = (int)length;
        f->ins = mem_alloc((u32)length * 4, 0);
        for (int i = 0; i < length; i++)
            f->ins[i] = NOP;
        f->link_lct = -1;
    }
    unlock();
    return id;
}

long dc_crlct(long path, long plane, long lines, long attr)
{
    (void)path; (void)attr;
    lock();
    int id = -1;
    for (int i = 1; i < MAX_LCT; i++)
        if (!lcts[i].used) {
            id = i;
            break;
        }
    if (id > 0) {
        struct lct *l = &lcts[id];
        l->used = 1;
        l->plane = (int)plane;
        l->lines = (int)lines;
        l->ins = mem_alloc((u32)lines * COLS * 4, 0);
        l->link_lct = mem_alloc((u32)lines * sizeof(int), 0);
        l->link_line = mem_alloc((u32)lines * sizeof(int), 0);
        for (int i = 0; i < lines * COLS; i++)
            l->ins[i] = NOP;
        for (int i = 0; i < lines; i++)
            l->link_lct[i] = -1;
    }
    unlock();
    return id;
}

static struct fct *get_fct(long id) { return id > 0 && id < MAX_FCT && fcts[id].used ? &fcts[id] : NULL; }
static struct lct *get_lct(long id) { return id > 0 && id < MAX_LCT && lcts[id].used ? &lcts[id] : NULL; }

void dc_wrfct(long path, long id, long start, long n, long *table)
{
    (void)path;
    lock();
    struct fct *f = get_fct(id);
    for (long i = 0; f && i < n; i++)
        if (start + i < f->length)
            f->ins[start + i] = (u32)table[i];
    unlock();
}

void dc_wrfi(long path, long id, long index, long instr)
{
    (void)path;
    lock();
    struct fct *f = get_fct(id);
    if (f && index >= 0 && index < f->length)
        f->ins[index] = (u32)instr;
    unlock();
}

void dc_wrli(long path, long id, long line, long col, long instr)
{
    (void)path;
    lock();
    struct lct *l = get_lct(id);
    if (l && line >= 0 && line < l->lines && col >= 0 && col < COLS)
        l->ins[line * COLS + col] = (u32)instr;
    unlock();
    if (((u32)instr >> 24 == 0xdb || (u32)instr >> 24 == 0xdc) && !fast_forward)
        Sleep(1);
}

void dc_wrlct(long path, long id, long line, long col, long nlines, long ncols, long *table)
{
    (void)path;
    lock();
    struct lct *l = get_lct(id);
    for (long y = 0; l && y < nlines; y++)
        for (long x = 0; x < ncols; x++)
            if (line + y < l->lines && col + x < COLS)
                l->ins[(line + y) * COLS + col + x] = (u32)table[y * ncols + x];
    unlock();
}

void dc_rdlct(long path, long id, long line, long col, long nlines, long ncols, long *out)
{
    (void)path;
    lock();
    struct lct *l = get_lct(id);
    for (long y = 0; l && y < nlines; y++)
        for (long x = 0; x < ncols; x++)
            out[y * ncols + x] = line + y < l->lines && col + x < COLS ? (long)l->ins[(line + y) * COLS + col + x] : (long)NOP;
    unlock();
}

void dc_flnk(long path, long fct, long lct, long line)
{
    (void)path;
    lock();
    struct fct *f = get_fct(fct);
    if (f) {
        f->link_lct = (int)lct;
        f->link_line = (int)line;
    }
    unlock();
}

void dc_llnk(long path, long lct, long line, long lct2, long line2)
{
    (void)path;
    lock();
    struct lct *l = get_lct(lct);
    if (l && line >= 0 && line < l->lines) {
        l->link_lct[line] = (int)lct2;
        l->link_line[line] = (int)line2;
    }
    unlock();
}

void dc_exec(long path, long fct_a, long fct_b)
{
    (void)path;
    lock();
    exec_fct[0] = (int)fct_a;
    exec_fct[1] = (int)fct_b;
    unlock();
}

void dc_intl(long path, long on) { (void)path; (void)on; }
void dc_set_display(long path, long mode) { (void)path; (void)mode; }

void dc_ssig(long path, long sig, long line)
{
    (void)path; (void)line;
    line_signal = (int)sig;
}

void gc_pos(long path, long x, long y) { (void)path; cursor_x = (int)x; cursor_y = (int)y; }
void gc_show(long path) { (void)path; cursor_on = 1; }
void gc_hide(long path) { (void)path; cursor_on = 0; }
void gc_col(long path, long c) { (void)path; cursor_color = (int)c; }
void gc_blnk(long path, long n) { (void)path; (void)n; }
void gc_org(long path, long x, long y) { (void)path; cursor_hx = (int)x; cursor_hy = (int)y; }

void gc_def(long path, long a, long b, long w, long h, long c, char *data)
{
    (void)path; (void)a; (void)b; (void)w; (void)c;
    for (int i = 0; i < 16 && i < h; i++)
        cursor_rows[i] = be16(data + i * 2);
}

void video_frame(void)
{
    int sig = line_signal;
    if (sig >= 0) {
        line_signal = -1;
        signal_send(sig);
    }
}

static void set_register(int path, u32 reg, u32 v)
{
    if (reg >= 0x80 && reg < 0xc0) {
        clut[(clut_bank[path] & 3) * 64 + (reg - 0x80)] = v & 0xffffff;
        return;
    }
    switch (reg) {
    case 0xc0: icm = v; break;
    case 0xc1: tcr = v; break;
    case 0xc2: plane_order = v & 1; break;
    case 0xc3: clut_bank[path] = v & 3; break;
    case 0xc4: trans_color[0] = v & 0xfcfcfc; break;
    case 0xc6: trans_color[1] = v & 0xfcfcfc; break;
    case 0xc7: mask_color[0] = v & 0xfcfcfc; break;
    case 0xc9: mask_color[1] = v & 0xfcfcfc; break;
    case 0xca: dyuv_start[0] = v; break;
    case 0xcb: dyuv_start[1] = v; break;
    case 0xd8: backdrop = v; break;
    case 0xdb: weight[0] = v & 0x3f; break;
    case 0xdc: weight[1] = v & 0x3f; break;
    default: break;
    }
}

static int run_instruction(int path, u32 ins, u32 *vsr)
{
    u32 op = ins >> 24;
    if (op < 0x10)
        return 1;
    if (op < 0x20)
        return 0;
    if (op >= 0x40 && op < 0x60) {
        *vsr = ins & 0xffffff;
        return op >= 0x50;
    }
    if (op >= 0x78 && op < 0x80) {
        file_type[path] = ins & 0x0f;
        return 0;
    }
    if (op >= 0x80)
        set_register(path, op, ins & 0xffffff);
    return 0;
}

static const int dyuv_delta[16] = {0, 1, 4, 9, 16, 27, 44, 79, 128, 177, 212, 229, 240, 247, 252, 255};

static u32 yuv_rgb(int y, int u, int v)
{
    int r = y + (1371 * (v - 128)) / 1000;
    int g = y - (336 * (u - 128) + 698 * (v - 128)) / 1000;
    int b = y + (1732 * (u - 128)) / 1000;
    r = r < 0 ? 0 : r > 255 ? 255 : r;
    g = g < 0 ? 0 : g > 255 ? 255 : g;
    b = b < 0 ? 0 : b > 255 ? 255 : b;
    return (u32)r << 16 | (u32)g << 8 | (u32)b;
}

static u32 decode_line(int path, u32 vsr, u32 *rgb, u8 *transparent)
{
    int mode = path == 0 ? (icm & 0xf) : (icm >> 8 & 0xf);
    u32 base_index = path ? 0x80 : 0;
    const u8 *src = mem24(vsr);
    int n = 0;
    if (mode == 0) {
        for (int x = 0; x < SCREEN_W; x++)
            rgb[x] = 0, transparent[x] = 1;
        return vsr;
    }
    if (mode == 5) {
        int y = dyuv_start[path] >> 16 & 0xff, u = dyuv_start[path] >> 8 & 0xff, v = dyuv_start[path] & 0xff;
        for (int x = 0; x < SCREEN_W; x += 2) {
            u8 b0 = src[n++], b1 = src[n++];
            u = (u + dyuv_delta[b0 >> 4]) & 0xff;
            y = (y + dyuv_delta[b0 & 15]) & 0xff;
            rgb[x] = yuv_rgb(y, u, v);
            v = (v + dyuv_delta[b1 >> 4]) & 0xff;
            y = (y + dyuv_delta[b1 & 15]) & 0xff;
            rgb[x + 1] = yuv_rgb(y, u, v);
            transparent[x] = transparent[x + 1] = 0;
        }
    } else if (file_type[path] == 2) {
        int x = 0;
        while (x < SCREEN_W) {
            u8 b = src[n++];
            int len = 1;
            if (b & 0x80) {
                len = src[n++];
                if (len == 0)
                    len = SCREEN_W - x;
            }
            u32 idx = mode == 1 ? (b & 0x7f) : (base_index | (b & 0x7f));
            for (int k = 0; k < len && x < SCREEN_W; k++, x++) {
                rgb[x] = clut[idx];
                transparent[x] = 0;
            }
        }
    } else if (mode == 1 || mode == 3 || mode == 4) {
        for (int x = 0; x < SCREEN_W; x++) {
            u8 b = src[n++];
            u32 idx = mode == 1 ? b : (base_index | (b & 0x7f));
            rgb[x] = clut[idx];
            transparent[x] = 0;
        }
    } else if (mode == 0xb) {
        for (int x = 0; x < SCREEN_W; x += 2) {
            u8 b = src[n++];
            rgb[x] = clut[base_index | b >> 4];
            rgb[x + 1] = clut[base_index | (b & 15)];
            transparent[x] = transparent[x + 1] = 0;
        }
    } else {
        for (int x = 0; x < SCREEN_W; x++)
            rgb[x] = 0, transparent[x] = 1;
    }
    u32 cond = (path == 0 ? tcr : tcr >> 8) & 0xf;
    u32 keymask = ~mask_color[path] & 0xfcfcfc;
    u32 key = trans_color[path] & keymask;
    for (int x = 0; x < SCREEN_W; x++) {
        int match = (rgb[x] & keymask) == key;
        int t;
        switch (cond & 7) {
        case 0: t = 1; break;
        case 1: case 5: case 6: t = match; break;
        default: t = 0; break;
        }
        if (cond & 8)
            t = !t;
        transparent[x] |= (u8)t;
    }
    return vsr + n;
}

static u32 plane_rgb[2][SCREEN_H][SCREEN_W];
static u8 plane_tr[2][SCREEN_H][SCREEN_W];
static int plane_weight[2][SCREEN_H];

static void render_plane(int path)
{
    struct fct *f = get_fct(exec_fct[path]);
    if (!f) {
        memset(plane_tr[path], 1, sizeof plane_tr[path]);
        return;
    }
    u32 vsr = 0;
    for (int i = 0; i < f->length; i++)
        if (run_instruction(path, f->ins[i], &vsr))
            break;
    struct lct *l = get_lct(f->link_lct);
    int line = f->link_line;
    int skip = settings.fill ? 2 : 0;
    static u32 spare_rgb[SCREEN_W];
    static u8 spare_tr[SCREEN_W];
    for (int yy = 0; yy < SCREEN_H + skip; yy++) {
        int y = yy - skip;
        for (int hops = 0; l && hops < 8 && line < l->lines && l->link_lct[line] >= 0; hops++) {
            int to = l->link_lct[line];
            line = l->link_line[line];
            l = get_lct(to);
        }
        if (l && line < l->lines) {
            for (int c = 0; c < COLS; c++)
                if (run_instruction(path, l->ins[line * COLS + c], &vsr))
                    break;
            line++;
        }
        if (y < 0) {
            vsr = decode_line(path, vsr, spare_rgb, spare_tr);
            continue;
        }
        plane_weight[path][y] = weight[path];
        vsr = decode_line(path, vsr, plane_rgb[path][y], plane_tr[path][y]);
    }
}

extern volatile int logo_showing;
void logo_frame(u32 *out);

void video_render(u32 *out)
{
    if (logo_showing) {
        logo_frame(out);
        return;
    }
    EnterCriticalSection(&video_lock);
    render_plane(0);
    render_plane(1);
    int front = plane_order ? 1 : 0, back = 1 - front;
    int mix = !(tcr & 0x800000);
    for (int y = 0; y < SCREEN_H; y++) {
        int wf = plane_weight[front][y], wb = plane_weight[back][y];
        for (int x = 0; x < SCREEN_W; x++) {
            int tf = plane_tr[front][y][x], tb = plane_tr[back][y][x];
            u32 cf = tf ? 0 : plane_rgb[front][y][x];
            u32 cb = tb ? 0 : plane_rgb[back][y][x];
            u32 o = 0;
            for (int s = 0; s < 24; s += 8) {
                int a = cf >> s & 0xff, b = cb >> s & 0xff, v;
                if (mix)
                    v = (a * wf + b * wb) / 63;
                else
                    v = tf ? b * wb / 63 : a * wf / 63;
                o |= (u32)(v > 255 ? 255 : v) << s;
            }
            out[y * SCREEN_W + x] = o;
        }
    }
    if (cursor_on) {
        u32 col = 0xf0f0f0;
        for (int r = 0; r < 16; r++)
            for (int c = 0; c < 16; c++)
                if (cursor_rows[r] & (0x8000 >> c)) {
                    int x = cursor_x / 2 - cursor_hx + c, y = cursor_y / 2 - cursor_hy + r;
                    if (x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
                        out[y * SCREEN_W + x] = col;
                }
    }
    LeaveCriticalSection(&video_lock);
}
