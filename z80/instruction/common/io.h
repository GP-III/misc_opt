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

#ifndef __COMMON_IO_H_INCLUED_
#define __COMMON_IO_H_INCLUED_

// =================================================================
// port operate. in fact, only the first 256 ports can be used	
// well, I'm going to ignore the high address index of the register.
// =================================================================
	
// write register A to imm8 port's index. opcode:D3. 11 cycles
static 
void out_ai8 (z80cpu *cpu)	{
	
	cpu->IP = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
	cpu->z80_port_write (cpu, cpu->IL, cpu->A);
}
// read imm8 port's index to register A. opcode:DB. 11 cycles
static 
void in_ai8 (z80cpu *cpu)	{
	
	cpu->IP = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
	cpu->A = cpu->z80_port_read (cpu, cpu->IL);
}
#endif