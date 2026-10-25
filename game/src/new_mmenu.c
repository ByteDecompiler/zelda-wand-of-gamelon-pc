#include "game.h"

void new_mmenu(unsigned char menu_no)
{
    switch (menu_no) {
    case 0:
        mm_quit_after_save = 0;
        mm_quit = 0;
        mm_help = 0;
        mm_new_game = 0;
    }
    mm_choice = menu_no;
    mm_row_count = mm_rows[menu_no];
    if (help_aborted) {
        mm_cursor = 5;
        help_aborted = 0;
    } else
        mm_cursor = 0;
    mm_old_cursor = 0;
    if (mm_menu_shown && menu_scrolling)
        mmenu_scroll_up_intermediate();
    mmenu_clear_area();
    mmenu_redraw();
    if (!mm_menu_shown)
        mmenu_scroll_down();
    if (!menu_scrolling)
        mmenu_scroll_down_intermediate();
    debounce_stick();
}
