#include "game.h"

char *mm_text[7][6];

void mmenu_redraw(void)
{
    long i;
    long saved;
    long maxw;
    long w;
    char *text;
    long color;

    maxw = 0;
    w = 0;
    color_font_draw(0xc0 - text_width(mm_titles[mm_choice]) / 2, font_height + 25, mm_titles[mm_choice], plane_a_buf0,
                    (unsigned char)mm_title_color);
    saved = draw_target;
    draw_target = plane_a_buf0;
    draw_line(0xc0 - text_width(mm_titles[mm_choice]) / 2, font_height + 25 + font_height - 4,
            text_width(mm_titles[mm_choice]) / 2 + 0xc0, font_height + 25 + font_height - 4, 0x41);
    draw_line(0xc0 - text_width(mm_titles[mm_choice]) / 2, font_height + 25 + font_height - 3,
            text_width(mm_titles[mm_choice]) / 2 + 0xc0, font_height + 25 + font_height - 3, 0x41);
    draw_target = saved;
    i = 0;
    if (!game_started && !mm_choice) {
        w = text_width(mm_start_text);
        if (w > maxw)
            maxw = w;
        i = 1;
    }
    for (; i < mm_row_count; ++i) {
        w = text_width(mm_text[mm_choice][i]);
        if (w > maxw)
            maxw = w;
    }
    if (mm_choice == 2 || mm_choice == 1)
        for (i = 0; i < (unsigned char)(mm_row_count - 1); ++i) {
            if (mm_choice == 2 && slot_used[mm_cursor])
                break;
            if (mm_choice == 1 && !save_flags[mm_cursor])
                break;
            ++mm_cursor;
            ++mm_old_cursor;
        }
    for (i = 0; i < mm_row_count; ++i) {
        if (!game_started && !mm_choice && !i)
            text = mm_start_text;
        else
            text = mm_text[mm_choice][i];
        if (mm_cursor == i)
            color = mm_hilite_color;
        else if (mm_choice == 1 && i < 3 && save_flags[i])
            color = (unsigned char)mm_dim_color;
        else if (mm_choice == 2 && i < 3 && !slot_used[i])
            color = (unsigned char)mm_dim_color;
        else
            color = mm_text_color;
        color_font_draw(0xc0 - maxw / 2, font_height + 25 + font_height + 2 + (font_height + -6) * i, text, plane_a_buf0,
                        color);
    }
    mm_text_x = 0xc0 - maxw / 2;
}

void mmenu_highlight_option(long row, unsigned char color)
{
    if (!game_started && !mm_choice && !row)
        color_font_draw(mm_text_x, font_height + 25 + font_height + 2 + (font_height + -6) * row,
                        mm_start_text, plane_a_buf0, color);
    else
        color_font_draw(mm_text_x, font_height + 25 + font_height + 2 + (font_height + -6) * row,
                        mm_text[mm_choice][row], plane_a_buf0, color);
}
