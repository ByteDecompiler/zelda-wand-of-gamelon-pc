#include "game.h"

void wait_rubies_sign(void)
{
    rubies_sign_wait = 1;
    not_enough_rubies = 0;
    wait_for_button_press_music();
    pointer_buttons = 0;
}
