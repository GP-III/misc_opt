.text:0043D970 sub_43D970      proc near               ; CODE XREF: sub_445453+19Dp
.text:0043D970                                         ; sub_445453:loc_44566Cp ...
.text:0043D970
.text:0043D970 var_108         = dword ptr -108h
.text:0043D970 var_104         = dword ptr -104h
.text:0043D970 KeyState        = byte ptr -100h
.text:0043D970 var_FF          = byte ptr -0FFh
.text:0043D970 var_F3          = byte ptr -0F3h
.text:0043D970 var_F0          = byte ptr -0F0h
.text:0043D970 var_EF          = byte ptr -0EFh
.text:0043D970 var_ED          = byte ptr -0EDh
.text:0043D970 var_E7          = byte ptr -0E7h
.text:0043D970 var_E5          = byte ptr -0E5h
.text:0043D970 var_E4          = byte ptr -0E4h // 0x1C DIK_RETURN
.text:0043D970 var_E3          = byte ptr -0E3h // 0x1D DIK_LCONTROL
.text:0043D970 var_E1          = byte ptr -0E1h 
.text:0043D970 var_E0          = byte ptr -0E0h
.text:0043D970 var_DC          = byte ptr -0DCh
.text:0043D970 var_DB          = byte ptr -0DBh
.text:0043D970 var_DA          = byte ptr -0DAh
.text:0043D970 var_D9          = byte ptr -0D9h
.text:0043D970 var_D8          = byte ptr -0D8h
.text:0043D970 var_D6          = byte ptr -0D6h // 0x1D DIK_LSHIFT
.text:0043D970 var_D4          = byte ptr -0D4h // 0x2C DIK_Z
.text:0043D970 var_D3          = byte ptr -0D3h // 0x2D DIK_X
.text:0043D970 var_CA          = byte ptr -0CAh // 0x36 DIK_RSHIFT
.text:0043D970 var_BC          = byte ptr -0BCh
.text:0043D970 var_B9          = byte ptr -0B9h
.text:0043D970 var_B8          = byte ptr -0B8h // 0x48 DIK_NUMPAD8
.text:0043D970 var_B7          = byte ptr -0B7h
.text:0043D970 var_B5          = byte ptr -0B5h // 0x4B DIK_NUMPAD4
.text:0043D970 var_B3          = byte ptr -0B3h
.text:0043D970 var_B1          = byte ptr -0B1h
.text:0043D970 var_B0          = byte ptr -0B0h // 0x50 DIK_NUMPAD2
.text:0043D970 var_AF          = byte ptr -0AFh
.text:0043D970 var_AE          = byte ptr -0AEh
.text:0043D970 var_AD          = byte ptr -0ADh
.text:0043D970 var_A8          = byte ptr -0A8h
.text:0043D970 var_A6          = byte ptr -0A6h 
.text:0043D970 var_9F          = byte ptr -9Fh 
.text:0043D970 var_9E          = byte ptr -9Eh 
.text:0043D970 var_9D          = byte ptr -9Dh 
.text:0043D970 var_9C          = byte ptr -9Ch 
.text:0043D970 var_9A          = byte ptr -9Ah 
.text:0043D970 var_99          = byte ptr -99h 
.text:0043D970 var_98          = byte ptr -98h 
.text:0043D970 var_97          = byte ptr -97h 
.text:0043D970 var_63          = byte ptr -63h // 0x9D DIK_RCONTROL
.text:0043D970 var_39          = byte ptr -39h // 0xC7 DIK_HOME
.text:0043D970 var_38          = byte ptr -38h // 0xC8 DIK_UP
.text:0043D970 var_35          = byte ptr -35h // 0xCB DIK_LEFT
.text:0043D970 var_33          = byte ptr -33h // 0xCD DIK_RIGHT
.text:0043D970 var_30          = byte ptr -30h // 0xD0 DIK_DOWN

.text:0043D970 55                         push    ebp
.text:0043D971 8B EC                      mov     ebp, esp 
.text:0043D973 81 EC 08 01 00 00          sub     esp, 108h // 保存ctx, 开栈, 划栈
// 这段可以替换->
// C8 08 01 00          enter 108h,0 四个字节
// 53                   push ebx     一个字节
// 33 DB                xor  ebx,ebx 两个字节
// B3 80                mov   bl,80h 两个字节  刚刚好9个字节的操作码

.text:0043D979 66 C7 85 FC FE FF FF 00 00 mov     word ptr [ebp+var_104], 0  // IDinput8致能?
.text:0043D982 83 3D 68 E7 7C 01 00       cmp     dword_17CE768, 0 // IDinput8 == NULL?
.text:0043D989 0F 85 89 03 00 00          jnz     loc_43DD18 // != null goto loc_43DD18
// 略去无关的代码块
// loc_43DD18
// --------------------------------------------------------------------------------------------------------------------
.text:0043DD18                               loc_43DD18:                             ; CODE XREF: sub_43D970+19j
.text:0043DD18 8D 8D 00 FF FF FF              lea     ecx, [ebp+KeyState]
.text:0043DD1E 51                             push    ecx                 // 256的BYTE按键数组
.text:0043DD1F 68 00 01 00 00                 push    100h                // 当然是256 
.text:0043DD24 8B 15 68 E7 7C 01              mov     edx, dword_17CE768  // IDirectInputDevice8 this指针
.text:0043DD2A 8B 02                          mov     eax, [edx]          // 获取 __vfptr
.text:0043DD2C 8B 0D 68 E7 7C 01              mov     ecx, dword_17CE768
.text:0043DD32 51                             push    ecx                 // this指针
.text:0043DD33 FF 50 24                       call    dword ptr [eax+24h] // IDirectInputDevice8::GetDeviceState 
.text:0043DD36 89 85 F8 FE FF FF                             mov     [ebp+var_108], eax // HRESULT 结果
.text:0043DD3C 66 C7 85 FC FE FF FF 00 00                    mov     word ptr [ebp+var_104], 0
.text:0043DD45 81 BD F8 FE FF FF 1E 00 07 80                 cmp     [ebp+var_108], 8007001Eh // 设备丢失? 
.text:0043DD4F 75 22                                         jnz     short loc_43DD73 // 
.text:0043DD51 8B 15 68 E7 7C 01                             mov     edx, dword_17CE768
.text:0043DD57 8B 02                                         mov     eax, [edx]
.text:0043DD59 8B 0D 68 E7 7C 01                             mov     ecx, dword_17CE768
.text:0043DD5F 51                                            push    ecx
.text:0043DD60 FF 50 1C                                      call    dword ptr [eax+1Ch]
.text:0043DD63 8B 8D FC FE FF FF                             mov     ecx, [ebp+var_104] 
.text:0043DD69 E8 B2 F3 FF FF                                call    sub_43D120
.text:0043DD6E E9 E6 03 00 00                                jmp     loc_43E159
.text:0043DD73                               ; ---------------------------------------------------------------------------
.text:0043DD73
.text:0043DD73                               loc_43DD73:                             ; CODE XREF: sub_43D970+3DFj
.text:0043DD73 83 BD F8 FE FF FF 00                          cmp     [ebp+var_108], 0 // DI_OK??
.text:0043DD7A 74 22                                         jz      short loc_43DD9E // 获取键盘信息
.text:0043DD7C 8B 15 68 E7 7C 01                             mov     edx, dword_17CE768
.text:0043DD82 8B 02                                         mov     eax, [edx]
.text:0043DD84 8B 0D 68 E7 7C 01                             mov     ecx, dword_17CE768
.text:0043DD8A 51                                            push    ecx
.text:0043DD8B FF 50 1C                                      call    dword ptr [eax+1Ch]
.text:0043DD8E 8B 8D FC FE FF FF                             mov     ecx, [ebp+var_104]
.text:0043DD94 E8 87 F3 FF FF                                call    sub_43D120
.text:0043DD99 E9 BB 03 00 00                                jmp     loc_43E159
.text:0043DD9E                               ; ---------------------------------------------------------------------------
.text:0043DD9E

// 在这里需要对偏移量 <= 0x7F的按键进行代码的矫正, 以适应位置的 >0x7F偏移的按键设置 [+0~0x7F -0~-0x80为单字节偏移]
// 因为对于 基本操作 reg, reg 总是两个字节 [reg和reg合并为一个字节通过ModR/M[详见IA手册]。
// 所以可以压缩当前操作码来到达兼容效果, 对于多出来的空间, 填NOP即可
// 最明显的破绽便是and     edx, 80h 6个字节, 改成 and edx, ebx只需要两个字节
// 剩下四个字节绰绰有余
// ----------------------------------------------------------------------------------------

.text:0043DD9E                               loc_43DD9E:                             ; CODE XREF: sub_43D970+40Aj
.text:0043DD9E 0F B6 55 C8                                   movzx   edx, [ebp+var_38] // DIK_UP 需要扩容
.text:0043DDA2 81 E2 80 00 00 00                             and     edx, 80h
//.text:0043DD9E 0F B6 8D 48 FF FF FF                        movzx   edx, [ebp+var_38] // TYPE-01
//.text:0043DD9E 0F B6 8D 48 90 90 90                        movzx   edx, [ebp+var_38] // TYPE-02
//.text:0043DDA5 90                                          nop
//.text:0043DDA6 23 D3                                       and     edx, ebx
.text:0043DDA8 F7 DA                                         neg     edx
.text:0043DDAA 1B D2                                         sbb     edx, edx
.text:0043DDAC 83 E2 10                                      and     edx, 10h
.text:0043DDAF 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043DDB6 0B C2                                         or      eax, edx
.text:0043DDB8 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043DDBF 0F B6 4D D0                                   movzx   ecx, [ebp+var_30] // DIK_DOWN 需要扩容
.text:0043DDC3 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043DDC9 F7 D9                                         neg     ecx
.text:0043DDCB 1B C9                                         sbb     ecx, ecx
.text:0043DDCD 83 E1 20                                      and     ecx, 20h
.text:0043DDD0 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043DDD7 0B D1                                         or      edx, ecx
.text:0043DDD9 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043DDE0 0F B6 45 CB                                   movzx   eax, [ebp+var_35] // DIK_LEFT 需要扩容
.text:0043DDE4 25 80 00 00 00                                and     eax, 80h
.text:0043DDE9 F7 D8                                         neg     eax
.text:0043DDEB 1B C0                                         sbb     eax, eax
.text:0043DDED 83 E0 40                                      and     eax, 40h
.text:0043DDF0 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043DDF7 0B C8                                         or      ecx, eax
.text:0043DDF9 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043DE00 0F B6 55 CD                                   movzx   edx, [ebp+var_33] // DIK_RIGHT 需要扩容
.text:0043DE04 81 E2 80 00 00 00                             and     edx, 80h
.text:0043DE0A F7 DA                                         neg     edx
.text:0043DE0C 1B D2                                         sbb     edx, edx
.text:0043DE0E 81 E2 80 00 00 00                             and     edx, 80h
.text:0043DE14 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043DE1B 0B C2                                         or      eax, edx
.text:0043DE1D 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043DE24 0F B6 8D 48 FF FF FF                          movzx   ecx, [ebp+var_B8] // DIK_NUMPAD8
.text:0043DE2B 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043DE31 F7 D9                                         neg     ecx
.text:0043DE33 1B C9                                         sbb     ecx, ecx
.text:0043DE35 83 E1 10                                      and     ecx, 10h
.text:0043DE38 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043DE3F 0B D1                                         or      edx, ecx
.text:0043DE41 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043DE48 0F B6 85 50 FF FF FF                          movzx   eax, [ebp+var_B0] // DIK_NUMPAD2
.text:0043DE4F 25 80 00 00 00                                and     eax, 80h
.text:0043DE54 F7 D8                                         neg     eax
.text:0043DE56 1B C0                                         sbb     eax, eax
.text:0043DE58 83 E0 20                                      and     eax, 20h
.text:0043DE5B 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043DE62 0B C8                                         or      ecx, eax
.text:0043DE64 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043DE6B 0F B6 95 4B FF FF FF                          movzx   edx, [ebp+var_B5] // DIK_NUMPAD4
.text:0043DE72 81 E2 80 00 00 00                             and     edx, 80h
.text:0043DE78 F7 DA                                         neg     edx
.text:0043DE7A 1B D2                                         sbb     edx, edx
.text:0043DE7C 83 E2 40                                      and     edx, 40h
.text:0043DE7F 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043DE86 0B C2                                         or      eax, edx
.text:0043DE88 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043DE8F 0F B6 8D 4D FF FF FF                          movzx   ecx, [ebp+var_B3] // DIK_NUMPAD6
.text:0043DE96 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043DE9C F7 D9                                         neg     ecx
.text:0043DE9E 1B C9                                         sbb     ecx, ecx
.text:0043DEA0 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043DEA6 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043DEAD 0B D1                                         or      edx, ecx
.text:0043DEAF 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043DEB6 0F B6 85 47 FF FF FF                          movzx   eax, [ebp+var_B9] // DIK_NUMPAD7
.text:0043DEBD 25 80 00 00 00                                and     eax, 80h
.text:0043DEC2 F7 D8                                         neg     eax
.text:0043DEC4 1B C0                                         sbb     eax, eax
.text:0043DEC6 83 E0 50                                      and     eax, 50h
.text:0043DEC9 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043DED0 0B C8                                         or      ecx, eax
.text:0043DED2 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043DED9 0F B6 95 49 FF FF FF                          movzx   edx, [ebp+var_B7] // DIK_NUMPAD9
.text:0043DEE0 81 E2 80 00 00 00                             and     edx, 80h
.text:0043DEE6 F7 DA                                         neg     edx
.text:0043DEE8 1B D2                                         sbb     edx, edx
.text:0043DEEA 81 E2 90 00 00 00                             and     edx, 90h
.text:0043DEF0 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043DEF7 0B C2                                         or      eax, edx
.text:0043DEF9 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043DF00 0F B6 8D 4F FF FF FF                          movzx   ecx, [ebp+var_B1] // DIK_NUMPAD1
.text:0043DF07 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043DF0D F7 D9                                         neg     ecx
.text:0043DF0F 1B C9                                         sbb     ecx, ecx
.text:0043DF11 83 E1 60                                      and     ecx, 60h
.text:0043DF14 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043DF1B 0B D1                                         or      edx, ecx
.text:0043DF1D 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043DF24 0F B6 85 51 FF FF FF                          movzx   eax, [ebp+var_AF] // DIK_NUMPAD3
.text:0043DF2B 25 80 00 00 00                                and     eax, 80h
.text:0043DF30 F7 D8                                         neg     eax
.text:0043DF32 1B C0                                         sbb     eax, eax
.text:0043DF34 25 A0 00 00 00                                and     eax, 0A0h
.text:0043DF39 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043DF40 0B C8                                         or      ecx, eax
.text:0043DF42 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043DF49 0F B6 55 C7                                   movzx   edx, [ebp+var_39] // DIK_HOME 需要扩容
.text:0043DF4D 81 E2 80 00 00 00                             and     edx, 80h
.text:0043DF53 F7 DA                                         neg     edx
.text:0043DF55 1B D2                                         sbb     edx, edx
.text:0043DF57 81 E2 00 08 00 00                             and     edx, 800h
.text:0043DF5D 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043DF64 0B C2                                         or      eax, edx
.text:0043DF66 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043DF6D 0F B6 8D 19 FF FF FF                          movzx   ecx, [ebp+var_E7] // DIK_P
.text:0043DF74 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043DF7A F7 D9                                         neg     ecx
.text:0043DF7C 1B C9                                         sbb     ecx, ecx
.text:0043DF7E 81 E1 00 08 00 00                             and     ecx, 800h
.text:0043DF84 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043DF8B 0B D1                                         or      edx, ecx
.text:0043DF8D 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043DF94 0F B6 85 20 FF FF FF                          movzx   eax, [ebp+var_E0] // DIK_D
.text:0043DF9B 25 80 00 00 00                                and     eax, 80h
.text:0043DFA0 F7 D8                                         neg     eax
.text:0043DFA2 1B C0                                         sbb     eax, eax
.text:0043DFA4 25 00 20 00 00                                and     eax, 2000h
.text:0043DFA9 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043DFB0 0B C8                                         or      ecx, eax
.text:0043DFB2 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043DFB9 0F B6 95 2C FF FF FF                          movzx   edx, [ebp+var_D4] // DIK_Z
.text:0043DFC0 81 E2 80 00 00 00                             and     edx, 80h
.text:0043DFC6 F7 DA                                         neg     edx
.text:0043DFC8 1B D2                                         sbb     edx, edx
.text:0043DFCA F7 DA                                         neg     edx
.text:0043DFCC 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043DFD3 0B C2                                         or      eax, edx
.text:0043DFD5 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043DFDC 0F B6 8D 2D FF FF FF                          movzx   ecx, [ebp+var_D3] // DIK_X
.text:0043DFE3 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043DFE9 F7 D9                                         neg     ecx
.text:0043DFEB 1B C9                                         sbb     ecx, ecx
.text:0043DFED 83 E1 02                                      and     ecx, 2
.text:0043DFF0 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043DFF7 0B D1                                         or      edx, ecx
.text:0043DFF9 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043E000 0F B6 85 2A FF FF FF                          movzx   eax, [ebp+var_D6] // DIK_LSHIFT
.text:0043E007 25 80 00 00 00                                and     eax, 80h
.text:0043E00C F7 D8                                         neg     eax
.text:0043E00E 1B C0                                         sbb     eax, eax
.text:0043E010 83 E0 04                                      and     eax, 4
.text:0043E013 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043E01A 0B C8                                         or      ecx, eax
.text:0043E01C 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043E023 0F B6 95 36 FF FF FF                          movzx   edx, [ebp+var_CA] // DIK_RSHIFT
.text:0043E02A 81 E2 80 00 00 00                             and     edx, 80h
.text:0043E030 F7 DA                                         neg     edx
.text:0043E032 1B D2                                         sbb     edx, edx
.text:0043E034 83 E2 04                                      and     edx, 4
.text:0043E037 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043E03E 0B C2                                         or      eax, edx
.text:0043E040 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043E047 0F B6 8D 01 FF FF FF                          movzx   ecx, [ebp+var_FF] // DIK_ESCAPE
.text:0043E04E 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043E054 F7 D9                                         neg     ecx
.text:0043E056 1B C9                                         sbb     ecx, ecx
.text:0043E058 83 E1 08                                      and     ecx, 8
.text:0043E05B 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043E062 0B D1                                         or      edx, ecx
.text:0043E064 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043E06B 0F B6 85 1D FF FF FF                          movzx   eax, [ebp+var_E3] // DIK_LCONTROL
.text:0043E072 25 80 00 00 00                                and     eax, 80h
.text:0043E077 F7 D8                                         neg     eax
.text:0043E079 1B C0                                         sbb     eax, eax
.text:0043E07B 25 00 01 00 00                                and     eax, 100h
.text:0043E080 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043E087 0B C8                                         or      ecx, eax
.text:0043E089 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043E090 0F B6 55 9D                                   movzx   edx, [ebp+var_63] // DIK_RCONTROL 需要扩容
.text:0043E094 81 E2 80 00 00 00                             and     edx, 80h
.text:0043E09A F7 DA                                         neg     edx
.text:0043E09C 1B D2                                         sbb     edx, edx
.text:0043E09E 81 E2 00 01 00 00                             and     edx, 100h
.text:0043E0A4 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043E0AB 0B C2                                         or      eax, edx
.text:0043E0AD 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043E0B4 0F B6 8D 10 FF FF FF                          movzx   ecx, [ebp+var_F0] // DIK_Q
.text:0043E0BB 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043E0C1 F7 D9                                         neg     ecx
.text:0043E0C3 1B C9                                         sbb     ecx, ecx
.text:0043E0C5 81 E1 00 02 00 00                             and     ecx, 200h
.text:0043E0CB 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043E0D2 0B D1                                         or      edx, ecx
.text:0043E0D4 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043E0DB 0F B6 85 1F FF FF FF                          movzx   eax, [ebp+var_E1] // DIK_S
.text:0043E0E2 25 80 00 00 00                                and     eax, 80h
.text:0043E0E7 F7 D8                                         neg     eax
.text:0043E0E9 1B C0                                         sbb     eax, eax
.text:0043E0EB 25 00 04 00 00                                and     eax, 400h
.text:0043E0F0 0F B7 8D FC FE FF FF                          movzx   ecx, word ptr [ebp+var_104]
.text:0043E0F7 0B C8                                         or      ecx, eax
.text:0043E0F9 66 89 8D FC FE FF FF                          mov     word ptr [ebp+var_104], cx
.text:0043E100 0F B6 95 1C FF FF FF                          movzx   edx, [ebp+var_E4] // DIK_RETURN
.text:0043E107 81 E2 80 00 00 00                             and     edx, 80h
.text:0043E10D F7 DA                                         neg     edx
.text:0043E10F 1B D2                                         sbb     edx, edx
.text:0043E111 81 E2 00 10 00 00                             and     edx, 1000h
.text:0043E117 0F B7 85 FC FE FF FF                          movzx   eax, word ptr [ebp+var_104]
.text:0043E11E 0B C2                                         or      eax, edx
.text:0043E120 66 89 85 FC FE FF FF                          mov     word ptr [ebp+var_104], ax
.text:0043E127 0F B6 8D 13 FF FF FF                          movzx   ecx, [ebp+var_ED] // DIK_R
.text:0043E12E 81 E1 80 00 00 00                             and     ecx, 80h
.text:0043E134 F7 D9                                         neg     ecx
.text:0043E136 1B C9                                         sbb     ecx, ecx
.text:0043E138 81 E1 00 40 00 00                             and     ecx, 4000h
.text:0043E13E 0F B7 95 FC FE FF FF                          movzx   edx, word ptr [ebp+var_104]
.text:0043E145 0B D1                                         or      edx, ecx
.text:0043E147 66 89 95 FC FE FF FF                          mov     word ptr [ebp+var_104], dx
.text:0043E14E
.text:0043E14E                               loc_43E14E:                             ; CODE XREF: sub_43D970+3A3j
.text:0043E14E 8B 8D FC FE FF FF                             mov     ecx, [ebp+var_104]
.text:0043E154 E8 C7 EF FF FF                                call    sub_43D120        // 对按键掩码进行处理
.text:0043E159
.text:0043E159                               loc_43E159:                             ; CODE XREF: sub_43D970+3FEj
.text:0043E159                                                                       ; sub_43D970+429j
.text:0043E159 8B E5                                         mov     esp, ebp
.text:0043E15B 5D                                            pop     ebp
.text:0043E15C C3                                            retn
.text:0043E15C                               sub_43D970      endp