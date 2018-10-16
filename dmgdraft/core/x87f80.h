/* float support base X87fpu
 *
 * Copyright (C) 2018 moecmks
 * This file is part of KS3578.
 * 
 * do What The Fuck you want to Public License
 * 
 * Version 1.0, March 2000
 * Copyright (C) 2000 Banlu Kemiyatorn (]d).
 * 136 Nives 7 Jangwattana 14 Laksi Bangkok
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 * 
 * Ok, the purpose of this license is simple
 * and you just
 * 
 * DO WHAT THE FUCK YOU WANT TO.
 */

#include "conf.h"
#define SAFE_CONTEXT
#ifdef SAFE_CONTEXT
# define CONTEXT_SAVE() __asm push eax 
# define CONTEXT_RESUME() __asm pop eax 
#else 
# define CONTEXT_SAVE() (void) 0
# define CONTEXT_RESUME() (void) 0
#endif 

typedef struct ks_f80_tag {
  ks_uint64 fractional;
  ks_uint16 exponent :15;
  ks_uint16 sign: 1;
  ks_uint32 align;
} ks_f80; 
typedef struct ks_f32_tag {
  ks_uint32 fractional:24;
  ks_uint32 exponent :7;
  ks_uint32 sign: 1;
} ks_f32; 
typedef struct ks_f64_tag {
  ks_uint64 fractional:52;
  ks_uint64 exponent :11;
  ks_uint64 sign: 1;
} ks_f64; 

#if defined (IA32) && (defined (_MSC_VER) || defined (__ICC) || defined (__INTEL_COMPILER))
/* It is the drawback of Intel assembler to explicitly specify register allocation. 
   Moreover, it can not match the compiler to allocate and use registers more rationally, 
   which indirectly reduces efficiency.
 */
void ks_finline f80_fromI16 (ks_f80 *f80, ks_int16 value) 
{
  __asm fild word ptr[value]
  CONTEXT_SAVE() ;
  __asm mov eax, dword ptr[f80]
  __asm fstp tbyte ptr[eax]
  CONTEXT_RESUME();
}
void ks_finline f80_fromI32 (ks_f80 *f80, ks_int32 value) 
{
  __asm fild dword ptr[value]
  CONTEXT_SAVE() ;
  __asm mov eax, dword ptr[f80]
  __asm fstp tbyte ptr[eax]
  CONTEXT_RESUME();
}
void ks_finline f80_fromI64 (ks_f80 *f80, ks_int64 value) 
{
  __asm fild qword ptr[value]
  CONTEXT_SAVE() ;
  __asm mov eax, dword ptr[f80]
  __asm fstp tbyte ptr[eax]
  CONTEXT_RESUME();
}
void ks_finline f80_fromfloat (ks_f80 *f80, ks_float value) 
{
  __asm fld dword ptr[value]
  CONTEXT_SAVE() ;
  __asm mov eax, dword ptr[f80]
  __asm fstp tbyte ptr[eax]
  CONTEXT_RESUME();
}
void ks_finline f80_fromdouble (ks_f80 *f80, ks_double value) 
{
  __asm fld qword ptr[value]
  CONTEXT_SAVE() ;
  __asm mov eax, dword ptr[f80]
  __asm fstp tbyte ptr[eax]
  CONTEXT_RESUME();
}
void ks_finline f80_fromf80 (ks_f80 *f80, ks_f80 * value) 
{
  CONTEXT_SAVE() 
  __asm mov eax, dword ptr[value]
  __asm fld tbyte ptr[eax]
  __asm mov eax, dword ptr[f80]
  __asm fstp tbyte ptr[eax]
  CONTEXT_RESUME();
}
/* TODO: more. */
#endif 
/* TODO: SoftEMU float- N.A */