#ifndef __FRAME_CC_INC_
#define __FRAME_CC_INC_

#include "frame.h"

#define FBCC_OAM_LIMIT 20 
#define FBCC_EFFECT_LIMIT 12
#define FBCC_OAM_STARTUPADDRESS 0
#define FBCC_OAM_STARTUPADDRESS2 (FBCC_OAM_LIMIT)
#define FBCC_MASK_CLR 0x2dc338
#define FBCC_TEXTURE_PATH "texture\\cody-0.png"
#define FBCC_TEXTURE_PATH2 "texture\\cody-1.png"

typedef int CCACT_EVT;
#define CCACT_NULL 0 
#define CCACT_WALK (CCACT_NULL+1)
#define CCACT_PUNCH (CCACT_WALK+1)
#define CCACT_WAVE (CCACT_PUNCH+1) 
#define CCACT_JUMP (CCACT_WAVE+1)
#define CCACT_JUMP_DIR (CCACT_JUMP+1)
#define CCACT_JUMP_EBLOW (CCACT_JUMP_DIR+1) 
#define CCACT_JUMP_KICK (CCACT_JUMP_EBLOW+1) 
#define CCACT_JUMP_HOOK (CCACT_JUMP_KICK+1)
#define CCACT_CATCH (CCACT_JUMP_HOOK+1) 
#define CCACT_CATCH_HEAD_KICK (CCACT_CATCH+1)
#define CCACT_CATCH_THROW (CCACT_CATCH_HEAD_KICK+1)
#define CCACT_BEATTACK (CCACT_CATCH_THROW+1)

struct fm_cody_fd {
  struct sp_chunk fm_cody_oam_cache[FBCC_OAM_LIMIT];
  struct sp_chunk fm_cody_effect_cache[FBCC_EFFECT_LIMIT];

  struct frame_desc_t *fm_cody_oam;
  struct frame_desc_t *fm_cody_effect;
  struct emu8bit_ppu *ppu;

  bool_t oam_use;
  bool_t effect_use;
  bool_t hfilp_enable[2];
  bool_t vfilp_enable[2];

  uint8 switch_bank;

  frame_attr fm_cody_oam_crash[FBCC_OAM_LIMIT];
  frame_attr fm_cody_effect_crash[FBCC_EFFECT_LIMIT];

  uint8 locposo_x; /** pivot coord <body> ***/
  uint8 locposo_y; /** pivot coord <body> ***/
  uint8 locposo_oc_x;
  uint8 locposo_oc_y;
  uint8 locpose_x; /** pivot coord <effect> ***/
  uint8 locpose_y; /** pivot coord <effect> ***/
  uint8 locpose_oc_x;
  uint8 locpose_oc_y;
  uint8 oamStartupAddress;
  uint8 oamStartupAddress2;
};

void open_frame_cody (struct emu8bit_ppu *ppu);
void close_frame_cody (void);
void **entries_frame_cody (void);

void fm_cody_init (struct fm_cody_fd **fdb, struct emu8bit_ppu *host);
void fm_cody_uninit (struct fm_cody_fd *fd);
void fm_cody_copyoam (struct fm_cody_fd *fd, struct frame_desc_t *fm_cody_dc, bool_t enable);
void fm_cody_drawcache (struct fm_cody_fd *fd, uint8 locposx, uint8 locposy, void *vcall, void *hcall);
void fm_cody_draw (struct fm_cody_fd *fd, uint8 pivot_x, uint8 pivot_y, bool_t hfilp, bool_t vfilp);

#endif