#include "molios.h"

char board_s0[] = 
{
  /*.text:00000000*/ 0xB9, 0x58, 0xB8, 0x4B, 0x00,//                          mov     ecx, 4BB858h
  /*.text:00000005*/ 0x83, 0x39, 0x00,//                                cmp     dword ptr [ecx], 0
  /*.text:00000008*/ 0x75, 0x30,//                                   jnz     short $dosub$83684
  /*.text:0000000A*/ 0x68, 0xDC, 0x68, 0x4C, 0x00,//                          push    4C68DCh
  /*.text:0000000F*/ 0x6A, 0x20,//                                   push    20h ; ' '
  /*.text:00000011*/ 0x68, 0x00, 0x06, 0x00, 0x00,//                          push    600h
  /*.text:00000016*/ 0x68, 0x50, 0xB2, 0x4B, 0x00,//                          push    4BB250h
  /*.text:0000001B*/ 0xB8, 0x14, 0x41, 0x4B, 0x00,//                          mov     eax, 4B4114h
  /*.text:00000020*/ 0xFF, 0x10,//                                   call    dword ptr [eax]
  /*.text:00000022*/ 0x68, 0xDC, 0x68, 0x4C, 0x00,//                          push    4C68DCh
  /*.text:00000027*/ 0x6A, 0x04,//                                   push    4
  /*.text:00000029*/ 0x68, 0x20, 0x01, 0x00, 0x00,//                          push    120h
  /*.text:0000002E*/ 0x68, 0x50, 0xB8, 0x4B, 0x00,//                          push    4BB850h
  /*.text:00000033*/ 0xB8, 0x14, 0x41, 0x4B, 0x00,//                          mov     eax, 4B4114h
  /*.text:00000038*/ 0xFF, 0x10,//                                   call    dword ptr [eax]
  /*.text:0000003A*/
  /*.text:0000003A                      $dosub$83684:                           ; CODE XREF: ___board+8j  **/
  /*.text:0000003A*/ 0x90,//                                      nop
  /*.text:0000003B*/ 0x90,//                                      nop
  /*.text:0000003C*/ 0x90,//                                      nop
  /*.text:0000003D*/ 0x90,//                                      nop
  /*.text:0000003E*/ 0x90,//                                      nop
  /*.text:0000003F*/ 0x90,//                                      nop
  /*.text:00000040*/ 0x68, 0x50, 0xB9, 0x4B, 0x00,//                          push    4BB950h
  /*.text:00000045*/ 0x68, 0x50, 0xB8, 0x4B, 0x00,//                          push    4BB850h
  /*.text:0000004A*/ 0x8D, 0x85, 0x3B, 0xFF, 0xFF, 0xFF,//                       lea     eax, [ebp-0C5h]
  /*.text:00000050*/ 0x50,//                                      push    eax
  /*.text:00000051*/ 0x51,//                                      push    ecx
  /*.text:00000052*/ 0xB8, 0x50, 0xB2, 0x4B, 0x00,//                          mov     eax, 4BB250h
  /*.text:00000057*/ 0xFF, 0xD0,//                                   call    eax
  /*.text:00000059*/ 0xB8, 0x9E, 0xDD, 0x43, 0x00,//                          mov     eax, 43DD9Eh
  /*.text:0000005E*/ 0xFF, 0xE0 //                                   jmp     eax  /* .text:00000005 */ 0x83, 0x39, 0x00,/*                             cmp     dword ptr [ecx],/**/
};

void __make_lea_ecx_ebpoffset8or32 (BYTE *buf_appect, BYTE DIK_code_set ) 
{ 
  //  LEA 
  //  8D /r
  //  LEA r32,m
  buf_appect[0] = 0x8D; 
  // ecx /r == 001
  // mod 01 or 10 (ebp+[disp8|disp32])
  // R/M 101 
  buf_appect[1] = 1 << 3; // set R
  buf_appect[1] |= 5; // set R/M 
  if ( (0x100 - DIK_code_set) <= 0x80) {
    buf_appect[1] |= 0x40; // set MOD
    buf_appect[2] = DIK_code_set;
  } else {
    buf_appect[1] |= 0x80; // set MOD 
    buf_appect[2] = DIK_code_set;
    buf_appect[3] = 0xFF;
    buf_appect[4] = 0xFF;
    buf_appect[5] = 0xFF;
  }
}

void __board_write (HANDLE fHandle, BYTE DIK_set) {
  // ÉèÖÃÌø°å PEµØÖ· 0x3DB73 
  // Ìø°åµØÖ· 0x004B3B78 PE 0xB3978
  // .text:0043DD73 83 BD F8 FE FF FF 00                    cmp     [ebp+var_108], 0
  // .text:0043DD7A 74 22                                   jz      short loc_43DD9E
  // 6:   __asm mov eax, 0x12345678 
  // 00CA139E B8 78 56 34 12       mov         eax,12345678h  
  // 7:   __asm jmp eax 
  // 00CA13A3 FF E0                jmp         eax 
  char dummy [] = 
  {
    0xB8, 0x78, 0x3B, 0x4B, 0x00, // mov         eax,12345678h  
    0xFF, 0xE0,                   // jmp         eax 
    0x90, 0x90
  };
  DWORD temps__;
  * (UINT32 *)& dummy[1] = MOLIOS_BOARD_SELF;
  SetFilePointer (fHandle, MOLIOS_PEBOARD_ENTRY, NULL, FILE_BEGIN);
  WriteFile (fHandle, dummy, sizeof (dummy), & temps__, NULL);
  __make_lea_ecx_ebpoffset8or32 (& board_s0[0x3A], DIK_set);
  SetFilePointer (fHandle, MOLIOS_PEBOARD_SELF, NULL, FILE_BEGIN);
  WriteFile (fHandle, board_s0, sizeof (board_s0), & temps__, NULL);
}