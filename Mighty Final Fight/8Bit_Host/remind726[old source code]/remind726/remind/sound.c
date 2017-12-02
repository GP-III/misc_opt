#include "system.h"
#include "wave.h"
#include "async_callback.h"

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
  float aw_sample;
#ifdef __WINDOWS_
  IXAudio2SourceVoice *sound;
  struct tWAVEFORMATEX *tools;
  int wf_size;
#endif       
};
__rttype_ void *init_sound (struct system_s *sys, struct pcm_s *pcm) {
#ifdef __WINDOWS_
  struct sound_mps sm;
  void *ch = init_async_callback();
  
  if (ch == NULL)
      return NULL;
  
  if (S_OK == IXAudio2_CreateSourceVoice (sys->sound_drv, 
                                         &sm.sound, 
                                          pcm->infos, 
                                          0, 
                                          0, 
                                          ch, 
                                          NULL, 
                                          NULL)) {
                                              
    if (S_OK == IXAudio2SourceVoice_Start (sm.sound, 0, XAUDIO2_COMMIT_NOW)) {
        struct sound_mps *p = dlmalloc (sizeof(struct sound_mps));
        if (p) {
            p->tools = dlmalloc (pcm->wf_size);
            if (p->tools) {
                // success 
                memcpy (p->tools, pcm->infos, pcm->wf_size);
                p->wf_size = pcm->wf_size;
                p->sound = sm.sound;
                p->callback_handle = ch;
                p->cushion_lps = 0;
				p->aw_sample = (float)(((double)p->tools->nAvgBytesPerSec) / (double)60.0);
                return p;
            }
            dlfree(p);
        }
    }
    IXAudio2SourceVoice_DestroyVoice (sm.sound);        
  }
  return NULL;
#endif       
}
/* Nocheck */
__rttype_ void post_audio_cushion (struct sound_mps *sm, struct audio_cushion *ac) {
    
    struct XAUDIO2_BUFFER xbuf = {0};
    xbuf.LoopCount = ac->loop_count;
    xbuf.AudioBytes = sm->tools->nAvgBytesPerSec * ac->quad_tick;
	xbuf.AudioBytes+= ((int)(sm->aw_sample * (float)ac->small_tick));
    xbuf.pAudioData = &((unsigned char *)ac->pcm->snd_buf)[(int)(
                          sm->tools->nAvgBytesPerSec * ac->quad_begin 
                        + ((int)(sm->aw_sample * (float)ac->small_begin)))];
    IXAudio2SourceVoice_SubmitSourceBuffer(sm->sound, &xbuf, NULL);
}
__rttype_ void uninit_sound (struct sound_mps *sm, struct audio_cushion *ac)  {
	if (sm->callback_handle)
		uninit_async_callback (sm->callback_handle);
	if (sm->tools)
		dlfree (sm->tools);
	if (sm)
		dlfree (sm);
}
__rttype_ void start_sound (struct sound_mps *sm)  {
	IXAudio2SourceVoice_Start(sm->sound, 0, 0);
}
__rttype_ void stop_sound (struct sound_mps *sm)  {
	IXAudio2SourceVoice_Stop(sm->sound, 0, 0);
}
__rttype_ void clear_audio_cushion (struct sound_mps *sm) {
    IXAudio2SourceVoice_FlushSourceBuffers(sm->sound);
}