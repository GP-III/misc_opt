#include "simd_type.h"

extern uint32 xRatZoom[4096];
extern uint32 yRatZoom[4096];

//  zoom_c
// ================
EXPORT
void CDECL zoom_c (
/* des pointer */ uint32 *d,
/* src pointer */ uint32 *s, 
/* des pitch */ msize_uint dp, 
/* src pitch */ msize_uint sp, 
/* width */ msize_uint w, 
/* height */ msize_uint h, 
/* zoom count */ msize_float c)
{
	msize_uint x;
	msize_uint y;
	msize_uint new_w = (msize_uint)(c * (msize_float)w);
	msize_uint new_h = (msize_uint)(c * (msize_float)h); 
	msize_uint _sp;
	msize_uint _dp;
	msize_uint y_count_cache;
	msize_uint y_cahce;
	msize_float r;
	msize_float y_add_cache;
	msize_float x_count_cache;
	uint32 *dptr_cache;
	uint32 *sptr_cache;
	
	if (!new_w || !new_h)
		return;

	r = 1.0/c;
	
	_sp = sp >> 2;
	_dp = dp >> 2;

	y_count_cache = 0;
	y_add_cache = 0.0;
	// please OPEN sse2 optimization in Release Mode 
	for (y = 0; y != new_h; y ++) {
		x_count_cache = 0.0;
		dptr_cache = &d[y_count_cache];
		sptr_cache = &s[_sp * (msize_uint)(y_add_cache)];	
		
		for (x = 0; x != new_w; x ++) {
			dptr_cache[x] = sptr_cache[(msize_uint)x_count_cache];
			x_count_cache += r;
		}
		y_add_cache += r;
		y_count_cache += _dp;
	}
}
// =============================================================
//  zoom_sc [bilinear interpolation]
// =============================================================
// note, not range check, use inline texture type [realloc]...
// =============================================================
#ifdef _M_IX86
NAKED
#endif
EXPORT
void CDECL zoom_sc (
/* des pointer */ uint32 *d,
/* src pointer */ uint32 *s, 
/* des pitch */ msize_uint dp, 
/* src pitch */ msize_uint sp, 
/* width */ msize_uint w, 
/* height */ msize_uint h, 
/* zoom count */ msize_float c)
{
#ifndef _M_IX86

#define GET_B(x)((msize_float)(((x) & 0x000000FF) >> 0))
#define GET_G(x)((msize_float)(((x) & 0x0000FF00) >> 8))
#define GET_R(x)((msize_float)(((x) & 0x00FF0000) >> 16))

	msize_uint X;
	msize_uint Y;
	msize_uint NewWidth  = (msize_uint)(c * (msize_float)w);
	msize_uint NewHeight = (msize_uint)(c * (msize_float)h);
	msize_uint DesPitch;
	msize_uint SrcPitch;
	msize_uint BPosTX;
	msize_uint BPosTY;
	uint32 TLeft;
	uint32 TRight;
	uint32 BLeft;
	uint32 BRight;
	uint32 R;
	uint32 G;
	uint32 B;
	msize_float RevseRatio;
	msize_float XOffsetRatio;
	msize_float YOffsetRatio;
	msize_float r0;
	msize_float r1;
	msize_float r2;
	msize_float r3;
	if (!NewWidth || !NewHeight)
		return;

	RevseRatio = 1.0/c;
	
	SrcPitch = sp >> 2;
	DesPitch = dp >> 2;

	for (Y = 0; Y != NewHeight; Y ++) {
		YOffsetRatio = (RevseRatio * (msize_float)Y);
		BPosTY = (msize_uint)YOffsetRatio;
		YOffsetRatio-=(msize_float)BPosTY;
		for (X = 0; X != NewWidth; X ++) {
			XOffsetRatio = RevseRatio * (msize_float)X;
			BPosTX = (msize_uint)XOffsetRatio;
			XOffsetRatio-=(msize_float)BPosTX;
			
			TLeft = s[BPosTX + BPosTY * SrcPitch + 0];
			TRight= s[BPosTX + BPosTY * SrcPitch + 1];
			BLeft = s[BPosTX + BPosTY * SrcPitch + SrcPitch + 0];
			BRight= s[BPosTX + BPosTY * SrcPitch + SrcPitch + 1];
			
			r3 = XOffsetRatio * YOffsetRatio;
			r2 = (1.0 - XOffsetRatio) * YOffsetRatio;
			r1 = (1.0 - YOffsetRatio) * XOffsetRatio;
			r0 = (1.0 - XOffsetRatio) * (1.0 - YOffsetRatio);
			
			B = (uint32)(((GET_B(TLeft) * r0)) + ((GET_B(TRight) * r1)) + ((GET_B(BLeft) * r2)) + ((GET_B(BRight) * r3))) & 255;
			G = (uint32)(((GET_G(TLeft) * r0)) + ((GET_G(TRight) * r1)) + ((GET_G(BLeft) * r2)) + ((GET_G(BRight) * r3))) & 255;
			R = (uint32)(((GET_R(TLeft) * r0)) + ((GET_R(TRight) * r1)) + ((GET_R(BLeft) * r2)) + ((GET_R(BRight) * r3))) & 255;
			
			d[X + Y * DesPitch] = ((R << 16) | (G << 8) | B);
		}

	}
#else 
	static ALIGN(16) double zoom_ratio = (double)1024.00000000000000000000;
	static ALIGN(16) double one_ = (double)1.0000000000;
	static ALIGN(16) uint8 sse_suffe0[16] = 
	{
		0xFF, // 00 
		0x00, // B0 
		0xFF, // 00 
		0x04, // B1 
		0xFF, // 00 
		0x01, // G0 
		0xFF, // 00 
		0x05, // G1 	
		0xFF, // 00 
		0x02, // R0 
		0xFF, // 00 
		0x06, // R1 
		0xFF, // 00 
		0xFF, // 00 
		0xFF, // 00 
		0xFF, // 00		mix result 00000000 R100R000 G100G000 B100B000
	};
	// xxx10000 xxx10000 xxx10000 xxx10000
	// xxx0FF00 xxx0FF00 xxx0FF00 xxx0FF00
	
	static ALIGN(16) uint8 sse_suffe1[16] = 
	{
		0x01,  
		0x02,  	
		0x05, 
		0x06, // r1 r0
		
		0x01, 
		0x02, 
		0x05, 
		0x06, // r1 r0
		
		0x01, 
		0x02, 
		0x05, 
		0x06, // r1 r0
		
		0xFF,  
		0xFF,  
		0xFF,  
		0xFF, // 00	00
	};
ASM_BEGIN

	cvtpi2pd 	xmm0,  [esp+20] 	// w | h
	cvtss2sd 	xmm2,  [esp+28] 	// c 
	movddup 	xmm1, 	xmm2 		// copy twice
	movsd 		xmm3,  [one_]		// 1.0000000000
	divsd 		xmm3, 	xmm2 		// revse_ratio [const]
	mulpd 		xmm1, 	xmm0 		// get float-twice 
	mulsd 		xmm3,  [zoom_ratio]	// get ratio 
	cvttsd2si	 ecx, 	xmm1 		// new width 
	psrldq		xmm1, 	   8 		// shift 
	cvttsd2si 	 eax, 	xmm1 		// new height
	movd		xmm6, 	 esp 		// store stack		 
	test 		 eax,  	 eax 		// ZERO ?
	je 			__exit				// jcc 
	test 		 ecx, 	 ecx 		// ZERO ?
	je 			__exit 				// jcc 

	push 		ebp 
	push 		ebx 
	push 		esi 
	push 		edi 				// save context
	
	/********************************/
	/**** nor register dispatch *****/
	/********************************/	
	mov 		edi,   [esp+16+4] 	// edi <- des pointer 
	lea 		ebx,   [ecx*4] 		
	mov 		esi,   [esp+16+8] 	// esi <- src pointer 
	mov 		ebp,   [esp+16+12]	 
	mov 		edx,   [esp+16+16]	// edx <- src pitch
	sub 		ebp, 	ebx 		
	xor 		ebx, 	ebx 		// ebx <- init ratio_x
	mov 		eax, 	ecx 		// eax <- des width count 
	lea 		ecx,   [xRatZoom]	// ecx <- init ratio_x look up table index
	cvttsd2si 	esp, 	xmm3 		// esp <- revse_ratio
									// ebp <- sec-loop-add-vram ptr [after0]								
	/********************************/
	/***** mm register dispatch *****/
	/********************************/
	pxor 		mm0,	mm0 		// mm0 <- n/a 
	movd 		mm1,	ebp			// mm1 <- des rva 
	cvttpd2pi	mm2, 	xmm1		// mm2 <- des height 
	movd 		mm3,	esi 		// mm3 <- src pointer 
	movd 		mm4,	eax 		// mm4 <- des width 
	movd 		mm5, 	edx 		// mm5 <- src pitch 
	movd 		mm6,	esp 		// mm6 <- reverse ratio 
	pxor 		mm7, 	mm7 		// mm7 <- init ratio_y 
	/********************************/
	/**** xmm register dispatch *****/
	/********************************/	
	movdqa 		xmm7,  [yRatZoom] 	// xmm7 <- y_reverse_ratio martix 
	movdqa 		xmm6,	xmm6 		// xmm6 <- esp temp 
	movdqa 		xmm5,  [sse_suffe1] // xmm5 <- reverse_ratio shuffe sequence
	movdqa 		xmm4,  [sse_suffe0] // xmm4 <- pixel shuffe sequence
									// xmm3 <- n/a
									// xmm2 <- n/a
									// xmm1 <- n/a
									// xmm0 <- n/a
	/********************************/
	/************ after0 ************/
	/********************************/								
	mov 		ebp,	esi 		// src-loop-add-vram ptr
align 16
biline_entry:
	movq 		xmm0,  qword ptr[esi] 	  // pixel0, pixel1
	movq		xmm1,  qword ptr[esi+edx] // pixel2, pixel3
	pshufb 		xmm0,   xmm4 		// shuffe pixel0, pixel1
	movdqa 		xmm2,  [ecx] 		// ratio group
	pshufb 		xmm1,   xmm4 		// shuffe pixel0, pixel1
	pmaddwd 	xmm2, 	xmm7 		// get r0, r1, r2, r3
	movhlps 	xmm3, 	xmm2 		// high qword->low qword
	pshufb 		xmm2, 	xmm5 		// ratio shuffe low
	pshufb 		xmm3, 	xmm5 		// ratio shuffe high
	pmulhuw 	xmm0, 	xmm2 		// xmm0 00000000, 00r100r0, 00g100g0, 00b100b0 
	pmulhuw 	xmm1, 	xmm3 		// xmm1 00000000, 00r300r2, 00g300g2, 00b300b2 
	paddusb	 	xmm0, 	xmm1 		// vert mix
	phaddsw 	xmm0, 	xmm0 		// hori mix
	packuswb 	xmm0, 	xmm0 		// pack
	movd 	   [edi], 	xmm0 		// write back des buffer
// ====================================
// inloop check 
// ====================================
	add 		ebx, 	esp 		// new ratioX 
	add 		edi, 	  4 		// addr to next des-line-buffer-X
	mov 		esi, 	ebx 		// save ratioX 
	mov 		ecx, 	ebx 		// save ratioX
	shr 		esi, 	 10 		// get n/1024
	mov 		eax, 	eax 		// spare
	lea 		esi, 	[esi*4]		// mul color depth 
	and 		ecx,   1020 		// get (n%1024)/1024
	add 		esi, 	ebp 		// get new vram ptr in x line [ebp:basevram y line]
	lea 		ecx,   [xRatZoom+ecx*4] // (n%1024)/1024 * 4  <= (n%1024)/1024 * 256 * 16 
	dec 		eax 				// jcc-to do x check
	jne 		biline_entry	  
// ====================================	
// outloop check 
// ====================================		
	paddd 		mm7, 	mm6 		// new ratioY
	movd 		ebx, 	mm2 		// height 	
	movq 		mm0, 	mm7 		// new ratioY 
	movd 		ecx, 	mm7 		// new ratioY 	
	psrld 		mm0, 	 10 		// get round 
	and 		ecx,   1020 		// 1023	
	pmuludq 	mm0, 	mm5 		// get pitch vram
	paddd 		mm0, 	mm3 
	movd 		esi,	mm0 		// vram-y 
	movd 		ebp,	mm1 		// des RVA 
	movdqa 	   xmm7,   [yRatZoom+ecx*4]	
	dec 		ebx 
	lea 		edi,   [edi+ebp]	
	mov 		ebp, 	esi 
	lea		 	ecx,   [xRatZoom]
	movd 		mm2, 	ebx 
	movd 		eax, 	mm4 
	mov 		ebx, 	0
	jne 		biline_entry  
	movd 		esp, 	xmm6 
	sub			esp,	16
	pop 		edi 
	pop 		esi
	pop 		ebx
	pop 		ebp 
	emms
__exit:
	ret 
	
	ASM_END
	
#endif
	
}