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

#ifndef __COMMON_MISC_H_INCLUED_
#define __COMMON_MISC_H_INCLUED_

/* 	RLCA: - same as x86's ROL

			 .----------------.
	.----.   |  .---------.   |
	| CF |<-----| 7 <-- 0 |<--'
	'----'	    '---------'
			
	only register A use, 4 cycles. opcode:07
	
	details: F &= (~(H_FLAG | N_FLAG | C_FLAG));
			 t  = A >> 7; 
			      A <<=1;
			 A |= t;
			 F |= t;
			 
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		reset
	C 		affected as defined	
	H  		reset */

static 
void rlca_ (z80cpu *cpu)	{
	
	cpu->F &= (~(H_FLAG | N_FLAG | C_FLAG));
	cpu->MB4= cpu->A >> 7;
	cpu->A <<= 1;
	cpu->A |= cpu->MB4;
	cpu->F |= cpu->MB4;
}

/* 	RRCA: - same as x86's ROR

	.----------------.
	|   .---------.  |   .----.
	'-->| 7 --> 0 |----->| CF |
		'---------'      '----'
			
	only register A use, 4 cycles. opcode:0F
	
	details: F &= (~(H_FLAG | N_FLAG | C_FLAG));
			 t  = A & 1; 
			      A >>=1;
			 A |=(t <<7);
			 F |= t;
			 
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		reset
	C 		affected as defined	
	H  		reset */	
	
static 
void rrca_ (z80cpu *cpu)	{
	
	cpu->F &= (~(H_FLAG | N_FLAG | C_FLAG));
	cpu->MB4= cpu->A & 1;
	cpu->A >>= 1;
	cpu->A |=(cpu->MB4 << 7);
	cpu->F |= cpu->MB4;
}	

/* 	RLA: - same as mos6502's ROL

	.-------------------------.
	|  .----.   .---------.   |
	'--| CF |<--| 7 <-- 0 |<--'
	   '----'   '---------'
			
	only register A use, 4 cycles. opcode:17
	
	details: M8 = F & C_FLAG;
			 F &= (~(H_FLAG | N_FLAG | C_FLAG));
			 t  = A >> 7; 
			      A <<=1;
			 A |= M8;
			 F |= t;
			 
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		reset
	C 		affected as defined	
	H  		reset */

static 
void rla_ (z80cpu *cpu)	{
	
	cpu->MB5= cpu->F & 1;
	cpu->F &= (~(H_FLAG | N_FLAG | C_FLAG));
	cpu->MB4= cpu->A >> 7;
	cpu->A <<= 1;
	cpu->A |= cpu->MB5;
	cpu->F |= cpu->MB4;
}	

/* 	RRA: - same as mos6502's ROR

	.-------------------------.
	|   .---------.   .----.  |
	'-->| 7 --> 0 |-->| CF |--'
		'---------'   '----'
			
	only register A use, 4 cycles. opcode:1F
	
	details: M8 = F & C_FLAG;
			 F &= (~(H_FLAG | N_FLAG | C_FLAG));
			 t  = A & C_FLAG; 
			      A >>=1;
			 A |=(M8 << 7);
			 F |= t;
			 
	S 		unaffected
	Z 		unaffected
	H 		unaffected
	P/V 	unaffected
	N 		reset
	C 		affected as defined	*/
	
static 
void rra_ (z80cpu *cpu)	{
	
	cpu->MB5= cpu->F << 7;
	cpu->F &= (~(H_FLAG | N_FLAG | C_FLAG));
	cpu->MB4= cpu->A & 1;
	cpu->A >>= 1;
	cpu->A |= cpu->MB5;
	cpu->F |= cpu->MB4;
}

#endif