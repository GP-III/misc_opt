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

#ifndef __COMMON_MOV_H_INCLUED_
#define __COMMON_MOV_H_INCLUED_

/* 	MARK: - for y 4~7 line mov instruction martix *.*/	
static 
void mmov (z80cpu *cpu) {
	
	if (cpu->cur_opcode == HALT) {
        cpu->PC --; // halt  
        cpu->runtime_status |= HL_FLAG; 
    } else {
		if (cpu->sel_field != 6) 
			cpu->IL = cpu->regs_mapb8[cpu->sel_field][0];
		else {
			cpu->regs_mapbfp[6] (cpu); 
			cpu->IL = cpu->z80_data_read (cpu, cpu->trap_address);
		}
		if (cpu->y_field != 7 || cpu->neg_field) {
			cpu->regs_map_byte[calc_who_unknown (4)][0] = cpu->IL;
		else {
			cpu->regs_mapbfp[6] (cpu); 
			cpu->z80_data_write (cpu, cpu->trap_address, cpu->IL);	
		}
	}		
}
/* 	MARK: - for extend mov instruction martix *.*/	
static 
void emov (z80cpu *cpu) {
	
	if (cpu->sel_field == 6) {
		
		cpu->IP = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
		
		if (cpu->y_field != 3 || cpu->neg_field) {
			cpu->regs_map_byte[calc_who_unknown (4)][0] = cpu->IL;
		else {
			cpu->regs_mapbfp[6] (cpu); 
			cpu->z80_data_write (cpu, cpu->trap_address, cpu->IL);	
		}
	} else if (cpu->sel_field == 1) {
		
		cpu->IL = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
		cpu->IH = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
		
		cpu->regs_mapw16[cpu->y_field][0] = cpu->IP;
	} else {
		if (!cpu->neg_field) {
			if (cpu->y_field < 2) {
				cpu->z80_data_write (cpu, cpu->regs_mapw16[cpu->y_field][0], cpu->A);
			} else {
				cpu->AL = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
				cpu->AH = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
				
				if (cpu->y_field == 3)
					cpu->z80_data_write (cpu, cpu->AP, cpu->A);
				else {
					cpu->z80_data_write (cpu, cpu->AP++, cpu->regs_mapw8[2][0]);
					cpu->z80_data_write (cpu, cpu->AP+0, cpu->regs_mapw8[2][1]);
				}
			}
		}
		else {
			if (cpu->y_field < 2) {
				cpu->A = cpu->z80_data_read (cpu, cpu->regs_mapw16[cpu->y_field][0]);
			} else {
				cpu->AL = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
				cpu->AH = u16 ( cpu->z80_data_read (cpu, cpu->PC++) );
				
				if (cpu->y_field == 3)
					cpu->A = cpu->z80_data_read (cpu, cpu->AP);
				else {		
					cpu->regs_mapw8[2][0] = cpu->z80_data_read (cpu, cpu->AP++);
					cpu->regs_mapw8[2][1] = cpu->z80_data_read (cpu, cpu->AP+0);
				}
			}
		}
	}
}

#endif