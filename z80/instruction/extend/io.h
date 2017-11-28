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

#ifndef __EXTEND_IO_H_INCLUED_
#define __EXTEND_IO_H_INCLUED_

// =================================================================
// port operate. in fact, only the first 256 ports can be used	
// well, I'm going to ignore the high address index of the register.
// =================================================================
	
static 
void out_ (struct z80cpu *cpu)	{
	
	if ( !(cpu->y_field == 7 && !cpu->sel_field ) ) {
		cpu->z80_port_write (cpu, cpu->C, cpu->regs_mapb8[calc_who_unknown (4)][0]);
	} else {
		cpu->z80_port_write (cpu, cpu->C, 0);
	}
}

static 
void in_ (struct z80cpu *cpu)	{
	
	cpu->IL = cpu->z80_port_read (cpu, cpu->C);
	
	if ( !(cpu->y_field == 7 && !cpu->sel_field ) ) {
		cpu->regs_mapb8[calc_who_unknown (4)][0] = cpu->IL;
	} 
	
	cpu->F &= 1;
	cpu->F |= psz_table[cpu->IL];	
}

#endif