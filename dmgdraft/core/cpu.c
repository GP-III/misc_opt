/* CPU - LR35902
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

int cpu_init (struct cpu **cpu) {
  struct cpu *cpu_ =ks_null;
  assert (cpu != ks_null);

  cpu_ = (struct cpu *)
     calloc (sizeof (struct cpu), 1);
  assert (cpu_ != ks_null);
  * cpu = cpu_;
  return 0;
}

void cpu_uninit (struct cpu **cpu) {
  struct cpu *cpu_;
  assert (cpu != ks_null);
  cpu_ = *cpu;
  *cpu = ks_null;
  if (cpu_ != ks_null)
    free (cpu_);
  else ;
}

void cpu_reset (struct cpu *cpu) {
  /*   AF=$01B0
  BC=$0013
  DE=$00D8
  HL=$014D
  Stack Pointer=$FFFE
  
  See. http://gbdev.gg8.se/wiki/articles/Power_Up_Sequence
  */

  cpu->AF = 0x01B0; /* XXX: F not stand bitfield. */
  cpu->BC = 0x0013;
  cpu->HL = 0x014D;
  cpu->SP = 0xFFFE;
  /* When the GameBoy is powered up, 
    a 256 byte program starting at memory location 0 is executed. */
  cpu->PC = 0x0100; 
  cpu->IME = 0;
  cpu->halt = ks_false;
  cpu->stop = ks_false;
  cpu->_backup = 0;
}