#ifndef __setup_
#define __setup_

/* Use DLmempool */
#include "malloc.h"

#ifndef __cplusplus
# define __null_ ((void *)0)
#else 
# define __null_ 0
#endif

#ifndef _MSC_VER 
# include <stdint.h>
# define __rttype_ __attribute__((dllexport))
# define __finline_ static __inline__ __attribute__((always_inline))
# define __cdecl_ __attribute__((__cdecl__))
# define __stdcall_ __attribute__((__stdcall__))
#else 
# include "__stdint.h"
# define __rttype_ __declspec(dllexport)
# define __finline_ static __forceinline
# define __cdecl_ __cdecl
# define __stdcall_ __stdcall
#endif 
    
#define UINTPTR_CAST($)((uintptr_t)($))
#define UINT64_CAST($)((uint64_t)($))
#define UINT32_CAST($)((uint32_t)($))
#define UINT16_CAST($)((uint16_t)($))
#define UINT8_CAST($)((uint8_t)($))

#define INTPTR_CAST($)((intptr_t)($))
#define INT64_CAST($)((int64_t)($))
#define INT32_CAST($)((int32_t)($))
#define INT16_CAST($)((int16_t)($))
#define INT8_CAST($)((int8_t)($))

#define UINTPTR_P_CAST($)((uintptr_t *)($))
#define UINT64_P_CAST($)((uint64_t *)($))
#define UINT32_P_CAST($)((uint32_t *)($))
#define UINT16_P_CAST($)((uint16_t *)($))
#define UINT8_P_CAST($)((uint8_t *)($))

#define INTPTR_P_CAST($)((intptr_t *)($))
#define INT64_P_CAST($)((int64_t *)($))
#define INT32_P_CAST($)((int32_t *)($))
#define INT16_P_CAST($)((int16_t *)($))
#define INT8_P_CAST($)((int8_t *)($))

#endif 