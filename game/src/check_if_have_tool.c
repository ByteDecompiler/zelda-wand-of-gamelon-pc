#include "game.h"

long check_if_have_tool(long tool_id)
{
    return tool_id == 0 && check_experience(0x81)
        || tool_id == 1 && (check_experience(0x82) || !check_experience(0xa4))
        || tool_id == 2 && check_experience(0x83)
        || tool_id == 3 && check_experience(0x84)
        || tool_id == 4 && check_experience(0x85)
        || tool_id == 5 && (check_experience(0x86) || !check_experience(0xa5))
        || tool_id == 6 && check_experience(0x87)
        || tool_id == 7 && (check_experience(0x88) || !check_experience(0xa6))
        || tool_id == 8 && (check_experience(0x89) || !check_experience(0xa7))
        || tool_id == 9 && (check_experience(0x8a) || !check_experience(0xa8))
        || tool_id == 10 && (check_experience(0x8b) || !check_experience(0xa9))
        || tool_id == 11 && (check_experience(0x8c) || !check_experience(0xaa))
        || tool_id == 12 && (check_experience(0x8d) || !check_experience(0xab))
        || tool_id == 13 && (check_experience(0x8e) || !check_experience(0xac))
        || tool_id == 14 && (check_experience(0x8f) || !check_experience(0xad))
        || tool_id == 15 && (check_experience(0x90) || !check_experience(0xae))
        || tool_id == 16 && (check_experience(0x91) || !check_experience(0xaf))
        || tool_id == 17 && (check_experience(0x92) || !check_experience(0xb0))
        || tool_id == 18 && !check_experience(0xb1)
        || tool_id == 26 && !check_experience(0xb2)
        || tool_id == 52 && (item_counts[17] || sword_level3 || !check_experience(0xa2))
        || tool_id == 53 && (item_counts[0] || item_counts[1] || item_counts[2])
        || tool_id == 48 && shield_level
        || tool_id == 49 && (item_counts[19] || item_counts[4])
        || tool_id == 50 && (item_counts[14] || item_counts[3])
        || tool_id == 51 && (item_counts[15] || item_counts[9])
        || tool_id == 47 && (sword_level2 || sword_level3)
        || tool_id == 54 && (item_counts[10] || !check_experience(0xa3));
}
