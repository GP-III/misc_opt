/* type and settings for compiler, common header 
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

#ifndef _CONFIG_H
#define _CONFIG_H 1

#ifdef _WIN32
# if defined (_UNICODE) || defined (UNICODE)
#  define UCS2
# endif 
# define _CRT_SECURE_NO_DEPRECATE
#endif 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
# define ks_null nullptr
#else 
# define ks_null ((void *)0)
#endif  

#ifndef STATIC_DEFINED
# define STATIC static
#endif

typedef int ks_bool;
# define ks_false 0 
# define ks_true 1

typedef int8_t ks_int8;
typedef int16_t ks_int16;
typedef int32_t ks_int32;
typedef int64_t ks_int64;
typedef uint8_t ks_uint8;
typedef uint16_t ks_uint16;
typedef uint32_t ks_uint32;
typedef uint64_t ks_uint64;
typedef float ks_float;
typedef double ks_double;
typedef signed int ks_int;
typedef unsigned int ks_uint;
typedef intptr_t ks_intptr;
typedef unsigned int ks_uint;
typedef void ks_void;

# define KS_UINT8_CAST(x) ((ks_uint8)(x))
# define KS_UINT16_CAST(x) ((ks_uint16)(x))
# define KS_UINT32_CAST(x) ((ks_uint32)(x))
# define KS_UINT64_CAST(x) ((ks_uint64)(x))
# define KS_INT8_CAST(x) ((ks_int8)(x))
# define KS_INT16_CAST(x) ((ks_int16)(x))
# define KS_INT32_CAST(x) ((ks_int32)(x))
# define KS_INT64_CAST(x) ((ks_int64)(x))
# define KS_FLOAT_CAST(x) ((ks_float)(x))
# define KS_DOUBLE_CAST(x) ((ks_double)(x))

#if defined (_MSC_VER) || defined (__ICC) || defined (__INTEL_COMPILER) /* MSVC/ICC starting... */
# define ks_finline static  __forceinline
# define ks_dinline         __declspec(noinline)
# define ks_callstd         __stdcall
# define ks_callc           __cdecl
# define ks_cvalign(x)      __declspec(align(x))
# define ks_cvimpl          __declspec(dllexport) 
#elif defined (__GNUC__) || defined (__GNUG__) /* MSVC/ICC end... GNUC starting */
# define ks_finline static  __attribute__((always_inline))
# define ks_dinline         __attribute__((noinline))
# define ks_callstd         __attribute__((stdcall))
# define ks_callc           __attribute__((cdecl))
# define ks_cvalign(x)      __attribute__((aligned(x)))
# define ks_cvimpl          __attribute__((dllexport))
#else /* unsupported */
# error unsupported compiler! 
#endif 
# define KS_CMP_EQUAL 0
# define KS_CMP_ABOVE 1
# define KS_CMP_LOW 2
# define KS_CMP_NaN 3

ks_finline 
int ks_cmp0_double (ks_double *value) {
  /*  XXX: IEEE754  */
  ks_int64 *v = (ks_int64 *)(value);
  /* S:nocare M and E is 0 */
  if (! (v[0] & 0x7FFFFFFFFFFFFFFF)) /* -0 or +0*/
    return KS_CMP_EQUAL;
  if (  ( ((ks_int32 *)&v[0])[1] & 0x80000000)) 
    return KS_CMP_LOW;
  else 
    return KS_CMP_ABOVE;
}

#ifdef _WIN32
# ifdef _MSC_VER
#  define _DEBUG_BREAK() _asm int 3
# else 
# endif 
#endif 

/* some hardware settings. */
#define TIMER_DIVIDER_SAME_FREQ_BOTH_CBG_DMG_BASE_DMG
#define JOYPAD_FREQ_IN_DMG 50.3
#define JOYPAD_FREQ_IN_CGB 50.3 
#define JOYPAD_INTERRUPT_GEN_ENABLE 
#define JOYPAD_LOAD_ACTION_IN_P1415_ALLOPEN 3 /* 0:OR 1:AND 2:XOR 3:AND-NOT */
#define TIMER_RESET_IN_RESET_FREQ_MOD_
#define PPU_POST_RENDER_TIME 0 /* 0:VBLANK START 1:VBLANK END. */
/* #define PPU_SPRITE_LIMIT10_IN_SCANLINE */ 

#ifdef UCS2
typedef  wchar_t host_char;
#else 
typedef  char host_char;
#endif 

#endif 