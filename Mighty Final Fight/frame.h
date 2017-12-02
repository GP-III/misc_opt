/*
 * Copyright (C) 2017 moecmks, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You should have received a copy of the GNU General Public
 * License along with the tile-mr10; if not, see:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef __FRAME_INC_
#define __FRAME_INC_

#include "type.h"
#include "8bit_host.h"

#define A_EN 9 /** attack unit */
#define C_EN 3 /* join in crash */
#define E_EN 7 /* only draw */
#define I_EN 13  /* nodone */
typedef int frame_attr;

#define PIVOT_LINE 3 
#define PIVOT_VECTOR 7
#define PIVOT_UNSET 9
typedef int pivot_type;

struct frame_base_t {
  uint8 page; /* chr page */
  uint8 lend; /* index in chr page */
  frame_attr attr; /* current unit's attr [see C_EN/E_EN/I_EN]*/
};

struct frame_desc_t {
	
  uint8 yline; /** chunk of col */
  uint8 xline;  /* chunk of line */

  int8 hoff; /* hori offset [for adjust tile's coord]*/
  int8 voff; /* vert offset */
  
  int8 hpivot; 
  pivot_type hpivot_type;
  
  int8 vpivot; 
  pivot_type vpivot_type;
  
  struct frame_base_t *desc;
};

#if ! defined (HV_SWAP_MAYBE_NECESSARY)
/** base 
 *  crash_oam2 had fill unit .
 *	spc had fill base coord 
 *	adj_x, y fill base coord 
 */
void vswap_oam (void *frame_desc_t, struct sp_chunk *spc,
  uint8 pivot_y, uint8 *adj_y, frame_attr *crash_oam2);

void hswap_oam (void *frame_desc_t, struct sp_chunk *spc,
  uint8 pivot_x, uint8 *adj_x, frame_attr *crash_oam2, const char *debug_infos);
#if defined (_DEBUG)
  void dbg_output_frameattr_ (void *frame_desc_t, frame_attr *crash_oam2const, const char *misc_comment);

  #  define dbg_output_frameattr dbg_output_frameattr_
#else 
  #  define dbg_output_frameattr(...) ((void) 0)
#endif 
#endif 
#endif