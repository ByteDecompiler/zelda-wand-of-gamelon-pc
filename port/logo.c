#include <windows.h>
#include <string.h>
#include "pc.h"

void audio_stream_sector(const u8 *payload, int coding, int music);
void audio_stream_reset(void);
void pt_coord(long path, long *buttons, long *x, long *y);

static u8 module_stub[0x100];
static u32 logo_rgb[SCREEN_W * SCREEN_H];
volatile int logo_showing;
volatile int logo_level = 63;

char *logo_module(void)
{
    *(s32 *)(module_stub + 0x30) = 0x40;
    return (char *)module_stub;
}

void logo_frame(u32 *out)
{
    for (int i = 0; i < SCREEN_W * SCREEN_H; i++) {
        u32 c = logo_rgb[i];
        out[i] = ((c >> 16 & 0xff) * logo_level / 63) << 16 | ((c >> 8 & 0xff) * logo_level / 63) << 8
                 | (c & 0xff) * logo_level / 63;
    }
}

static void rl7(const u8 *src, int len, const u32 *clut)
{
    int n = 0;
    for (int y = 0; y < SCREEN_H; y++)
        for (int x = 0; x < SCREEN_W && n < len;) {
            u8 b = src[n++];
            int run = 1;
            if (b & 0x80) {
                run = n < len ? src[n++] : 0;
                if (run == 0)
                    run = SCREEN_W - x;
            }
            for (; run > 0 && x < SCREEN_W; run--, x++)
                logo_rgb[y * SCREEN_W + x] = clut[b & 0x7f];
        }
}

static void dyuv(const u8 *src)
{
    static const int delta[16] = {0, 1, 4, 9, 16, 27, 44, 79, 128, 177, 212, 229, 240, 247, 252, 255};
    int n = 0;
    for (int y = 0; y < SCREEN_H; y++) {
        int Y = 0x90, U = 0x80, V = 0x80;
        for (int x = 0; x < SCREEN_W; x += 2) {
            u8 b0 = src[n++], b1 = src[n++];
            for (int k = 0; k < 2; k++) {
                if (k == 0) {
                    U = (U + delta[b0 >> 4]) & 0xff;
                    Y = (Y + delta[b0 & 15]) & 0xff;
                } else {
                    V = (V + delta[b1 >> 4]) & 0xff;
                    Y = (Y + delta[b1 & 15]) & 0xff;
                }
                int r = Y + 1371 * (V - 128) / 1000, g = Y - (336 * (U - 128) + 698 * (V - 128)) / 1000,
                    b = Y + 1732 * (U - 128) / 1000;
                r = r < 0 ? 0 : r > 255 ? 255 : r;
                g = g < 0 ? 0 : g > 255 ? 255 : g;
                b = b < 0 ? 0 : b > 255 ? 255 : b;
                logo_rgb[y * SCREEN_W + x + k] = (u32)r << 16 | (u32)g << 8 | (u32)b;
            }
        }
    }
}

static int entry(const u8 *d, const char *name)
{
    if (be32(d) != 0xbabeface)
        return -1;
    int count = (int)be32(d + 4);
    for (int i = 0; i < count && i < 32; i++) {
        const u8 *e = d + 14 + i * 10;
        u32 nm = be32(e + 2), val = be32(e + 6);
        if (nm < 2000 && val < 2048 && strcmp((const char *)d + nm, name) == 0)
            return (int)val;
    }
    return -1;
}

static int skipped(void)
{
    long b;
    pt_coord(0, &b, NULL, NULL);
    return (b & 3) != 0;
}

long logo_play(void)
{
    u32 first, size;
    if (!disc_file("bumper.rtf", &first, &size))
        return 0;
    static u8 rlbuf[160 * 2324], dybuf[48 * 2324];
    u8 raw[SECTOR_RAW];
    struct sector s;
    u32 clut[128] = {0};
    int frame_sectors[64] = {0}, frames = 0;
    int rl_len = 0, rl_shown = 0, rl_frame = 0, rl_frame_start = 0, dy_len = 0;
    memset(logo_rgb, 0, sizeof logo_rgb);
    logo_level = 63;
    logo_showing = 1;
    LARGE_INTEGER freq, t0, now;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);
    int done_at = 0;
    for (u32 n = 0; n * 2048 < size; n++) {
        for (;;) {
            QueryPerformanceCounter(&now);
            if ((now.QuadPart - t0.QuadPart) * 75 >= (long long)n * freq.QuadPart)
                break;
            Sleep(1);
        }
        if (skipped())
            goto out;
        if (!disc_read(first + n, raw))
            break;
        disc_parse(raw, &s);
        if (s.data && s.channel == 16) {
            int c = entry(s.payload, "cluts"), f = entry(s.payload, "frame_sizes"), k = entry(s.payload, "count");
            for (int i = 0; c >= 0 && i < 128; i++)
                clut[i] = be32(s.payload + c + i * 4) & 0xffffff;
            int end = entry(s.payload, "offset");
            for (frames = 0; f >= 0 && f + frames * 4 < (end > f ? end : f + 256) && frames < 64; frames++)
                frame_sectors[frames] = (int)be32(s.payload + f + frames * 4);
            (void)k;
        } else if (s.video && s.channel == 15 && rl_len + s.size <= (int)sizeof rlbuf) {
            memcpy(rlbuf + rl_len, s.payload, s.size);
            rl_len += s.size;
            while (rl_frame < frames && rl_len >= (rl_frame_start + frame_sectors[rl_frame]) * 2324) {
                rl7(rlbuf + rl_frame_start * 2324, frame_sectors[rl_frame] * 2324, clut);
                rl_frame_start += frame_sectors[rl_frame];
                rl_frame++;
                rl_shown = 1;
            }
        } else if (s.video && s.channel == 16 && dy_len + s.size <= (int)sizeof dybuf) {
            memcpy(dybuf + dy_len, s.payload, s.size);
            dy_len += s.size;
        } else if (s.audio && s.channel == 15)
            audio_stream_sector(s.payload, s.coding, 0);
        if (rl_shown && rl_frame >= frames && frames && !done_at && dy_len >= SCREEN_W * SCREEN_H) {
            dyuv(dybuf);
            done_at = (int)n;
        }
    }
    for (int i = 0; i < 60 && !skipped(); i++)
        Sleep(16);
out:
    for (int l = logo_level; l >= 0; l -= 4) {
        logo_level = l;
        Sleep(16);
    }
    logo_showing = 0;
    audio_stream_reset();
    for (int i = 0; i < 100 && skipped(); i++)
        Sleep(16);
    return 0;
}
