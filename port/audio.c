#include <windows.h>
#include <math.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "pc.h"

#pragma pack(push, 2)
struct smap {
    u8 pad0[10];
    u32 data;
    s32 size;
    u8 pad1[8];
    s16 w;
    s16 pos;
};
struct sound_pcb { s16 a, b; };
#pragma pack(pop)

static SDL_AudioDeviceID device;
static SDL_AudioStream *map_stream;
static SDL_AudioStream *disc_stream;
static int disc_rate, disc_channels;
static CRITICAL_SECTION audio_lock;

#define MAX_MAPS 8
static struct { int used, type; struct smap *desc; } maps[MAX_MAPS];

static struct { u64 end; int sig; } pending[16];
static int npending;
static u64 map_pushed;

static float disc_gain = 1;
volatile int fast_forward;
static float gain_ll = 1, gain_lr = 1, gain_rl = 1, gain_rr = 1;

static const int K0[4] = {0, 60, 115, 98}, K1[4] = {0, 0, -52, -55};
struct adpcm { int s1[2], s2[2]; };

static int decode_groups(struct adpcm *st, const u8 *src, int groups, int coding, s16 *out)
{
    int stereo = coding & 1, eight = coding & 0x10;
    int units = eight ? 4 : 8;
    int n = 0;
    for (int g = 0; g < groups; g++) {
        const u8 *grp = src + g * 128;
        s16 us[8][28];
        for (int u = 0; u < units; u++) {
            int param = eight ? grp[u] : grp[4 + u];
            int shift = param & 15;
            if (shift > 12)
                shift = 9;
            int f = param >> 4 & 3;
            int side = stereo ? (u & 1) : 0;
            int s1 = st->s1[side], s2 = st->s2[side];
            for (int j = 0; j < 28; j++) {
                const u8 *row = grp + 16 + 4 * j;
                int v;
                if (eight) {
                    v = (s8)row[u];
                    v = (v << 8) >> shift;
                } else {
                    int nib = (row[u >> 1] >> ((u & 1) * 4)) & 15;
                    if (nib & 8)
                        nib -= 16;
                    v = (nib << 12) >> shift;
                }
                int s = v + ((s1 * K0[f] + s2 * K1[f] + 32) >> 6);
                s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
                s2 = s1;
                s1 = s;
                us[u][j] = (s16)s;
            }
            st->s1[side] = s1;
            st->s2[side] = s2;
        }
        if (stereo) {
            for (int u = 0; u < units; u += 2)
                for (int j = 0; j < 28; j++) {
                    out[n * 2] = us[u][j];
                    out[n * 2 + 1] = us[u + 1][j];
                    n++;
                }
        } else {
            for (int u = 0; u < units; u++)
                for (int j = 0; j < 28; j++)
                    out[n++] = us[u][j];
        }
    }
    return n;
}

static void attenuate(s16 *s, int frames)
{
    for (int i = 0; i < frames; i++) {
        float l = s[i * 2], r = s[i * 2 + 1];
        float ol = l * gain_ll + r * gain_rl, or_ = l * gain_lr + r * gain_rr;
        s[i * 2] = (s16)(ol > 32767 ? 32767 : ol < -32768 ? -32768 : ol);
        s[i * 2 + 1] = (s16)(or_ > 32767 ? 32767 : or_ < -32768 ? -32768 : or_);
    }
}

void audio_init(void)
{
    InitializeCriticalSection(&audio_lock);
    device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!device) {
        LOG("no audio: %s\n", SDL_GetError());
        return;
    }
    SDL_AudioSpec spec = {SDL_AUDIO_S16, 2, 18900};
    map_stream = SDL_CreateAudioStream(&spec, NULL);
    SDL_BindAudioStream(device, map_stream);
    SDL_ResumeAudioDevice(device);
}

static void lock(void) { port_enter(); EnterCriticalSection(&audio_lock); }
static void unlock(void) { LeaveCriticalSection(&audio_lock); port_leave(); }

long sm_creat(long path, long type, long groups, long *mem)
{
    (void)path;
    lock();
    int id = -1;
    for (int i = 1; i < MAX_MAPS; i++)
        if (!maps[i].used) {
            id = i;
            break;
        }
    if (id > 0) {
        struct smap *d = mem_alloc(sizeof(struct smap), 0);
        u8 *data = mem_alloc((u32)groups * 128, 0);
        d->data = (u32)(uintptr_t)data;
        d->size = (s32)groups * 128;
        d->w = (s16)groups;
        maps[id].used = 1;
        maps[id].type = (int)type;
        maps[id].desc = d;
        if (mem)
            *mem = (long)(uintptr_t)data;
    }
    unlock();
    return id;
}

struct smap *sm_info(long path, long id)
{
    (void)path;
    return id > 0 && id < MAX_MAPS && maps[id].used ? maps[id].desc : NULL;
}

long sm_out(long path, long id, struct sound_pcb *p)
{
    (void)path;
    if (id <= 0 || id >= MAX_MAPS || !maps[id].used || !map_stream)
        return -1;
    lock();
    struct smap *d = maps[id].desc;
    int groups = d->w > 0 ? d->w : d->size / 128;
    static s16 pcm[18 * 112 * 2 * 64];
    static struct adpcm st;
    if (groups > 18 * 64)
        groups = 18 * 64;
    int frames = decode_groups(&st, (const u8 *)(uintptr_t)d->data, groups, maps[id].type | 1, pcm);
    float vm = settings.music / 100.0f, ve = settings.effects / 100.0f;
    for (int i = 0; i < frames; i++) {
        pcm[i * 2] = (s16)(pcm[i * 2] * vm);
        pcm[i * 2 + 1] = (s16)(pcm[i * 2 + 1] * ve);
    }
    attenuate(pcm, frames);
    SDL_PutAudioStreamData(map_stream, pcm, frames * 4);
    map_pushed += (u64)frames * 4;
    if (p && p->b && npending < 16) {
        pending[npending].end = map_pushed;
        pending[npending].sig = p->b;
        npending++;
    }
    unlock();
    return 0;
}

void sm_off(long path)
{
    (void)path;
    lock();
    if (map_stream)
        SDL_ClearAudioStream(map_stream);
    npending = 0;
    unlock();
}

void sc_atn(long path, long value)
{
    (void)path;
    u32 v = (u32)value;
    float g[4];
    for (int i = 0; i < 4; i++) {
        int a = v >> (24 - i * 8) & 0xff;
        g[i] = a >= 0x80 ? 0 : powf(10, -a / 20.0f);
    }
    gain_lr = g[0];
    gain_ll = g[1];
    gain_rl = g[2];
    gain_rr = g[3];
    disc_gain = g[1] > g[3] ? g[1] : g[3];
}

void audio_tick(void)
{
    if (!map_stream)
        return;
    lock();
    int queued = SDL_GetAudioStreamQueued(map_stream);
    u64 played = map_pushed - (queued > 0 ? (u64)queued : 0);
    const u64 early = 18900 * 4 / 25;
    while (npending > 0 && played + early >= pending[0].end) {
        signal_send(pending[0].sig);
        memmove(pending, pending + 1, --npending * sizeof pending[0]);
    }
    unlock();
}

void audio_stream_reset(void)
{
    if (disc_stream)
        SDL_ClearAudioStream(disc_stream);
}

void audio_stream_sector(const u8 *payload, int coding, int music)
{
    static struct adpcm st;
    static s16 pcm[18 * 224 * 2];
    if (!device)
        return;
    if (fast_forward) {
        if (disc_stream)
            SDL_ClearAudioStream(disc_stream);
        return;
    }
    int rate = coding & 4 ? 18900 : 37800, ch = coding & 1 ? 2 : 1;
    if (!disc_stream || rate != disc_rate || ch != disc_channels) {
        if (disc_stream)
            SDL_DestroyAudioStream(disc_stream);
        SDL_AudioSpec spec = {SDL_AUDIO_S16, ch, rate};
        SDL_AudioSpec out = {SDL_AUDIO_S16, 2, 44100};
        disc_stream = SDL_CreateAudioStream(&spec, &out);
        SDL_BindAudioStream(device, disc_stream);
        disc_rate = rate;
        disc_channels = ch;
        memset(&st, 0, sizeof st);
    }
    int n = decode_groups(&st, payload, 18, coding, pcm);
    SDL_SetAudioStreamGain(disc_stream, disc_gain * (music ? settings.music : settings.voice) / 100.0f);
    SDL_PutAudioStreamData(disc_stream, pcm, n * ch * 2);
}

void audio_pause(int paused)
{
    if (!device)
        return;
    if (paused)
        SDL_PauseAudioDevice(device);
    else
        SDL_ResumeAudioDevice(device);
}

void state_add(void *p, u32 size);

void audio_state(void)
{
    state_add(maps, sizeof maps);
    state_add(pending, sizeof pending);
    state_add(&npending, sizeof npending);
    state_add(&gain_ll, sizeof gain_ll);
    state_add(&gain_lr, sizeof gain_lr);
    state_add(&gain_rl, sizeof gain_rl);
    state_add(&gain_rr, sizeof gain_rr);
    state_add(&disc_gain, sizeof disc_gain);
}

void audio_after_load(void)
{
    if (map_stream)
        SDL_ClearAudioStream(map_stream);
    if (disc_stream)
        SDL_ClearAudioStream(disc_stream);
    map_pushed = 0;
    for (int i = 0; i < npending; i++)
        pending[i].end = 0;
}
