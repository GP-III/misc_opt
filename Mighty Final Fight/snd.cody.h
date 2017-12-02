#ifndef __SND_CODY_INC_
#define __SND_CODY_INC_

#include "snd.h"

extern struct snd_t csnd_jump_start;
extern struct snd_t csnd_jump_end;
extern struct snd_t csnd_beattacked;
extern struct snd_t csnd_punch0;
extern struct snd_t csnd_punch1;
extern struct snd_t csnd_punch2;
extern struct snd_t csnd_x_skill;

void  open_snd_cody ();
void  close_snd_cody ();

#endif

