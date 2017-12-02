#include "snd.h"

struct snd_t csnd_jump_start;
struct snd_t csnd_jump_end;
struct snd_t csnd_beattacked;
struct snd_t csnd_punch0;
struct snd_t csnd_punch1;
struct snd_t csnd_punch2;
struct snd_t csnd_x_skill;

void  open_snd_cody () 
{
  soundtrack_openwave ("sound\\effect\\cody\\jump_start.wav", & csnd_jump_start.wave);
  soundtrack_openwave ("sound\\effect\\cody\\jump_end.wav", & csnd_jump_end.wave);
  soundtrack_openwave ("sound\\effect\\cody\\beattacked.wav", & csnd_beattacked.wave);
  soundtrack_openwave ("sound\\effect\\cody\\punch0.wav", & csnd_punch0.wave);
  soundtrack_openwave ("sound\\effect\\cody\\punch1.wav", & csnd_punch1.wave);
  soundtrack_openwave ("sound\\effect\\cody\\punch2.wav", & csnd_punch2.wave);
  soundtrack_openwave ("sound\\effect\\cody\\x_skill.wav", & csnd_x_skill.wave);
  
  soundtrack_init (csnd_jump_start.wave, & csnd_jump_start.soundtrack);
  soundtrack_init (csnd_jump_end.wave, & csnd_jump_end.soundtrack);
  soundtrack_init (csnd_beattacked.wave, & csnd_beattacked.soundtrack);
  soundtrack_init (csnd_punch0.wave, & csnd_punch0.soundtrack);
  soundtrack_init (csnd_punch1.wave, & csnd_punch1.soundtrack);
  soundtrack_init (csnd_punch2.wave, & csnd_punch2.soundtrack);
  soundtrack_init (csnd_x_skill.wave, & csnd_x_skill.soundtrack);
  
  csnd_jump_start.auc.pcm =  csnd_jump_start.wave;
  csnd_jump_start.auc.loop_count = 0;
  csnd_jump_start.auc.quad_tick = -1;

  csnd_jump_end.auc.pcm =  csnd_jump_end.wave;
  csnd_jump_end.auc.loop_count = 0;
  csnd_jump_end.auc.quad_tick = -1;
  
  csnd_beattacked.auc.pcm =  csnd_beattacked.wave;
  csnd_beattacked.auc.loop_count = 0;
  csnd_beattacked.auc.quad_tick = -1;
  
  csnd_punch0.auc.pcm =  csnd_punch0.wave;
  csnd_punch0.auc.loop_count = 0;
  csnd_punch0.auc.quad_tick = -1;

  csnd_punch1.auc.pcm =  csnd_punch1.wave;
  csnd_punch1.auc.loop_count = 0;
  csnd_punch1.auc.quad_tick = -1;

  csnd_punch2.auc.pcm =  csnd_punch2.wave;
  csnd_punch2.auc.loop_count = 0;
  csnd_punch2.auc.quad_tick = -1;

  csnd_x_skill.auc.pcm =  csnd_x_skill.wave;
  csnd_x_skill.auc.loop_count = 0;
  csnd_x_skill.auc.quad_tick = -1;
  csnd_x_skill.auc.loop_count =-1;
}

void  close_snd_cody () 
{
  soundtrack_uninit (csnd_jump_start.soundtrack);
  soundtrack_uninit (csnd_jump_end.soundtrack);
  soundtrack_uninit (csnd_beattacked.soundtrack);
  soundtrack_uninit (csnd_punch0.soundtrack);
  soundtrack_uninit (csnd_punch1.soundtrack);
  soundtrack_uninit (csnd_punch2.soundtrack);
  soundtrack_uninit (csnd_x_skill.soundtrack);
  
  soundtrack_closewave (csnd_jump_start.wave);
  soundtrack_closewave (csnd_jump_end.wave);
  soundtrack_closewave (csnd_beattacked.wave);
  soundtrack_closewave (csnd_punch0.wave);
  soundtrack_closewave (csnd_punch1.wave);
  soundtrack_closewave (csnd_punch2.wave);
  soundtrack_closewave (csnd_x_skill.wave);  
}
