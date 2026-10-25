#ifndef PORT_H
#define PORT_H

#pragma pack(2)

#define strlen          port_strlen
#define strcmp          port_strcmp
#define strcpy          port_strcpy
#define findstr         port_findstr
#define memset          port_memset
#define memmove         port_memmove
#define free            port_free
#define open            port_open
#define close           port_close
#define create          port_create
#define unlink          port_unlink
#define read            port_read
#define write           port_write
#define lseek           port_lseek
#define exit            port_exit
#define getpid          port_getpid
#define kill            port_kill
#define wait            port_wait
#define sigmask         port_sigmask
#define sysdate         port_sysdate
#define srqcmem         port_srqcmem
#define srtmem          port_srtmem
#define modcload        port_modcload
#define modlink         port_modlink
#define munlink         port_munlink
#define os9exec         port_os9exec
#define os9fork         port_os9fork
#define alm_cycle       port_alm_cycle
#define alm_delete      port_alm_delete
#define csd_devname     port_csd_devname
#define csd_devparam    port_csd_devparam
#define intercept       port_intercept
#define errno           game_errno
#define main            game_main

#define BIG_ENDIAN_DATA __attribute__((scalar_storage_order("big-endian")))
struct port_be_shorts { short v[64]; } BIG_ENDIAN_DATA;

#define port_be16(p) ((unsigned short)(((unsigned char *)(p))[0] << 8 | ((unsigned char *)(p))[1]))
#define port_be32(p) ((unsigned long)port_be16(p) << 16 | port_be16((unsigned char *)(p) + 2))

void port_swap_longs(long addr, long count);
void port_swap_shorts(long addr, long count);
void port_fix_room(void);
void port_fix_macros(long macro_data);
void port_hero_loaded(void);

void port_idle(void);

void port_call_sprite(long code, long x, long y, long base, long *lines);

#endif
