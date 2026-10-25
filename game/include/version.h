#ifndef VERSION_H
#define VERSION_H

#if defined(FRENCH) || defined(GERMAN) || defined(DUTCH)
#define NEW_SAVE_CHECK
#endif

#if !defined(FRENCH) && !defined(GERMAN) && !defined(DUTCH)
#define ANIMATION_TESTS
#endif

#if defined(FRENCH)

#define STARTUP_SECTOR      0xb0
#define ROOM_SECTOR         0xcb
#define EFFECTS_SECTOR      0x3c87
#define VOICE_SECTOR        0x394f
#define VOICE_SECTOR_2      0x3afc

#define PLANE_A_EXTRA_SIZE  0x1fc00
#define ROOM_PALETTE_OFFSET 0x16c00
#define MENU_ICONS_SIZE     0x6400
#define FONT_GLYPH_OFFSET   0x5aca
#define CREDITS_LINES       236
#define MAP_CURSOR_START_X  0x100

#define RUBIES_TEXT_X       0x96
#define RUBIES_NUM_X        0x9e
#define LOCKED_BOX_X        0x80
#define LOCKED_BOX_X_END    0x100
#define LOCKED_TEXT_X       0x96
#define CLEAN_BOX_X         0x72
#define CLEAN_BOX_X_END     0x11e
#define CLEAN_TEXT1_X       0x82
#define CLEAN_TEXT2_X       0x98

#define DEATH_BOX_X         0x60
#define DEATH_BOX_Y_END     0x94
#define DEATH_LINK_X        0x31
#define DEATH_TEXT_X        0x68
#define DEATH_NUMBER_X      0xfe
#define DEATH_ERASE_X       200
#define DEATH_ERASE_X_END   264

#define ICON_BOX_X          29
#define ICON_BOX_X2         98
#define ICON_BOX_W          28
#define ICON_COUNT_X        0x3b
#define SELECTED_ICON_X     0xb6
#define SELECTED_ICON_X_END 0xcd
#define ITEM_NAME_X         0xd4
#define QUEST_ITEM_STEP     23
#define SWORD_SHIELD_X      0x116
#define BORDER_X            24
#define BORDER_X_END        0x84
#define BOXES_X             0xb0
#define BOXES_X_END         0x13c
#define BOXES_SPLIT         0x52

#elif defined(GERMAN)

#define STARTUP_SECTOR      0xae
#define ROOM_SECTOR         0xc9
#define EFFECTS_SECTOR      0x3d15
#define VOICE_SECTOR        0x394d
#define VOICE_SECTOR_2      0x3b4a

#define PLANE_A_EXTRA_SIZE  0x1fc00
#define ROOM_PALETTE_OFFSET 0x16c00
#define MENU_ICONS_SIZE     0x6400
#define FONT_GLYPH_OFFSET   0x5aca
#define CREDITS_LINES       236
#define MAP_CURSOR_START_X  0xf0

#define RUBIES_TEXT_X       0x90
#define RUBIES_NUM_X        0x9e
#define LOCKED_BOX_X        0x6e
#define LOCKED_BOX_X_END    0x10e
#define LOCKED_TEXT_X       0x84
#define CLEAN_BOX_X         0x7a
#define CLEAN_BOX_X_END     0x116
#define CLEAN_TEXT1_X       0x90
#define CLEAN_TEXT2_X       0x94

#define DEATH_BOX_X         0x58
#define DEATH_BOX_Y_END     0x9c
#define DEATH_LINK_X        0x32
#define DEATH_TEXT_X        0x62
#define DEATH_NUMBER_X      0x102
#define DEATH_ERASE_X       200
#define DEATH_ERASE_X_END   268

#define ICON_BOX_X          31
#define ICON_BOX_X2         106
#define ICON_BOX_W          30
#define ICON_COUNT_X        0x3f
#define SELECTED_ICON_X     0xb8
#define SELECTED_ICON_X_END 0xd1
#define ITEM_NAME_X         0xd6
#define QUEST_ITEM_STEP     25
#define SWORD_SHIELD_X      0x120
#define BORDER_X            26
#define BORDER_X_END        0x8e
#define BOXES_X             0xb2
#define BOXES_X_END         0x14a
#define BOXES_SPLIT         0x5a

#elif defined(DUTCH)

#define STARTUP_SECTOR      0xb0
#define ROOM_SECTOR         0xcb
#define EFFECTS_SECTOR      0x3cc7
#define VOICE_SECTOR        0x394f
#define VOICE_SECTOR_2      0x3b4c

#define PLANE_A_EXTRA_SIZE  0x1fc00
#define ROOM_PALETTE_OFFSET 0x16c00
#define MENU_ICONS_SIZE     0x6400
#define FONT_GLYPH_OFFSET   0x5aca
#define CREDITS_LINES       240
#define MAP_CURSOR_START_X  0xf0

#define RUBIES_TEXT_X       0x90
#define RUBIES_NUM_X        0x9b
#define LOCKED_BOX_X        0x80
#define LOCKED_BOX_X_END    0x100
#define LOCKED_TEXT_X       0x98
#define CLEAN_BOX_X         0x66
#define CLEAN_BOX_X_END     0x102
#define CLEAN_TEXT1_X       0x7c
#define CLEAN_TEXT2_X       0x98

#define DEATH_BOX_X         0x60
#define DEATH_BOX_Y_END     0x94
#define DEATH_LINK_X        0x31
#define DEATH_TEXT_X        0x68
#define DEATH_NUMBER_X      0xfe
#define DEATH_ERASE_X       196
#define DEATH_ERASE_X_END   264

#define ICON_BOX_X          36
#define ICON_BOX_X2         126
#define ICON_BOX_W          35
#define ICON_COUNT_X        0x49
#define SELECTED_ICON_X     0xbd
#define SELECTED_ICON_X_END 0xdb
#define ITEM_NAME_X         0xdb
#define QUEST_ITEM_STEP     30
#define SWORD_SHIELD_X      0x139
#define BORDER_X            31
#define BORDER_X_END        0xa7
#define BOXES_X             0xb7
#define BOXES_X_END         0x16d
#define BOXES_SPLIT         0x6e

#else

#define STARTUP_SECTOR      0xaf
#define ROOM_SECTOR         0xc9
#define EFFECTS_SECTOR      0x3ce4
#define VOICE_SECTOR        0x394d
#define VOICE_SECTOR_2      0x3b3a

#define PLANE_A_EXTRA_SIZE  0x1fc00
#define ROOM_PALETTE_OFFSET 0x16c00
#define MENU_ICONS_SIZE     0x5c00
#define FONT_GLYPH_OFFSET   0x5692
#define CREDITS_LINES       240
#define MAP_CURSOR_START_X  0x100

#define RUBIES_TEXT_X       0x8e
#define RUBIES_NUM_X        0x9e
#define LOCKED_BOX_X        0x80
#define LOCKED_BOX_X_END    0x100
#define LOCKED_TEXT_X       0xa0
#define CLEAN_BOX_X         0x80
#define CLEAN_BOX_X_END     0x112
#define CLEAN_TEXT1_X       0x90
#define CLEAN_TEXT2_X       0x9c

#define DEATH_BOX_X         0x60
#define DEATH_BOX_Y_END     0x94
#define DEATH_LINK_X        0x31
#define DEATH_TEXT_X        0x68
#define DEATH_NUMBER_X      0xfe
#define DEATH_ERASE_X       200
#define DEATH_ERASE_X_END   264

#define ICON_BOX_X          29
#define ICON_BOX_X2         98
#define ICON_BOX_W          28
#define ICON_COUNT_X        0x3b
#define SELECTED_ICON_X     0xb6
#define SELECTED_ICON_X_END 0xcd
#define ITEM_NAME_X         0xd4
#define QUEST_ITEM_STEP     23
#define SWORD_SHIELD_X      0x116
#define BORDER_X            24
#define BORDER_X_END        0x84
#define BOXES_X             0xb0
#define BOXES_X_END         0x13c
#define BOXES_SPLIT         0x52

#endif

#endif
