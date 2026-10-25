#include "game.h"

void retrieve_help_state(long *stick_event, long *button_state)
{
    if (get_input()) {
        help_repeat = 0;
        *button_state = 0;
        *stick_event = 0;
        demo_ptr = demo_buf;
        help_aborted = 1;
        demo_started = 1;
    } else if (!voice_wait) {
        voice_ready = 0;
        if (--help_repeat > 0) {
            *stick_event = help_cmd & 0x1f;
            *button_state = help_cmd >> 5;
        } else {
            help_cmd = *demo_ptr++;
            if (help_cmd == 0xff) {
                set_help_off();
                help_repeat = 0;
                *button_state = 0;
                *stick_event = 0;
                start_music(music_track);
            } else if (help_cmd & 0x80) {
                *stick_event = 0;
                *button_state = 0;
                switch (help_cmd) {
                case 0x80:
                    if (!play_done)
                        voice_wait = 1;
                    else {
                        voice_ready = 1;
                        attenuation(0);
                    }
                    break;
                case 0x81:
                    if (!play_done)
                        voice_wait = 1;
                    else
                        attenuation(0);
                    help_on = 1;
                    break;
                case 0x82:
                    help_on = 0;
                    voice_ready = 1;
                    break;
                case 0x83:
                    sword_level2 = 1;
                    damage_taken = 0;
                    break;
                case 0x84:
                    if (zelda_dir == 2)
                        zelda_dir = 6;
                    else if (zelda_dir == 6)
                        zelda_dir = 2;
                    break;
                case 0x85:
                    not_enough_rubies = 1;
                    break;
                case 0x86:
                    ((char *)(enemy_data + 0x520))[0] = 2;
                    break;
                }
            } else {
                help_repeat = help_cmd;
                help_cmd = *demo_ptr++;
                *stick_event = help_cmd & 0x1f;
                *button_state = help_cmd >> 5;
            }
        }
    } else if (play_done) {
        voice_wait = 0;
        attenuation(0);
        if (!help_on)
            voice_ready = 1;
    }
}
