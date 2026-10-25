#include "game.h"

long can_open_door(long door, long room)
{
    long sign_offset;
    unsigned char ok = 1;

    if (door_open)
        return 1;
    ok = check_for_key(door, room);
    if (!ok) {
        sign_offset = plane_a_buffer * 99840;
        sign_offset = sign_offset + 32;
        bring_up_locked_sign(sign_offset);
        wait_for_button_press_music();
        pointer_buttons = 0;
    }
    return ok;
}
