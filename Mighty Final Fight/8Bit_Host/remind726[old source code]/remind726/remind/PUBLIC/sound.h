#ifndef __sound_
#define __sound_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "wave.h"

/* Audio buffer */
struct audio_cushion {   
  struct pcm_s *pcm; /* PCM data */
  int quad_begin; /* Data Begin */
  int quad_tick;   /* Data End */
  int small_begin;  /* Mirco Data Begin */
  int small_tick;    /* Mirco Data End */
  int loop_count;   /* Loops Count */
};
/* Audio buffer list */
struct audio_cushion_list {   
  struct audio_cushion_list *nst;
  struct audio_cushion *cushion;
};
struct sound_mps {
  void *callback_handle;
  int cushion_lps;
};
void *init_sound (void *sys, void *wm);
int uninit_sound (void *sm);
/* Nocheck */
void post_audio_cushion (struct sound_mps *sm, struct audio_cushion *ac);
void start_sound (struct sound_mps *sm);
void stop_sound (struct sound_mps *sm);
void clear_audio_cushion (struct sound_mps *sm);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif