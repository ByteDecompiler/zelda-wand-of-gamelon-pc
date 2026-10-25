#include <string.h>
#include "pc.h"

#define RETURN 0xfffffff0u

static u32 d[8], a[8], pc;
static int fn, fz, fv, fc;
static int failed;

static const u32 mask_of[5] = {0, 0xff, 0xffff, 0, 0xffffffff};

static u8 *at(u32 addr) { return (u8 *)(uintptr_t)addr; }

static u32 rd(u32 addr, int size)
{
    u8 *p = at(addr);
    return size == 1 ? p[0] : size == 2 ? be16(p) : be32(p);
}

static void wr(u32 addr, int size, u32 v)
{
    u8 *p = at(addr);
    if (size == 1)
        p[0] = (u8)v;
    else if (size == 2)
        wbe16(p, (u16)v);
    else
        wbe32(p, v);
}

static u32 fetch(int size)
{
    u32 v = rd(pc, size);
    pc += size;
    return v;
}

static s32 sext(u32 v, int size)
{
    return size == 1 ? (s8)v : size == 2 ? (s16)v : (s32)v;
}

static void setd(int n, int size, u32 v)
{
    u32 m = mask_of[size];
    d[n] = (d[n] & ~m) | (v & m);
}

static void flags(u32 v, int size)
{
    v &= mask_of[size];
    fz = v == 0;
    fn = (v >> (size * 8 - 1)) & 1;
    fv = fc = 0;
}

struct loc { char kind; u32 v; };

static u32 index_ext(void)
{
    u32 ext = fetch(2);
    int r = ext >> 12 & 7;
    u32 x = ext & 0x8000 ? a[r] : d[r];
    if (!(ext & 0x800))
        x = (u32)sext(x, 2);
    return x + (u32)sext(ext & 0xff, 1);
}

static struct loc ea(int mode, int reg, int size)
{
    struct loc l = {'m', 0};
    u32 base;
    switch (mode) {
    case 0: l.kind = 'd'; l.v = reg; return l;
    case 1: l.kind = 'a'; l.v = reg; return l;
    case 2: l.v = a[reg]; return l;
    case 3: l.v = a[reg]; a[reg] += (reg == 7 && size == 1) ? 2 : size; return l;
    case 4: a[reg] -= (reg == 7 && size == 1) ? 2 : size; l.v = a[reg]; return l;
    case 5: l.v = a[reg] + (u32)sext(fetch(2), 2); return l;
    case 6: l.v = a[reg] + index_ext(); return l;
    case 7:
        switch (reg) {
        case 0: l.v = (u32)sext(fetch(2), 2); return l;
        case 1: l.v = fetch(4); return l;
        case 2: base = pc; l.v = base + (u32)sext(fetch(2), 2); return l;
        case 3: base = pc; l.v = base + index_ext(); return l;
        case 4: l.kind = 'i'; l.v = size == 4 ? fetch(4) : fetch(2) & mask_of[size]; return l;
        }
    }
    failed = 1;
    return l;
}

static u32 get(struct loc l, int size)
{
    switch (l.kind) {
    case 'd': return d[l.v] & mask_of[size];
    case 'a': return a[l.v] & mask_of[size];
    case 'm': return rd(l.v, size);
    default: return l.v & mask_of[size];
    }
}

static void put(struct loc l, int size, u32 v)
{
    switch (l.kind) {
    case 'd': setd(l.v, size, v); break;
    case 'a': a[l.v] = (u32)sext(v, size); break;
    case 'm': wr(l.v, size, v); break;
    default: failed = 1; break;
    }
}

static int cond(int cc)
{
    switch (cc) {
    case 0: return 1;
    case 1: return 0;
    case 2: return !fc && !fz;
    case 3: return fc || fz;
    case 4: return !fc;
    case 5: return fc;
    case 6: return !fz;
    case 7: return fz;
    case 8: return !fv;
    case 9: return fv;
    case 10: return !fn;
    case 11: return fn;
    case 12: return fn == fv;
    case 13: return fn != fv;
    case 14: return !fz && fn == fv;
    default: return fz || fn != fv;
    }
}

static u32 addsub(u32 x, u32 y, int size, int sub)
{
    u32 m = mask_of[size];
    u32 r = (sub ? x - y : x + y) & m;
    int top = size * 8 - 1;
    x &= m;
    y &= m;
    fc = sub ? y > x : (u64)x + y > m;
    int sa = x >> top & 1, sb = y >> top & 1, sr = r >> top & 1;
    fv = sub ? (sa != sb && sr != sa) : (sa == sb && sr != sa);
    fz = r == 0;
    fn = sr;
    return r;
}

static void movem(u32 op)
{
    int size = op & 0x40 ? 4 : 2;
    u32 mask = fetch(2);
    int mode = op >> 3 & 7, reg = op & 7;
    if (op & 0x400) {
        u32 addr = mode != 3 ? ea(mode, reg, size).v : a[reg];
        for (int k = 0; k < 16; k++)
            if (mask >> k & 1) {
                u32 v = (u32)sext(rd(addr, size), size);
                if (k < 8) d[k] = v; else a[k - 8] = v;
                addr += size;
            }
        if (mode == 3)
            a[reg] = addr;
    } else if (mode == 4) {
        u32 addr = a[reg];
        for (int k = 0; k < 16; k++)
            if (mask >> k & 1) {
                int r = 15 - k;
                addr -= size;
                wr(addr, size, r < 8 ? d[r] : a[r - 8]);
            }
        a[reg] = addr;
    } else {
        u32 addr = ea(mode, reg, size).v;
        for (int k = 0; k < 16; k++)
            if (mask >> k & 1) {
                wr(addr, size, k < 8 ? d[k] : a[k - 8]);
                addr += size;
            }
    }
}

static void alu(u32 op)
{
    int hi = op >> 12, size = 1 << (op >> 6 & 3), r = op >> 9 & 7;
    int to_ea = (op & 0x100) != 0;
    if (hi == 0xb && to_ea) {
        struct loc l = ea(op >> 3 & 7, op & 7, size);
        u32 v = get(l, size) ^ d[r];
        put(l, size, v);
        flags(v, size);
        return;
    }
    if (to_ea && (op >> 3 & 7) <= 1) {
        failed = 1;
        return;
    }
    struct loc l = ea(op >> 3 & 7, op & 7, size);
    u32 x, y, v;
    if (to_ea) {
        x = get(l, size);
        y = d[r] & mask_of[size];
    } else {
        x = d[r] & mask_of[size];
        y = get(l, size);
    }
    if (hi == 0x8) { v = x | y; flags(v, size); }
    else if (hi == 0xc) { v = x & y; flags(v, size); }
    else if (hi == 0xd) v = addsub(x, y, size, 0);
    else v = addsub(x, y, size, 1);
    if (hi == 0xb)
        return;
    if (to_ea)
        put(l, size, v);
    else
        setd(r, size, v);
}

static void immediate(u32 op)
{
    int kind = op >> 9 & 7, size = 1 << (op >> 6 & 3);
    u32 imm = size == 4 ? fetch(4) : fetch(2) & mask_of[size];
    struct loc l = ea(op >> 3 & 7, op & 7, size);
    u32 x = get(l, size), v;
    switch (kind) {
    case 0: v = x | imm; flags(v, size); break;
    case 1: v = x & imm; flags(v, size); break;
    case 5: v = x ^ imm; flags(v, size); break;
    case 2: v = addsub(x, imm, size, 1); break;
    case 3: v = addsub(x, imm, size, 0); break;
    default: addsub(x, imm, size, 1); return;
    }
    put(l, size, v);
}

static void bitop(u32 op, u32 bit)
{
    int mode = op >> 3 & 7, reg = op & 7;
    int size = mode == 0 ? 4 : 1;
    bit &= size * 8 - 1;
    struct loc l = ea(mode, reg, size);
    u32 v = get(l, size);
    fz = !(v >> bit & 1);
    int kind = op >> 6 & 3;
    if (kind) {
        if (kind == 1) v ^= 1u << bit;
        else if (kind == 2) v &= ~(1u << bit);
        else v |= 1u << bit;
        put(l, size, v);
    }
}

static void shift(u32 op)
{
    int size = 1 << (op >> 6 & 3), r = op & 7;
    int count = op & 0x20 ? (int)(d[op >> 9 & 7] & 63) : ((op >> 9 & 7) ? (int)(op >> 9 & 7) : 8);
    int kind = op >> 3 & 3, left = (op & 0x100) != 0, bits = size * 8;
    u32 m = mask_of[size], v = d[r] & m;
    if (kind <= 1) {
        if (left)
            v = count >= 32 ? 0 : (v << count) & m;
        else if (kind == 0)
            v = (u32)(sext(v, size) >> (count > 31 ? 31 : count)) & m;
        else
            v = count >= 32 ? 0 : v >> count;
    } else if (kind == 3) {
        count %= bits;
        if (count)
            v = left ? ((v << count) | (v >> (bits - count))) & m : ((v >> count) | (v << (bits - count))) & m;
    } else {
        failed = 1;
        return;
    }
    setd(r, size, v);
    flags(v, size);
}

static void step(void)
{
    u32 here = pc;
    u32 op = fetch(2);
    int hi = op >> 12;
    if (op == 0x4e75) {
        pc = rd(a[7], 4);
        a[7] += 4;
    } else if (op == 0x4e71) {
    } else if (hi == 1 || hi == 2 || hi == 3) {
        int size = hi == 1 ? 1 : hi == 3 ? 2 : 4;
        u32 v = get(ea(op >> 3 & 7, op & 7, size), size);
        int dmode = op >> 6 & 7, dreg = op >> 9 & 7;
        if (dmode == 1)
            a[dreg] = (u32)sext(v, size);
        else {
            put(ea(dmode, dreg, size), size, v);
            flags(v, size);
        }
    } else if ((op & 0xf100) == 0x7000) {
        u32 v = (u32)sext(op & 0xff, 1);
        d[op >> 9 & 7] = v;
        flags(v, 4);
    } else if ((op & 0xffb8) == 0x4880) {
        int r = op & 7;
        if (op & 0x40)
            d[r] = (u32)sext(d[r], 2);
        else
            setd(r, 2, (u32)sext(d[r], 1));
    } else if ((op & 0xfff8) == 0x4840) {
        int r = op & 7;
        d[r] = d[r] >> 16 | d[r] << 16;
    } else if ((op & 0xfb80) == 0x4880) {
        movem(op);
    } else if ((op & 0xf1c0) == 0x41c0) {
        a[op >> 9 & 7] = ea(op >> 3 & 7, op & 7, 4).v;
    } else if (((op & 0xff00) == 0x4200 || (op & 0xff00) == 0x4a00) && (op & 0xc0) != 0xc0) {
        int size = 1 << (op >> 6 & 3);
        struct loc l = ea(op >> 3 & 7, op & 7, size);
        if ((op & 0xff00) == 0x4200) {
            put(l, size, 0);
            flags(0, size);
        } else
            flags(get(l, size), size);
    } else if ((op & 0xf0c0) == 0xd0c0 || (op & 0xf0c0) == 0x90c0) {
        int size = op & 0x100 ? 4 : 2;
        s32 v = sext(get(ea(op >> 3 & 7, op & 7, size), size), size);
        int r = op >> 9 & 7;
        a[r] += hi == 0xd ? (u32)v : (u32)-v;
    } else if ((op & 0xf1c0) == 0xc0c0) {
        u32 v = get(ea(op >> 3 & 7, op & 7, 2), 2);
        int r = op >> 9 & 7;
        d[r] = (d[r] & 0xffff) * v;
        flags(d[r], 4);
    } else if ((hi == 8 || hi == 9 || hi == 0xb || hi == 0xc || hi == 0xd) && (op & 0xc0) != 0xc0) {
        alu(op);
    } else if (hi == 0xb && (op & 0xc0) == 0xc0) {
        int size = op & 0x100 ? 4 : 2;
        s32 v = sext(get(ea(op >> 3 & 7, op & 7, size), size), size);
        addsub(a[op >> 9 & 7], (u32)v, 4, 1);
    } else if (hi == 0xe && (op & 0xc0) != 0xc0) {
        shift(op);
    } else if (hi == 5 && (op & 0xf8) == 0xc8) {
        s32 disp = sext(fetch(2), 2);
        if (!cond(op >> 8 & 15)) {
            int r = op & 7;
            u32 cnt = (d[r] - 1) & 0xffff;
            setd(r, 2, cnt);
            if (cnt != 0xffff)
                pc = here + 2 + (u32)disp;
        }
    } else if (hi == 5 && (op & 0xc0) != 0xc0) {
        int size = 1 << (op >> 6 & 3);
        u32 q = (op >> 9 & 7) ? (op >> 9 & 7) : 8;
        int mode = op >> 3 & 7, reg = op & 7;
        if (mode == 1)
            a[reg] += op & 0x100 ? (u32)-(s32)q : q;
        else {
            struct loc l = ea(mode, reg, size);
            put(l, size, addsub(get(l, size), q, size, (op & 0x100) != 0));
        }
    } else if (hi == 6) {
        s32 disp = sext(op & 0xff, 1);
        if (disp == 0)
            disp = sext(fetch(2), 2);
        int cc = op >> 8 & 15;
        if (cc == 1) {
            a[7] -= 4;
            wr(a[7], 4, pc);
            pc = here + 2 + (u32)disp;
        } else if (cond(cc))
            pc = here + 2 + (u32)disp;
    } else if ((op & 0xf100) == 0 && (op & 0xc0) != 0xc0 && (op >> 9 & 7) != 4 && (op >> 9 & 7) != 7) {
        immediate(op);
    } else if ((op & 0xff00) == 0x0800) {
        bitop(op, fetch(2) & 0xff);
    } else if ((op & 0xf100) == 0x0100 && (op >> 3 & 7) != 1) {
        bitop(op, d[op >> 9 & 7]);
    } else {
        failed = 1;
        LOG("m68k: unknown opcode %04x at %08x\n", op, here);
    }
}

static u8 stack[4096];
static u8 lines_be[1024 * 4];

void port_call_sprite(long code, long x, long y, long base, long *lines)
{
    if (!code)
        return;
    port_enter();
    for (int i = -256; i < 768; i++)
        wbe32(lines_be + (i + 256) * 4, (u32)(i >= 0 && i < 240 ? lines[i] : lines[0] + i * 0x1a0));
    memset(d, 0, sizeof d);
    memset(a, 0, sizeof a);
    fn = fz = fv = fc = 0;
    failed = 0;
    a[7] = (u32)(uintptr_t)(stack + sizeof stack);
    u32 args[4] = {(u32)x, (u32)y, (u32)base, (u32)(uintptr_t)(lines_be + 256 * 4)};
    for (int i = 3; i >= 0; i--) {
        a[7] -= 4;
        wr(a[7], 4, args[i]);
    }
    a[7] -= 4;
    wr(a[7], 4, RETURN);
    pc = (u32)code;
    for (int steps = 0; pc != RETURN && !failed; steps++) {
        if (steps > 200000) {
            LOG("m68k: sprite at %08lx runs too long\n", code);
            break;
        }
        step();
    }
    port_leave();
}
