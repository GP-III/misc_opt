#ifndef __wave_
#define __wave_

#include "setup.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* Same as WAVEFORMATEX */
struct pcm_header {
  unsigned short  type_id;     
  unsigned short  channel_nums; 
  unsigned int sample_rate;
  unsigned int average_rate;
  unsigned short  limit_block_align;
  unsigned short  sample_bits; 
  unsigned short  ext_size; 
};
struct pcm_s {
  struct pcm_header *infos;
  int quad_;
  int small_; 
  int wf_size; /* Header size */
  int mb_size; /* Data size */
  float fquad_;
  float fsmall_;
  void *snd_buf;
};

__rttype_ struct pcm_s *_open_wave (const char *name);
__rttype_ void _close_wave (struct pcm_s *pcm);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif