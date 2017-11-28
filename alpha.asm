;*********************************************************************************************************************
; Image Alpha Blend-SSE4
; Formula: output:= saturation addition (desalpha*RGB target components, srcalpha*RGB source components)
; C formula: despixel = des_orgpixel * [BYTE] des_alpha_count / 256
; Srcpixel = src_orgpixel * [BYTE] src_alpha_count / 256
; Output = sutaradd (despixel, srcpixel)
; Derivation: 
;     output = (src_orgpixel[R|G|B] * src_alpha_count + des_orgpixel[R|G|B] * des_alpha_count) / 256
; Due to the pmaddubsw relationship, the coefficient is reduced from 255 to 127
;
; TODO:more case, more mode, MMX???                                                                              
; TODO:not mine, not mine, ...http://wm.ite.pl/articles/sse4-crossfading.html, thanks                     -  moecmks
;*********************************************************************************************************************

    .386
    .486
    .586
    .mmx
    .xmm
    .model flat, c

;; Set thread custom user data [keep ESP]
ifdef __X86_32_
    STF_SAVE equ 0700h 
elseifdef __X86_WOW64_ 
    STF_SAVE equ 01ACh
elseifdef __X86_64_ 
    STF_SAVE equ 02D0h 
else 
    STF_SAVE equ 01ACh 
endif 
;;  Program/memory code alignment
    ALIGN_Z  equ align 16

    .data
ALIGN_Z
alpha_init DD 0 

    .data?
ALIGN_Z
shuffleRes_lo dd 4 dup(?)
shuffleRes_hi dd 4 dup(?)

    .code

alphaX86 proc C
    option prologue:none, epilogue:none
IF   0
    ;; LOCK 0 The global table is not initialized 
    ;; LOCK 1 Busy, need to wait
    ;; LOCK 2 Global table initialization complete 
    
    xor eax, eax 
    mov edx, 1 
    lock cmpxchg dword ptr[alpha_init], edx 
    jz lockinit 
    
    ;; Waiting for the global table to initialize is complete, 
    ;; and this is a spin process
    .while eax == 1
       ;; Spin optimization
       pause 
       mov  eax, [alpha_init]
    .endw 
ENDIF
    ;; Parameter base offset
    pDst equ 4 
    xDst equ 8 
    yDst equ 12 
    hDst equ 16
    aDst equ 20
    pSrc equ 24
    xSrc equ 28
    ySrc equ 32
    hSrc equ 36
    aSrc equ 40
    cWidth equ 44 
    cHeight equ 48 
    
mainentry:
    push esi 
    push edi 
    push ebx 
    push ebp     ;; save  context  register  
    
; save  esp 
assume  fs:nothing 
    mov fs:[STF_SAVE], esp 
    cld 
    
    mov edi, [esp+pDst + 16] ;; edi <- target pointer
    mov ebx, [esp+xDst + 16] ;; ebx <- target x
    mov ecx, [esp+yDst + 16] ;; ecx <- target y
    mov edx, [esp+hDst + 16] ;; edx <- target pitch 
    
    shl ebx, 2   
    add edi, ebx 
    imul ecx, edx 
    add edi, ecx          ;; edi <- target(x, y)'s start pointer 
    
    mov esi, [esp+pSrc+ 16] ;; esi <- source pointer
    mov ebx, [esp+xSrc+ 16] ;; ebx <- source x
    mov ecx, [esp+ySrc+ 16] ;; ecx <- source y
    mov ebp, [esp+hSrc+ 16] ;; ebp <- source pitch  
   
    shl ebx, 2   
    add esi, ebx 
    imul ecx, ebp 
    add esi, ecx          ;; esi <- source(x, y)'s start pointer 
    
    mov ecx, [esp+cWidth +16] ;; ecx <- width 
    mov ebx, [esp+cHeight +16] ;; ebx <- height 
    
    test ecx, ecx         
    jz V_EXIT
    
    test ebx, ebx          
    jz V_EXIT
    
    lea eax, [ecx*4] 
    sub edx, eax          ;; edx <- target RVA
    sub ebp, eax          ;; ebp <- source RVA
    mov eax, esp          ;; eax <- esp' cache 
    mov esp, ecx          ;; esp <- width's cache 
    nop                      ;; spare 
    
    ;; Get alpha into the SSE register
    movss xmm7, dword ptr[eax+aSrc+ 16]
    pinsrb xmm7, byte ptr[eax+aDst+ 16], 1
    
    ;; re-array
    ;; vpbroadcastw xmm7, xmm7 
    punpcklwd xmm7, xmm7 
    pshufd xmm7, xmm7, 0
    
    and eax, 3 
    nop ; sparce 

    cmp ecx, 3 
    jbe vsmall 
    
    test ecx, 3 
    jz dopure 

;; ----------------------------------------------------------------------------------
;; SSE-unwind base 
;; ----------------------------------------------------------------------------------
sCopyIn_unwind macro label_, ldcs, ldcd, wrcd

ALIGN_Z
label_:
    ldcs xmm1, xmmword ptr[esi]
    ldcd xmm2, xmmword ptr[edi] 
    
    movq xmm0, xmm1      ;; Source pixel low 4 word cache

    ;; Cross decompression 
    punpckhbw xmm1, xmm2 ;; DWORD[0] = G2<D>-G2<S>-B2<D>-B2<S>
                         ;; DWORD[1] = A2<D>-A2<S>-R2<D>-R2<S>
                         ;; DWORD[2] = G3<D>-G3<S>-B3<D>-B3<S>
                         ;; DWORD[3] = A3<D>-A3<S>-R3<D>-R3<S>
    ;; Cross decompression 
    punpcklbw xmm0, xmm2 ;; DWORD[0] = G0<D>-G0<S>-B0<D>-B0<S>
                         ;; DWORD[1] = A0<D>-A0<S>-R0<D>-R0<S>
                         ;; DWORD[2] = G1<D>-G1<S>-B1<D>-B1<S>
                         ;; DWORD[3] = A1<D>-A1<S>-R1<D>-R1<S>

    ;; Cross multiply saturated RGB. [XXX: signed!!!]
    ;;
    ;;  L1: 
    ;;     xmm0-word[0] = d[B0]*d[Alpha]+s[B0]*s[Alpha]
    ;;     xmm0-word[1] = d[G0]*d[Alpha]+s[G0]*s[Alpha]
    ;;     xmm0-word[2] = d[R0]*d[Alpha]+s[R0]*s[Alpha]
    ;;     xmm0-word[3] = d[A0]*d[Alpha]+s[A0]*s[Alpha]
    ;;     xmm0-word[4] = d[B1]*d[Alpha]+s[B1]*s[Alpha]
    ;;     xmm0-word[5] = d[G1]*d[Alpha]+s[G1]*s[Alpha]
    ;;     xmm0-word[6] = d[R1]*d[Alpha]+s[R1]*s[Alpha]
    ;;     xmm0-word[7] = d[A1]*d[Alpha]+s[A1]*s[Alpha]
    ;;
    ;;  L2:same as L1
    ;; ----------------------------------------------------------------------------------

    pmaddubsw xmm0, xmm7 
    pmaddubsw xmm1, xmm7 

    ;; /128
    psrlw xmm0, 7 
    psrlw xmm1, 7

    ;; packpage rgb..!
    packuswb xmm0, xmm1
    ;; write target
    wrcd [edi], xmm0
    
    add esi, 16 
    add edi, 16 
    
    sub ecx, 4 
    jnz label_
endm ;; !! sCopyIn_unwind
    
ALIGN_Z  
dodirty0:
    and  ecx, 0FFFFFFFCh
    mov  eax, esp 
    and  eax, 3
    ;; -----------------------------------------------------------------------------------------------
    ;; dodirty unwind 
    ;; -----------------------------------------------------------------------------------------------
    sCCdirtyunwind macro label_, ldcs, ldcd, wrcd
      sCopyIn_unwind label_, ldcs, ldcd, wrcd
      
        ;; remain doop ... 
      smloop&label_&:
        movd xmm1, dword ptr[esi]
        movd xmm2, dword ptr[edi] 
        
        punpcklbw xmm1, xmm2 
        pmaddubsw xmm1, xmm7 

        ;; /128
        psrlw xmm1, 7 
        packuswb xmm1, xmm1
        ;; write target
        movd dword ptr[edi], xmm1
        
        add esi, 4 
        add edi, 4 
        
        dec eax 
        jnz smloop&label_&

        add edi, edx 
        add esi, ebp 
        mov ecx, esp 
        mov eax, esp 
        and ecx, 0FFFFFFFCh
        and eax, 3
        dec ebx 
        jnz label_
        jmp V_EXIT 
    endm

    ;; align test 
    .if !(esi & 15)
       .if !(edi & 15) 
         ;; target align/source align 
          sCCdirtyunwind dirtyloopTASA, movdqa, movdqa, movdqa 
       .else 
         ;; target unalign/source align 
          sCCdirtyunwind dirtyloopTUSA, movdqa, lddqu, movdqu 
       .endif 
    .else 
       .if !(edi & 15) 
         ;; target align/source unalign 
          sCCdirtyunwind dirtyloopTASU, lddqu, movdqa, movdqa 
       .else 
         ;; target unalign/source unalign 
          sCCdirtyunwind dirtyloopTUSU, lddqu, lddqu, movdqu 
       .endif 
    .endif 
    
ALIGN_Z
dopure:
    ;; -----------------------------------------------------------------------------------------------
    ;; dopure unwind 
    ;; -----------------------------------------------------------------------------------------------
    sCCpureunwind macro label_, ldcs, ldcd, wrcd
      sCopyIn_unwind label_, ldcs, ldcd, wrcd
      
      add edi, edx 
      add esi, ebp 
      mov ecx, esp 
      xor eax, eax 
      dec ebx 
      jnz label_
      jmp V_EXIT
      
    endm
    
    ;; align test 
    .if !(esi & 15)
       .if !(edi & 15) 
         ;; target align/source align 
          sCCpureunwind pureloopTASA, movdqa, movdqa, movdqa 
       .else 
         ;; target unalign/source align 
          sCCpureunwind pureloopTUSA, movdqa, lddqu, movdqu 
       .endif 
    .else 
       .if !(edi & 15) 
         ;; target align/source unalign 
          sCCpureunwind pureloopTASU, lddqu, movdqa, movdqa 
       .else 
         ;; target unalign/source unalign 
          sCCpureunwind pureloopTUSU, lddqu, lddqu, movdqu 
       .endif 
    .endif 
    
ALIGN_Z
vsmall:
    movd xmm1, dword ptr[esi]
    movd xmm2, dword ptr[edi] 
    
    punpcklbw xmm1, xmm2 
    pmaddubsw xmm1, xmm7 

    ;; /128
    psrlw xmm1, 7 
    packuswb xmm1, xmm1
    ;; write target
    movd dword ptr[edi], xmm1
    
    add esi, 4 
    add edi, 4 
    
    dec eax 
    jnz vsmall

    add edi, edx 
    add esi, ebp 
    mov ecx, esp 
    nop 
    dec ebx 
    jnz vsmall
V_EXIT:
    mov esp, fs:[STF_SAVE]
    pop ebp 
    pop ebx
    pop edi 
    pop esi 
    ret  
     
lockinit:
    ;; Initialize the alpha table used by the shuffle command
    ;; left 
    mov eax, 07050301H
    mov [shuffleRes_lo], eax 
    mov eax, 0F0D0B09H
    mov [shuffleRes_lo+4],eax 
    mov eax, 0FFFFFFFFH
    mov [shuffleRes_lo+8], eax 
    mov eax, 0FFFFFFFFH
    mov [shuffleRes_lo+12],eax  
    ;; right 
    mov eax, 0FFFFFFFFH
    mov [shuffleRes_hi],  eax 
    mov eax, 0FFFFFFFFH
    mov [shuffleRes_hi+4], eax 
    mov eax, 07050301H
    mov [shuffleRes_hi+8], eax 
    mov eax, 0F0D0B09H
    mov [shuffleRes_hi+12], eax 
    ;; unlock 
    mov [alpha_init], 2 
    
    jmp mainentry
        
alphaX86 endp 
  end 

