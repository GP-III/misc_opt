/* DIV - fixed frequency distributor
 * It's just 1/256 of the cpu frequency.
 *
 * Copyright (C) 2018 moecmks
 * This file is part of KS3578.
 * 
 * do What The Fuck you want to Public License
 * 
 * Version 1.0, March 2000
 * Copyright (C) 2000 Banlu Kemiyatorn (]d).
 * 136 Nives 7 Jangwattana 14 Laksi Bangkok
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 * 
 * Ok, the purpose of this license is simple
 * and you just
 * 
 * DO WHAT THE FUCK YOU WANT TO.
 */

#include "gameboy.h"
#include "internal.h"

void divider_write (struct divider *divider, ks_uint8 value) {

  divider->reg04_DIV = value;
}

ks_uint8 divider_read (struct divider *divider) {

  return divider->reg04_DIV;
}

void divider_cgb_mode (struct divider *divider) {
#ifdef TIMER_DIVIDER_SAME_FREQ_BOTH_CBG_DMG_BASE_DMG
  divider->freq = 512.0;
#else 
  divider->freq = 256.0;
#endif 
}

void divider_dmg_mode (struct divider *divider) {
  divider->freq = 256.0;
}

/*XXX: this function looks bad. */
static 
void divider_update (struct divider *divider) {
  /* !must update gameboy's context about divider before this call */
  /*   double cpu_clks_divider;
  double cpu_clks_el */
  ks_double elap;
  ks_int32 coub;
  assert (divider != ks_null);
  /* get elapsed cpu time */
  elap = divider->gb->cpu_clks_divider - divider->timestamp;
  coub = 0;
  while (elap > divider->freq) {
    /* update divider */
    divider->reg04_DIV++;
    elap -= divider->freq;
    coub = 1;
  }
  /* update timestamp */
  if (coub != 0) {
    divider->gb->cpu_clks_divider = elap;
    divider->timestamp = 0.0;
  }
}

int divider_init (struct divider **divider) {
  struct divider *divider_ =ks_null;
  assert (divider != ks_null);

  divider_ = (struct divider *)
     calloc (sizeof (struct divider), 1);
  divider_->clks = divider_update;
  assert (divider_ != ks_null);
  * divider = divider_;
  divider_dmg_mode (divider_);
  return 0;
}

void divider_uninit (struct divider **divider) {
  struct divider *divider_;
  assert (divider != ks_null);
  divider_ = *divider;
  *divider = ks_null;
  if (divider_ != ks_null)
    free (divider_);
  else ;
}