#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "pc.h"

u8 *cdi_mem;
#define MEM_SIZE 0x1000000

struct piece { u32 size; u32 used; };

void mem_init(void)
{
    cdi_mem = VirtualAlloc((void *)0x20000000, MEM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    for (int tries = 0; tries < 16 && !cdi_mem; tries++) {
        u8 *p = VirtualAlloc(NULL, 2 * MEM_SIZE, MEM_RESERVE, PAGE_NOACCESS);
        if (!p)
            break;
        u8 *aligned = (u8 *)(((uintptr_t)p + MEM_SIZE - 1) & ~(uintptr_t)(MEM_SIZE - 1));
        VirtualFree(p, 0, MEM_RELEASE);
        cdi_mem = VirtualAlloc(aligned, MEM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    }
    if (!cdi_mem)
        fatal("Couldn't get 16 MB of memory on a 16 MB boundary.");
    struct piece *first = (struct piece *)(cdi_mem + 0x10000);
    first->size = MEM_SIZE - 0x10000 - sizeof(struct piece);
    first->used = 0;
}

void *mem_alloc(u32 size, int color)
{
    (void)color;
    size = (size + 15) & ~15u;
    u8 *at = cdi_mem + 0x10000;
    while (at < cdi_mem + MEM_SIZE) {
        struct piece *p = (struct piece *)at;
        if (!p->used && p->size >= size) {
            if (p->size >= size + 64) {
                struct piece *rest = (struct piece *)(at + 16 + size);
                rest->size = p->size - size - 16;
                rest->used = 0;
                p->size = size;
            }
            p->used = 1;
            memset(at + 16, 0, size);
            return at + 16;
        }
        at += 16 + p->size;
    }
    LOG("mem_alloc: out of memory (%u bytes)\n", size);
    return NULL;
}

void mem_free(void *ptr)
{
    if (!ptr || (u8 *)ptr < cdi_mem || (u8 *)ptr >= cdi_mem + MEM_SIZE)
        return;
    struct piece *p = (struct piece *)((u8 *)ptr - 16);
    p->used = 0;
    for (;;) {
        u8 *next = (u8 *)p + 16 + p->size;
        if (next >= cdi_mem + MEM_SIZE || ((struct piece *)next)->used)
            break;
        p->size += 16 + ((struct piece *)next)->size;
    }
}

extern u8 a5space[];
extern const s32 a5_below;
extern const u16 a5swap[][2];
extern const int a5swap_count;

static const u8 unpacker[] = {0x48, 0xe7, 0xff, 0xf0, 0x42, 0x47, 0x41, 0xfa};

void a5_init(const u8 *mod, u32 modsize)
{
    u32 at = 0;
    for (at = 0; at + 10 < modsize; at += 2)
        if (memcmp(mod + at, unpacker, sizeof unpacker) == 0)
            break;
    if (at + 10 >= modsize)
        fatal("The game's module has no global data unpacker (is this Zelda: The Wand of Gamelon?).");
    u32 p = at + 8 + (s16)be16(mod + at + 8);
    s32 below = (s32)be32(mod + p);
    if (below != a5_below)
        fatal("This disc's program has %d bytes of globals, the port expects %d: wrong version of the game?",
              below, a5_below);
    u8 *img = a5space;
    memset(img, 0, below);
    static u32 relocs[256];
    int nrel = 0;

    #define TAKE(n) (p += (n), take_val(mod + p - (n), (n)))
    u32 take_val(const u8 *b, int n) { u32 v = 0; while (n--) v = v << 8 | *b++; return v; }

    s32 size = (s32)be32(mod + p);
    p += 4;
    if (size > 0) {
        u32 base = TAKE(4);
        base = base == 0 ? (u32)(below - size) : base;
        u32 entries = TAKE(2);
        TAKE(2);
        for (u32 e = 0; e < entries; e++) {
            u32 flags = TAKE(1);
            u32 n = flags & 0xf;
            if (flags & 0x10) {
                n = n << 8 | TAKE(1);
                if (n & 0x800)
                    n = (n & ~0x800u) << 8 | TAKE(1);
            }
            if (flags & 0x80) {
                u32 r = TAKE(1);
                if (r & 0x80) {
                    r = (r & 0x7f) << 8 | TAKE(1);
                    if (r & 0x4000)
                        TAKE(1);
                }
            }
            u32 off = TAKE(2);
            if (off & 0x8000)
                off = (off & 0x7fff) << 8 | TAKE(1);
            u32 dst = base + off;
            if (flags & 0x20) {
                u32 v = TAKE(n);
                u32 old = 0;
                for (u32 i = 0; i < n; i++)
                    old = old << 8 | img[dst + i];
                old += v;
                for (u32 i = n; i-- > 0; old >>= 8)
                    img[dst + i] = (u8)old;
            } else {
                memcpy(img + dst, mod + p, n);
                p += n;
            }
            if ((flags & 0x40) && nrel < 256)
                relocs[nrel++] = dst;
        }
    }
    for (int i = 0; i < a5swap_count; i++) {
        u8 *v = img + a5swap[i][0];
        if (a5swap[i][1] == 2) {
            u16 x; memcpy(&x, v, 2); x = swap16(x); memcpy(v, &x, 2);
        } else {
            u32 x; memcpy(&x, v, 4); x = swap32(x); memcpy(v, &x, 4);
        }
    }
    u32 a5 = (u32)(uintptr_t)(img + below);
    for (int i = 0; i < nrel; i++) {
        u8 *v = img + relocs[i];
        int swapped = 0;
        for (int k = 0; k < a5swap_count; k++)
            if (a5swap[k][0] == relocs[i] && a5swap[k][1] == 4) {
                swapped = 1;
                break;
            }
        u32 x;
        memcpy(&x, v, 4);
        if (!swapped)
            x = swap32(x);
        x = a5 + (s32)x;
        memcpy(v, &x, 4);
    }
    LOG("globals: %d bytes, %d pointers\n", below, nrel);
}
