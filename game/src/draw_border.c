#include "game.h"

void draw_border(long color)
{
    long top;
    long bottom;
    long saved;
    long i;
    long right;
    long left;

    left = BORDER_X;
    top = 28;
    right = BORDER_X_END;
    bottom = top + 0xc1;
    saved = draw_target;
    draw_target = color;
    for (i = top; i <= bottom; ++i)
        draw_line(left + 3, i, right - 8, i, 20);
    draw_target = saved;
    for (i = top + 12; i <= bottom; i += 12) {
        switch (randomize() % 32) {
        case 0:
        case 1:
        case 2:
            draw_edge(6, left, i, color);
            break;
        case 5:
        case 6:
            draw_edge(8, left, i, color);
            break;
        case 7:
            draw_edge(9, left, i, color);
            break;
        default:
            draw_edge(7, left, i, color);
        }
        switch (randomize() % 32) {
        case 0:
        case 1:
        case 2:
            draw_edge(10, right - 8, i, color);
            break;
        case 5:
        case 6:
            draw_edge(12, right - 8, i, color);
            break;
        case 7:
            draw_edge(13, right - 8, i, color);
            break;
        default:
            draw_edge(11, right - 8, i, color);
        }
    }
    for (i = left + 8; i < right; i += 8) {
        draw_edge(1, i, top, color);
        draw_edge(4, i, bottom, color);
    }
    draw_edge(0, left - 12, top, color);
    draw_edge(1, right - 8, top, color);
    draw_edge(2, right, top, color);
    draw_edge(3, left, bottom, color);
    draw_edge(5, right - 8, bottom, color);
    left = BOXES_X;
    top = 28;
    right = BOXES_X_END;
    bottom = top + 52;
    draw_outlined_box(color, left, top, right, bottom, 12, 24);
    left = BOXES_X;
    top = 0x54;
    right = left + BOXES_SPLIT;
    bottom = top + 0x51;
    draw_outlined_box(color, left, top, right, bottom, 20, 24);
    left = right + 6;
    top = top;
    right = BOXES_X_END;
    bottom = bottom;
    draw_outlined_box(color, left, top, right, bottom, 20, 24);
    left = BOXES_X;
    top = 0xa9;
    right = BOXES_X_END;
    bottom = top + 58;
    draw_outlined_box(color, left, top, right, bottom, 12, 24);
}
