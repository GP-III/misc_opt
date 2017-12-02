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

#ifndef soundtrack0_H 
#define soundtrack0_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "system.h"

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

cvimpl int callc soundtrack_init (struct pcm_wave *pcm_wave0, soundtrack_t *soundtrack);
cvimpl void callc soundtrack_uninit (soundtrack_t soundtrack);
cvimpl int callc soundtrack_post (soundtrack_t soundtrack, struct audio_cushion *cushion);
cvimpl int callc soundtrack_stop (soundtrack_t soundtrack); 
cvimpl int callc soundtrack_start (soundtrack_t soundtrack);
cvimpl int callc soundtrack_flush (soundtrack_t soundtrack);   
cvimpl int callc soundtrack_openwave (const char *name, struct pcm_wave **wave);  
cvimpl void callc soundtrack_closewave (struct pcm_wave *wave);  

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif