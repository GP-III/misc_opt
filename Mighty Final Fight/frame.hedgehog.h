#ifndef __FRAME_EH_INC_
#define __FRAME_EH_INC_

#include "frame.h"

#define FBEH_OAM_LIMIT 16 
#define FBEH_MASK_CLR 0x2dc338

typedef int EHACT_EVT;
#define EHACT_NULL 0 
#define EHACT_WALK (EHACT_NULL+1)
#define EHACT_PUNCH (EHACT_WALK+1)
#define EHACT_BEATTACK (EHACT_PUNCH+1)
#define EHACT_BEKONCKOUT (EHACT_BEATTACK+1)

struct fbeh_fd {
  struct sp_chunk fm_cody_oam_cache[FBEH_OAM_LIMIT];
  struct frame_desc_t *fbeh_oam;

  bool_t oam_use;
  bool_t hfilp_enable;

  void *fsmcc1; /* player 1 **/
  void *fsmcc2; /* player 2 **/

  frame_attr fbeh_oam_crash[FBEH_OAM_LIMIT];

  uint8 locpo_x; 
  uint8 locpo_y; 
  uint8 locpo_oc_x;
  uint8 locpo_oc_y;

  uint8 oamCopyAddress;
  uint8 oamSize;
};

void open_framehedgehog (struct emu8bit_ppu *ppu);
void close_framehedgehog (void);
void **entries_framehedgehog (void);

void fbeh_init (struct fbeh_fd **fdb, struct emu8bit_ppu *host);
void fbeh_uninit (struct fbeh_fd *fd);
void fbeh_copyoam (struct fbeh_fd *fd, struct frame_desc_t *fm_cody_dc, bool_t enable);
void fbeh_drawcache (struct fbeh_fd *fd, uint8 locposx, uint8 locposy, void *vcall, void *hcall);
void fbeh_draw (struct fbeh_fd *fd, uint8 pivot_x, uint8 pivot_y, bool_t hfilp, bool_t vfilp);

#endif