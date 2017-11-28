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

#ifndef __COMMON_ATOM_H_INCLUED_
#define __COMMON_ATOM_H_INCLUED_

/* 	INC_REGWORD

	destination register <- BC/DE/HL/AF/IX/IY 
	cycles 					06 06 06 06 10 10
	opcode 					03 13 23 33 23 23
	
	details: ++ reg ... 
	
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		unaffected
	C 		unaffected	
	H  		unaffected */
	
static 
void inc_w (struct z80cpu *cpu) {
	
	++cpu->regs_mapw16[calc_who_unknown (0)][0];
}

/* 	INC_BYTE

	target/source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)
	cycles			   		  4 4 4 4 4 4  11  4   8   8   23    8   8   23     
	opcode 	   			  x:4 0   1   2     3      2        3    2        3
	opcode				  x:C	0   1   2 	   3       2             2

	details: ++ reg, flags same as add mb/rb, 1, but .., save c_flag ~ */

static 
void inc_b (struct z80cpu *cpu) {
	
	if (cpu->y_field != 3 || cpu->neg_field) 
		cpu->NP = u16 ( cpu->regs_mapb8[calc_who_unknown (0)][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	
	cpu->MW0= cpu->NP + 1;
	cpu->F &= 1;
	cpu->F |=(cpu->NL ^ cpu->MB0 & H_FLAG);
	cpu->F |= cpu->MB1;
	cpu->F |= zs_table[cpu->MB0];
	cpu->F |= (((~(cpu->NL ^ cpu->MB0)) & cpu->MB0) & 0x80) ? V_FLAG : 0;
	
	if (cpu->y_field != 3 || cpu->neg_field) 
		cpu->regs_mapb8[calc_who_unknown (0)][0] = cpu->MB0;
	else 
		cpu->z80_data_write (cpu, cpu->trap_address, cpu->MB0);	
}

/* 	DEC_REGWORD

	destination register <- BC/DE/HL/AF/IX/IY 
	cycles 					06 06 06 06 10 10
	opcode 					05 15 25 35 25 25
	
	details: -- reg ... 
	
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		unaffected
	C 		unaffected	
	H  		unaffected */
	
static 
void dec_w (struct z80cpu *cpu) {
	
	--cpu->regs_mapw16[calc_who_unknown (0)][0];
}	

/* 	DEC_BYTE

	target/source register <- B/C/D/E/H/L/(HL)/A/IXH/IXL/(sIX)/IYH/IYL/(sIY)
	cycles			   		  4 4 4 4 4 4  11  4   8   8   23    8   8   23     
	opcode 	   			  x:5 0   1   2     3      2        3    2        3
	opcode				  x:D	0   1   2 	   3       2             2

	details: -- reg, flags same as sub mb/rb, 1, but .., save c_flag ~ */

static 
void dec_b (struct z80cpu *cpu) {
	
	if (cpu->y_field != 3 || cpu->neg_field) 
		cpu->NP = u16 ( cpu->regs_mapb8[calc_who_unknown (0)][0] );
	else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->NP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	
	cpu->MW0= cpu->NP - 1;
	cpu->F &= 1;
	cpu->F |= N_FLAG;
	cpu->F |=(cpu->NL ^ cpu->MB0 & H_FLAG);
	cpu->F |= cpu->MB1;
	cpu->F |= zs_table[cpu->MB0];
	cpu->F |= (((cpu->MB0 ^ cpu->NL) & cpu->NL) & 0x80) ? V_FLAG : 0;
	
	if (cpu->y_field != 3 || cpu->neg_field) 
		cpu->regs_mapb8[calc_who_unknown (0)][0] = cpu->MB0;
	else 
		cpu->z80_data_write (cpu, cpu->trap_address, cpu->MB0);	
}

#endif