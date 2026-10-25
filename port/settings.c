#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "pc.h"

#define SETTINGS_FILE "zelda_port.ini"

const struct settings settings_defaults = {
    .pixels = 0, .square = 0, .whole = 0, .fullscreen = 0, .vsync = 1, .scanlines = 0, .fill = 1,
    .crt_curve = 0, .crt_mask = 0,
    .music = 100, .effects = 100, .voice = 100, .bg_pause = 1, .slot = 1,
    .win_x = -1, .win_y = -1, .win_w = 960, .win_h = 720,
    .keys = {
        [ACT_UP] = "Up,W", [ACT_DOWN] = "Down,S", [ACT_LEFT] = "Left,A", [ACT_RIGHT] = "Right,D",
        [ACT_B1] = "Z,Space,J,Return", [ACT_B2] = "X,Left Ctrl,K",
        [ACT_FAST] = "Tab", [ACT_PAUSE] = "P,Pause", [ACT_MENU] = "Escape",
        [ACT_SAVE] = "F5", [ACT_LOAD] = "F9", [ACT_SLOT_DOWN] = "F6", [ACT_SLOT_UP] = "F7", [ACT_SHOT] = "F12",
    },
    .pad = {
        [ACT_UP] = "dpup", [ACT_DOWN] = "dpdown", [ACT_LEFT] = "dpleft", [ACT_RIGHT] = "dpright",
        [ACT_B1] = "a", [ACT_B2] = "b,x", [ACT_FAST] = "rightshoulder", [ACT_PAUSE] = "",
        [ACT_MENU] = "start", [ACT_SAVE] = "", [ACT_LOAD] = "", [ACT_SLOT_DOWN] = "", [ACT_SLOT_UP] = "",
        [ACT_SHOT] = "",
    },
};

struct settings settings;

const char *const pixel_names[3] = {"sharp", "nearest", "smooth"};
const char *const scanline_names[3] = {"off", "light", "strong"};
const char *const action_names[ACT_COUNT] = {
    "up", "down", "left", "right", "button1", "button2", "fast_forward", "pause", "menu", "quick_save", "quick_load",
    "slot_down", "slot_up", "screenshot",
};

static int choice(const char *v, const char *const *names, int n, int old)
{
    for (int i = 0; i < n; i++)
        if (strcmp(v, names[i]) == 0)
            return i;
    return old;
}

static int onoff(const char *v, int old)
{
    return strcmp(v, "on") == 0 ? 1 : strcmp(v, "off") == 0 ? 0 : old;
}

static int percent(const char *v)
{
    int n = atoi(v);
    return n < 0 ? 0 : n > 100 ? 100 : n;
}

void settings_load(void)
{
    settings = settings_defaults;
    FILE *f = fopen(SETTINGS_FILE, "r");
    char line[256], key[64], value[192];
    if (!f)
        return;
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || sscanf(line, " %63[^= ] = %191[^\r\n]", key, value) < 1)
            continue;
        if (sscanf(line, " %63[^= ] = %191[^\r\n]", key, value) == 1)
            value[0] = 0;
        if (strcmp(key, "pixels") == 0) settings.pixels = choice(value, pixel_names, 3, settings.pixels);
        else if (strcmp(key, "shape") == 0) settings.square = strcmp(value, "square") == 0;
        else if (strcmp(key, "whole_number_scaling") == 0) settings.whole = onoff(value, settings.whole);
        else if (strcmp(key, "fullscreen") == 0) settings.fullscreen = onoff(value, settings.fullscreen);
        else if (strcmp(key, "scanlines") == 0) settings.scanlines = choice(value, scanline_names, 3, settings.scanlines);
        else if (strcmp(key, "vsync") == 0) settings.vsync = onoff(value, settings.vsync);
        else if (strcmp(key, "crt_curve") == 0) settings.crt_curve = onoff(value, settings.crt_curve);
        else if (strcmp(key, "crt_mask") == 0) settings.crt_mask = onoff(value, settings.crt_mask);
        else if (strcmp(key, "quick_save_slot") == 0) settings.slot = SDL_clamp(atoi(value), 1, 9);
        else if (strcmp(key, "fill_top_lines") == 0) settings.fill = onoff(value, settings.fill);
        else if (strcmp(key, "music_volume") == 0) settings.music = percent(value);
        else if (strcmp(key, "effects_volume") == 0) settings.effects = percent(value);
        else if (strcmp(key, "voice_volume") == 0) settings.voice = percent(value);
        else if (strcmp(key, "pause_in_background") == 0) settings.bg_pause = onoff(value, settings.bg_pause);
        else if (strcmp(key, "hero") == 0) settings.hero = strcmp(value, "link") == 0;
        else if (strcmp(key, "link_disc") == 0) snprintf(settings.link_disc, sizeof settings.link_disc, "%s", value);
        else if (strcmp(key, "window") == 0)
            sscanf(value, "%d %d %d %d", &settings.win_x, &settings.win_y, &settings.win_w, &settings.win_h);
        else
            for (int a = 0; a < ACT_COUNT; a++) {
                char name[80];
                snprintf(name, sizeof name, "key_%s", action_names[a]);
                if (strcmp(key, name) == 0)
                    snprintf(settings.keys[a], sizeof settings.keys[a], "%s", value);
                snprintf(name, sizeof name, "pad_%s", action_names[a]);
                if (strcmp(key, name) == 0)
                    snprintf(settings.pad[a], sizeof settings.pad[a], "%s", value);
            }
    }
    fclose(f);
    if (settings.win_w < 384 || settings.win_h < 240) {
        settings.win_w = 960;
        settings.win_h = 720;
    }
}

void settings_save(void)
{
    FILE *f = fopen(SETTINGS_FILE, "w");
    if (!f)
        return;
    fprintf(f, "# Settings of the Zelda port. The game's menu (Esc) changes most of them.\n\n");
    fprintf(f, "# sharp, nearest or smooth\npixels = %s\n", pixel_names[settings.pixels]);
    fprintf(f, "# tv (4:3) or square\nshape = %s\n", settings.square ? "square" : "tv");
    fprintf(f, "whole_number_scaling = %s\n", settings.whole ? "on" : "off");
    fprintf(f, "fullscreen = %s\n", settings.fullscreen ? "on" : "off");
    fprintf(f, "# on stops the picture tearing\nvsync = %s\n", settings.vsync ? "on" : "off");
    fprintf(f, "# off, light or strong\nscanlines = %s\n", scanline_names[settings.scanlines]);
    fprintf(f, "# the look of an old TV: a curved screen, and its colour stripes\n");
    fprintf(f, "crt_curve = %s\ncrt_mask = %s\n", settings.crt_curve ? "on" : "off", settings.crt_mask ? "on" : "off");
    fprintf(f, "# the CD-i leaves the top 2 lines black; on moves the picture up to fill them\n");
    fprintf(f, "fill_top_lines = %s\n\n", settings.fill ? "on" : "off");
    fprintf(f, "# 0 to 100\nmusic_volume = %d\neffects_volume = %d\nvoice_volume = %d\n\n",
            settings.music, settings.effects, settings.voice);
    fprintf(f, "pause_in_background = %s\n", settings.bg_pause ? "on" : "off");
    fprintf(f, "# 1 to 9\nquick_save_slot = %d\n\n", settings.slot);
    fprintf(f, "# zelda or link: play as Link, with his pictures from a Link: The Faces of Evil disc\n");
    fprintf(f, "hero = %s\n", settings.hero ? "link" : "zelda");
    fprintf(f, "# the Link disc image, if it isn't next to zelda.exe or in the disc folder\n");
    fprintf(f, "link_disc = %s\n\n", settings.link_disc);
    fprintf(f, "# x y width height\nwindow = %d %d %d %d\n\n", settings.win_x, settings.win_y, settings.win_w, settings.win_h);
    fprintf(f, "# Keys: SDL key names, separated by commas (\"Left Ctrl\", \"Keypad 8\", \"F5\", ...)\n");
    for (int a = 0; a < ACT_COUNT; a++)
        fprintf(f, "key_%s = %s\n", action_names[a], settings.keys[a]);
    fprintf(f, "\n# Gamepad buttons: a, b, x, y, back, start, leftshoulder, rightshoulder, dpup, dpdown, dpleft, dpright, ...\n");
    for (int a = 0; a < ACT_COUNT; a++)
        fprintf(f, "pad_%s = %s\n", action_names[a], settings.pad[a]);
    fclose(f);
}
