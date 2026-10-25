#include "game.h"

void open_rtrs(void)
{
    data_path = open(data_file_name, 1);
    anim_path = open(anim_file_name, 1);
    init_rtr();
}
