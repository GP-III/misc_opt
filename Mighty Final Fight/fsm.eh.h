#ifndef __FSM_EH_INC_
#define __FSM_EH_INC_

#include "frame.uuid.h"
#include "type.h"
#include "8bit_host.h"
#include "frame.cc.h"
#include "joypad_buffer_deque.h"
#include "snd.cc.h"

struct fsm_eh {

  CCACT_EVT fevt; 

  enum FDIR_ fdir_current;
  enum FDIR_ fdir_misc;

  uint8 pivot_x; /*** role x coord **/
  uint8 pivot_y; /*** role y coord **/
  uint8 pivot_x_cache; /*** role x coord **/
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

  struct fbcc_fd  *ccfd;
  struct frame_desc_t *pickframe_cur;

};

fsm_cc_collect_ctx (struct fsm_cc *cc, struct joypad *jpad, int sys_ticks);
struct fsm_cc *fsm_cc_init (struct emu8bit_ppu *ppu);
fsm_cc_decode_frame (struct fsm_cc *cc);
fsm_cc_show_frame (struct fsm_cc *cc);
void fsm_cc_destroy (struct fsm_cc *cc);

#endif