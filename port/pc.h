#ifndef PC_H
#define PC_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int16_t s16;
typedef int32_t s32;
typedef int8_t s8;
typedef uint64_t u64;

static inline u16 be16(const void *p) { const u8 *b = p; return (u16)(b[0] << 8 | b[1]); }
static inline u32 be32(const void *p) { const u8 *b = p; return (u32)b[0] << 24 | (u32)b[1] << 16 | (u32)b[2] << 8 | b[3]; }
static inline void wbe16(void *p, u16 v) { u8 *b = p; b[0] = v >> 8; b[1] = (u8)v; }
static inline void wbe32(void *p, u32 v) { u8 *b = p; b[0] = v >> 24; b[1] = v >> 16; b[2] = v >> 8; b[3] = (u8)v; }
static inline u16 swap16(u16 v) { return (u16)(v >> 8 | v << 8); }
static inline u32 swap32(u32 v) { return v >> 24 | (v >> 8 & 0xff00) | (v << 8 & 0xff0000) | v << 24; }

void fatal(const char *fmt, ...);
void logf_(const char *fmt, ...);
#define LOG(...) logf_(__VA_ARGS__)

#define SECTOR_RAW 2352
struct sector {
    u8 file, channel, submode, coding;
    int form2, eor, eof, video, audio, data;
    const u8 *payload;
    int size;
};
int disc_open(const char *path);
void disc_select(int which);
int disc_find(const char *program, char *path, int n);
int disc_file(const char *name, u32 *first, u32 *size);
int disc_read(u32 lba, u8 *raw);
void disc_parse(const u8 *raw, struct sector *s);
u8 *disc_load_file(const char *name, u32 *size);

extern u8 *cdi_mem;
void mem_init(void);
void *mem_alloc(u32 size, int color);
void mem_free(void *p);
static inline u8 *mem24(u32 addr) { return cdi_mem + (addr & 0xffffff); }
void a5_init(const u8 *module, u32 size);

void signal_send(int sig);
void port_enter(void);
void port_leave(void);
extern void (*game_signal_handler)(long sig);

void video_init(void);
void video_render(u32 *rgb);
void video_frame(void);
#define SCREEN_W 384
#define SCREEN_H 240

void play_tick(void);
int play_path_open(const char *name);

void input_event(void *sdl_event);
void input_frame(void);

void audio_init(void);
void audio_tick(void);
void os9_tick(double seconds);

enum { ACT_UP, ACT_DOWN, ACT_LEFT, ACT_RIGHT, ACT_B1, ACT_B2, ACT_FAST, ACT_PAUSE, ACT_MENU, ACT_SAVE, ACT_LOAD,
       ACT_SLOT_DOWN, ACT_SLOT_UP, ACT_SHOT, ACT_COUNT };
struct settings {
    int pixels;
    int square;
    int whole;
    int fullscreen;
    int vsync;
    int scanlines;
    int crt_curve;
    int crt_mask;
    int fill;
    int music, effects, voice;
    int bg_pause;
    int slot;
    int hero;
    char link_disc[260];
    int win_x, win_y, win_w, win_h;
    char keys[ACT_COUNT][96];
    char pad[ACT_COUNT][64];
};
extern struct settings settings;
extern const struct settings settings_defaults;
extern const char *const pixel_names[3], *const scanline_names[3], *const action_names[ACT_COUNT];
void settings_load(void);
void settings_save(void);

int input_held(int action);
int input_pressed(int action);

void audio_pause(int paused);
extern volatile int fast_forward;

#endif
