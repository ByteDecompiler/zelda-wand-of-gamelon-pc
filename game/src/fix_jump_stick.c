#include "game.h"

void fix_jump_stick(struct stick *stick_ev)
{
    if (stick_ev->x == 0 && stick_ev->count++ == 3) {
        stick_ev->y = 8;
        stick_ev->count = 0;
    } else if (stick_ev->y == 8 && stick_ev->z != 1 && stick_ev->x >= 5 && stick_ev->x <= 7) {
        stick_ev->y = 7;
        stick_ev->z = 7;
    } else if (stick_ev->y == 8 && stick_ev->z != 7 && stick_ev->x >= 1 && stick_ev->x <= 3) {
        stick_ev->y = 1;
        stick_ev->z = 1;
    }
}
