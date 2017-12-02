#ifndef __FSM_CC_INC_
#define __FSM_CC_INC_

#define FSMCC_INVARIANT_JKICK_DELAY 1 
#define FSMCC_INVALIANT_JUMP_FRAME 40 

#include "frame.uuid.h"
#include "type.h"
#include "8bit_host.h"
#include "frame.cody.h"
#include "joypad_buffer_deque.h"
#include "snd.cody.h"

struct fsm_cc {

  struct joypad joypad_current;
  struct joypad joypad_previous;
  struct jpbuffer joypad_infos[2]; /* 1:cur 0:old **/

  CCACT_EVT fevt; 

  enum FDIR_ fdir_current;
  enum FDIR_ fdir_misc;

  uint8 pivot_x; /*** role x coord **/
  uint8 pivot_y; /*** role y coord **/
  uint8 pivot_x_cache; /*** role x coord **/
  uint8 pivot_v; /*** role height[or z???] coord **/
  uint8 vvector; /** **/

  int sys_ticks;
  int cache_ticks;
  int pulse_phase;
  int punch_combine;
  int head_attack_combine;
  int beattack_combine;
  int health;
  int level;
  int punch_level;
  int contin_walk_count;
  int ieparam[24];

  bool_t attack_behavior;
  bool_t attack_ignore; /*** invincible ***/
  bool_t lr_lock[2];
  bool_t ud_lock[2];
  bool_t scroll_map;
  bool_t hit_out;
  bool_t *attach_bg_limit;
  bool_t continue_;
  bool_t bparam[24];
  bool_t joypad_intterupt_mode;
  bool_t punch_last;
  bool_t punch_init;

  int (*epc_callback)(void *eparams);
  int (*epc_callback2)(void *eparams);

  struct rect2 body_rect;
  struct rect2 attack_rect;
  
  struct emu8bit_ppu *ppu_host;

  struct fqueue_section *fs_queue;
  struct jpbuffer_section *jp_queue;

  struct fm_cody_fd  *ccfd;
  struct frame_desc_t *pickframe_cur;

};

void fsm_cc_collect_ctx (struct fsm_cc *cc, struct joypad *jpad, int sys_ticks);
struct fsm_cc *fsm_cc_init (struct emu8bit_ppu *ppu);
void fsm_cc_decode_frame (struct fsm_cc *cc);
void fsm_cc_show_frame (struct fsm_cc *cc);
void fsm_cc_destroy (struct fsm_cc *cc);

#endif