/* timer about TIMA, TMA, TAC
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

void timer_write (struct timer *timer, ks_uint16 addr, ks_uint8 value) {

  switch (addr) {
  case 0xFF05: /* TIMA */
    timer->reg05_TIMA = value;
    break;
  case 0xFF06: /* TMA */
    timer->reg06_TMA = value;
    break;
  case 0xFF07: /* TAC */
    if ( (value & 0x04) && !(timer->reg07_TAC & 0x04)) {
      timer->timestamp = timer->gb->cpu_clks_timer; /* low to high, open pulse, init timestamp. */
      timer->reg05_TIMA = timer->reg06_TMA;
    }
    timer->reg07_TAC = value;
    break;
  default:
    assert(0);
  }
#ifdef TIMER_RESET_IN_RESET_FREQ_MOD_
  timer->timestamp = timer->gb->cpu_clks_timer;
#endif 
}

ks_uint8 timer_read (struct timer *timer, ks_uint16 addr) {

  switch (addr) {
  case 0xFF05: /* TIMA */
    return timer->reg05_TIMA;
  case 0xFF06: /* TMA */
    return timer->reg06_TMA;
  case 0xFF07: /* TAC */
    return timer->reg07_TAC;
  default:
    assert(0);
  }
  return 0;
}

/*XXX: this function looks bad. */
static 
void timer_update (struct timer *timer) {
  /* !must update gameboy's context about timer before this call */
  /*   double cpu_clks_timer;
  double cpu_clks_el */
  ks_double elap;
  ks_int32 coub;
  ks_double calc_cac;
  assert (timer != ks_null);
  /* enable timing???*/
  if ( !(timer->reg07_TAC & 0x04))
     return  ;
  else ;
  /* get elapsed cpu time */
  elap = timer->gb->cpu_clks_timer - timer->timestamp;
  /* convert to basic frequency of timer */
  calc_cac = timer->freq_tab[timer->reg07_TAC & 3];
  coub = 0;
  while (elap > calc_cac) {
    /* update timer */
    if (timer->reg05_TIMA == 0xFF) {
      /* overflow, IRQ_3 interrupt will requested */
      timer->reg05_TIMA = timer->reg06_TMA;
      timer->gb->reg0F_IF |= IRQ_3;   
    } else {
      timer->reg05_TIMA ++;
    }
    elap -= calc_cac;
    coub = 1;
  }
  /* update timestamp */
  if (coub != 0) {
    timer->gb->cpu_clks_timer = elap;
    timer->timestamp = 0.0;
  }
}



void timer_cgb_mode (struct timer *timer) {
#ifdef TIMER_DIVIDER_SAME_FREQ_BOTH_CBG_DMG_BASE_DMG
  timer->freq_tab[0] = 2047.998;
  timer->freq_tab[1] = 31.998;
  timer->freq_tab[2] = 127.998;
  timer->freq_tab[3] = 511.998;
#else 
  timer->freq_tab[0] = 1023.998;
  timer->freq_tab[1] = 15.998;
  timer->freq_tab[2] = 63.998;
  timer->freq_tab[3] = 255.998;
#endif 
}

void timer_dmg_mode (struct timer *timer) {
  timer->freq_tab[0] = 1023.998;
  timer->freq_tab[1] = 15.998;
  timer->freq_tab[2] = 63.998;
  timer->freq_tab[3] = 255.998;
}

void timer_uninit (struct timer **timer) {
  struct timer *timer_;
  assert (timer != ks_null);
  timer_ = *timer;
  *timer = ks_null;
  if (timer_ != ks_null)
    free (timer_);
  else ;
}

int timer_init (struct timer **timer) {
  struct timer *timer_ =ks_null;
  assert (timer != ks_null);

  timer_ = (struct timer *)
     calloc (sizeof (struct timer), 1);
  timer_->clks = timer_update;
  assert (timer_ != ks_null);
  * timer = timer_;
  timer_dmg_mode (*timer);
  return 0;
}