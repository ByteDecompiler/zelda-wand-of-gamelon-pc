#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "pc.h"

void game_main(void);

void state_add(void *p, u32 size);
void state_park(void);
void state_save(HANDLE thread, int slot);
void state_load(HANDLE thread, int slot);
const char *slot_text(void);
void hero_init(void);
void video_state(void), audio_state(void), play_state(void), os9_state(void), input_state(void);

void display_init(SDL_Window *w, SDL_Renderer *r, SDL_Texture *t);
int display_event(const SDL_Event *e);
void display_present(const u32 *pixels, const char *status);
void display_message(const char *text);
void display_screenshot(void);
void display_cursor(void);

extern int menu_open, input_blocked;
void menu_toggle(void);
void menu_frame(void);
int menu_key_event(const SDL_Event *e);
void input_rebind(void);

void (*game_signal_handler)(long sig);
static HANDLE game_thread;
static DWORD game_thread_id;
static volatile LONG in_port;
static volatile LONG sig_masked;
static CRITICAL_SECTION sig_lock;
static int sig_queue[64];
static int sig_head, sig_tail;
static FILE *log_file;

void fatal(const char *fmt, ...)
{
    char msg[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof msg, fmt, ap);
    va_end(ap);
    if (log_file)
        fprintf(log_file, "fatal: %s\n", msg), fflush(log_file);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Zelda: The Wand of Gamelon", msg, NULL);
    ExitProcess(1);
}

void logf_(const char *fmt, ...)
{
    if (!log_file)
        return;
    port_enter();
    va_list ap;
    va_start(ap, fmt);
    vfprintf(log_file, fmt, ap);
    va_end(ap);
    fflush(log_file);
    port_leave();
}

void port_enter(void)
{
    if (GetCurrentThreadId() == game_thread_id)
        InterlockedIncrement(&in_port);
}

void port_leave(void)
{
    if (GetCurrentThreadId() == game_thread_id)
        InterlockedDecrement(&in_port);
}

void port_idle(void)
{
    state_park();
    Sleep(1);
}

void port_sigmask(long level)
{
    if (level > 0)
        sig_masked = 1;
    else if (level < 0)
        sig_masked = 0;
}

void port_intercept(void (*handler)())
{
    game_signal_handler = (void (*)(long))handler;
}

void signal_send(int sig)
{
    EnterCriticalSection(&sig_lock);
    int next = (sig_tail + 1) % 64;
    if (next != sig_head) {
        sig_queue[sig_tail] = sig;
        sig_tail = next;
    } else
        LOG("signal queue full, lost signal $%x\n", sig);
    LeaveCriticalSection(&sig_lock);
}

static void deliver_signals(void)
{
    if (!game_signal_handler || !game_thread)
        return;
    for (int tries = 0; tries < 64; tries++) {
        EnterCriticalSection(&sig_lock);
        if (sig_head == sig_tail) {
            LeaveCriticalSection(&sig_lock);
            return;
        }
        int sig = sig_queue[sig_head];
        LeaveCriticalSection(&sig_lock);

        SuspendThread(game_thread);
        CONTEXT ctx;
        ctx.ContextFlags = CONTEXT_CONTROL;
        GetThreadContext(game_thread, &ctx);
        if (in_port || sig_masked) {
            ResumeThread(game_thread);
            return;
        }
        EnterCriticalSection(&sig_lock);
        sig_head = (sig_head + 1) % 64;
        LeaveCriticalSection(&sig_lock);
        game_signal_handler(sig);
        ResumeThread(game_thread);
    }
}

static LONG WINAPI crashed(EXCEPTION_POINTERS *e)
{
    DWORD code = e->ExceptionRecord->ExceptionCode;
    if (code != EXCEPTION_ACCESS_VIOLATION && code != EXCEPTION_ILLEGAL_INSTRUCTION && code != EXCEPTION_INT_DIVIDE_BY_ZERO
            && code != EXCEPTION_STACK_OVERFLOW && code != EXCEPTION_PRIV_INSTRUCTION)
        return EXCEPTION_CONTINUE_SEARCH;
    CONTEXT *c = e->ContextRecord;
    if (log_file) {
        fprintf(log_file, "crash: exception %08lx at %08lx (thread %s), reading/writing %08lx\n",
                e->ExceptionRecord->ExceptionCode, (unsigned long)c->Eip,
                GetCurrentThreadId() == game_thread_id ? "game" : "main",
                e->ExceptionRecord->NumberParameters > 1 ? (unsigned long)e->ExceptionRecord->ExceptionInformation[1] : 0);
        fprintf(log_file, "callers:");
        u32 *fp = (u32 *)(uintptr_t)c->Ebp;
        for (int i = 0; i < 24 && fp && !IsBadReadPtr(fp, 8); i++) {
            fprintf(log_file, " %08x", fp[1]);
            fp = (u32 *)(uintptr_t)fp[0];
        }
        fprintf(log_file, "\n");
        fflush(log_file);
    }
    ExitProcess(3);
    return EXCEPTION_EXECUTE_HANDLER;
}

#define GAME_STACK (512 * 1024)
static u8 *game_stack;

static void game_thread_body(void)
{
    game_main();
    ExitProcess(0);
}

static DWORD WINAPI game_thread_main(LPVOID arg)
{
    (void)arg;
    NT_TIB *tib = (NT_TIB *)NtCurrentTeb();
    u8 *top = game_stack + GAME_STACK - 64;
    tib->StackBase = game_stack + GAME_STACK;
    tib->StackLimit = game_stack;
    __asm__ volatile("movl %0, %%esp; call *%1" : : "a"(top), "c"(game_thread_body) : "memory");
    return 0;
}

static const char *find_disc(int argc, char **argv)
{
    static char path[MAX_PATH];
    if (argc > 1)
        return argv[1];
    return disc_find("cdi_zelda", path, sizeof path) ? path : NULL;
}

extern const char game_version[];

static void play_version(u32 modsize, const char *disc_path)
{
    static const struct { u32 size; const char *version, *exe, *name; } versions[] = {
        {134464, "en", "zelda.exe", "English"},
        {134292, "fr", "zelda_fr.exe", "French"},
        {134332, "de", "zelda_de.exe", "German"},
        {134248, "nl", "zelda_nl.exe", "Dutch"},
    };
    for (int i = 0; i < 4; i++) {
        if (modsize != versions[i].size || strcmp(versions[i].version, game_version) == 0)
            continue;
        char exe[MAX_PATH], cmd[MAX_PATH * 2 + 8];
        GetModuleFileNameA(NULL, exe, sizeof exe);
        char *slash = strrchr(exe, '\\');
        snprintf(slash ? slash + 1 : exe, sizeof exe - (slash ? (size_t)(slash + 1 - exe) : 0), "%s", versions[i].exe);
        snprintf(cmd, sizeof cmd, "\"%s\" \"%s\"", exe, disc_path);
        LOG("this is the %s version: starting %s\n", versions[i].name, versions[i].exe);
        fclose(log_file);
        log_file = NULL;
        STARTUPINFOA si = {sizeof si};
        PROCESS_INFORMATION pi;
        if (!CreateProcessA(exe, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
            fatal("This disc has the %s version of the game, which needs %s next to zelda.exe.", versions[i].name,
                  versions[i].exe);
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD code = 0;
        GetExitCodeProcess(pi.hProcess, &code);
        ExitProcess(code);
    }
    for (int i = 0; i < 4; i++)
        if (modsize == versions[i].size)
            return;
    LOG("unknown version of the game (program of %u bytes), trying it as %s\n", modsize, game_version);
}

int main(int argc, char **argv)
{
    log_file = fopen("zelda_port.log", "w");
    InitializeCriticalSection(&sig_lock);
    timeBeginPeriod(1);
    AddVectoredExceptionHandler(1, crashed);
    SDL_SetHint(SDL_HINT_WINDOWS_INTRESOURCE_ICON, "1");
    SDL_SetHint(SDL_HINT_WINDOWS_INTRESOURCE_ICON_SMALL, "1");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
        fatal("SDL couldn't start: %s", SDL_GetError());

    const char *disc_path = find_disc(argc, argv);
    if (!disc_path)
        fatal("Put your disc image (.bin, raw 2352-byte sectors) next to zelda.exe or in a disc folder,\n"
              "or drag it onto zelda.exe.");
    if (!disc_open(disc_path))
        fatal("Couldn't read %s as a CD-i disc image.", disc_path);
    LOG("disc: %s\n", disc_path);

    mem_init();
    u32 modsize;
    u8 *mod = disc_load_file("cdi_zelda", &modsize);
    if (!mod)
        fatal("No cdi_zelda program on this disc: is it Zelda: The Wand of Gamelon?");
    play_version(be32(mod + 4), disc_path);
    a5_init(mod, modsize);
    free(mod);

    settings_load();
    hero_init();
    input_rebind();
    SDL_Window *win = SDL_CreateWindow("Zelda: The Wand of Gamelon", settings.win_w, settings.win_h, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, NULL);
    if (!win || !ren)
        fatal("Couldn't open a window: %s", SDL_GetError());
    if (settings.win_x != -1)
        SDL_SetWindowPosition(win, settings.win_x, settings.win_y);
    SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING,
                                         SCREEN_W, SCREEN_H);
    display_init(win, ren, tex);
    static u32 pixels[SCREEN_W * SCREEN_H];

    video_init();
    audio_init();

    game_stack = mem_alloc(GAME_STACK, 0);
    video_state();
    audio_state();
    play_state();
    os9_state();
    input_state();
    state_add(sig_queue, sizeof sig_queue);
    state_add(&sig_head, sizeof sig_head);
    state_add(&sig_tail, sizeof sig_tail);
    state_add((void *)&sig_masked, sizeof sig_masked);
    game_thread = CreateThread(NULL, 64 * 1024, game_thread_main, NULL, 0, &game_thread_id);

    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 next = SDL_GetPerformanceCounter();
    const Uint64 field = freq * 1001 / 60000;
    int paused = 0, focused = 1, was_stopped = 0, quick_presents = 0;
    double credit = 0;
    for (;;) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                settings_save();
                ExitProcess(0);
            }
            if (e.type == SDL_EVENT_WINDOW_FOCUS_LOST)
                focused = 0;
            if (e.type == SDL_EVENT_WINDOW_FOCUS_GAINED)
                focused = 1;
            if (menu_key_event(&e) || display_event(&e))
                continue;
            input_event(&e);
        }

        Uint64 now = SDL_GetPerformanceCounter();
        int vsync = 0, due = 0;
        float hz = 0;
        if (SDL_GetRenderVSync(ren, &vsync) && vsync && quick_presents < 3) {
            const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(win));
            hz = mode ? mode->refresh_rate : 0;
        }
        static float logged_hz = -1;
        if (hz != logged_hz) {
            LOG(hz > 20 ? "timing: vsync, %.2f Hz\n" : "timing: by the clock\n", hz);
            logged_hz = hz;
        }
        if (hz > 20) {
            credit +=60000.0 / 1001.0 / hz;
            due = (int)credit;
            credit -= due;
            double behind = ((double)now - (double)next) / (double)field;
            if (behind > 2)
                due++;
            else if (behind < -2 && due > 0)
                due--;
            if (behind > 8 || behind < -8)
                next = now;
        } else {
            while (now >= next + (Uint64)due * field && due < 4)
                due++;
            if (due == 4)
                next = now - field * 4;
        }
        next += (Uint64)due * field;

        for (; due > 0; due--) {
            input_frame();
            if (menu_open)
                menu_frame();
            else if (input_pressed(ACT_MENU))
                menu_toggle();
            if (input_pressed(ACT_PAUSE) && !menu_open)
                paused = !paused;
            if (!menu_open && input_pressed(ACT_SAVE))
                state_save(game_thread, settings.slot);
            if (!menu_open && input_pressed(ACT_LOAD))
                state_load(game_thread, settings.slot);
            if (!menu_open && (input_pressed(ACT_SLOT_DOWN) || input_pressed(ACT_SLOT_UP))) {
                char text[64];
                settings.slot = (settings.slot + 9 - 1 + (input_pressed(ACT_SLOT_UP) ? 1 : -1)) % 9 + 1;
                snprintf(text, sizeof text, "Slot %s", slot_text());
                display_message(text);
                settings_save();
            }
            if (input_pressed(ACT_SHOT))
                display_screenshot();
            int stopped = paused || menu_open || (settings.bg_pause && !focused);
            if (stopped != was_stopped) {
                audio_pause(stopped);
                was_stopped = stopped;
            }
            input_blocked = stopped;
            fast_forward = !stopped && input_held(ACT_FAST);

            if (!stopped)
                for (int n = fast_forward ? 4 : 1; n > 0; n--) {
                    play_tick();
                    os9_tick(1001.0 / 60000.0);
                    audio_tick();
                    video_frame();
                    deliver_signals();
                    if (n > 1)
                        Sleep(1);
                }
            if (due > 1)
                Sleep(1);
        }

        display_cursor();
        video_render(pixels);
        Uint64 t = SDL_GetPerformanceCounter();
        display_present(pixels, paused && !menu_open ? "Paused" : fast_forward ? ">> Fast-forward" : NULL);
        now = SDL_GetPerformanceCounter();
        if (hz > 20) {
            quick_presents = now - t < freq / 2000 ? quick_presents + 1 : 0;
        } else {
            if (now < next)
                SDL_DelayPrecise((next - now) * 1000000000ull / freq);
            if (vsync && quick_presents && now - t >= freq / 2000)
                quick_presents = 0;
        }
    }
}
