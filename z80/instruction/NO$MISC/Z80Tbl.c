#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------------------
//	purpose:built [P]-Z-S/BCD look up table 
//	use tcc: command-> compiler\tinycc\tcc source\NO$MISC\Z80Tbl.c
// ---------------------------------------------------------------------

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

uint8_t  ZST[256] = {0};
uint8_t PZST[256] = {0};
uint8_t DAAT_HDCD[256] = {0};
uint8_t DAAT_HECD[256] = {0};
uint8_t DAAT_HDCE[256] = {0};
uint8_t DAAT_HECE[256] = {0};
uint8_t DAST_HDCD[256] = {0};
uint8_t DAST_HECD[256] = {0};
uint8_t DAST_HDCE[256] = {0};
uint8_t DAST_HECE[256] = {0};

int main (void) {

	FILE *fp;

	uint32_t i;
	uint32_t t;

#ifdef _MSC_VER

	__asm {
		push ebx 
		xor ecx, ecx 
	loopw:
		setz al
		sets bl
		lea	eax, [eax+ebx*2]
		setp bl
		jc outw
		ror al, 2 
		rol bl, 2; nn00 0n00
		or bl, al
		mov BYTE PTR[ ZST+ecx], al
		add BYTE PTR[PZST+ecx], bl 
		mov al, cl 
		daa 
		add BYTE PTR[DAAT_HDCD+ecx], al	
		mov bl, 0Fh 
		add bl, 05h 
		mov al, cl 
		daa
		add BYTE PTR[DAAT_HECD+ecx], al	
		add bl, 0F0h
		mov al, cl
		daa
		mov BYTE PTR[DAAT_HDCE+ecx], al	 
		add bl, 0FFh
		mov al, cl 
		daa
		add BYTE PTR[DAAT_HECE+ecx], al	
		mov al, cl 
		das
		mov BYTE PTR[DAST_HDCD+ecx], al	
		mov bl, 0Fh 
		add bl, 05h 
		mov al, cl 
		das
		add BYTE PTR[DAST_HECD+ecx], al	
		stc			
		mov al, cl
		das
		add BYTE PTR[DAST_HDCE+ecx], al	
		add bl, 0FFh 
		mov al, cl 
		das
		mov BYTE PTR[DAST_HECE+ecx], al	
		add cl, 1 
		jmp loopw 
	outw:
		pop ebx 
	}
#else 
	__asm__ __volatile__(

		"pushl %ebx\n\t"
		"xorl %ecx, %ecx\n\t"
		"loopw:\n\t"
		"setz %al\n\t"
		"sets %bl\n\t"
		"leal (%eax,%ebx,0x2),%eax\n\t"
		"setp %bl\n\t"
		"jc outw\n\t"
		"rorb $0x2,%al\n\t"
		"rolb $0x2,%bl\n\t"
		"orb  %al,%bl\n\t"
		"movb %al,ZST(%ecx)\n\t"
		"addb %bl,PZST(%ecx)\n\t"
		"movb %cl, %al\n\t"
		"daa\n\t"
		"addb %al, DAAT_HDCD(%ecx)\n\t"
		"movb $15, %bl\n\t"
		"addb $5, %bl\n\t"
		"movb %cl, %al\n\t"
		"daa\n\t"
		"addb %al, DAAT_HECD(%ecx)\n\t"
		"addb $240, %bl\n\t"
		"movb %cl, %al\n\t"
		"daa\n\t"
		"addb %al, DAAT_HDCE(%ecx)\n\t"
		"addb $255, %bl\n\t"
		"movb %cl, %al\n\t"
		"daa\n\t"
		"addb %al, DAAT_HECE(%ecx)\n\t"
		"movb %cl, %al\n\t"
		"das\n\t"
		"addb %al, DAST_HDCD(%ecx)\n\t"
		"movb $15, %bl\n\t"
		"addb $5, %bl\n\t"
		"movb %cl, %al\n\t"
		"das\n\t"
		"addb %al, DAST_HECD(%ecx)\n\t"
		"addb $240, %bl\n\t"
		"movb %cl, %al\n\t"
		"das\n\t"
		"addb %al, DAST_HDCE(%ecx)\n\t"
		"addb $255, %bl\n\t"
		"movb %cl, %al\n\t"
		"das\n\t"
		"addb %al, DAST_HECE(%ecx)\n\t"
		"addb $1, %cl\n\t"
		"jmp loopw\n\t"
		"outw:\n\t"
		"popl %ebx\n\t"
		);
#endif 	

#define AKA_MRS(x) /* ------------- out print marco -------------- */	\
	fprintf (fp, "static const uint8_t "#x"[256] = {\n\n\t");		\
	    for (i = 0; i != 256; ++ i) {									\
			 fprintf (fp, "0x%02X", x[i]);							\
				    i == 255 ? fprintf (fp, "\n};\n")					\
						     : i % 8 == 7 							\
						     ? fprintf (fp, ","), fprintf(fp, "\n\t")	\
						     : fprintf (fp, ",");					\
		}
	fp
	= fopen ("Z80Tbl.lcs", "wt+");

	AKA_MRS (ZST);		/* --- out  ZSTable --- */
	AKA_MRS (PZST);		/* --- out PZSTable --- */
	AKA_MRS (DAAT_HDCD);/* --- out DAATable --- */
	AKA_MRS (DAAT_HECD);/* --- out DAATable --- */
	AKA_MRS (DAAT_HDCE);/* --- out DAATable --- */
	AKA_MRS (DAAT_HECE);/* --- out DAATable --- */
	AKA_MRS (DAST_HDCD);/* --- out DASTable --- */
	AKA_MRS (DAST_HECD);/* --- out DASTable --- */
	AKA_MRS (DAST_HDCE);/* --- out DASTable --- */
	AKA_MRS (DAST_HECE);/* --- out DASTable --- */
	
#undef AKA_MRS
	fclose (fp);
	return 0;
}