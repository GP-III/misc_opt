/*-
 * Copyright (c) 2016 moecmks
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef PPuPORT_H 
#define PPuPORT_H 

#include "setup.h"
 
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*
 * tile_s - base tile unit.
 *
 * Describe the basic information for 8*8 pixel.
 * Used as the basic object of rendering.
 */ 
struct tile_s {
        int8_t      maptable_sel[64];  //    pixels select table index, trans ? -1 : 0~type_counts
        int8_t      dattable_count[64];//    pixels select table count
        int8_t      dattable_nat[64];//      1?trans:notrans
        int8_t      isvailed_tebl;
        int8_t      datcount_type;//         In addition to remaining transparent color outside the color type count
        int8_t      datcount_numb;//In addition to remaining transparent color outside the color numb count
        int32_t     dattable_sel[64]; //  pixels select table 0..63<-> max->min
        int32_t     dattable_org[64];  //    same as name 
      };
      
/*
 * BG mask 
 *
 * MAPPER:palette mode
 * CACHE:use R/W mesh pool
 * ALPHA:blend with trans
 * HFLIP:x-flip
 * VFLIP:y-filp
 */       
#define TEB_MAPPER 0x00000002
#define TEB_CACHE  0x00000004
#define TEB_ALPHA 0x00000010
#define TEB_HFLIP 0x0000080
#define TEB_VFLIP 0x0000100

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
#define TES_CLOSE 0x00000008
#define TES_ALPHA 0x00000010
#define TES_HFLIP 0x0000080
#define TES_VFLIP 0x0000100
#define TES_BACK 0x0000200   
#define TES_FLAGS_GLOBAL 0x00020000
#define TES_ALPHA_GLOBAL 0x80000000
#define TES_AMC_APPEND 0x00004000

/*
 * PPU status mask
 *
 * BG_VISABLE:do bg
 * SP_VISABLE:do sp
 * BG_ALPHA:do alpha with trans 
 * FT_VISABLE:do inlinefont
 * UN_ALPHA:do union blend with onecolor
 */   
#define PPU_BG_VISABLE 0x00000001
#define PPU_SP_VISABLE 0x00000002
#define PPU_BG_ALPHA 0x00000004
#define PPU_FT_VISABLE 0x00000010
#define PPU_UN_ALPHA 0x00000020

/*
 * <bg_chunk_s> - base background unit     
 * 
 * describe the basic information for background vram.
 * total 4096 bg_chunk_s object <32(width) * 32(height+2) * 4(page nums)>
 */                                                             
struct bg_chunk_s {
        int32_t mask; // flags mask 
        uint8_t    section; // mesh interval
        uint8_t    impact; // mesh point of fall
        uint8_t    blend; // alpha count 0-255 <-> trans-100%pixel
      };
      
/*
 * <sp_chunk_s> - base sprite unit     
 * 
 * describe the basic information for sprite, total 256 sp_chunk_s.
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
struct sp_chunk_s {
        struct tile_s cache[8]; // sprite's tile-cache 
        uint8_t chan; // tile's index
        int32_t     mask_common; // global sprite control flags      
        uint8_t        x_coord_common; // global horizontal coordinates  
        uint8_t        y_coord_common; // global vertical coordinates  
        uint8_t        l_blend_common; // global blend count    
        int32_t     mask[8]; // sprite control flags 
        uint8_t        x_coord[8];    //    the X coordinates of the upper left corner of the sprite      
        uint8_t        y_coord[8];  //    the Y coordinates of the upper left corner of the sprite 
        uint8_t        blend[8];        //    alpha count 0-255 <-> nodraw-100%pixel   
        uint8_t        section[8];// grounp- mesh interval
        uint8_t        impact[8];// grounp - mesh point of fall
      };
      
/*
 * <sp_amc_s> - base sprite crash unit list   
 * 
 * add a piece of data to the AMC chain
 * used to do some accurate pixel testing, but this is very slow. 
 * internal frequent use of memio<malloc and free>. 
 * under normal circumstances do not easily try 
 */ 
struct sp_amc_s {
        struct sp_amc_s   *nst; // next list   
        uint8_t    chan; // tile's index
        uint8_t        x_coord_std; // global horizontal coordinates 
        uint8_t        y_coord_std; // global vertical coordinates     
        uint8_t    mask; // control flags
        uint8_t        x_coord;   //  the X coordinates of the upper left corner of the sprite         
        uint8_t        y_coord;  //the Y coordinates of the upper left corner of the sprite 
        uint8_t    section;//mesh interval
        uint8_t        x_coord_imp; // mesh horizontal coordinates  
        uint8_t        y_coord_imp; // mesh vertical coordinates 
        uint32_t       data; // color
        uint8_t    t_color, impact, slot;   // <1>:trans<2>:mesh point of fall<3>:sprite slot
      };
      
/*
 * <vrmscr_s> - vram/scroll infos access 
 * 
 * this structure is used to read and write screen scrolling and vram control, 
 * read the reference is a independent of the actual information cached copy
 */ 
struct vrmscr_s {
        uint8_t    nametable_quad; // Scroll - nametable index 
        uint8_t        x_map_scroll; // Scroll - X[0~255] 
        uint8_t        y_map_scroll; // Scroll - Y[0~239] 
        uint8_t    vram_quad;  // Vram - access page index 
        uint8_t        x_ram_access;  // Vram - access X [0~31] 
        uint8_t        y_ram_access; // Vram - access Y [0~29] 
      };
      
/*
 * <tnslen_s> - about trans, blend     
 *
 * this structure is used to read and write some control information and miscellaneous, 
 * read the reference is a independent of the actual information cached copy
 */ 
struct tnslen_s {
        uint32_t bg_trans, union_trans; // trans/cross color 
        uint8_t      bg_blend; // bg::global blend count 
        uint8_t      union_blend; // xx::global blend count 
      };

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif 
