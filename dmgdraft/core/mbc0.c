/* MAPPER0 - Standard 32K ROM.
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

#include "internal.h"

static 
void clks (struct cartridge *cartridge) {
  /* nodone in device mapper0*/
}

static 
void  write (struct cartridge *cartridge, ks_uint16 address, ks_uint8 value) {
  switch (address >> 13)  {
  case 5: /* 0xA000-0xBFFF - SRAM or ext device */
    if (cartridge->sramworks != ks_null 
     && cartridge->sramsize !=0)
     cartridge->sramworks[cartridge->sramsize*0x2000+address-0xA000] = value;
    else 
  case 0: /* 0x0000-0x1FFF */
  case 1: /* 0x2000-0x3FFF  Bank0 - 16KB ROM bank 00 */
  case 2: /* 0x4000-0x5FFF */
  case 3: /* 0x6000-0x7FFF Bank N-  */
  default:
    cartridge->gb->unknow_ram[address] = value;
    break;
  }
}

static 
ks_uint8 read (struct cartridge *cartridge, ks_uint16 address) {
  switch (address >> 13)  {
  case 0: /* 0x0000-0x1FFF */
  case 1: /* 0x2000-0x3FFF  Bank0 - 16KB ROM bank 00 */
  case 2: /* 0x4000-0x5FFF */
  case 3: /* 0x6000-0x7FFF Bank N-  */
    return cartridge->promworks[address];
  case 5: /* 0xA000-0xBFFF - SRAM or ext device */
    if (cartridge->sramworks != ks_null 
     && cartridge->sramsize !=0)
     return cartridge->sramworks[cartridge->sramsize*0x2000+address-0xA000];
  default: 
    return cartridge->gb->unknow_ram[address];
  }
}

void cartridge_mbc0_uninit (struct cartridge *cartridge) {
  /* nodone */
}

int cartridge_mbc0_init (struct cartridge *cartridge) {
  cartridge->read = read;
  cartridge->write = write;
  cartridge->clks = clks;

  return 0;
}