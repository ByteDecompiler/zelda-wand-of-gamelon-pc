#include <string.h>
#include "pc.h"

extern s32 zelda_sprite_y[], zelda_sprite_x[], cur_sprite[];
extern s32 combat_frame;
extern s32 *line_table;
extern s32 draw_target;
extern s32 font_height, font_width, font_bitmap_a, font_bitmap_b, font_bitmap_c;

#define P(x) ((u8 *)(uintptr_t)(u32)(x))
#define LINE 0x1a0

struct channel { s32 next; s32 f4; u8 pad0[4]; u8 flags; u8 pad1; u32 len; s32 data; } __attribute__((packed));

void copy_sound(long dst, long src)
{
    u8 *d = P(dst), *s = P(src);
    for (int g = 0; g < 18; g++, d += 128, s += 128) {
        for (int i = 0; i < 16; i += 2)
            d[i + 1] = s[i + 1];
        for (int i = 16; i < 128; i++)
            d[i] |= s[i];
    }
}

void queue_insert_channel(long *queue, struct channel *c)
{
    struct channel *a3 = (struct channel *)(uintptr_t)*queue, *a4;
    if (!a3) {
        c->next = 0;
        *queue = (long)(uintptr_t)c;
        return;
    }
    if (c->f4 == 0) {
        c->next = (s32)(uintptr_t)a3;
        *queue = (long)(uintptr_t)c;
        return;
    }
    a4 = a3;
    while (a3) {
        if (c->f4 - a3->f4 < 0)
            break;
        a4 = a3;
        a3 = (struct channel *)(uintptr_t)a3->next;
    }
    c->next = (s32)(uintptr_t)a3;
    if ((long)(uintptr_t)a3 == *queue)
        *queue = (long)(uintptr_t)c;
    else
        a4->next = (s32)(uintptr_t)c;
}

struct channel *queue_pop_channel(long *queue)
{
    struct channel *c = (struct channel *)(uintptr_t)*queue;
    if (c)
        *queue = c->next;
    return c;
}

void clear_384x240(long buf) { memset(P(buf), 0, 384 * 240); }
void clear_416x240(long buf) { memset(P(buf), 0, 416 * 240); }

void draw_label_save_under(long src, long map, long save, long w, long h)
{
    u8 *s = P(src), *m = P(map), *b = P(save);
    for (long y = 0; y < h; y++, m += 0x2c0) {
        memcpy(b, m, w);
        memcpy(m, s, w);
        b += w;
        s += w;
    }
}

void restore_under_label(long map, long save, long w, long h)
{
    u8 *m = P(map), *b = P(save);
    for (long y = 0; y < h; y++, m += 0x2c0, b += w)
        memcpy(m, b, w);
}

static void rle(u8 *dst, const u8 *src, long stride, int flip, int flash)
{
    u8 color = (u8)combat_frame;
    for (int row = 0; row < 56; row++) {
        int left = 72;
        u8 *d = flip ? dst + 72 : dst;
        while (left > 0) {
            u8 n = *src++;
            if (n & 0x80) {
                n &= 0x7f;
                d += flip ? -n : n;
                left -= n;
            } else {
                for (int k = 0; k <= n; k++) {
                    u8 v = flash ? color : *src;
                    if (flash)
                        color = (color + 1) & 7;
                    src++;
                    if (flip)
                        *--d = v;
                    else
                        *d++ = v;
                }
                left -= n + 1;
            }
        }
        dst += stride;
    }
}

static u8 *sprite_dest(long base, long n, long stride)
{
    u32 y = (u32)(zelda_sprite_y[n] << 2) & 0xffff;
    return P(base + (long)(y * ((u32)stride & 0xffff)) + (zelda_sprite_x[n] << 2));
}

void draw_rle_sprite(long base, long n, long stride) { rle(sprite_dest(base, n, stride), P(cur_sprite[n]), stride, 0, 0); }
void draw_rle_sprite_flipped(long base, long n, long stride) { rle(sprite_dest(base, n, stride), P(cur_sprite[n]), stride, 1, 0); }
void draw_rle_sprite_flash(long base, long n, long stride) { rle(sprite_dest(base, n, stride), P(cur_sprite[n]), stride, 0, 1); }
void draw_rle_sprite_flash_flipped(long base, long n, long stride) { rle(sprite_dest(base, n, stride), P(cur_sprite[n]), stride, 1, 1); }

void draw_rle_icon(long base, long x, long y, long image)
{
    rle(P(base + (long)((u32)(y & 0xffff) * LINE) + x), P(image), LINE, 0, 0);
}

static u8 *cell(long x, long y, long base)
{
    return P(base + line_table[y << 2] + (x << 2));
}

void erase_32x32(long x, long y, long base)
{
    u8 *d = cell(x, y, base);
    for (int r = 0; r < 32; r++, d += LINE)
        memset(d, 0, 32);
}

void erase_sprite_rows(long x, long y, long base, long row, long count)
{
    if (!count)
        return;
    u8 *d = P(base + line_table[(y << 2) + row] + (x << 2));
    for (long r = 0; r <= count; r++, d += LINE)
        memset(d, 0, 72);
}

void erase_16x16(long x, long y, long base)
{
    u8 *d = cell(x, y, base);
    for (int r = 0; r < 16; r++, d += LINE)
        memset(d, 0, 16);
}

void erase_16x4(long x, long y, long base)
{
    u8 *d = cell(x, y, base);
    for (int r = 0; r < 4; r++, d += LINE)
        memset(d, 0, 16);
}

void draw_line(long x1, long y1, long x2, long y2, long color)
{
    u8 *a = P(draw_target + (long)((u32)(y1 & 0xffff) * LINE) + x1);
    u8 c = (u8)color;
    if (x1 == x2) {
        if (y1 == y2) {
            *a = c;
        } else if (y1 > y2) {
            for (long n = y1 - y2; n >= 0; n--, a -= LINE)
                *a = c;
        } else {
            for (long n = y2 - y1; n >= 0; n--, a += LINE)
                *a = c;
        }
        return;
    }
    if (y1 == y2) {
        if (x1 >= x2)
            for (long n = x1 - x2; n >= 0; n--)
                *a-- = c;
        else
            for (long n = x2 - x1; n >= 0; n--)
                *a++ = c;
        return;
    }
    long ystep, dy, xstep, dx;
    if (y1 >= y2) {
        ystep = -LINE;
        dy = y1 - y2;
    } else {
        ystep = LINE;
        dy = y2 - y1;
    }
    if (x1 >= x2) {
        xstep = -1;
        dx = x1 - x2;
    } else {
        xstep = 1;
        dx = x2 - x1;
    }
    long acc = 0, lim;
    if (dx >= dy) {
        lim = ((dx << 6) / dy) & 0xffff;
        for (long n = dx; n >= 0; n--) {
            *a = c;
            a += xstep;
            acc += 64;
            if (acc >= lim) {
                acc -= lim;
                a += ystep;
            }
        }
    } else {
        lim = ((dy << 6) / dx) & 0xffff;
        for (long n = dy; n >= 0; n--) {
            *a = c;
            a += ystep;
            acc += 64;
            if (acc >= lim) {
                acc -= lim;
                a += xstep;
            }
        }
    }
}

static void rotate(s16 *len, s16 *timer, s16 *delay, s16 *start, s32 *colors, int n)
{
    for (int i = 0; i < n; i++) {
        if (len[i] < 2 || !delay[i] || !start[i])
            continue;
        if (--timer[i] != 0)
            continue;
        timer[i] = delay[i];
        s32 *p = colors + (start[i] - 1);
        u32 last = (u32)p[len[i] - 1] & 0xffffff;
        for (int k = len[i] - 1; k > 0; k--)
            p[k] = (s32)(((u32)p[k] & 0xff000000) | ((u32)p[k - 1] & 0xffffff));
        p[0] = (s32)(((u32)p[0] & 0xff000000) | last);
    }
}

extern s16 bg_cycle_len[], bg_cycle_timer[], bg_cycle_delay[], bg_cycle_start[];
extern s32 bg_cycle[], cycle_colors[];

void rotate_bg_cycle(void)
{
    rotate(bg_cycle_len, bg_cycle_timer, bg_cycle_delay, bg_cycle_start, bg_cycle, 6);
}

void rotate_sprite_cycle(void)
{
    u8 *s = (u8 *)bg_cycle_start;
    rotate((s16 *)(s - 0x2c), (s16 *)(s - 0x34), (s16 *)(s - 0x3c), (s16 *)(s - 0x44), cycle_colors, 4);
}

static void glyphs(long x, long y, const char *text, int color)
{
    const u8 *offs = P(font_bitmap_a), *widths = P(font_bitmap_b), *data = P(font_bitmap_c);
    u8 *a = P(draw_target + (long)((u32)(y & 0xffff) * ((u32)font_width & 0xffff)) + x);
    for (; *text; text++) {
        int c = ((u8)*text - 0x20) * 2;
        int w = be16(widths + c);
        if (!w)
            continue;
        const u8 *g = data + be16(offs + c);
        u8 *row = a;
        for (int h = font_height; h > 0; h--) {
            u8 *d = row;
            int left = w;
            while (left > 0) {
                u8 b = *g++;
                if (b & 0x80) {
                    d += b & 0x7f;
                    left -= b & 0x7f;
                } else {
                    *d++ = (u8)color;
                    left--;
                }
            }
            row += font_width;
        }
        a += w + 1;
    }
}

void font_draw_glyphs(long x, long y, char *text) { glyphs(x, y, text, 7); }
void color_font_draw_glyphs(long x, long y, char *text, long color) { glyphs(x, y, text, (int)color); }
void font_erase_glyphs(long x, long y, char *text) { glyphs(x, y, text, 12); }

long text_width(char *text)
{
    const u8 *widths = P(font_bitmap_b);
    long w = 0;
    for (; *text; text++)
        w += be16(widths + ((u8)*text - 0x20) * 2) + 1;
    return w;
}

long nvr_pack_name(char *in, char *out, long n, char *key, long keylen)
{
    (void)key; (void)keylen;
    if (!in || !out || n <= 0)
        return 0;
    long i;
    for (i = 0; i < n && in[i]; i++)
        out[i] = in[i];
    out[i] = 0;
    return i + 1;
}

long nvr_unpack_name(char *in, char *out, long n, char *key, long keylen)
{
    return nvr_pack_name(in, out, n, key, keylen);
}
