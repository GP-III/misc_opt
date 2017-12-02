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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND COnametable_RIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR COnametable_RIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN COnametable_RACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
 
#include "ppu_inline.h"
#include <stdarg.h>

 /* =============================================================
 * Swap tile_s, The basis of internal quick sort
 */
__finline_
void swap_tebl (struct tile_s *T, 
                const int32_t l, 
                const int32_t r) 
{  
  int32_t rtol_count;
  int32_t ltor_count;
  /*
   * Swap pixels_select, pixels_counts
   */
  _SWAP_OBJECT(int32_t, &T->dattable_sel[l], &T->dattable_sel[r]);
  _SWAP_OBJECT(int32_t, &T->dattable_count[l], &T->dattable_count[r]);

  /*
   * Swap mapper_select table right to left
   */
  for (rtol_count = 0; rtol_count != 64; rtol_count++)
    if (T->maptable_sel[rtol_count] == l) {
        /* Swap, directly */
        T->maptable_sel[rtol_count]  = r;
    } else if (T->maptable_sel[rtol_count] == r) {
        /* Set flags */
        T->maptable_sel[rtol_count]  = -2;      
    }
      
  /*
   * Swap mapper_select table left to right
   */   
  for (ltor_count = 0; ltor_count != 64; ltor_count++)
    if (T->maptable_sel[ltor_count] == -2) {
        /* Swap, at cur flags pos */
        T->maptable_sel[ltor_count]  = l;
    }
}
 /* ==========================================================
 * partition, The basis of internal quick sort
 */
__finline_ 
int32_t partition (struct tile_s *T, 
                   int32_t l, 
                   int32_t r) 
{
  int32_t i;
  int32_t c = T->dattable_count[r];
    
  for (i = l - 1; l <= r - 1; l++) 
    if (T->dattable_count[l] >= c)
        /* Swap tile */
        swap_tebl (T, ++i, l);
  /* Last pass */
  swap_tebl (T, ++i, r);
            
  return i;
}
/* ======= Decode tebl, ret:A0R8G8B8 pixel ================== */
__finline_
uint32_t *decode_tebl (struct tile_s * R, const int bmap)
{
  static 
  int32_t T[64];
    
  int32_t i;
    
  if (!bmap) 
      return memcpy (&T[0], &R->dattable_org[0], 256);
  else for (i = 0; i != 64; i++)
         if (!R->dattable_nat[i])
  /* Copy mapper */
              T[i] = R->dattable_sel[R->maptable_sel[i]];
                
  return &T[0];
}
/* ======= Decode tebl, ret:A0R8G8B8 pixel ================== */
__finline_
uint32_t *decode_tebl_ (struct tile_s *R, const int bmap, int32_t *D)
{
  int32_t i;
    
  if (!bmap) 
      return &R->dattable_org[0];
  else for (i = 0; i != 64; i++)
         if (!R->dattable_nat[i])
  /* Copy mapper */
              D[i] = R->dattable_sel[R->maptable_sel[i]];

  return &D[0];
}
/* ======= Decode font, ret:font mask ================== */
__finline_
int8_t *decode_font (uint8_t *R)
{
  static 
  int8_t T[64];
  
  int i;
  for (i = 0; i != 64; i++)
       T[i] = !!(R[i >> 3] & (1 << i % 8));
  return &T[0];
}              
/* ======= About alpha ================== */ 
__finline_ 
uint32_t alphamix___ (const int32_t data, const uint8_t shirk) 
{
  uint32_t x, y, z;
 
  x = data & 0x000000FF; //
  y = data & 0x0000FF00; //
  z = data & 0x00FF0000; // limit range 
    
  x = x * shirk / 256 & 0x000000FF; //
  y = y * shirk / 256 & 0x0000FF00; //
  z = z * shirk / 256 & 0x00FF0000; // strong reduce | FIXME:div/shift
    
  return (x | y | z);  
}
/* ======= saturation addition ================== */ 
__finline_ 
uint32_t saddudt (const uint32_t k, const uint32_t l) 
{
  uint32_t x, y, z;
  
  x = (k & 0x000000FF) + (l & 0x000000FF); //
  y = (k & 0x0000FF00) + (l & 0x0000FF00); //
  z = (k & 0x00FF0000) + (l & 0x00FF0000); // cross add 
  
  if (x > 0x000000FF) x = 0x000000FF; //
  if (y > 0x0000FF00) y = 0x0000FF00; //
  if (z > 0x00FF0000) z = 0x00FF0000; // limit range 
    
  return (x | y | z);  
}
/* ======= About alpha ================== */ 
__finline_ 
uint32_t alphamix__ (uint32_t ag, uint32_t ag2, uint8_t ag_s, 
         uint8_t ag_s2) 
{
  return saddudt ( alphamix___ (ag, ag_s), 
                   alphamix___ (ag2, ag_s2));  
}
__finline_
struct tile_s *calc_tile_domain (struct ex2a03_ppu *ppu, 
                   uint32_t fmsk, uint8_t section, uint8_t impact)
{
  return &ppu->R_mesh
  [(UINTPTR_CAST(section) << 8) + impact              // ## FIXME:memory layout dependency
               + (UINTPTR_CAST((fmsk & TEB_CACHE) >> 2) << 16)]; // ## FIXME:bad affect type<enum mask> to int32      
}
__finline_
struct bg_chunk_s *calc_bg_chunk_domain (struct ex2a03_ppu *ppu)
{
  return &ppu->map[ppu->msc.vram_quad * 1024 + ppu->msc.x_ram_access +
                   ppu->msc.x_ram_access * 32];  
}
/* ======= AMC list ================== */
__finline_
int32_t amclist_hadd (struct sp_amc_s *P, struct sp_amc_s *D)
{
  struct sp_amc_s *S;// = P->nst;
  struct sp_amc_s *N;// = dlmalloc (sizeof(struct spramc_s));

  N = dlmalloc (sizeof(struct sp_amc_s));
  if (!N) 
    return -1;
  S = P->nst;
  P->nst = memcpy (N, D, sizeof(struct sp_amc_s));; 
  N->nst = S;
  return 0;
}
__finline_
void amclist_die_gc (struct sp_amc_s *P)
{
  struct sp_amc_s *S;
  struct sp_amc_s *N;
  S = P->nst;
  P->nst = __null_;
  for (; S != __null_; S = N)
      N = S->nst, dlfree (S);//, P->isvailed = 0; // FIXME:...-_-
}
__finline_
void _amc_destroy (struct ex2a03_ppu *ppu)
{
  uint32_t I = 0;
  uint32_t P = ppu->amc_limit << 1;
  uint8_t *SP = &ppu->amc_coord_slot[4368]; /* Directgrounp in amc_list */
  /* Poll's amc_list pointer */
  struct sp_amc_s *AMC = &ppu->amc_pool[2184];
  for (; I != P; I += 2)
         amclist_die_gc (&AMC[_CSV(SP[I+1]) + UINTPTR_CAST(SP[I])]);
  ppu->amc_limit = 0; /* Reset AMC status */
  memset(&ppu->amc_hits[0], 0x00, sizeof(ppu->amc_hits));
}
/* ======= Some inline method ================== */
__finline_
void _alpha (uint32_t *D, uint32_t DATA, uint8_t *A, uint8_t AP)
{
#define I (_CSV(n)+k)
  int32_t n, k;
  for (n = 0; n != 240; n++) 
                        for (k = 0; k != 256; k++)  
           if (!A[I])
                D[I] = alphamix__(D[I], DATA, AP, 255 - AP);
}
// static     
__finline_
void _alpha2 (uint32_t *D, uint32_t DATA, uint8_t AP)
{
  int32_t n, k;
  for (n = 0; n != 240; n++) 
                        for (k = 0; k != 256; k++)  
                D[I] = alphamix__(D[I], DATA, AP, 255 - AP);
}
// static 
__finline_
void _memcpy (int32_t *D, int32_t *S, int8_t *A)
{
  int32_t n, k;
  for (n = 0; n != 240; n++) 
                        for (k = 0; k != 256; k++)  
           if (!A[I])
                D[I] = S[I];
#undef I
}   

                          
#define AMC_NODONE ((void)0)
#define AMC_INSERT                                 \
  AM.data = DTB2[-1];                              \
  AM.x_coord = AM.x_coord_std + (8 - g);           \
  AM.y_coord = AM.y_coord_std + t;                 \
  AM.x_coord_imp = z & 7;                          \
  AM.y_coord_imp = (z & 48) >> 3;                  \
  t = _CSV(AM.y_coord) + AM.x_coord;               \
  if (0 != amc_hits[s]) {                          \
      uintptr_t s = _CSV(AM.y_coord) + AM.x_coord; \
      amc_hits[s] = 1;                            \
      amc_coord_slot[s << 1] = AM.x_coord;         \
      amc_coord_slot[s  * 2 + 1] = AM.y_coord;     \
      ppu->amc_limit++;                            \
  }                                                \
  amclist_hadd (&AS[s], &AM)
  
#define BG_RENDER_BANK(fs,cs)                     \
case (fs):                                       \
for (; z != s; /* loop out y count */           \
      atrbuf = &atrbuf[264], desbuf = &desbuf[264], z += k) \
 for (g = 8; /* loop out x count */             \
      g-- != 0; z += a)                         \
      if (*atrbuf++ = T->dattable_nat[z])          \
          *desbuf++ = ppu->bg_trans;                         \
      else                                      \
          *desbuf++ = (cs);             \
 break
 
#define SP_RENDER_BANK(fs,cs,am)                     \
case (fs):                                       \
t = 0;                                               \
for (; z != s; t += 1, /* loop out y count */    \
      ATB2 = &ATB2[264], DTB2 = &DTB2[264], z += k)  \
 for (g = 8; /* loop out x count */              \
      g-- != 0; z += a)                          \
      if ((*ATB2 == _flags)                       \
      && (0 == T->dattable_nat[z])) {            \
          *ATB2++ = T->dattable_nat[z];          \
          *DTB2++ = cs;                          \
           am;                                   \
      }                                          \
      break
 
static //FIXME
void bgticks (struct ex2a03_ppu *ppu)
{
  uintptr_t nametable_H, _nametable_H, nametable_R, nametable_B;
  uintptr_t poll_pivot_H, poll_pivot_R, poll_pivot_G;  
  uintptr_t i, x, y = 31;
  uint32_t *desbuf; 
  uint8_t *atrbuf;
  uintptr_t mask;
  struct tile_s *T; 
  struct bg_chunk_s *B, *I, *H, *R;   
      
  nametable_B = UINTPTR_CAST(ppu->msc.nametable_quad & 3) << 10; 
  nametable_H = nametable_B & 1024; 
  nametable_R = nametable_B & 2048;
 _nametable_H = nametable_H ^ 1024;
 
  poll_pivot_R =(ppu->msc.y_map_scroll >> 3) << 5;
  poll_pivot_H = ppu->msc.x_map_scroll >> 3;
  poll_pivot_G = poll_pivot_H; 
  
  i = ppu->msc.y_map_scroll & 7;
  i = 2184 - _CSV(i) - UINTPTR_CAST(ppu->msc.x_map_scroll & 7);
  
  B = &ppu->map[0]; 
  H = &B[nametable_H + poll_pivot_H]; 
  I = &H[nametable_R + poll_pivot_R];  
  R = &B[nametable_R + poll_pivot_R + _nametable_H]; 
  
  /* calc init offset screen pixel pointer/misc param */
  desbuf = &ppu->sub_data[i]; 
  atrbuf = &ppu->sub_slot[i]; 

  /* render <33*31 Tiles[264*248]>
     this could be a very slow process */
  do {
      /* main loop, each scan 33 tiles */
      for (x = 33; x != 0; x--) {
          
         uintptr_t s, z; 
         uintptr_t g, k, a;
         
         /* Get tile_s 's mask */
         mask = I->mask;
         
         /* do CACHE deal, if use cache, we will 
            read data from RW_mesh */
         T = 
         calc_tile_domain (ppu, mask & TEB_CACHE, 
                I->section, I->impact); // FIXME:myabe,...more value
                    
         /* Hori/Vert mapper */
         switch (mask & 
         (TEB_HFLIP | TEB_VFLIP)) {
             
         case 0:
            s = 64;
            z = 0;
            a = 1;
            k = 0;
            break;
            
         case TEB_HFLIP:
            s = 71;
            z = 7;
            a =-1;
            k = 16;
            break;
            
         case TEB_VFLIP:
            s = -8;
            z = 56;
            a = 1;
            k =-16;
            break;
            
         case (TEB_HFLIP | TEB_VFLIP):
            s = -1;
            z = 63;
            a = -1;
            k =  0;
            break;
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
         BG_RENDER_BANK(TEB_MAPPER, T->dattable_sel[T->maptable_sel[z]]);
           
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
         BG_RENDER_BANK(TEB_ALPHA, alphamix__(ppu->bg_trans, T->dattable_org[z], 
                0xFF ^ I->blend, I->blend));                          
    
         /*
          * ALPHA && MAPPER:
          */               
         BG_RENDER_BANK(TEB_MAPPER | TEB_ALPHA, alphamix__(ppu->bg_trans, 
                T->dattable_sel[T->maptable_sel[z]], 0xFF ^ I->blend, I->blend));   
                
         /*
          * noaffect
          */      
#if 1      
         BG_RENDER_BANK(0, T->dattable_org[z]);
#else 
// for debug...
    case (0):                                       \
for (; z != s; /* loop out y count */           \
      atrbuf = &atrbuf[264], desbuf = &desbuf[264], z += k) \
 for (g = 8; /* loop out x count */             \
      g-- != 0; z += a)                         \
      if (*atrbuf++ = T->dattable_nat[z])          \
          *desbuf++ = ppu->bg_trans;                         \
      else                                      \
          *desbuf++ = T->dattable_org[z];             \
		  //return;
 break;
#endif
#undef   BG_RENDER_BANK         
         }
         
         /* if poll_pivot_G equal 32, then next action will cross
            x page, switch hori nametable */
         I = ++poll_pivot_G != 32 ? &I[1] : &R[0];  
         
         /* back to correctly buffer */
         desbuf = &desbuf[-2168];
         atrbuf = &atrbuf[-2168];
      }         
      /* if poll_pivot_R equal 960, then next action will cross
         y page, switch vert nametable */
      if ((poll_pivot_R += 32) != 960)                      
           I = &H[poll_pivot_R + nametable_R];                     
      else {                                                      
           I = &H[poll_pivot_R];  
           
           nametable_R ^= 2048;                         
           poll_pivot_R = 0;               
      }            
      
      /* back to correctly buffer */      
      desbuf = &desbuf[1912];
      atrbuf = &atrbuf[1912];
      
      /* reset cross x page ptr */
      R = &B[nametable_R + _nametable_H + poll_pivot_R];
      
      /* reset scroll y */
      poll_pivot_G = poll_pivot_H;
        
  } while (--y);
}
static 
void spticks (struct ex2a03_ppu *ppu)
{     
  uintptr_t x_coord, y_coord, mask, blend;
  uintptr_t i = 255, u, t, chan;
  uint32_t *DTB, *DTB2, *TBUF; 
  uint8_t _flags, *ATB, *ATB2, *amc_hits, *amc_coord_slot;
  uint32_t dt_cache[64];
  const /* in stack, reduce cache miss */
  uintptr_t sf_map_instack[8] = 
  { 0, 1, 7, -1, 0, 8, 56, -8 };
  
  struct tile_s *T; 
  struct sp_chunk_s *P;
  struct sp_amc_s AM, *AS;  
  
  /* Init screen pos */
  ATB = &ppu->sub_slot[2184];
  DTB = &ppu->sub_data[2184];
  /* Init AMC */
  AS = &ppu->amc_pool[2184];
  AM.nst = __null_;  
  /* Init SPROBB */
  P = &ppu->oam[255];
  /* clear hit area */
  amc_hits = &ppu->amc_hits[2184];
  /* lock init slot */
  amc_coord_slot = &ppu->amc_coord_slot[2184 << 1];
  /* Rebuild init status */
  _amc_destroy(ppu);
  
  do {
      uintptr_t s, z; 
      uintptr_t g, k, a;
         
      /* channel index */
      chan = P->chan & 7;
      
      if (P->mask_common & TES_FLAGS_GLOBAL)
           mask = P->mask_common & ~TES_FLAGS_GLOBAL;
      else 
           mask = P->mask[chan];

      if (mask & TES_CLOSE)
            goto __final;
        
      if (mask & TES_SEAT_GLOBAL) 
           x_coord = P->x_coord_common, y_coord = P->y_coord_common;
      else
           y_coord = P->x_coord[chan], y_coord = P->y_coord[chan];          
      
      if (y_coord > 239)
            goto __final;
       
      if (mask & TES_ALPHA_GLOBAL) 
           blend = P->l_blend_common;
      else
           blend = P->blend[chan];   
       
      if (mask & TES_BACK)
          _flags = 0;
      else 
          _flags = 0x5f3759df;      
      
      u = _CSV(y_coord) + x_coord; // FIXME:mul

      /* Hori/Vert mapper */
      z = (mask & TEB_HFLIP) >> 6; // H ## FIXME:bad affect type<enum mask> to intptr
      g = ((mask & TEB_VFLIP) >> 7) + 4; // V ## FIXME:bad affect type<enum mask> to intptr 
     
      a = sf_map_instack[z+1];
      z = sf_map_instack[z] + sf_map_instack[g];
      k = sf_map_instack[g+1] - (a << 3);    
      s = (z ^ 63) + a * 9;       // FIXME: << 3 + base 
       
      AM.x_coord_std = x_coord;
      AM.y_coord_std = y_coord;  
      AM.section = P->section[chan];
      AM.impact = P->impact[chan];
      AM.slot = i; // slot 
      AM.chan = chan; // channel mask
      AM.mask = mask; // flags mask
      
      ATB2 = &ATB[u];
      DTB2 = &DTB[u];    
      
      T = &P->cache[chan];

      TBUF = decode_tebl_(T, mask & TES_MAPPER, &dt_cache[0]);    
      
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
      SP_RENDER_BANK(TES_ALPHA, alphamix__(*DTB2, TBUF[z], 
            0xFF ^ blend, blend), AMC_NODONE);   
            
      /*
       * AMC_APPEND:
       *
       * Add a collision chain information for the current pixel.
       * This is a very slow process, there is no need not to try
       */ 
      SP_RENDER_BANK(TES_AMC_APPEND, TBUF[z], AMC_INSERT);    
      
      /*
       * ALPHA && AMC_APPEND:
       */  
      SP_RENDER_BANK(TES_ALPHA | TES_AMC_APPEND, alphamix__(*DTB2, TBUF[z], 
            0xFF ^ blend, blend), AMC_INSERT); 
            
      /*
       * noaffect:
       */              
      SP_RENDER_BANK(0, TBUF[z], AMC_NODONE);   
#undef SP_RENDER_BANK
      }
      
  __final:
	  P = &P[-1];
      
  } while (--i != -1);
}    
struct ex2a03_ppu *init_ppu (void) {
  /* try malloc */
  int i, m;
  struct ex2a03_ppu *ctor = dlmalloc (sizeof(struct ex2a03_ppu));
  if (ctor) {
    memset (ctor, 0x00, sizeof(struct ex2a03_ppu));
  //memset (&ctor->spramc[0], 0x00, sizeof(ctor->spramc));

	for (i = 0; i != 256; i++) {
		 for (m = 0; m != 8; m++)
		      ctor->oam[i].y_coord[m] = 0xFF;
		 ctor->oam[i].y_coord_common = 0xFF;
	}
    ctor->bg_trans = 0;
      
    memset(&ctor->sub_slot[0], 0x01, sizeof(ctor->sub_slot));
    memset(&ctor->lft_slot[0], 0x01, sizeof(ctor->lft_slot));
  }
  return ctor;
}
void uninit_ppu (struct ex2a03_ppu *ppu) {
  /* try malloc */
        _amc_destroy (ppu);
        dlfree (ppu);
}
#define GETFTTYPEBUF(i) (&common_letter0[0])
static int64_t common_letter0[128] = {
/* mapper D7-D0<->Right-Left */
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
0x0000000000000000, 0x0018001818181818, 0x0000000000363636, 0x00367F36367F3600, 
0x00183E603C067C18, 0x0062660C18306646, 0x00DC66D61C386C38, 0x0000000000303030, 
0x0060301818183060, 0x00060C1818180C06, 0x0000663CFF3C6600, 0x000018187E181800, 
0x060C0C0000000000, 0x000000007E000000, 0x0006060000000000, 0x0002060C18306040, 
0x001C26676373321C, 0x007E181818181C18, 0x007F071E3C70633E, 0x003E63603C18307E, 
0x0030307F33363C38, 0x003E6360603F033F, 0x003E63633F03063C, 0x000C0C0C1830637F, 
0x003E61611E27231E, 0x001E30607E63633E, 0x0018180000181800, 0x0618180000181800, 
0x0000180C060C1800, 0x00007E00007E0000, 0x0000183060301800, 0x000C000C3860633E, 
0x007C06765676663C, 0x0063637F6363361C, 0x003F63633F63633F, 0x003C66030303663C, 
0x001F33636363331F, 0x007F03033F03037F, 0x000303033F03037F, 0x007C66637303067C, 
0x006363637F636363, 0x003C18181818183C, 0x003E636360606078, 0x0063331B0F1B3363, 
0x007E060606060606, 0x0063636B7F7F7763, 0x0063737B7F6F6763, 0x003E63636363633E, 
0x000303033F63633F, 0x006E335B6363633E, 0x0063331B3F63633F, 0x003E63603E03633E, 
0x000C0C0C0C0C0C3F, 0x003E636363636363, 0x00081C3E77636363, 0x0063777F7F6B6363, 
0x0063773E1C3E7763, 0x001818183C666666, 0x007F070E1C38707F, 0x0078181818181878, 
0x00406030180C0602, 0x003C30303030303C, 0x0000000063361C08, 0x007F000000000000, 
0x00000000000C0603, 0x006E333E301E0000, 0x001F3333331F0303, 0x001E3303331E0000, 
0x003E3333333E3030, 0x001E033F331E0000, 0x000C0C0C1E0C0C38, 0x1E303E33333E0000, 
0x00333333331F0303, 0x000C0C0C0C000C00, 0x0E18181818001800, 0x00331B0F1B330303, 
0x000C0C0C0C0C0C0C, 0x006B6B6B7F370000, 0x00333333371B0000, 0x003C6666663C0000, 
0x03031F33331F0000, 0x30303E33333E0000, 0x00030303371B0000, 0x001F301E031E0000, 
0x00380C0C0C3E0C00, 0x00363B3333330000, 0x000C1E3333330000, 0x00367F6B6B6B0000, 
0x00331E0C1E330000, 0x1E303E3333330000, 0x003F060C183F0000, 0x007018180C181870, 
0x0018181818181818, 0x000E18183018180E, 0x0000000030794F06, 0x0000000000000000
};
// __rttype_ 
// uint32_t *uniget (struct ex2a03_ppu *ppu) { return &ppu->fa_data[2184]; }
// FIXME:So bad .. 

void ppu_render (struct ex2a03_ppu *ppu) {
    
  if (ppu->mask & PPU_BG_VISABLE) {     
      bgticks (ppu);     
      if (ppu->mask & PPU_BG_ALPHA)
          _alpha (&ppu->sub_data[2184], ppu->bg_trans, &ppu->sub_slot[2184], ppu->bg_blend);
  } else {
      memset  (&ppu->sub_slot[0], 0x01,                     sizeof(ppu->sub_slot));
      memsetd (&ppu->sub_data[0], ppu->bg_trans & 0xFFFFFF, sizeof(ppu->sub_slot));
  }
  if (ppu->mask & PPU_SP_VISABLE) {
     //_amc_destroy (ppu);
           spticks (ppu); 
  }  
  if (ppu->mask & PPU_FT_VISABLE) {  
      _memcpy (&ppu->sub_data[2184], &ppu->lft_data[2184], &ppu->lft_slot[2184]);
       memset (&ppu->lft_slot[0],     0x01,          sizeof(ppu->lft_slot));
  }
  if (ppu->mask & PPU_UN_ALPHA)
      _alpha2 (&ppu->sub_data[2184], ppu->union_trans, ppu->union_blend);      
}
__rttype_ 
void mesh_padding (struct ex2a03_ppu *ppu, 
    uint32_t *l_vmr_base,
    uint32_t l_stp_base, uint32_t l_chunk)
{
  uint32_t *l_vmr;
  uint32_t  l_stp = l_stp_base >> 2;

  struct tile_s *S = &ppu->R_mesh[l_chunk << 8];
  struct tile_s *P;

  int32_t x;
  int32_t y;
  int32_t z;
  int32_t g;

  int32_t p;
  int32_t l;
  int32_t r; 
  int32_t top;
  int32_t stack[64];

  int32_t tmk = ppu->bg_trans & 0x00FFFFFF;  

  for (y = 0; y != 16; y++) {
      
    for (x = 0; x != 16; x++) { 
     
       /* Set init-coord buffer */
       l_vmr = &l_vmr_base[l_stp * y * 8 + x * 8];
       
       /* Set init-coord tile */
       P = &S[x + y * 16];
       
       /*
        * Clear tebl's infos, reset all status 
        */
       for (z = 0; z != 64; z++) {
           
          P->dattable_org[z] = l_vmr[z / 8 * l_stp + z % 8] & 0x00FFFFFF;
          P->dattable_sel[z] = tmk;
          P->dattable_count[z] = 0;
          P->maptable_sel[z] = -1;
          P->dattable_nat[z] = 1;
       }       

       P->isvailed_tebl = 1; /* Set vaild flag */
       P->datcount_type = 0; /* Init Count-type */
       P->datcount_numb = 0; /* Init Count-numb */
       
       /* Collect infos */
       for (z = 0; z != 64; z++) {
           
          if (P->dattable_org[z] != tmk) {
                
              /* Atom pixel */
              P->datcount_numb++;      

              /* Set notrans */
              P->dattable_nat[z] = 0;

              /* Test whether to repeat the color selection list */
              for (g = 0; P->dattable_sel[g] != tmk; g++)

                  if (P->dattable_sel[g] == P->dattable_org[z]) {
                      
                      /* Atom cur pixel select table 's count */
                      P->dattable_count[g]++;
                      
                      /* Ignore pixel select table */
                      goto repeat_;
                  }
                  
              /* No repeat:add new pixel in pixel select table .
               * Set init pixel count, Atom type count, and mapper table
               */
              P->dattable_sel[P->datcount_type] = P->dattable_org[z];
              P->dattable_count[P->datcount_type++] = 1;
              
      repeat_:P->maptable_sel[z] = g;
      
          }
       }  
      /*
       * fully transparent
       */
       if (P->datcount_type == 0)
           continue;
      /*
       * Inline tile iterative-quicksort
       */
       top = 1;
       stack[0] = 0;
       stack[1] = P->datcount_type - 1;

       while (top >= 0) {
           
         r = stack[top--];
         l = stack[top--];
         
         p = partition (P, l, r);
         
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
__rttype_ 
void __cdecl_ _textout (struct ex2a03_ppu *ppu, 
    uint8_t t, 
    uint8_t x_, uint8_t y_, 
    int32_t data, const char *fmt, ...)
{     
    int x_coord, y_coord, s_x_coord, s_y_coord;
    va_list args; 
    char BUF[1024];
    int i, x, y;

    uint64_t *lb_buf;
    int8_t *ls_buf, *ln_buf;
    int32_t *lp_buf;

    /* Test enable */
    if (!(ppu->mask & PPU_FT_VISABLE))
        return;
    
    /* Get pos */
    x_coord = x_;
    y_coord = y_;

    /* Limit seat */
    if (y_coord > 239)
        y_coord -= 240;
    
    s_x_coord = x_coord;
    s_y_coord = y_coord;
    
    /* Init back-buffer pos */
    lp_buf = &ppu->lft_data[2184];
    ln_buf = &ppu->lft_slot[2184];
    
    /* for stack arg */
    va_start(args, fmt);
    (void)vsprintf(&BUF[0], fmt, args);
    va_end(args);

    /* Get font type */
    lb_buf = GETFTTYPEBUF(t);

    for (i = 0; BUF[i]; i++) {
      
        BUF[i] &= 0x7F;
      
        if (BUF[i] == '\n') {
            
            s_y_coord += 8;
        
            /* Limit seat */
            if (s_y_coord > 239)
                s_y_coord -= 240;
        
            x_coord = s_x_coord;
            y_coord = s_y_coord;
        }  
        else 
        {
            int t_x_coord = x_coord;
            int t_y_coord = y_coord;
            int st_y_coord = y_coord;
            
            /* Get token<ascii> buffer */
            ls_buf = decode_font (&lb_buf[BUF[i]]);

            for (y = 0; y != 8; y++) {
                
                for (x = 0; x != 8; x++) {
                    
                    if (ls_buf[y*8+x]) {    
                        /* Draw font-point */
                        lp_buf[_CSV(y_coord)+x_coord] = data;
                        ln_buf[_CSV(y_coord)+x_coord] = 0;
                    }
                    
                    /* Tst, deal x overflow 
                     * if hit, x reset. Tst, deal y overflow                
                     */
                    if (++ x_coord == 256) {
                        
                        x_coord  = 0;  
                        y_coord += 8;
                        
                        if (y_coord > 239)
                            y_coord -= 240;
                    } 
                }
                /* do step-y. 
                 * Tst, deal y overflow. and reset 
                 */
                if (++ t_y_coord == 240)
                       t_y_coord = 240;   
                   
                x_coord = t_x_coord;
                y_coord = t_y_coord;                
            }
            /* Tst, deal x overflow 
             * if hit, x reset. Tst, deal y overflow again... 
             * an font scan complete..
             */ 
            t_x_coord += 8;
                            y_coord = t_y_coord - 8;  
            if (t_x_coord > 255) {
                        
                        t_x_coord -= 256;  
                          y_coord += 8;
                        
                        if (t_y_coord > 239)
                            t_y_coord -= 240;
                    }             

                            x_coord = t_x_coord;
                // y_coord = t_y_coord;
      }
    }
} 
__rttype_       struct vrmscr_s *vsread (struct ex2a03_ppu *ppu) {
  static 
  struct vrmscr_s T;   
  
  T.nametable_quad = ppu->msc.nametable_quad;
  T.x_map_scroll = ppu->msc.x_map_scroll;  
  T.y_map_scroll = ppu->msc.y_map_scroll;
  T.vram_quad = ppu->msc.vram_quad;
  T.x_ram_access = ppu->msc.x_ram_access;
  T.y_ram_access = ppu->msc.y_ram_access;  
  
  return &T;
}
__rttype_   void vswrite (struct ex2a03_ppu *ppu, 
                            const struct vrmscr_s *R) {
                                
  ppu->msc.nametable_quad = R->nametable_quad;
  ppu->msc.x_map_scroll = R->x_map_scroll;  
  ppu->msc.y_map_scroll = R->y_map_scroll;
  ppu->msc.vram_quad = R->vram_quad;
  ppu->msc.x_ram_access = R->x_ram_access;
  ppu->msc.y_ram_access = R->y_ram_access;  
}
__rttype_       struct tnslen_s *tlread (struct ex2a03_ppu *ppu) {
  static 
  struct tnslen_s T;   
  
  T.bg_trans = ppu->bg_trans;
  T.union_trans = ppu->union_trans;  
  T.bg_blend = ppu->bg_blend;
  T.union_blend = ppu->union_blend;
  
  return &T;
}
__rttype_   void tlwrite (struct ex2a03_ppu *ppu, 
                            const struct tnslen_s *R) {
                                
  ppu->bg_trans = R->bg_trans;
  ppu->union_trans = R->union_trans;  
  ppu->bg_blend = R->bg_blend;
  ppu->union_blend = R->union_blend;
}
__rttype_ uint32_t fmskread (struct ex2a03_ppu *ppu) { return ppu->mask; } 
__rttype_ void fmskwrite (struct ex2a03_ppu *ppu, uint32_t mask) { ppu->mask = mask; } 
__rttype_ void fmskxor (struct ex2a03_ppu *ppu, uint32_t mask) { ppu->mask ^= mask; } 
__rttype_ void fmskor (struct ex2a03_ppu *ppu, uint32_t mask) { ppu->mask |= mask; } 
__rttype_ void fmskand (struct ex2a03_ppu *ppu, uint32_t mask) { ppu->mask &= mask; } 

__rttype_ struct back_s *ksmread (struct ex2a03_ppu *ppu)
{
  static 
  struct bg_chunk_s T;
  
  return memcpy (&T, calc_bg_chunk_domain(ppu), sizeof(T));
} 
__rttype_ void ksmwrite (struct ex2a03_ppu *ppu, const struct bg_chunk_s *R)
{
  memcpy (calc_bg_chunk_domain(ppu), R, sizeof(struct bg_chunk_s));
}
__rttype_ void vestomas (struct ex2a03_ppu *ppu, 
                            uint8_t d_chunk, 
                            uint8_t d_callon,
                            uint8_t s_chunk, 
                            uint8_t s_callon)
{
  memcpy (
  calc_tile_domain(ppu, TEB_CACHE, d_chunk, d_callon),
          calc_tile_domain(ppu, 0, s_chunk, s_callon),
  sizeof(struct tile_s) 
  );
}
__rttype_ void mastomas (struct ex2a03_ppu *ppu, 
                            uint8_t d_chunk, 
                            uint8_t d_callon,
                            uint8_t s_chunk, 
                            uint8_t s_callon)
{
  memcpy (
  calc_tile_domain(ppu, TEB_CACHE, d_chunk, d_callon),
          calc_tile_domain(ppu, TEB_CACHE, s_chunk, s_callon),
  sizeof(struct tile_s) 
  );
}
__rttype_ struct tile_s *vesread (struct ex2a03_ppu *ppu, 
                            uint8_t section, 
                            uint8_t impact)
{
  static 
  struct tile_s T;
  
  return memcpy ( &T, calc_tile_domain(ppu, 0, 
                         section, impact), sizeof(T) );
}
__rttype_ struct tile_s *masread (struct ex2a03_ppu *ppu, 
                            uint8_t section, 
                            uint8_t impact)
{
  static 
  struct tile_s T;
  
  return memcpy ( &T, calc_tile_domain(ppu, 
                   TEB_CACHE, section, impact), sizeof(T) );
}
__rttype_ struct sp_amc_s *amcread (struct ex2a03_ppu *ppu, 
                            uint8_t x_crood, 
                            uint8_t y_crood)
{
  static 
  struct sp_amc_s T;
  
  return 
  memcpy ( &T, &ppu->amc_pool[2184 + _CSV(y_crood) + x_crood], sizeof(T) );
}

__rttype_ void maswrite (struct ex2a03_ppu *ppu, 
                            struct tile_s *R, 
                            uint8_t section, 
                            uint8_t impact)
{
  memcpy ( calc_tile_domain(ppu, TEB_CACHE, 
                section, impact), R, sizeof(*R) );   
}
__rttype_ void oamwrite (struct ex2a03_ppu *ppu, 
          uint8_t slot, 
          int chan, 
          int source, 
          int section, 
          int impact) 
{ 
  memcpy (&ppu->oam[slot].cache[chan & 7], 
     calc_tile_domain(ppu, source ? 0 : TEB_CACHE, section, impact), 
     sizeof(struct tile_s));   
         (&ppu->oam[slot])->section[chan] = section;
  (&ppu->oam[slot])->impact[chan] = impact; 
}
