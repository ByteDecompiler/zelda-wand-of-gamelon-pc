#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL3/SDL.h>
#include "pc.h"

static const SDL_ScaleMode filter_modes[] = {SDL_SCALEMODE_PIXELART, SDL_SCALEMODE_NEAREST, SDL_SCALEMODE_LINEAR};

static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Texture *tex;
static char message[64];
static Uint64 message_until;
static int shot_wanted;
static Uint64 mouse_moved;

#define MAX_PATH_LEN 260

void menu_draw(SDL_Renderer *r, int w, int h);

void display_message(const char *text)
{
    snprintf(message, sizeof message, "%s", text);
    message_until = SDL_GetTicks() + 2000;
}

void display_apply(void)
{
    SDL_SetTextureScaleMode(tex, filter_modes[settings.pixels]);
    SDL_SetWindowFullscreen(win, settings.fullscreen);
    SDL_SetRenderVSync(ren, settings.vsync ? 1 : 0);
}

void display_init(SDL_Window *w, SDL_Renderer *r, SDL_Texture *t)
{
    win = w;
    ren = r;
    tex = t;
    display_apply();
}

int display_event(const SDL_Event *e)
{
    char text[64];
    if (e->type == SDL_EVENT_WINDOW_MOVED || e->type == SDL_EVENT_WINDOW_RESIZED) {
        if (!(SDL_GetWindowFlags(win) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_MINIMIZED))) {
            SDL_GetWindowPosition(win, &settings.win_x, &settings.win_y);
            SDL_GetWindowSize(win, &settings.win_w, &settings.win_h);
        }
        return 0;
    }
    if (e->type == SDL_EVENT_MOUSE_MOTION)
        mouse_moved = SDL_GetTicks();
    if (e->type != SDL_EVENT_KEY_DOWN || e->key.repeat)
        return 0;
    switch (e->key.key) {
    case SDLK_F2:
        settings.pixels = (settings.pixels + 1) % 3;
        snprintf(text, sizeof text, "Pixels: %s", pixel_names[settings.pixels]);
        break;
    case SDLK_F3:
        settings.square = !settings.square;
        snprintf(text, sizeof text, "Shape: %s", settings.square ? "square pixels" : "TV (4:3)");
        break;
    case SDLK_F4:
        settings.whole = !settings.whole;
        snprintf(text, sizeof text, "Whole-number scaling: %s", settings.whole ? "on" : "off");
        break;
    case SDLK_F11:
        settings.fullscreen = !settings.fullscreen;
        text[0] = 0;
        break;
    case SDLK_RETURN:
        if (!(e->key.mod & SDL_KMOD_ALT))
            return 0;
        settings.fullscreen = !settings.fullscreen;
        text[0] = 0;
        break;
    default:
        return 0;
    }
    display_apply();
    settings_save();
    if (text[0])
        display_message(text);
    return 1;
}

static SDL_FRect place(int w, int h)
{
    double aspect = settings.square ? (double)SCREEN_W / SCREEN_H : 4.0 / 3.0;
    double pw, ph;
    if (settings.whole) {
        int n = h / SCREEN_H;
        if (settings.square && w / SCREEN_W < n)
            n = w / SCREEN_W;
        while (n > 1 && SCREEN_H * n * aspect > w)
            n--;
        if (n < 1)
            n = 1;
        ph = SCREEN_H * n;
        pw = settings.square ? SCREEN_W * n : ph * aspect;
    } else {
        pw = w;
        ph = w / aspect;
        if (ph > h) {
            ph = h;
            pw = h * aspect;
        }
    }
    SDL_FRect r = {(float)((w - pw) / 2), (float)((h - ph) / 2), (float)pw, (float)ph};
    return r;
}

static void scanlines(const SDL_FRect *dst)
{
    float line = dst->h / SCREEN_H;
    if (line < 2)
        return;
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, settings.scanlines == 1 ? 70 : 140);
    float gap = line * 0.4f;
    for (int y = 0; y < SCREEN_H; y++) {
        SDL_FRect r = {dst->x, dst->y + (y + 1) * line - gap, dst->w, gap};
        SDL_RenderFillRect(ren, &r);
    }
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
}

static void crt_mask(const SDL_FRect *dst)
{
    static SDL_Texture *mask;
    if (dst->h < SCREEN_H * 2)
        return;
    if (!mask) {
        static const u32 px[3] = {0xffc0c0, 0xc0ffc0, 0xc0c0ff};
        mask = SDL_CreateTexture(ren, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STATIC, 3, 1);
        if (!mask)
            return;
        SDL_UpdateTexture(mask, NULL, px, sizeof px);
        SDL_SetTextureScaleMode(mask, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureBlendMode(mask, SDL_BLENDMODE_MOD);
    }
    SDL_RenderTextureTiled(ren, mask, NULL, 1, dst);
}

#define GRID 24
static void crt_curve(SDL_Texture *frame, const SDL_FRect *dst, int w, int h)
{
    static SDL_Vertex v[(GRID + 1) * (GRID + 1)];
    static int idx[GRID * GRID * 6];
    const float k = 0.05f;
    int n = 0;
    for (int j = 0; j <= GRID; j++)
        for (int i = 0; i <= GRID; i++) {
            float u = (float)i / GRID, t = (float)j / GRID, x = u * 2 - 1, y = t * 2 - 1;
            float f = (1 - k * (x * x + y * y)) / (1 - k);
            float shade = 1 - 0.15f * (x * x * x * x + y * y * y * y);
            SDL_Vertex *p = &v[j * (GRID + 1) + i];
            p->position.x = dst->x + dst->w / 2 * (1 + x * f);
            p->position.y = dst->y + dst->h / 2 * (1 + y * f);
            p->tex_coord.x = (dst->x + u * dst->w) / w;
            p->tex_coord.y = (dst->y + t * dst->h) / h;
            p->color = (SDL_FColor){shade, shade, shade, 1};
        }
    for (int j = 0; j < GRID; j++)
        for (int i = 0; i < GRID; i++) {
            int a = j * (GRID + 1) + i, b = a + 1, c = a + GRID + 1, d = c + 1;
            idx[n++] = a; idx[n++] = b; idx[n++] = c;
            idx[n++] = b; idx[n++] = d; idx[n++] = c;
        }
    SDL_RenderGeometry(ren, frame, v, (GRID + 1) * (GRID + 1), idx, n);
}

static void save_screenshot(const SDL_FRect *dst)
{
    SDL_Rect r = {(int)dst->x, (int)dst->y, (int)dst->w, (int)dst->h};
    SDL_Surface *s = SDL_RenderReadPixels(ren, &r);
    if (!s) {
        display_message("Couldn't take the screenshot");
        return;
    }
    char path[MAX_PATH_LEN];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    SDL_CreateDirectory("screenshots");
    for (int n = 1; n < 100; n++) {
        int len = (int)strftime(path, sizeof path, "screenshots/zelda_%Y-%m-%d_%H-%M-%S", tm);
        snprintf(path + len, sizeof path - len, n == 1 ? ".png" : "_%d.png", n);
        if (!SDL_GetPathInfo(path, NULL))
            break;
    }
    display_message(SDL_SavePNG(s, path) ? "Screenshot saved" : "Couldn't save the screenshot");
    SDL_DestroySurface(s);
}

void display_screenshot(void)
{
    shot_wanted = 1;
}

void display_cursor(void)
{
    int show = !settings.fullscreen && mouse_moved && SDL_GetTicks() - mouse_moved < 2000;
    if (show != (int)SDL_CursorVisible()) {
        if (show)
            SDL_ShowCursor();
        else
            SDL_HideCursor();
    }
}

void display_text(int h, const char *text)
{
    float scale = h >= 1000 ? 4.0f : h >= 700 ? 3.0f : 2.0f;
    SDL_SetRenderScale(ren, scale, scale);
    float x = 6, y = 6;
    SDL_FRect box = {x - 3, y - 3, (float)strlen(text) * 8 + 6, 14};
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 170);
    SDL_RenderFillRect(ren, &box);
    SDL_SetRenderDrawColor(ren, 255, 255, 160, 255);
    SDL_RenderDebugText(ren, x, y, text);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
    SDL_SetRenderScale(ren, 1, 1);
}

void display_present(const u32 *pixels, const char *status)
{
    int w, h;
    SDL_UpdateTexture(tex, NULL, pixels, SCREEN_W * 4);
    SDL_GetCurrentRenderOutputSize(ren, &w, &h);
    SDL_FRect dst = place(w, h);
    static SDL_Texture *frame;
    static int frame_w, frame_h;
    if (settings.crt_curve && (!frame || frame_w != w || frame_h != h)) {
        SDL_DestroyTexture(frame);
        frame = SDL_CreateTexture(ren, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
        if (frame)
            SDL_SetTextureScaleMode(frame, SDL_SCALEMODE_LINEAR);
        frame_w = w;
        frame_h = h;
    }
    int curve = settings.crt_curve && frame;
    if (curve)
        SDL_SetRenderTarget(ren, frame);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    SDL_RenderTexture(ren, tex, NULL, &dst);
    if (settings.scanlines)
        scanlines(&dst);
    if (settings.crt_mask)
        crt_mask(&dst);
    if (curve) {
        SDL_SetRenderTarget(ren, NULL);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        crt_curve(frame, &dst, w, h);
    }
    if (shot_wanted) {
        save_screenshot(&dst);
        shot_wanted = 0;
    }
    if (status)
        display_text(h, status);
    else if (message[0] && SDL_GetTicks() < message_until)
        display_text(h, message);
    menu_draw(ren, w, h);
    SDL_RenderPresent(ren);
}
