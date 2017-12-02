/*
 * Copyright (C) 2017 moecmks, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You should have received a copy of the GNU General Public
 * License along with the tile-mr10; if not, see:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef avxc_icxz4_moecmks_inclued_2017_520_cc22557744
#define avxc_icxz4_moecmks_inclued_2017_520_cc22557744 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * !!! Temporarily only support Windows.
 */
#if !defined (_WIN32) && !defined (_WIN64)
#  error unsupported platform
#endif 

#if defined (__ICC) || defined (__INTEL_COMPILER)
#  define __INTEL_ICL_
#elif defined (_MSC_VER)
#  define __MSVC_CL_
#elif defined (__GNUC__) || defined (__GNUG__)
#  define __GNU_C_
#else 
#  error unsupported compiler
#endif 

#if !defined (__cplusplus)
#  ifndef null_DEFINED
#    define null ((void *)0)
#  endif
#else 
#  ifndef null_DEFINED
#    define null 0
#  endif
#endif

#if defined (__MSVC_CL_) || defined (__INTEL_ICL_) /* MSVC/ICC starting... */
#  define finline static  __forceinline
#  define dinline         __declspec(noinline)
#  define callstd         __stdcall
#  define callc           __cdecl
#  define cvalign(x)      __declspec(align(x))
#  define cvimpl          __declspec(dllimport) 
#  define cvimpl2         __declspec(dllexport) 
#elif defined (__GNU_C_) /* MSVC/ICC end... GNUC starting */
#  define finline static            __attribute__((always_inline))
#  define dinline                   __attribute__((noinline))
#  define callstd                   __attribute__((stdcall))
#  define callc                     __attribute__((cdecl))
#  define cvalign(x)                __attribute__((aligned(x)))
#  define cvimpl                    __attribute__((dllimport))
#  define cvimpl2                   __attribute__((dllexport))
#else /* unsupported */
#  error unsupported compiler! 
#endif 

/*
 ========================================================================
 SND. 
 ========================================================================
 */
typedef void *soundtrack_t;

struct pcm_header {
  uint16_t format;     
  uint16_t chan; 
  uint32_t sam_rate;
  uint32_t avg_rate;
  uint16_t align;
  uint16_t sam_bits; 
  uint16_t ext_size; 
};
struct pcm_wave {
  
  union {
    void *sbuf;
    uint8_t *sbuf8;
    uint16_t *sbuf16;
    uint32_t *sbuf32;   
    uint64_t *sbuf64; 
    float *sbuff;   
    double *sbufdf;  
  };
  uint32_t quad_len;
  uint32_t small_len;
  uint32_t pcm_head_size;
  uint32_t wave_chunk_size;
  float fquad_;
  float fsmall_;
  uint32_t small_;
  uint32_t quad_;
  struct pcm_header *infos;
};
struct audio_cushion {   
  uint32_t quad_start; /* data begin */
  uint32_t quad_tick;   /* data end, if equal = -1, all quad_ticks and small_tick */
  uint32_t small_start;  /* mirco data begin */
  uint32_t small_tick;    /* mirco data end */
  uint32_t loop_count;   /* loops count, if equal = -1, forever */
  struct pcm_wave *pcm; /* pcm data */
};

/* TODO:snd callback **/
cvimpl int callc soundtrack_init (struct pcm_wave *pcm_wave0, soundtrack_t *soundtrack);
cvimpl void callc soundtrack_uninit (soundtrack_t soundtrack);
cvimpl int callc soundtrack_post (soundtrack_t soundtrack, struct audio_cushion *cushion);
cvimpl int callc soundtrack_stop (soundtrack_t soundtrack); 
cvimpl int callc soundtrack_start (soundtrack_t soundtrack);
cvimpl int callc soundtrack_flush (soundtrack_t soundtrack);   
cvimpl int callc soundtrack_openwave (const char *name, struct pcm_wave **wave);  
cvimpl void callc soundtrack_closewave (struct pcm_wave *wave); 

/*
 ========================================================================
 PPU. 
 ========================================================================
 */
/*
 * tile - base tile unit.
 *
 * Describe the basic information for 8*8 pixel.
 * used as the basic object of rendering.
 */ 
struct tile {
  int8_t maptable_sel[64];  /*    pixels select table index, trans ? -1 : 0~type_counts */
  int8_t dattable_count[64];/*    pixels select table count */
  int8_t dattable_nat[64];/*      0?trans:notrans */
  int8_t datcount_type;/*         In addition to remaining transparent color outside the color type count */
  int8_t datcount_numb;/*In addition to remaining transparent color outside the color numb count */
  int32_t dattable_sel[64]; /*  pixels select table 0..63<-> max->min */
  int32_t dattable_org[64];  /*    same as name  */
};
      
/*
 * BG mask 
 *
 * MAPPER:palette mode
 * CACHE:use R/W mesh pool
 * ALPHA:blend
 * HFLIP:x-flip
 * VFLIP:y-filp
 */       
#define TEB_MAPPER 0x00000002
#define TEB_CACHE  0x00000004
#define TEB_ALPHA 0x00000008
#define TEB_HFLIP 0x00000010
#define TEB_VFLIP 0x00000020

/*
 * SP mask
 *
 * SEAT_GLOBAL:no use local seat
 * MAPPER:palette mode
 * CLOSE:no draw
 * ALPHA:blend with cur bg pixel
 * HFLIP:x-flip
 * VFLIP:y-filp
 * BACK:back sprite
 * FLAGS_GLOBAL:no use local flags 
 * ALPHA_GLOBAL:no use local alpha 
 * AMC_APPEND:add pixel crash test in amc_list <very slow>
 */   
#define TES_SEAT_GLOBAL 0x00000001 
#define TES_MAPPER 0x00000002
#define TES_CLOSE 0x00000004
#define TES_ALPHA 0x00000008
#define TES_HFLIP 0x00000010
#define TES_VFLIP 0x00000020
#define TES_BACK  0x00000040
#define TES_FLAGS_GLOBAL 0x00020000
#define TES_ALPHAL_GLOBAL 0x80000000
#define TES_ALPHAP_GLOBAL 0x40000000
#define TES_AMC_APPEND 0x00004000

/*
 * PPU status mask
 *
 * BG_VISABLE:do bg
 * SP_VISABLE:do sp
 * BG_ALPHA:do alpha
 * SP_ALPHA:do alpha
 * UN_ALPHA:do alpha
 */   
#define PPU_BG_VISABLE 0x00000001
#define PPU_SP_VISABLE 0x00000002
#define PPU_BG_ALPHA 0x00000004
#define PPU_UN_ALPHA 0x00000008

/*
 * bg_chunk - base background unit     
 * 
 * Describe the basic information for background vram.
 * total 4096 bg_chunk object <32(width) * 32(height+2) * 4(page nums)>
 */                                                             
struct bg_chunk {
  int32_t mask; /* ctrl flags mask **/
  uint8_t page; /* mesh interval **/
  uint8_t land; /* mesh point of fall **/
  uint8_t lend_land;/* alpha-value index in bg-alpha-table 0-255 <-> trans-100%pixel **/
  uint8_t lend_percent; /* alpha-percent, 256 step. **/
};
      
/*
 * sp_chunk - base sprite unit     
 * 
 * Describe the basic information for sprite, total 256 sp_chunk.
 * the last one is the first painting, the first to be the last painting, and so on
 * each object has 8 cache tile that can be used.
 *
 * need to pay attention to three points:
 *
 * ---:mixed use of the reference pixel is not the ppu of the transparent color 
 *     but the current coordinates of the bg pixels
 *
 * ---:if you use a background sprite then 
 *     the background pixels in the sprite coordinates are not transparent and can be drawn.
 *
 * ---:write operations to use the relevant function as far as possible
 */                                                             
struct sp_chunk {
  struct tile cache[8]; /* sprite's tile-cache */
  uint8_t chan; /* tile's index*/
  int32_t   mask_common; /* global sprite control flags     */ 
  uint8_t x_coord_common; /* global horizontal coordinates*/  
  uint8_t y_coord_common; /* global vertical coordinates  */
  uint8_t  l_blend_percent_common; /* global blend count    */
  uint8_t  l_blend_common; /* global blend count    */
  int32_t mask[8]; /* sprite control flags */
  uint8_t lend_percent[8]; /* alpha-percent, 256 step. **/
  uint8_t   x_coord[8];    /*    the X coordinates of the upper left corner of the sprite     */ 
  uint8_t      y_coord[8];  /*   the Y coordinates of the upper left corner of the sprite */
  uint8_t    lend[8];/* alpha-value index in bg-alpha-table 0-255 <-> trans-100%pixel **/
  uint8_t        page[8];/*  grounp- mesh interval, Note that the information is not real-time*/
  uint8_t         land[8];/* grounp - mesh point of fall, Note that the information is not real-time*/
};
      
/*
 * sp_amc - base sprite crash unit list   
 * 
 * add a piece of data to the AMC chain
 * used to do some accurate pixel testing, but this is very slow. 
 * internal frequent use of memio<malloc and free>. [now, static memory is now being used by restriction]
 * under normal circumstances do not easily try 
 */ 
struct sp_amc {
  struct sp_amc   *level; /* next list    */
  uint8_t    chan; /* tile's index */
  uint8_t        x_coord_std; /* global horizontal coordinates  */
  uint8_t        y_coord_std; /* global vertical coordinates      */
  uint8_t    mask; /* control flags */
  uint8_t        x_coord;   /*  the X coordinates of the upper left corner of the sprite     */     
  uint8_t        y_coord; /*the Y coordinates of the upper left corner of the sprite  */
  uint8_t    land;/*mesh interval */
  uint8_t        x_coord_imp; /*mesh horizontal coordinates   */
  uint8_t        y_coord_imp; /*mesh vertical coordinates  */
  uint32_t       data; /* color */
  uint8_t    lend_percent;
  uint8_t    lend, page, slot;   /* <1>:trans<2>:mesh point of fall<3>:sprite slot in PPU's sp-chunk */
  uint32_t       alloc_idx; /* alloc iindex  */
};

/* inline struct for ex2a03_ppu */
struct __attv_v {
  uint32_t *buf;
  uint8_t *atr;
};
struct __attv_u {
  uint32_t buf[272*256];
  uint8_t atr[272*256];
};
struct __dir {
  uint8_t x;
  uint8_t y;
};

/*                                       
 * graph ppu render uint 
 * the reference : NES PPU rendering model 
 */                                                              
struct emu8bit_ppu {
  struct __attv_u __attv_output;
  struct bg_chunk     sn_map[32*32*4]; /* NES-Tile *.*/  
  struct sp_chunk       oam[256]; /* Sprite-object *.*/
  struct tile     __rd_mesh[256*256], __rw_mesh[256*256]; /* Mesh cache */
  struct sp_amc       amc_cache[64*256]; /* Crash Tst */
  struct sp_amc      *amc_chain[272*256]; /* Crash Tst */ 
  int8_t       amc_chain_hit[272*256]; /* Crash Tst */
  int8_t       amc_cache_enable[64*256]; /* Crash Tst */
  uint32_t              amc_lim; /* ref count */
  struct __dir               amc_cslot[272*256]; /* vailed coord slot */   
  uint32_t lend_table[256]; /* alpha-mapper table */
  uint8_t lland_bg, lland_sp, lland_un;
  uint8_t llendp_bg, llendp_sp, llendp_un;
  uint8_t sn_page;
  uint8_t ss_x;
  uint8_t ss_y; 
  int32_t       ctl_mask;
  void *memblock; /* for align_malloc's free **/
};   
 
cvimpl void callc load_page (struct emu8bit_ppu *ppu, uint32_t *texHeadAPartZeroSqu128,
           uint32_t pitchMapperByte,
           int cache /* 0 ? rd_mesh : rw_mesh */, 
           uint8_t page, int32_t page_shield);
cvimpl void callc sp_write (struct emu8bit_ppu *ppu, uint8_t slot, 
           uint8_t chan, int cache, uint8_t page, uint8_t land);
cvimpl void callc sp_write2 (struct emu8bit_ppu *ppu, struct sp_chunk *spc, 
           uint8_t chan, int cache, uint8_t page, uint8_t land);
cvimpl void callc tilecpy (struct emu8bit_ppu *ppu, 
             uint8_t des_page, uint8_t des_land, uint8_t src_page, uint8_t src_land);
cvimpl void callc tilecpy2 (struct emu8bit_ppu *ppu, 
             uint8_t des_page, uint8_t des_land, uint8_t src_page, uint8_t src_land);
cvimpl void *callc load_imageSqu128 (const char *fname);
cvimpl void callc free_imageSqu128 (void *tex);
cvimpl void spticks (struct emu8bit_ppu *ppu);
cvimpl void bgticks (struct emu8bit_ppu *ppu);


/*
 ========================================================================
 INPUT. 
 ========================================================================
 */
 
/*
 * Joypad mask
 */
#define JOYPAD_MASK_PRESS 0x01
#define JOYPAD_MASK_LOOSE 0x02
#define JOYPAD_MASK_FIRST 0x80
#define JOYPAD_MASK_PRESS_FIRST (JOYPAD_MASK_PRESS | JOYPAD_MASK_FIRST) 
#define JOYPAD_MASK_LOOSE_FIRST (JOYPAD_MASK_LOOSE | JOYPAD_MASK_FIRST) 

struct joypad {
  int8_t left;
  int8_t right;
  int8_t up;
  int8_t down;
  int8_t b;
  int8_t a;
  int8_t select;
  int8_t start;   
};

/* - normal keymapper - *\
+        left:A          | 
+        right:D         | 
+        up:W            |
+        down:S          |
|        b:J             +
|        a:K             +
|        select:space    +
|        start:enter     +
\* - normal keymapper - */ 


/*
 ========================================================================
 CALLBACK. 
 ========================================================================
 */

#if 0
cvimpl void callc mr10_loop (struct emu8bit_ppu *ppu, struct joypad *pad, uint64_t ticks);
cvimpl void callc mr10_init (struct emu8bit_ppu *ppu);
cvimpl void callc mr10_dtor (void);
#endif 

cvimpl int system_init (void *init, void *loop, void *dtor);
cvimpl int system_schedule (void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif