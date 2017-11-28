/*
** Z80 CPU Emu 
** Copyright (C) 2016 moecmks 
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#ifndef __COMMON_EXCHANGE_H_INCLUED_
#define __COMMON_EXCHANGE_H_INCLUED_

#define swap_(x, y) do 	\
 {						\
	uint16_t _spin = x; \
	x = y;				\
	y = _spin;			\
 } while (0)

/*.*. EXX - swap BC/DE/HL and their cache 4 cycles, no affect any flag OP:D9 .*.*/
static 
void exx_ (struct z80cpu *cpu) {	
	swap_ (cpu->BC, cpu->BC_Cache);
	swap_ (cpu->DE, cpu->DE_Cache);
	swap_ (cpu->HL, cpu->HL_Cache);
}
/*.*. EX_AF_AF' - swap AF and it's cache 4 cycles, no affect any flag OP:08 .*.*/
static 
void ex_af_cache (struct z80cpu *cpu) {	
	swap_ (cpu->AF, cpu->AF_Cache);
}
/*.*. EX_DE_HL - swap DE and HL 4 cycles, no affect any flag OP:EB .*.*/
static 
void ex_de_hl (struct z80cpu *cpu) {	
	swap_ (cpu->DE, cpu->HL);
}
/*.*. EX_xSP_trap_register - swap (sp)-word and trap-reg, common 19, index 23 cycles.no affect any flag OP:E3 .*.*/
static 
void ex_trap_mem (struct z80cpu *cpu) {
	
	cpu->NL = cpu->z80_data_read (cpu, cpu->SP++);
	cpu->NH = cpu->z80_data_read (cpu, cpu->SP+0);
	cpu->IP = cpu->regs_mapw16[2][0];
	cpu->z80_data_write (cpu, cpu->SP--, cpu->IH);
	cpu->z80_data_write (cpu, cpu->SP-0, cpu->IL);
	cpu->regs_mapw16[2][0] = cpu->NP;
}
#undef swap_
#endif