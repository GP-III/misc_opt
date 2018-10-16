/* PSG Sound Synthesizer
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

void apu_write (struct apu *apu, ks_uint16 addr, ks_uint8 value) {
}

ks_uint8 apu_read (struct apu *apu, ks_uint16 addr) {
  return 0;
}

static 
void apu_update (struct apu *apu) {
}

int apu_init (struct apu **apu) {
  struct apu *apu_ =ks_null;
  assert (apu != ks_null);

  apu_ = (struct apu *)
     calloc (sizeof (struct apu), 1);
  apu_->clks = apu_update;
  assert (apu_ != ks_null);
  * apu = apu_;
  return 0;
}

void apu_uninit (struct apu **apu) {
  struct apu *apu_;
  assert (apu != ks_null);
  apu_ = *apu;
  *apu = ks_null;
  if (apu_ != ks_null)
    free (apu_);
  else ;
}