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
 
#include "emu_NESppu.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define _SWAP_OBJECT(__TYPE_NOT_P, sWaP_LeFt_P, sWaP_RiGhT_P)    \
do {                                                \
   __TYPE_NOT_P tMeP_vAlUe = *((__TYPE_NOT_P *)(sWaP_RiGhT_P)); \
   *((__TYPE_NOT_P *)(sWaP_RiGhT_P)) = *((__TYPE_NOT_P *)(sWaP_LeFt_P)); \
   *((__TYPE_NOT_P *)(sWaP_LeFt_P)) = tMeP_vAlUe;  \
} while (0)

/*
 * Swap tile, The basis of internal quick sort
 */
finline 
void swap_tebl (struct tile *T, 
                const int32_t l, 
                const int32_t r) 
{  
  int32_t rtol_count;
  int32_t ltor_count;
  /*
   * Swap pixels_select, pixels_counts
   */
  _SWAP_OBJECT(int32_t, & T->dattable_sel[l],   & T->dattable_sel[r]);
  _SWAP_OBJECT(int32_t, & T->dattable_count[l], & T->dattable_count[r]);

  /*
   * Swap mapper_select table right to left
   */
  for (rtol_count = 0; rtol_count != 64; rtol_count++) {
    if (T->maptable_sel[rtol_count] == l) {
      /* Swap, directly */
      T->maptable_sel[rtol_count]  = r;
    } else if (T->maptable_sel[rtol_count] == r) {
      /* Set flags */
      T->maptable_sel[rtol_count]  = -2;      
    }
  }
      
  /*
   * Swap mapper_select table left to right
   */   
  for (ltor_count = 0; ltor_count != 64; ltor_count++) {
    if (T->maptable_sel[ltor_count] == -2) {
      /* Swap, at cur flags pos */
      T->maptable_sel[ltor_count]  = l;
    }
  }
}

/* 
 * partition, The basis of internal quick sort
 */
finline 
int32_t partition (struct tile *T, 
                   int32_t l, 
                   int32_t r) 
{
  int32_t i;
  int32_t c = T->dattable_count[r];
    
  for (i = l - 1; l <= r - 1; l++) {
    if (T->dattable_count[l] > c)
      /* Swap tile */
      swap_tebl (T, ++i, l);
  }
  /* Last pass */
  swap_tebl (T, ++i, r);
            
  return i;
}

/* 
 * percent per RGB transparent 
 */ 
finline 
uint32_t alphamix (const int32_t data, const uint8_t purity) {
  
  uint32_t x, y, z;
 
  x = data & 0x000000FF; //
  y = data & 0x0000FF00; //
  z = data & 0x00FF0000; // limit range 
    
  x = ((x * purity) >> 8) & 0x000000FF; //
  y = ((y * purity) >> 8) & 0x0000FF00; //
  z = ((z * purity) >> 8) & 0x00FF0000; // strong reduce | FIXME:div/shift
    
  return (x | y | z);  
}

/* 
 * saturation addition
 */ 
finline  
uint32_t saddudt (const uint32_t d0, const uint32_t d2) {
  
  uint32_t x, y, z;
  
  x = (d0 & 0x000000FF) + (d2 & 0x000000FF); //
  y = (d0 & 0x0000FF00) + (d2 & 0x0000FF00); //
  z = (d0 & 0x00FF0000) + (d2 & 0x00FF0000); // cross add 
  
  if (x > 0x000000FF) x = 0x000000FF; //
  if (y > 0x0000FF00) y = 0x0000FF00; //
  if (z > 0x00FF0000) z = 0x00FF0000; // limit range 
    
  return (x | y | z);  
}

/* 
 * alpha double
 */  
finline 
uint32_t alphamixd (uint32_t ag, uint32_t ag2, uint8_t ag_s, 
         uint8_t ag_s2) 
{
  return saddudt ( alphamix (ag, ag_s), 
                   alphamix (ag2, ag_s2));  
}
        
finline  void
__alpha_attv_u0 (void *to, int32_t shied, uint8_t count) {

  uint32_t ix, iy;
  uint32_t *to_cache = to;
  int32_t shied_mix = alphamix (shied, count ^ 0xFF);
  
  for (iy = 0; iy != 240; iy++) {
    for (ix = 0; ix != 256; ix += 4) 
    {
      // FIXME:SIMD stream.
      to_cache[ix+0] = saddudt ( alphamix (to_cache[ix+0], count), shied_mix);
      to_cache[ix+1] = saddudt ( alphamix (to_cache[ix+1], count), shied_mix);
      to_cache[ix+2] = saddudt ( alphamix (to_cache[ix+2], count), shied_mix);
      to_cache[ix+3] = saddudt ( alphamix (to_cache[ix+3], count), shied_mix);
    }
    // total calc-adjust in per line. 
    to_cache = & to_cache[272];
  }
}    
         
#if defined (__INTEL_ICL_) && defined (_WIN32) && !defined(_DEBUG) && !defined(DEBUG)
__declspec(naked) void callc __alpha_attv_memset0 (void *to, int32_t shied) {

  __asm {
#  if defined (_WIN64)
      /* WINDOWS's ICC x64 inline asm (AVX256) 
       * more see. https://msdn.microsoft.com/en-us/library/ms235286.aspx
       * rcx <- void *to
       * rdx <- int32_t shied [in fact, is edx]
       ********************************************************************/
       and edx, 0x00FFFFFF
       movd xmm0, edx 
       vbroadcastss ymm0, xmm0 /* DWORD mapper to DWORD ARRAY [numbs:8] XXX:FP*/
       mov rdx, 240 
       mov rax, 4 
       
       align 32 
    icc_x64_loop:
       vmovdqa [rcx+0x00], ymm0 
       vmovdqa [rcx+0x20], ymm0 
       vmovdqa [rcx+0x40], ymm0   
       vmovdqa [rcx+0x60], ymm0  
       vmovdqa [rcx+0x80], ymm0    
       vmovdqa [rcx+0xA0], ymm0  
       vmovdqa [rcx+0xC0], ymm0 
       vmovdqa [rcx+0xE0], ymm0
       add rcx, 256 
       nop        
       dec rax 
       jne icc_x64_loop
       dec rdx 
       lea rcx, [rcx+64]
       mov rax, 4 
       jne icc_x64_loop
       ret 
#  else 
      /* WINDOWS's ICC x86 inline asm (SSE128) 
       * more see. https://msdn.microsoft.com/en-us/library/ms235286.aspx
       * esp+4 <- void *to
       * esp+8 <- int32_t shied
       ********************************************************************/
       mov ecx, [esp+4]
       mov edx, [esp+8]
       and edx, 0x00FFFFFF
       movd xmm0, edx 
       pshufd xmm0, xmm0, 0 /* DWORD mapper to DWORD ARRAY [numbs:4] */
       mov edx, 240 
       mov eax, 8
       
       align 32 
    icc_x86_loop:
       movdqa [ecx+0x00], xmm0 
       movdqa [ecx+0x10], xmm0 
       movdqa [ecx+0x20], xmm0   
       movdqa [ecx+0x30], xmm0  
       movdqa [ecx+0x40], xmm0    
       movdqa [ecx+0x50], xmm0  
       movdqa [ecx+0x60], xmm0 
       movdqa [ecx+0x70], xmm0
       add ecx, 128
       nop        
       dec eax 
       jne icc_x86_loop
       dec edx 
       lea ecx, [ecx+64]
       mov eax, 8 
       jne icc_x86_loop
       ret
#  endif 
  }
}
#else 
finline void
__alpha_attv_memset0 (void *to, int32_t shied) {
      /* use intrin.h? **/
  uint32_t ix, iy;
  uint32_t *to_cache = to;
  uint32_t shied0 = shied & 0x00FFFFFF;

  for (iy = 0; iy != 240; iy++) {
    for (ix = 0; ix != 256; ix += 4) 
    {
      // FIXME:SIMD stream.
      to_cache[ix+0] = shied0;
      to_cache[ix+1] = shied0;
      to_cache[ix+2] = shied0;
      to_cache[ix+3] = shied0;
    }
    // total calc-adjust in per line. 
    to_cache = & to_cache[272];
  }
} 
#endif 
 
finline
void decode_tebl_ (struct tile *R, const int bmap, int32_t *D)
{
  int32_t i;
    
  if (!bmap) 
    memcpy (D, & R->dattable_org[0], 256);
  else for (i = 0; i != 64; i++) {
    if (!R->dattable_nat[i])
  /* Copy mapper */
      D[i] = R->dattable_sel[R->maptable_sel[i]];
  }
}

finline
struct tile *_tile_domain (struct emu8bit_ppu *ppu, 
                   int cache, uint8_t page, uint8_t land)
{
  return 
    ( ( !!cache) != 0 ? & ppu->__rw_mesh[land + page * 256]
                      : & ppu->__rd_mesh[land + page * 256]);
}

finline
struct bg_chunk *_bg_domain (struct emu8bit_ppu *ppu, 
                   unsigned int page, uint8_t x, uint8_t y)
{
  return & ppu->sn_map[page % 3 * 1024 + x + y * 32];
}

finline
struct sp_amc *_amc_alloc (struct emu8bit_ppu *ppu)
{
  int i = 0;
  for (; i != 64*(256+16); i++) {
    if (ppu->amc_cache_enable[i] == 0) {
      ppu->amc_cache_enable[i] = 1;
      ppu->amc_cache[i].alloc_idx = i;
      return & ppu->amc_cache[i];
    }
  }
  assert (0);
  return 0;
}

finline
void _amc_free (struct emu8bit_ppu *ppu, struct sp_amc *amc)
{
  if (ppu->amc_cache_enable[amc->alloc_idx] != 0) {
    ppu->amc_cache_enable[amc->alloc_idx] = 0;
  }
}

finline
void _amc_delete_0 (struct emu8bit_ppu *ppu, struct sp_amc *base_chain)
{
  struct sp_amc *S;
  struct sp_amc *N;
  if (base_chain == (void *) 0)
    return ;
  S = base_chain->level;
  for (; S !=  (void *) 0; S = N)
      N = S->level, _amc_free (ppu, S);//, P->isvailed = 0; // FIXME:...-_-
}

finline
void _amc_destroy (struct emu8bit_ppu *ppu)
{
  uint32_t ii = 0;
  uint32_t id = ppu->amc_lim;
  struct __dir *sp = & ppu->amc_cslot[272*8+8]; /* Directgrounp in amc_list */
  /* Poll's amc_list pointer */
  for (; ii != id; ii++) {
         _amc_delete_0 (ppu, ppu->amc_chain[272*8+8+sp[ii].x+sp[ii].y*272]);
         ppu->amc_chain[272*8+8+sp[ii].x+sp[ii].y*272] = (void *)0;
  }
  memset (& ppu->amc_chain_hit[0], 0x00, sizeof (ppu->amc_chain_hit));
  ppu->amc_lim = 0; /* Reset AMC status */
}

cvimpl //FIXME
void bgticks (struct emu8bit_ppu *ppu)
{
  uint32_t c_page, c_pageh, c_pagev, c_pageh0;
  uint32_t           c_scrollh, c_scrollv, c_scrollt; 
  uint32_t ii, x, y = 31;
  uint32_t mask;
  struct __attv_v __attv_vs;
  struct tile *rend_tile; 
  struct bg_chunk *c_mapPollb, *c_mapPoll0;   
      
  c_page  = ppu->sn_page % 3 << 10;  /* page address in nametale */
  c_pageh = c_page & 1024; /* hori nametable index */
  c_pagev = c_page & 2048; /* vert nametable index */
  c_pageh0= c_pageh ^ 1024; /* hori-neg nametable index */
 
  c_scrollv = (ppu->ss_y >> 3) << 5; /* y address in nametale */
  c_scrollh =  ppu->ss_x >> 3; /* x address in nametale */
  c_scrollt =  c_scrollh; /* line x rend poll/reset in hori-nametable **/
  
  ii = ppu->ss_y & 7;   /* calc offset screen pixel pointer/attr param */
  ii = 2184 - ii * 272 - (unsigned)(ppu->ss_x & 7);
  
  c_mapPollb = & ppu->sn_map[0]; 
  c_mapPoll0 = & c_mapPollb[c_page + c_scrollv + c_scrollh]; 
  /* calc init offset screen pixel pointer/misc param */
  __attv_vs.buf = & ppu->__attv_output.buf[ii]; 
  __attv_vs.atr = & ppu->__attv_output.atr[ii]; 

  /* render <33*31 Tiles[264*248]>
     this could be a very slow process */
  do {
      /* main loop, each scan 33 tiles */
      for (x = 33; x != 0; x--) {
          
         uint32_t s, z; /* last line start/current poll */
         uint32_t g, k, a; /* temp 8/x phase/y phase [with accumulated value]*/
         
         /* Get tile 's mask */
         mask = c_mapPoll0->mask;
         
         /* do CACHE deal, if use cache, we will 
            read data from __rw_mesh */
         rend_tile = _tile_domain (ppu, mask & TEB_CACHE, 
                c_mapPoll0->page, c_mapPoll0->land); // FIXME:myabe,...more value
                    
         /* Hori/Vert mapper */
         switch (mask & (TEB_HFLIP | TEB_VFLIP)) {

         case 0:         s = 64; z = 0; a = 1; k = 0;  break; 
         case TEB_HFLIP: s = 71; z = 7; a =-1; k = 16; break;   
         case TEB_VFLIP: s = -8; z = 56;a = 1; k =-16; break;
         case (TEB_HFLIP | TEB_VFLIP):
                         s = -1; z = 63;a = -1;k =  0; break;
         }

         /* Draw buffer. it may not be so efficient */
         switch (mask & 
         (TEB_MAPPER | TEB_ALPHA)) {
           
         /*
          * MAPPER:
          *
          * use palette mode, first check cur tile chunk
          * (8*8 pixel martix) 's attr, if trans then nodraw
          * second,  
Use a selection table to map a lookup table to get a real pixel
          */     
#define  BG_RENDER_BANK(fs, cs)                     \
case (fs):                                       \
for (; z != s; /* loop out y count */           \
      __attv_vs.atr = & __attv_vs.atr[264], __attv_vs.buf = & __attv_vs.buf[264], z += k) \
 for (g = 8; /* loop out x count */             \
      g-- != 0; z += a)                         \
      if (* __attv_vs.atr++ = rend_tile->dattable_nat[z])          \
          * __attv_vs.buf++ = (cs);            \
      else                                      \
          * __attv_vs.buf++ = ppu->lend_table[c_mapPoll0->lend_land];  \
 break 
         BG_RENDER_BANK (TEB_MAPPER, rend_tile->dattable_sel[rend_tile->maptable_sel[z]]);

         /*
          * ALPHA:
          *
          * simple vector normalization
          *
          * formula:
          * result(rR/rG/rB) = alpha*(dR/dG/dB)+(255-alpha)*(sR/sG/sB)
          * 
          * des:background pixels of the current coordinates
          * src:transparent color of the current ppu element
          */             
         BG_RENDER_BANK (TEB_ALPHA, alphamixd (ppu->lend_table[c_mapPoll0->lend_land], rend_tile->dattable_org[z], 
                0xFF ^ c_mapPoll0->lend_percent, c_mapPoll0->lend_percent));                          
    
         /*
          * ALPHA && MAPPER:
          */               
         BG_RENDER_BANK (TEB_MAPPER | TEB_ALPHA, alphamixd (ppu->lend_table[c_mapPoll0->lend_land], 
               rend_tile->dattable_sel[rend_tile->maptable_sel[z]], 
                0xFF ^ c_mapPoll0->lend_percent, c_mapPoll0->lend_percent));   
                
         /*
          * noaffect
          */         
         BG_RENDER_BANK (0, rend_tile->dattable_org[z]);
#undef   BG_RENDER_BANK         
         }
         
         /* if c_scrollt equal 32, then next action will cross
            x page, switch hori nametable, save old y */
         c_mapPoll0 = ++c_scrollt != 32 ? & c_mapPoll0[1] : & c_mapPollb[c_pageh0 + c_pagev + c_scrollv];  
         
         /* back to correctly buffer */
         __attv_vs.buf = & __attv_vs.buf[-2168];
         __attv_vs.atr = & __attv_vs.atr[-2168];
      }         
      /* if c_scrollv equal 960, then next action will cross
         y page, switch vert nametable */
      if ((c_scrollv += 32) != 960)                      
        c_mapPoll0 = & c_mapPollb[c_pageh + c_pagev + c_scrollv + c_pageh];                     
      else {                                                      
        c_mapPoll0 = & c_mapPollb[c_pageh + c_pagev ^ 2048 + c_scrollh]; 
           
        c_pagev ^= 2048;                         
        c_scrollv = 0;               
      }            
      
      /* back to correctly buffer */      
      __attv_vs.buf = & __attv_vs.buf[1912];
      __attv_vs.atr = & __attv_vs.atr[1912];
       
      /* reset scroll x */
      c_scrollt = c_scrollh;
        
  } while (--y);
}

cvimpl
void spticks (struct emu8bit_ppu *ppu)
{     
  uint8_t backup, *amc_hits, lend_land, lend_percent;
  uint32_t x_coord, y_coord, mask;
  uint32_t i = 255, chan;
  uint32_t dtsv_cache[64];
  
  struct tile *rend_tile; 
  struct sp_chunk *rend_chunk;
  struct sp_amc amc_temp, **amcp, *tempsp;  
  struct __attv_v __attv_vs, __attv_vssp, __attv_vssp0;
  struct __dir *amc_cslot;
  
  /* Init screen, amc's pos */
  __attv_vs.buf = & ppu->__attv_output.buf[2184];
  __attv_vs.atr = & ppu->__attv_output.atr[2184];
  /* Init AMC */
  amcp = & ppu->amc_chain[2184];
  /* Init SPROBB */
  rend_chunk = & ppu->oam[255];
  /* lock init slot, hits */
  amc_hits = & ppu->amc_chain_hit[2184];
  amc_cslot = & ppu->amc_cslot[2184];
  /* Rebuild init status */
  _amc_destroy (ppu);
  
  do {
      uint32_t s, z, v; 
      uint32_t g, k, a, t, u;
         
      /* channel index */
      chan = rend_chunk->chan & 7;
      
      if (rend_chunk->mask_common & TES_FLAGS_GLOBAL)
        mask = rend_chunk->mask_common & ~TES_FLAGS_GLOBAL;
      else 
        mask = rend_chunk->mask[chan];

      if (mask & TES_CLOSE)
        goto __final;
        
      if (mask & TES_SEAT_GLOBAL) 
        x_coord = rend_chunk->x_coord_common, y_coord = rend_chunk->y_coord_common;
      else
        x_coord = rend_chunk->x_coord[chan], y_coord = rend_chunk->y_coord[chan];          
      
      if (y_coord > 239)
        goto __final;
      
      if (mask & TES_ALPHAL_GLOBAL) 
        lend_land = rend_chunk->l_blend_common;
      else
        lend_land = rend_chunk->lend[chan];     
      
      if (mask & TES_ALPHAP_GLOBAL) 
        lend_percent = rend_chunk->l_blend_percent_common;
      else
        lend_percent = rend_chunk->lend_percent[chan];   
       
      if (mask & TES_BACK)
        backup = 0x01;
      else 
        backup = 0xFF;      
      
      u = y_coord * 272 + x_coord; // FIXME:mul

      /* Hori/Vert mapper */
      switch (mask & (TES_HFLIP | TES_VFLIP)) {

      case 0:         s = 64; z = 0; a = 1; k = 0;  break; 
      case TES_HFLIP: s = 71; z = 7; a =-1; k = 16; break;   
      case TES_VFLIP: s = -8; z = 56;a = 1; k =-16; break;
      case (TES_HFLIP | TES_VFLIP):
                      s = -1; z = 63;a = -1;k =  0; break;
      }
         
      amc_temp.x_coord_std = x_coord;
      amc_temp.y_coord_std = y_coord;  
      amc_temp.page = rend_chunk->page[chan];
      amc_temp.land = rend_chunk->land[chan];
      amc_temp.lend = lend_land;
      amc_temp.lend_percent = lend_percent;
      amc_temp.slot = i; // slot 
      amc_temp.chan = chan; // channel mask
      amc_temp.mask = mask; // flags mask

      rend_tile = & rend_chunk->cache[chan];
      decode_tebl_ (rend_tile, mask & TES_MAPPER, & dtsv_cache[0]);    

      __attv_vssp.atr = & __attv_vs.atr[u];
      __attv_vssp.buf = & __attv_vs.buf[u]; 
      __attv_vssp0.atr = & rend_tile->dattable_nat[0];
      __attv_vssp0.buf = & dtsv_cache[0];    
      
      switch (mask &
      (TES_ALPHA | TES_AMC_APPEND)) {

      /*
       * ALPHA:
       *
       * simple vector normalization
       *
       * formula:
       * result(rR/rG/rB) = alpha*(dR/dG/dB)+(255-alpha)*(sR/sG/sB)
       * 
       * des:background pixels of the current coordinates
       * src:sprite pixels of the current coordinates
       */  

#define AMC_NODONE ((void)0)
#define AMC_INSERT                                 \
      amc_temp.data = __attv_vssp0.buf[-1];                              \
      amc_temp.x_coord = amc_temp.x_coord_std + (8 - g);           \
      amc_temp.y_coord = amc_temp.y_coord_std + t / 8;                 \
      amc_temp.x_coord_imp = z & 7;                          \
      amc_temp.y_coord_imp = (z & 48) >> 3;                  \
      \
      v = 272 * (amc_temp.y_coord) + amc_temp.x_coord;               \
      if (0 != amc_hits[v]) {                          \
        amc_hits[v] = 1;                            \
        amc_cslot[v].x = amc_temp.x_coord;         \
        amc_cslot[v].y = amc_temp.y_coord;     \
        ppu->amc_lim++;                            \
      }                                                \
      tempsp = _amc_alloc (ppu);/* = dlmalloc (sizeof(struct spramc_s)); */ \
      (void) memcpy (tempsp, & amc_temp, sizeof (struct sp_amc)); \
      tempsp->level = amcp[s] == null ? null : amcp[v]->level; \
      amcp[s] = tempsp
// s = 64; z = 0; a = 1; k = 0;
#define SP_RENDER_BANK(fs, cs, am)                     \
case (fs):                                       \
t = 0;                                               \
for (; z != s; t += 1, /* loop out y count */    \
      __attv_vssp.atr = & __attv_vssp.atr[264], __attv_vssp.buf = & __attv_vssp.buf[264], z += k)  \
 for (g = 8; /* loop out x count */              \
      g-- != 0; z += a)                          \
      if ((*__attv_vssp.atr != backup) && (0 != __attv_vssp0.atr[z])) {            \
          *__attv_vssp.atr++ = 1;          \
          *__attv_vssp.buf++ = cs;                          \
           am;                                   \
      } else {\
          __attv_vssp.atr++;          \
          __attv_vssp.buf++;       \
      }\
      break
        
      SP_RENDER_BANK (TES_ALPHA, alphamixd (ppu->lend_table[lend_land], __attv_vssp0.buf[z], 
            0xFF ^ lend_percent, lend_percent), AMC_NODONE);   
            
      /*
       * AMC_APPEND:
       *
       * Add a collision chain information for the current pixel.
       * This is a very slow process, there is no need not to try
       */ 
      SP_RENDER_BANK (TES_AMC_APPEND, __attv_vssp0.buf[z], AMC_INSERT);    
      
      /*
       * ALPHA && AMC_APPEND:
       */  
      SP_RENDER_BANK (TES_ALPHA | TES_AMC_APPEND, alphamixd (ppu->lend_table[lend_land], __attv_vssp0.buf[z], 
            0xFF ^ lend_percent, lend_percent), AMC_INSERT); 
            
      /*
       * noaffect:
       */              
      SP_RENDER_BANK (0, __attv_vssp0.buf[z], AMC_NODONE);   
#undef SP_RENDER_BANK
      }
      
  __final:
	  rend_chunk = & rend_chunk[-1];
      
  } while (--i != -1);
}    

cvimpl
void load_page (struct emu8bit_ppu *ppu, uint32_t *texHeadAPartZeroSqu128,
           uint32_t pitchMapperByte,
           int cache /* 0 ? rd_mesh : rw_mesh */, 
           uint8_t page, int32_t page_shield)
{
  int32_t shied = page_shield & 0x00FFFFFF;
  int32_t pitchMapperDword = pitchMapperByte >> 2;
  int32_t p;
  int32_t l;
  int32_t r; 
  int32_t top;
  int32_t stack[64];
  
  struct tile *boff = 
        (!!cache) == 0 ? & ppu->__rd_mesh[page*256]
                       : & ppu->__rw_mesh[page*256];
  struct tile *poll;          
  
  int32_t x;
  int32_t y;
  int32_t z;
  int32_t g;              
  int32_t *pts;
  
  for (y = 0; y != 16; y++) {
    for (x = 0; x != 16; x++) { 
     
       /* Set init-coord buffer */
       pts = & texHeadAPartZeroSqu128[pitchMapperDword * y * 8 + x * 8];
       
       /* Set init-coord tile */
       poll = & boff[x + y * 16];
       
       /*
        * Clear tebl's infos, reset all status 
        */
       for (z = 0; z != 64; z++) {  
       
         poll->dattable_org[z] = pts[z / 8 * pitchMapperDword + z % 8] & 0x00FFFFFF;
         poll->dattable_sel[z] = shied;
         poll->dattable_count[z] = 0;
         poll->maptable_sel[z] = -1;
         poll->dattable_nat[z] = 0; /* default trans */
         poll->datcount_type = 0; /* Init Count-type, Compiler optimization, do not want to write a line*/
         poll->datcount_numb = 0; /* Init Count-numb, Compiler optimization, do not want to write a line */
       }       

       /* Collect infos */
       for (z = 0; z != 64; z++) {
           
         if ( (poll->dattable_org[z] & 0xFFFFFF) != shied) {
               
           /* Atom pixel */
           poll->datcount_numb++;      

           /* Set notrans */
           poll->dattable_nat[z] = 1;

           /* Test whether to repeat the color selection list */
           for (g = 0; poll->dattable_sel[g] != page_shield; g++) {

             if (poll->dattable_sel[g] == poll->dattable_org[z]) {
                 
               /* Atom cur pixel select table 's count */
               poll->dattable_count[g]++;
               
               /* Ignore pixel select table */
               goto repeat_;
             }
           }
               
           /* No repeat:add new pixel in pixel select table .
            * Set init pixel count, Atom type count, and mapper table
            */
           poll->dattable_sel[poll->datcount_type] = poll->dattable_org[z] & 0xFFFFFF;
           poll->dattable_count[poll->datcount_type++] = 1;
           
   repeat_:poll->maptable_sel[z] = g;
     
         }
       }  
      /*
       * fully transparent
       */
       if (poll->datcount_type == 0)
           continue;
      /*
       * Inline tile iterative-quicksort
       */
       top = 1;
       stack[0] = 0;
       stack[1] = poll->datcount_type - 1;

       while (top >= 0) {
           
         r = stack[top--];
         l = stack[top--];
         
         p = partition (poll, l, r);
         
        /* 
         * Left side of pivot
         */          
         if (p - 1 > l) {
             stack[++top] = l;
             stack[++top] = p - 1;
         }
        /* 
         * Right side of pivot
         */
         if (p + 1 < r) {
             stack[++top] = p + 1;
             stack[++top] = r;
         }
       }
    }
  }                  
}

void emu8bitPPuRender (struct emu8bit_ppu *ppu) {
  
  if (ppu->ctl_mask & PPU_BG_VISABLE) {     
      bgticks (ppu);     
      if (ppu->ctl_mask & PPU_BG_ALPHA)
          __alpha_attv_u0 (& ppu->__attv_output.buf[2184], ppu->lend_table[ppu->lland_bg], ppu->llendp_bg);
  } else {
      memset  (& ppu->__attv_output.atr, 0x00, sizeof (ppu->__attv_output.atr));
      __alpha_attv_memset0 (& ppu->__attv_output.buf[2184], ppu->lend_table[ppu->lland_bg] & 0xFFFFFF);
  }
  if (ppu->ctl_mask & PPU_SP_VISABLE) {
           spticks (ppu); 
  }  
  if (ppu->ctl_mask & PPU_UN_ALPHA)
      __alpha_attv_u0 (& ppu->__attv_output.buf[2184], ppu->lend_table[ppu->lland_un], ppu->llendp_un);
}


int emu8bitPPuInit (struct emu8bit_ppu **ppu) {

  int ii =0;
  int ii0 ;
  int8_t *ppu_boff = calloc ( sizeof (struct emu8bit_ppu) + 64, 1);
  struct emu8bit_ppu *ppu0 = null;
  assert (ppu_boff != null);
  /* align 64 bytes **?*/
  ppu0 = (struct emu8bit_ppu *)& ppu_boff[((uintptr_t)64) - ((uintptr_t)ppu_boff) & 63];
  ppu0->memblock = ppu_boff;
  
  /* set OAM disable **/
  for (; ii != 256; ii++) {
    for (ii0 = 0; ii0 != 8; ii0++)
      ppu0->oam[ii].mask[ii0] |= TES_CLOSE;
      ppu0->oam[ii].mask_common |= TES_CLOSE;
    }
  *ppu = ppu0;
   return 0;
}

void emu8bitPPuUninit (struct emu8bit_ppu *ppu) {
  
  free (ppu->memblock);
}

cvimpl void 
sp_write (struct emu8bit_ppu *ppu, uint8_t slot, 
   uint8_t chan, int cache, uint8_t page, uint8_t land)
   {
  memcpy (& ppu->oam[slot].cache[chan & 7], 
     _tile_domain (ppu, !! cache, page, land), 
     sizeof(struct tile));   
         (& ppu->oam[slot])->page[chan & 7] = page;
  (& ppu->oam[slot])->land[chan & 7] = land;
   }

cvimpl void 
sp_write2 (struct emu8bit_ppu *ppu, struct sp_chunk *spc,
          uint8_t chan, int cache, uint8_t page, uint8_t land)
{
  memcpy (& spc->cache[chan & 7], 
    _tile_domain (ppu, !! cache, page, land), 
    sizeof(struct tile));   
}

cvimpl void 
sp_memcpy (struct emu8bit_ppu *ppu, struct sp_chunk *spc, uint8_t slot, 
           uint8_t chan, int cache, uint8_t page, uint8_t land)
{
  memcpy (& spc->cache[chan & 7], 
    _tile_domain (ppu, !! cache, page, land), 
    sizeof(struct tile));   
  (& ppu->oam[slot])->page[chan & 7] = page;
  (& ppu->oam[slot])->land[chan & 7] = land;
}

cvimpl void 
tilecpy (struct emu8bit_ppu *ppu, 
    uint8_t page2, uint8_t land2,
    uint8_t page, uint8_t land)
   {
  memcpy ( _tile_domain (ppu, 1, page2, land2), 
     _tile_domain (ppu, 1, page, land), 
     sizeof(struct tile));   
   }
   
cvimpl void 
tilecpy2 (struct emu8bit_ppu *ppu, 
    uint8_t page2, uint8_t land2,
    uint8_t page, uint8_t land)
   {
  memcpy ( _tile_domain (ppu, 1, page2, land2), 
     _tile_domain (ppu, 0, page, land), 
     sizeof(struct tile));   
   }

cvimpl void 
fetch_bgmap (struct emu8bit_ppu *ppu, 
    const char *fname)
   {  
   }
   
   
   