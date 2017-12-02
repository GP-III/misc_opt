#include "simd_type.h"

//  gray_s_sudu
// ================
NAKED
EXPORT
void CDECL gray_s_sudu (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h)
{
#undef SSE_SLOAD 
#undef SSE_SWRITE
#undef SSE_DLOAD 
#undef SSE_DWRITE
#undef OVER_ALIGNED
#define SSE_SLOAD movdqu
#define SSE_SWRITE movdqu
#define SSE_DLOAD movdqu
#define SSE_DWRITE movdqu
#include "gray_s"
}
//  gray_s_sada
// ================
NAKED
EXPORT
void CDECL gray_s_sada (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h)
{
#undef SSE_SLOAD 
#undef SSE_SWRITE
#undef SSE_DLOAD 
#undef SSE_DWRITE
#undef OVER_ALIGNED
#define SSE_SLOAD movdqa
#define SSE_SWRITE movdqa
#define SSE_DLOAD movdqa
#define SSE_DWRITE movdqa
#define OVER_ALIGNED
#include "gray_s"
}
//  gray_s_sadu
// ================
NAKED
EXPORT
void CDECL gray_s_sadu (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h)
{
#undef SSE_SLOAD 
#undef SSE_SWRITE
#undef SSE_DLOAD 
#undef SSE_DWRITE
#undef OVER_ALIGNED
#define SSE_SLOAD movdqa
#define SSE_SWRITE movdqa
#define SSE_DLOAD movdqu
#define SSE_DWRITE movdqu
#include "gray_s"
}
//  gray_s_suda
// ================
NAKED
EXPORT
void CDECL gray_s_suda (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h)
{
#undef SSE_SLOAD 
#undef SSE_SWRITE
#undef SSE_DLOAD 
#undef SSE_DWRITE
#undef OVER_ALIGNED
#define SSE_SLOAD movdqu
#define SSE_SWRITE movdqu
#define SSE_DLOAD movdqa
#define SSE_DWRITE movdqa
#include "gray_s"
}