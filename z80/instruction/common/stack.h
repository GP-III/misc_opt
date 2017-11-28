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

#ifndef __COMMON_STACK_H_INCLUED_
#define __COMMON_STACK_H_INCLUED_

/* 	PUSH: - word register on top of stack

	source register <- BC/DE/HL/AF/IX/IY 
	cycles 			   11 11 11 11 15 15
	opcode 			   C5 D5 E5 F5 E5 E5	
	
	details:reg-hi = byte ptr[--sp]
			reg-lo = byte ptr[--sp]
	
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		unaffected
	C 		unaffected	
	H  		unaffected */

static 
void push_w (struct z80cpu *cpu) {
	
	cpu->z80_data_write (cpu, --cpu->SP, cpu->regs_mapw8_ext[calc_who_unknown (12)][1]);
	cpu->z80_data_write (cpu, --cpu->SP, cpu->regs_mapw8_ext[calc_who_unknown (12)][0]);	
}

/* 	POP: - Pop top two bytes to register 

	target register <- BC/DE/HL/AF/IX/IY 
	cycles 			   10 10 10 10 14 14
	opcode 			   C1 D1 E1 F1 E1 E1
	
	details:reg-lo = byte ptr[sp++]
			reg-hi = byte ptr[sp++]
	
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		unaffected
	C 		unaffected	
	H  		unaffected */
	
static 
void pop_w (struct z80cpu *cpu) {
	
	cpu->regs_mapw8_ext[calc_who_unknown (12)][0] = cpu->z80_data_read (cpu, cpu->SP++);
	cpu->regs_mapw8_ext[calc_who_unknown (12)][1] = cpu->z80_data_read (cpu, cpu->SP++);
}

#endif