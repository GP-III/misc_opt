#ifndef __TYPE_DEFINE_INC_
#define __TYPE_DEFINE_INC_

/* easy type define, you know. ***/

typedef int bool_t;
#define false_t 0
#define true_t 1 

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef signed char int8;
typedef signed short int16;

/* rect, range struct define */

struct rect2 {
 uint8 x;
 uint8 y;
 uint8 w;
 uint8 h;
};

struct rect3 {
 uint8 left;
 uint8 right;
 uint8 top;
 uint8 bottom;
};

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#if defined (_WIN32)
#  include <windows.h>
#endif 

#ifdef _DEBUG 
#  if 1
#    define DEBUG_OUTPUT(...) printf(__VA_ARGS__)
#  else 
#    include <windows.h>

#    define DEBUG_OUTPUT(...) do {          \
       char tbbuf[265];              \
       sprintf (& tbbuf[0], __VA_ARGS__);   \
	                                        \
       OutputDebugStringA (& tbbuf[0]);     \
     } while (0)
#  endif 
#else 
#  define DEBUG_OUTPUT(...) ((void) 0)
#endif 


#define DISABLE_OUTPUT_DEBUG_ATTR
#endif 