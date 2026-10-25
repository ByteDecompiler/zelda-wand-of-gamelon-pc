#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>
#include "pc.h"

static volatile int ptr_x = 384, ptr_y = 240, ptr_buttons;
static SDL_Gamepad *pad;
static float mouse_dx, mouse_dy;
static int mouse_buttons;
static int held[ACT_COUNT], was_held[ACT_COUNT];
int input_blocked;

#define SPEED 6
#define MAX_BIND 8

static struct { SDL_Scancode keys[MAX_BIND]; int nkeys; SDL_GamepadButton pads[MAX_BIND]; int npads; } binds[ACT_COUNT];


void input_rebind(void)
{
    for (int a = 0; a < ACT_COUNT; a++) {
        char list[96], *tok, *save;
        binds[a].nkeys = binds[a].npads = 0;
        snprintf(list, sizeof list, "%s", settings.keys[a]);
        for (tok = strtok_r(list, ",", &save); tok && binds[a].nkeys < MAX_BIND; tok = strtok_r(NULL, ",", &save)) {
            while (*tok == ' ')
                tok++;
            SDL_Scancode sc = SDL_GetScancodeFromName(tok);
            if (sc != SDL_SCANCODE_UNKNOWN)
                binds[a].keys[binds[a].nkeys++] = sc;
            else if (*tok)
                LOG("unknown key name \"%s\" for %s\n", tok, action_names[a]);
        }
        snprintf(list, sizeof list, "%s", settings.pad[a]);
        for (tok = strtok_r(list, ",", &save); tok && binds[a].npads < MAX_BIND; tok = strtok_r(NULL, ",", &save)) {
            while (*tok == ' ')
                tok++;
            SDL_GamepadButton b = SDL_GetGamepadButtonFromString(tok);
            if (b != SDL_GAMEPAD_BUTTON_INVALID)
                binds[a].pads[binds[a].npads++] = b;
        }
    }
}

void state_park(void);
void state_add(void *p, u32 size);

void input_state(void)
{
    state_add((void *)&ptr_x, sizeof ptr_x);
    state_add((void *)&ptr_y, sizeof ptr_y);
}

void pt_coord(long path, long *buttons, long *x, long *y)
{
    (void)path;
    state_park();
    SDL_Delay(1);
    if (buttons)
        *buttons = ptr_buttons;
    if (x)
        *x = ptr_x;
    if (y)
        *y = ptr_y;
}

void pt_pos(long path, long x, long y) { (void)path; ptr_x = (int)x; ptr_y = (int)y; }
void pt_org(long path, long x, long y) { (void)path; (void)x; (void)y; }

void input_event(void *ev)
{
    SDL_Event *e = ev;
    switch (e->type) {
    case SDL_EVENT_GAMEPAD_ADDED:
        if (!pad)
            pad = SDL_OpenGamepad(e->gdevice.which);
        break;
    case SDL_EVENT_GAMEPAD_REMOVED:
        if (pad && SDL_GetGamepadID(pad) == e->gdevice.which) {
            SDL_CloseGamepad(pad);
            pad = NULL;
        }
        break;
    case SDL_EVENT_MOUSE_MOTION:
        mouse_dx += e->motion.xrel;
        mouse_dy += e->motion.yrel;
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        int bit = e->button.button == SDL_BUTTON_LEFT ? 1 : e->button.button == SDL_BUTTON_RIGHT ? 2 : 0;
        if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            mouse_buttons |= bit;
        else
            mouse_buttons &= ~bit;
        break;
    }
    default:
        break;
    }
}

int input_held(int a) { return held[a]; }
int input_pressed(int a) { return held[a] && !was_held[a]; }

void input_frame(void)
{
    const bool *k = SDL_GetKeyboardState(NULL);
    memcpy(was_held, held, sizeof held);
    for (int a = 0; a < ACT_COUNT; a++) {
        held[a] = 0;
        for (int i = 0; i < binds[a].nkeys; i++)
            if (k[binds[a].keys[i]])
                held[a] = 1;
        for (int i = 0; pad && i < binds[a].npads; i++)
            if (SDL_GetGamepadButton(pad, binds[a].pads[i]))
                held[a] = 1;
    }
    if (k[SDL_SCANCODE_LALT] || k[SDL_SCANCODE_RALT])
        for (int i = 0; i < binds[ACT_B1].nkeys; i++)
            if (binds[ACT_B1].keys[i] == SDL_SCANCODE_RETURN && k[SDL_SCANCODE_RETURN])
                held[ACT_B1] = 0;
    if (pad) {
        int ax = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTX), ay = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTY);
        if (ax < -12000) held[ACT_LEFT] = 1;
        if (ax > 12000) held[ACT_RIGHT] = 1;
        if (ay < -12000) held[ACT_UP] = 1;
        if (ay > 12000) held[ACT_DOWN] = 1;
    }
    if (input_blocked) {
        ptr_buttons = 0;
        mouse_dx = mouse_dy = 0;
        return;
    }
    int dx = held[ACT_RIGHT] - held[ACT_LEFT], dy = held[ACT_DOWN] - held[ACT_UP];
    int b = mouse_buttons | (held[ACT_B1] ? 1 : 0) | (held[ACT_B2] ? 2 : 0);
    int x = ptr_x + dx * SPEED + (int)mouse_dx, y = ptr_y + dy * SPEED + (int)mouse_dy;
    mouse_dx -= (int)mouse_dx;
    mouse_dy -= (int)mouse_dy;
    ptr_x = x < 0 ? 0 : x > 767 ? 767 : x;
    ptr_y = y < 0 ? 0 : y > 479 ? 479 : y;
    ptr_buttons = b;
}
