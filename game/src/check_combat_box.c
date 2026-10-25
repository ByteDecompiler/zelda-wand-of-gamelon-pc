#include "game.h"

long check_combat_box(short *box, long box_dir, long sprite_no, long facing)
{
    if (in_combat)
        return 0;
    return check_box(box, box_dir, sprite_no, facing);
}
