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
 
#ifndef emu_NESppu_H 
#define emu_NESppu_H 

#include "config0.h"
 
#if defined (__cplusplus)
extern "C" {
#endif  /* __cplusplus */

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
cvimpl void callc tilecpy (struct emu8bit_ppu *ppu, 
             uint8_t page2, uint8_t land2, uint8_t page, uint8_t land);
cvimpl void callc fillPPu_scr_page (struct emu8bit_ppu *ppu, const char *scr_file, 
             int cache /* 0 ? rd_mesh : rw_mesh */, uint8_t page);
             
#if defined (__cplusplus)
}
#endif  /* __cplusplus */

#endif 
