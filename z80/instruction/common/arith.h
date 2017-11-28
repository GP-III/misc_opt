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

#ifndef __COMMON_ARITH_H_INCLUED_
#define __COMMON_ARITH_H_INCLUED_

/* 	ADDB/ADCB

	target register <- A 
	source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)/imm8
	cycles			   4 4 4 4 4 4   7  4   8   8   19    8   8   19     7
	opcode add	   y:8 0 1 2 3 4 5   6  7   4   5    6    4   5    6 -- C6
	opcode adc     y:8 8 9 A B C D   E  F   C   D    E    C   D    E -- CE 
	
	details: M16= src + A + if ADCB ? c_flag : 0
			 F &= 0;
			 F |= M16 > 0xFF ? set c : clr c
			 F |= (src ^ A ^ M16-lobyte) & 0x10 ? set h : clr h;
			 F |= M16-lobyte & 0x80 ? set s : clr s 
			 F |= M16-lobyte ? clr z : set z
			 F |= (((~(A ^ M16-lobyte)) & (src ^ M16-lobyte)) & 0x80) ? V_FLAG : 0;
			 A = M16-lobyte
			 
	S 		affected as defined
	Z 		affected as defined
	H 		affected as defined	
	P/V 	detects overflow
	N 		reset
	C 		affected as defined	*/
	
static 
void add_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xC6) 
		cpu->NP = u16 ( fetch_8bits_immediate_number (cpu) );
	else if (cpu->sel_field != 6) 
		cpu->NP = u16 ( cpu->regs_mapb8[cpu->sel_field][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	cpu->MW0= cpu->NP + u16 (cpu->A);
	cpu->F &= 0; // reset all
	cpu->F |= cpu->MB1;
	cpu->F |= zs_table[cpu->MB0];
	cpu->F |= ((cpu->NL ^ cpu->A ^ cpu->MB0) & 0x10) ? H_FLAG : 0;	
	cpu->F |= (((~(cpu->A ^ cpu->MB0)) & (cpu->NL ^ cpu->MB0)) & 0x80) ? V_FLAG : 0;
	cpu->A = cpu->MB0;
}		
static 
void adc_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xCE) 
		cpu->NP = u16 ( fetch_8bits_immediate_number (cpu) );
	else if (cpu->sel_field != 6) 
		cpu->NP = u16 ( cpu->regs_mapb8[cpu->sel_field][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	cpu->MW0= cpu->NP + u16 (cpu->A) + u16 (cpu->F & C_FLAG);
	cpu->F &= 0; // reset all
	cpu->F |= cpu->MB1;
	cpu->F |= zs_table[cpu->MB0];
	cpu->F |= ((cpu->NL ^ cpu->A ^ cpu->MB0) & 0x10) ? H_FLAG : 0;	
	cpu->F |= (((~(cpu->A ^ cpu->MB0)) & (cpu->NL ^ cpu->MB0)) & 0x80) ? V_FLAG : 0;
	cpu->A = cpu->MB0;
}	
	
/* 	ADDW

	source register <- BC/DE/HL/SP
	opcode 			   09 19 29 39
	target regsiter <- HL/IX/IY	
	cycles			   11 15 15
	
	details: M32= src + des 
			 F &= ((Z_FLAG | S_FLAG | V_FLAG));
			 F |= M32 > 0xFFFF ? set c : clr c
			 F |= (src ^ des ^ M32-loword) & 0x1000 ? set h : clr h;
			 des = M32-loword
			 
	S 		unaffected
	Z 		unaffected
	H 		affected as defined	
	P/V 	unaffected
	N 		reset
	C 		affected as defined	*/
	
static 
void add_w (struct z80cpu *cpu) {
	
	cpu->NP = cpu->regs_mapw16[2][0];
	cpu->IP = cpu->regs_mapw16[calc_who_unknown (0)][0];
	cpu->MD0= u32 (cpu->NP) + u32 (cpu->IP);
	cpu->F &= 0xC4; // Z-S-V
	cpu->F |= cpu->MB2;
	cpu->F |= ((cpu->NP ^ cpu->IP ^ cpu->MW0) & 0x1000) ? H_FLAG : 0;	
	cpu->regs_mapw16[2][0] = cpu->MW0;
}	

/* 	SUBB/SBCB

	source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)/imm8
	cycles			   4 4 4 4 4 4   7  4   8   8   19    8   8   19     7
	opcode sub	   y:9 0 1 2 3 4 5   6  7   4   5    6    4   5    6 -- D6
	opcode sbc     y:9 8 9 A B C D   E  F   C   D    E    C   D    E -- DE
	
	details: M16= A - (src + if SBCB ? c_flag : 0)
			 F  = N_FLAG;
			 F |= -M16-hibyte
			 F |= (src ^ A ^ M16-lobyte) & 0x10 ? set h : clr h;
			 F |= M16-lobyte & 0x80 ? set s : clr s 
			 F |= M16-lobyte ? clr z : set z
			 F |= (((A ^ M16-lobyte) & (src ^ A)) & 0x80) ? V_FLAG : 0;
			 A = M16-lobyte
			 
	S 		affected as defined
	Z 		affected as defined
	H 		affected as defined	
	P/V 	detects overflow
	N 		set
	C 		affected as defined	*/
	
static 
void sub_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xD6) 
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
	cpu->A = cpu->MB0;
}		
static 
void sbc_b (struct z80cpu *cpu) {
	
	if (cpu->cur_opcode == 0xDE) 
		cpu->NP = u16 ( fetch_8bits_immediate_number (cpu) );
	else if (cpu->sel_field != 6) 
		cpu->NP = u16 ( cpu->regs_mapb8[cpu->sel_field][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	cpu->MW0= u16 (cpu->A) - cpu->NP - u16 (cpu->F & C_FLAG);
	cpu->F  = N_FLAG; 
	cpu->F |=-cpu->MB1;
	cpu->F |= zs_table[cpu->MB0];
	cpu->F |= ((cpu->NL ^ cpu->A ^ cpu->MB0) & 0x10) ? H_FLAG : 0;	
	cpu->F |= (((cpu->A ^ cpu->MB0) & (cpu->NL ^ cpu->MB0)) & 0x80) ? V_FLAG : 0;
	cpu->A = cpu->MB0;
}		

#if 0 

static 
void sub_w (struct z80cpu *cpu) {
	
	cpu->NP = cpu->regs_mapw16[2][0];
	cpu->IP = cpu->regs_mapw16[calc_who_unknown (0)][0];
	cpu->MD0= u32 (cpu->NP) - u32 (cpu->IP);
	cpu->F &= 0xC4; // Z-S-V
	cpu->F |= N_FLAG;
	cpu->F |=-cpu->MB2;
	cpu->F |= ((cpu->NP ^ cpu->IP ^ cpu->MW0) & 0x1000) ? H_FLAG : 0;	
	cpu->regs_mapw16[2][0] = cpu->MW0;
}	 

#endif

#endif