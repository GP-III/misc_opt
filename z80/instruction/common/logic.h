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

#ifndef __COMMON_LOGIC_H_INCLUED_
#define __COMMON_LOGIC_H_INCLUED_

/* 	AND

	target register <- A 
	source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)/imm8
	cycles			   4 4 4 4 4 4   7  4   8   8   19    8   8   19     7
	opcode    	   y:A 0 1 2 3 4 5   6  7   4   5    6    4   5    6 -- E6
	
	details: A &= src 
			 F = h_flag
			 F |= pzs_table[A]
			 
	S 		affected as defined
	Z 		affected as defined
	H 		set
	P/V 	detects parity
	N 		reset
	C 		reset	*/
	
static 
void and_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xE6) 
		cpu->NP = u16 ( fetch_8bits_immediate_number (cpu) );
	else if (cpu->sel_field != 6) 
		cpu->NP = u16 ( cpu->regs_mapb8[cpu->sel_field][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	cpu->F = H_FLAG;
	cpu->A&= cpu->NL;
	cpu->F|= pzs_table[cpu->A];
}

/* 	XOR

	target register <- A 
	source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)/imm8
	cycles			   4 4 4 4 4 4   7  4   8   8   19    8   8   19     7
	opcode    	   y:A 8 9 A B C D   E  F   C   D    E    C   D    E -- EE
	
	details: A ^= src 
			 F  = pzs_table[A]
			 
	S 		affected as defined
	Z 		affected as defined
	H 		reset
	P/V 	detects parity
	N 		reset
	C 		reset	*/
	
static 
void xor_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xEE) 
		cpu->NP = u16 ( fetch_8bits_immediate_number (cpu) );
	else if (cpu->sel_field != 6) 
		cpu->NP = u16 ( cpu->regs_mapb8[cpu->sel_field][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	cpu->A^= cpu->NL;
	cpu->F = pzs_table[cpu->A];
}

/* 	OR(A)

	target register <- A 
	source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)/imm8
	cycles			   4 4 4 4 4 4   7  4   8   8   19    8   8   19     7
	opcode    	   y:B 0 1 2 3 4 5   6  7   4   5    6    4   5    6 -- F6
	
	details: A |= src 
			 F  = pzs_table[A]
			 
	S 		affected as defined
	Z 		affected as defined
	H 		reset
	P/V 	detects parity
	N 		reset
	C 		reset	*/
	
static 
void ora_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xF6) 
		cpu->NP = u16 ( fetch_8bits_immediate_number (cpu) );
	else if (cpu->sel_field != 6) 
		cpu->NP = u16 ( cpu->regs_mapb8[cpu->sel_field][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	cpu->A|= cpu->NL;
	cpu->F = pzs_table[cpu->A];
}

/* 	CMP

	target register <- A 
	source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)/imm8
	cycles			   4 4 4 4 4 4   7  4   8   8   19    8   8   19     7
	opcode    	   y:B 8 9 A B C D   E  F   C   D    E    C   D    E -- FE
	
	details: same as sub, but no save result */
	
static 
void cmp_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xFE) 
		cpu->NP = u16 ( fetch_8bits_immediate_number (cpu) );
	else if (cpu->sel_field != 6) 
		cpu->NP = u16 ( cpu->regs_mapb8[cpu->sel_field][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	cpu->MW0= u16 (cpu->A) - cpu->NP;
	cpu->F  = N_FLAG; 
	cpu->F |=-cpu->MB1;
	cpu->F |= zs_table[cpu->MB0];
	cpu->F |= ((cpu->NL ^ cpu->A ^ cpu->MB0) & 0x10) ? H_FLAG : 0;	
	cpu->F |= (((cpu->A ^ cpu->MB0) & (cpu->NL ^ cpu->MB0)) & 0x80) ? V_FLAG : 0;
}

#endif