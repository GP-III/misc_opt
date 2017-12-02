#include "frame.cody.h"

#define STEP_Y(_nums) \
  ((_nums)*16)
#define STEP_X(_nums0) \
    (_nums0)
#define POS_LOC(xX_, yY_) \
    ((STEP_Y(yY_))+(STEP_X(xX_)))
	
#define PL0 (POS_LOC(IX, IY)+16*0)
#define PL1 (POS_LOC(IX, IY)+16*1)
#define PL2 (POS_LOC(IX, IY)+16*2)
#define PL3 (POS_LOC(IX, IY)+16*3)
#define PL4 (POS_LOC(IX, IY)+16*4)

#undef  IX
#define IX 0 
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_walk0[4][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN} }
};

#undef  IX
#define IX 3 
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_walk1[4][2] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN} }
};

#undef  IX
#define IX 5
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_walk2[4][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, E_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, I_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, I_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, I_EN} }
}; 

#undef  IX
#define IX 8 
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_walk3[4][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, E_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, I_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, I_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, I_EN} }
};

// <<WALK>>=========================================================================================================
struct frame_desc_t fm_cody_walk[4] = {
{4, 3, 0, 0, 1, PIVOT_VECTOR,   2, PIVOT_UNSET, & fm_cody_walk0[0][0]}, 
{4, 2, 4, 0, 1, PIVOT_VECTOR,     0, PIVOT_UNSET, & fm_cody_walk1[0][0]}, 
{4, 3, 3, 0, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_walk2[0][0]}, 
{4, 3, 3, 0, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_walk3[0][0]} 
};

#undef  IX
#define IX 0
#undef  IY
#define IY 9 
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_jump0[5][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, E_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, E_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, E_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN} },
{ {IDP, 0+PL4, E_EN}, {IDP, 1+PL4, I_EN}, {IDP, 2+PL4, I_EN} }
};

#undef  IX
#define IX 0
#undef  IY
#define IY 4
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_jump1[5][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, E_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, E_EN}, {IDP, 2+PL2, I_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, E_EN}, {IDP, 2+PL3, I_EN} },
{ {IDP, 0+PL4, E_EN}, {IDP, 1+PL4, E_EN}, {IDP, 2+PL4, I_EN} }
};

#undef  IX
#define IX 3
#undef  IY
#define IY 4
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_jump2[3][4] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, E_EN} },
{ {IDP, 0+PL2, E_EN}, {IDP, 1+PL2, E_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, E_EN} }
};

#undef  IX
#define IX 7
#undef  IY
#define IY 4
#undef  IDP
#define IDP 0 
struct frame_base_t fm_cody_jump3[4][3] = {
{ {IDP, 0+PL0, E_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN} },
{ {IDP, 0+PL1, E_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN} },
{ {IDP, 0+PL3, E_EN}, {IDP, 1+PL3, E_EN}, {IDP, 2+PL3, I_EN} }
};

// <<JUMP>>=========================================================================================================
struct frame_desc_t fm_cody_jump[4] = {
{5, 3, 2, 3, 1, PIVOT_VECTOR,   2, PIVOT_UNSET, & fm_cody_jump0[0][0]}, 
{5, 3, 3, 2, 1, PIVOT_VECTOR,   2, PIVOT_UNSET, & fm_cody_jump1[0][0]}, 
{3, 4, 0, 0, 1, PIVOT_VECTOR,   2, PIVOT_UNSET, & fm_cody_jump2[0][0]},  // !!! more details
{4, 3, 0, 0, 1, PIVOT_VECTOR,   2, PIVOT_UNSET, & fm_cody_jump3[0][0]}   // !!! more details
};


#undef  IX
#define IX 10
#undef  IY
#define IY 4
#undef  IDP 
#define IDP 0
struct frame_base_t fm_cody_punch_combine0[4][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN} }
};

#undef  IX
#define IX 11
#undef  IY
#define IY 0
#undef  IDP 
#define IDP 0
struct frame_base_t fm_cody_punch_combine1[4][4] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, C_EN} },
{ {IDP, 0+PL2, E_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, E_EN}, {IDP, 3+PL2, E_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN}, {IDP, 3+PL3, I_EN} }
};

#undef  IX
#define IX 13
#undef  IY
#define IY 4
#undef  IDP 
#define IDP 0
struct frame_base_t fm_cody_punch_combine2[4][3] = {
{ {IDP, 0+PL0, I_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, E_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN} }
};

#undef  IX
#define IX 3
#undef  IY
#define IY 7
#undef  IDP 
#define IDP 0
struct frame_base_t fm_cody_punch_combine3[4][4] = {
{ {IDP, 0+PL0, I_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, E_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, E_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN}, {IDP, 3+PL3, I_EN} }
};

#undef  IX
#define IX 13
#undef  IY
#define IY 8
#undef  IDP 
#define IDP 0
struct frame_base_t fm_cody_punch_combine4[4][3] = {
{ {IDP, 0+PL0, I_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, E_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN} }
};

#undef  IX
#define IX 10
#undef  IY
#define IY 8
#undef  IDP 
#define IDP 0
struct frame_base_t fm_cody_punch_combine5[5][2] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN} },
{ {IDP, 0+PL4, C_EN}, {IDP, 1+PL4, E_EN} }
};

// <<PUNCH-COMBINE>>=========================================================================================================
struct frame_desc_t fm_cody_punch_combine[6] = {
{4, 3, 0, 0, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_punch_combine0[0][0]},
{4, 4, 0, 0, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_punch_combine1[0][0]}, 
{4, 3, 0, 0, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_punch_combine2[0][0]}, 
{4, 4, 0, 0, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_punch_combine3[0][0]}, 
{4, 3, 0, 0, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_punch_combine4[0][0]}, 
{5, 2, 0, 0, 1, PIVOT_LINE,     0, PIVOT_UNSET, & fm_cody_punch_combine5[0][0]}
};

#undef  IX
#define IX 9
#undef  IY
#define IY 10
#undef  IDP 
#define IDP 1 
struct frame_base_t fm_cody_jump_kick0[4][4] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, E_EN}, {IDP, 3+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, I_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, E_EN} },
{ {IDP, 0+PL3, I_EN}, {IDP, 1+PL3, E_EN}, {IDP, 2+PL3, E_EN}, {IDP, 3+PL3, I_EN} }
};

// <<JUMP KICK>>=========================================================================================================
struct frame_desc_t fm_cody_jump_kick[1] = {
{4, 4, 2,-3, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_jump_kick0[0][0]}
};

#undef  IX
#define IX 0
#undef  IY
#define IY 4
#undef  IDP 
#define IDP 1 
struct frame_base_t fm_cody_jump_hook0[4][4] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, I_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, C_EN} },
{ {IDP, 0+PL3, E_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN}, {IDP, 3+PL3, E_EN} }
};

#undef  IX
#define IX 4
#undef  IY
#define IY 4
#undef  IDP 
#define IDP 1 
struct frame_base_t fm_cody_jump_hook1[4][5] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, I_EN}, {IDP, 4+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, I_EN}, {IDP, 4+PL1, I_EN} },
{ {IDP, 0+PL2, I_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, I_EN}, {IDP, 4+PL2, E_EN} },
{ {IDP, 0+PL3, E_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN}, {IDP, 3+PL3, E_EN}, {IDP, 4+PL3, E_EN} }
};

#undef  IX
#define IX 8
#undef  IY
#define IY 2
#undef  IDP 
#define IDP 1 
struct frame_base_t fm_cody_jump_hook2[4][4] = {
{ {IDP, 0+PL0, I_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, C_EN} },
{ {IDP, 0+PL1, I_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, C_EN} },
{ {IDP, 0+PL2, E_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, C_EN} },
{ {IDP, 0+PL3, E_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN}, {IDP, 3+PL3, C_EN} }
};

#undef  IX
#define IX 12
#undef  IY
#define IY 2
#undef  IDP 
#define IDP 1 
struct frame_base_t fm_cody_jump_hook3[4][4] = {
{ {IDP, 0+PL0, I_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, E_EN} },
{ {IDP, 0+PL1, I_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, C_EN} },
{ {IDP, 0+PL2, E_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, E_EN} },
{ {IDP, 0+PL3, I_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN}, {IDP, 3+PL3, E_EN} }
};

// <<JUMP-HOOK>>=========================================================================================================
struct frame_desc_t fm_cody_jump_hook[4] = { //// !!!!!!!!!!!!!
{4, 4, 2,-5, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_jump_hook0[0][0]}, 
{4, 5,-7,-5, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_jump_hook1[0][0]}, 
{4, 4,-10,-5,1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_jump_hook2[0][0]}, 
{4, 4,-2,-5, 1, PIVOT_VECTOR,   0, PIVOT_UNSET, & fm_cody_jump_hook3[0][0]} 
};

#ifdef UNLOCK_TEST


#undef  IX
#define IX 9
#undef  IY
#define IY 10
#undef  IDP 
#define IDP 1 
struct frame_base_t fm_cody_jump_kick0[4][4] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, E_EN}, {IDP, 3+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, I_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, E_EN} },
{ {IDP, 0+PL3, I_EN}, {IDP, 1+PL3, E_EN}, {IDP, 2+PL3, E_EN}, {IDP, 3+PL3, I_EN} }
};

// <<JUMP KICK>>=========================================================================================================
struct frame_desc_t fm_cody_jump_kick[1] = {
{4, 4, 0, 0, 2, 0, & fm_cody_jump_kick0[0][0]}
};

#undef  IX
#define IX 10
#undef  IY
#define IY 6
#undef  IDP 
#define IDP 1 
struct frame_base_t fm_cody_jump_eblow0[4][4] = {
{ {IDP, 0+PL0, I_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, E_EN} },
{ {IDP, 0+PL1, E_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, E_EN} },
{ {IDP, 0+PL3, I_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN}, {IDP, 3+PL3, E_EN} }
};
// <<JUMP EBLOW>>=========================================================================================================
struct frame_desc_t fm_cody_jump_eblow[1] = {
{4, 4, 0, 0, 2, 0, & fm_cody_jump_eblow0[0][0]}
};





#endif 


void fm_cody_copyoam (struct fm_cody_fd *fd, struct frame_desc_t *fm_cody_dc, bool_t enable) {
  uint8 ccix, cciy , cci;
   
  fd->oam_use = !! enable;
  fd->fm_cody_oam = fm_cody_dc;
  /**** dis all ***/
  for (cci = 0; cci != FBCC_OAM_LIMIT; cci++) {
    fd->fm_cody_oam_cache[cci].mask_common =TES_CLOSE | TES_FLAGS_GLOBAL | TES_SEAT_GLOBAL;
    fd->fm_cody_oam_cache[cci].mask[0]= TES_CLOSE | TES_FLAGS_GLOBAL | TES_SEAT_GLOBAL;
    fd->fm_cody_oam_cache[cci].chan = 0;
    fd->fm_cody_oam_cache[cci].x_coord_common = 0;
    fd->fm_cody_oam_cache[cci].y_coord_common = 0;
    fd->fm_cody_oam_crash[cci] = I_EN;
  }

  /**** cody cache, copy it **/
  for (cciy = fm_cody_dc->yline - 1; cciy != 0xFF; cciy--) {
    for (ccix = 0; ccix != fm_cody_dc->xline; ccix++) {
      uint8 ii = cciy *fm_cody_dc->xline + ccix;
      fd->fm_cody_oam_crash[ii] 
               = (fm_cody_dc->desc[ii].attr == C_EN)
                 ? fm_cody_dc->desc[ii].attr : fm_cody_dc->desc[ii].attr;
       if (fm_cody_dc->desc[ii].attr != I_EN) {
         /*** open display, copy cache ***/
         fd->fm_cody_oam_cache[ii].mask_common =TES_FLAGS_GLOBAL | TES_SEAT_GLOBAL;
         fd->fm_cody_oam_cache[ii].mask[0]= TES_FLAGS_GLOBAL | TES_SEAT_GLOBAL;
         fd->fm_cody_oam_cache[ii].chan = 0;
         fd->fm_cody_oam_cache[ii].x_coord_common = ccix << 3;
         fd->fm_cody_oam_cache[ii].y_coord_common = cciy << 3;

         sp_write2(fd->ppu, & fd->fm_cody_oam_cache[ii], 0, 1, fm_cody_dc->desc[ii].page,
           fm_cody_dc->desc[ii].lend );
       }
    }
  }
}

void fm_cody_init (struct fm_cody_fd **fdb, struct emu8bit_ppu *host) {

  struct fm_cody_fd *pmw = calloc (sizeof (struct fm_cody_fd), 1);
  assert (pmw != null);

  pmw->oamStartupAddress = FBCC_OAM_STARTUPADDRESS;
  pmw->oamStartupAddress2= FBCC_OAM_STARTUPADDRESS2;

  pmw->ppu = host;
  * fdb = pmw;
}

void fm_cody_uninit (struct fm_cody_fd *fd) {

  free (fd);
}

static 
void * framecody[2];


void  open_frame_cody (struct emu8bit_ppu *ppu) {

  framecody[0] = load_imageSqu128 (FBCC_TEXTURE_PATH);
  framecody[1] = load_imageSqu128 (FBCC_TEXTURE_PATH2);

  load_page (ppu, framecody[0], 512, 1, 0, FBCC_MASK_CLR);
  load_page (ppu, framecody[0], 512, 0, 0, FBCC_MASK_CLR);
  load_page (ppu, framecody[1], 512, 1, 1, FBCC_MASK_CLR);
  load_page (ppu, framecody[1], 512, 0, 1, FBCC_MASK_CLR);
}

void  close_frame_cody (void) {

  free_imageSqu128 (framecody[0]);
  free_imageSqu128 (framecody[1]);
}

void **entries_framecody (void) {
  return & framecody[0];
}

void fm_cody_draw (struct fm_cody_fd *fd, 
           uint8 pivot_x, 
           uint8 pivot_y, bool_t hfilp, bool_t vfilp) {

  if (fd->oam_use != false_t && (fd->fm_cody_oam != null)) {

    fd->locposo_x = pivot_x;
    fd->locposo_y = pivot_y;
    fd->locposo_oc_x = pivot_x + fd->fm_cody_oam->hoff;
    fd->locposo_oc_y = pivot_y + fd->fm_cody_oam->voff;
    
    /*** first, draw primitive ****/
    {
      uint8 ex, ey;
      uint8 posy = fd->locposo_y - fd->fm_cody_oam->yline *8;

      /**** cody cache, copy it **/
      for (ey = fd->fm_cody_oam->yline - 1; ey != 0xFF; ey--) {
        for (ex = 0; ex != fd->fm_cody_oam->xline; ex++) {
          uint8 ii = ey *fd->fm_cody_oam->xline + ex;
          if (fd ->fm_cody_oam_crash[ii] != I_EN) {
            /*** reloc pos  ***/
            fd->fm_cody_oam_cache[ii].mask_common = TES_FLAGS_GLOBAL | TES_SEAT_GLOBAL;// | TES_HFLIP | TES_VFLIP;
            fd->fm_cody_oam_cache[ii].mask[0] = TES_FLAGS_GLOBAL | TES_SEAT_GLOBAL;// | TES_HFLIP | TES_VFLIP;
            fd->fm_cody_oam_cache[ii].chan = 0;
            fd->fm_cody_oam_cache[ii].x_coord_common = pivot_x + (ex << 3);
            fd->fm_cody_oam_cache[ii].y_coord_common = posy + (ey << 3);
          }
        }
      }

    }

    /***** filp - hori *****/
    if ( hfilp != false_t) {
      hswap_oam (fd->fm_cody_oam, & fd->fm_cody_oam_cache[0], pivot_x, & fd->locpose_oc_x, & fd->fm_cody_oam_crash[0], "cody_frame");
      fd->hfilp_enable[!! fd->switch_bank] = true_t;
    } else {
      fd->hfilp_enable[!! fd->switch_bank] = false_t;
    }

    /***** filp - vert *****/
    if ( vfilp != false_t) {
      vswap_oam (fd->fm_cody_oam, & fd->fm_cody_oam_cache[0], pivot_y, & fd->locpose_oc_y, & fd->fm_cody_oam_crash[0]);
      fd->vfilp_enable[!! fd->switch_bank] = true_t;
    } else {
      fd->vfilp_enable[!! fd->switch_bank] = false_t;
    }

    {
      uint8 ex, ey;
      uint8 posy = fd->locposo_y - fd->fm_cody_oam->yline *8;

      for (ey = fd->fm_cody_oam->yline - 1; ey != 0xFF; ey--) {
        for (ex = 0; ex != fd->fm_cody_oam->xline; ex++) {
          uint8 ii = ey *fd->fm_cody_oam->xline + ex;
            /*** reloc pos  ***/
            fd->fm_cody_oam_cache[ii].x_coord_common+= 
              hfilp ? -fd->fm_cody_oam->hoff : fd->fm_cody_oam->hoff;
            fd->fm_cody_oam_cache[ii].y_coord_common+=
              vfilp ? -fd->fm_cody_oam->voff : fd->fm_cody_oam->voff;
        }
      }

    }

    /***** COPY TO  source **/
    {
      uint8 ii;
      for  (ii =0;  ii != FBCC_OAM_LIMIT; ii ++) {
        memcpy (& fd->ppu->oam [fd->oamStartupAddress+ii], 
          & fd->fm_cody_oam_cache[ii], sizeof (struct sp_chunk));
      }
    }

  } else {
    /* clear oam **/
    uint8 cci;

    /**** dis all ***/
    for (cci = 0; cci != FBCC_OAM_LIMIT; cci++) {
      fd->fm_cody_oam_cache[cci].mask_common |=TES_CLOSE;
      fd->fm_cody_oam_cache[cci].mask[0] |= TES_CLOSE;
    }
  }
}