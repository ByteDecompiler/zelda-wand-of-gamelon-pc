#include "game.h"

void mmenu_init(void)
{
    mm_banner = mm_game_title;
    font_width = 0x1a0;
    mm_banner_color = 0x4d;
    mm_title_color = 0x4e;
    mm_text_color = 0x4f;
    mm_hilite_color = 0x52;
    mm_dim_color = 0x13;
    if (mm_continue) {
        mm_choice = 6;
        mm_banner = mm_no_memory_title;
        mm_banner_color = mm_title_color;
    } else if (disc_errors != 3)
        mm_choice = 0;
    else
        mm_choice = 4;
    mm_done = 0;
    mm_saved = 0;
    mmenu_initial_clear();
    mmenu_initial_draw();
    gc_hide(video_path);
    new_mmenu(mm_choice);
    debounce_trig();
    mm_idle_frames = 0;
    mmenu_loop();
    mmenu_scroll_up();
    debounce_stick();
}
