#include <stdio.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "pc.h"

void display_apply(void);
void input_rebind(void);
const char *slot_text(void);
void hero_toggle(void);
const char *hero_text(void);

int menu_open;
static int page;
static int sel;
static int waiting;

enum { M_RESUME, M_PIXELS, M_SHAPE, M_WHOLE, M_SCAN, M_CURVE, M_MASK, M_FILL, M_FULL, M_VSYNC, M_MUSIC, M_EFFECTS, M_VOICE, M_BGPAUSE, M_SLOT, M_HERO,
       M_CONTROLS, M_QUIT, M_COUNT };

static const char *const action_titles[ACT_COUNT] = {
    "Up", "Down", "Left", "Right", "Button 1 (sword)", "Button 2 (item)", "Fast-forward (hold)", "Pause",
    "Menu", "Quick save", "Quick load", "Slot down", "Slot up", "Screenshot",
};

void menu_toggle(void)
{
    menu_open = !menu_open;
    page = 0;
    sel = 0;
    waiting = 0;
    if (!menu_open)
        settings_save();
}

static int items(void) { return page == 0 ? M_COUNT : ACT_COUNT + 2; }

static void change(int dir)
{
    int vol_step = 10 * dir;
    switch (sel) {
    case M_RESUME: if (dir > 0) menu_toggle(); return;
    case M_PIXELS: settings.pixels = (settings.pixels + 3 + dir) % 3; break;
    case M_SHAPE: settings.square = !settings.square; break;
    case M_WHOLE: settings.whole = !settings.whole; break;
    case M_SCAN: settings.scanlines = (settings.scanlines + 3 + dir) % 3; break;
    case M_CURVE: settings.crt_curve = !settings.crt_curve; break;
    case M_MASK: settings.crt_mask = !settings.crt_mask; break;
    case M_HERO: hero_toggle(); return;
    case M_SLOT: settings.slot = (settings.slot + 9 - 1 + dir) % 9 + 1; break;
    case M_FILL: settings.fill = !settings.fill; break;
    case M_FULL: settings.fullscreen = !settings.fullscreen; break;
    case M_VSYNC: settings.vsync = !settings.vsync; break;
    case M_MUSIC: settings.music = SDL_clamp(settings.music + vol_step, 0, 100); break;
    case M_EFFECTS: settings.effects = SDL_clamp(settings.effects + vol_step, 0, 100); break;
    case M_VOICE: settings.voice = SDL_clamp(settings.voice + vol_step, 0, 100); break;
    case M_BGPAUSE: settings.bg_pause = !settings.bg_pause; break;
    case M_CONTROLS: if (dir > 0) { page = 1; sel = 0; } return;
    case M_QUIT: if (dir > 0) { settings_save(); SDL_Event q = {.type = SDL_EVENT_QUIT}; SDL_PushEvent(&q); } return;
    }
    display_apply();
}

void menu_frame(void)
{
    if (waiting)
        return;
    if (input_pressed(ACT_MENU) || input_pressed(ACT_B2)) {
        if (page == 1) {
            page = 0;
            sel = M_CONTROLS;
        } else
            menu_toggle();
        return;
    }
    if (input_pressed(ACT_UP))
        sel = (sel + items() - 1) % items();
    if (input_pressed(ACT_DOWN))
        sel = (sel + 1) % items();
    if (page == 0) {
        if (input_pressed(ACT_LEFT))
            change(-1);
        if (input_pressed(ACT_RIGHT) || input_pressed(ACT_B1))
            change(1);
    } else if (input_pressed(ACT_B1)) {
        if (sel < ACT_COUNT)
            waiting = sel + 1;
        else if (sel == ACT_COUNT) {
            memcpy(settings.keys, settings_defaults.keys, sizeof settings.keys);
            memcpy(settings.pad, settings_defaults.pad, sizeof settings.pad);
            input_rebind();
            settings_save();
        } else {
            page = 0;
            sel = M_CONTROLS;
        }
    }
}

int menu_key_event(const SDL_Event *e)
{
    if (!menu_open || !waiting)
        return 0;
    int a = waiting - 1;
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat) {
        if (e->key.scancode == SDL_SCANCODE_DELETE || e->key.scancode == SDL_SCANCODE_BACKSPACE)
            settings.keys[a][0] = 0;
        else if (e->key.scancode != SDL_SCANCODE_ESCAPE || a == ACT_MENU)
            snprintf(settings.keys[a], sizeof settings.keys[a], "%s", SDL_GetScancodeName(e->key.scancode));
    } else if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        const char *name = SDL_GetGamepadStringForButton((SDL_GamepadButton)e->gbutton.button);
        if (name)
            snprintf(settings.pad[a], sizeof settings.pad[a], "%s", name);
    } else
        return 0;
    waiting = 0;
    input_rebind();
    settings_save();
    return 1;
}

static void line_text(SDL_Renderer *r, float x, float y, const char *text, int selected)
{
    if (selected) {
        SDL_SetRenderDrawColor(r, 255, 255, 160, 255);
        SDL_RenderDebugText(r, x - 12, y, ">");
    } else
        SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
    SDL_RenderDebugText(r, x, y, text);
}

void menu_draw(SDL_Renderer *r, int w, int h)
{
    if (!menu_open)
        return;
    char text[160];
    float scale = h >= 1000 ? 3.0f : 2.0f;
    float lw = w / scale, lh = h / scale;
    SDL_SetRenderScale(r, scale, scale);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 190);
    SDL_FRect all = {0, 0, lw, lh};
    SDL_RenderFillRect(r, &all);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    float x = lw / 2 - 150, y = lh / 2 - items() * 6 - 18;
    if (x < 16)
        x = 16;
    if (y < 8)
        y = 8;
    SDL_SetRenderDrawColor(r, 255, 255, 160, 255);
    SDL_RenderDebugText(r, x, y, page == 0 ? "SETTINGS" : "CONTROLS");
    y += 20;
    if (page == 0) {
        for (int i = 0; i < M_COUNT; i++) {
            switch (i) {
            case M_RESUME: snprintf(text, sizeof text, "Back to the game"); break;
            case M_PIXELS: snprintf(text, sizeof text, "Pixels: %s", pixel_names[settings.pixels]); break;
            case M_SHAPE: snprintf(text, sizeof text, "Shape: %s", settings.square ? "square pixels" : "TV (4:3)"); break;
            case M_WHOLE: snprintf(text, sizeof text, "Whole-number scaling: %s", settings.whole ? "on" : "off"); break;
            case M_SCAN: snprintf(text, sizeof text, "Scanlines: %s", scanline_names[settings.scanlines]); break;
            case M_CURVE: snprintf(text, sizeof text, "CRT curve: %s", settings.crt_curve ? "on" : "off"); break;
            case M_MASK: snprintf(text, sizeof text, "CRT colour stripes: %s", settings.crt_mask ? "on" : "off"); break;
            case M_HERO: snprintf(text, sizeof text, "Hero: %s", hero_text()); break;
            case M_SLOT: snprintf(text, sizeof text, "Quick save slot: %s", slot_text()); break;
            case M_FILL: snprintf(text, sizeof text, "Fill the 2 black top lines: %s", settings.fill ? "on" : "off"); break;
            case M_FULL: snprintf(text, sizeof text, "Fullscreen: %s", settings.fullscreen ? "on" : "off"); break;
            case M_VSYNC: snprintf(text, sizeof text, "Vsync: %s", settings.vsync ? "on" : "off"); break;
            case M_MUSIC: snprintf(text, sizeof text, "Music volume: %d%%", settings.music); break;
            case M_EFFECTS: snprintf(text, sizeof text, "Effects volume: %d%%", settings.effects); break;
            case M_VOICE: snprintf(text, sizeof text, "Voice and cutscene volume: %d%%", settings.voice); break;
            case M_BGPAUSE: snprintf(text, sizeof text, "Pause in the background: %s", settings.bg_pause ? "on" : "off"); break;
            case M_CONTROLS: snprintf(text, sizeof text, "Controls..."); break;
            case M_QUIT: snprintf(text, sizeof text, "Quit"); break;
            }
            line_text(r, x, y, text, i == sel);
            y += 12;
        }
    } else {
        for (int i = 0; i < ACT_COUNT + 2; i++) {
            if (i < ACT_COUNT) {
                if (waiting == i + 1)
                    snprintf(text, sizeof text, "%-20s press a key or button (Delete clears)", action_titles[i]);
                else
                    snprintf(text, sizeof text, "%-20s %s%s%s", action_titles[i], settings.keys[i],
                             settings.pad[i][0] ? "  / pad: " : "", settings.pad[i]);
            } else
                snprintf(text, sizeof text, i == ACT_COUNT ? "Back to the default controls" : "Back");
            line_text(r, x, y, text, i == sel);
            y += 12;
        }
    }
    SDL_SetRenderScale(r, 1, 1);
}
