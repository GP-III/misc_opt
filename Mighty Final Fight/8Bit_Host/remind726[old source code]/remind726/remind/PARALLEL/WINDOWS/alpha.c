#include "simd_type.h"

extern unsigned long apt_standard[4096];
extern unsigned long apt_standard_r[4096];
extern unsigned long apt_cross[4096];

//  alpha_uss
// ================
NAKED
EXPORT
void CDECL alpha_uss (
/* src|des pointer */ void *s, 
/* pitch */ void *p, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
{
#undef SSE_SLOAD 
#undef SSE_SWRITE
#undef SSE_DLOAD 
#undef SSE_DWRITE
#define SSE_SLOAD movdqu
#define SSE_SWRITE movdqu
#define SSE_DLOAD movdqu
#define SSE_DWRITE movdqu
#include "alpha"  
}
//  alpha_ass
// ================
NAKED
EXPORT
void CDECL alpha_ass (
/* src|des pointer */ void *s, 
/* pitch */ void *p, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
{
#undef SSE_SLOAD 
#undef SSE_SWRITE
#undef SSE_DLOAD 
#undef SSE_DWRITE
#define SSE_SLOAD movdqa
#define SSE_SWRITE movdqa
#define SSE_DLOAD movdqa
#define SSE_DWRITE movdqa
#include "alpha" 
}
//  alpha_v_sudu
// ================
NAKED
EXPORT
void CDECL alpha_v_sudu (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_v"
}
//  alpha_v_sada
// ================
NAKED
EXPORT
void CDECL alpha_v_sada (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_v"
}
//  alpha_v_sadu
// ================
NAKED
EXPORT
void CDECL alpha_v_sadu (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_v"
}
//  alpha_v_suda
// ================
NAKED
EXPORT
void CDECL alpha_v_suda (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_v"
}
//  alpha_s_sudu
// ================
NAKED
EXPORT
void CDECL alpha_s_sudu (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_s"
}
//  alpha_s_sada
// ================
NAKED
EXPORT
void CDECL alpha_s_sada (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_s"
}
//  alpha_s_sadu
// ================
NAKED
EXPORT
void CDECL alpha_s_sadu (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_s"
}
//  alpha_s_suda
// ================
NAKED
EXPORT
void CDECL alpha_s_suda (
/* des pointer */ void *d,
/* src pointer */ void *s, 
/* des pitch */ void *dp, 
/* src pitch */ void *sp, 
/* width */ void *w, 
/* height */ void *h, 
/* alpha count */ int c)
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
#include "alpha_s"
}