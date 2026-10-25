#include "game.h"

char tool_list[27];

char find_next_item(unsigned char dir, char cur)
{
    if (cur == 99)
        return 99;
    switch (dir) {
    case 1:
    case 7:
    case 8:
        if (cur < 5) {
            --cur;
            if (cur == -1)
                cur = 0;
            if (tool_list[cur] == 99)
                return 99;
        } else {
            if (--cur == 4)
                cur = 5;
            if (tool_list[cur] == 99)
                return 99;
        }
        break;
    case 3:
    case 4:
    case 5:
        if (cur < 5) {
            if (++cur == 5)
                cur = 4;
            if (tool_list[cur] == 99)
                return 99;
        } else {
            if (++cur == 10)
                cur = 9;
            if (tool_list[cur] == 99)
                return 99;
        }
        break;
    case 6:
        if (cur < 5 || cur > 9)
            return 99;
        if (tool_list[cur - 5] != 99)
            return cur - 5;
        return 99;
    case 2:
        if (cur < 5) {
            if (tool_list[cur + 5] != 99)
                return cur + 5;
            return 99;
        }
        return 99;
    case 0:
        return 99;
    }
    return cur;
}
