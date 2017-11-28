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

#ifndef __XEEC04878_
#define __XEEC04878_

#define S_FLAG      0x80    /*\ - 1: Result negative      	- \*/
#define Z_FLAG      0x40    /*\ - 1: Result is zero       	- \*/
#define H_FLAG      0x10    /*\ - 1: Halfcarry/Halfborrow 	- \*/
#define P_FLAG      0x04    /*\ - 1: Result is even       	- \*/
#define V_FLAG      0x04    /*\ - 1: Overflow occured     	- \*/
#define N_FLAG      0x02    /*\ - 1: Subtraction occured  	- \*/
#define C_FLAG      0x01    /*\ - 1: Carry/Borrow occured 	- \*/
#define H_MASK		0x04
#define V_MASK		0x80 

#define IFF_SET		0x01 	/*\ - set enable  interrupt 	- \*/
#define IFF_RESET	0x00 	/*\ - clr inhibit interrupt 	- \*/

#define INT_MODE0	0x00	/*\ - IMF_a:0 IMF_b:0 			- \*/
#define INT_NOUSE	0x01	/*\ - IMF_a:0 IMF_b:1 			- \*/
#define INT_MODE1 	0x02	/*\ - IMF_a:1 IMF_b:0 			- \*/
#define INT_MODE2 	0x03	/*\ - IMF_a:1 IMF_b:1 			- \*/

#define SB_FLAG 0x01 
#define HL_FLAG 0x10 

#define NMI_VECTOR 	0x66	/*\ - NMI Rountine Address 		- \*/
#define SPLIT_CPU_CYCLES 0x7FFFFFFF	/*\ - timing split 		- \*/

#define REG_MAPPER_TYPE0 0 
#define REG_MAPPER_TYPE1 1
#define REG_MAPPER_TYPE2 2

typedef char cint8_t;
typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed int sint32_t;
typedef signed long long sint64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef float float32_t;
typedef double float64_t;
typedef signed int boolean_t;
typedef signed int flip_flops_t;
typedef signed int flip_flops_mix_t;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL (((void *)0))
#endif

enum MAPPER_TYPE {
	
REG_MAPPER_TYPE0,
REG_MAPPER_TYPE1,
REG_MAPPER_TYPE2
};

/* extern interface */
typedef uint8_t (*fp_z80)	 	 (void *z80cpu);
typedef uint8_t (*fp_io_read)	 (void *z80cpu, uint16_t);
typedef uint8_t (*fp_io_read_ex) (void *z80cpu, uint8_t);
typedef void 	(*fp_io_write)	 (void *z80cpu, uint16_t, uint8_t);	
typedef void 	(*fp_io_write_ex)(void *z80cpu, uint8_t,  uint8_t);

struct z80cpu {
	
#define DEFINE_REG_x(h,l) 	   /* normal */ \
	union { struct { uint8_t 	 l,   h; }; \
					 uint16_t h##l; 		\
	}
#define DEFINE_REG_c(h,l) 	   /* cache */	\
	union { struct { uint8_t 	 l##_Cache, \
								 h##_Cache; \
			}; 								\
					 uint16_t h##l##_Cache; \
	}
#define DEFINE_REG_s(h,l,x)    /* tp-mc */	\
	union { struct { uint8_t 	 l,   h; }; \
					 uint16_t    x; 		\
	}
// normal register/mirror 
	DEFINE_REG_x (A, F);
	DEFINE_REG_x (B, C);
	DEFINE_REG_x (D, E);
	DEFINE_REG_x (H, L);
	DEFINE_REG_x (I, R);
	DEFINE_REG_c (A, F);
	DEFINE_REG_c (B, C);
	DEFINE_REG_c (D, E);
	DEFINE_REG_c (H, L);	
	DEFINE_REG_s (IXH, IXL, IX);
	DEFINE_REG_s (IYH, IYL, IY);
	DEFINE_REG_s (SPH, SPL, SP);
	DEFINE_REG_s (PCH, PCL, PC);
// temp register 
	DEFINE_REG_s (TH, TL, TP);
	DEFINE_REG_s (NH, NL, NP);
	DEFINE_REG_s (AH, AL, AP);
	DEFINE_REG_s (IH, IL, IP);	
// large num ... 
	union { struct { uint8_t  MB0, MB1, MB2, MB3; }; 
			struct { uint16_t MW0, MW1; }; 
					 uint32_t MD0; 
	}; 
	union { struct { uint8_t  MB4, MB5, MB6, MB7; }; 
			struct { uint16_t MW2, MW3; }; 
					 uint32_t MD1; 
	}; 
// extend 
	union { void 	 *v0ext_read;
			uint8_t  *u8ext_read;
			uint16_t *u16ext_read;
			uint32_t *u32ext_read;
	}; 
	union { void 	 *v0ext_write;
			uint8_t  *u8ext_write;
			uint16_t *u16ext_write;
			uint32_t *u32ext_write;
	};
	union { void 	 *v0ext_ref;
			uint8_t  *u8ext_ref;
			uint16_t *u16ext_ref;
			uint32_t *u32ext_ref;
	};
	union { void 	 *regs_mapb0[8];
			uint8_t  *regs_mapb8[8];
			uin16_t  *regs_mapb16[8];
			uint32_t *regs_mapb32[8];
			fp_z80 regs_mapbfp[8];
	};
	union { void 	 *regs_mapw0[4];
			uint8_t  *regs_mapw8[4];
			uin16_t  *regs_mapw16[4];
			uint32_t *regs_mapw32[4];
			fp_z80 regs_mapwfp[4];
	};
	union { void 	 *regs_mapw0_ext[4];
			uint8_t  *regs_mapw8_ext[4];
			uin16_t  *regs_mapw16_ext[4];
			uint32_t *regs_mapw32_ext[4];
			fp_z80 regs_mapwfp_ext[4];
	};
	uint16_t x_field;
	uint16_t y_field;
	uint16_t sel_field;
	uint16_t neg_field;

	flip_flops_t IFF_1;
	flip_flops_t IFF_2; // interrupt flag 
	
	flip_flops_t IMF_a;
	flip_flops_t IMF_b; // interrupt mode 

	flip_flops_mix_t runtime_status; 
	
	uint8_t cur_opcode; // cur pick up's PRG
	uint8_t extern_io_byte; // extern device 's bus data 

	uint16_t trap_address;
	
	uint32_t exec_cycles; 
	uint32_t exec_offset_cycles
	uint32_t split_cycles_count0; 
	uint32_t split_cycles_count1; // cycle about/ cycles split 
	
	// todo:
	// ===========
	// list *irql;
	// list *irq;
	
	boolean_t nmi_signal; // NMI signal 
	boolean_t irq_signal; // IRQ signal

	fp_io_read 	   z80_data_read;
	fp_io_read_ex  z80_port_read;
	fp_io_write    z80_data_write;
	fp_io_write_ex z80_port_write;
} ;

/*  MARK: - A.B.?.*..*/
static 
uint32_t calc_who_unknown (z80cpu *cpu, uint32_t pm) {
	
	return u32 ( ( (cpu->y_field - pm) << 1) + (cpu->neg_field >> 3) );
}
/*  MARK: - built register field *.*/
static 
void built_register_field (z80cpu *cpu) {
	
	cpu->x_field = cpu->cur_opcode & 0x0F;
    cpu->y_field = cpu->cur_opcode & 0xF0; 
    cpu->sel_field = cpu->cur_opcode & 0x07; 
    cpu->neg_field = cpu->cur_opcode & 0x08; 
}
/* 	MARK: - fetch 16 bits Immediate number *.*/
static 
void fetch_16bits_immediate_number (z80cpu *cpu) {
	
	cpu->IL = cpu->z80_data_read (cpu, cpu->PC++);
	cpu->IH = cpu->z80_data_read (cpu, cpu->PC++);
}
/* 	MARK: - fetch 8 bits Immediate number *.*/
static 
void fetch_8bits_immediate_number (z80cpu *cpu) {
	
	cpu->IP = u16( cpu->z80_data_read (cpu, cpu->PC++) );
}
/* 	MARK: - symbol link *.*/	
static 
void common_link_trap (z80cpu *cpu) {
	
	cpu->trap_address = cpu->HL;
}
static 
void orionx_link_trap (z80cpu *cpu) {
	cpu->trap_address = cpu->IX + s16 ( fetch_8bits_immediate_number (cpu) );
}
static 
void oriony_link_trap (z80cpu *cpu) {
	cpu->trap_address = cpu->IY + s16 ( fetch_8bits_immediate_number (cpu) );
}
/*  MARK: - built register mapper grounp *.*/
static 
void built_register_mapper (z80cpu *cpu, MAPPER_TYPE map_type) {
	
	if (map_type == REG_MAPPER_TYPE0) {	
		cpu->regs_mapbfp[6] = common_link_trap;
		cpu->v0ext_read = &cpu->L;
	} 
	else if (map_type == REG_MAPPER_TYPE1) {		
		cpu->regs_mapbfp[6] = orionx_link_trap;
		cpu->v0ext_read = &cpu->IXL;
	}	
	else if (map_type == REG_MAPPER_TYPE2) {		
		cpu->regs_mapbfp[6] = oriony_link_trap;
		cpu->v0ext_read = &cpu->IYL;
	}
		
	cpu->regs_mapb0[0] = &cpu->B;
	cpu->regs_mapb0[1] = &cpu->C;
	cpu->regs_mapb0[2] = &cpu->D;
	cpu->regs_mapb0[3] = &cpu->E;
	cpu->regs_mapb0[4] = &cpu->u8ext_read[1];
	cpu->regs_mapb0[5] = &cpu->u8ext_read[0];		
	cpu->regs_mapb0[7] = &cpu->A;	
    cpu->regs_mapw0[0] = &cpu->BC;
	cpu->regs_mapw0[1] = &cpu->DE;
	cpu->regs_mapw0[2] = &cpu->u16ext_read[0];
	cpu->regs_mapw0[3] = &cpu->SP;
    cpu->regs_mapw0_ext[0] = &cpu->BC;
	cpu->regs_mapw0_ext[1] = &cpu->DE;
	cpu->regs_mapw0_ext[2] = &cpu->u16ext_read[0];
	cpu->regs_mapw0_ext[3] = &cpu->AF;	
}
/* 	MARK: - for y 4~7 line mov instruction martix *.*/	
static 
void launch_across_register_martix (z80cpu *cpu) {
	
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

#endif
