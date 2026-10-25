#include "game.h"

void mmenu_choice(long selection)
{
    long slot;
    long result;

    switch (mm_choice) {
    case 0:
        switch (selection) {
        case 0:
            if (!game_started)
                mm_new_game = 1;
            mm_done = 1;
            break;
        case 1:
            new_mmenu(1);
            break;
        case 2:
            new_mmenu(2);
            break;
        case 3:
            new_mmenu(3);
            break;
        case 4:
            if (!mm_saved && game_started && (slot_used[0] || slot_used[1] || slot_used[2]))
                new_mmenu(4);
            else
                new_mmenu(5);
            break;
        case 5:
            mm_done = 1;
            mm_help = 1;
            break;
        }
        break;
    case 1:
        slot = 0;
        switch (selection) {
        case 0:
        case 1:
        case 2:
            slot = selection + 1;
            break;
        case 3:
            new_mmenu(0);
            break;
        }
        if (slot > 0) {
            result = load_game_from_NVR(slot);
            mm_done = 1;
            set_life_hearts();
            lantern_pos = 0;
        }
        break;
    case 2:
        slot = 0;
        switch (selection) {
        case 0:
        case 1:
        case 2:
            slot = selection + 1;
            break;
        case 3:
            if (disc_errors == 3)
                quit_game(0);
            new_mmenu(0);
            break;
        }
        if (slot > 0) {
            result = save_game_to_NVR(slot);
            if (disc_errors == 3)
                quit_game(0);
            mm_saved = 1;
            if (mm_quit_after_save) {
                mm_done = 1;
                mm_quit = 1;
            } else
                new_mmenu(0);
        }
        break;
    case 3:
        switch (selection) {
        case 0:
            mm_done = 1;
            mm_new_game = 1;
            break;
        case 1:
            new_mmenu(0);
            break;
        }
        break;
    case 4:
        switch (selection) {
        case 0:
            mm_quit_after_save = 1;
            new_mmenu(2);
            break;
        case 1:
            if (disc_errors == 3)
                quit_game(0);
            mm_done = 1;
            mm_quit = 1;
            break;
        case 2:
            new_mmenu(0);
            break;
        }
        break;
    case 5:
        switch (selection) {
        case 0:
            mm_done = 1;
            mm_quit = 1;
            break;
        case 1:
            new_mmenu(0);
            break;
        }
        break;
    case 6:
        switch (selection) {
        case 0:
            mm_continue = 0;
            mm_banner = mm_game_title;
            mm_banner_color = 0x4d;
            mmenu_scroll_up();
            mmenu_initial_clear();
            mmenu_initial_draw();
            new_mmenu(0);
            break;
        case 1:
            mm_done = 1;
            mm_quit = 1;
            break;
        }
        break;
    }
}
