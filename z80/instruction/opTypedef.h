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
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU 
** Library General Public License for more details.	 To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#ifndef __OPCODE_TYPE_DEF_INCLUED_
#define __OPCODE_TYPE_DEF_INCLUED_

/*\ --------------------- Common --------------------- \*/

// begin 00 
#define NOP				0x00 // 00	 Idle					-  4 cycles 
#define I16_TO_BC		0x01 // 01	 BC = imm16				- 10 cycles 
#define A_TO_xBC		0x02 // 02	(BC)= A					-  7 cycles
#define INC_BC			0x03 // 03 ++BC						-  6 cycles 
#define INC_B			0x04 // 04 ++B						-  4 cycles 
#define DEC_B			0x05 // 05 --B						-  4 cycles
#define I08_TO_B		0x06 // 06	 B	= imm8				-  7 cycles 
#define RLCA			0x07 // 07				x86's ROL	-  4 cycles 
#define SWP_AF_MIR		0x08 // 08	 AF~= AF'				-  4 cycles 
#define ADD_HL_BC		0x09 // 09	 HL+= BC				- 11 cycles 
#define xBC_TO_A		0x0A // 0A	 A	=(BC)				-  7 cycles 
#define DEC_BC			0x0B // 0B --BC						-  6 cycles
#define INC_C			0x0C // 0C ++C						-  4 cycles
#define DEC_C			0x0D // 0D --C						-  4 cycles 
#define I08_TO_C		0x0E // 0E	 C	= imm8				-  7 cycles 
#define RRCA			0x0F // 0F				x86's ROR	-  4 cycles 

// begin 10 
#define DJNZ			0x10 // 10					   !--B -  8 cycles + hit -> 5 cycles 
#define I16_TO_DE		0x11 // 11	 DE = imm16				- 10 cycles 
#define A_TO_xDE		0x12 // 12	(DE)= A					-  7 cycles 
#define INC_DE			0x13 // 13 ++DE						-  6 cycles 
#define INC_D			0x14 // 14 ++D						-  4 cycles 
#define DEC_D			0x15 // 15 --D						-  4 cycles
#define I08_TO_D		0x16 // 16	 D	= imm8				-  7 cycles 
#define RLA				0x17 // 17				6502's ROL	-  4 cycles 
#define JR				0x18 // 18	 sjmp					- 12 cycles 
#define ADD_HL_DE		0x19 // 19	 HL+= DE				- 11 cycles 
#define xDE_TO_A		0x1A // 1A	 A	=(DE)				-  7 cycles 
#define DEC_DE			0x1B // 1B --DE						-  6 cycles
#define INC_E			0x1C // 1C ++E						-  4 cycles
#define DEC_E			0x1D // 1D --E						-  4 cycles 
#define I08_TO_E		0x1E // 1E	 E	= imm8				-  7 cycles 
#define RRA				0x1F // 1F				6502's ROR	-  4 cycles 

// begin 20 
#define JR_NZ			0x20 // 20						!ZF -  7 cycles + hit -> 5 cycles 
#define I16_TO_HL		0x21 // 21	 HL = imm16				- 10 cycles 
#define HL_TO_xI16		0x22 // 22(imm16)= HL				- 16 cycles 
#define INC_HL			0x23 // 23 ++HL						-  6 cycles
#define INC_H			0x24 // 24 ++H						-  4 cycles 
#define DEC_H			0x25 // 25 --H						-  4 cycles 
#define I08_TO_H		0x26 // 26	 H	= imm8				-  7 cycles 
#define DAA				0x27 // 27					  A-BCD -  4 cycles 
#define JR_Z			0x28 // 28						 ZF -  7 cycles + hit -> 5 cycles 
#define ADD_HL_HL		0x29 // 29	 HL+= HL				- 11 cycles 
#define xI16_TO_HL		0x2A // 2A	 HL =(imm16)			- 16 cycles 
#define DEC_HL			0x2B // 2B --HL						-  6 cycles 
#define INC_L			0x2C // 2C ++L						-  4 cycles 
#define DEC_L			0x2D // 2D --L						-  4 cycles 
#define I08_TO_L		0x2E // 2E	 L	= imm8				-  7 cycles 
#define CPL				0x2F // 2F						 ~A -  4 cycles 

// begin 30 
#define JR_NC			0x30 // 30						!CF -  7 cycles + hit -> 5 cycles 
#define I16_TO_SP		0x31 // 31	 SP = imm16				- 10 cycles 
#define A_TO_xI16		0x32 // 32(imm16)= A				- 13 cycles 
#define INC_SP			0x33 // 33 ++SP						-  6 cycles 
#define INC_xHL			0x34 // 34 ++(HL)					- 11 cycles 
#define DEC_xHL			0x35 // 35 --(HL)					- 11 cycles 
#define I08_TO_xHL		0x36 // 36	 (HL)=imm8				- 10 cycles 
#define SCF				0x37 // 37					 set CF -  4 cycles 
#define JR_C			0x38 // 38						 CF -  7 cycles + hit -> 5 cycles 
#define ADD_HL_SP		0x39 // 39	 HL+= SP				- 11 cycles 
#define xI16_TO_A		0x3A // 3A	 A	=(imm16)			- 13 cycles 
#define DEC_SP			0x3B // 3B --SP						-  6 cycles 
#define INC_A			0x3C // 3C ++A						-  4 cycles 
#define DEC_A			0x3D // 3D --A						-  4 cycles 
#define I08_TO_A		0x3E // 3E	 A	=imm8				-  7 cycles 
#define CCF				0x3F // 3F						^CF -  4 cycles 

// begin 40 
#define B_TO_B			0x40 // 40	 B	= B					-  4 cycles 
#define C_TO_B			0x41 // 41	 B	= C					-  4 cycles 
#define D_TO_B			0x42 // 42	 B	= D					-  4 cycles 
#define E_TO_B			0x43 // 43	 B	= E					-  4 cycles 
#define H_TO_B			0x44 // 44	 B	= H					-  4 cycles 
#define L_TO_B			0x45 // 45	 B	= L					-  4 cycles 
#define xHL_TO_B		0x46 // 46	 B	=(HL)				-  7 cycles 
#define A_TO_B			0x47 // 47	 B	= A					-  4 cycles 
#define B_TO_C			0x48 // 48	 C	= B					-  4 cycles 
#define C_TO_C			0x49 // 49	 C	= C					-  4 cycles 
#define D_TO_C			0x4A // 4A	 C	= D					-  4 cycles 
#define E_TO_C			0x4B // 4B	 C	= E					-  4 cycles
#define H_TO_C			0x4C // 4C	 C	= H					-  4 cycles 
#define L_TO_C			0x4D // 4D	 C	= L					-  4 cycles 
#define xHL_TO_C		0x4E // 4E	 C	=(HL)				-  7 cycles 
#define A_TO_C			0x4F // 4F	 C	= A					-  4 cycles 

// begin 50 
#define B_TO_D			0x50 // 50	 D	= B					-  4 cycles 
#define C_TO_D			0x51 // 51	 D	= C					-  4 cycles 
#define D_TO_D			0x52 // 52	 D	= D					-  4 cycles 
#define E_TO_D			0x53 // 53	 D	= E					-  4 cycles 
#define H_TO_D			0x54 // 54	 D	= H					-  4 cycles 
#define L_TO_D			0x55 // 55	 D	= L					-  4 cycles 
#define xHL_TO_D		0x56 // 56	 D	=(HL)				-  7 cycles 
#define A_TO_D			0x57 // 57	 D	= A					-  4 cycles 
#define B_TO_E			0x58 // 58	 E	= B					-  4 cycles 
#define C_TO_E			0x59 // 59	 E	= C					-  4 cycles 
#define D_TO_E			0x5A // 5A	 E	= D					-  4 cycles 
#define E_TO_E			0x5B // 5B	 E	= E					-  4 cycles
#define H_TO_E			0x5C // 5C	 E	= H					-  4 cycles 
#define L_TO_E			0x5D // 5D	 E	= L					-  4 cycles 
#define xHL_TO_E		0x5E // 5E	 E	=(HL)				-  7 cycles 
#define A_TO_E			0x5F // 5F	 E	= A					-  4 cycles

// begin 60 
#define B_TO_H			0x60 // 60	 H	= B					-  4 cycles 
#define C_TO_H			0x61 // 61	 H	= C					-  4 cycles 
#define D_TO_H			0x62 // 62	 H	= D					-  4 cycles 
#define E_TO_H			0x63 // 63	 H	= E					-  4 cycles 
#define H_TO_H			0x64 // 64	 H	= H					-  4 cycles 
#define L_TO_H			0x65 // 65	 H	= L					-  4 cycles 
#define xHL_TO_H		0x66 // 66	 H	=(HL)				-  7 cycles 
#define A_TO_H			0x67 // 67	 H	= A					-  4 cycles 
#define B_TO_L			0x68 // 68	 L	= B					-  4 cycles 
#define C_TO_L			0x69 // 69	 L	= C					-  4 cycles 
#define D_TO_L			0x6A // 6A	 L	= D					-  4 cycles 
#define E_TO_L			0x6B // 6B	 L	= E					-  4 cycles
#define H_TO_L			0x6C // 6C	 L	= H					-  4 cycles 
#define L_TO_L			0x6D // 6D	 L	= L					-  4 cycles 
#define xHL_TO_L		0x6E // 6E	 L	=(HL)				-  7 cycles 
#define A_TO_L			0x6F // 6F	 L	= A					-  4 cycles

// begin 70 
#define B_TO_xHL		0x70 // 70	(HL)= B					-  7 cycles 
#define C_TO_xHL		0x71 // 71	(HL)= C					-  7 cycles 
#define D_TO_xHL		0x72 // 72	(HL)= D					-  7 cycles 
#define E_TO_xHL		0x73 // 73	(HL)= E					-  7 cycles 
#define H_TO_xHL		0x74 // 74	(HL)= H					-  7 cycles 
#define L_TO_xHL		0x75 // 75	(HL)= L					-  7 cycles 
#define HALT			0x76 // 76			stop : interrupt-  4 cycles 
#define A_TO_xHL		0x77 // 77	(HL)= A					-  7 cycles 
#define B_TO_A			0x78 // 78	 A	= B					-  4 cycles 
#define C_TO_A			0x79 // 79	 A	= C					-  4 cycles 
#define D_TO_A			0x7A // 7A	 A	= D					-  4 cycles 
#define E_TO_A			0x7B // 7B	 A	= E					-  4 cycles 
#define H_TO_A			0x7C // 7C	 A	= H					-  4 cycles 
#define L_TO_A			0x7D // 7D	 A	= L					-  4 cycles 
#define xHL_TO_A		0x7E // 7E	 A	=(HL)				-  7 cycles 
#define A_TO_A			0x7F // 7F	 A	= A					-  4 cycles

// begin 80
#define ADD_B			0x80 // 80	 A += B					-  4 cycles 
#define ADD_C			0x81 // 81	 A += C					-  4 cycles 
#define ADD_D			0x82 // 82	 A += D					-  4 cycles 
#define ADD_E			0x83 // 83	 A += E					-  4 cycles 
#define ADD_H			0x84 // 84	 A += H					-  4 cycles 
#define ADD_L			0x85 // 85	 A += L					-  4 cycles 
#define ADD_xHL			0x86 // 86	 A +=(HL)				-  7 cycles 
#define ADD_A			0x87 // 87	 A += A					-  4 cycles 
#define ADC_B			0x88 // 88	 A += B	  + CF			-  4 cycles 
#define ADC_C			0x89 // 89	 A += C	  + CF			-  4 cycles 
#define ADC_D			0x8A // 8A	 A += D	  + CF			-  4 cycles 
#define ADC_E			0x8B // 8B	 A += E	  + CF			-  4 cycles 
#define ADC_H			0x8C // 8C	 A += H	  + CF			-  4 cycles 
#define ADC_L			0x8D // 8D	 A += L	  + CF			-  4 cycles 
#define ADC_xHL			0x8E // 8E	 A +=(HL) + CF			-  7 cycles 
#define ADC_A			0x8F // 8F	 A += A	  + CF			-  4 cycles 
	
// begin 90 
#define SUB_B			0x90 // 90	 A -= B					-  4 cycles 
#define SUB_C			0x91 // 91	 A -= C					-  4 cycles 
#define SUB_D			0x92 // 92	 A -= D					-  4 cycles 
#define SUB_E			0x93 // 93	 A -= E					-  4 cycles 
#define SUB_H			0x94 // 94	 A -= H					-  4 cycles 
#define SUB_L			0x95 // 95	 A -= L					-  4 cycles 
#define SUB_xHL			0x96 // 96	 A -=(HL)				-  7 cycles 
#define SUB_A			0x97 // 97	 A -= A					-  4 cycles 
#define SBC_B			0x98 // 98	 A -= B	  + CF			-  4 cycles 
#define SBC_C			0x99 // 99	 A -= C	  + CF			-  4 cycles 
#define SBC_D			0x9A // 9A	 A -= D	  + CF			-  4 cycles 
#define SBC_E			0x9B // 9B	 A -= E	  + CF			-  4 cycles 
#define SBC_H			0x9C // 9C	 A -= H	  + CF			-  4 cycles 
#define SBC_L			0x9D // 9D	 A -= L	  + CF			-  4 cycles 
#define SBC_xHL			0x9E // 9E	 A -=(HL) + CF			-  7 cycles 
#define SBC_A			0x9F // 9F	 A -= A	  + CF			-  4 cycles 

// begin A0 
#define AND_B			0xA0 // A0	 A &= B					-  4 cycles 
#define AND_C			0xA1 // A1	 A &= C					-  4 cycles 
#define AND_D			0xA2 // A2	 A &= D					-  4 cycles 
#define AND_E			0xA3 // A3	 A &= E					-  4 cycles 
#define AND_H			0xA4 // A4	 A &= H					-  4 cycles 
#define AND_L			0xA5 // A5	 A &= L					-  4 cycles 
#define AND_xHL			0xA6 // A6	 A &=(HL)				-  7 cycles 
#define AND_A			0xA7 // A7	 A &= A					-  4 cycles 
#define XOR_B			0xA8 // A8	 A ^= B					-  4 cycles 
#define XOR_C			0xA9 // A9	 A ^= C					-  4 cycles 
#define XOR_D			0xAA // AA	 A ^= D					-  4 cycles 
#define XOR_E			0xAB // AB	 A ^= E					-  4 cycles 
#define XOR_H			0xAC // AC	 A ^= H					-  4 cycles 
#define XOR_L			0xAD // AD	 A ^= L					-  4 cycles 
#define XOR_xHL			0xAE // AE	 A ^=(HL)				-  7 cycles 
#define XOR_A			0xAF // AF	 A ^= A					-  4 cycles 

// begin B0 
#define OR_B			0xB0 // B0	 A |= B					-  4 cycles 
#define OR_C			0xB1 // B1	 A |= C					-  4 cycles 
#define OR_D			0xB2 // B2	 A |= D					-  4 cycles 
#define OR_E			0xB3 // B3	 A |= E					-  4 cycles 
#define OR_H			0xB4 // B4	 A |= H					-  4 cycles 
#define OR_L			0xB5 // B5	 A |= L					-  4 cycles 
#define OR_xHL			0xB6 // B6	 A |=(HL)				-  7 cycles 
#define OR_A			0xB7 // B7	 A |= A					-  4 cycles 
#define CP_B			0xB8 // B8	 A !? B					-  4 cycles 
#define CP_C			0xB9 // B9	 A !? C					-  4 cycles 
#define CP_D			0xBA // BA	 A !? D					-  4 cycles 
#define CP_E			0xBB // BB	 A !? E					-  4 cycles 
#define CP_H			0xBC // BC	 A !? H					-  4 cycles 
#define CP_L			0xBD // BD	 A !? L					-  4 cycles 
#define CP_xHL			0xBE // BE	 A !?(HL)				-  7 cycles 
#define CP_A			0xBF // BF	 A !? A					-  4 cycles 

// begin C0 
#define RET_NZ			0xC0 // C0						!ZF -  5 cycles + hit -> 6 cycles 
#define POP_BC			0xC1 // C1	 BC =(SP), += 2			- 10 cycles 
#define JP_NZ			0xC2 // C2						!ZF - 10 cycles 
#define JP_X			0xC3 // C3					jmp		- 10 cycles 
#define CALL_NZ			0xC4 // C4						!ZF - 10 cycles + hit -> 7 cycles 
#define PUSH_BC			0xC5 // C5	   -= 2, (SP) = BC		- 10 cycles 
#define ADD_I08			0xC6 // C6	 A += imm8				-  7 cycles 
#define RST_00H			0xC7 // C7			   push PC, = 0 - 11 cycles 
#define RET_Z			0xC8 // C8						 ZF -  5 cycles + hit -> 6 cycles
#define RET_X			0xC9 // C9					ret		- 10 cycles 
#define JP_Z			0xCA // CA						 ZF - 10 cycles 
#define PREFIX_CB		0xCB // CB	 --- Bit !!! entry ---	-N/A cycles 
#define CALL_Z			0xCC // CC						 ZF - 10 cycles + hit -> 7 cycles 
#define CALL_X			0xCD // CD					call	- 17 cycles 
#define ADC_I08			0xCE // CE	 A += imm8	+ CF		-  7 cycles
#define RST_08H			0xCF // CF			   push PC, = 8 - 11 cycles

// begin D0 
#define RET_NC			0xD0 // D0						!CF -  5 cycles + hit -> 6 cycles 
#define POP_DE			0xD1 // D1	 DE =(SP), += 2			- 10 cycles 
#define JP_NC			0xD2 // D2						!CF - 10 cycles 
#define OUT_xI08		0xD3 // D3	 port[imm8] = A			- 11 cycles 
#define CALL_NC			0xD4 // D4						!CF - 10 cycles + hit -> 7 cycles 
#define PUSH_DE			0xD5 // D5	   -= 2, (SP) = DE		- 10 cycles 
#define SUB_I08			0xD6 // D6	 A -= imm8				-  7 cycles 
#define RST_10H			0xD7 // D7			   push PC, =16 - 11 cycles 
#define RET_C			0xD8 // D8						 CF -  5 cycles + hit -> 6 cycles
#define EXX				0xD9 // D9	 BC/DE/HL~= BC'/DE'/HL' -  4 cycles 
#define JP_C			0xDA // DA						 CF - 10 cycles 
#define IN_xI08			0xDB // DB	 A = port[imm8]			- 11 cycles 
#define CALL_C			0xDC // DC						 CF - 10 cycles + hit -> 7 cycles 
#define PREFIX_DD		0xDD // DD	 --- IX !!! entry ---	-N/A cycles 
#define SBC_I08			0xDE // DE	 A -= imm8	+ CF		-  7 cycles
#define RST_18H			0xDF // DF			   push PC, =24 - 11 cycles

// begin E0 
#define RET_PO			0xE0 // E0						!PF -  5 cycles + hit -> 6 cycles
#define POP_HL			0xE1 // E1	 HL =(SP), += 2			- 10 cycles 
#define JP_PO			0xE2 // E2						!PF - 10 cycles 
#define SWP_xSP_HL		0xE3 // E3 (SP)~= HL				- 19 cycles 
#define CALL_PO			0xE4 // E4						!PF - 10 cycles + hit -> 7 cycles
#define PUSH_HL			0xE5 // E5	   -= 2, (SP) = HL		- 10 cycles 
#define AND_I08			0xE6 // E6	 A &= imm8				-  7 cycles 
#define RST_20H			0xE7 // E7			push PC, PC =32 - 11 cycles 
#define RET_PE			0xE8 // E8						 PF -  5 cycles + hit -> 6 cycles
#define JP_HL			0xE9 // E9	 jmp  HL				- 10 cycles 
#define JP_PE			0xEA // EA						 PF - 10 cycles 
#define SWP_DE_HL		0xEB // EB	 DE~= HL				- 11 cycles 
#define CALL_PE			0xEC // EC						 PF - 10 cycles + hit -> 7 cycles 
#define PREFIX_ED		0xED // ED	 --- EXTD !!! entry --- -N/A cycles 
#define XOR_I08			0xEE // EE	 A ^= imm8				-  7 cycles
#define RST_28H			0xEF // EF			   push PC, =40 - 11 cycles

// begin F0 
#define RET_P			0xF0 // F0						!SF -  5 cycles + hit -> 6 cycles
#define POP_AF			0xF1 // F1	 AF =(SP), += 2			- 10 cycles 
#define JP_P			0xF2 // F2						!SF - 10 cycles 
#define DI				0xF3 // F3		  disable interrupt -  4 cycles 
#define CALL_P			0xF4 // F4						!SF - 10 cycles + hit -> 7 cycles
#define PUSH_AF			0xF5 // F5	   -= 2, (SP) = AF		- 10 cycles 
#define OR_I08			0xF6 // F6	 A |= imm8				-  7 cycles 
#define RST_30H			0xF7 // F7			   push PC, =48 - 11 cycles
#define RET_M			0xF8 // F8						 SF -  5 cycles + hit -> 6 cycles
#define HL_TO_SP		0xF9 // F9	 SP = HL				-  6 cycles 
#define JP_M			0xFA // FA						 SF - 10 cycles 
#define EI				0xFB // FB		  enable interrupt	- 11 cycles 
#define CALL_M			0xFC // FC						 SF - 10 cycles + hit -> 7 cycles 
#define PREFIX_FD		0xFD // FD	 --- IY !!! entry ---	-N/A cycles
#define CMP_I08			0xFE // FE	 A !? imm8				-  7 cycles
#define RST_38H			0xFF // FF			   push PC, =56 - 11 cycles

/*\ --------------------- PREFIX_CB --------------------- \*/ 
// begin 00 
#define RLC_B			0x00 // 00	 xROL					-  8 cycles 
#define RLC_C			0x01 // 01	 xROL					-  8 cycles 
#define RLC_D			0x02 // 02	 xROL					-  8 cycles 
#define RLC_E			0x03 // 03	 xROL					-  8 cycles 
#define RLC_H			0x04 // 04	 xROL					-  8 cycles 
#define RLC_L			0x05 // 05	 xROL					-  8 cycles 
#define RLC_xHL			0x06 // 06	 xROL					- 15 cycles 
#define RLC_A			0x07 // 07	 xROL					-  8 cycles 
#define RRC_B			0x08 // 08	 xROR					-  8 cycles 
#define RRC_C			0x09 // 09	 xROR					-  8 cycles 
#define RRC_D			0x0A // 0A	 xROR					-  8 cycles 
#define RRC_E			0x0B // 0B	 xROR					-  8 cycles 
#define RRC_H			0x0C // 0C	 xROR					-  8 cycles 
#define RRC_L			0x0D // 0D	 xROR					-  8 cycles 
#define RRC_xHL			0x0E // 0E	 xROR					- 15 cycles 
#define RRC_A			0x0F // 0F	 xROR					-  8 cycles 

// begin 10 
#define RL_B			0x10 // 10	 mROL					-  8 cycles 
#define RL_C			0x11 // 11	 mROL					-  8 cycles 
#define RL_D			0x12 // 12	 mROL					-  8 cycles 
#define RL_E			0x13 // 13	 mROL					-  8 cycles 
#define RL_H			0x14 // 14	 mROL					-  8 cycles 
#define RL_L			0x15 // 15	 mROL					-  8 cycles 
#define RL_xHL			0x16 // 16	 mROL					- 15 cycles 
#define RL_A			0x17 // 17	 mROL					-  8 cycles 
#define RR_B			0x18 // 18	 mROR					-  8 cycles 
#define RR_C			0x19 // 19	 mROR					-  8 cycles 
#define RR_D			0x1A // 1A	 mROR					-  8 cycles 
#define RR_E			0x1B // 1B	 mROR					-  8 cycles 
#define RR_H			0x1C // 1C	 mROR					-  8 cycles 
#define RR_L			0x1D // 1D	 mROR					-  8 cycles 
#define RR_xHL			0x1E // 1E	 mROR					- 15 cycles 
#define RR_A			0x1F // 1F	 mROR					-  8 cycles 

// begin 20
#define SLA_B			0x20 // 20	  ASL					-  8 cycles 
#define SLA_C			0x21 // 21	  ASL					-  8 cycles 
#define SLA_D			0x22 // 22	  ASL					-  8 cycles 
#define SLA_E			0x23 // 23	  ASL					-  8 cycles 
#define SLA_H			0x24 // 24	  ASL					-  8 cycles 
#define SLA_L			0x25 // 25	  ASL					-  8 cycles 
#define SLA_xHL			0x26 // 26	  ASL					- 15 cycles 
#define SLA_A			0x27 // 27	  ASL					-  8 cycles 
#define SRA_B			0x28 // 28	  LSR					-  8 cycles 
#define SRA_C			0x29 // 29	  LSR					-  8 cycles 
#define SRA_D			0x2A // 2A	  LSR					-  8 cycles 
#define SRA_E			0x2B // 2B	  LSR					-  8 cycles 
#define SRA_H			0x2C // 2C	  LSR					-  8 cycles 
#define SRA_L			0x2D // 2D	  LSR					-  8 cycles 
#define SRA_xHL			0x2E // 2E	  LSR					- 15 cycles 
#define SRA_A			0x2F // 2F	  LSR					-  8 cycles 

// begin 30
#define SLA_B			0x30 // 30	  ASL, LSB=1			-  8 cycles 
#define SLA_C			0x31 // 31	  ASL, LSB=1			-  8 cycles 
#define SLA_D			0x33 // 33	  ASL, LSB=1			-  8 cycles 
#define SLA_E			0x33 // 33	  ASL, LSB=1			-  8 cycles 
#define SLA_H			0x34 // 34	  ASL, LSB=1			-  8 cycles 
#define SLA_L			0x35 // 35	  ASL, LSB=1			-  8 cycles 
#define SLA_xHL			0x36 // 36	  ASL, LSB=1			- 15 cycles 
#define SLA_A			0x37 // 37	  ASL, LSB=1			-  8 cycles 
#define SRA_B			0x38 // 38	  LSR, MSB=1			-  8 cycles 
#define SRA_C			0x39 // 39	  LSR, MSB=1			-  8 cycles 
#define SRA_D			0x3A // 3A	  LSR, MSB=1			-  8 cycles 
#define SRA_E			0x3B // 3B	  LSR, MSB=1			-  8 cycles 
#define SRA_H			0x3C // 3C	  LSR, MSB=1			-  8 cycles 
#define SRA_L			0x3D // 3D	  LSR, MSB=1			-  8 cycles 
#define SRA_xHL			0x3E // 3E	  LSR, MSB=1			- 15 cycles 
#define SRA_A			0x3F // 3F	  LSR, MSB=1			-  8 cycles 

// begin 40 
#define BIT_B_0			0x40 // 40	  test d0 of B			-  8 cycles 
#define BIT_C_0			0x41 // 41	  test d0 of C			-  8 cycles 
#define BIT_D_0			0x42 // 42	  test d0 of D			-  8 cycles 
#define BIT_E_0			0x43 // 43	  test d0 of E			-  8 cycles 
#define BIT_H_0			0x44 // 44	  test d0 of H			-  8 cycles 
#define BIT_L_0			0x45 // 45	  test d0 of L			-  8 cycles 
#define BIT_xHL_0		0x46 // 46	  test d0 of(HL)		- 12 cycles 
#define BIT_A_0			0x47 // 47	  test d0 of A			-  8 cycles 
#define BIT_B_1			0x48 // 48	  test d1 of B			-  8 cycles 
#define BIT_C_1			0x49 // 49	  test d1 of C			-  8 cycles 
#define BIT_D_1			0x4A // 4A	  test d1 of D			-  8 cycles 
#define BIT_E_1			0x4B // 4B	  test d1 of E			-  8 cycles 
#define BIT_H_1			0x4C // 4C	  test d1 of H			-  8 cycles 
#define BIT_L_1			0x4D // 4D	  test d1 of L			-  8 cycles 
#define BIT_xHL_1		0x4E // 4E	  test d1 of(HL)		- 12 cycles 
#define BIT_A_1			0x4F // 4F	  test d1 of A			-  8 cycles 

// begin 50 
#define BIT_B_2			0x50 // 50	  test d2 of B			-  8 cycles 
#define BIT_C_2			0x51 // 51	  test d2 of C			-  8 cycles 
#define BIT_D_2			0x52 // 52	  test d2 of D			-  8 cycles 
#define BIT_E_2			0x53 // 53	  test d2 of E			-  8 cycles 
#define BIT_H_2			0x54 // 54	  test d2 of H			-  8 cycles 
#define BIT_L_2			0x55 // 55	  test d2 of L			-  8 cycles 
#define BIT_xHL_2		0x56 // 56	  test d2 of(HL)		- 12 cycles 
#define BIT_A_2			0x57 // 57	  test d2 of A			-  8 cycles 
#define BIT_B_3			0x58 // 58	  test d3 of B			-  8 cycles 
#define BIT_C_3			0x59 // 59	  test d3 of C			-  8 cycles 
#define BIT_D_3			0x5A // 5A	  test d3 of D			-  8 cycles 
#define BIT_E_3			0x5B // 5B	  test d3 of E			-  8 cycles 
#define BIT_H_3			0x5C // 5C	  test d3 of H			-  8 cycles 
#define BIT_L_3			0x5D // 5D	  test d3 of L			-  8 cycles 
#define BIT_xHL_3		0x5E // 5E	  test d3 of(HL)		- 12 cycles 
#define BIT_A_3			0x5F // 5F	  test d3 of A			-  8 cycles 

// begin 60 
#define BIT_B_4			0x60 // 60	  test d4 of B			-  8 cycles 
#define BIT_C_4			0x61 // 61	  test d4 of C			-  8 cycles 
#define BIT_D_4			0x62 // 62	  test d4 of D			-  8 cycles 
#define BIT_E_4			0x63 // 63	  test d4 of E			-  8 cycles 
#define BIT_H_4			0x64 // 64	  test d4 of H			-  8 cycles 
#define BIT_L_4			0x65 // 65	  test d4 of L			-  8 cycles 
#define BIT_xHL_4		0x66 // 66	  test d4 of(HL)		- 12 cycles 
#define BIT_A_4			0x67 // 67	  test d4 of A			-  8 cycles 
#define BIT_B_5			0x68 // 68	  test d5 of B			-  8 cycles 
#define BIT_C_5			0x69 // 69	  test d5 of C			-  8 cycles 
#define BIT_D_5			0x6A // 6A	  test d5 of D			-  8 cycles 
#define BIT_E_5			0x6B // 6B	  test d5 of E			-  8 cycles 
#define BIT_H_5			0x6C // 6C	  test d5 of H			-  8 cycles 
#define BIT_L_5			0x6D // 6D	  test d5 of L			-  8 cycles 
#define BIT_xHL_5		0x6E // 6E	  test d5 of(HL)		- 12 cycles 
#define BIT_A_5			0x6F // 6F	  test d5 of A			-  8 cycles 

// begin 70 
#define BIT_B_6			0x70 // 70	  test d6 of B			-  8 cycles 
#define BIT_C_6			0x71 // 71	  test d6 of C			-  8 cycles 
#define BIT_D_6			0x72 // 72	  test d6 of D			-  8 cycles 
#define BIT_E_6			0x73 // 73	  test d6 of E			-  8 cycles 
#define BIT_H_6			0x74 // 74	  test d6 of H			-  8 cycles 
#define BIT_L_6			0x75 // 75	  test d6 of L			-  8 cycles 
#define BIT_xHL_6		0x76 // 76	  test d6 of(HL)		- 12 cycles 
#define BIT_A_6			0x77 // 77	  test d6 of A			-  8 cycles 
#define BIT_B_7			0x78 // 78	  test d7 of B			-  8 cycles 
#define BIT_C_7			0x79 // 79	  test d7 of C			-  8 cycles 
#define BIT_D_7			0x7A // 7A	  test d7 of D			-  8 cycles 
#define BIT_E_7			0x7B // 7B	  test d7 of E			-  8 cycles 
#define BIT_H_7			0x7C // 7C	  test d7 of H			-  8 cycles 
#define BIT_L_7			0x7D // 7D	  test d7 of L			-  8 cycles 
#define BIT_xHL_7		0x7E // 7E	  test d7 of(HL)		- 12 cycles 
#define BIT_A_7			0x7F // 7F	  test d7 of A			-  8 cycles 

// begin 80 
#define RES_B_0			0x80 // 80	  reset d0 of B			-  8 cycles 
#define RES_C_0			0x81 // 81	  reset d0 of C			-  8 cycles 
#define RES_D_0			0x82 // 82	  reset d0 of D			-  8 cycles 
#define RES_E_0			0x83 // 83	  reset d0 of E			-  8 cycles 
#define RES_H_0			0x84 // 84	  reset d0 of H			-  8 cycles 
#define RES_L_0			0x85 // 85	  reset d0 of L			-  8 cycles 
#define RES_xHL_0		0x86 // 86	  reset d0 of(HL)		- 12 cycles 
#define RES_A_0			0x87 // 87	  reset d0 of A			-  8 cycles 
#define RES_B_1			0x88 // 88	  reset d1 of B			-  8 cycles 
#define RES_C_1			0x89 // 89	  reset d1 of C			-  8 cycles 
#define RES_D_1			0x8A // 8A	  reset d1 of D			-  8 cycles 
#define RES_E_1			0x8B // 8B	  reset d1 of E			-  8 cycles 
#define RES_H_1			0x8C // 8C	  reset d1 of H			-  8 cycles 
#define RES_L_1			0x8D // 8D	  reset d1 of L			-  8 cycles 
#define RES_xHL_1		0x8E // 8E	  reset d1 of(HL)		- 12 cycles 
#define RES_A_1			0x8F // 8F	  reset d1 of A			-  8 cycles 

// begin 90 
#define RES_B_2			0x90 // 90	  reset d2 of B			-  8 cycles 
#define RES_C_2			0x91 // 91	  reset d2 of C			-  8 cycles 
#define RES_D_2			0x92 // 92	  reset d2 of D			-  8 cycles 
#define RES_E_2			0x93 // 93	  reset d2 of E			-  8 cycles 
#define RES_H_2			0x94 // 94	  reset d2 of H			-  8 cycles 
#define RES_L_2			0x95 // 95	  reset d2 of L			-  8 cycles 
#define RES_xHL_2		0x96 // 96	  reset d2 of(HL)		- 12 cycles 
#define RES_A_2			0x97 // 97	  reset d2 of A			-  8 cycles 
#define RES_B_3			0x98 // 98	  reset d3 of B			-  8 cycles 
#define RES_C_3			0x99 // 99	  reset d3 of C			-  8 cycles 
#define RES_D_3			0x9A // 9A	  reset d3 of D			-  8 cycles 
#define RES_E_3			0x9B // 9B	  reset d3 of E			-  8 cycles 
#define RES_H_3			0x9C // 9C	  reset d3 of H			-  8 cycles 
#define RES_L_3			0x9D // 9D	  reset d3 of L			-  8 cycles 
#define RES_xHL_3		0x9E // 9E	  reset d3 of(HL)		- 12 cycles 
#define RES_A_3			0x9F // 9F	  reset d3 of A			-  8 cycles 

// begin A0 
#define RES_B_4			0xA0 // A0	  reset d4 of B			-  8 cycles 
#define RES_C_4			0xA1 // A1	  reset d4 of C			-  8 cycles 
#define RES_D_4			0xA2 // A2	  reset d4 of D			-  8 cycles 
#define RES_E_4			0xA3 // A3	  reset d4 of E			-  8 cycles 
#define RES_H_4			0xA4 // A4	  reset d4 of H			-  8 cycles 
#define RES_L_4			0xA5 // A5	  reset d4 of L			-  8 cycles 
#define RES_xHL_4		0xA6 // A6	  reset d4 of(HL)		- 12 cycles 
#define RES_A_4			0xA7 // A7	  reset d4 of A			-  8 cycles 
#define RES_B_5			0xA8 // A8	  reset d5 of B			-  8 cycles 
#define RES_C_5			0xA9 // A9	  reset d5 of C			-  8 cycles 
#define RES_D_5			0xAA // AA	  reset d5 of D			-  8 cycles 
#define RES_E_5			0xAB // AB	  reset d5 of E			-  8 cycles 
#define RES_H_5			0xAC // AC	  reset d5 of H			-  8 cycles 
#define RES_L_5			0xAD // AD	  reset d5 of L			-  8 cycles 
#define RES_xHL_5		0xAE // AE	  reset d5 of(HL)		- 12 cycles 
#define RES_A_5			0xAF // AF	  reset d5 of A			-  8 cycles 

// begin B0 
#define RES_B_6			0xB0 // B0	  reset d6 of B			-  8 cycles 
#define RES_C_6			0xB1 // B1	  reset d6 of C			-  8 cycles 
#define RES_D_6			0xB2 // B2	  reset d6 of D			-  8 cycles 
#define RES_E_6			0xB3 // B3	  reset d6 of E			-  8 cycles 
#define RES_H_6			0xB4 // B4	  reset d6 of H			-  8 cycles 
#define RES_L_6			0xB5 // B5	  reset d6 of L			-  8 cycles 
#define RES_xHL_6		0xB6 // B6	  reset d6 of(HL)		- 12 cycles 
#define RES_A_6			0xB7 // B7	  reset d6 of A			-  8 cycles 
#define RES_B_7			0xB8 // B8	  reset d7 of B			-  8 cycles 
#define RES_C_7			0xB9 // B9	  reset d7 of C			-  8 cycles 
#define RES_D_7			0xBA // BA	  reset d7 of D			-  8 cycles 
#define RES_E_7			0xBB // BB	  reset d7 of E			-  8 cycles 
#define RES_H_7			0xBC // BC	  reset d7 of H			-  8 cycles 
#define RES_L_7			0xBD // BD	  reset d7 of L			-  8 cycles 
#define RES_xHL_7		0xBE // BE	  reset d7 of(HL)		- 12 cycles 
#define RES_A_7			0xBF // BF	  reset d7 of A			-  8 cycles

// Begin C0 
#define SET_B_0			0xC0 // C0		set d0 of B			-  8 cycles 
#define SET_C_0			0xC1 // C1		set d0 of C			-  8 cycles 
#define SET_D_0			0xC2 // C2		set d0 of D			-  8 cycles 
#define SET_E_0			0xC3 // C3		set d0 of E			-  8 cycles 
#define SET_H_0			0xC4 // C4		set d0 of H			-  8 cycles 
#define SET_L_0			0xC5 // C5		set d0 of L			-  8 cycles 
#define SET_xHL_0		0xC6 // C6		set d0 of(HL)		- 12 cycles 
#define SET_A_0			0xC7 // C7		set d0 of A			-  8 cycles 
#define SET_B_1			0xC8 // C8		set d1 of B			-  8 cycles 
#define SET_C_1			0xC9 // C9		set d1 of C			-  8 cycles 
#define SET_D_1			0xCA // CA		set d1 of D			-  8 cycles 
#define SET_E_1			0xCB // CB		set d1 of E			-  8 cycles 
#define SET_H_1			0xCC // CC		set d1 of H			-  8 cycles 
#define SET_L_1			0xCD // CD		set d1 of L			-  8 cycles 
#define SET_xHL_1		0xCE // CE		set d1 of(HL)		- 12 cycles 
#define SET_A_1			0xCF // CF		set d1 of A			-  8 cycles 

// Begin D0 
#define SET_B_2			0xD0 // D0		set d2 of B			-  8 cycles 
#define SET_C_2			0xD1 // D1		set d2 of C			-  8 cycles 
#define SET_D_2			0xD2 // D2		set d2 of D			-  8 cycles 
#define SET_E_2			0xD3 // D3		set d2 of E			-  8 cycles 
#define SET_H_2			0xD4 // D4		set d2 of H			-  8 cycles 
#define SET_L_2			0xD5 // D5		set d2 of L			-  8 cycles 
#define SET_xHL_2		0xD6 // D6		set d2 of(HL)		- 12 cycles 
#define SET_A_2			0xD7 // D7		set d2 of A			-  8 cycles 
#define SET_B_3			0xD8 // D8		set d3 of B			-  8 cycles 
#define SET_C_3			0xD9 // D9		set d3 of C			-  8 cycles 
#define SET_D_3			0xDA // DA		set d3 of D			-  8 cycles 
#define SET_E_3			0xDB // DB		set d3 of E			-  8 cycles 
#define SET_H_3			0xDC // DC		set d3 of H			-  8 cycles 
#define SET_L_3			0xDD // DD		set d3 of L			-  8 cycles 
#define SET_xHL_3		0xDE // DE		set d3 of(HL)		- 12 cycles 
#define SET_A_3			0xDF // DF		set d3 of A			-  8 cycles 

// Begin E0 
#define SET_B_4			0xE0 // E0		set d4 of B			-  8 cycles 
#define SET_C_4			0xE1 // E1		set d4 of C			-  8 cycles 
#define SET_D_4			0xE2 // E2		set d4 of D			-  8 cycles 
#define SET_E_4			0xE3 // E3		set d4 of E			-  8 cycles 
#define SET_H_4			0xE4 // E4		set d4 of H			-  8 cycles 
#define SET_L_4			0xE5 // E5		set d4 of L			-  8 cycles 
#define SET_xHL_4		0xE6 // E6		set d4 of(HL)		- 12 cycles 
#define SET_A_4			0xE7 // E7		set d4 of A			-  8 cycles 
#define SET_B_5			0xE8 // E8		set d5 of B			-  8 cycles 
#define SET_C_5			0xE9 // E9		set d5 of C			-  8 cycles 
#define SET_D_5			0xEA // EA		set d5 of D			-  8 cycles 
#define SET_E_5			0xEB // EB		set d5 of E			-  8 cycles 
#define SET_H_5			0xEC // EC		set d5 of H			-  8 cycles 
#define SET_L_5			0xED // ED		set d5 of L			-  8 cycles 
#define SET_xHL_5		0xEE // EE		set d5 of(HL)		- 12 cycles 
#define SET_A_5			0xEF // EF		set d5 of A			-  8 cycles 

// Begin F0 
#define SET_B_6			0xF0 // F0		set d6 of B			-  8 cycles 
#define SET_C_6			0xF1 // F1		set d6 of C			-  8 cycles 
#define SET_D_6			0xF2 // F2		set d6 of D			-  8 cycles 
#define SET_E_6			0xF3 // F3		set d6 of E			-  8 cycles 
#define SET_H_6			0xF4 // F4		set d6 of H			-  8 cycles 
#define SET_L_6			0xF5 // F5		set d6 of L			-  8 cycles 
#define SET_xHL_6		0xF6 // F6		set d6 of(HL)		- 12 cycles 
#define SET_A_6			0xF7 // F7		set d6 of A			-  8 cycles 
#define SET_B_7			0xF8 // F8		set d7 of B			-  8 cycles 
#define SET_C_7			0xF9 // F9		set d7 of C			-  8 cycles 
#define SET_D_7			0xFA // FA		set d7 of D			-  8 cycles 
#define SET_E_7			0xFB // FB		set d7 of E			-  8 cycles 
#define SET_H_7			0xFC // FC		set d7 of H			-  8 cycles 
#define SET_L_7			0xFD // FD		set d7 of L			-  8 cycles 
#define SET_xHL_7		0xFE // FE		set d7 of(HL)		- 12 cycles 
#define SET_A_7			0xFF // FF		set d7 of A			-  8 cycles

/*\ ------------------- PREFIX_DD~FD -------------------- \*/ 
#define ADD_IS_BC		0x09 // 09		is += bc			- 15 cycles 
#define ADD_IS_DE		0x19 // 19		is += de			- 15 cycles 
#define I16_TO_IS		0x21 // 21		is	= imm16			- 14 cycles 
#define IS_TO_xI16		0x22 // 22	 (imm16)= is			- 20 cycles 
#define INC_IS			0x23 // 23		is ++				- 10 cycles 
#define INC_ISH			0x24 // 24		isH++				-  8 cycles 
#define DEC_ISH			0x25 // 25		isH--				-  8 cycles 
#define I08_TO_ISH		0x26 // 26		isH	 = imm8			- 11 cycles 
#define ADD_IS_IS		0x29 // 29		is += is			- 15 cycles 
#define xI16_TO_IS		0x2A // 2A		is	=(imm16)		- 20 cycles 
#define DEC_IS			0x2B // 2B		is --				- 10 cycles 
#define INC_ISL			0x2C // 2C		isL++				-  8 cycles 
#define DEC_ISL			0x2D // 2D		isL--				-  8 cycles 
#define I08_TO_ISL		0x2E // 2E		isL = imm8			- 11 cycles
#define INC_xISwI08		0x34 // 34	   (is	+ imm8) ++		- 23 cycles 
#define DEC_xISwI08		0x35 // 35	   (is	+ imm8) --		- 23 cycles
/* first byte is offset, second is imm8 which will be written */
#define I08_TO_xISwI08	0x36 // 36	   (is	+ imm8) = imm8	- 19 cycles 
#define ADD_IS_SP		0x39 // 39		is += sp			- 15 cycles 
#define ISH_TO_B		0x44 // 44		b	= isH			-  8 cycles 
#define ISL_TO_B		0x45 // 45		b	= isL			-  8 cycles 
#define xISwI08_TO_B	0x46 // 46		b	=(is  + imm8)	- 19 cycles 
#define ISH_TO_C		0x4C // 4C		c	= isH			-  8 cycles 
#define ISL_TO_C		0x4D // 4D		c	= isL			-  8 cycles 
#define xISwI08_TO_C	0x4E // 4E		c	=(is  + imm8)	- 19 cycles
#define ISH_TO_D		0x54 // 54		d	= isH			-  8 cycles 
#define ISL_TO_D		0x55 // 55		d	= isL			-  8 cycles 
#define xISwI08_TO_D	0x56 // 56		d	=(is  + imm8)	- 19 cycles 
#define ISH_TO_E		0x5C // 5C		e	= isH			-  8 cycles 
#define ISL_TO_E		0x5D // 5D		e	= isL			-  8 cycles 
#define xISwI08_TO_E	0x5E // 5E		e	=(is  + imm8)	- 19 cycles
#define B_TO_ISH		0x60 // 60		isH = b				-  8 cycles 
#define C_TO_ISH		0x61 // 61		isH = c				-  8 cycles 
#define D_TO_ISH		0x62 // 62		isH = d				-  8 cycles 
#define E_TO_ISH		0x63 // 63		isH = e				-  8 cycles
#define H_TO_ISH		0x64 // 64		isH = h				-  8 cycles 
#define L_TO_ISH		0x65 // 65		isH = l				-  8 cycles
#define xISwI08_TO_H	0x66 // 66		h	=(is  + imm8)	- 19 cycles 
#define A_TO_ISH		0x67 // 67		isH = a				-  8 cycles
#define B_TO_ISL		0x68 // 68		isL = b				-  8 cycles 
#define C_TO_ISL		0x69 // 69		isL = c				-  8 cycles 
#define D_TO_ISL		0x6A // 6A		isL = d				-  8 cycles 
#define E_TO_ISL		0x6B // 6B		isL = e				-  8 cycles
#define H_TO_ISL		0x6C // 6C		isL = h				-  8 cycles 
#define L_TO_ISL		0x6D // 6D		isL = l				-  8 cycles
#define xISwI08_TO_L	0x6E // 6E		l	=(is  + imm8)	- 19 cycles 
#define A_TO_ISL		0x6F // 6F		isL = a				-  8 cycles
#define B_TO_xISwI08	0x70 // 70	(is	 + imm8) = b		- 19 cycles 
#define C_TO_xISwI08	0x71 // 71	(is	 + imm8) = c		- 19 cycles	 
#define D_TO_xISwI08	0x72 // 72	(is	 + imm8) = d		- 19 cycles 
#define E_TO_xISwI08	0x73 // 73	(is	 + imm8) = e		- 19 cycles 
#define H_TO_xISwI08	0x74 // 74	(is	 + imm8) = h		- 19 cycles 
#define L_TO_xISwI08	0x75 // 75	(is	 + imm8) = l		- 19 cycles 
#define A_TO_xISwI08	0x77 // 77	(is	 + imm8) = a		- 19 cycles 
#define ISH_TO_A		0x7C // 7C		a	= isH			-  8 cycles 
#define ISL_TO_A		0x7D // 7D		a	= isL			-  8 cycles 
#define xISwI08_TO_A	0x7E // 7E		a	=(is  + imm8)	- 19 cycles
#define ADD_ISH			0x84 // 84 
#define ADD_ISL			0x85 // 85 
#define ADD_xISwI08		0x86 // 86 
#define ADC_ISH			0x8C // 84 
#define ADC_ISL			0x8D // 85 
#define ADC_xISwI08		0x8E // 86 
#define SUB_ISH			0x94 // 94 
#define SUB_ISL			0x95 // 95 
#define SUB_xISwI08		0x96 // 96 
#define SBC_ISH			0x9C // 94 
#define SBC_ISL			0x9D // 95 
#define SBC_xISwI08		0x9E // 96
#define AND_ISH			0xA4 // A4 
#define AND_ISL			0xA5 // A5 
#define AND_xISwI08		0xA6 // A6 
#define XOR_ISH			0xAC // A4 
#define XOR_ISL			0xAD // A5 
#define XOR_xISwI08		0xAE // A6 
#define OR_ISH			0xB4 // B4 
#define OR_ISL			0xB5 // B5 
#define OR_xISwI08		0xB6 // B6 
#define CP_ISH			0xBC // B4 
#define CP_ISL			0xBD // B5 
#define CP_xISwI08		0xBE // B6 
#define POP_IS			0xE1 // E1 
#define EX_xSP_IS		0xE3 // E3 
#define PUSH_IS			0xE5 // E5 
#define JP_IS			0xE9 // E9 
#define IS_TO_SP		0xF9 // F9 

/*\ ----------------- PREFIX_DD~FD CB Cycles error 
all index bit instruction is 20 cycles 
other is 23 cycles 
------------------ \*/ 
// begin 00 
#define IS_RLC_B		0x00 // 00	 xROL					-  8 cycles 
#define IS_RLC_C		0x01 // 01	 xROL					-  8 cycles 
#define IS_RLC_D		0x02 // 02	 xROL					-  8 cycles 
#define IS_RLC_E		0x03 // 03	 xROL					-  8 cycles 
#define IS_RLC_H		0x04 // 04	 xROL					-  8 cycles 
#define IS_RLC_L		0x05 // 05	 xROL					-  8 cycles 
#define IS_RLC_xHL		0x06 // 06	 xROL					- 15 cycles 
#define IS_RLC_A		0x07 // 07	 xROL					-  8 cycles 
#define IS_RRC_B		0x08 // 08	 xROR					-  8 cycles 
#define IS_RRC_C		0x09 // 09	 xROR					-  8 cycles 
#define IS_RRC_D		0x0A // 0A	 xROR					-  8 cycles 
#define IS_RRC_E		0x0B // 0B	 xROR					-  8 cycles 
#define IS_RRC_H		0x0C // 0C	 xROR					-  8 cycles 
#define IS_RRC_L		0x0D // 0D	 xROR					-  8 cycles 
#define IS_RRC_xHL		0x0E // 0E	 xROR					- 15 cycles 
#define IS_RRC_A		0x0F // 0F	 xROR					-  8 cycles 

// begin 10 
#define IS_RL_B			0x10 // 10	 mROL					-  8 cycles 
#define IS_RL_C			0x11 // 11	 mROL					-  8 cycles 
#define IS_RL_D			0x12 // 12	 mROL					-  8 cycles 
#define IS_RL_E			0x13 // 13	 mROL					-  8 cycles 
#define IS_RL_H			0x14 // 14	 mROL					-  8 cycles 
#define IS_RL_L			0x15 // 15	 mROL					-  8 cycles 
#define IS_RL_xHL		0x16 // 16	 mROL					- 15 cycles 
#define IS_RL_A			0x17 // 17	 mROL					-  8 cycles 
#define IS_RR_B			0x18 // 18	 mROR					-  8 cycles 
#define IS_RR_C			0x19 // 19	 mROR					-  8 cycles 
#define IS_RR_D			0x1A // 1A	 mROR					-  8 cycles 
#define IS_RR_E			0x1B // 1B	 mROR					-  8 cycles 
#define IS_RR_H			0x1C // 1C	 mROR					-  8 cycles 
#define IS_RR_L			0x1D // 1D	 mROR					-  8 cycles 
#define IS_RR_xHL		0x1E // 1E	 mROR					- 15 cycles 
#define IS_RR_A			0x1F // 1F	 mROR					-  8 cycles 

// begin 20
#define IS_SLA_B		0x20 // 20	  ASL					-  8 cycles 
#define IS_SLA_C		0x21 // 21	  ASL					-  8 cycles 
#define IS_SLA_D		0x22 // 22	  ASL					-  8 cycles 
#define IS_SLA_E		0x23 // 23	  ASL					-  8 cycles 
#define IS_SLA_H		0x24 // 24	  ASL					-  8 cycles 
#define IS_SLA_L		0x25 // 25	  ASL					-  8 cycles 
#define IS_SLA_xHL		0x26 // 26	  ASL					- 15 cycles 
#define IS_SLA_A		0x27 // 27	  ASL					-  8 cycles 
#define IS_SRA_B		0x28 // 28	  LSR					-  8 cycles 
#define IS_SRA_C		0x29 // 29	  LSR					-  8 cycles 
#define IS_SRA_D		0x2A // 2A	  LSR					-  8 cycles 
#define IS_SRA_E		0x2B // 2B	  LSR					-  8 cycles 
#define IS_SRA_H		0x2C // 2C	  LSR					-  8 cycles 
#define IS_SRA_L		0x2D // 2D	  LSR					-  8 cycles 
#define IS_SRA_xHL		0x2E // 2E	  LSR					- 15 cycles 
#define IS_SRA_A		0x2F // 2F	  LSR					-  8 cycles 

// begin 30
#define IS_SLA_B		0x30 // 30	  ASL, LSB=1			-  8 cycles 
#define IS_SLA_C		0x31 // 31	  ASL, LSB=1			-  8 cycles 
#define IS_SLA_D		0x33 // 33	  ASL, LSB=1			-  8 cycles 
#define IS_SLA_E		0x33 // 33	  ASL, LSB=1			-  8 cycles 
#define IS_SLA_H		0x34 // 34	  ASL, LSB=1			-  8 cycles 
#define IS_SLA_L		0x35 // 35	  ASL, LSB=1			-  8 cycles 
#define IS_SLA_xHL		0x36 // 36	  ASL, LSB=1			- 15 cycles 
#define IS_SLA_A		0x37 // 37	  ASL, LSB=1			-  8 cycles 
#define IS_SRA_B		0x38 // 38	  LSR, MSB=1			-  8 cycles 
#define IS_SRA_C		0x39 // 39	  LSR, MSB=1			-  8 cycles 
#define IS_SRA_D		0x3A // 3A	  LSR, MSB=1			-  8 cycles 
#define IS_SRA_E		0x3B // 3B	  LSR, MSB=1			-  8 cycles 
#define IS_SRA_H		0x3C // 3C	  LSR, MSB=1			-  8 cycles 
#define IS_SRA_L		0x3D // 3D	  LSR, MSB=1			-  8 cycles 
#define IS_SRA_xHL		0x3E // 3E	  LSR, MSB=1			- 15 cycles 
#define IS_SRA_A		0x3F // 3F	  LSR, MSB=1			-  8 cycles 

// begin 40 
#define IS_BIT_B_0		0x40 // 40	  test d0 of B			-  8 cycles 
#define IS_BIT_C_0		0x41 // 41	  test d0 of C			-  8 cycles 
#define IS_BIT_D_0		0x42 // 42	  test d0 of D			-  8 cycles 
#define IS_BIT_E_0		0x43 // 43	  test d0 of E			-  8 cycles 
#define IS_BIT_H_0		0x44 // 44	  test d0 of H			-  8 cycles 
#define IS_BIT_L_0		0x45 // 45	  test d0 of L			-  8 cycles 
#define IS_BIT_xHL_0	0x46 // 46	  test d0 of(HL)		- 12 cycles 
#define IS_BIT_A_0		0x47 // 47	  test d0 of A			-  8 cycles 
#define IS_BIT_B_1		0x48 // 48	  test d1 of B			-  8 cycles 
#define IS_BIT_C_1		0x49 // 49	  test d1 of C			-  8 cycles 
#define IS_BIT_D_1		0x4A // 4A	  test d1 of D			-  8 cycles 
#define IS_BIT_E_1		0x4B // 4B	  test d1 of E			-  8 cycles 
#define IS_BIT_H_1		0x4C // 4C	  test d1 of H			-  8 cycles 
#define IS_BIT_L_1		0x4D // 4D	  test d1 of L			-  8 cycles 
#define IS_BIT_xHL_1	0x4E // 4E	  test d1 of(HL)		- 12 cycles 
#define IS_BIT_A_1		0x4F // 4F	  test d1 of A			-  8 cycles 

// begin 50 
#define IS_BIT_B_2		0x50 // 50	  test d2 of B			-  8 cycles 
#define IS_BIT_C_2		0x51 // 51	  test d2 of C			-  8 cycles 
#define IS_BIT_D_2		0x52 // 52	  test d2 of D			-  8 cycles 
#define IS_BIT_E_2		0x53 // 53	  test d2 of E			-  8 cycles 
#define IS_BIT_H_2		0x54 // 54	  test d2 of H			-  8 cycles 
#define IS_BIT_L_2		0x55 // 55	  test d2 of L			-  8 cycles 
#define IS_BIT_xHL_2	0x56 // 56	  test d2 of(HL)		- 12 cycles 
#define IS_BIT_A_2		0x57 // 57	  test d2 of A			-  8 cycles 
#define IS_BIT_B_3		0x58 // 58	  test d3 of B			-  8 cycles 
#define IS_BIT_C_3		0x59 // 59	  test d3 of C			-  8 cycles 
#define IS_BIT_D_3		0x5A // 5A	  test d3 of D			-  8 cycles 
#define IS_BIT_E_3		0x5B // 5B	  test d3 of E			-  8 cycles 
#define IS_BIT_H_3		0x5C // 5C	  test d3 of H			-  8 cycles 
#define IS_BIT_L_3		0x5D // 5D	  test d3 of L			-  8 cycles 
#define IS_BIT_xHL_3	0x5E // 5E	  test d3 of(HL)		- 12 cycles 
#define IS_BIT_A_3		0x5F // 5F	  test d3 of A			-  8 cycles 

// begin 60 
#define IS_BIT_B_4		0x60 // 60	  test d4 of B			-  8 cycles 
#define IS_BIT_C_4		0x61 // 61	  test d4 of C			-  8 cycles 
#define IS_BIT_D_4		0x62 // 62	  test d4 of D			-  8 cycles 
#define IS_BIT_E_4		0x63 // 63	  test d4 of E			-  8 cycles 
#define IS_BIT_H_4		0x64 // 64	  test d4 of H			-  8 cycles 
#define IS_BIT_L_4		0x65 // 65	  test d4 of L			-  8 cycles 
#define IS_BIT_xHL_4	0x66 // 66	  test d4 of(HL)		- 12 cycles 
#define IS_BIT_A_4		0x67 // 67	  test d4 of A			-  8 cycles 
#define IS_BIT_B_5		0x68 // 68	  test d5 of B			-  8 cycles 
#define IS_BIT_C_5		0x69 // 69	  test d5 of C			-  8 cycles 
#define IS_BIT_D_5		0x6A // 6A	  test d5 of D			-  8 cycles 
#define IS_BIT_E_5		0x6B // 6B	  test d5 of E			-  8 cycles 
#define IS_BIT_H_5		0x6C // 6C	  test d5 of H			-  8 cycles 
#define IS_BIT_L_5		0x6D // 6D	  test d5 of L			-  8 cycles 
#define IS_BIT_xHL_5	0x6E // 6E	  test d5 of(HL)		- 12 cycles 
#define IS_BIT_A_5		0x6F // 6F	  test d5 of A			-  8 cycles 

// begin 70 
#define IS_BIT_B_6		0x70 // 70	  test d6 of B			-  8 cycles 
#define IS_BIT_C_6		0x71 // 71	  test d6 of C			-  8 cycles 
#define IS_BIT_D_6		0x72 // 72	  test d6 of D			-  8 cycles 
#define IS_BIT_E_6		0x73 // 73	  test d6 of E			-  8 cycles 
#define IS_BIT_H_6		0x74 // 74	  test d6 of H			-  8 cycles 
#define IS_BIT_L_6		0x75 // 75	  test d6 of L			-  8 cycles 
#define IS_BIT_xHL_6	0x76 // 76	  test d6 of(HL)		- 12 cycles 
#define IS_BIT_A_6		0x77 // 77	  test d6 of A			-  8 cycles 
#define IS_BIT_B_7		0x78 // 78	  test d7 of B			-  8 cycles 
#define IS_BIT_C_7		0x79 // 79	  test d7 of C			-  8 cycles 
#define IS_BIT_D_7		0x7A // 7A	  test d7 of D			-  8 cycles 
#define IS_BIT_E_7		0x7B // 7B	  test d7 of E			-  8 cycles 
#define IS_BIT_H_7		0x7C // 7C	  test d7 of H			-  8 cycles 
#define IS_BIT_L_7		0x7D // 7D	  test d7 of L			-  8 cycles 
#define IS_BIT_xHL_7	0x7E // 7E	  test d7 of(HL)		- 12 cycles 
#define IS_BIT_A_7		0x7F // 7F	  test d7 of A			-  8 cycles 

// begin 80 
#define IS_RES_B_0		0x80 // 80	  reset d0 of B			-  8 cycles 
#define IS_RES_C_0		0x81 // 81	  reset d0 of C			-  8 cycles 
#define IS_RES_D_0		0x82 // 82	  reset d0 of D			-  8 cycles 
#define IS_RES_E_0		0x83 // 83	  reset d0 of E			-  8 cycles 
#define IS_RES_H_0		0x84 // 84	  reset d0 of H			-  8 cycles 
#define IS_RES_L_0		0x85 // 85	  reset d0 of L			-  8 cycles 
#define IS_RES_xHL_0	0x86 // 86	  reset d0 of(HL)		- 12 cycles 
#define IS_RES_A_0		0x87 // 87	  reset d0 of A			-  8 cycles 
#define IS_RES_B_1		0x88 // 88	  reset d1 of B			-  8 cycles 
#define IS_RES_C_1		0x89 // 89	  reset d1 of C			-  8 cycles 
#define IS_RES_D_1		0x8A // 8A	  reset d1 of D			-  8 cycles 
#define IS_RES_E_1		0x8B // 8B	  reset d1 of E			-  8 cycles 
#define IS_RES_H_1		0x8C // 8C	  reset d1 of H			-  8 cycles 
#define IS_RES_L_1		0x8D // 8D	  reset d1 of L			-  8 cycles 
#define IS_RES_xHL_1	0x8E // 8E	  reset d1 of(HL)		- 12 cycles 
#define IS_RES_A_1		0x8F // 8F	  reset d1 of A			-  8 cycles 

// begin 90 
#define IS_RES_B_2		0x90 // 90	  reset d2 of B			-  8 cycles 
#define IS_RES_C_2		0x91 // 91	  reset d2 of C			-  8 cycles 
#define IS_RES_D_2		0x92 // 92	  reset d2 of D			-  8 cycles 
#define IS_RES_E_2		0x93 // 93	  reset d2 of E			-  8 cycles 
#define IS_RES_H_2		0x94 // 94	  reset d2 of H			-  8 cycles 
#define IS_RES_L_2		0x95 // 95	  reset d2 of L			-  8 cycles 
#define IS_RES_xHL_2	0x96 // 96	  reset d2 of(HL)		- 12 cycles 
#define IS_RES_A_2		0x97 // 97	  reset d2 of A			-  8 cycles 
#define IS_RES_B_3		0x98 // 98	  reset d3 of B			-  8 cycles 
#define IS_RES_C_3		0x99 // 99	  reset d3 of C			-  8 cycles 
#define IS_RES_D_3		0x9A // 9A	  reset d3 of D			-  8 cycles 
#define IS_RES_E_3		0x9B // 9B	  reset d3 of E			-  8 cycles 
#define IS_RES_H_3		0x9C // 9C	  reset d3 of H			-  8 cycles 
#define IS_RES_L_3		0x9D // 9D	  reset d3 of L			-  8 cycles 
#define IS_RES_xHL_3	0x9E // 9E	  reset d3 of(HL)		- 12 cycles 
#define IS_RES_A_3		0x9F // 9F	  reset d3 of A			-  8 cycles 

// begin A0 
#define IS_RES_B_4		0xA0 // A0	  reset d4 of B			-  8 cycles 
#define IS_RES_C_4		0xA1 // A1	  reset d4 of C			-  8 cycles 
#define IS_RES_D_4		0xA2 // A2	  reset d4 of D			-  8 cycles 
#define IS_RES_E_4		0xA3 // A3	  reset d4 of E			-  8 cycles 
#define IS_RES_H_4		0xA4 // A4	  reset d4 of H			-  8 cycles 
#define IS_RES_L_4		0xA5 // A5	  reset d4 of L			-  8 cycles 
#define IS_RES_xHL_4	0xA6 // A6	  reset d4 of(HL)		- 12 cycles 
#define IS_RES_A_4		0xA7 // A7	  reset d4 of A			-  8 cycles 
#define IS_RES_B_5		0xA8 // A8	  reset d5 of B			-  8 cycles 
#define IS_RES_C_5		0xA9 // A9	  reset d5 of C			-  8 cycles 
#define IS_RES_D_5		0xAA // AA	  reset d5 of D			-  8 cycles 
#define IS_RES_E_5		0xAB // AB	  reset d5 of E			-  8 cycles 
#define IS_RES_H_5		0xAC // AC	  reset d5 of H			-  8 cycles 
#define IS_RES_L_5		0xAD // AD	  reset d5 of L			-  8 cycles 
#define IS_RES_xHL_5	0xAE // AE	  reset d5 of(HL)		- 12 cycles 
#define IS_RES_A_5		0xAF // AF	  reset d5 of A			-  8 cycles 

// begin B0 
#define IS_RES_B_6		0xB0 // B0	  reset d6 of B			-  8 cycles 
#define IS_RES_C_6		0xB1 // B1	  reset d6 of C			-  8 cycles 
#define IS_RES_D_6		0xB2 // B2	  reset d6 of D			-  8 cycles 
#define IS_RES_E_6		0xB3 // B3	  reset d6 of E			-  8 cycles 
#define IS_RES_H_6		0xB4 // B4	  reset d6 of H			-  8 cycles 
#define IS_RES_L_6		0xB5 // B5	  reset d6 of L			-  8 cycles 
#define IS_RES_xHL_6	0xB6 // B6	  reset d6 of(HL)		- 12 cycles 
#define IS_RES_A_6		0xB7 // B7	  reset d6 of A			-  8 cycles 
#define IS_RES_B_7		0xB8 // B8	  reset d7 of B			-  8 cycles 
#define IS_RES_C_7		0xB9 // B9	  reset d7 of C			-  8 cycles 
#define IS_RES_D_7		0xBA // BA	  reset d7 of D			-  8 cycles 
#define IS_RES_E_7		0xBB // BB	  reset d7 of E			-  8 cycles 
#define IS_RES_H_7		0xBC // BC	  reset d7 of H			-  8 cycles 
#define IS_RES_L_7		0xBD // BD	  reset d7 of L			-  8 cycles 
#define IS_RES_xHL_7	0xBE // BE	  reset d7 of(HL)		- 12 cycles 
#define IS_RES_A_7		0xBF // BF	  reset d7 of A			-  8 cycles

// Begin C0 
#define IS_SET_B_0		0xC0 // C0		set d0 of B			-  8 cycles 
#define IS_SET_C_0		0xC1 // C1		set d0 of C			-  8 cycles 
#define IS_SET_D_0		0xC2 // C2		set d0 of D			-  8 cycles 
#define IS_SET_E_0		0xC3 // C3		set d0 of E			-  8 cycles 
#define IS_SET_H_0		0xC4 // C4		set d0 of H			-  8 cycles 
#define IS_SET_L_0		0xC5 // C5		set d0 of L			-  8 cycles 
#define IS_SET_xHL_0	0xC6 // C6		set d0 of(HL)		- 12 cycles 
#define IS_SET_A_0		0xC7 // C7		set d0 of A			-  8 cycles 
#define IS_SET_B_1		0xC8 // C8		set d1 of B			-  8 cycles 
#define IS_SET_C_1		0xC9 // C9		set d1 of C			-  8 cycles 
#define IS_SET_D_1		0xCA // CA		set d1 of D			-  8 cycles 
#define IS_SET_E_1		0xCB // CB		set d1 of E			-  8 cycles 
#define IS_SET_H_1		0xCC // CC		set d1 of H			-  8 cycles 
#define IS_SET_L_1		0xCD // CD		set d1 of L			-  8 cycles 
#define IS_SET_xHL_1	0xCE // CE		set d1 of(HL)		- 12 cycles 
#define IS_SET_A_1		0xCF // CF		set d1 of A			-  8 cycles 

// Begin D0 
#define IS_SET_B_2		0xD0 // D0		set d2 of B			-  8 cycles 
#define IS_SET_C_2		0xD1 // D1		set d2 of C			-  8 cycles 
#define IS_SET_D_2		0xD2 // D2		set d2 of D			-  8 cycles 
#define IS_SET_E_2		0xD3 // D3		set d2 of E			-  8 cycles 
#define IS_SET_H_2		0xD4 // D4		set d2 of H			-  8 cycles 
#define IS_SET_L_2		0xD5 // D5		set d2 of L			-  8 cycles 
#define IS_SET_xHL_2	0xD6 // D6		set d2 of(HL)		- 12 cycles 
#define IS_SET_A_2		0xD7 // D7		set d2 of A			-  8 cycles 
#define IS_SET_B_3		0xD8 // D8		set d3 of B			-  8 cycles 
#define IS_SET_C_3		0xD9 // D9		set d3 of C			-  8 cycles 
#define IS_SET_D_3		0xDA // DA		set d3 of D			-  8 cycles 
#define IS_SET_E_3		0xDB // DB		set d3 of E			-  8 cycles 
#define IS_SET_H_3		0xDC // DC		set d3 of H			-  8 cycles 
#define IS_SET_L_3		0xDD // DD		set d3 of L			-  8 cycles 
#define IS_SET_xHL_3	0xDE // DE		set d3 of(HL)		- 12 cycles 
#define IS_SET_A_3		0xDF // DF		set d3 of A			-  8 cycles 

// Begin E0 
#define IS_SET_B_4		0xE0 // E0		set d4 of B			-  8 cycles 
#define IS_SET_C_4		0xE1 // E1		set d4 of C			-  8 cycles 
#define IS_SET_D_4		0xE2 // E2		set d4 of D			-  8 cycles 
#define IS_SET_E_4		0xE3 // E3		set d4 of E			-  8 cycles 
#define IS_SET_H_4		0xE4 // E4		set d4 of H			-  8 cycles 
#define IS_SET_L_4		0xE5 // E5		set d4 of L			-  8 cycles 
#define IS_SET_xHL_4	0xE6 // E6		set d4 of(HL)		- 12 cycles 
#define IS_SET_A_4		0xE7 // E7		set d4 of A			-  8 cycles 
#define IS_SET_B_5		0xE8 // E8		set d5 of B			-  8 cycles 
#define IS_SET_C_5		0xE9 // E9		set d5 of C			-  8 cycles 
#define IS_SET_D_5		0xEA // EA		set d5 of D			-  8 cycles 
#define IS_SET_E_5		0xEB // EB		set d5 of E			-  8 cycles 
#define IS_SET_H_5		0xEC // EC		set d5 of H			-  8 cycles 
#define IS_SET_L_5		0xED // ED		set d5 of L			-  8 cycles 
#define IS_SET_xHL_5	0xEE // EE		set d5 of(HL)		- 12 cycles 
#define IS_SET_A_5		0xEF // EF		set d5 of A			-  8 cycles 

// Begin F0 
#define IS_SET_B_6		0xF0 // F0		set d6 of B			-  8 cycles 
#define IS_SET_C_6		0xF1 // F1		set d6 of C			-  8 cycles 
#define IS_SET_D_6		0xF2 // F2		set d6 of D			-  8 cycles 
#define IS_SET_E_6		0xF3 // F3		set d6 of E			-  8 cycles 
#define IS_SET_H_6		0xF4 // F4		set d6 of H			-  8 cycles 
#define IS_SET_L_6		0xF5 // F5		set d6 of L			-  8 cycles 
#define IS_SET_xHL_6	0xF6 // F6		set d6 of(HL)		- 12 cycles 
#define IS_SET_A_6		0xF7 // F7		set d6 of A			-  8 cycles 
#define IS_SET_B_7		0xF8 // F8		set d7 of B			-  8 cycles 
#define IS_SET_C_7		0xF9 // F9		set d7 of C			-  8 cycles 
#define IS_SET_D_7		0xFA // FA		set d7 of D			-  8 cycles 
#define IS_SET_E_7		0xFB // FB		set d7 of E			-  8 cycles 
#define IS_SET_H_7		0xFC // FC		set d7 of H			-  8 cycles 
#define IS_SET_L_7		0xFD // FD		set d7 of L			-  8 cycles 
#define IS_SET_xHL_7	0xFE // FE		set d7 of(HL)		- 12 cycles 
#define IS_SET_A_7		0xFF // FF		set d7 of A			-  8 cycles

/*\ -------------------- PREFIX_ED ---------------------- \*/ 
// begin 40 
#define IN_B_xC			0x40 
#define OUT_xC_B		0x41
#define SBC_HL_BC		0x42 
#define BC_TO_xI16		0x43 
#define NEG_44			0x44
#define RETN_45			0x45 
#define MODE0_46		0x46 
#define A_TO_I			0x47 
#define IN_C_xC			0x48 
#define OUT_xC_C		0x49 
#define ADC_HL_BC		0x4A 
#define xI16_TO_BC		0x4B 
#define NEG_4C			0x4C 
#define RET_INTERRUPT	0x4D 
#define MODE_UNUSE_4E	0x4E 
#define A_TO_R			0x4F 
// begin 50 
#define IN_D_xC			0x50 
#define OUT_xC_D		0x51 
#define SBC_HL_DE		0x52 
#define DE_TO_xI16		0x53 
#define NEG_54			0x54 
#define RETN_55			0x55 
#define MODE1_56		0x56 
#define I_TO_A			0x57 
#define IN_E_xC			0x58 
#define OUT_xC_E		0x59 
#define ADC_HL_DE		0x5A 
#define xI16_TO_DE		0x5B 
#define NEG_5C			0x5C 
#define RETN_5D			0x5D 
#define MODE2_5E		0x5E 
#define R_TO_A			0x5F  
// begin 60 
#define IN_H_xC			0x60 
#define OUT_xC_H		0x61 
#define SBC_HL_HL		0x62 
#define HL_TO_xI16		0x63 
#define NEG_64			0x64 
#define RETN_65			0x65 
#define MODE0_66		0x66 
#define RRD				0x67 
#define IN_L_xC			0x68 
#define OUT_xC_L		0x69 
#define ADC_HL_HL		0x6A 
#define xI16_TO_HL		0x6B 
#define NEG_6C			0x6C 
#define RETN_6D			0x6D 
#define MODE_UNUSE_6E	0x6E 
#define RLD				0x6F 
// begin 70 
#define IN_xC			0x70 
#define OUT_xC_0		0x71 
#define SBC_HL_SP		0x72 
#define SP_TO_xI16		0x73 
#define NEG_74			0x74 
#define RETN_75			0x75 
#define MODE1_76		0x76 
#define IN_A_xC			0x78 
#define OUT_xC_A		0x79 
#define ADC_HL_SP		0x7A 
#define xI16_TO_SP		0x7B 
#define NEG_7C			0x7C 
#define RETN_7D			0x7D 
#define MODE2_7E		0x7E 
// begin A0 
#define LDI				0xA0 
#define CPI				0xA1 
#define INI				0xA2 
#define OUTI			0xA3
#define LDD				0xA8 
#define CPD				0xA9 
#define IND				0xAA 
#define OUTD			0xAB 
// begin B0
#define LDIR			0xB0
#define CPIR			0xB1 
#define INIR			0xB2 
#define OTIR			0xB3 
#define LDDR			0xB8 
#define CPDR			0xB9 
#define INDR			0xBA 
#define OTDR			0xBB

#endif