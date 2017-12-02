/*
 * Copyright (C) 2017 moecmks, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You should have received a copy of the GNU General Public
 * License along with the tile-mr10; if not, see:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef config0_H 
#define config0_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "config.h"
#include "stdint.h"

#if !defined (__cplusplus)
#  ifndef null_DEFINED
#    define null ((void *)0)
#  endif
#else 
#  ifndef null_DEFINED
#    define null 0
#  endif
#endif

#if defined (__MSVC_CL_) || defined (__INTEL_ICL_) /* MSVC/ICC starting... */
#  define finline static  __forceinline
#  define dinline         __declspec(noinline)
#  define callstd         __stdcall
#  define callc           __cdecl
#  define cvalign(x)      __declspec(align(x))
#  define cvimpl          __declspec(dllexport) 
#elif defined (__GNU_C_) /* MSVC/ICC end... GNUC starting */
#  define finline static            __attribute__((always_inline))
#  define dinline                   __attribute__((noinline))
#  define callstd                   __attribute__((stdcall))
#  define callc                     __attribute__((cdecl))
#  define cvalign(x)                __attribute__((aligned(x)))
#  define cvimpl                    __attribute__((dllexport))
#else /* unsupported */
#  error unsupported compiler! 
#endif 

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif