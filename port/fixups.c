#include <string.h>
#include "pc.h"

extern s32 enemy_list, env_proj_buf;

void port_swap_longs(long addr, long count)
{
    u32 *p = (u32 *)(uintptr_t)addr;
    for (long i = 0; i < count; i++)
        p[i] = swap32(p[i]);
}

void port_swap_shorts(long addr, long count)
{
    u16 *p = (u16 *)(uintptr_t)addr;
    for (long i = 0; i < count; i++)
        p[i] = swap16(p[i]);
}

void port_fix_room(void)
{
    u32 *ids = (u32 *)(uintptr_t)enemy_list;
    for (int i = 0; i < 8; i++) {
        ids[i] = swap32(ids[i]);
        if (ids[i] == 0xffffffff)
            break;
    }
    port_swap_longs(env_proj_buf, 0x800 / 4);
}

void port_fix_macros(long macro_data)
{
    u8 *base = (u8 *)(uintptr_t)macro_data;
    port_swap_longs(macro_data, 4);
    u32 *hdr = (u32 *)base;
    u32 count = hdr[3], start = hdr[2];
    if (count > 50 || start >= 0x800) {
        LOG("macro data looks wrong: %u scripts from %u\n", count, start);
        return;
    }
    port_swap_longs(macro_data + 16, count);
    port_swap_shorts(macro_data + start, (0x800 - start) / 2);
}
