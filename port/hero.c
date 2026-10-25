#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"

#define PICS_SIZE 0x6000
#define HITS_SIZE 0x570
#define LINK_STARTUP 0xae
#define P(x) ((u8 *)(uintptr_t)(u32)(x))

extern s32 zelda_imagery, cmaps[], *sprite_frames, cur_sprite;
void display_message(const char *text);
void state_add(void *p, u32 size);

static u8 link_pics[PICS_SIZE], link_hits[HITS_SIZE], zelda_pics[PICS_SIZE], zelda_hits[HITS_SIZE];
static int have_link;
static int have_zelda;
static int hero_now;

static int read_link(void)
{
    u32 first, size;
    u8 raw[SECTOR_RAW];
    struct sector s;
    int pics = 0, hits = 0;
    if (!disc_file("ldata.rtr", &first, &size))
        return 0;
    for (u32 n = LINK_STARTUP; n * 2048 < size; n++) {
        if (!disc_read(first + n, raw))
            return 0;
        disc_parse(raw, &s);
        if (s.data && s.channel == 4 && pics + s.size <= PICS_SIZE) {
            memcpy(link_pics + pics, s.payload, s.size);
            pics += s.size;
        } else if (s.data && s.channel == 5 && hits < HITS_SIZE) {
            int k = s.size < HITS_SIZE - hits ? s.size : HITS_SIZE - hits;
            memcpy(link_hits + hits, s.payload, k);
            hits += k;
        }
        if (s.eor)
            break;
    }
    return be32(link_pics) == 0x50414330 && be32(link_pics + 8) == 0x6c && hits == HITS_SIZE;
}

void hero_init(void)
{
    char path[260];
    u32 first, size;
    disc_select(1);
    if (settings.link_disc[0] && disc_open(settings.link_disc) && disc_file("cdi_link", &first, &size))
        snprintf(path, sizeof path, "%s", settings.link_disc);
    else if (!disc_find("cdi_link", path, sizeof path))
        path[0] = 0;
    have_link = path[0] && read_link();
    disc_select(0);
    if (path[0])
        LOG("link disc: %s (%s)\n", path, have_link ? "his pictures read" : "couldn't read his pictures");
    state_add(&hero_now, sizeof hero_now);
}

static void put(int hero)
{
    u8 *pics = P(zelda_imagery);
    memcpy(pics, hero ? link_pics : zelda_pics, PICS_SIZE);
    memcpy(P(cmaps[0]), hero ? link_hits : zelda_hits, HITS_SIZE);
    if (sprite_frames)
        for (int i = 0; i < 24; i++) {
            s32 old = sprite_frames[i];
            sprite_frames[i] = zelda_imagery + 0x6c + (s32)be32(pics + 0x0c + 4 * i);
            if (cur_sprite == old)
                cur_sprite = sprite_frames[i];
        }
    hero_now = hero;
}

void port_hero_loaded(void)
{
    memcpy(zelda_pics, P(zelda_imagery), PICS_SIZE);
    memcpy(zelda_hits, P(cmaps[0]), HITS_SIZE);
    have_zelda = 1;
    hero_now = 0;
    if (settings.hero && have_link)
        put(1);
}

void hero_toggle(void)
{
    if (!have_link) {
        display_message("No Link disc found: put its .bin next to zelda.exe");
        return;
    }
    settings.hero = !settings.hero;
    if (have_zelda)
        put(settings.hero);
    settings_save();
}

const char *hero_text(void)
{
    if (!have_link)
        return "Zelda (no Link disc found)";
    return settings.hero ? "Link" : "Zelda";
}

void hero_after_load(void)
{
    if (have_zelda && hero_now != (settings.hero && have_link))
        put(settings.hero && have_link);
}
