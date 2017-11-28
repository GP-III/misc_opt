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

#ifndef __COMMON_PROC_H_INCLUED_
#define __COMMON_PROC_H_INCLUED_

static 
void rst_ (struct z80cpu *cpu) {
	
	cpu->z80_write_data (cpu, --cpu->SP, cpu->PCH);
	cpu->z80_write_data (cpu, --cpu->SP, cpu->PCL);
	//..i.e
	cpu->PC = u16 ( calc_who_unknown (12) ) * 8;
}

static 
void cc_exec (struct z80cpu *cpu) {
	
	static 
    const uint8_t contex_martix[16] = {
        
        Z_FLAG, Z_FLAG,
        C_FLAG, C_FLAG,
        P_FLAG, P_FLAG, 
        S_FLAG, S_FLAG, 
        Z_FLAG,      0,
        C_FLAG,      0,
        P_FLAG,      0,
        S_FLAG,      0
    };
    static 
    const uint8_t ccycle_martix[16] = {	
		
        6, 6,
		0, 0,
		7, 7,
		9, 9,
        5, 5,
        5, 5,
		5, 5,
		5, 5
    }; 
	static 
    const uint8_t length_martix[16] = {
		
        0, 0,
		2, 2,
		2, 2,
		7, 7,
        1, 1,
        1, 1,
		1, 1,
		1, 1
    }; 
	if (cpu->y_field < 12) {
		
		if (cpu->cur_opcode < 0x20) {		
		
			if (cpu->cur_opcode == 0x18) {
				goto _jr;
			}
			if (--cpu->B) {
				goto _jr; 
			} else {
				cpu->PC++;
				return;
			}
		} 
		else {
			cpu->y_field += 10; // field adjust
		}
	} else if (cpu->sel_field & 1) {
		if (cpu->sel_field != 1) {
			cpu->sel_field --;
			goto _jmp;
		}
		else { 
			goto _ret;
		}
	}
	// -
	// -
	cpu->MD0 = calc_who_unknown (12); // calc opposite limit index	
	// -
	// -
	cpu->neg_field = cpu->cur_opcode < 0x80 ? 0x08 : 0x00; // field adjust 
	// -
	// -
	if ((cpu->F & contex_martix[cpu->MD0]) \
				^ contex_martix[cpu->MD0]) {
		// - 
		cpu->burning_cycles += ccycle_martix[cpu->neg_field + cpu->sel_field];
		// -
		if (!cpu->sel_field) {
			if (!cpu->neg_field) {
			/*. ret .*/ _ret:
				cpu->PCL = cpu->z80_read_data (cpu, cpu->SP++);
				cpu->PCH = cpu->z80_read_data (cpu, cpu->SP++);
			} else {
			/*. jr .*/ _jr:	
				cpu->PC = cpu->PC + s16 ( cpu->z80_read_data (cpu, cpu->PC) ) + 1;
			}
		} else {
			/*. jmp/call .*/ _jmp:
			cpu->NL = cpu->z80_read_data (cpu, cpu->PC++);
			cpu->NH = cpu->z80_read_data (cpu, cpu->PC+0);
			
			if (cpu->sel_field == 4) {
			// -
				cpu->z80_write_data (cpu, --cpu->SP, cpu->PCH);
				cpu->z80_write_data (cpu, --cpu->SP, cpu->PCL);
			}
			cpu->PC = cpu->NP;	
		}	
	} else {	
		cpu->PC += length_martix[cpu->neg_field + cpu->sel_field];
	}
}

#endif