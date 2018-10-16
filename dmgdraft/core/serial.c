/* Serial device for gameboy
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

void serial_write (struct serial *serial, ks_uint16 addr, ks_uint8 value) {
}

ks_uint8 serial_read (struct serial *serial, ks_uint16 addr) {
  return 0;
}

static 
void serial_update (struct serial *serial) {
}

int serial_init (struct serial **serial) {
  struct serial *serial_ =ks_null;
  assert (serial != ks_null);

  serial_ = (struct serial *)
     calloc (sizeof (struct serial), 1);
  serial_->clks = serial_update;
  assert (serial_ != ks_null);
  * serial = serial_;
  return 0;
}

void serial_uninit (struct serial **serial) {
  struct serial *serial_;
  assert (serial != ks_null);
  serial_ = *serial;
  *serial = ks_null;
  if (serial_ != ks_null)
    free (serial_);
  else ;
}