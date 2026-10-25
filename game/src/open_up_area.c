#include "game.h"

void open_up_area(long area, long show)
{
    unsigned char order[12];

    order[11] = 7;
    order[10] = 6;
    order[9] = 8;
    order[8] = 5;
    order[7] = 10;
    order[6] = 9;
    order[5] = 2;
    order[4] = 3;
    order[3] = 4;
    order[2] = 13;
    order[1] = 14;
    order[0] = 1;
    if (areas[order[area - 18]] != 1 && show)
        set_new_map(labels[order[area - 18]].hot_x, labels[order[area - 18]].hot_y,
                    labels[order[area - 18]].hot_w, labels[order[area - 18]].hot_h);
    areas[order[area - 18]] = 1;
}
