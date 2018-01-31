# if !defined (__NES_DEVICE_CONTEXT_H_)
#  define __NES_DEVICE_CONTEXT_H_ 

# include "stdint.h"

# if defined (__cplusplus)
  extern "C" {
# endif  /* __cplusplus */

# if !defined (_WIN32) && !defined (_WIN64)
#   error unsupported platform
# endif 

# if !defined (__cplusplus)
#   ifndef null_DEFINED
#     define null ((void *)0)
#   endif
# else 
#   ifndef null_DEFINED
#     define null 0
#   endif
# endif

# if defined (_MSC_VER) || defined (__ICC) || defined (__INTEL_COMPILER) /* MSVC/ICC starting... */
#   define finline static  __forceinline
#   define dinline         __declspec(noinline)
#   define callstd         __stdcall
#   define callc           __cdecl
#   define cvalign(x)      __declspec(align(x))
#   define cvimpl          __declspec(dllexport) 
# elif defined (__GNUC__) || defined (__GNUG__) /* MSVC/ICC end... GNUC starting */
#   define finline static  __attribute__((always_inline))
#   define dinline         __attribute__((noinline))
#   define callstd         __attribute__((stdcall))
#   define callc           __attribute__((cdecl))
#   define cvalign(x)      __attribute__((aligned(x)))
#   define cvimpl          __attribute__((dllexport))
# else /* unsupported */
#   error unsupported compiler! 
# endif 

/* joypad mask*/
# define JOYPAD_MASK_PRESS 0x01
# define JOYPAD_MASK_CONTINUOUSLY 0x80
# define JOYPAD_MASK_PRESS_FIRST 0x01

struct nes_joypad {
  unsigned char up; /* KEYBOARD-W*/
  unsigned char down;/* KEYBOARD-S*/
  unsigned char left;/* KEYBOARD-A*/
  unsigned char right;/* KEYBOARD-D*/
  unsigned char start;/* KEYBOARD-ENTER*/
  unsigned char select;/* KEYBOARD-SPACE*/
  unsigned char b;/* KEYBOARD-J*/
  unsigned char a;/* KEYBOARD-K*/
  unsigned char x;/* KEYBOARD-U*/
  unsigned char y;/* KEYBOARD-I*/
};

struct ns_texture;

int ns_texture_load (char *filename, struct ns_texture **ntex);
void ns_texture_unload (struct ns_texture **ntex);

struct sys_media;

int sys_media_load (char *filename, struct sys_media **snd);
void sys_media_unload (struct sys_media **snd);
int sys_media_setpos (struct sys_media *snd, double pos);
int sys_media_startplay (struct sys_media *snd);
int sys_media_stopplay (struct sys_media *snd);
int sys_media_curpos	(struct sys_media *snd, double *pos);
int sys_media_getposmax (struct sys_media *snd, double *length);
int sys_media_curistail (struct sys_media *snd, int *ats);  // 0:tail  

struct ns_sound;

struct ns_sound_post {
  int start_ticks;
  int burning_ticks;
  int loop_start_ticks;
  int loop_burning_ticks;
  int loop_count;
};

int ns_sound_open (char *filename, struct ns_sound **nsound, int *maxticks);
int ns_sound_post2 (struct ns_sound *nsound, struct ns_sound_post *sm_post);
int ns_sound_post (struct ns_sound *nsound, int start_tikcs, int burning_ticks, int loop_count);
int ns_sound_stop (struct ns_sound *nsound);
int ns_sound_start (struct ns_sound *nsound);
int ns_sound_flushqueue (struct ns_sound *nsound);
void ns_sound_close (struct ns_sound **nsound);

struct tile {
  int8_t maptable_sel[64];  /*    pixels select table index, trans ? -1 : 0~type_counts */
  int8_t dattable_count[64];/*    pixels select table count */
  int8_t dattable_nat[64];/*      0?trans:notrans */
  int8_t datcount_type;/*         In addition to remaining transparent color outside the color type count */
  int8_t datcount_numb;/*In addition to remaining transparent color outside the color numb count */
  int32_t dattable_sel[64]; /*  pixels select table */
  int32_t dattable_org[64];  /*    same as name  */
};
      
/*
 * MAPPER:palette mode
 * ALPHA:bld
 * HFLIP:x-flip
 * VFLIP:y-filp
 */       
# define TEB_MAPPER 0x00000001
# define TEB_ALPHA 0x00000002
# define TEB_HFLIP 0x00000004
# define TEB_VFLIP 0x00000008

/*
 * MAPPER:palette mode
 * CLOSE:no draw
 * ALPHA:bld with cur bg pixel
 * HFLIP:x-flip
 * VFLIP:y-filp
 * BACK:back sprite
 */    
# define TES_MAPPER 0x00000001
# define TES_CLOSE 0x00000002
# define TES_ALPHA 0x00000004
# define TES_HFLIP 0x00000008
# define TES_VFLIP 0x00000010
# define TES_BACK  0x00000020

/*
 * BG_VISABLE:do bg
 * SP_VISABLE:do sp
 * BG_ALPHA:do alpha
 * UNION_ALPHA:do alpha
 */   
# define PPU_BG_VISABLE 0x00000001
# define PPU_SP_VISABLE 0x00000002
# define PPU_BG_ALPHA 0x00000004
# define PPU_UNION_ALPHA 0x00000008
                                                             
struct bg_chunk {
  int32_t mask; /* ctrl flags mask */
  uint8_t page; /* mesh interval */
  uint8_t land; /* mesh point of fall */
  uint8_t ld_land;/* alpha-value index in bg-alpha-table 0-255 <-> trans-100%pixel */
  uint8_t ld_percent; /* alpha-percent, 256 step. */
};                                                        
struct sp_chunk_item {
  struct tile cache;
  int32_t mask; /* sprite control flags */
  uint8_t ld_percent; /* alpha-percent, 256 step. */
  uint8_t   x_coord;    /*    the X coordinates of the upper left corner of the sprite     */ 
  uint8_t      y_coord;  /*   the Y coordinates of the upper left corner of the sprite */
};

int ns_entry (   void * (* cbinit) (void), 
                   void (* cbloop) (struct nes_joypad *joypad, uint64_t ticks, void *params),
                   void (* cbdtor) (void *params) );
void ns_setscrx (uint8_t x);
void ns_setscrx2 (uint8_t x_round, uint8_t x_mini);
void ns_setscry (uint8_t y);
void ns_setscry2 (uint8_t y_round, uint8_t y_mini);
void ns_setscry (uint8_t y);
void ns_setscrpage (int page);
void ns_setctl_mask (int ctlmask);
void ns_setbg_ldpercent (uint8_t percent);
void ns_setun_ldpercent (uint8_t percent);
void ns_setbg_ld (uint8_t idx);
void ns_setun_ld (uint8_t idx);
void ns_setld_lut (uint8_t idx, uint32_t col);
void ns_writesprite (int idx, int channels, struct sp_chunk_item *sci);
void ns_readsprite (int idx, int channels, struct sp_chunk_item *sci);
void ns_setsprite_channels (int idx, int channels);
void ns_copy_sprite_cache (struct sp_chunk_item *sci, struct tile *te);

void ns_load_page (struct ns_texture *ntex,
           uint8_t page, 
           int xstart_nsr, 
           int ystart_nsr, 
           int xstart_ntex, 
           int ystart_ntex, 
           int xnums, int ynums,
           int32_t page_shield);
           
void ns_writebg ( int page, 
          int x_idx, 
          int y_idx, 
          int write_page, 
          int write_land, 
          int mask, uint8_t ldland, uint8_t ldpercent);
          
void ns_getmap ( int page,  int x_idx,  int y_idx, struct bg_chunk *bgc);
void ns_setmap ( int page,  int x_idx,  int y_idx, struct bg_chunk *bgc);
struct tile *ns_gettile (uint8_t page, uint8_t land);
int ns_adjusttile_col (uint8_t page, uint8_t land, int32_t catch_col, int32_t adjust_col);
int32_t *ns_adjusttile_colptr (uint8_t page, uint8_t land, int32_t catch_col);
int32_t *ns_adjustsprite_colptr (int idx, int channels, int32_t catch_col);
void ns_setsprite_mask (int idx, int channels, int32_t mask);
void ns_orsprite_mask (int idx, int channels, int32_t mask);
void ns_setsprite_ld_percent (int idx, int channels, int32_t ld_percent);
void ns_setsprite_x_coord (int idx, int channels, int32_t x);
void ns_setsprite_y_coord (int idx, int channels, int32_t y);
int32_t ns_getsprite_mask (int idx, int channels);

# ifdef __cplusplus
}
# endif  /* __cplusplus */
# endif