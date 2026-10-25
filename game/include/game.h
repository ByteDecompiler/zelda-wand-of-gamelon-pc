#ifndef GAME_H
#define GAME_H

#include "version.h"

#ifndef BIG_ENDIAN_DATA
#define BIG_ENDIAN_DATA
#endif

struct tool {
    short x;
    short y;
    short screen_x;
    short screen_y;
    short draw_x;
    short draw_y;
    char kind;
    unsigned char state;
    char active;
    char drawn;
    char mode;
    char pad2;
    unsigned short timer;
};

struct projectile {
    unsigned char type;
    char state;
    short map_x;
    short map_y;
    short x;
    short y;
    short start_x;
    short start_y;
    short fx;
    short fy;
    short vx;
    short vy;
    short start_vx;
    short start_vy;
    unsigned char gravity;
    unsigned char frame;
    unsigned char *anim;
    char shown;
    char visible;
    char repeats;
    char interval;
    char wait;
    char lifetime;
    char life;
    unsigned char damage;
};

struct pcl {
    char ctrl;
    char pad0;
    char c2;
    char c3;
    short sig;
    char *nxt;
    long buf;
    long buf_size;
    long err;
    short pad1;
    long cnt;
};

struct menu_icons {
    long pad[14];
    long sword1;
    long sword2;
    long sword3;
    long small_shield;
    long big_shield;
};

extern long video_path;
extern long fct_a;
extern long line_event;
extern long pf_y;
extern long pf_y_px;
extern long pf_x;
extern long pf_x_px;
extern long *lct_b;
extern long cur_label;
extern long plane_b_buffer;
extern long plane_b_buf0;
extern long map_y;
extern long map_x;
extern char mkt_demo;
extern long zelda_x;
extern long zelda_screen_x;
extern long zelda_sprite_x;
extern char zelda_moved;
extern char jump_info_solid;
extern long walk_toggle;
extern char zelda_anim_step;
extern char zelda_thrusting;
extern char thrust_lunge;
extern char continuing;
extern long anim_frame_idx;
extern long disc_errors;
extern char selected_item;
extern short have_water;
extern short water_used;
extern long damage_taken;
extern char bomb_placed;
extern long *data_cil;
extern char area_has_exp;
extern char nvr_name[];
extern unsigned short rubies;
extern char not_enough_rubies;
extern long pointer_x;
extern long pointer_y;
extern long old_pointer_x;
extern long old_pointer_y;
extern unsigned char music_volume;
extern long music_path;
extern long menu_event;
extern char in_combat;
extern short shield_level;
extern struct menu_icons menu_icons;
extern unsigned long random_seed;
extern struct projectile *projectiles;
extern struct tool tools[];
extern long *line_table;
extern long plane_a_mem;
extern long heart_w;
extern long heart_x[];
extern void (*compiled_sprites[])(long x, long w, long y, long *lines);
extern long door_open;
extern long plane_a_buffer;
extern long pointer_buttons;
extern char got_sign_up;
extern char got_sign_wait;
extern char rubies_sign_wait;
extern long got_item;
extern long fct_b;
extern long palette_in[];
extern unsigned char palette_out[];
extern long anim_path;
extern long data_path;
extern long *video_cil;

void restore_label(long label_id);
void move_map(void);
void play_overworld_music(void);
long add_to_playfield_tools(unsigned char kind, char sub, unsigned short x, unsigned short y, char tool_kind, short timer);
void unmark_experience(long exp_no);
void go_find_hero_action(char action);
void abort_rtrs(void);
void flip_world_lcts(long *lct);
long check_box(short *box, long dir, long sprite_idx, long state);
void move_1_block(long sprite_idx, long dx, long stop_at_edge);
void bring_up_locked_sign(long n);

void fade_out_plane_A(void);
void wait_for_line_event(void);
void sattenuation(long n);
void check_stick(long *event);
unsigned char check_for_key(long door, long room);
void fade_up_plane_AB(void);
void reinit_lct_A(void);
void install_palette(long plane, char *pal, char *table);
void wait_for_button_press_music(void);

extern char anim_file_name[];
extern char data_file_name[];
extern char initial_area_tab[];
extern char areas[];
extern long draw_target;
extern unsigned short item_counts[];
extern char *sprite_ptrs[];
extern char sprite_gone[];
extern char sprite_killed[];
extern char hero_killed;
extern unsigned char *block_map;
extern long map_width;
extern long got_sign_x;
extern long got_sign_w;
extern long game_won;
extern char game_started;
extern long menu_icon_tab[];
void init_rtr(void);
void initial_areas(void);
void draw_filled_rect(long x, long y, long x_end, long y_end, long buf_base, unsigned char color);
void draw_outlined_box(long where, long x1, long y1, long x2, long y2, long fill, long border);
void play_background_animation(long n);
void font_draw_glyphs(long x, long y, char *text);
void font_erase_glyphs(long x, long y, char *text);
long get_input(void);
void initial_item_quantity(void);
void set_life_hearts(void);

struct hero_action {
    char id;
    char pad;
    short *frames;
};

extern long font_height;
extern long font_width;
extern long font_bitmap_a;
extern long font_bitmap_b;
extern long font_bitmap_c;
extern unsigned char experiences[];
extern char locked_sign_done;
extern char tool_list[];
extern long tool_count;
extern struct hero_action hero_actions[];
extern unsigned char hero_action;
extern char jump_wait_timer;
extern char jump_rising;
extern char jump_step;
extern char climbing_blk48;
extern char climbing_blk25;
extern long zelda_y;
extern long zelda_screen_y;
extern long zelda_sprite_y;
extern long max_hearts;
extern char cheats;
void color_font_draw_glyphs(long x, long y, char *text, long shadow);
long demo_playfield(long room, long track_no, long load_enemies, long unused, long zelda_start_x, long zelda_start_y, long x, long y, unsigned char resume, unsigned char last_room);
void init_projectile(long type, long x, long y, long vx, long vy, long accel_y, long num_repeats, long start_delay, long fire_interval, long life_frames, long hit_damage);
void kill_sprite(long sprite_no);

struct sprite_palette {
    char pad[0x188];
    long cycle2[32];
};
extern long lct_plane_a1;
extern long lct_plane_a2;
extern long lct_plane_b1;
extern long lct_plane_b2;
extern char menu_scrolling;
extern short sword_level2;
extern short sword_level3;
extern long cycle_colors[];
extern struct sprite_palette *sprite_palette;
void set_icf(long plane, long which, long value);
void scroll_position(long y);
void erase_item_name(char slot, long color);
void erase_selected_icon(long where);
void icon_to_from_save_buffer(char slot, long where, long to);

extern long play_done;
extern char music_paused;
extern long audio_path;
extern long death_y;
extern long saved_zelda_y;
extern long saved_zelda_x;
extern long deaths;
extern long deaths_shown;
long check_experience(long exp_no);
void erase_death_number(long y);
void restart_music(void);

struct imagery {
    long pad[3];
    long off[24];
} BIG_ENDIAN_DATA;
struct alt_action {
    unsigned char id;
    char pad;
    short *frames;
};
struct macro {
    long state;
    unsigned short enemy_id;
    char pad0[9];
    char moved;
    char animate;
    char pad1[5];
    char kind;
    char layer;
    char step_delay;
    char step_timer;
    char alt_active;
    char alt_step_delay;
    short sleep_timer;
    char wake_pending;
    char in_wake_box;
    unsigned char lives;
    char respawned;
    unsigned short lo;
    unsigned short hi;
    short target;
    short spread;
};
struct file_pos {
    long path;
    short color;
    unsigned short sum;
    unsigned short host_off;
    unsigned short info_off;
    unsigned short data_off;
    long pos;
};
extern char file_name[];
extern char host_name[];
extern struct alt_action sprite_actions[][2];
extern char hero_action_of[];
extern struct macro *macros[];
extern long lct_head_a;
extern long anim_flip;
extern long plane_a_buf0;
extern long zelda_dir;
extern char mkt_demo_end;
extern unsigned char *mkt_demo_ptr;
extern unsigned char mkt_demo_byte;

extern short help_save[];
extern char help_on;
extern unsigned char initial_counts[];
extern long item_icons[];
extern char rope_out;
extern long rope_x;
extern long rope_bottom;
extern long rope_top;
extern long lantern_size;
extern long lantern_pos;
extern char locked_text[];
extern long errno;
void draw_selected_icon(char slot, long where);
void draw_highlighted_icon(char slot, long where);
void draw_item_name(char slot, long color);
void font_draw(long x, long y, char *text, long z);

struct pcb {
    short stat;
    short sig;
    long rec;
    long chan;
    short achan;
    char *video;
    char *audio;
    long data;
};
extern struct pcb *pcb;
extern long plane_a_buf1;
extern long voice_done;
extern long *sprite_frames;
extern long cur_sprite;
extern char big_jump_active;
extern long zelda_idle_timer;
extern char attack_state[];
extern long sprite_damage[];
extern char attack_damage[];
extern long knockback[];
extern char freeze_timer;
extern long plane_a_data;
extern long plane_b_buf1;
extern long plane_b_buf2;
extern long glove_flash;
extern long lantern_level;
extern unsigned short music_chan;
extern long play_failed;
void plane_B_over_A(void);
void flip_A(void);
void display_clean_disc_sign(long n);
void fade_up_plane_A(void);
void plane_A_over_B(void);
void attenuation(long n);
void my_sprintf(char *buf, long digits, long value);
void color_font_draw(long x, long y, char *text, long dest_buf, long color);
void init_enemy_projectiles(void);
void start_voice_over(long voice_no);
void set_pcb(short stat, short sig, long rec, long chan, short achan, char *video, char *audio, long data);

long play_sound(long sound_id);
void stopsound(long sound_id);
void init_bomb_hit(long x, long y, long direct_hit);

extern long macro_ptrs[];
extern char *macro_data;
extern long lantern_level2;
extern char rubies_text[];
extern char rubies_num[];
extern long sprite_xs[];
extern long sprite_ys[];

long nvr_free_mem(long ret_size, char *ret_mem);
char *nvr_alloc_mem(long mem_size, long mem_color);
long nvr_close(struct file_pos *nvr_file);
long nvr_check_name(char *name);
long nvr_checksum(struct file_pos *fp);

extern char nvr_magic[];
extern char nvr_key[];
long nvr_unpack_name(char *in, char *out, long n, char *key, long keylen);
long nvr_read_host(struct file_pos *fpos, char *out);
struct file_pos *nvr_open_into(char *name, char *want, long mode, struct file_pos *file_rec);

long nvr_pack_name(char *in, char *out, long n, char *key, long keylen);

extern char clean_text1[];
extern char clean_text2[];
extern long line_step;
extern long line_count;
extern long thrust_lunged;
extern char mem_space[];
void end_thrust(void);

struct block_info {
    long left;
    long right;
    long climb_a;
    long climb_b;
    char step;
    char solid;
    char f12;
    char f13;
    char f14;
    char f15;
    char door;
    char deadly;
    char ground;
    char f19;
    char floor;
    char f1b;
    char f1c;
    char f1d;
    char track;
    char pad4;
};
extern char death_done;
extern char error_bufs[];
extern long sprite_drawn[];
extern long sprite_asleep[];
extern long sprite_draw_xs[];
extern long sprite_draw_ys[];
extern char help_ended;
void get_sprite_block(long x, long y, struct block_info *info);

struct channel {
    long next;
    long f4;
    char pad0[4];
    unsigned char flags;
    char pad1;
    unsigned long len;
    long data;
};
extern char dark_room;
extern unsigned short *alt_macro_lists[];
extern unsigned char alt_frame_base[];
extern char mm_menu_shown;
extern short music_flags;
extern long music_path2;
extern char cursor_data[];
extern char alt_repeat[];
extern unsigned char alt_index[];
extern struct channel channels[];
void reinit_lct_B(void);
void dim_lantern(void);
void get_new_macro_image(long sprite_index, unsigned char flip, unsigned short code);
void reinit_sprite(long sprite_idx);

struct stick {
    long x;
    long y;
    long z;
    long count;
};
struct label {
    unsigned short x;
    unsigned short y;
    unsigned short w;
    unsigned short h;
    unsigned short hot_x;
    unsigned short hot_y;
    unsigned short hot_w;
    unsigned short hot_h;
    short group;
};
extern char mod_dir_a[];
extern char mod_dir_b[];
extern long cmaps[];
extern long zelda_cmap;
extern long enemy_cmaps;
extern long proj_cmap_base;
extern char death_text[];
extern struct label labels[];
extern long label_save_buf;
void fill_tool_list(void);

extern long boss_flag;
extern char jump_falling;
extern struct block_info jump_info;
void decode_mem_space(void);
void check_map_while_jump_descent(long dir);
long nvr_close_free(struct file_pos *file_rec);
struct file_pos *nvr_open(char *name, char *want, long mode);
long nvr_read(struct file_pos *fpos, char *buf, long nbytes);

extern long lct_head_b;
extern long font_glyph_data;
extern char menu_icons_ready;
extern char font_ready;
void add_fake_bombs(void);
void add_fake_ropes(void);
void add_fake_oil(void);
void load_font(long data);

extern long music_track;
extern struct pcb music_pcb;
extern long room_number;
extern unsigned char dark_rooms[];
extern struct channel *cur_channel;
extern long channel_queue;
extern long mm_game_title;
extern long mm_banner;
extern long mm_no_memory_title;
extern char mm_banner_color;
extern char mm_title_color;
extern unsigned char mm_text_color;
extern unsigned char mm_hilite_color;
extern char mm_dim_color;
extern char mm_continue;
extern unsigned char mm_choice;
extern char mm_done;
extern char mm_saved;
extern long mm_idle_frames;
char *init_error_buffer(long buf_index);
long randomize(void);
void debounce_trig(void);
void debounce_stick(void);
void new_mmenu(unsigned char menu_no);
void mmenu_initial_clear(void);
void mmenu_initial_draw(void);
void mmenu_loop(void);
void mmenu_scroll_up(void);

extern char mm_quit_after_save;
extern char mm_quit;
extern char mm_help;
extern char mm_new_game;
extern unsigned char mm_cursor;
extern unsigned char mm_old_cursor;
extern unsigned char mm_row_count;
extern char help_aborted;
extern long old_zelda_dir;
extern unsigned char crouch_wait_timer;
extern char zelda_crouching;
extern long pf_plane_b_buffer;
struct macro_def {
    char type;
    char pad0[3];
    short x;
    short y;
    char pad1[6];
    short start_state;
    short lo;
    short hi;
    short spread;
    char alt_macro;
    char alt_repeat;
    char alt_chance;
    char alt_dir;
    char alt_speed;
} BIG_ENDIAN_DATA;
struct motion {
    long dir;
    long next;
    long prev;
    long fc;
};
struct item_name {
    char *line1;
    char *line2;
};
void mmenu_scroll_up_intermediate(void);
void mmenu_scroll_down_intermediate(void);
void mmenu_scroll_down(void);
void mmenu_clear_area(void);
void mmenu_redraw(void);
void init_1_or_4(long sprite_idx);
void check_for_climb(long n);
void set_link_position(long dx);
void set_link_position_and_dir(long dx);

unsigned char mm_rows[8];

extern long fall_timer;
extern char jump_info_ground;
extern long save_size;
extern char save_host_info[];
extern char save_flags[];
extern long room_gfx;
extern long rtr_index;
extern char data_file_name0[];
extern char data_file_name1[];
extern char data_file_name2[];
void create_file_and_host_names(long slot_number);
long nvr_avail(void);
long nvr_delete(char *name);
struct file_pos *nvr_create(char *name, char *str_a, char *str_b, long extra, long mode);
long nvr_write(struct file_pos *fp, char *buf, long write_len);
void encode_mem_space(void);
void open_up_area(long area, long flag);
void clean_disc(void);
void do_mmenu_code(void);
void quit_game(long n);

extern long *lct_a;
extern long bg_cycle[];
void font_erase(long x, long y, char *text, long dest_buf);
void draw_heart(short kind, short heart_no, long y);

extern char zelda_riding;
extern long music_wait;
extern short music_side;
extern char sig0_flags;
extern char sig1_flags;
extern long sig0_count;
extern long sig1_count;
long ac_process(long sig);
long check_vulnerablility(long sprite_idx, long weapon);
struct enemy_def {
    char pad0[0x2c];
    short sleep_time;
    char pad1[2];
    short sleep_time2;
    char pad2[0x17];
    unsigned char chance;
    unsigned char chance2;
} BIG_ENDIAN_DATA;
void jump_up_dir(long dx);

extern long *sprite_pos[];
extern long sprite_buffer;
extern struct block_info *block_ptrs[];
extern char *mm_start_text;
extern unsigned char mm_text_x;
void place_raft(long x, long y, long state);

extern long frame_flag;
extern long death_x;
long text_width(char *text);
extern long enemy_data;
void start_continue(void);
void update_death_pose(void);
void draw_death_number(long y);
void death_loop_bottom(void);
long old_get_sprite_block(long x, long y);

struct smap {
    char pad0[10];
    long data;
    long size;
    char pad1[8];
    short w;
    short pos;
};
struct sound_pcb {
    short a;
    short b;
};
extern long audio_signal;
extern long audio_task;
extern char music_file_name[];
extern short sound_level;
void load_effects(void);
void add_to_tool_list(char tool);
void check_lamp_highlight(char item);
void check_canteen_highlight(char item);

extern char slot_used[];
extern long sprite_frame_no[];
extern long pf_x_px_max;
extern long pf_y_px_max;
void explode_bomb(unsigned char tool_idx);
void add_bombs_to_buy(void);
void add_ropes_to_buy(void);
void add_oil_to_buy(void);
void delete_tool(unsigned char tool_index);
void clear_area_experiences(void);
void init_playfield_size(long room);
long drop_sprite(long track, long enemies, long zelda_x0, long zelda_y0, unsigned char resume, unsigned char last);
void stop_music(void);

extern unsigned short bit_masks[];
extern unsigned char sound_len[];
extern char *demo_ptr;
extern char demo_buf[];
extern char demo_started;
void draw_border(long where);
void draw_an_item(long where, short icon, long slot, long count);
void draw_sword(long where);
void draw_shield(long where);
long delayed_wait(long frames);
void screen_dimmer(long on, long count, long mode, long sound, long stereo);
void debounce_trig_music(void);

extern char heart_bonus;
extern char anim_restores_life;
extern char anim_flag_239c;
extern char bomb_bounced;
struct enemy {
    char type;
    char pad0a;
    short id;
    char pad0b[5];
    char layer;
    char pad0c;
    char step_delay;
    short present;
    char pad0d[0x1e];
    unsigned short first_sleep;
    char first_wake_mode;
    char lives;
    unsigned short sleep;
    char mode;
    unsigned char kind;
    unsigned char experience;
    char pad1[0x13];
    unsigned char item;
    char pad2[7];
    char state;
    char pad3[0x5f];
} BIG_ENDIAN_DATA;
void leave_item(long kind, long x, long y);
void mark_experience(long exp_id);
void start_anim(long room, long sprite_no, long kind);
long play_animation(long anim_number);
void start_music(long track);
long check_map_row(long start, long y, long count);
void set_new_map(long spot_x, long spot_y, long spot_w, long spot_h);

extern struct pcl *bg_pcl;
void go_find_action(long who, long id);
void draw_initial_death_screen(long y);
long death_loop(void);
void zero_video_cil(void);
void zero_data_cil(void);
void set_pcl(struct pcl *pcl, char ctrl, char c2, char c3, short sig, char *nxt, long buf, long buf_size,
             long err, long cnt);

extern long pf_extra_screens;
extern char pf_scrolls_horiz;
extern long map_height;
extern long map_width_px;
extern long map_height_px;
extern char room_widths[];
extern char room_heights[];
extern long bg_loaded;
extern long frame_count;
extern struct pcl *pcl_b;
extern struct pcl *pcl_c;
void ac_signal_process(long sig);
void init_type_2(long sprite_no, struct macro_def *def);
void init_type_3(long sprite_idx, struct macro_def *def);
void init_type_7(long sprite_index, struct macro_def *def);
void init_type_8(long sprite_no, struct macro_def *def);
void init_type_5(long sprite_idx, struct macro_def *def);
void move_type_5(long sprite_no);
struct macro_def6 {
    char pad0[0xe];
    unsigned char state;
    unsigned char kind;
    char pad1[0x20];
    short pause_time;
} BIG_ENDIAN_DATA;
struct macro_def5 {
    char pad0[0xe];
    short start_state;
    short lo;
    short hi;
    short spread;
    unsigned char kind;
    unsigned char alt_kind;
    unsigned char speed;
    char alt_loops;
    unsigned char alt_chance;
    char alt_delay;
    unsigned char stop_kind;
    unsigned char speed2;
    char stop_loops;
};
struct macro5 {
    char pad0[0x22];
    short target_x;
    char spread;
    unsigned char frame_offset;
    unsigned char kind;
    char alt_timer;
    char loops;
};
void init_5_or_6(long sprite_no, long macro_index, long dx, long dy, long size);

extern long map_cursor_x;
extern long map_cursor_y;
extern long saved_map_cursor_x;
extern long saved_map_cursor_y;
extern long saved_map_x;
extern long saved_map_y;
void main_init(long sprite_no, struct macro_def *def);

extern char *palette_file;
void init_miss(long x, long y);
void convert_palette(void);
long play_error(void);

extern long is_pal;
void retrieve_help_state(long *stick_event, long *button_state);
void retrieve_mkt_demo(long *x, long *y);

extern long plane_a_extra;
extern long plane_b_data;
extern long tool_imagery_buf;
extern long sfx_data;
extern long ctl_tables;
extern long menu_icons_buf;
extern long proj_gfx_buf;
extern long proj_tables;
extern long zelda_imagery;
extern long sfx4_data;
extern long env_proj_buf;
extern long enemy_list;
extern long path_blocks;
extern long path_blocks_r;
extern long frame_color_hi;
extern long frame_color_lo;
void init_cmap_addresses(void);
void init_table_widths(void);

struct tool_spot {
    unsigned char kind;
    unsigned char room;
    short x;
    short y;
};
long check_if_have_tool(long tool_id);

extern long zelda_target_y;
extern long zelda_target_x;
void inc_pf_y(void);
void dec_pf_y(void);
void inc_pf_x(void);
void dec_pf_x(void);
void write_sprite_blocks(long x, long y, long nblocks, char *save, long value);
void restore_sprite_blocks(long x, long y, long count, char *src);
long get_image_number(long sprite_idx);

struct proj_def {
    long anim_left;
    long anim_right;
    char owner;
    char stops_at_walls;
    char bounces;
    char offscreen_ok;
};
extern struct proj_def *proj_defs;
struct macro_b {
    char pad0[0x22];
    char macro_no;
    char loops;
    char loops_left;
    char dir_mode;
    char frame_offset;
    unsigned char alt2_macro;
    char alt2_repeat;
    char alt2_chance;
    char b2a;
    char b2b;
    char alt3_chance;
    char alt3_dir;
    char alt3_speed;
};
long check_for_alternate_macro(long sprite_idx, long state);

extern char bumper_dir[];
extern char bumper_module[];
extern char bumper_dir2[];
extern long bumper_proc;
extern long bumper_env;
extern char *bumper_name;

extern long *env_projectiles;
extern long proj_min_x;
extern long proj_max_x;
extern long proj_min_y;
extern long proj_max_y;
extern long last_side;
extern short chan_pos;
void copy_sound(long dest, long src);
extern long sound_mem[100000];
extern long sound_ids[100000];
extern struct sound_pcb sound_pcbs[100000];

extern void (*proj_draw[])();

extern long idle_frames;

extern long button_pressed;
extern long *cil_b;
extern struct pcl *pcl_d;
extern struct pcl *pcl_e;
extern struct pcl *pcl_f;
void sprite_hits_tool(unsigned char tool_idx, char skip);

extern unsigned char *voice_ptr;
extern char voice_ready;
extern unsigned long repeat_count;
long get_first_item(long refill);
void highlight_item(char item, long where);
void unhighlight_item(char item, long where);
extern long sprite_macros[];

struct macro_r {
    char pad0[0x22];
    short lo;
    short hi;
    short target;
    short spread;
};

extern char help_repeat;
extern unsigned char help_cmd;
extern char voice_wait;
extern char help_on;
void restore_vert_blocks(long x, long y, long block_count, char *src);
void set_help_off(void);

extern long knocked;
extern short zelda_flag_1df2;
extern short zelda_frames_1[];
extern short zelda_frames_2[];
extern short zelda_frames_6[];
extern short zelda_frames_7[];
extern short zelda_frames_8[];
extern short zelda_frames_9[];
extern short zelda_frames_10[];
extern short zelda_frames_11[];
extern short zelda_frames_12[];
extern short zelda_frames_13[];
extern short zelda_frames_14[];
extern short zelda_frames_15[];
extern short zelda_frames_16[];

extern char mkt_demo_on;
extern unsigned char mkt_demo_buf[];
void mmenu_choice(long selection);
void wait_for_menu_option(void);

struct sig_block {
    char flags;
    char pad0;
    char a;
    char b;
    short sig;
    char *next;
    long mem;
    long one;
    char *err;
    char pad1[2];
    long count;
};
extern struct sig_block sig_blocks[2];
void write_vert_blocks(long x, long y, long count, char *save, long value);

extern long sprite_gfx[];
extern char help_demo[];
extern unsigned char voice_buf[];
void check_collisions(long sprite_idx, long cmap_idx, long proj_idx, long proj_frame);
void check_map(long proj_idx, long proj_frame);
void roll_credits(void);
void set_mkt_demo_off(void);
void set_ov_help_off(void);
void mmenu_init(void);

extern long clut_table[];
char pick_up_perm_tool(unsigned char tool);
void more_items(long item, long add, long max);

extern struct tool_spot tool_spots[];

struct macro_alt {
    char pad0[0x27];
    char alt2_macro;
    char alt2_repeat;
    char alt2_chance;
    char b2a;
    char b2b;
    char alt3_chance;
    char alt3_dir;
    char alt3_speed;
};
long set_alternate_macro(long sprite_idx, long which, long repeat, long state, long speed);
void do_alternate_macro(long sprite_no);

struct enemy_st {
    char state;
    char pad0[0x1b];
    short count;
} BIG_ENDIAN_DATA;

extern short rope_count;
extern long rope_anim_toggle;
long check_for_rope_path(long x, long y);

extern unsigned char ov_help_cmd;
extern long ov_help_repeat;
extern long map_scroll_dx;
extern long map_scroll_dy;

void slide(long dx, long wall_flag);

struct macro_def2 {
    char pad0[0xe];
    char macro_no;
    char loops;
    char dir_mode;
    char alt_macro;
    char alt_speed;
    char alt_repeat;
    char alt_chance;
    char alt_dir;
} BIG_ENDIAN_DATA;

extern char str_625[];
extern char str_tv[];
extern char cd_dev_name[];
extern long plane_b_mem;
extern long lct_pal_a;
extern long lct_pal_b;
extern long dcp_regs_a[];
extern long dcp_regs_b[];

extern char *mm_titles[7];

extern long proj_cmaps[];
void init_bomb(long x, long y);

void finish_move(long state, long dx, long action, long step, long sprite_no);
void get_random_bound(long sprite_no, long state);

struct enemy_type {
    long id;
    struct enemy_gfx *gfx;
};
struct enemy_gfx {
    long f0;
    long f4;
    long f8;
    long frames[24];
    short colors[24];
} BIG_ENDIAN_DATA;
extern struct enemy_type enemy_types[];
extern long sprite_drawn_frame[];

extern long in_area;

extern long clut_table_lo[];
extern long clut_table_hi[];

extern short bg_cycle_start[];
extern short bg_cycle_len[];
extern short bg_cycle_delay[];
void reset_pcl(struct pcl *pcl, char ctrl, char *buf, long cnt);

struct climb_dir {
    long dir;
    long next;
    long prev;
};
extern long climb_toggle;
void jump_off(void);

extern unsigned short low_bits_mask[];
extern unsigned short high_bits_mask[];
void init_hit(long x, long y);
void hero_hit(long damage);
void reflect_projectile(long proj_no);

extern short world_cycle_start[];
extern short world_cycle_len[];
extern short world_cycle_delay[];
extern short bg_cycle_timer[];
extern char bg_cycles_on;
void read_bg_cycle_colors(void);
void check_world_hotspots(void);

extern long anim_delay;
void kill_sprite_noflash(long sprite_idx);

void get_collision_result(long high_bit, long low_bit, unsigned short *mask, long unused, long down, long flip, long *hit, long *pos);

void check_which_downward_motion(struct motion *motion_ptr);
void check_which_upward_motion(struct motion *move_info);
void fix_jump_stick(struct stick *stick_ev);
void jump_up(struct motion *move_info);
void handle_knockback(void);
void using_rope(void);
void big_jump_up(void);
void big_jump_down(void);
void sword_thrust(void);
void climb(unsigned char up_action, unsigned char climb_action, long ladder_flag, struct climb_dir *dir_state);
void check_map_while_walking(long dir);

void load_backgrounds(long n);
void flip_backgrounds(long pos);
extern long bg_frames_b[];
extern long palette_in_lo[];
extern long palette_in_hi[];

long check_buttons(void);

extern char proj_frames_0[];
extern char proj_frames_1[];
extern char proj_frames_6[];
extern char proj_frames_2[];
extern char proj_frames_3[];
extern char proj_frames_5[];
extern char proj_frames_7[];
extern char proj_frames_8[];
extern char proj_frames_9[];
extern char proj_frames_10[];
extern char proj_frames_11[];
extern char proj_frames_12[];
extern char proj_frames_13[];
extern char proj_frames_14[];
extern char proj_frames_15[];
extern char proj_frames_16[];
extern char proj_frames_17[];
extern char proj_frames_18[];
extern char proj_frames_19[];
extern char proj_frames_20[];
extern char proj_frames_21[];
extern char proj_frames_22[];
extern char proj_frames_23[];
extern char enemy1_proj_right[];
extern char enemy1_proj_left[];
extern char enemy2_proj_right[];
extern char enemy2_proj_left[];
extern char enemy3_proj_right[];
extern char enemy3_proj_left[];

extern long area_deaths;

extern short death_frames[];
extern char sprite_cycles_on;
extern long frame_alarm;
extern long frame_count_b;
extern long button_held;
extern long combat_frames;
extern long combat_frame;
extern long old_pf_x_px;
extern long old_pf_y_px;

long get_abs_x(long which, long x, long y, long dir);
void set_walker(long sprite_idx, long x);
void end_alternate_macro(long sprite_no);
void check_for_restart(long sprite_no);

long strlen(char *s);
long strcmp(char *a, char *b);
void strcpy(long *dest, char *src);
long findstr(long pos, char *string, char *pattern);
void memset(long buf, long value, long n);
void memmove(long dest, long src, long n);
void free(char *p);

long open(char *name, long mode);
long close(long path);
long create(char *name, long mode, long attr, long size);
long unlink(char *name);
long read(long path, char *buf, long n);
long write(long path, char *buf, long n);
long lseek(long path, long pos, long mode);
void exit(long code);
long getpid(void);
void kill(long pid, long sig);
void wait(long *status);
void sigmask(long level);
void sysdate(long format, long *time, long *date, short *day, long *tick);
char *srqcmem(long size, long color);
long srtmem(long size, char *p);
char *modcload(char *name, long mode, long color);
char *modlink(char *name, long mode);
long munlink(char *module);
long os9exec(char *start, long name, long *argv, long envp, long a, long b, long paths);
long os9fork(void);
long alm_cycle(long sig, long time);
void alm_delete(long id);
char *csd_devname(long type, long n);
char *csd_devparam(char *name);

void ss_play(long path, struct pcb *p);
void ss_abort(long path);
void cd_setstat_39(long path);

long sm_creat(long path, long type, long n, long *mem);
long sm_out(long path, long id, struct sound_pcb *p);
void sm_off(long path);
struct smap *sm_info(long path, long id);
void sc_atn(long path, long value);

long dc_crfct(long path, long plane, long length, long attr);
long dc_crlct(long path, long plane, long lines, long attr);
void dc_wrfct(long path, long fct, long start, long n, long *table);
void dc_wrfi(long path, long fct, long index, long instr);
void dc_rdlct(long path, long lct, long a, long b, long c, long d, long *out);
void dc_wrlct(long path, long lct, long a, long b, long n, long c, long *table);
void dc_wrli(long path, long lct, long line, long column, long instr);
void dc_flnk(long path, long fct, long lct, long line);
void dc_llnk(long path, long lct, long line, long lct2, long line2);
void dc_exec(long path, long fct_a, long fct_b);
void dc_intl(long path, long on);
void dc_ssig(long path, long sig, long line);
void dc_set_display(long path, long mode);

void pt_coord(long path, long *buttons, long *x, long *y);
void pt_pos(long path, long x, long y);
void pt_org(long path, long x, long y);
void gc_pos(long path, long x, long y);
void gc_show(long path);
void gc_hide(long path);
void gc_def(long path, long a, long b, long w, long h, long c, char *data);
void gc_col(long path, long color);
void gc_blnk(long path, long n);
void gc_org(long path, long x, long y);

void queue_insert_channel(long *queue, struct channel *c);
struct channel *queue_pop_channel(long *queue);
void clear_384x240(long buf);
void clear_416x240(long buf);
void draw_label_save_under(long dest, long src, long buf, long w, long h);
void restore_under_label(long dst, long buf, long w, long h);
void draw_rle_sprite(long y, long a, long b);
void draw_rle_sprite_flipped(long y, long a, long b);
void draw_rle_sprite_flash(long y, long a, long b);
void draw_rle_sprite_flash_flipped(long y, long a, long b);
void erase_32x32(long x, long y, long where);
void draw_rle_icon(long where, long a, long b, long image);
void erase_sprite_rows(long x, long y, long where, long a, long b);
void erase_16x16(long x, long w, long y);
void erase_16x4(long x, long h, long y);
void draw_line(long x1, long y1, long x2, long y2, long color);
long getstat_free_space(long path);
void rotate_bg_cycle(void);
void rotate_sprite_cycle(void);

extern char bread_timer;
extern short bread_x;
extern short bread_y;
extern char anim_sequence[];
void init_bread(long x, long y);
void blow_out_lantern(void);
void play_animations(void);
void play_all_animations(void);
void restore_arkidai_path(long sprite_no);
void fade_out_plane_B(void);

#endif
