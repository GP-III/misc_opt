#include "snd.h"

struct snd_t csnd_background1_0;
//struct snd_t csnd_jump_end;
//struct snd_t csnd_beattacked;
//struct snd_t csnd_punch_attack;
//struct snd_t csnd_x_skill;

void  open_sndbg () 
{
  soundtrack_openwave ("sound\\background\\1_0.wav", & csnd_background1_0.wave);
  //soundtrack_openwave ("sound\\effect\\cody\\jump_end.wav", & csnd_jump_end.wave);\
  soundtrack_openwave ("sound\\effect\\cody\\beattacked.wav", & csnd_beattacked.wave);\
  soundtrack_openwave ("sound\\effect\\cody\\punch_attack.wav", & csnd_punch_attack.wave);\
  soundtrack_openwave ("sound\\effect\\cody\\x_skill.wav", & csnd_x_skill.wave);

  soundtrack_init (csnd_background1_0.wave, & csnd_background1_0.soundtrack);
  //soundtrack_init (csnd_jump_end.wave, & csnd_jump_end.soundtrack);\
  soundtrack_init (csnd_beattacked.wave, & csnd_beattacked.soundtrack);\
  soundtrack_init (csnd_beattacked.wave, & csnd_punch_attack.soundtrack);\
  soundtrack_init (csnd_beattacked.wave, & csnd_x_skill.soundtrack);

  csnd_background1_0.auc.pcm =  csnd_background1_0.wave;
  csnd_background1_0.auc.loop_count = 0;
  csnd_background1_0.auc.quad_tick = -1;
#  if   0
  csnd_jump_end.auc.pcm =  csnd_jump_end.wave;
  csnd_jump_end.auc.loop_count = 0;
  csnd_jump_end.auc.quad_tick = -1;

  csnd_beattacked.auc.pcm =  csnd_beattacked.wave;
  csnd_beattacked.auc.loop_count = 0;
  csnd_beattacked.auc.quad_tick = -1;

  csnd_punch_attack.auc.pcm =  csnd_punch_attack.wave;
  csnd_punch_attack.auc.loop_count = 0;
  csnd_punch_attack.auc.quad_tick = -1;

  csnd_x_skill.auc.pcm =  csnd_x_skill.wave;
  csnd_x_skill.auc.loop_count = 0;
  csnd_x_skill.auc.quad_tick = -1;
  csnd_x_skill.auc.loop_count =-1;
#  endif 
}

void  close_sndbg () 
{
  soundtrack_uninit (csnd_background1_0.soundtrack);
  //\
  soundtrack_uninit (csnd_jump_end.soundtrack);\
  soundtrack_uninit (csnd_beattacked.soundtrack);\
  soundtrack_uninit (csnd_punch_attack.soundtrack);\
  soundtrack_uninit (csnd_x_skill.soundtrack);

  soundtrack_closewave (csnd_background1_0.wave);
  //\
  soundtrack_closewave (csnd_jump_end.wave);\
  soundtrack_closewave (csnd_beattacked.wave);\
  soundtrack_closewave (csnd_punch_attack.wave);  \
  soundtrack_closewave (csnd_x_skill.wave);  
}