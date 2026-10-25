#include "game.h"

unsigned char check_for_key(long door, long room)
{
    if (room == 23 && door == 89 && !check_experience(0x81))
        return 0;
    if (room == 14 && door == 79 && !check_experience(0x82))
        return 0;
    if (room == 30 && door == 79 && !check_experience(0x83))
        return 0;
    if (room == 0 && door == 84 && !check_experience(0x84))
        return 0;
    if (room == 47 && door == 84 && !check_experience(0x85))
        return 0;
    if (room == 17 && door == 84 && !check_experience(0x86))
        return 0;
    if (room == 54 && door == 79 && !check_experience(0x87))
        return 0;
    if (room == 68 && door == 79 && !check_experience(0x88))
        return 0;
    if (room == 23 && door == 77 && !check_experience(0x89))
        return 0;
    if (room == 39 && door == 79 && !check_experience(0x8a))
        return 0;
    if (room == 7 && door == 79 && !check_experience(0x8b))
        return 0;
    if (room == 8 && door == 79 && !check_experience(0x8c))
        return 0;
    if (room == 52 && door == 79 && !check_experience(0x8d))
        return 0;
    if (room == 56 && door == 84 && !check_experience(0x8e))
        return 0;
    if (room == 70 && door == 79 && !check_experience(0x8f))
        return 0;
    if (room == 71 && door == 84 && !check_experience(0x90))
        return 0;
    if (room == 74 && door == 79 && !check_experience(0x91))
        return 0;
    return !(room == 53 && (door == 91 || door == 89) && !check_experience(0x92));
}
