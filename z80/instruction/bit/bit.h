/*
** Z80 CPU Emu 
** Copyright (C) 2015 moecmks 
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

#ifndef __BIT_BIT_H_INCLUED_
#define __BIT_BIT_H_INCLUED_

static 
void bit_i (struct z80cpu *cpu) {
	
	cpu->F &= 1;
	cpu->F |= H_FLAG;
	
	if (cpu->regs_mapbfp[6] == common_link_trap && cpu->sel_field != 6) {
		
		cpu->IP = u16 (cpu->regs_mapb8[cpu->sel_field])
	} else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->IP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	if (! ( cpu->IL & ( 1 << calc_who_unknown (4) ) ) ) {
		cpu->F |= Z_FLAG;
	}
}	

#endif