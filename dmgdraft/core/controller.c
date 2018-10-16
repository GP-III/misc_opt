/* controller (PL0-PL5, with DAN215)
 * Game boy's joypad information read and write
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

/* for internal call */
#define joypad_hostdrv ubdata_user

/* Assert IRQ_5 Interrupt Gen MACRO */
#ifndef IRQ_5
# define IRQ_5 0x10
#endif 

#ifndef JOYPAD_INTERRUPT_GEN_ENABLE
# undef IRQ_5
# define IRQ_5 0
#endif 

static 
void default_update (struct controller *ctl, void *obj, struct controller_pad *gb_infos, struct controller_pad *host_infos) {
  printf ("%s:%s please set joypad callback( controller_setupdate function)\n", __FILE__, __LINE__);
  assert (0);
}

void controller_write (struct controller *ctl, ks_uint8 value) {
  ctl->reg00_P1 = value & 0x30;
}

/* Gameboy Joypad Infos:http://gbdev.gg8.se/files/docs/mirrors/pandocs.html#joypadinput */
ks_uint8 controller_read (struct controller *ctl) {
  ks_uint8 value =ctl->reg00_P1 & 0x30;
  switch (value) {
  case 0x00: /* maybe result direction | action ??*/
    value = ctl->reg00_P1 & 0xF0;
    value|= 0x0F;
#if (JOYPAD_LOAD_ACTION_IN_P1415_ALLOPEN == 0)
    if (ctl->gb_pad.right || ctl->gb_pad.a) value &= ~0x01; /*Right ||A*/
    if (ctl->gb_pad.left || ctl->gb_pad.b) value &= ~0x02; /*Left ||B */
    if (ctl->gb_pad.up || ctl->gb_pad.select) value &= ~0x04; /*Up ||Select */
    if (ctl->gb_pad.down || ctl->gb_pad.start) value &= ~0x08; /*Down || Start*/
#elif (JOYPAD_LOAD_ACTION_IN_P1415_ALLOPEN == 1)
    if (ctl->gb_pad.right && ctl->gb_pad.a) value &= ~0x01; /*Right ||A*/
    if (ctl->gb_pad.left && ctl->gb_pad.b) value &= ~0x02; /*Left ||B */
    if (ctl->gb_pad.up && ctl->gb_pad.select) value &= ~0x04; /*Up ||Select */
    if (ctl->gb_pad.down && ctl->gb_pad.start) value &= ~0x08; /*Down || Start*/
#elif (JOYPAD_LOAD_ACTION_IN_P1415_ALLOPEN == 2)
    if ((!!ctl->gb_pad.right) ^ !!ctl->gb_pad.a) value &= ~0x01; /*Right ||A*/
    if ((!!ctl->gb_pad.left) ^ !!ctl->gb_pad.b) value &= ~0x02; /*Left ||B */
    if ((!!ctl->gb_pad.up) ^ !!ctl->gb_pad.select) value &= ~0x04; /*Up ||Select */
    if ((!!ctl->gb_pad.down) ^ !!ctl->gb_pad.start) value &= ~0x08; /*Down || Start*/
#elif (JOYPAD_LOAD_ACTION_IN_P1415_ALLOPEN == 3)
    if (!(ctl->gb_pad.right && ctl->gb_pad.a)) value &= ~0x01; /*Right ||A*/
    if (!(ctl->gb_pad.left && ctl->gb_pad.b)) value &= ~0x02; /*Left ||B */
    if (!(ctl->gb_pad.up && ctl->gb_pad.select)) value &= ~0x04; /*Up ||Select */
    if (!(ctl->gb_pad.down && ctl->gb_pad.start)) value &= ~0x08; /*Down || Start*/
#else 
# error "JOYPAD_LOAD_ACTION_IN_P1415_ALLOPEN define error."
#endif 
    break;
  case 0x30: /* reset device ?*/
    return 0xFF;
  case 0x20: /* p14 out, set button, active low*/
    value = ctl->reg00_P1 & 0xF0;
    value|= 0x0F;
    if (ctl->gb_pad.right) value &= ~0x01; /*Right */
    if (ctl->gb_pad.left) value &= ~0x02; /*Left */
    if (ctl->gb_pad.up) value &= ~0x04; /*Up */
    if (ctl->gb_pad.down) value &= ~0x08; /*Down */
    break;
  case 0x10: /* p15 out, set direction, active low */
    value = ctl->reg00_P1 & 0xF0;
    value|= 0x0F;
    if (ctl->gb_pad.a) value &= ~0x01; /*A */
    if (ctl->gb_pad.b) value &= ~0x02; /*B */
    if (ctl->gb_pad.select) value &= ~0x04; /*Select */
    if (ctl->gb_pad.start) value &= ~0x08; /*Start */
    break;
  }
  return value;
}

void controller_uninit (struct controller **ctl) {
  struct controller *ctl_;
  assert (ctl != ks_null);
  ctl_ = *ctl;
  *ctl = ks_null;
  if (ctl_ != ks_null)
    free (ctl_);
  else ;
}

void controller_setupdate_ (struct controller *ctl, void (*update) 
             (struct controller *, 
             void *, 
                struct controller_pad *, /* self */ 
             struct controller_pad * /* host edge */), void *obj){
  assert (ctl != ks_null);
  assert (update != ks_null);
  ctl->joypad_hostdrv = update;
  ctl->obj = obj;
}

static
void controller_update_hostdrv (struct controller *ctl) { 
  
  if (ctl->gb->cpu_clks_joypad > ctl->gb->mach_tools->joypad_gen_freq) {
    ks_bool edge_gen =ks_false;
    /* temrp cache for check edge (high to low*/
    struct controller_pad edge;
    assert (ctl != ks_null);
    memset (& edge, 0, sizeof (edge));
    /*call hostdrv*/
    ((void (*)(struct controller *, void *, 
                  struct controller_pad *,
         struct controller_pad *))(ctl->joypad_hostdrv)) (ctl, ctl->obj, & ctl->gb_pad, & edge);
    ctl->gb->cpu_clks_joypad -= ctl->gb->mach_tools->joypad_gen_freq; /* sub a clk block */
    /* there is a bouncing phenomenon similar to electrical components flutter in real gameboy,
       making joypad interrupt very difficult to use. */
    /* check register */
    switch (ctl->reg00_P1 & 0x30) {
    case 0x30: /* close device*/
      return ;
    case 0x00: /* any button press will cause interrupt requested, resume stop command */
      if (edge.right || edge.a) edge_gen = ks_true; /*Right ||A*/
      if (edge.left || edge.b) edge_gen = ks_true; /*Left ||B */
      if (edge.up || edge.select) edge_gen = ks_true; /*Up ||Select */
      if (edge.down || edge.start) edge_gen = ks_true; /*Down || Start*/ 
    case 0x20: /* p14 out, button, */
      if (edge.right) edge_gen = ks_true; /*Right */
      if (edge.left) edge_gen = ks_true; /*Left */
      if (edge.up) edge_gen = ks_true; /*Up */
      if (edge.down) edge_gen = ks_true; /*Down */
      break;
    case 0x10: /* p15 out, direction */
      if (edge.a) edge_gen = ks_true; /*A */
      if (edge.b) edge_gen = ks_true; /*B */
      if (edge.select) edge_gen = ks_true; /*Select */
      if (edge.start) edge_gen = ks_true; /*Start */
      break;
    }
    if (edge_gen != ks_false) {
      /* Set interrupt flags */
  #   if 1
      /* try resume gameboy from stop,  (if stop) */
      if (ctl->gb->lr35902->stop != ks_false) {
        ctl->gb->lr35902->stop = ks_false;
        ctl->gb->reg0F_IF |= IRQ_5;
      }
  #   else 
      /* IRQ_5 is the lowest priority interruption.*/
      ctl->gb->reg0F_IF |= IRQ_5;
      /* try resume gameboy from stop,  (if stop) */
      gameboy_resume_ifstop (ctl->gb);
  #   endif 
    }
  }
}

int controller_init (struct controller **ctl) {
  struct controller *ctl_ =ks_null;
  assert (ctl != ks_null);

  ctl_ = (struct controller *)
     calloc (sizeof (struct controller), 1);
  ctl_->joypad_hostdrv = default_update;
  ctl_->obj = ks_null;
  ctl_->clks = controller_update_hostdrv;
  assert (ctl_ != ks_null);
  // memset (& ctl_->gb_pad, 0x01, sizeof (ctl_->gb_pad));
  * ctl = ctl_;
  return 0;
}