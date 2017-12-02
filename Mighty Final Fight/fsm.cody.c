#include "frame.queue.h"
#include "8bit_host.h"
#include "fsm.cody.h"
#include "frame.cody.h"
#include <windows.h>

extern struct frame_desc_t fm_cody_walk[4];
extern struct frame_desc_t fm_cody_jump[4];
extern struct frame_desc_t fm_cody_jump_kick[1];
extern struct frame_desc_t fm_cody_jump_eblow[1];
extern struct frame_desc_t fm_cody_jump_hook[4];
extern struct frame_desc_t fm_cody_punch_combine[6];


/***
 punch action normal:
 first:after 10 frames can do loop, last:0-8
     before punch:8 frames 
     punch last:4 frames

 loop:6 frames, last:0-8
      before punch:3 frames 
     punch last:4 frames
 ***/
/***
 walk action normal:
 first:after 10 frames can do loop, last:0-8
     before punch:8 frames 
     punch last:4 frames

 loop:6 frames, last:0-8
      before punch:3 frames 
     punch last:4 frames
 ***/

/*
 * some frame speed infos from\
 * fceux's tas editor and virtuanes 
 *
 * hook OAM:CPU's RAM startaddress:0204/0207
 *
 * OAM_struct 
 *   y[one scanline delay]
 *   tid 
 *   attr 
 *   x
 */
static const int vmptV_jump[] = {
  0,
  4,
  3,
  4,
  3,
  4,
  2,
  3,
  3, /* @@!!8 **/
  2,2,2,1,
  2,1,1,0,1,
  0,0,0/* @@!!20 **/,0,
  /* negtive phase **/
  -1,-1,-1,-1,-1,
  -2,-2,-2,-2,
  -3,-3,-3,-3,-3,
  -4,-4,-2,-2,-2/* @@!!35 end maybe .. **/
};

struct jump_desc {
  bool_t disable_hack;
  bool_t kick_done;
  bool_t direction_done;
};

void cctor_jump_cb (struct fqueue *desc, void *ext) {
  soundtrack_post (csnd_jump_start.soundtrack, & csnd_jump_start.auc);
}
void cdtor_jump_cb (struct fqueue *desc, void *ext) {
  soundtrack_post (csnd_jump_end.soundtrack, & csnd_jump_end.auc);
}
void centries_jump_cb (struct fqueue *desc, int elapsed_ticks, void *ext) {
  if (elapsed_ticks < 24) {
    /* enable kick/hack direction when low 24 frames ***/\
      if ( (( (struct fsm_cc *)desc->fsm_object)->joypad_current.b & JOYPAD_MASK_PRESS_FIRST)
      == JOYPAD_MASK_PRESS_FIRST) {
        ( (struct fsm_cc *)desc->fsm_object)->pickframe_cur = & fm_cody_jump_kick[0];
        ( (struct jump_desc *)& desc->evparam[0]) ->kick_done = true_t;
      }
  }
}

void cctor_jump_hook_cb (struct fqueue *desc, void *ext) {

  soundtrack_start (csnd_x_skill.soundtrack);  
  soundtrack_post (csnd_x_skill.soundtrack, & csnd_x_skill.auc);
}

void cdtor_jump_hook_cb (struct fqueue *desc, void *ext) {

  soundtrack_stop (csnd_x_skill.soundtrack);
  soundtrack_post (csnd_x_skill.soundtrack, & csnd_x_skill.auc);
}

void fsm_cc_collect_ctx (struct fsm_cc *cc, struct joypad *jpad, uint64_t sys_ticks) {

  if (cc->joypad_intterupt_mode != false_t) {
    /* TODO:more op **/
    ;
  } else {
    /* current to old ***/
    memcpy (& cc->joypad_infos[1], & cc->joypad_infos[0], sizeof (cc->joypad_infos[0]));
  }

  memcpy (& cc->joypad_previous, & cc->joypad_current, sizeof (*jpad));
  memcpy (& cc->joypad_current, jpad, sizeof (*jpad));

  if (cc->lr_lock[0] == true_t || (cc->lr_lock[1] == true_t) ) {
    /*** ****/
    if (cc->joypad_current.left & JOYPAD_MASK_PRESS) {
      if (! (cc->joypad_current.left & JOYPAD_MASK_FIRST)) {
        cc->joypad_current.left = JOYPAD_MASK_PRESS | JOYPAD_MASK_FIRST;
      }
    }
    if (cc->joypad_current.right & JOYPAD_MASK_PRESS) {
      if (! (cc->joypad_current.right & JOYPAD_MASK_FIRST)) {
        cc->joypad_current.right = JOYPAD_MASK_PRESS | JOYPAD_MASK_FIRST;
      }
    }
  }

  if (cc->ud_lock[0] == true_t || (cc->ud_lock[1] == true_t) ) {
    /*** ****/
    if (cc->joypad_current.up & JOYPAD_MASK_PRESS) {
      if (! (cc->joypad_current.up & JOYPAD_MASK_FIRST)) {
        cc->joypad_current.up = JOYPAD_MASK_PRESS | JOYPAD_MASK_FIRST;
      }
    }
    if (cc->joypad_current.down & JOYPAD_MASK_PRESS) {
      if (! (cc->joypad_current.down & JOYPAD_MASK_FIRST)) {
        cc->joypad_current.down = JOYPAD_MASK_PRESS | JOYPAD_MASK_FIRST;
      }
    }
  }
  cc->sys_ticks = sys_ticks;
}

struct fsm_cc *fsm_cc_init (struct emu8bit_ppu *ppu) {

  struct fsm_cc *p = malloc (sizeof (struct fsm_cc));
  assert (p != null);

  p->attack_behavior = false_t;
  p->attack_ignore = false_t;
  p->fdir_current = FDIR_RIGHT;
  p->fevt = CCACT_NULL;
  p->pivot_x = 20;
  p->pivot_y = 100;
  p->pivot_v = 0;
  p->ppu_host = ppu;
  p->pulse_phase = 2;
  p->punch_last = false_t;
  p->punch_init = false_t;
  p->punch_level= 0;

  fqueue_init (& p->fs_queue);
  jpbuffer_init(& p->jp_queue);

  fm_cody_init (& p->ccfd, ppu);

  return p;
}

void fsm_cc_destroy (struct fsm_cc *cc) {

  fqueue_destroy (cc->fs_queue);
  jpbuffer_destroy(cc->jp_queue);

  fm_cody_uninit (cc->ccfd);
  free (cc);
}

fsm_cc_deal_input (struct fsm_cc *cc) {

}



fsm_cc_set_output (struct fsm_cc *cc) {

}

void ccact_lock (struct fsm_cc *cc, struct fqueue *desc) {

  if (desc->isCtor == true_t) {
    return;
  } else {
    desc->isCtor = true_t;
    desc->dispatch_count = desc->count;
    if (desc->ctor_routine != null) {
      desc->ctor_routine (desc, desc->eparam_ctor);
    }
  }

  if (desc->update_type == FRAME_AUTO_UPDATE) {
    desc->poll_time = cc->sys_ticks;
  } else if (desc->update_type == FRAME_MAN_UPDATE) {
    desc->poll_time = desc->man_time;
  } else if (desc->update_type == FRAME_CONTINUITY_UPDATE) {
    desc->poll_time = cc->sys_ticks - * desc->continuity_time;
    ++* desc->continuity_time;
  }
}

void ccact_runloop (struct fsm_cc *cc, struct fqueue *desc) {

  /* Interpreting walking frames 
   * details:[from virtuanes/fceux]
   * 3:ticks 0->1:
   * loop ticks: 
   *   12 ticks switch action per .
   *   24 ticks switch leg's direction 
   *   ************************************************/
  int elapsed_ticks;
  int cf_num;
  int cf_phase;

  // ccact_lock (cc, desc);
  elapsed_ticks = cc->sys_ticks -desc->poll_time;

  DEBUG_OUTPUT ("run loop elapsed_ticks:%d\n", elapsed_ticks);

  cf_phase = elapsed_ticks % 24;
  cc->vvector = (((elapsed_ticks % 4) == 0 && (elapsed_ticks != 0)) ? 2 : 1);

  if (cf_phase < 12) {
    cf_num = 1;
  } else if ((((elapsed_ticks / 24) %2 == 0))) {
    cf_num = 2;
  } else if ( 1) {
    cf_num = 3;
  }
 
  cc->pickframe_cur = & fm_cody_walk[cf_num];
}

void ccact_jump_ (struct fsm_cc *cc, struct fqueue *desc) {

  int elapsed_ticks;
  int cf_num;
  int8 tem = cc->pivot_v;

  // ccact_lock (cc, desc);
  elapsed_ticks = cc->sys_ticks -desc->poll_time;

  DEBUG_OUTPUT ("ccact_jump_ elapsed_ticks:%d pivot-v:%d\n", elapsed_ticks, tem);

  cc->pivot_v -= vmptV_jump[elapsed_ticks];
  if (elapsed_ticks >= 20) { /* swap frame **/
    cf_num = 1;
  } else { /* normal frame **/
    cf_num = 0;
  }
  if ( ( (struct jump_desc *)& desc->evparam[0]) ->kick_done != false_t) {
    cc->pickframe_cur = & fm_cody_jump_kick[0];
  } else {
    cc->pickframe_cur = & fm_cody_jump[cf_num];
  } 
}

void ccact_jump2_ (struct fsm_cc *cc, struct fqueue *desc) {

  int elapsed_ticks;
  int cf_num;
  int8 tem = cc->pivot_v;

  // ccact_lock (cc, desc);
  elapsed_ticks = cc->sys_ticks -desc->poll_time;

  DEBUG_OUTPUT ("ccact_jump_hook elapsed_ticks:%d pivot-v:%d\n", elapsed_ticks, tem);

  cc->pivot_v -= vmptV_jump[elapsed_ticks];

  if (elapsed_ticks <= 5) {
    /* swap frame **/
    cf_num = 0;
  } else if (elapsed_ticks <= 12)  {
    /* normal frame **/
    cf_num = 1;
  } else if (elapsed_ticks <= 18)  {
    /* normal frame **/
    cf_num = 2;
  } else if (elapsed_ticks <= 25)  {
    /* normal frame **/
    cf_num = 3;
  } else if (elapsed_ticks <= 31)  {
    /* normal frame **/
    cf_num = 1;
  } else if (elapsed_ticks <= 35)  {
    /* normal frame **/
    cf_num = 0;
  } else {
    cf_num = 2;
  }
  cc->pickframe_cur = & fm_cody_jump_hook[cf_num];
}

void ccact_punch0_ (struct fsm_cc *cc, struct fqueue *desc) {

  int elapsed_ticks;
  int cf_num;

  // ccact_lock (cc, desc);
  elapsed_ticks = cc->sys_ticks -desc->poll_time;

  DEBUG_OUTPUT ("ccact_punch0 elapsed_ticks:%d \n", elapsed_ticks);

  if (elapsed_ticks < 7) { 
    cf_num = 0;
  } else if (elapsed_ticks < 11) { 
    /** alive 2 frames ***/
    if (elapsed_ticks == 7)
      soundtrack_post (csnd_punch0.soundtrack, & csnd_punch0.auc);
    cf_num = 1;
  } else {
    cc->punch_last = true_t;
    cc->punch_init = true_t;
    cf_num = 0;
  }
  cc->pickframe_cur = & fm_cody_punch_combine[cf_num];
}

void ccact_punch2loop_ (struct fsm_cc *cc, struct fqueue *desc) {

  int elapsed_ticks;
  int cf_num;

  // ccact_lock (cc, desc);
  elapsed_ticks = cc->sys_ticks -desc->poll_time;

  DEBUG_OUTPUT ("ccact_punch0 elapsed_ticks:%d \n", elapsed_ticks);

  if (elapsed_ticks < 2) { 
    cf_num = 0;
  } else if (elapsed_ticks < 6) { 
    /** alive 2 frames ***/
    if (elapsed_ticks == 2)
      soundtrack_post (csnd_punch0.soundtrack, & csnd_punch0.auc);
    cf_num = 1;
  } else {
    cc->punch_last = true_t;
    cf_num = 0;
  }
  cc->pickframe_cur = & fm_cody_punch_combine[cf_num];
}

void fsm_cc_decode_frame (struct fsm_cc *cc) {

  struct fqueue fq;
  bool_t walk_employ = false_t;
  bool_t dimc_RL = false_t;
  bool_t dimc_TB = false_t;

  memset (& fq, 0, sizeof (fq));

  cc->lr_lock [1] = cc->lr_lock[0];
  cc->ud_lock [1] = cc->ud_lock[0];

  cc->lr_lock[0] = false_t;
  cc->ud_lock[0] = false_t;

  /* Update test conflict key **/
  if ( (cc->joypad_current.right & JOYPAD_MASK_PRESS)
    && (cc->joypad_current.left & JOYPAD_MASK_PRESS)) {

      cc->lr_lock[0] = true_t;
      DEBUG_OUTPUT ("RIGHT and LEFT all press !!!\n");
  } 
  if ( (cc->joypad_current.down & JOYPAD_MASK_PRESS)
    &&        (cc->joypad_current.up & JOYPAD_MASK_PRESS)) {

      cc->ud_lock[0] = true_t;
      DEBUG_OUTPUT ("UP and DOWN all press !!!\n");
  }

  if (cc->lr_lock[0] == true_t) {
    dimc_RL = true_t;
  }

  if (cc->ud_lock[0] == true_t) {
    dimc_TB = true_t;
  }

  fq.fsm_object = cc;
  fq.count = 1;

  /** Continuous action test, interrupt */
  switch (cc->fevt) {

  case CCACT_PUNCH:
    if (cc->punch_init == true_t) {
      if (cc->punch_last != false_t) {
        cc->punch_last = false_t;
        /* hit again, more ****/
        if ((cc->joypad_current.b & JOYPAD_MASK_LOOSE_FIRST) == JOYPAD_MASK_LOOSE_FIRST) {
          /* throw top **/
          fqueue_pop(cc->fs_queue);

          fq.evt_cc = CCACT_PUNCH;
          fq.update_type = FRAME_AUTO_UPDATE;
          fq.dir = cc->fdir_current;
          fq.count = 14;
          fqueue_push(cc->fs_queue, & fq);
        }
      }
    }
    /** test jump-hook again, 
      ** for primitive games, jump-hook has a much higher priority than pure punches **/
    if ( (cc->joypad_current.a & JOYPAD_MASK_PRESS_FIRST) == JOYPAD_MASK_PRESS_FIRST
#  if 0
      && ( (cc->joypad_current.b &JOYPAD_MASK_PRESS_FIRST) == JOYPAD_MASK_PRESS_FIRST))
#  else 
      && ( (cc->joypad_current.b &JOYPAD_MASK_PRESS) ))
#  endif 
    {
      /* throw top **/
      fqueue_pop(cc->fs_queue);

      fq.evt_cc = CCACT_JUMP_HOOK;
      fq.update_type = FRAME_AUTO_UPDATE;
      fq.dir = cc->fdir_current;
      fq.count = FSMCC_INVALIANT_JUMP_FRAME;
      fq.ctor_routine = cctor_jump_hook_cb;
      fq.dtor_routine = cdtor_jump_hook_cb;
      //fq.entries_routine = centries_jump_cb;
      fqueue_push(cc->fs_queue, & fq);
    }
  case CCACT_JUMP:
  case CCACT_JUMP_HOOK:

    return;
  default: break;

  }

  if (dimc_TB == true_t || (dimc_RL == true_t)) {
    return;
  }

  /** first test jump-hook **/
  if ( (cc->joypad_current.a & JOYPAD_MASK_PRESS_FIRST) == JOYPAD_MASK_PRESS_FIRST
#  if 0
    && ( (cc->joypad_current.b &JOYPAD_MASK_PRESS_FIRST) == JOYPAD_MASK_PRESS_FIRST))
#  else 
    && ( (cc->joypad_current.b &JOYPAD_MASK_PRESS) ))
#  endif 
  {
    fq.evt_cc = CCACT_JUMP_HOOK;
    fq.update_type = FRAME_AUTO_UPDATE;
    fq.dir = cc->fdir_current;
    fq.count = FSMCC_INVALIANT_JUMP_FRAME;
    fq.ctor_routine = cctor_jump_hook_cb;
    fq.dtor_routine = cdtor_jump_hook_cb;
    //fq.entries_routine = centries_jump_cb;
    fqueue_push(cc->fs_queue, & fq);
    return ;
  }

  /** test walk right **/
  if (cc->joypad_current.right & JOYPAD_MASK_PRESS) {
    walk_employ = true_t;
    if (cc->joypad_current.right & JOYPAD_MASK_FIRST) {
      if ((cc->joypad_current.left & JOYPAD_MASK_LOOSE_FIRST) == JOYPAD_MASK_LOOSE_FIRST) 
        goto _cont;
      fq.evt_cc = CCACT_NULL;
      fq.dir = FDIR_RIGHT;
      fq.update_type = FRAME_AUTO_UPDATE;
      fqueue_push(cc->fs_queue, & fq);
      fq.evt_cc = CCACT_WALK;
      cc->contin_walk_count = 1;
      fqueue_push(cc->fs_queue, & fq);
    } else {
      _cont:
      fq.evt_cc = CCACT_WALK;
      fq.continuity_time = & cc->contin_walk_count;
      fq.update_type = FRAME_CONTINUITY_UPDATE;
      fq.dir = FDIR_RIGHT;
      fqueue_push(cc->fs_queue, & fq);
    }
  }

  /** test walk left, [Conflict with the walk on the right] */
  if (cc->joypad_current.left & JOYPAD_MASK_PRESS && (walk_employ == false_t)) {
    walk_employ = true_t;
    if (cc->joypad_current.left & JOYPAD_MASK_FIRST) {
      if ((cc->joypad_current.right & JOYPAD_MASK_LOOSE_FIRST) == JOYPAD_MASK_LOOSE_FIRST) 
        goto _cont2;
      cc->fevt = CCACT_WALK;
      fq.evt_cc = CCACT_NULL;
      cc->contin_walk_count = 1;
      fq.dir = FDIR_LEFT;
      fq.update_type = FRAME_AUTO_UPDATE;
      fqueue_push(cc->fs_queue, & fq);
      fq.evt_cc = CCACT_WALK;
      fqueue_push(cc->fs_queue, & fq);
    } else {
      _cont2:
      fq.evt_cc = CCACT_WALK;
      fq.continuity_time = & cc->contin_walk_count;
      fq.update_type = FRAME_CONTINUITY_UPDATE;
      fq.dir = FDIR_LEFT;
      fqueue_push(cc->fs_queue, & fq);
    }
  }

  /** test walk up, [Conflict with the walk on the down] */
  if (cc->joypad_current.up & JOYPAD_MASK_PRESS && (walk_employ == false_t)) {
    walk_employ = true_t;
    if (cc->joypad_current.up & JOYPAD_MASK_FIRST) {
      if ((cc->joypad_current.up & JOYPAD_MASK_LOOSE_FIRST) == JOYPAD_MASK_LOOSE_FIRST) 
        goto _cont25;
      cc->fevt = CCACT_WALK;
      fq.evt_cc = CCACT_NULL;
      cc->contin_walk_count = 1;
      fq.dir = FDIR_LEFT;
      fq.update_type = FRAME_AUTO_UPDATE;
      fqueue_push(cc->fs_queue, & fq);
      fq.evt_cc = CCACT_WALK;
      fqueue_push(cc->fs_queue, & fq);
    } else {
_cont25:
      fq.evt_cc = CCACT_WALK;
      fq.continuity_time = & cc->contin_walk_count;
      fq.update_type = FRAME_CONTINUITY_UPDATE;
      fq.dir = FDIR_LEFT;
      fqueue_push(cc->fs_queue, & fq);
    }
  }

  /* jump ***/
  if (cc->joypad_current.a & JOYPAD_MASK_PRESS && (walk_employ == false_t)) {
    if (cc->joypad_current.a & JOYPAD_MASK_FIRST) {
      struct jump_desc *jdp = (struct jump_desc *) & fq.evparam[0];
      jdp->direction_done = false_t;
      jdp->disable_hack = false_t;
      jdp->kick_done = false_t;

      fq.evt_cc = CCACT_JUMP;
      fq.update_type = FRAME_AUTO_UPDATE;
      fq.dir = cc->fdir_current;
      fq.count = FSMCC_INVALIANT_JUMP_FRAME;
      fq.ctor_routine = cctor_jump_cb;
      fq.dtor_routine = cdtor_jump_cb;
      fq.entries_routine = centries_jump_cb;
      fqueue_push(cc->fs_queue, & fq);
    } 
  }
  /* jump ***/
  if (cc->joypad_current.b & JOYPAD_MASK_PRESS && (walk_employ == false_t)) {
    if (cc->joypad_current.b & JOYPAD_MASK_FIRST) {
      fq.evt_cc = CCACT_PUNCH;
      fq.update_type = FRAME_AUTO_UPDATE;
      fq.dir = cc->fdir_current;
      fq.count = 20;
      fqueue_push(cc->fs_queue, & fq);
    } 
  }


}

void fsm_cc_decode_frame2 (struct fsm_cc *cc) {

}

void fsm_cc_show_frame (struct fsm_cc *cc) {

  /*** get top action infos **/
  struct fqueue_link *ai = cc->fs_queue->root;
  enum FDIR_ temp = FDIR_NULL;

  if (ai != null) {
    bool_t _init = !! ai->desc.isCtor;
    ccact_lock (cc, & ai->desc);
    if (ai->desc.entries_routine != null) {
      ai->desc.entries_routine (& ai->desc, cc->sys_ticks - ai->desc.poll_time,
          ai->desc.eparam_entries);
    }

    switch (ai->desc.evt_cc) {
    case CCACT_JUMP:
      cc->fevt = CCACT_JUMP; 
      ccact_jump_ (cc, & ai->desc);
      break;
    case CCACT_JUMP_HOOK:
      cc->vvector = 0;
      cc->fevt = CCACT_JUMP_HOOK;
      ccact_jump2_ (cc, & ai->desc);
      break;
    case CCACT_PUNCH:
      cc->fevt = CCACT_PUNCH;
      if (cc->punch_init != false_t)
        ccact_punch2loop_(cc, & ai->desc);
      else
        ccact_punch0_(cc, & ai->desc);
      break;
    case CCACT_WALK:
      ccact_runloop (cc, & ai->desc);
    default: break;
    case CCACT_NULL:
      cc->pickframe_cur = & fm_cody_walk[0];;
      break;
    }

    temp = ai->desc.dir;
    cc->fdir_current = temp;
    if ( --ai->desc.count <= 0) {
      switch (ai->desc.evt_cc) {
      case CCACT_JUMP:
        cc->pivot_v = 0; 
        cc->fevt = CCACT_NULL; 
        break;
      case CCACT_PUNCH:
        cc->vvector = 0;
        cc->punch_init = false_t;
        cc->punch_last = false_t;
        cc->fevt = CCACT_NULL;
        break;
      case CCACT_JUMP_HOOK:
        
        cc->pivot_v = 0;
        cc->fevt = CCACT_NULL;
        break;
      case CCACT_WALK:
        cc->fevt = CCACT_NULL; 
      default: break;
      case CCACT_NULL:
        cc->pickframe_cur = & fm_cody_walk[0];;
        break;
      }
      if (ai->desc.dtor_routine != null) {
        ai->desc.dtor_routine (& ai->desc, ai->desc.eparam_dtor);
      }
      /* remove current frame infos **/
      fqueue_pop (cc->fs_queue);
    }
    /** test scroll **/

  } else {
    //__outs:
    /* empty ***/
    cc->pickframe_cur = & fm_cody_walk[0];
    cc->vvector = 0;
    temp = cc->fdir_current;
  }
  //cc->pickframe_cur = & fm_cody_walk[2];
  fm_cody_copyoam (cc->ccfd, cc->pickframe_cur, true_t); 
  cc->pivot_x += temp == FDIR_RIGHT ? cc->vvector : -cc->vvector;
  /** draw frame/or do effect **/
  fm_cody_draw (cc->ccfd, cc->pivot_x, cc->pivot_y + cc->pivot_v,
    temp == FDIR_LEFT ? true_t :false_t, false_t);
  // Sleep (500);
}