#include "wave.h"
#include <stdio.h>
#include <math.h>
#ifdef __WINDOWS_
# include <windows.h>
# include <mmsystem.h>
#endif     

__rttype_ void _close_wave (struct pcm_s *wm) {
        dlfree (wm->snd_buf);
        dlfree (wm->infos);
        dlfree (wm);   
}
// Only PCM!!!!, no any check 
__rttype_ struct pcm_s *_open_wave (const char *name) {
    
#ifdef __WINDOWS_ 
  HMMIO mmioHandle;
  MMCKINFO mmckinfoParent;
  MMCKINFO mmckinfoSubChunk;
  struct pcm_s waveHandle;
  mmioHandle = mmioOpenA((LPSTR)name, __null_, MMIO_ALLOCBUF | MMIO_READ);
  if (__null_ == mmioHandle)
      return __null_;
  memset (&mmckinfoParent, 0, sizeof(MMCKINFO));
  memset (&mmckinfoSubChunk, 0, sizeof(MMCKINFO));  
  memset (&waveHandle, 0, sizeof(waveHandle));
  mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
  if (MMSYSERR_NOERROR == mmioDescend(mmioHandle, (LPMMCKINFO) & mmckinfoParent, __null_, MMIO_FINDRIFF)) {
      mmckinfoSubChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
      /* Find 'fmt ' chunk */
      if (MMSYSERR_NOERROR == mmioDescend(mmioHandle, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK)) {
          waveHandle.infos = dlmalloc (mmckinfoSubChunk.cksize);
          waveHandle.wf_size = mmckinfoSubChunk.cksize;
          if (__null_ != waveHandle.infos) {
              if (mmckinfoSubChunk.cksize == mmioRead(mmioHandle, (HPSTR)waveHandle.infos, mmckinfoSubChunk.cksize)) {
                  if (MMSYSERR_NOERROR == mmioAscend(mmioHandle, &mmckinfoSubChunk, 0)) {
                      /* Find 'data' chunk */
                      mmckinfoSubChunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
                      if (MMSYSERR_NOERROR == mmioDescend(mmioHandle, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK)) {    
                          if (0 != mmckinfoSubChunk.cksize) {
                              waveHandle.snd_buf = dlmalloc (mmckinfoSubChunk.cksize + 176400);
                              if (__null_ != waveHandle.snd_buf) {
                                  if (-1 != mmioSeek(mmioHandle, mmckinfoSubChunk.dwDataOffset, SEEK_SET)) {
                                      if (-1 != mmioRead(mmioHandle, waveHandle.snd_buf, mmckinfoSubChunk.cksize)) {
                                          /* Read success !!! */
                                          struct pcm_s *p = dlmalloc (sizeof(struct pcm_s));
										  if (__null_ != p) {
											  mmioClose (mmioHandle, 0);
                                              waveHandle.mb_size = mmckinfoSubChunk.cksize;
                                              waveHandle.fquad_ = ((float)mmckinfoSubChunk.cksize) / (float)waveHandle.infos->average_rate;
                                              waveHandle.quad_ = mmckinfoSubChunk.cksize / waveHandle.infos->average_rate;
											  waveHandle.fsmall_ = 
                                              fmod((float)mmckinfoSubChunk.cksize, 
                                                   (float)waveHandle.infos->average_rate) 
                                                 / (float)waveHandle.infos->average_rate; 
                                              waveHandle.small_ = (int)(waveHandle.fsmall_ * (float)60.000);
                                              memcpy (p, &waveHandle, sizeof (struct pcm_s));
                                              return p;
										  }
                                      }
                                  }
                              }
                          }
                      }
                  }
              }
          }
      }      
  }
  if (__null_ != waveHandle.infos) 
        dlfree (waveHandle.infos);
    
  if (__null_ != waveHandle.snd_buf)
        dlfree (waveHandle.snd_buf);
    
  mmioClose (mmioHandle, 0);
  return __null_;
#else 
#endif     
}