#include <windows.h>
#include <string.h>
#include "pc.h"

#pragma pack(push, 2)
struct pcb {
    s16 stat;
    s16 sig;
    s32 rec;
    s32 chan;
    s16 achan;
    u32 video;
    u32 audio;
    u32 data;
};
struct pcl {
    u8 ctrl;
    u8 pad0;
    u8 c2, c3;
    s16 sig;
    u32 nxt;
    u32 buf;
    s32 buf_size;
    u32 err;
    s16 pad1;
    s32 cnt;
};
#pragma pack(pop)

int path_rtr(long n, u32 *first, u32 *size, u32 *pos);
const char *path_name(long n);
void audio_stream_sector(const u8 *payload, int coding, int music);
void audio_stream_reset(void);

static struct play {
    int active;
    long path;
    u32 first, size, sector;
    struct pcb *pcb;
    int stream;
    int music;
    double credit;
} plays[4];
static CRITICAL_SECTION play_lock;
static int lock_ready;

static void lock(void)
{
    if (!lock_ready) {
        InitializeCriticalSection(&play_lock);
        lock_ready = 1;
    }
    port_enter();
    EnterCriticalSection(&play_lock);
}
static void unlock(void) { LeaveCriticalSection(&play_lock); port_leave(); }

void ss_abort(long path)
{
    lock();
    for (int i = 0; i < 4; i++)
        if (plays[i].active && plays[i].path == path) {
            plays[i].active = 0;
            if (plays[i].pcb->achan)
                audio_stream_reset();
        }
    unlock();
}

void ss_play(long path, struct pcb *pcb)
{
    u32 first, size, pos;
    if (!path_rtr(path, &first, &size, &pos)) {
        LOG("ss_play: path %ld is not a disc file\n", path);
        return;
    }
    ss_abort(path);
    lock();
    for (int i = 0; i < 4; i++)
        if (!plays[i].active) {
            struct play *p = &plays[i];
            p->path = path;
            p->first = first;
            p->size = size;
            p->sector = pos / 2048;
            p->pcb = pcb;
            p->stream = pcb->achan != 0 || pcb->audio != 0 || strstr(path_name(path), "anim") != NULL;
            p->credit = 0;
            p->music = strstr(path_name(path), "music") != NULL;
            p->active = 1;
            break;
        }
    unlock();
}

static void to_pcl(u32 cil_addr, int channel, const u8 *payload, int n)
{
    if (!cil_addr)
        return;
    u32 *cil = (u32 *)(uintptr_t)cil_addr;
    u32 at = cil[channel];
    if (!at)
        return;
    struct pcl *pcl = (struct pcl *)(uintptr_t)at;
    if (!pcl->buf || pcl->cnt >= pcl->buf_size)
        return;
    memcpy((u8 *)(uintptr_t)(pcl->buf + (u32)pcl->cnt * n), payload, n);
    pcl->cnt++;
    if (pcl->cnt >= pcl->buf_size) {
        if (pcl->sig)
            signal_send(pcl->sig);
        if (pcl->nxt)
            cil[channel] = pcl->nxt;
    }
}

static int play_sector(struct play *p, int *finished)
{
    u8 raw[SECTOR_RAW];
    struct sector s;
    struct pcb *pcb = p->pcb;
    if (p->sector * 2048 >= p->size || !disc_read(p->first + p->sector, raw)) {
        *finished = 1;
        return 1;
    }
    p->sector++;
    disc_parse(raw, &s);
    int ch = s.channel & 31;
    int stop = 0;
    if (pcb->chan & (1 << ch)) {
        if (s.audio) {
            if (pcb->achan & (1 << ch))
                audio_stream_sector(s.payload, s.coding, p->music);
            else
                to_pcl(pcb->audio, ch & 15, s.payload, 0x900);
        } else if (s.video)
            to_pcl(pcb->video, ch, s.payload, s.size);
        else if (s.data)
            to_pcl(pcb->data, ch, s.payload, s.size);
        if (s.submode & 0x10) {
            pcb->stat |= 0x10;
            signal_send(pcb->sig);
            stop = 1;
        }
        if (s.eor) {
            pcb->stat |= 1;
            signal_send(pcb->sig);
            if (--pcb->rec <= 0)
                *finished = 2;
            return 1;
        }
    }
    if (s.eof) {
        *finished = 1;
        return 1;
    }
    return stop;
}

void play_tick(void)
{
    lock();
    for (int i = 0; i < 4; i++) {
        struct play *p = &plays[i];
        if (!p->active)
            continue;
        int budget;
        if (p->stream) {
            p->credit += 75.0 * 1.001 / 60.0;
            budget = (int)p->credit;
            p->credit -= budget;
        } else
            budget = 64;
        while (budget-- > 0 && p->active) {
            int finished = 0;
            int stop = play_sector(p, &finished);
            if (finished) {
                p->active = 0;
                if (finished == 1) {
                    p->pcb->stat |= 1;
                    signal_send(p->pcb->sig);
                }
                break;
            }
            if (stop)
                break;
        }
    }
    unlock();
}

void state_add(void *p, u32 size);

void play_state(void)
{
    state_add(plays, sizeof plays);
}

void play_after_load(void)
{
    audio_stream_reset();
}
