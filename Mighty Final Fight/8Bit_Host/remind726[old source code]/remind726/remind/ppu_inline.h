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
 
#ifndef __PPU_INLINE_H_SLLDAS_ISP_
#define __PPU_INLINE_H_SLLDAS_ISP_

#include "ppu.h"
#include <stdio.h>
#include <string.h>

//static const uintptr_t const OFFSET_IXY = 272 * 8 + 8;
//static const uintptr_t const BASE_PITCH = 272;
//static const uintptr_t const PRENDER_ENABLE = 1;
//static const uintptr_t const PRENDER_DIS = 0;

#define _SWAP_OBJECT(__TYPE, a, z)                  \
do {                                                \
   __TYPE t = (a)[0]; (a)[0] = (z)[0]; (z)[0] = t;  \
} while (0)
#define memsetd(                                    \
/* uint32_t * */_des,                               \
/* uint32_t */ _n,                                  \
/* uint32_t */ _size)                               \
do  {                                               \
    uint32_t *des = _des, n = _n, size = _size;     \
    uint32_t i = 0;                                 \
    for (; i != size; i++)                          \
        des[i] = n;                                 \
} while (0)
    
#define _CSV($)((((uintptr_t)($)) << 4) + (((uintptr_t)($)) << 8))    
#define _IEP($)((struct ex2a03_ppu *)($))
#define _GETOAM($)(_IEP($)->oam)
#define _GETPPUBUF($)(&(_IEP($)->sub_data[2184]))

/*                                       
 * graph ppu render uint 
 * the reference : NES PPU rendering model 
 */                                                              
struct ex2a03_ppu {
      /* Complex data type */
      struct vrmscr_s   msc; /* Vram.scroll access */
      struct bg_chunk_s     map[32 << 7]; /* NES-Tile *.*/  
	  struct tile_s     R_mesh[256 << 8], RW_mesh[256 << 8]; /* Mesh cache */
      struct sp_chunk_s       oam[256]; /* Sprite-object *.*/
      struct sp_amc_s       amc_pool[272 << 8]; /* Crash Tst */
      /* ppu control, sprite misc */       
	  int32_t       mask, bg_trans, union_trans; /* Flags mask, trans/cross color */
	  uint32_t              amc_limit; /* ref count */
	  uint8_t               amc_coord_slot[272 << 9]; /* vailed coord slot */
	  uint8_t               amc_hits[272 << 8], bg_blend, union_blend; /* amc Hit area/bg::global blend count/xx::global blend count*/
      /* background, inlinefont 's backbuffer */
      uint8_t       sub_slot[272 << 8], lft_slot[272 << 8]; /* Font attr slot */ 
      uint32_t              sub_data[272 << 8], lft_data[272 << 8]; /* Font pixel slot */     
    }; 
    
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif 

#if 0 
#define ALIGN_MEMSET(_des,_n,_size)                         \
do  {                                                           \
      uint8_t *des = (uint8_t *)(_des);                     \
	  uint8_t fill_byte = (uint8_t)(_n);                        \
      uintptr_t size = (uintptr_t)(_size);           \
      uintptr_t limit_max = (uintptr_t)(16 - (uintptr_t)des & 15);  \
      uintptr_t limit_begin = 0;                                \
	  intptr_t remain_alive = size - limit_max;      \
      if (size < limit_max)                              \
          limit_max = size;                          \
      for (; limit_begin != limit_max; limit_begin++)           \
	         des[limit_begin] = fill_byte;                  \
	  if (remain_alive > 0)                                     \
             memset(&des[limit_max], _n, remain_alive); \
} while (0)
#define ALIGN_DMEMSET(_des,_n,_size)                                        \
do  {                                                                   \
      uint8_t combine[4], combine_[4];                                      \
      uint8_t *lwr_8 = (uint8_t *)(_des);                               \
      uintptr_t size = (uintptr_t)((_size) << 2); /* Get BYTE count */ \
      uintptr_t limit_max = (uintptr_t)(16 - (uintptr_t)lwr_8 & 15); /* Get InitAddressOffset */\
      uintptr_t limit_init = limit_max;                                 \
      uintptr_t limit_begin = 0; /* Init Count */                           \
	  intptr_t remain_alive = size - limit_max; /* Get AliveByte */ \
      *((uint32_t *)&combine_[0]) = (_n); /* Arg Load */            \
      if (size < limit_max)                              \
          limit_max = size;                                  \
      for (; limit_begin != limit_max; limit_begin++)           \
	         lwr_8[limit_begin] = combine_[limit_begin & 3];        \
      if (remain_alive > 0) {                                   \
          uintptr_t t = remain_alive & 3;                           \
          limit_begin = size - 1;                        \
          limit_max = limit_begin - t;                              \
          for (; limit_begin != limit_max; limit_begin--)       \
	             lwr_8[limit_begin] = combine_[limit_begin & 3];    \
          switch (t) {                                          \
          case 2: /* D2 D3 D0 D1 */                                 \
               *((uint16_t *)&combine[0]) = *((uint16_t *)&combine_[2]); \
               *((uint16_t *)&combine[2]) = *((uint16_t *)&combine_[0]); \
                 break;                                         \
          case 1: /* D3 D0 D1 D2 */                                 \
                 combine[0] = combine_[3];                          \
                 combine[3] = combine_[2];                      \
               *((uint16_t *)&combine[1]) = *((uint16_t *)&combine_[0]); \
                 break;                                             \
          case 3: /* D1 D2 D3 D0 */                             \
                 combine[2] = combine_[3];                      \
                 combine[3] = combine_[0];                          \
               *((uint16_t *)&combine[0]) = *((uint16_t *)&combine_[1]); \
                 break;                                         \
          case 0: /* D0 D1 D2 D3 */                                 \
               *((uint32_t *)&combine[0]) = *((uint32_t *)&combine_[0]); \
          default:                                              \
                 break;                                             \
          }                                                     \
          memsetd((uint32_t *)&lwr_8[limit_init], *((uint32_t *)&combine[0]), remain_alive >> 2); \
      }                                                         \
} while (0)   
#ifdef __INTEL_X86_32_ASM_
// #GP(0) If memory operand is not aligned on a 16 byte boundary, regardless of segment
static void __declspec(naked) __memset32(void *des, uint32_t n, uint32_t size) {
    __asm {
        push edi // U save frame <stack++4>
        push ebx // V save frame <stack++4>
        mov edi, [esp+8+4] // U Get desbuffer
        mov eax, [esp+8+8] // V Get fillbyte
        mov ecx, [esp+8+12] // U Get size
        mov ebx, 128 // V Contant
        mov edx, ecx // U Copy Size 
        and ecx, 127 // V % 128
        shr edx, 7 // U / 128 
        je ___MIN_COPY_r // V JCC<Y:Entry RepCopy,N:MAXCOPY>
 
        pxor xmm1, xmm1
        movd xmm0, eax 
        pshufb xmm0, xmm1 // shuffle
        
        align 16
        ___MAX_COPY_s: // Use data < cahce size 
            movdqa [edi+0x00], xmm0 
            movdqa [edi+0x10], xmm0             
            movdqa [edi+0x20], xmm0 
            movdqa [edi+0x30], xmm0 
            movdqa [edi+0x40], xmm0 
            movdqa [edi+0x50], xmm0             
            movdqa [edi+0x60], xmm0 
            movdqa [edi+0x70], xmm0      
            add edi, ebx 
            dec edx 
            jne ___MAX_COPY_s   
        ___MIN_COPY_r:
            rep stosb
            pop ebx 
            pop edi 
            ret                        
    }
}
// #GP(0) If memory operand is not aligned on a 16 byte boundary, regardless of segment
static void __declspec(naked) __memsetd32(void *des, uint32_t n, uint32_t size) {
    __asm {
        push edi // U save frame <stack++4>
        push ebx // V save frame <stack++4>
        mov edi, [esp+8+4] // U Get desbuffer
        mov eax, [esp+8+8] // V Get fillbyte
        mov ecx, [esp+8+12] // U Get size
        mov ebx, 128 // V Contant
        mov edx, ecx // U Copy Size 
        and ecx, 31 // V % 32
        shr edx, 5 // U / 32
        je ___MIN_COPY_r // V JCC<Y:Entry RepCopy,N:MAXCOPY>
 
        pxor xmm1, xmm1
        movd xmm0, eax 
        pshufd xmm0, xmm0, 0 // shuffle
        
        align 16
        ___MAX_COPY_s: // Use data < cahce size 
            movdqa [edi+0x00], xmm0 
            movdqa [edi+0x10], xmm0             
            movdqa [edi+0x20], xmm0 
            movdqa [edi+0x30], xmm0 
            movdqa [edi+0x40], xmm0 
            movdqa [edi+0x50], xmm0             
            movdqa [edi+0x60], xmm0 
            movdqa [edi+0x70], xmm0      
            add edi, ebx 
            dec edx 
            jne ___MAX_COPY_s   
        ___MIN_COPY_r:
            rep stosd
            pop ebx 
            pop edi 
            ret                        
    }
}
#else 
__finline_ void memsetd(uint32_t *des, uint32_t n, uint32_t size) {
    uint32_t i = 0;
    for (; i != size; i++)
        des[i] = n;
}
#define __memset32 memset 
#define __memsetd32 memsetd
#endif 
#endif 