.text:0044858D                         ; =============== S U B R O U T I N E =======================================
.text:0044858D
.text:0044858D                         ; Attributes: bp-based frame
.text:0044858D
.text:0044858D                         ; int __stdcall sub_44858D(char *, int, int)
.text:0044858D                         sub_44858D proc near                    ; CODE XREF: sub_445453+700p
.text:0044858D                                                                 ; sub_451D90+1ABp ...
.text:0044858D
.text:0044858D                         var_2C= dword ptr -2Ch
.text:0044858D                         var_28= dword ptr -28h
.text:0044858D                         var_22= byte ptr -22h
.text:0044858D                         var_21= byte ptr -21h
.text:0044858D                         var_20= dword ptr -20h
.text:0044858D                         var_1C= dword ptr -1Ch
.text:0044858D                         var_18= dword ptr -18h
.text:0044858D                         var_14= dword ptr -14h
.text:0044858D                         var_10= dword ptr -10h
.text:0044858D                         var_C= dword ptr -0Ch
.text:0044858D                         var_8= dword ptr -8
.text:0044858D                         var_4= dword ptr -4
.text:0044858D                         arg_0= dword ptr  8
.text:0044858D                         arg_4= dword ptr  0Ch
.text:0044858D                         arg_8= dword ptr  10h
.text:0044858D
.text:0044858D 55                      push    ebp
.text:0044858E 8B EC                   mov     ebp, esp
.text:00448590 83 EC 2C                sub     esp, 2Ch
.text:00448593 89 4D E8                mov     [ebp+var_18], ecx
.text:00448596 8B 45 E8                mov     eax, [ebp+var_18]
.text:00448599 83 B8 60 03 00 00 00    cmp     dword ptr [eax+360h], 0
.text:004485A0 75 07                   jnz     short loc_4485A9
.text:004485A2 33 C0                   xor     eax, eax
.text:004485A4 E9 0D 01 00 00          jmp     locret_4486B6
.text:004485A9                         ; ---------------------------------------------------------------------------
.text:004485A9
.text:004485A9                         loc_4485A9:                             ; CODE XREF: sub_44858D+13j
.text:004485A9 8B 45 E8                mov     eax, [ebp+var_18]
.text:004485AC 8B 80 60 03 00 00       mov     eax, [eax+360h]
.text:004485B2 89 45 FC                mov     [ebp+var_4], eax
.text:004485B5 8B 45 E8                mov     eax, [ebp+var_18]
.text:004485B8 8B 80 5C 03 00 00       mov     eax, [eax+35Ch]
.text:004485BE 89 45 F4                mov     [ebp+var_C], eax
.text:004485C1 6A 05                   push    5                               ; size_t
.text:004485C3 68 08 6D 4B 00          push    offset aDebug                   ; "debug"
.text:004485C8 FF 75 08                push    [ebp+arg_0]                     ; char *
.text:004485CB E8 10 D7 05 00          call    _strncmp
.text:004485D0 83 C4 0C                add     esp, 0Ch
.text:004485D3 85 C0                   test    eax, eax
.text:004485D5 75 07                   jnz     short loc_4485DE
.text:004485D7 33 C0                   xor     eax, eax
.text:004485D9 E9 D8 00 00 00          jmp     locret_4486B6
.text:004485DE                         ; ---------------------------------------------------------------------------
.text:004485DE
.text:004485DE                         loc_4485DE:                             ; CODE XREF: sub_44858D+48j
.text:004485DE C7 45 E4 08 6D 4B 00    mov     [ebp+var_1C], offset aDebug     ; "debug"
.text:004485E5 C7 45 E0 0C 66 4B 00    mov     [ebp+var_20], offset a0100d     ; "0100d"
.text:004485EC
.text:004485EC                         loc_4485EC:                             ; CODE XREF: sub_44858D+91j
.text:004485EC 8B 45 E0                mov     eax, [ebp+var_20]
.text:004485EF 8A 00                   mov     al, [eax]
.text:004485F1 88 45 DF                mov     [ebp+var_21], al
.text:004485F4 8B 4D E4                mov     ecx, [ebp+var_1C]
.text:004485F7 3A 01                   cmp     al, [ecx]
.text:004485F9 75 2B                   jnz     short loc_448626
.text:004485FB 80 7D DF 00             cmp     [ebp+var_21], 0
.text:004485FF 74 1F                   jz      short loc_448620
.text:00448601 8B 45 E0                mov     eax, [ebp+var_20]
.text:00448604 8A 40 01                mov     al, [eax+1]
.text:00448607 88 45 DE                mov     [ebp+var_22], al
.text:0044860A 8B 4D E4                mov     ecx, [ebp+var_1C]
.text:0044860D 3A 41 01                cmp     al, [ecx+1]
.text:00448610 75 14                   jnz     short loc_448626
.text:00448612 83 45 E0 02             add     [ebp+var_20], 2
.text:00448616 83 45 E4 02             add     [ebp+var_1C], 2
.text:0044861A 80 7D DE 00             cmp     [ebp+var_22], 0
.text:0044861E 75 CC                   jnz     short loc_4485EC
.text:00448620
.text:00448620                         loc_448620:                             ; CODE XREF: sub_44858D+72j
.text:00448620 83 65 D8 00             and     [ebp+var_28], 0
.text:00448624 EB 08                   jmp     short loc_44862E
.text:00448626                         ; ---------------------------------------------------------------------------
.text:00448626
.text:00448626                         loc_448626:                             ; CODE XREF: sub_44858D+6Cj
.text:00448626                                                                 ; sub_44858D+83j
.text:00448626 1B C0                   sbb     eax, eax
.text:00448628 83 D8 FF                sbb     eax, 0FFFFFFFFh
.text:0044862B 89 45 D8                mov     [ebp+var_28], eax
.text:0044862E
.text:0044862E                         loc_44862E:                             ; CODE XREF: sub_44858D+97j
.text:0044862E 8B 45 D8                mov     eax, [ebp+var_28]
.text:00448631 89 45 D4                mov     [ebp+var_2C], eax
.text:00448634 83 7D D4 00             cmp     [ebp+var_2C], 0
.text:00448638 75 04                   jnz     short loc_44863E
.text:0044863A 33 C0                   xor     eax, eax
.text:0044863C EB 78                   jmp     short locret_4486B6
.text:0044863E                         ; ---------------------------------------------------------------------------
.text:0044863E
.text:0044863E                         loc_44863E:                             ; CODE XREF: sub_44858D+ABj
.text:0044863E                                                                 ; sub_44858D+124j
.text:0044863E 83 7D F4 00             cmp     [ebp+var_C], 0
.text:00448642 76 6F                   jbe     short loc_4486B3                ; jmp loc_4486B3-> EB 6F 
.text:00448644 6A 05                   push    5                               ; size_t
.text:00448646 FF 75 FC                push    [ebp+var_4]                     ; char *
.text:00448649 FF 75 08                push    [ebp+arg_0]                     ; char *
.text:0044864C E8 8F D6 05 00          call    _strncmp
.text:00448651 83 C4 0C                add     esp, 0Ch
.text:00448654 85 C0                   test    eax, eax
.text:00448656 75 35                   jnz     short loc_44868D
.text:00448658 8B 45 FC                mov     eax, [ebp+var_4]
.text:0044865B 83 C0 06                add     eax, 6
.text:0044865E 89 45 FC                mov     [ebp+var_4], eax
.text:00448661 8D 45 F8                lea     eax, [ebp+var_8]
.text:00448664 50                      push    eax
.text:00448665 8D 45 EC                lea     eax, [ebp+var_14]
.text:00448668 50                      push    eax
.text:00448669 68 00 6D 4B 00          push    offset aDD                      ; "%d %d"
.text:0044866E FF 75 FC                push    [ebp+var_4]                     ; char *
.text:00448671 E8 28 D6 05 00          call    _sscanf
.text:00448676 83 C4 10                add     esp, 10h
.text:00448679 8B 45 EC                mov     eax, [ebp+var_14]
.text:0044867C 3B 45 0C                cmp     eax, [ebp+arg_4]
.text:0044867F 75 0C                   jnz     short loc_44868D
.text:00448681 8B 45 F8                mov     eax, [ebp+var_8]
.text:00448684 3B 45 10                cmp     eax, [ebp+arg_8]
.text:00448687 75 04                   jnz     short loc_44868D
.text:00448689 33 C0                   xor     eax, eax
.text:0044868B EB 29                   jmp     short locret_4486B6
.text:0044868D                         ; ---------------------------------------------------------------------------
.text:0044868D
.text:0044868D                         loc_44868D:                             ; CODE XREF: sub_44858D+C9j
.text:0044868D                                                                 ; sub_44858D+F2j ...
.text:0044868D 8B 45 FC                mov     eax, [ebp+var_4]
.text:00448690 89 45 F0                mov     [ebp+var_10], eax
.text:00448693 6A 0A                   push    0Ah                             ; int
.text:00448695 FF 75 FC                push    [ebp+var_4]                     ; char *
.text:00448698 E8 43 D5 05 00          call    _strchr
.text:0044869D 59                      pop     ecx
.text:0044869E 59                      pop     ecx
.text:0044869F 40                      inc     eax
.text:004486A0 89 45 FC                mov     [ebp+var_4], eax
.text:004486A3 8B 45 FC                mov     eax, [ebp+var_4]
.text:004486A6 2B 45 F0                sub     eax, [ebp+var_10]
.text:004486A9 8B 4D F4                mov     ecx, [ebp+var_C]
.text:004486AC 2B C8                   sub     ecx, eax
.text:004486AE 89 4D F4                mov     [ebp+var_C], ecx
.text:004486B1 EB 8B                   jmp     short loc_44863E
.text:004486B3                         ; ---------------------------------------------------------------------------
.text:004486B3
.text:004486B3                         loc_4486B3:                             ; CODE XREF: sub_44858D+B5j
.text:004486B3 83 C8 FF                or      eax, 0FFFFFFFFh                 ; -> and eax, 0 | 83 E0 00 
.text:004486B6
.text:004486B6                         locret_4486B6:                          ; CODE XREF: sub_44858D+17j
.text:004486B6                                                                 ; sub_44858D+4Cj ...
.text:004486B6 C9                      leave
.text:004486B7 C2 0C 00                retn    0Ch
.text:004486B7                         sub_44858D endp