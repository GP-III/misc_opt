#include "frame.h"
#include "type.h"

#if defined (_DEBUG)
void dbg_output_frameattr_ (void *frame_desc_t, frame_attr *crash_oam2, const char *misc_comment) {
#ifndef DISABLE_OUTPUT_DEBUG_ATTR
    struct frame_desc_t *pfdt = frame_desc_t;
    uint8 iy, ix;

    for (iy =0; iy !=pfdt->yline; iy ++) {
      for (ix =0; ix != pfdt->xline; ix++) {
        if (ix ==0) {
          if (iy ==0) {
            DEBUG_OUTPUT ("FT_INFOS_BEGIN-");
            if (misc_comment != null) {
              DEBUG_OUTPUT (misc_comment);
            }
            DEBUG_OUTPUT ("@@!\n");
          }
          DEBUG_OUTPUT ("FT_INFOS-");
        }
        switch (crash_oam2[ix+iy*pfdt->xline]) {
        case I_EN:DEBUG_OUTPUT ("I "); break;
        case E_EN:DEBUG_OUTPUT ("E "); break;
        case A_EN:DEBUG_OUTPUT ("A "); break;
        case C_EN:DEBUG_OUTPUT ("C ");
        default: break;
        }
        if (ix == (pfdt->xline -1)) {
          DEBUG_OUTPUT ("\n");
          if (iy == (pfdt->yline -1)) {
            DEBUG_OUTPUT ("FT_INFOS_END-");
            if (misc_comment != null) {
              DEBUG_OUTPUT (misc_comment);
            }
            DEBUG_OUTPUT ("@@!\n");
          }
        }
      }
    }
#endif
}

#endif 

void vswap_oam (void *frame_desc_t, struct sp_chunk *spc,
  uint8 pivot_y, uint8 *adj_y, frame_attr *crash_oam2) {

    struct frame_desc_t *pfdt = frame_desc_t;
    uint8 ei, ei_, pty = pivot_y - pfdt->yline *8;
    uint8 idtx = pfdt->xline, idty = pfdt->yline;
    int8 pivot = pfdt->vpivot;

    for (ei =0; ei !=pfdt->yline *pfdt->xline; ei ++) {
      uint8 ey = ei /pfdt->xline;
      uint8 ex = ei %pfdt->xline;

      spc[ei].mask_common |= TES_VFLIP;
      spc[ei].mask[0] |= TES_VFLIP;

      /* swap crash **/ 
      if ( ey <= pfdt->yline/ 2) {
        frame_attr fa_temp = crash_oam2[ei];
        crash_oam2[ei] = crash_oam2[(pfdt->yline-ey-1) *pfdt->xline+ex];
        crash_oam2[(pfdt->yline-ey-1) *pfdt->xline+ex] = fa_temp;
      }
    }

    if (pfdt->vpivot_type == PIVOT_LINE) {
      /*** swap coord ***/
      for (ei =0; ei != pfdt->yline; ei++) {
        for (ei_ =0; ei_ != pfdt->xline; ei_++) {
          /*** <ALL> pivot -> bottom**/
          if ((pivot >= ((signed)pfdt->yline) )) {
            spc[ei*pfdt->xline+pfdt->xline-ei_-1].y_coord_common = pfdt->yline * 8+pty + ( pivot - (pfdt->yline-ei)) * 8;
          } else if ((pivot <= ((signed)0) )) {
            /** <ALL> pivot -> top **/
            spc[ei*pfdt->xline+ei_].y_coord_common = pty + ( pivot - ei - 1) * 8;
          } else if ( ei < pivot) {
            /*** pivot -> bottom**/
            spc[ei*pfdt->xline+ei_].y_coord_common = pty + pivot * 8 + (pivot - ei - 1) * 8;
          } else {
            /** pivot -> top **/
            spc[ei*pfdt->xline+ei_].y_coord_common = pty + pivot * 8 - (ei- pivot +1) * 8;
          }
        }
      }
    } else if (pfdt->vpivot_type == PIVOT_VECTOR) {
      /*** swap coord ***/
      for (ei =0; ei != pfdt->yline; ei++) {
        for (ei_ =0; ei_ != pfdt->xline; ei_++) {
          /*** <ALL> pivot -> bottom**/
          if ((pivot >= ((signed)pfdt->xline) )) {
            spc[ei*pfdt->xline+pfdt->xline-ei_-1].y_coord_common = pfdt->yline * 8+pty + ( pivot - (pfdt->yline-ei)) * 8 + 8;
          } else if ((pivot < ((signed)0) )) {
            /** <ALL> pivot -> top **/
            spc[ei*pfdt->xline+ei_].y_coord_common = pty + ( pivot - ei - 1) * 8;
          } else if ( ei < pivot) {
            /*** pivot -> bottom**/
            spc[ei*pfdt->xline+ei_].y_coord_common = pty + pivot * 8 + (pivot - ei - 1) * 8 + 8;
          } else if ( ei > pivot) {
            /** pivot -> top **/
            spc[ei*pfdt->xline+ei_].y_coord_common = pty + pivot * 8 - (ei- pivot) * 8;
          }
        }
      }
    }
    /** relocus left-bottom coord ***/
    if (spc[0].y_coord_common > spc[(pfdt->yline-1)*pfdt->xline].y_coord_common) 
      * adj_y = spc[0].x_coord_common;
    else if (spc[0].y_coord_common > spc[(pfdt->yline-1)*pfdt->xline].y_coord_common)
      * adj_y = spc[(pfdt->yline-1)*pfdt->xline].y_coord_common;
}

void hswap_oam (void *frame_desc_t, struct sp_chunk *spc,
                uint8 pivot_x, uint8 *adj_x, frame_attr *crash_oam2, const char *debug_infos)
				{
					
          struct frame_desc_t *pfdt = frame_desc_t;
          uint8 ei, ei_, ptx = pivot_x;
          uint8 idtx = pfdt->xline, idty = pfdt->yline;
          int8 pivot = pfdt->hpivot;

          for (ei =0; ei !=pfdt->yline *pfdt->xline; ei ++) {
            uint8 ey = ei /pfdt->xline;
            uint8 ex = ei %pfdt->xline;

            spc[ei].mask_common |= TES_HFLIP;
            spc[ei].mask[0] |= TES_HFLIP;

            /* swap crash **/ 
            if ( ex <= pfdt->xline/ 2) {
              frame_attr fa_temp = crash_oam2[ei];
              crash_oam2[ei] = crash_oam2[pfdt->xline-ex-1+ey*pfdt->xline];
              crash_oam2[pfdt->xline-ex-1+ey*pfdt->xline] = fa_temp;
            }
          }

          /* output debug infos ***/
          dbg_output_frameattr (frame_desc_t, & crash_oam2[0], debug_infos);

          if (pfdt->hpivot_type == PIVOT_LINE) {
            /*** swap coord ***/
            for (ei =0; ei != pfdt->yline; ei++) {
              for (ei_ =0; ei_ != pfdt->xline; ei_++) {
                /*** <ALL> pivot -> right**/
                if ((pivot >= ((signed)pfdt->xline) )) {
                  spc[ei*pfdt->xline+pfdt->xline-ei_-1].x_coord_common = pfdt->xline * 8+ptx + ( pivot - (pfdt->xline-ei_)) * 8;
                } else if ((pivot <= ((signed)0) )) {
                  /** <ALL> pivot -> left **/
                  spc[ei*pfdt->xline+ei_].x_coord_common = ptx + ( pivot - ei_ - 1) * 8;
                } else if ( ei_ < pivot) {
                  /*** pivot -> right**/
                  spc[ei*pfdt->xline+ei_].x_coord_common = ptx + pivot * 8 + (pivot - ei_ - 1) * 8;
                } else {
                  /** pivot -> left **/
                  spc[ei*pfdt->xline+ei_].x_coord_common = ptx + pivot * 8 - (ei_- pivot +1) * 8;
                }
              }
            }
          } else if (pfdt->hpivot_type == PIVOT_VECTOR) {
            /*** swap coord ***/
            for (ei =0; ei != pfdt->yline; ei++) {
              for (ei_ =0; ei_ != pfdt->xline; ei_++) {
                /*** <ALL> pivot -> right**/
                if ((pivot >= ((signed)pfdt->xline) )) {
                  spc[ei*pfdt->xline+pfdt->xline-ei_-1].x_coord_common = pfdt->xline * 8+ptx + ( pivot - (pfdt->xline-ei_)) * 8 + 8;
                } else if ((pivot < ((signed)0) )) {
                  /** <ALL> pivot -> left **/
                  spc[ei*pfdt->xline+ei_].x_coord_common = ptx + ( pivot - ei_ - 1) * 8;
                } else if ( ei_ < pivot) {
                  /*** pivot -> right**/
                  spc[ei*pfdt->xline+ei_].x_coord_common = ptx + pivot * 8 + (pivot - ei_ - 1) * 8 + 8;
                } else if ( ei_ > pivot) {
                  /** pivot -> left **/
                  spc[ei*pfdt->xline+ei_].x_coord_common = ptx + pivot * 8 - (ei_- pivot) * 8;
                }
              }
            }
          }
          /** relocus left-bottom coord ***/
          if (spc[0].x_coord_common > spc[pfdt->xline-1].x_coord_common) 
            * adj_x = spc[pfdt->xline-1].x_coord_common;
          else if (spc[0].x_coord_common <= spc[pfdt->xline-1].x_coord_common)
            * adj_x = spc[0].x_coord_common;
				}