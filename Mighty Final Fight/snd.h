#ifndef __SND_INC_
#define __SND_INC_

#include "type.h"
#include "8bit_host.h"

struct snd_t {
  struct pcm_wave *wave;
  struct audio_cushion auc;
  
  soundtrack_t soundtrack;
};

#endif