#include "game.h"

void mmenu_initial_draw(void)
{
    long top;
    long bottom;
    long y;
    long right;
    long left;
    long x;
    long saved;

    left = 50;
    top = 13;
    right = 0x14e;
    bottom = 0xd4;
    for (y = top + 12; y <= bottom; y += 12) {
        switch (randomize() % 32) {
        case 0:
        case 1:
        case 2:
            draw_edge(6, left, y, plane_a_buf0);
            break;
        case 3:
        case 4:
            draw_edge(8, left, y, plane_a_buf0);
            break;
        case 5:
            draw_edge(9, left, y, plane_a_buf0);
            break;
        default:
            draw_edge(7, left, y, plane_a_buf0);
        }
        switch (randomize() % 32) {
        case 0:
        case 1:
        case 2:
            draw_edge(10, right - 8, y, plane_a_buf0);
            break;
        case 3:
        case 4:
            draw_edge(12, right - 8, y, plane_a_buf0);
            break;
        case 5:
            draw_edge(13, right - 8, y, plane_a_buf0);
            break;
        default:
            draw_edge(11, right - 8, y, plane_a_buf0);
        }
    }
    for (x = left + 8; x < right; x += 8) {
        draw_edge(1, x, top, plane_a_buf0);
        draw_edge(4, x, bottom, plane_a_buf0);
    }
    draw_edge(0, left - 12, top, plane_a_buf0);
    draw_edge(1, right - 8, top, plane_a_buf0);
    draw_edge(2, right, top, plane_a_buf0);
    draw_edge(3, left, bottom, plane_a_buf0);
    draw_edge(5, right - 8, bottom, plane_a_buf0);
    color_font_draw(0xc0 - text_width((char *)mm_banner) / 2, 25, (char *)mm_banner, plane_a_buf0, (unsigned char)mm_banner_color);
    saved = draw_target;
    draw_target = plane_a_buf0;
    draw_line(0xc0 - text_width((char *)mm_banner) / 2, font_height + 24,
            text_width((char *)mm_banner) / 2 + 0xc0, font_height + 24, 0x41);
    draw_line(0xc0 - text_width((char *)mm_banner) / 2, font_height + 23,
            text_width((char *)mm_banner) / 2 + 0xc0, font_height + 23, 0x41);
    draw_target = saved;
}
