#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "pc.h"

#define STATE_DIR "saves"
#define MAX_ITEMS 64

static struct { void *p; u32 size; } items[MAX_ITEMS];
static int nitems;

static volatile int request;
static volatile int parked;

#define CONTEXT_ALL_REGS (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT | CONTEXT_EXTENDED_REGISTERS)

extern u8 a5space[];
extern const s32 a5_below;
void display_message(const char *text);
void audio_after_load(void);
void play_after_load(void);
void os9_after_load(void);
void hero_after_load(void);

void state_add(void *p, u32 size)
{
    if (nitems < MAX_ITEMS) {
        items[nitems].p = p;
        items[nitems].size = size;
        nitems++;
    }
}

void state_park(void)
{
    if (!request)
        return;
    parked = 1;
    while (parked)
        YieldProcessor();
}

static void put_block(FILE *f, const u8 *p, u32 n)
{
    u32 i = 0;
    while (i < n) {
        u32 z = 0;
        while (i + z < n && p[i + z] == 0)
            z++;
        u32 lit = 0;
        while (i + z + lit < n && (p[i + z + lit] != 0 || (i + z + lit + 1 < n && p[i + z + lit + 1] != 0)))
            lit++;
        fwrite(&z, 4, 1, f);
        fwrite(&lit, 4, 1, f);
        fwrite(p + i + z, 1, lit, f);
        i += z + lit;
    }
}

static int get_block(FILE *f, u8 *p, u32 n)
{
    u32 i = 0;
    while (i < n) {
        u32 z, lit;
        if (fread(&z, 4, 1, f) != 1 || fread(&lit, 4, 1, f) != 1 || i + z + lit > n)
            return 0;
        memset(p + i, 0, z);
        if (fread(p + i + z, 1, lit, f) != lit)
            return 0;
        i += z + lit;
    }
    return 1;
}

static u32 build_stamp(void)
{
    u8 *base = (u8 *)GetModuleHandleA(NULL);
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(base + ((IMAGE_DOS_HEADER *)base)->e_lfanew);
    return nt->FileHeader.TimeDateStamp;
}

#define MEM_SIZE 0x1000000

static void path_of(char *out, int n, int slot)
{
    CreateDirectoryA(STATE_DIR, NULL);
    snprintf(out, n, STATE_DIR "\\quick%d.state", slot);
}

const char *slot_text(void)
{
    static char text[48];
    char path[MAX_PATH];
    WIN32_FILE_ATTRIBUTE_DATA fa;
    SYSTEMTIME st;
    FILETIME local;
    static const char *const months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
                                           "Nov", "Dec"};
    path_of(path, sizeof path, settings.slot);
    if (GetFileAttributesExA(path, GetFileExInfoStandard, &fa) && FileTimeToLocalFileTime(&fa.ftLastWriteTime, &local)
        && FileTimeToSystemTime(&local, &st))
        snprintf(text, sizeof text, "%d, saved %d %s %02d:%02d", settings.slot, st.wDay, months[st.wMonth - 1],
                 st.wHour, st.wMinute);
    else
        snprintf(text, sizeof text, "%d, empty", settings.slot);
    return text;
}

static int stop_game(HANDLE thread)
{
    (void)thread;
    request = 1;
    for (int i = 0; i < 1000 && !parked; i++)
        Sleep(1);
    request = 0;
    return parked;
}

void state_save(HANDLE thread, int slot)
{
    char path[MAX_PATH];
    if (!stop_game(thread)) {
        display_message("Can't save right now");
        return;
    }
    path_of(path, sizeof path, slot);
    FILE *f = fopen(path, "wb");
    if (f) {
        u32 hdr[4] = {0x54524f50 , build_stamp(), (u32)(uintptr_t)cdi_mem, (u32)nitems};
        fwrite(hdr, 4, 4, f);
        CONTEXT ctx;
        ctx.ContextFlags = CONTEXT_ALL_REGS;
        SuspendThread(thread);
        GetThreadContext(thread, &ctx);
        ResumeThread(thread);
        fwrite(&ctx, sizeof ctx, 1, f);
        put_block(f, a5space, a5_below + 16);
        for (int i = 0; i < nitems; i++)
            put_block(f, items[i].p, items[i].size);
        put_block(f, cdi_mem, MEM_SIZE);
        fclose(f);
        snprintf(path, sizeof path, "Saved to slot %d", slot);
        display_message(path);
    } else
        display_message("Couldn't write the save");
    parked = 0;
}

void state_load(HANDLE thread, int slot)
{
    char path[MAX_PATH];
    path_of(path, sizeof path, slot);
    FILE *f = fopen(path, "rb");
    if (!f) {
        snprintf(path, sizeof path, "Slot %d is empty", slot);
        display_message(path);
        return;
    }
    u32 hdr[4];
    if (fread(hdr, 4, 4, f) != 4 || hdr[0] != 0x54524f50 || hdr[3] != (u32)nitems) {
        fclose(f);
        display_message("That save isn't from this version");
        return;
    }
    if (hdr[1] != build_stamp() || hdr[2] != (u32)(uintptr_t)cdi_mem) {
        fclose(f);
        display_message(hdr[1] != build_stamp() ? "That save is from another build of zelda.exe"
                                                : "Can't load: memory is at another address this time");
        return;
    }
    if (!stop_game(thread)) {
        fclose(f);
        display_message("Can't load right now");
        return;
    }
    SuspendThread(thread);
    CONTEXT ctx;
    int ok = fread(&ctx, sizeof ctx, 1, f) == 1 && get_block(f, a5space, a5_below + 16);
    for (int i = 0; ok && i < nitems; i++)
        ok = get_block(f, items[i].p, items[i].size);
    ok = ok && get_block(f, cdi_mem, MEM_SIZE);
    fclose(f);
    if (!ok) {
        fatal("The quick save is damaged.");
    }
    audio_after_load();
    play_after_load();
    os9_after_load();
    hero_after_load();
    ctx.ContextFlags = CONTEXT_ALL_REGS;
    SetThreadContext(thread, &ctx);
    parked = 0;
    ResumeThread(thread);
    snprintf(path, sizeof path, "Loaded slot %d", slot);
    display_message(path);
}
