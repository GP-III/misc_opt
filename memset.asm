;*********************************************************************************************************************
; VRAM colour fill
; TODO:more case, more mode                                                                              -  moecmks
;*********************************************************************************************************************

    .386
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

    .code

vsetX86 proc C
    option prologue:none, epilogue:none

    ;; Parameter base offset
    pDst equ 4 
    xDst equ 8 
    yDst equ 12 
    hDst equ 16
    aPixel equ 20
    cWidth equ 24 
    cHeight equ 28 
    
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
    mov ebp, [esp+hDst + 16] ;; ebp <- target pitch 
    
    shl ebx, 2   
    add edi, ebx 
    imul ecx, ebp 
    add edi, ecx          ;; edi <- target(x, y)'s start pointer 
    
    mov ecx, [esp+cWidth +16] ;; ecx <- width 
    mov ebx, [esp+cHeight +16] ;; ebx <- height 
    
    test ecx, ecx         
    jz V_EXIT
    
    test ebx, ebx          
    jz V_EXIT
    
    lea eax, [ecx*4] 
    sub ebp, eax          ;; ebp <- target RVA
    mov eax, esp          ;; eax <- esp' cache 
    mov esi, ecx          ;; esi <- width's cache 
    nop 
    ;; Get RGB array into the SSE register
    mov eax, [eax+aPixel+ 16] 
    
    movd xmm7, eax 
    pshufd xmm7, xmm7, 0
    
    ;;  register dispatch 
    ;;  -------------------------------------------------------
    ;;  eax <- colour 
    ;;  ebx <- height 
    ;;  ecx <- width 
    ;;  ebp <- RVA 
    ;;  esi <- width cache 
    ;;  edi <- target pointer 
    ;;  edx <- spare [maybe tail count]
    ;;  esp <- spare [maybe tail count's cache]
    ;;  -------------------------------------------------------
    
    ;; ----------------------------------------------------------------------------------
    ;;  SSE-unwind base 
    ;; ----------------------------------------------------------------------------------
    sCopyInUnwind macro label_, offset0__

      ;; write target
      movdqa [edi+offset0__], xmm7 ;; movntdqa 
      movdqa [edi+16+offset0__], xmm7
      
      lea edi, [edi+32] 
      nop 

      sub ecx, 8
      jnz label_
    endm ;;; !! sCopyInUnwind
    
    sCopyHeadDirty1 macro 
      mov [edi], eax 
    endm ;;; !! sCopyHeadDirty
    
    sCopyHeadDirty2 macro 
      mov [edi], eax 
      mov [edi+4], eax 
    endm ;;; !! sCopyHeadDirty2
    
    sCopyHeadDirty3 macro 
      mov [edi], eax 
      mov [edi+4], eax 
      mov [edi+8], eax
    endm ;;; !! sCopyHeadDirty3
    
    sCopyTailPure macro label_

      add edi, ebp  
      mov ecx, esi  
      dec ebx 
      jnz label_
      jmp V_EXIT
    endm ;;; !! sCopyTailPure
    
    sCopyTailDirty macro label_
        
    @@:
      mov [edi], eax 
      dec edx 
      lea edi, [edi+4]
      jnz @B
      add edi, ebp  
      mov ecx, esi 
      dec ebx 
      mov edx, esp 
      jnz label_
      jmp V_EXIT
    endm ;;; !! sCopyTailDirty
    
    sSaveTailCtx macro 
        
      mov esp, ecx 
      and esp, 7 
      and ecx, 0FFFFFFF8h
      and esi, 0FFFFFFF8h
      mov edx, esp
    endm ;;; !! sSaveTailCtx
    
    cmp ecx, 7 
    jbe NORMAL0 
    
    test edi, 15 
    jz HA_CCHK 
    
    ;; head unalign 
    ;; first save some context !
    mov edx, edi 
    and edx, 15     ;;; 0000-4 | 0100-3 | 1000-2 | 1100-1
    shr edx, 2 
    sub edx, 4 
    neg edx 

    mov esp, ecx 
    sub esp, edx 
    
    cmp esp, 7
    jbe NORMAL0 
    
    test esp, 7 
    jz HUTA
    
    .if edx == 1
       sSaveTailCtx
      ALIGN_Z
      TUX1_loop0cc:
       sCopyHeadDirty1
      ALIGN_Z
      TUX1_loop:
       sCopyInUnwind TUX1_loop, 4
       sCopyTailDirty TUX1_loop0cc
    .elseif edx == 2 
       sSaveTailCtx
      ALIGN_Z
      TUX2_loop0cc:
       sCopyHeadDirty2
      ALIGN_Z
      TUX2_loop:
       sCopyInUnwind TUX2_loop, 8
       sCopyTailDirty TUX2_loop0cc
    .elseif edx == 3
       sSaveTailCtx
      ALIGN_Z
      TUX3_loop0cc:
       sCopyHeadDirty3
      ALIGN_Z
      TUX3_loop:
       sCopyInUnwind TUX3_loop, 12
       sCopyTailDirty TUX3_loop0cc
    .endif 
    
ALIGN_Z
HUTA:  
    .if edx == 1
      ALIGN_Z
      ccTUX1_loop0cc:
       sCopyHeadDirty1
      ALIGN_Z
      ccTUX1_loop:
       sCopyInUnwind ccTUX1_loop, 4
       sCopyTailDirty ccTUX1_loop0cc
    .elseif edx == 2 
      ALIGN_Z
      ccTUX2_loop0cc:
       sCopyHeadDirty2
      ALIGN_Z
      ccTUX2_loop:
       sCopyInUnwind ccTUX2_loop, 8
       sCopyTailDirty ccTUX2_loop0cc
    .elseif edx == 3
      ALIGN_Z
      ccTUX3_loop0cc:
       sCopyHeadDirty3
      ALIGN_Z
      ccTUX3_loop:
       sCopyInUnwind ccTUX3_loop, 12
       sCopyTailDirty ccTUX3_loop0cc
    .endif 
ALIGN_Z
HA_CCHK:
    ;; head align !! 
    test ecx, 7 
    jz HATA
    
    sSaveTailCtx
    ;; head align/tail remain 
ALIGN_Z
HATU:
    sCopyInUnwind HATU, 0
    sCopyTailDirty HATU
    ;; head align/tail noremain
ALIGN_Z
HATA:
    sCopyInUnwind HATA, 0
    sCopyTailPure HATA
    ;; small copy 
ALIGN_Z
NORMAL0:
    mov [edi], eax 
    dec ecx 
    lea edi, [edi+4]
    jnz NORMAL0
    add edi, ebp  
    mov ecx, esi 
    dec ebx 
    jnz NORMAL0
V_EXIT:
    mov esp, fs:[STF_SAVE]
    pop ebp 
    pop ebx
    pop edi 
    pop esi 
    ret  
        
vsetX86 endp 
  end 

