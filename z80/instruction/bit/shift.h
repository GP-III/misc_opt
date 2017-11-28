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

#ifndef __BIT_SHIFT_H_INCLUED_
#define __BIT_SHIFT_H_INCLUED_

static 
void sft_i (struct z80cpu *cpu) {
	
	if (cpu->regs_mapbfp[6] == common_link_trap && cpu->sel_field != 6) {
		
		cpu->IP = u16 (cpu->regs_mapb8[cpu->sel_field]);
	} else {
		cpu->regs_mapbfp[6] (cpu); 
		cpu->IP = u16 ( cpu->z80_data_read (cpu, cpu->trap_address) );
	}
	if (cpu->y_field > 1) {
		if (!cpu->neg_field) {
			
		/* SLA/SLL 
		   .----.   .---------.   
		   | CF |<--| 7 <-- 0 |<-- SLA:0 SLL:1
		   '----'   '---------' */
		   cpu->F = 0;
		   cpu->IP <<= 1;
		   cpu->F |= cpu->IH;
		   cpu->IL|= (cpu->y_field & 1);
		   cpu->F |= psz_table[cpu->IL];
		   
		} else {
			
		/* SRA/SRL 
						.---------.   .----.  
		SRA:0 SRL:1 --> | 7 --> 0 |-->| CF |
						'---------'   '----' */
		   cpu->F = 0;
		   cpu->F |= (cpu->IL & 1);
		   cpu->IP >>= 1;
		   cpu->IL|= ( (cpu->y_field & 1) << 7 );
		   cpu->F |= psz_table[cpu->IL];  
		}
	}
	else {
		if (!cpu->neg_field) {
			
		/* = RLC/RL ====================00
		||								||
		||    .------------- RL ------. ||
		||	  |						  | ||
		||	  |		 .----- RLC ------. ||
		||	.----.   |  .---------.   | ||
		||	| CF |<-----| 7 <-- 0 |<--' ||
		\*=	'----'	    '---------'    =*/
			
		   cpu->NL= cpu->y_filed ? (cpu->F & 1) : (cpu->IL >> 7);
		   cpu->F = 0;
		   cpu->IP <<= 1;
		   cpu->F |= cpu->IH;
		   cpu->IL|= cpu->NL;
		   cpu->F |= psz_table[cpu->IL];
		   
		} else {

		/* = RRC/RR ====================00
		||								||
		||	.---- RR ----------------.	||
		||	|						 |  ||
		||	.---- RRC -------.		 |  ||
		||	|   .---------.  |   .----. ||
		||	'-->| 7 --> 0 |----->| CF | ||
		\*==	'---------'      '----' */
				
		   cpu->NL= cpu->y_filed ? (cpu->F & 1) : (cpu->IL & 1);
		   cpu->F = 0;
		   cpu->NL <<= 7;
		   cpu->IP >>= 1;
		   cpu->F |= cpu->IH;
		   cpu->IL|= cpu->NL;
		   cpu->F |= psz_table[cpu->IL];
		}
	}
	if (cpu->sel_field == 6)
		cpu->z80_data_write (cpu, cpu->trap_address, cpu->IL);
	else 
		cpu->regs_mapb8[cpu->sel_field] = cpu->IL;
}	

#endif