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

#ifndef __EXTEND_INTERRUPT_H_INCLUED_
#define __EXTEND_INTERRUPT_H_INCLUED_

// select int mode.
// ===============
static 
void sel_irq (z80cpu *cpu)	{
	
	cpu->IMF_a = IFF_RESET;
	cpu->IMF_b = IFF_RESET;
	
	switch ( calc_who_unknown (4) ) {
		
	case 2:
	case 6:
		cpu->IMF_a = IFF_SET;
		break;
	case 3:
	case 7:
		cpu->IMF_a = IFF_SET;
	case 1:
	case 5:
		cpu->IMF_b = IFF_SET;
	case 0:
	case 4:
		break;
	}
}

static 
void retim (z80cpu *cpu)	{
	
	switch ( calc_who_unknown (4) ) {
		
	case 0: // NMI
	case 2: // NMI
	case 3: // NMI
	case 4: // NMI
	case 5: // NMI
	case 6: // NMI
	case 7: // NMI
		cpu->IFF_1 = cpu->IFF_2;
	case 1: // IRQ
		cpu->PCL = cpu->z80_read_data (cpu, cpu->SP++);
		cpu->PCH = cpu->z80_read_data (cpu, cpu->SP++);
		break;
	}
}

#endif