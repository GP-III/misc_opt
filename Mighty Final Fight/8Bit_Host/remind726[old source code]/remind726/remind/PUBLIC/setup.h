#ifndef __setup_
#define __setup_

#ifndef __cplusplus
# define __null_ ((void *)0)
#else 
# define __null_ 0
#endif

#ifndef _MSC_VER 
# include <stdint.h>
# define __rttype_ __attribute__((dllimport))
# define __finline_ static __inline__ __attribute__((always_inline))
# define __cdecl_ __attribute__((__cdecl__))
# define __stdcall_ __attribute__((__stdcall__))
#else 
# include "__stdint.h"
# define __rttype_ __declspec(dllimport)
# define __finline_ static __forceinline
# define __cdecl_ __cdecl
# define __stdcall_ __stdcall
#endif 

#endif 