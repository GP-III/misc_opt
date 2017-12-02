// only for test 

#include "8bit_host.h"
#include "frame.cody.h"

#include <string.h>
#include <windows.h>
#include "global.h"
#include "fsm.cody.h"
#include "snd.background.h"
#include "snd.cody.h"

struct fm_cody_fd *fbcc;

extern struct frame_desc_t fm_cody_walk[4];
extern struct frame_desc_t fm_cody_jump[4];
extern struct frame_desc_t fm_cody_jump_kick[1];
extern struct frame_desc_t fm_cody_jump_eblow[1];
extern struct frame_desc_t fm_cody_jump_hook[4];
extern struct frame_desc_t fm_cody_punch_combine[6];

void callc mr10_loop (struct emu8bit_ppu *ppu, struct joypad *pad, uint64_t ticks) {

  fsm_cc_collect_ctx (GISL_fsmcc, pad, ticks);
  fsm_cc_decode_frame (GISL_fsmcc);
  fsm_cc_show_frame (GISL_fsmcc);
}
void callc mr10_init (struct emu8bit_ppu *ppu) {

  open_frame_cody (ppu);
  open_snd_cody ();
  open_sndbg ();

  GISL_fsmcc =  fsm_cc_init (ppu);
  assert (GISL_fsmcc != null);

  // soundtrack_post (csnd_background1_0.soundtrack, & csnd_background1_0.auc);

  ppu->lend_table[0] = 5566;
  ppu->ctl_mask |= PPU_SP_VISABLE;
}

void callc mr10_dtor (void) {

  close_frame_cody();
  close_snd_cody();
  close_sndbg ();

  fsm_cc_destroy (GISL_fsmcc);
}

int main (void) 
{
#if defined (_WIN32)
  COORD ct = { 72, 200 };
  SMALL_RECT rc = {0, 0, 72-1, 20-1}; 
  SetConsoleWindowInfo (GetStdHandle (STD_OUTPUT_HANDLE), TRUE, & rc);
  SetConsoleScreenBufferSize (GetStdHandle (STD_OUTPUT_HANDLE), ct);
#endif 

  system_init(mr10_init, mr10_loop ,mr10_dtor);
  system_schedule();
  return 0;
}