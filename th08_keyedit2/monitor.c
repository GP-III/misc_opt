

char monitor_s0 [] = {
  //    0x20,//: void __stdcall __window_main_ (struct __CrackDataSegment *dataSeg,
  //    0x21,//:                      struct __CrackRdataSegmentPointer   *rdataSegp)
  //    0x22,//: {
  /*00FA1000*/ 0x55,//                   push        ebp  
  /*00FA1001*/ 0x8B, 0xEC,//                mov         ebp,esp  
  /*00FA1003*/ 0x83, 0xEC, 0x2C,//             sub         esp,2Ch  
  /*00FA1006*/ 0x53,//                   push        ebx  
  /*00FA1007*/ 0x56,//                   push        esi  
  /*00FA1008*/ 0x57,//                   push        edi  
  //    0x23,//:   HWND hwnd;
  //    0x24,//:   WNDCLASSA wndclass ;
  //    0x25,//:   HINSTANCE inst = (HINSTANCE) (* (HMODULE (WINAPI **)(LPCSTR)) (IAT_GetModuleHandleA)) (NULL);
  /*00FA1009*/ 0x33, 0xDB,//                xor         ebx,ebx  
  /*00FA100B*/ 0x53,//                   push        ebx  
  /*00FA100C*/ 0xFF, 0x15, 0xE0, 0x40, 0x4B, 0x00,//    call        dword ptr ds:[4B40E0h]  
  /*00FA1012*/ 0x8B, 0xD0,//                mov         edx,eax  
  //    0x26,//: 
  //    0x27,//:   ZeroMemory (& wndclass, sizeof (wndclass));
  /*00FA1014*/ 0x6A, 0x0A,//                push        0Ah  
  /*00FA1016*/ 0x59,//                   pop         ecx  
  /*00FA1017*/ 0x33, 0xC0,//                xor         eax,eax  
  /*00FA1019*/ 0x8D, 0x7D, 0xD4,//             lea         edi,[wndclass]  
  /*00FA101C*/ 0xF3, 0xAB,//                rep stos    dword ptr es:[edi]  
  //    0x28,//:   wndclass.lpfnWndProc = dataSeg->wndCallback ;
  /*00FA101E*/ 0x8B, 0x7D, 0x08,//             mov         edi,dword ptr [dataSeg]  
  /*00FA1021*/ 0x8B, 0x07,//                mov         eax,dword ptr [edi]  
  //    0x29,//:   wndclass.hInstance = inst ;
  //    0x30,//:   wndclass.hCursor = (* (HCURSOR (WINAPI **)(HINSTANCE, LPCSTR)) (IAT_LoadCursorA)) (NULL, MAKEINTRESOURCEA(32512));
  /*00FA1023*/ 0x68, 0x00, 0x7F, 0x00, 0x00,//       push        7F00h  
  /*00FA1028*/ 0x53,//                   push        ebx  
  /*00FA1029*/ 0x89, 0x55, 0xFC,//             mov         dword ptr [inst],edx  
  /*00FA102C*/ 0x89, 0x45, 0xD8,//             mov         dword ptr [ebp-28h],eax  
  /*00FA102F*/ 0x89, 0x55, 0xE4,//             mov         dword ptr [ebp-1Ch],edx  
  /*00FA1032*/ 0xFF, 0x15, 0xFC, 0x41, 0x4B, 0x00,//    call        dword ptr ds:[4B41FCh]  
  //    0x31,//:   wndclass.hbrBackground = (HBRUSH) (* (HGDIOBJ (WINAPI **)(int)) (IAT_GetStockObject)) (DKGRAY_BRUSH);
  /*00FA1038*/ 0x6A, 0x03,//                push        3  
  /*00FA103A*/ 0x89, 0x45, 0xEC,//             mov         dword ptr [ebp-14h],eax  
  /*00FA103D*/ 0xFF, 0x15, 0x44, 0x40, 0x4B, 0x00,//    call        dword ptr ds:[4B4044h]  
  //    0x32,//:   wndclass.lpszClassName = rdataSegp->classNameString ;
  /*00FA1043*/ 0x8B, 0x75, 0x0C,//             mov         esi,dword ptr [rdataSegp]  
  /*00FA1046*/ 0x89, 0x45, 0xF0,//             mov         dword ptr [ebp-10h],eax  
  /*00FA1049*/ 0x8B, 0x46, 0x2C,//             mov         eax,dword ptr [esi+2Ch]  
  /*00FA104C*/ 0x89, 0x45, 0xF8,//             mov         dword ptr [ebp-8],eax  
  //    0x33,//:   (* (ATOM (WINAPI **)(WNDCLASSA *)) (IAT_RegisterClassA)) (& wndclass);
  /*00FA104F*/ 0x8D, 0x45, 0xD4,//             lea         eax,[wndclass]  
  /*00FA1052*/ 0x50,//                   push        eax  
  /*00FA1053*/ 0xFF, 0x15, 0x00, 0x42, 0x4B, 0x00,//    call        dword ptr ds:[4B4200h]  
  //    0x34,//: 
  //    0x35,//:   // ------------------------------------------------------------------------------------
  //    0x36,//:   hwnd = (* (HWND (WINAPI **)(__in DWORD dwExStyle,
  //    0x37,//:                       __in_opt LPCSTR lpClassName,
  //    0x38,//:                       __in_opt LPCSTR lpWindowName,
  //    0x39,//:                       __in DWORD dwStyle,
  //    0x40,//:                       __in int X,
  //    0x41,//:                       __in int Y,
  //    0x42,//:                       __in int nWidth,
  //    0x43,//:                       __in int nHeight,
  //    0x44,//:                       __in_opt HWND hWndParent,
  //    0x45,//:                       __in_opt HMENU hMenu,
  //    0x46,//:                       __in_opt HINSTANCE hInstance,
  //    0x47,//:                       __in_opt LPVOID lpParam)) 
  //    0x48,//:      (IAT_CreateWindowExA)) ( 0L,
  //    0x49,//:                       rdataSegp->classNameString, 
  //    0x50,//:                       rdataSegp->captionString, 
  //    0x51,//:                       WS_OVERLAPPEDWINDOW&~WS_SIZEBOX, 
  //    0x52,//:                       0, 
  //    0x53,//:                       0, 
  //    0x54,//:                       250, 
  //    0x55,//:                       300, 
  //    0x56,//:                       NULL, 
  //    0x57,//:                       NULL, 
  //    0x58,//:                       inst, 
  //    0x59,//:                       NULL);
  /*00FA1059*/ 0x53,//                   push        ebx  
  /*00FA105A*/ 0xFF, 0x75, 0xFC,//             push        dword ptr [inst]  
  /*00FA105D*/ 0x53,//                   push        ebx  
  /*00FA105E*/ 0x53,//                   push        ebx  
  /*00FA105F*/ 0x68, 0x2C, 0x01, 0x00, 0x00,//       push        12Ch  
  /*00FA1064*/ 0x68, 0xFA, 0x00, 0x00, 0x00,//       push        0FAh  
  /*00FA1069*/ 0x53,//                   push        ebx  
  /*00FA106A*/ 0x53,//                   push        ebx  
  /*00FA106B*/ 0x68, 0x00, 0x00, 0xCB, 0x00,//       push        0CB0000h  
  /*00FA1070*/ 0xFF, 0x76, 0x34,//             push        dword ptr [esi+34h]  
  /*00FA1073*/ 0xFF, 0x76, 0x2C,//             push        dword ptr [esi+2Ch]  
  /*00FA1076*/ 0x53,//                   push        ebx  
  /*00FA1077*/ 0xFF, 0x15, 0x04, 0x42, 0x4B, 0x00,//    call        dword ptr ds:[4B4204h]  
  //    0x60,//: 
  //    0x61,//:   // ------------------------------------------------------------------------------------
  //    0x62,//:   (* (HWND (WINAPI **)(__in DWORD dwExStyle,
  //    0x63,//:                       __in_opt LPCSTR lpClassName,
  //    0x64,//:                       __in_opt LPCSTR lpWindowName,
  //    0x65,//:                       __in DWORD dwStyle,
  //    0x66,//:                       __in int X,
  //    0x67,//:                       __in int Y,
  //    0x68,//:                       __in int nWidth,
  //    0x69,//:                       __in int nHeight,
  //    0x70,//:                       __in_opt HWND hWndParent,
  //    0x71,//:                       __in_opt HMENU hMenu,
  //    0x72,//:                       __in_opt HINSTANCE hInstance,
  //    0x73,//:                       __in_opt LPVOID lpParam)) 
  //    0x74,//:      (IAT_CreateWindowExA)) ( 0L,
  //    0x75,//:                       rdataSegp->buttonString, 
  //    0x76,//:                       rdataSegp->statusString, 
  //    0x77,//:                       WS_CHILD | WS_VISIBLE|BS_GROUPBOX|WS_GROUP, 
  //    0x78,//:                       4, 6, 206, 108,
  //    0x79,//:                       hwnd, 0, inst, NULL);
  /*00FA107D*/ 0x53,//                   push        ebx  
  /*00FA107E*/ 0xFF, 0x75, 0xFC,//             push        dword ptr [inst]  
  /*00FA1081*/ 0x89, 0x45, 0x08,//             mov         dword ptr [dataSeg],eax  
  /*00FA1084*/ 0x53,//                   push        ebx  
  /*00FA1085*/ 0x50,//                   push        eax  
  /*00FA1086*/ 0x6A, 0x6C,//                push        6Ch  
  /*00FA1088*/ 0x68, 0xCE, 0x00, 0x00, 0x00,//       push        0CEh  
  /*00FA108D*/ 0x6A, 0x06,//                push        6  
  /*00FA108F*/ 0x6A, 0x04,//                push        4  
  /*00FA1091*/ 0x68, 0x07, 0x00, 0x02, 0x50,//       push        50020007h  
  /*00FA1096*/ 0xFF, 0x76, 0x3C,//             push        dword ptr [esi+3Ch]  
  /*00FA1099*/ 0xFF, 0x76, 0x30,//             push        dword ptr [esi+30h]  
  /*00FA109C*/ 0x53,//                   push        ebx  
  /*00FA109D*/ 0xFF, 0x15, 0x04, 0x42, 0x4B, 0x00,//    call        dword ptr ds:[4B4204h]  
  //    0x80,//: 
  //    0x81,//:   // ------------------------------------------------------------------------------------
  //    0x82,//:   dataSeg->wndStatus = (* (HWND (WINAPI **)(__in DWORD dwExStyle,
  //    0x83,//:                       __in_opt LPCSTR lpClassName,
  //    0x84,//:                       __in_opt LPCSTR lpWindowName,
  //    0x85,//:                       __in DWORD dwStyle,
  //    0x86,//:                       __in int X,
  //    0x87,//:                       __in int Y,
  //    0x88,//:                       __in int nWidth,
  //    0x89,//:                       __in int nHeight,
  //    0x90,//:                       __in_opt HWND hWndParent,
  //    0x91,//:                       __in_opt HMENU hMenu,
  //    0x92,//:                       __in_opt HINSTANCE hInstance,
  //    0x93,//:                       __in_opt LPVOID lpParam)) 
  //    0x94,//:      (IAT_CreateWindowExA)) ( 0L,
  //    0x95,//:                       rdataSegp->editClassString, 
  //    0x96,//:                       rdataSegp->emptyString, 
  //    0x97,//:                       WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | ES_READONLY, 
  //    0x98,//:                       0x10,//, 0x22,//, 194, 0x90,//,
  //    0x99,//:                       hwnd, 0, inst, NULL);
  /*00FA10A3*/ 0x53,//                   push        ebx  
  /*00FA10A4*/ 0xFF, 0x75, 0xFC,//             push        dword ptr [inst]  
  /*00FA10A7*/ 0x53,//                   push        ebx  
  /*00FA10A8*/ 0xFF, 0x75, 0x08,//             push        dword ptr [dataSeg]  
  /*00FA10AB*/ 0x6A, 0x5A,//                push        5Ah  
  /*00FA10AD*/ 0x68, 0xC2, 0x00, 0x00, 0x00,//       push        0C2h  
  /*00FA10B2*/ 0x6A, 0x16,//                push        16h  
  /*00FA10B4*/ 0x6A, 0x0A,//                push        0Ah  
  /*00FA10B6*/ 0x68, 0xC4, 0x18, 0x00, 0x50,//       push        500018C4h  
  /*00FA10BB*/ 0xFF, 0x76, 0x28,//             push        dword ptr [esi+28h]  
  /*00FA10BE*/ 0xFF, 0x76, 0x38,//             push        dword ptr [esi+38h]  
  /*00FA10C1*/ 0x53,//                   push        ebx  
  /*00FA10C2*/ 0xFF, 0x15, 0x04, 0x42, 0x4B, 0x00,//    call        dword ptr ds:[4B4204h]  
  //   100: 
  //   101:   // ------------------------------------------------------------------------------------
  //   102:   (* (HWND (WINAPI **)(__in DWORD dwExStyle,
  //   103:                       __in_opt LPCSTR lpClassName,
  //   104:                       __in_opt LPCSTR lpWindowName,
  //   105:                       __in DWORD dwStyle,
  //   106:                       __in int X,
  //   107:                       __in int Y,
  //   108:                       __in int nWidth,
  //   109:                       __in int nHeight,
  //   110:                       __in_opt HWND hWndParent,
  //   111:                       __in_opt HMENU hMenu,
  //   112:                       __in_opt HINSTANCE hInstance,
  //   113:                       __in_opt LPVOID lpParam)) 
  //   114:      (IAT_CreateWindowExA)) ( 0L,
  //   115:                       rdataSegp->buttonString, 
  //   116:                       rdataSegp->eventString, 
  //   117:                       WS_CHILD | WS_VISIBLE|BS_GROUPBOX|WS_GROUP, 
  //   118:                       4, 6+120, 206, 108,
  //   119:                       hwnd, 0, inst, NULL);
  /*00FA10C8*/ 0x53,//                   push        ebx  
  /*00FA10C9*/ 0xFF, 0x75, 0xFC,//             push        dword ptr [inst]  
  /*00FA10CC*/ 0x89, 0x47, 0x54,//             mov         dword ptr [edi+54h],eax  
  /*00FA10CF*/ 0x53,//                   push        ebx  
  /*00FA10D0*/ 0xFF, 0x75, 0x08,//             push        dword ptr [dataSeg]  
  /*00FA10D3*/ 0x6A, 0x6C,//                push        6Ch  
  /*00FA10D5*/ 0x68, 0xCE, 0x00, 0x00, 0x00,//       push        0CEh  
  /*00FA10DA*/ 0x6A, 0x7E,//                push        7Eh  
  /*00FA10DC*/ 0x6A, 0x04,//                push        4  
  /*00FA10DE*/ 0x68, 0x07, 0x00, 0x02, 0x50,//       push        50020007h  
  /*00FA10E3*/ 0xFF, 0x76, 0x40,//             push        dword ptr [esi+40h]  
  /*00FA10E6*/ 0xFF, 0x76, 0x30,//             push        dword ptr [esi+30h]  
  /*00FA10E9*/ 0x53,//                   push        ebx  
  /*00FA10EA*/ 0xFF, 0x15, 0x04, 0x42, 0x4B, 0x00,//    call        dword ptr ds:[4B4204h]  
  //   120: 
  //   121:   // ------------------------------------------------------------------------------------
  //   122:   dataSeg->wndEvent = (* (HWND (WINAPI **)(__in DWORD dwExStyle,
  //   123:                       __in_opt LPCSTR lpClassName,
  //   124:                       __in_opt LPCSTR lpWindowName,
  //   125:                       __in DWORD dwStyle,
  //   126:                       __in int X,
  //   127:                       __in int Y,
  //   128:                       __in int nWidth,
  //   129:                       __in int nHeight,
  //   130:                       __in_opt HWND hWndParent,
  //   131:                       __in_opt HMENU hMenu,
  //   132:                       __in_opt HINSTANCE hInstance,
  //   133:                       __in_opt LPVOID lpParam)) 
  //   134:      (IAT_CreateWindowExA)) ( 0L,
  //   135:                       rdataSegp->editClassString, 
  //   136:                       rdataSegp->emptyString, 
  //   137:                       WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | ES_READONLY, 
  //   138:                       0x10,//, 0x22,//+120, 194, 0x90,//,
  //   139:                       hwnd, 0, inst, NULL);
  /*00FA10F0*/ 0x53,//                   push        ebx  
  /*00FA10F1*/ 0xFF, 0x75, 0xFC,//             push        dword ptr [inst]  
  /*00FA10F4*/ 0x53,//                   push        ebx  
  /*00FA10F5*/ 0xFF, 0x75, 0x08,//             push        dword ptr [dataSeg]  
  /*00FA10F8*/ 0x6A, 0x5A,//                push        5Ah  
  /*00FA10FA*/ 0x68, 0xC2, 0x00, 0x00, 0x00,//       push        0C2h  
  /*00FA10FF*/ 0x68, 0x8E, 0x00, 0x00, 0x00,//       push        8Eh  
  /*00FA1104*/ 0x6A, 0x0A,//                push        0Ah  
  /*00FA1106*/ 0x68, 0xC4, 0x18, 0x00, 0x50,//       push        500018C4h  
  /*00FA110B*/ 0xFF, 0x76, 0x28,//             push        dword ptr [esi+28h]  
  /*00FA110E*/ 0xFF, 0x76, 0x38,//             push        dword ptr [esi+38h]  
  /*00FA1111*/ 0x53,//                   push        ebx  
  /*00FA1112*/ 0xFF, 0x15, 0x04, 0x42, 0x4B, 0x00,//    call        dword ptr ds:[4B4204h]  
  /*00FA1118*/ 0x89, 0x47, 0x58,//             mov         dword ptr [edi+58h],eax  
  //   140: 
  //   141:   dataSeg->holdWindow = 1;
  /*00FA111B*/ 0x33, 0xC0,//                xor         eax,eax  
  /*00FA111D*/ 0x40,//                   inc         eax  
  //   142: 
  //   143:   (* (BOOL (WINAPI **)(HWND, int)) 
  //   144:      (IAT_ShowWindow)) ( hwnd, SW_SHOWNORMAL);
  /*00FA111E*/ 0x50,//                   push        eax  
  /*00FA111F*/ 0xFF, 0x75, 0x08,//             push        dword ptr [dataSeg]  
  /*00FA1122*/ 0x89, 0x47, 0x60,//             mov         dword ptr [edi+60h],eax  
  /*00FA1125*/ 0xFF, 0x15, 0xC8, 0x41, 0x4B, 0x00,//    call        dword ptr ds:[4B41C8h]  
  /*00FA112B*/ 0x5F,//                   pop         edi  
  /*00FA112C*/ 0x5E,//                   pop         esi  
  /*00FA112D*/ 0x5B,//                   pop         ebx  
  //   145: }
  /*00FA112E*/ 0xC9,//                   leave  
  /*00FA112F*/ 0xC2, 0x08, 0x00//             ret         8  
};

char wncls_s0 [] = {

  0xB8, 0x02, 0x00, 0x00, 0x00,                          //   mov     eax, 2
  0x3B, 0x44, 0x24, 0x08,                               // cmp     eax, [esp+arg_4]
  0x75, 0x0E,                                    //  jnz     short $dosub$83691
  0x33, 0xC0,                                    //  xor     eax, eax
  0x8D, 0x80, 0xB0, 0xB8, 0x4B, 0x00,             //             lea     eax, [eax+4BB8B0h]
  0xC7, 0x00, 0x00, 0x00, 0x00, 0x00,             //             mov     dword ptr [eax], 0
  0xB8, 0xF8, 0x41, 0x4B, 0x00,                  //           mov     eax, 4B41F8h
  0x8B, 0x00,                                    //  mov     eax, [eax]
  0xFF, 0xE0                                    //  jmp     eax
};

#include "molios.h"

void __monitor_write (HANDLE fHandle) {

  // 写入回掉和参数 
  DWORD temps__;

  // monitor
  SetFilePointer (fHandle, MOLIOS_PEPROGRAM_ADDRESS + MOLIOS_PRGOFFSET_WINDOW, NULL, FILE_BEGIN);
  WriteFile (fHandle, monitor_s0, sizeof (monitor_s0), & temps__, NULL);
  // callback 
  SetFilePointer (fHandle, MOLIOS_PEPROGRAM_ADDRESS + MOLIOS_PRGOFFSET_CALLBACK, NULL, FILE_BEGIN);
  WriteFile (fHandle, wncls_s0, sizeof (wncls_s0), & temps__, NULL);
  if   (              1           )   {
  /* 写入   WNDPROC  wndCallback;// XXX:预设置 ! 控件回调
  void (WINAPI * createStatusWindow) (struct __CrackDataSegment *dataSeg, struct __CrackRdataSegmentPointer *rdataSegp); // XXX:预设置 ! 窗口创建函数
  */
    DWORD cbPointer[2] = { MOLIOS_PROGRAM_ADDRESS + MOLIOS_PRGOFFSET_CALLBACK, 
                           MOLIOS_PROGRAM_ADDRESS + MOLIOS_PRGOFFSET_WINDOW };
    SetFilePointer (fHandle, MOLIOS_PEPGRAM_ADDRESS + MOLIOS_PGRAMOFFSET_DATASEG, NULL, FILE_BEGIN);
    WriteFile (fHandle, cbPointer, sizeof (cbPointer), & temps__, NULL);
  }
}