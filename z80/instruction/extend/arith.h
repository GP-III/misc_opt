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

#ifndef __EXTEND_ARITH_H_INCLUED_
#define __EXTEND_ARITH_H_INCLUED_

/* 	ADCW: HL += src + c_flag

	source register <- BC/DE/HL/SP
	cycles			   15 15 15	15
	opcode 			   4A 5A 6A 7A 
	
	details: M32= src + HL + c_flag
			 F &= 0;
			 F |= M32 > 0xFFFF ? set c : clr c
			 F |= (src ^ HL ^ M32-loword) & 0x1000 ? set h : clr h;
			 F |= M32-loword & 0x8000 ? set s : clr s 
			 F |= M32-loword ? clr z : set z
			 F |= (((~(HL ^ M32-loword)) & (src ^ M32-loword)) & 0x8000) ? V_FLAG : 0;
			 HL = M32-loword
			 
	S 		affected as defined
	Z 		affected as defined
	H 		affected as defined	
	P/V 	detects overflow
	N 		reset
	C 		affected as defined	*/

static 
void adc_w (struct z80cpu *cpu) {
	
	cpu->IP = cpu->regs_mapw16[calc_who_unknown (4)][0];
	cpu->MD0= u32 (cpu->HL) + u32 (cpu->IP) + u32 (cpu->F & C_FLAG);
	cpu->F &= 0; // reset all
	cpu->F |= cpu->MB2;
	cpu->F |= cpu->MW0 ? 0x00 : Z_FLAG;
	cpu->F |= cpu->MB1 & 0x80 ? S_FLAG : 0;
	cpu->F |= ((cpu->HL ^ cpu->IP ^ cpu->MW0) & 0x1000) ? H_FLAG : 0;	
	cpu->F |= (((~(cpu->HL ^ cpu->MW0)) & (cpu->IP ^ cpu->MW0)) & 0x8000) ? V_FLAG : 0;
	cpu->HL = cpu->MW0;
}	

/* 	SDCW: HL -= src + c_flag

	source register <- BC/DE/HL/SP
	cycles			   15 15 15 15
	opcode 			   42 52 62 72
	
	details: M32= src - HL - c_flag
			 F  = N_FLAG;
			 F |= -M32 & 0x10000 ? set c : clr c
			 F |= (src ^ HL ^ M32-loword) & 0x1000 ? set h : clr h;
			 F |= M32-loword & 0x8000 ? set s : clr s 
			 F |= M32-loword ? clr z : set z
			 F |= (((HL ^ M32-loword) & (HL ^ src)) & 0x8000) ? V_FLAG : 0;
			 HL = M32-loword
			 
	S 		affected as defined
	Z 		affected as defined
	H 		affected as defined	
	P/V 	detects overflow
	N 		set
	C 		affected as defined	*/
	
static 
void sbc_w (struct z80cpu *cpu) {
	
	cpu->IP = cpu->regs_mapw16[calc_who_unknown (4)][0];
	cpu->MD0= u32 (cpu->HL) - u32 (cpu->IP) - u32 (cpu->F & C_FLAG);
	cpu->F  = N_FLAG; // .
	cpu->F |=-cpu->MB2;
	cpu->F |= cpu->MW0 ? 0x00 : Z_FLAG;
	cpu->F |=(cpu->MB1 & S_FLAG);
	cpu->F |= ((cpu->H ^ cpu->IH ^ cpu->MB1) & H_FLAG);	
	cpu->F |= (((cpu->H ^ cpu->MB1) & (cpu->H ^ cpu->IH)) & 0x80) ? V_FLAG : 0;
	cpu->HL = cpu->MW0;
}	

static 
void neg_ (struct z80cpu *cpu) {
	
	cpu->MW0= 0 - u16 (cpu->A);
	cpu->F  = N_FLAG; 
	cpu->F |=-cpu->MB1;
	cpu->F |= zs_table[cpu->MB0];
	cpu->F |= ((cpu->A ^ cpu->MB0) & 0x10) ? H_FLAG : 0;	
	cpu->F |= (((cpu->MB0) & (cpu->A ^ cpu->MB0)) & 0x80) ? V_FLAG : 0;
	cpu->A = cpu->MB0;
}

#endif