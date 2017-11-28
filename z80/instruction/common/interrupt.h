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

#ifndef __COMMON_INTERRUPT_H_INCLUED_
#define __COMMON_INTERRUPT_H_INCLUED_

// disable. opcode:F3. 4 cycles
static 
void clr_irq (z80cpu *cpu)	{
	
	cpu->IFF_1 = IFF_RESET;
	cpu->IFF_2 = IFF_RESET;
	cpu->runtime_status |= SB_FLAG;
}
// enable. opcode:FB. 4 cycles
static 
void clk_irq (z80cpu *cpu)	{
	
	cpu->IFF_1 = IFF_SET;
	cpu->IFF_2 = IFF_SET;
	cpu->runtime_status |= SB_FLAG;
}

#endif