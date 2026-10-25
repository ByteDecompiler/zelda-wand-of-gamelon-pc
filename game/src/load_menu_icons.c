#include "game.h"

long menu_icon_tab[24];

void load_menu_icons(long data)
{
    register struct imagery *hdr;
    long menu;
    long font;
    long i;

    menu = data + 0x1fd2;
    font = data + 0x32ce;
    font_glyph_data = data + FONT_GLYPH_OFFSET;
    hdr = (struct imagery *)data;
    for (i = 0; i < 24; ++i)
        item_icons[i] = data + 0x6c + hdr->off[i];
    hdr = (struct imagery *)menu;
    for (i = 0; i < 24; ++i)
        menu_icon_tab[i] = menu + 0x6c + hdr->off[i];
    load_font(font);
    menu_icons_ready = 1;
    font_ready = 1;
}
