#include "frame.hedgehog.h"

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
#define IDP 3
struct frame_base_t fbeh_walk0[4][2] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN} }
};

#undef  IX
#define IX 2
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 3
struct frame_base_t fbeh_walk1[4][2] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN} }
};

#undef  IX
#define IX 4
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 3
struct frame_base_t fbeh_walk2[5][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, E_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, E_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, E_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN} },
{ {IDP, 0+PL4, E_EN}, {IDP, 1+PL4, E_EN}, {IDP, 2+PL4, E_EN} }
}; 

#undef  IX
#define IX 7
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 3
struct frame_base_t fbeh_walk3[5][2] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN} },
{ {IDP, 0+PL4, E_EN}, {IDP, 1+PL4, E_EN} }
};

#undef  IX
#define IX 9
#undef  IY
#define IY 0 
#undef  IDP
#define IDP 3
struct frame_base_t fbeh_walk4[4][2] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN} }
};

// <<WALK>>=========================================================================================================
struct frame_desc_t fbeh_walk[5] = {
{4, 2, 0, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fbeh_walk0[0][0]}, 
{4, 2, 4, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fbeh_walk1[0][0]}, 
{5, 3, 3, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fbeh_walk2[0][0]}, 
{5, 2, 3, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fbeh_walk3[0][0]}, 
{4, 2, 3, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fbeh_walk4[0][0]} 
};

#undef  IX
#define IX 0
#undef  IY
#define IY 4
#undef  IDP
#define IDP 3
struct frame_base_t fbeh_punch0[4][4] = {
{ {IDP, 0+PL0, E_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN}, {IDP, 3+PL0, E_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN}, {IDP, 3+PL1, E_EN} },
{ {IDP, 0+PL2, E_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN}, {IDP, 3+PL2, E_EN} },
{ {IDP, 0+PL3, E_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN}, {IDP, 3+PL3, E_EN} }
};

// <<PUNCH>>=========================================================================================================
struct frame_desc_t fbeh_punch[1] = {
{4, 4, 2, 3, 1, PIVOT_LINE,   0, PIVOT_UNSET, & fbeh_punch0[0][0]}
};

#undef  IX
#define IX 0
#undef  IY
#define IY 8
#undef  IDP 
#define IDP 3
struct frame_base_t fm_cody_beattack0[4][3] = {
{ {IDP, 0+PL0, C_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, I_EN} },
{ {IDP, 0+PL2, E_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN} },
{ {IDP, 0+PL3, I_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN} }
};

#undef  IX
#define IX 3
#undef  IY
#define IY 9
#undef  IDP 
#define IDP 3
struct frame_base_t fm_cody_beattack1[3][3] = {
{ {IDP, 0+PL0, E_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, E_EN} }
};

#undef  IX
#define IX 6
#undef  IY
#define IY 8
#undef  IDP 
#define IDP 3
struct frame_base_t fm_cody_beattack2[4][3] = {
{ {IDP, 0+PL0, E_EN}, {IDP, 1+PL0, E_EN}, {IDP, 2+PL0, I_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, E_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, C_EN} },
{ {IDP, 0+PL3, E_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, C_EN} }
};

// <<BEATTACKED>>=========================================================================================================
struct frame_desc_t fbeh_beattack[3] = {
{4, 3, 0, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fm_cody_beattack0[0][0]}, 
{3, 3, 4, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fm_cody_beattack1[0][0]}, 
{4, 3, 3, 0, 1, PIVOT_LINE, 0, PIVOT_UNSET, & fm_cody_beattack2[0][0]}
};


#undef  IX
#define IX 3
#undef  IY
#define IY 7
#undef  IDP 
#define IDP 0
struct frame_base_t fm_cody_beattack0c[4][4] = {
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
struct frame_base_t fm_cody_p55unch_combine4[4][3] = {
{ {IDP, 0+PL0, I_EN}, {IDP, 1+PL0, C_EN}, {IDP, 2+PL0, C_EN} },
{ {IDP, 0+PL1, C_EN}, {IDP, 1+PL1, C_EN}, {IDP, 2+PL1, C_EN} },
{ {IDP, 0+PL2, C_EN}, {IDP, 1+PL2, C_EN}, {IDP, 2+PL2, E_EN} },
{ {IDP, 0+PL3, C_EN}, {IDP, 1+PL3, C_EN}, {IDP, 2+PL3, E_EN} }
};