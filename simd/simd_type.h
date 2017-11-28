/*
===========================================================================
simple graph deal rountine for mos

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
Library General Public License for more details.  To obtain a 
copy of the GNU Library General Public License, write to the Free 
Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Any permitted reproduction of these routines, in whole or in part,
must bear this legend.
===========================================================================
*/

#ifndef _SIMD_TYPE_DEFINED_
#define _SIMD_TYPE_DEFINED_ 

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#define ALIGN(x) __declspec(align(x))
#define NAKED __declspec(naked)
#define CDECL __cdecl
#define STDCALL __stdcall 
#define ASM_BEGIN __asm {
#define ASM_END }
#define FORCEINLINE static __forceinline 

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;
typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#if defined (_WIN64)

#define GAX rax
#define GBX rbx
#define GCX rcx
#define GDX rdx
#define GSI rsi
#define GDI rdi
#define GSP rsp
#define GBP rbp
#define SIZE 0x04
#define ARG0 0x08
#define ARG1 0x10
#define ARG2 0x18
#define ARG3 0x20
#define ARG4 0x28
#define ARG5 0x30
#define ARG6 0x38
#define ARG7 0x40
#define ARG8 0x48
#define ARG9 0x50
#define SIZE_MOV movq 
#define XXX_MSIZE_MOV mov 
#define MMX_MSIZE_MOV movq
#define SSE_MSIZE_MOV movq
#define SIZE_PTR qword ptr 

typedef int64 msize_int;
typedef uint64 msize_uint;
typedef double msize_float;

#elif defined(_M_IX86)

#define GAX eax
#define GBX ebx
#define GCX ecx
#define GDX edx
#define GSI esi
#define GDI edi
#define GSP esp
#define GBP ebp
#define SIZE 0x04
#define ARG0 0x04
#define ARG1 0x08
#define ARG2 0x0C
#define ARG3 0x10
#define ARG4 0x14
#define ARG5 0x18
#define ARG6 0x1C
#define ARG7 0x20
#define ARG8 0x24
#define ARG9 0x28
#define SIZE_MOV movd
#define MMX_MSIZE_MOV movd
#define SSE_MSIZE_MOV movd
#define SIZE_PTR dword ptr 

typedef int32 msize_int;
typedef uint32 msize_uint;
typedef float msize_float;

#else

#error Oops, this platform is not supported.

#endif

#endif 