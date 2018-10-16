;;  simple bilinear interpolation for IDirect3DSurface9 - RGB15 (Alpha empty) MASM32 
;;     not mapper center point (SrcX=(dstX+0.5)* (srcWidth/dstWidth) -0.5 )  - moecmks . 
;;  
;;  Bad code
MOD_SFT equ 21     ;;    n:= 21
MOD_MAP equ 00200000h    ;;   2^21 
;;          001FFFFFH 
MOD_MASK equ 001FFFFFh  
MOD_MASK_LEVEL2 equ 001F0000h 

;; 
;;
;; prototype 

;; void __cdecl bilinear_x32K2 (uint16_t *dstPtr, intptr_t dstPitch, 
;;                      uint16_t *srcPtr, intptr_t srcPitch,
;;               intptr_t dstW, intptr_t dstH,
;;               intptr_t srcW, intptr_t srcH) 


        .686                      ; create 32 bit code
        .mmx
        .xmm                     
        .model flat, c      ; 32 bit memory model
        option casemap :none      ; case sensitive
        
comment @
  64K+64K 1+1
  128K+64K 2+1
  256K+64K 4 +1
  512K+64K 8 +1
  1M+64K 16+1
  2M+64K 32+1
@

.data?
align 16 
;; aptTab dw 1081344 dup(?) ;; (2M+64K)/sizeof (word)
  
;; extrn  aptTab:proc        
;; export symbols  
;; public aptTab

.code 
align 16
bilinear_x32K2 proc c 
 option prologue:none, epilogue:none
 
  ;;  stack arg base  offset typedef .
  dstPtr equ 4*1
  dstPitch equ 4*2
  srcPtr equ 4*3
  srcPitch equ 4*4
  dstW equ 4*5
  dstH equ 4*6
  srcW equ 4*7
  srcH equ 4*8
   
        ;;   first, save ctx  
        push   edi 
        push   esi 
        push   ebx 
        push   ebp ;; stack += 16 
        
  stk_skip equ 16 
  
        movd   xmm7, esp 
        
        ;;   calc vptr and rva pitch 
        ;;   for target pitch, we can get rva 
        ;;   source pitch can't  
        
        mov    edi, stk_skip[esp+dstPtr]   ;; edi !! dstPtr 
        mov    ebx, stk_skip[esp+dstPitch]   
        mov    eax, stk_skip[esp+dstW]  ;; eax !! dstW
        mov    ebp, stk_skip[esp+dstH]  ;; ebp !! dstH
        
        cmp    eax, 0 
        jle    V_EXIT 
        cmp    ebp, 0 
        jle    V_EXIT        
        test   edi, edi 
        je     V_EXIT
        
        lea    edx, [eax*2] 
        sub    ebx, edx           ;;  ebx !! target pitch rva . 
        
        mov    edx, stk_skip[esp+srcH]  ;;  edx !!srcH
        mov    ecx, stk_skip[esp+srcW]  ;;  ecx !!srcW
        mov    esi, stk_skip[esp+srcPtr] ;; esi !!srcPtr

        cmp    edx, 0 
        jle    V_EXIT 
        cmp    ecx, 0 
        jle    V_EXIT        
        test   esi, esi 
        je     V_EXIT
   
        ;;    eax <_  dstW        
        ;;    ebx <_  target pitch rva . 
        ;;    ecx <_  srcW
        ;;    edx <_  srcH       
        ;;    esi <_  source ptr poll
        ;;    edi <_  target ptr 
        ;;    ebp <_  dstH
        ;;    esp <_  temp 
        
        movd  mm4, dword ptr stk_skip[esp+srcPitch]
        mov   esp,   MOD_MAP 

        cvtsi2sd  xmm3,  edx      ;;   srcH 
        cvtsi2sd  xmm5,  esp      ;;   MOD_MAP 
        cvtsi2sd  xmm6,  ebp      ;;   dstH 
        divsd xmm3, xmm6          ;;   srcH/ dstH 
        mulsd xmm3, xmm5          ;;    * MOD_MAP
        cvtpd2pi mm3, xmm3      ;; cast(int)  rt_delta_y:DWORD 
        
        cvtsi2sd  xmm6,  ecx      ;;   srcW 
        cvtsi2sd  xmm0,  eax      ;;   dstW
        divsd xmm6, xmm0          ;; srcW/ dstW
        mulsd xmm6, xmm5          ;;  * MOD_MAP
        cvtsd2si esp, xmm6      ;; cast(int)  rt_delta_x:DWORD 
        
       ;;    xmm6 <_ rt_delta_x_with rev
        pxor mm7, mm7 
        movd mm5, ebp 
        movd mm2, eax
        movd mm1, esi 
        movd mm0, ebx
        mov ebx, 00000000h
        movd mm6, ebx 
            
        xor ecx, ecx 
        xor edx, edx 

        ;;    eax <_  dstW        
        ;;    ebx <_  rt-total-X  
        ;;    ecx <_  temp 
        ;;    edx <_  temp        
        ;;    esi <_  source ptr poll
        ;;    edi <_  target ptr 
        ;;    ebp <_  temp
        ;;    esp <_  rt_delta_x 
        ;; 
        ;;    mm7 <_ rt-total-Y .  
        ;;    mm6 <_ rt_delta_x_with rev 
        ;;    mm5 <_ dstH
        ;;    mm4 <_ srcPitch 
        ;;    mm3 <_ rt_delta_y 
        ;;    mm2 <_ dstW 
        ;;    mm1 <_ source ptr base 
        ;;    mm0 <_ Target RVA 
align 16
doscanline:
        mov ebp, [esi+edx*2]
        mov edx, ebp 
if    1
        ;; LOW ready 
        mov ecx, ebp 
        shl ebp,  16 
        and ecx, 0FFFFH 
        or ebp, ecx  ;; ebp-x
        and ebp, 03E07C1FH
        ;; HIGH ready 
        mov ecx, edx 
        and edx, 0FFFF0000H
        shr ecx,  16
        or edx, ecx  ;; edx-y 
        and edx, 03E07C1FH
        sub edx, ebp ;; x-y 
else 
        ;; LOW ready 
        mov ecx, ebp 
        shl ebp,  16 
        mov bp, cx 
        and ebp, 03E07C1FH
        ;; HIGH ready 
        mov ecx, edx 
        shr ecx,  16
        mov dx, cx
        and edx, 03E07C1FH
        sub edx, ebp ;; x-y 
endif  
        comment @
        unsigned long _blender_trans15(unsigned long x, unsigned long y, unsigned long n)
        {
           unsigned long result;

           x = ((x & 0xFFFF) | (x << 16)) & 0x3E07C1F;
           y = ((y & 0xFFFF) | (y << 16)) & 0x3E07C1F;
                                                           
           result = ((x - y) * n / 32 + y) & 0x3E07C1F;

           return ((result & 0xFFFF) | (result >> 16));
        }
        @  
if      0        
        xor ecx, ecx                             
        
        bt ebx, MOD_SFT
        mov ecx, 31 
        
        shl ecx, 16 
        and ecx, ebx 
        
        shr ecx, 16 
        
        imul edx, ecx 
        shr edx, 5 
        
        add edx, ebp 
        and edx, 03E07C1FH
        
        mov ecx, edx 
        shr ecx, 16
        
        or ecx, edx 
        mov [edi], cx
elseif 0 
        mov ecx, ebx                             
        shr ecx, 16 
        and ecx, 31 
        
        imul edx, ecx 
        shr edx, 5 
        
        add edx, ebp 
        and edx, 03E07C1FH
        
        mov ecx, edx 
        shr ecx, 16
        
        or ecx, edx 
        mov [edi], cx
elseif 1  
        mov ecx, ebx                             
        shr ecx, 16 
        and ecx, 31 
        
        imul edx, ecx 
        shr edx, 5 
        
        add edx, ebp 
        and edx, 03E07C1FH
        
        mov ecx, edx 
        shr ecx, 16
        
        or ecx, edx 
        mov [edi], cx
endif   
        add ebx, esp   
        mov edx, ebx 
        shr edx, MOD_SFT
        add edi, 2
        dec eax 
        jne doscanline
        ;;   reset inline doscanline data/status, outloop check set 
        ;;   calc target pointer. 
        
        paddd mm7, mm3 
        movd edx, mm4 ;; SrcPitch  
        movd ebx, mm6 ;; revXTotalBase 
        movd ebp, mm0 ;; T- RVA.
        movd ecx, mm7 ;; Y- Total 
        movd eax, mm1 ;; source ptr base 
        
        shr ecx, MOD_SFT
        add edi, ebp 
        imul ecx, edx   ;; SRC Base Offset 
        lea esi, [eax+ecx] ;; source get 
        
        movd eax, mm2 ;; Te,pWidth 
        movd ecx, mm5 ;; TH 
        
        xor edx, edx 
        sub ecx, 1
        
        movd mm5, ecx ;; write back . 
        jne doscanline
V_EXIT: 
        movd    esp, xmm7
        emms 
        pop    ebp 
        pop    ebx 
        pop    esi 
        pop    edi    
        ret           
        
bilinear_x32K2 endp 
end


comment @

;; old code, Table driven mode is very slow because of the relationship between computer cache, so discard this way.
extern "C" WORD aptTab[0x8000][33]; 

void bilinear_x32K_init (void) {
  unsigned int _rgb15_idx;
  unsigned int _slot_idx; 
  for (_slot_idx = 0; _slot_idx > 32; _slot_idx++)  {
    for (_rgb15_idx = 0; _rgb15_idx != 0x8000; _rgb15_idx++)  {
       int out_ = _rgb15_idx | _rgb15_idx << 16;
       out_ = 0x3E07C1F;
       out_ *=_slot_idx;
       out_/= 32;
       aptTab[_rgb15_idx][_slot_idx] = ((out_ & 0xFFFF) | (out_ >> 16));
    }
  }
}

@