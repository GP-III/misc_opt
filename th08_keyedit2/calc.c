#include "molios.h"

// BYTES:0x2FC 
// ~= 0x300 
char calc_s0[] = 
{
  // --- c:\users\nozaki\documents\visual studio 2010\projects\th08_keyedit\calc_src.c 
  //     1: #include "molios.h"
  //     2: 
  //     3: void __stdcall __crack_main_ (BYTE *DIK_shift, BYTE *curF1set, 
  //     4:                      struct __CrackDataSegment *dataSeg,
  //     5:                      struct __CrackRdataSegmentPointer   *rdataSegp)
  //     6: {
  /*00E61000*/ 0x55,//                   push        ebp  
  /*00E61001*/ 0x8B, 0xEC,//                mov         ebp,esp  
  /*00E61003*/ 0x81, 0xEC, 0xDC, 0x00, 0x00, 0x00,//    sub         esp,0DCh  
  /*00E61009*/ 0x53,//                   push        ebx  
  //     7:     const char *evt_DescGroup[2] = { NULL, NULL };
  //     8:     BYTE F1_cache[8]; 
  //     9:     BOOL bRecalcNK = FALSE;
  //    0x10,//:     INT id;
  //    0x11,//:     
  //    0x12,//:     if (dataSeg->Global_Init == 0) 
  /*00E6100A*/ 0x8B, 0x5D, 0x10,//             mov         ebx,dword ptr [ebp+10h]  
  /*00E6100D*/ 0x33, 0xC0,//                xor         eax,eax  
  /*00E6100F*/ 0x56,//                   push        esi  
  //    0x13,//:     {
  //    0x14,//:       // 模拟IAT调用 
  //    0x15,//:       HMODULE ntdll = (* (HMODULE (WINAPI **)(LPCSTR)) (IAT_GetModuleHandleA)) (rdataSegp->DLL_ntdll);
  /*00E61010*/ 0x8B, 0x75, 0x14,//             mov         esi,dword ptr [ebp+14h]  
  /*00E61013*/ 0x57,//                   push        edi  
  /*00E61014*/ 0x89, 0x45, 0xF4,//             mov         dword ptr [evt_DescGroup],eax  
  /*00E61017*/ 0x89, 0x45, 0xF8,//             mov         dword ptr [ebp-8],eax  
  /*00E6101A*/ 0x89, 0x45, 0xFC,//             mov         dword ptr [bRecalcNK],eax  
  /*00E6101D*/ 0x39, 0x43, 0x08,//             cmp         dword ptr [ebx+8],eax  
  /*00E61020*/ 0x75, 0x46,//                jne         __crack_main_+68h (0E61068h)  
  /*00E61022*/ 0xFF, 0x36,//                push        dword ptr [esi]  
  /*00E61024*/ 0xFF, 0x15, 0xE0, 0x40, 0x4B, 0x00,//    call        dword ptr ds:[4B40E0h]  
  //    0x16,//:       HMODULE user32 = (* (HMODULE (WINAPI **)(LPCSTR)) (IAT_GetModuleHandleA)) (rdataSegp->DLL_user32);
  /*00E6102A*/ 0xFF, 0x76, 0x04,//             push        dword ptr [esi+4]  
  /*00E6102D*/ 0x89, 0x45, 0x10,//             mov         dword ptr [dataSeg],eax  
  /*00E61030*/ 0xFF, 0x15, 0xE0, 0x40, 0x4B, 0x00,//    call        dword ptr ds:[4B40E0h]  
  //    0x17,//:       dataSeg->__sprintf = (void *) (* (HMODULE (WINAPI **)(HMODULE, LPCSTR)) (IAT_GetProcAddress)) (ntdll, rdataSegp->CRTAPI_sprintf);
  /*00E61036*/ 0xFF, 0x76, 0x08,//             push        dword ptr [esi+8]  
  /*00E61039*/ 0x8B, 0xF8,//                mov         edi,eax  
  /*00E6103B*/ 0xFF, 0x75, 0x10,//             push        dword ptr [dataSeg]  
  /*00E6103E*/ 0xFF, 0x15, 0xD8, 0x40, 0x4B, 0x00,//    call        dword ptr ds:[4B40D8h]  
  /*00E61044*/ 0x89, 0x43, 0x0C,//             mov         dword ptr [ebx+0Ch],eax  
  //    0x18,//:       dataSeg->setWindowTextA = (void *) (* (HMODULE (WINAPI **)(HMODULE, LPCSTR)) (IAT_GetProcAddress)) (user32, rdataSegp->WINAPI_SetWindowTextA);
  /*00E61047*/ 0xFF, 0x76, 0x0C,//             push        dword ptr [esi+0Ch]  
  /*00E6104A*/ 0x57,//                   push        edi  
  /*00E6104B*/ 0xFF, 0x15, 0xD8, 0x40, 0x4B, 0x00,//    call        dword ptr ds:[4B40D8h]  
  /*00E61051*/ 0x89, 0x43, 0x14,//             mov         dword ptr [ebx+14h],eax  
  //    0x19,//:       dataSeg->sendMessageA = (void *) (* (HMODULE (WINAPI **)(HMODULE, LPCSTR)) (IAT_GetProcAddress)) (user32, rdataSegp->WINAPI_SendMessageA);
  /*00E61054*/ 0xFF, 0x76, 0x10,//             push        dword ptr [esi+10h]  
  /*00E61057*/ 0x57,//                   push        edi  
  /*00E61058*/ 0xFF, 0x15, 0xD8, 0x40, 0x4B, 0x00,//    call        dword ptr ds:[4B40D8h]  
  /*00E6105E*/ 0x89, 0x43, 0x10,//             mov         dword ptr [ebx+10h],eax  
  //    0x20,//:       dataSeg->Global_Init = 1;
  /*00E61061*/ 0xC7, 0x43, 0x08, 0x01, 0x00, 0x00, 0x00,// mov         dword ptr [ebx+8],1  
  //    0x21,//:     }
  //    0x22,//: 
  //    0x23,//:     // 清除杂质,放入按键缓存
  //    0x24,//:     for (id = 0; id != 8; id++)
  /*00E61068*/ 0x8B, 0x4D, 0x0C,//             mov         ecx,dword ptr [curF1set]  
  /*00E6106B*/ 0x8D, 0x45, 0xEC,//             lea         eax,[F1_cache]  
  /*00E6106E*/ 0x33, 0xFF,//                xor         edi,edi  
  /*00E61070*/ 0x2B, 0xC8,//                sub         ecx,eax  
  //    0x25,//:       F1_cache[id] = curF1set[id] & 0x80;
  /*00E61072*/ 0x8D, 0x44, 0x3D, 0xEC,//          lea         eax,F1_cache[edi]  
  /*00E61076*/ 0x8A, 0x14, 0x01,//             mov         dl,byte ptr [ecx+eax]  
  /*00E61079*/ 0x80, 0xE2, 0x80,//             and         dl,80h  
  /*00E6107C*/ 0x47,//                   inc         edi  
  /*00E6107D*/ 0x88, 0x10,//                mov         byte ptr [eax],dl  
  /*00E6107F*/ 0x83, 0xFF, 0x08,//             cmp         edi,8  
  /*00E61082*/ 0x75, 0xEE,//                jne         __crack_main_+72h (0E61072h)  
  //    0x26,//: 
  //    0x27,//:     if (curF1set[0] && ! dataSeg->F1array[0]) {
  /*00E61084*/ 0x8B, 0x55, 0x0C,//             mov         edx,dword ptr [curF1set]  
  /*00E61087*/ 0x80, 0x3A, 0x00,//             cmp         byte ptr [edx],0  
  /*00E6108A*/ 0x74, 0x22,//                je          __crack_main_+0AEh (0E610AEh)  
  /*00E6108C*/ 0x80, 0x7B, 0x18, 0x00,//          cmp         byte ptr [ebx+18h],0  
  /*00E61090*/ 0x75, 0x1C,//                jne         __crack_main_+0AEh (0E610AEh)  
  //    0x28,//:       if (dataSeg->megaPower == 0) 
  /*00E61092*/ 0x8B, 0x43, 0x5C,//             mov         eax,dword ptr [ebx+5Ch]  
  /*00E61095*/ 0x85, 0xC0,//                test        eax,eax  
  /*00E61097*/ 0x75, 0x05,//                jne         __crack_main_+9Eh (0E6109Eh)  
  //    0x29,//:         evt_DescGroup[0] = rdataSegp->evt_MegaOpen;
  /*00E61099*/ 0x8B, 0x4E, 0x14,//             mov         ecx,dword ptr [esi+14h]  
  //    0x30,//:       else 
  /*00E6109C*/ 0xEB, 0x03,//                jmp         __crack_main_+0A1h (0E610A1h)  
  //    0x31,//:         evt_DescGroup[0] = rdataSegp->evt_MegaClose;
  /*00E6109E*/ 0x8B, 0x4E, 0x18,//             mov         ecx,dword ptr [esi+18h]  
  /*00E610A1*/ 0x89, 0x4D, 0xF4,//             mov         dword ptr [evt_DescGroup],ecx  
  //    0x32,//:       dataSeg->megaPower ^= 1;
  /*00E610A4*/ 0x33, 0xC9,//                xor         ecx,ecx  
  /*00E610A6*/ 0x41,//                   inc         ecx  
  /*00E610A7*/ 0x33, 0xC1,//                xor         eax,ecx  
  /*00E610A9*/ 0x89, 0x43, 0x5C,//             mov         dword ptr [ebx+5Ch],eax  
  /*00E610AC*/ 0xEB, 0x03,//                jmp         __crack_main_+0B1h (0E610B1h)  
  /*00E610AE*/ 0x33, 0xC9,//                xor         ecx,ecx  
  /*00E610B0*/ 0x41,//                   inc         ecx  
  //    0x33,//:     }
  //    0x34,//:     
  //    0x35,//:     if (curF1set[1] && ! dataSeg->F1array[1])
  /*00E610B1*/ 0x80, 0x7A, 0x01, 0x00,//          cmp         byte ptr [edx+1],0  
  /*00E610B5*/ 0x74, 0x17,//                je          __crack_main_+0CEh (0E610CEh)  
  /*00E610B7*/ 0x80, 0x7B, 0x19, 0x00,//          cmp         byte ptr [ebx+19h],0  
  /*00E610BB*/ 0x75, 0x11,//                jne         __crack_main_+0CEh (0E610CEh)  
  //    0x36,//:       if (dataSeg->holdWindow  == 0)
  /*00E610BD*/ 0x83, 0x7B, 0x60, 0x00,//          cmp         dword ptr [ebx+60h],0  
  /*00E610C1*/ 0x75, 0x0B,//                jne         __crack_main_+0CEh (0E610CEh)  
  //    0x37,//:         dataSeg->createStatusWindow (dataSeg, rdataSegp);
  /*00E610C3*/ 0x56,//                   push        esi  
  /*00E610C4*/ 0x53,//                   push        ebx  
  /*00E610C5*/ 0xFF, 0x53, 0x04,//             call        dword ptr [ebx+4]  
  /*00E610C8*/ 0x8B, 0x55, 0x0C,//             mov         edx,dword ptr [curF1set]  
  /*00E610CB*/ 0x33, 0xC9,//                xor         ecx,ecx  
  /*00E610CD*/ 0x41,//                   inc         ecx  
  //    0x38,//:       else ;
  //    0x39,//:     else ;
  //    0x40,//:   
  //    0x41,//:     // 设置n/k 
  //    0x42,//:     if (curF1set[2] && ! dataSeg->F1array[2]) {
  /*00E610CE*/ 0x80, 0x7A, 0x02, 0x00,//          cmp         byte ptr [edx+2],0  
  /*00E610D2*/ 0x74, 0x0C,//                je          __crack_main_+0E0h (0E610E0h)  
  /*00E610D4*/ 0x80, 0x7B, 0x1A, 0x00,//          cmp         byte ptr [ebx+1Ah],0  
  /*00E610D8*/ 0x75, 0x06,//                jne         __crack_main_+0E0h (0E610E0h)  
  //    0x43,//:       dataSeg->nSet ++; 
  /*00E610DA*/ 0xFF, 0x43, 0x24,//             inc         dword ptr [ebx+24h]  
  //    0x44,//:       bRecalcNK = TRUE;
  /*00E610DD*/ 0x89, 0x4D, 0xFC,//             mov         dword ptr [bRecalcNK],ecx  
  //    0x45,//:     }
  //    0x46,//:     if (curF1set[3] && ! dataSeg->F1array[3]) {
  /*00E610E0*/ 0x80, 0x7A, 0x03, 0x00,//          cmp         byte ptr [edx+3],0  
  /*00E610E4*/ 0x74, 0x0C,//                je          __crack_main_+0F2h (0E610F2h)  
  /*00E610E6*/ 0x80, 0x7B, 0x1B, 0x00,//          cmp         byte ptr [ebx+1Bh],0  
  /*00E610EA*/ 0x75, 0x06,//                jne         __crack_main_+0F2h (0E610F2h)  
  //    0x47,//:       -- dataSeg->nSet; 
  /*00E610EC*/ 0xFF, 0x4B, 0x24,//             dec         dword ptr [ebx+24h]  
  //    0x48,//:       bRecalcNK = TRUE;
  /*00E610EF*/ 0x89, 0x4D, 0xFC,//             mov         dword ptr [bRecalcNK],ecx  
  //    0x49,//:     }
  //    0x50,//:     if (curF1set[4] && ! dataSeg->F1array[5]) {
  /*00E610F2*/ 0x80, 0x7A, 0x04, 0x00,//          cmp         byte ptr [edx+4],0  
  /*00E610F6*/ 0x74, 0x0C,//                je          __crack_main_+104h (0E61104h)  
  /*00E610F8*/ 0x80, 0x7B, 0x1C, 0x00,//          cmp         byte ptr [ebx+1Ch],0  
  /*00E610FC*/ 0x75, 0x06,//                jne         __crack_main_+104h (0E61104h)  
  //    0x51,//:       ++ dataSeg->kSet; 
  /*00E610FE*/ 0xFF, 0x43, 0x20,//             inc         dword ptr [ebx+20h]  
  //    0x52,//:       bRecalcNK = TRUE;
  /*00E61101*/ 0x89, 0x4D, 0xFC,//             mov         dword ptr [bRecalcNK],ecx  
  //    0x53,//:     }
  //    0x54,//:     if (curF1set[5] && ! dataSeg->F1array[5]) {
  /*00E61104*/ 0x80, 0x7A, 0x05, 0x00,//          cmp         byte ptr [edx+5],0  
  /*00E61108*/ 0x74, 0x0C,//                je          __crack_main_+116h (0E61116h)  
  /*00E6110A*/ 0x80, 0x7B, 0x1D, 0x00,//          cmp         byte ptr [ebx+1Dh],0  
  /*00E6110E*/ 0x75, 0x06,//                jne         __crack_main_+116h (0E61116h)  
  //    0x55,//:       -- dataSeg->kSet; 
  /*00E61110*/ 0xFF, 0x4B, 0x20,//             dec         dword ptr [ebx+20h]  
  //    0x56,//:       bRecalcNK = TRUE;
  /*00E61113*/ 0x89, 0x4D, 0xFC,//             mov         dword ptr [bRecalcNK],ecx  
  //    0x57,//:     }
  //    0x58,//:     // 设置 reset
  //    0x59,//:     if (curF1set[6] && ! dataSeg->F1array[6]) {
  /*00E61116*/ 0x80, 0x7A, 0x06, 0x00,//          cmp         byte ptr [edx+6],0  
  /*00E6111A*/ 0x74, 0x1D,//                je          __crack_main_+139h (0E61139h)  
  /*00E6111C*/ 0x80, 0x7B, 0x1E, 0x00,//          cmp         byte ptr [ebx+1Eh],0  
  /*00E61120*/ 0x75, 0x17,//                jne         __crack_main_+139h (0E61139h)  
  //    0x60,//:       if (dataSeg->autoReset == 0) 
  /*00E61122*/ 0x8B, 0x43, 0x28,//             mov         eax,dword ptr [ebx+28h]  
  /*00E61125*/ 0x85, 0xC0,//                test        eax,eax  
  /*00E61127*/ 0x75, 0x05,//                jne         __crack_main_+12Eh (0E6112Eh)  
  //    0x61,//:         evt_DescGroup[1] = rdataSegp->evt_AutoReset_Open;
  /*00E61129*/ 0x8B, 0x56, 0x1C,//             mov         edx,dword ptr [esi+1Ch]  
  //    0x62,//:       else 
  /*00E6112C*/ 0xEB, 0x03,//                jmp         __crack_main_+131h (0E61131h)  
  //    0x63,//:         evt_DescGroup[1] = rdataSegp->evt_AutoReset_Close;
  /*00E6112E*/ 0x8B, 0x56, 0x20,//             mov         edx,dword ptr [esi+20h]  
  //    0x64,//:       dataSeg->autoReset ^= 1;
  /*00E61131*/ 0x33, 0xC1,//                xor         eax,ecx  
  /*00E61133*/ 0x89, 0x55, 0xF8,//             mov         dword ptr [ebp-8],edx  
  /*00E61136*/ 0x89, 0x43, 0x28,//             mov         dword ptr [ebx+28h],eax  
  //    0x65,//:     }
  //    0x66,//:     for (id = 0; id != 8; id++)
  /*00E61139*/ 0x8D, 0x7B, 0x18,//             lea         edi,[ebx+18h]  
  //    0x67,//:       dataSeg->F1array[id] = F1_cache[id];
  /*00E6113C*/ 0x8D, 0x75, 0xEC,//             lea         esi,[F1_cache]  
  /*00E6113F*/ 0xA5,//                   movs        dword ptr es:[edi],dword ptr [esi]  
  /*00E61140*/ 0xA5,//                   movs        dword ptr es:[edi],dword ptr [esi]  
  //    0x68,//:   
  //    0x69,//:     if (dataSeg->kSet <= 0) {
  //    0x70,//:       dataSeg->kSet = 1;
  /*00E61141*/ 0x33, 0xFF,//                xor         edi,edi  
  /*00E61143*/ 0x33, 0xC0,//                xor         eax,eax  
  /*00E61145*/ 0x47,//                   inc         edi  
  /*00E61146*/ 0xC7, 0x45, 0x0C, 0x08, 0x00, 0x00, 0x00,// mov         dword ptr [curF1set],8  
  /*00E6114D*/ 0x39, 0x43, 0x20,//             cmp         dword ptr [ebx+20h],eax  
  /*00E61150*/ 0x7F, 0x06,//                jg          __crack_main_+158h (0E61158h)  
  /*00E61152*/ 0x89, 0x7B, 0x20,//             mov         dword ptr [ebx+20h],edi  
  //    0x71,//:       bRecalcNK = TRUE;
  /*00E61155*/ 0x89, 0x7D, 0xFC,//             mov         dword ptr [bRecalcNK],edi  
  //    0x72,//:     }
  //    0x73,//:     if (dataSeg->nSet <= 0) {
  /*00E61158*/ 0x39, 0x43, 0x24,//             cmp         dword ptr [ebx+24h],eax  
  /*00E6115B*/ 0x7F, 0x06,//                jg          __crack_main_+163h (0E61163h)  
  //    0x74,//:       dataSeg->nSet = 1;
  /*00E6115D*/ 0x89, 0x7B, 0x24,//             mov         dword ptr [ebx+24h],edi  
  //    0x75,//:       bRecalcNK = TRUE;
  /*00E61160*/ 0x89, 0x7D, 0xFC,//             mov         dword ptr [bRecalcNK],edi  
  //    0x76,//:     }
  //    0x77,//: 
  //    0x78,//:     if ( bRecalcNK) {
  /*00E61163*/ 0x39, 0x45, 0xFC,//             cmp         dword ptr [bRecalcNK],eax  
  /*00E61166*/ 0x74, 0x62,//                je          __crack_main_+1CAh (0E611CAh)  
  //    0x79,//:       int nk = dataSeg->nSet / dataSeg->kSet; // 8 / 3 
  /*00E61168*/ 0x8B, 0x4B, 0x24,//             mov         ecx,dword ptr [ebx+24h]  
  /*00E6116B*/ 0x8B, 0x73, 0x20,//             mov         esi,dword ptr [ebx+20h]  
  /*00E6116E*/ 0x8B, 0xC1,//                mov         eax,ecx  
  /*00E61170*/ 0x99,//                   cdq  
  /*00E61171*/ 0xF7, 0xFE,//                idiv        eax,esi  
  //    0x80,//:       int nk_mod = dataSeg->nSet % dataSeg->kSet;
  //    0x81,//:       dataSeg->uCount = -1;
  /*00E61173*/ 0x83, 0x4B, 0x38, 0xFF,//          or          dword ptr [ebx+38h],0FFFFFFFFh  
  //    0x82,//:       if (nk >= 2) {
  /*00E61177*/ 0x6A, 0x02,//                push        2  
  /*00E61179*/ 0x89, 0x55, 0x10,//             mov         dword ptr [dataSeg],edx  
  /*00E6117C*/ 0x5A,//                   pop         edx  
  /*00E6117D*/ 0x3B, 0xC2,//                cmp         eax,edx  
  /*00E6117F*/ 0x7C, 0x21,//                jl          __crack_main_+1A2h (0E611A2h)  
  //    0x83,//:         dataSeg->scanOneRingTicks = dataSeg->nSet;
  //    0x84,//:         dataSeg->scanNextFrameSwitchVPos = dataSeg->kSet * nk - 1;
  /*00E61181*/ 0x8B, 0xD6,//                mov         edx,esi  
  /*00E61183*/ 0x0F, 0xAF, 0xD0,//             imul        edx,eax  
  /*00E61186*/ 0x4A,//                   dec         edx  
  //    0x85,//:         dataSeg->scanPerWaveWidth = nk_mod ? nk + 1 : nk;
  /*00E61187*/ 0x83, 0x7D, 0x10, 0x00,//          cmp         dword ptr [dataSeg],0  
  /*00E6118B*/ 0x89, 0x4B, 0x40,//             mov         dword ptr [ebx+40h],ecx  
  /*00E6118E*/ 0x89, 0x53, 0x44,//             mov         dword ptr [ebx+44h],edx  
  /*00E61191*/ 0x74, 0x01,//                je          __crack_main_+194h (0E61194h)  
  /*00E61193*/ 0x40,//                   inc         eax  
  /*00E61194*/ 0x89, 0x43, 0x48,//             mov         dword ptr [ebx+48h],eax  
  //    0x86,//:         dataSeg->vCount = dataSeg->scanPerWaveWidth;
  /*00E61197*/ 0x89, 0x43, 0x3C,//             mov         dword ptr [ebx+3Ch],eax  
  //    0x87,//:         dataSeg->kReal = dataSeg->kSet;
  /*00E6119A*/ 0x89, 0x73, 0x2C,//             mov         dword ptr [ebx+2Ch],esi  
  //    0x88,//:         dataSeg->nReal = dataSeg->nSet;
  /*00E6119D*/ 0x89, 0x4B, 0x30,//             mov         dword ptr [ebx+30h],ecx  
  //    0x89,//:       } else {
  /*00E611A0*/ 0xEB, 0x16,//                jmp         __crack_main_+1B8h (0E611B8h)  
  //    0x90,//:         // 饱和 
  //    0x91,//:         dataSeg->scanOneRingTicks = 2;
  /*00E611A2*/ 0x89, 0x53, 0x40,//             mov         dword ptr [ebx+40h],edx  
  //    0x92,//:         dataSeg->scanNextFrameSwitchVPos = 8;
  /*00E611A5*/ 0xC7, 0x43, 0x44, 0x08, 0x00, 0x00, 0x00,// mov         dword ptr [ebx+44h],8  
  //    0x93,//:         dataSeg->scanPerWaveWidth = 2;
  /*00E611AC*/ 0x89, 0x53, 0x48,//             mov         dword ptr [ebx+48h],edx  
  //    0x94,//:         dataSeg->vCount = 2;
  /*00E611AF*/ 0x89, 0x53, 0x3C,//             mov         dword ptr [ebx+3Ch],edx  
  //    0x95,//:         dataSeg->kReal = 1;
  /*00E611B2*/ 0x89, 0x7B, 0x2C,//             mov         dword ptr [ebx+2Ch],edi  
  //    0x96,//:         dataSeg->nReal = 2;
  /*00E611B5*/ 0x89, 0x53, 0x30,//             mov         dword ptr [ebx+30h],edx  
  //    0x97,//:       } 
  //    0x98,//:       if (dataSeg->autoReset) {
  /*00E611B8*/ 0x83, 0x7B, 0x28, 0x00,//          cmp         dword ptr [ebx+28h],0  
  /*00E611BC*/ 0x74, 0x0C,//                je          __crack_main_+1CAh (0E611CAh)  
  //    0x99,//:         dataSeg->ticksTotal = 0;
  /*00E611BE*/ 0x83, 0x63, 0x34, 0x00,//          and         dword ptr [ebx+34h],0  
  //   100:         dataSeg->scanPassTotal = 0;
  /*00E611C2*/ 0x83, 0x63, 0x50, 0x00,//          and         dword ptr [ebx+50h],0  
  //   101:         dataSeg->scanPassNoroundTicks = -1;
  /*00E611C6*/ 0x83, 0x4B, 0x4C, 0xFF,//          or          dword ptr [ebx+4Ch],0FFFFFFFFh  
  //   102:       }
  //   103:     }
  //   104:     // molisi 炮测试
  //   105:     if (  dataSeg->megaPower )  {
  /*00E611CA*/ 0x83, 0x7B, 0x5C, 0x00,//          cmp         dword ptr [ebx+5Ch],0  
  /*00E611CE*/ 0x74, 0x5F,//                je          __crack_main_+22Fh (0E6122Fh)  
  //   106:       // 分派至环 
  //   107:       int mn = dataSeg->ticksTotal % dataSeg->scanOneRingTicks;
  /*00E611D0*/ 0x8B, 0x4B, 0x34,//             mov         ecx,dword ptr [ebx+34h]  
  /*00E611D3*/ 0x8B, 0x73, 0x40,//             mov         esi,dword ptr [ebx+40h]  
  /*00E611D6*/ 0x8B, 0xC1,//                mov         eax,ecx  
  /*00E611D8*/ 0x99,//                   cdq  
  /*00E611D9*/ 0xF7, 0xFE,//                idiv        eax,esi  
  //   108:       ++ dataSeg->uCount;
  /*00E611DB*/ 0xFF, 0x43, 0x38,//             inc         dword ptr [ebx+38h]  
  /*00E611DE*/ 0x8B, 0x43, 0x38,//             mov         eax,dword ptr [ebx+38h]  
  //   109:       ++ dataSeg->ticksTotal; // 0: 
  /*00E611E1*/ 0x41,//                   inc         ecx  
  //   110:       id = ++ dataSeg->scanPassNoroundTicks;
  /*00E611E2*/ 0xFF, 0x43, 0x4C,//             inc         dword ptr [ebx+4Ch]  
  /*00E611E5*/ 0x89, 0x4B, 0x34,//             mov         dword ptr [ebx+34h],ecx  
  /*00E611E8*/ 0x8B, 0x4B, 0x4C,//             mov         ecx,dword ptr [ebx+4Ch]  
  //   111:       if (mn == (dataSeg->scanOneRingTicks - 1)) {
  /*00E611EB*/ 0x4E,//                   dec         esi  
  /*00E611EC*/ 0x89, 0x4D, 0x0C,//             mov         dword ptr [curF1set],ecx  
  /*00E611EF*/ 0x3B, 0xD6,//                cmp         edx,esi  
  /*00E611F1*/ 0x75, 0x19,//                jne         __crack_main_+20Ch (0E6120Ch)  
  //   112:         // 终点 
  //   113:         dataSeg->uCount = -1;
  //   114:         dataSeg->vCount = dataSeg->scanPerWaveWidth;
  /*00E611F3*/ 0x8B, 0x43, 0x48,//             mov         eax,dword ptr [ebx+48h]  
  /*00E611F6*/ 0x83, 0x4B, 0x38, 0xFF,//          or          dword ptr [ebx+38h],0FFFFFFFFh  
  /*00E611FA*/ 0x89, 0x43, 0x3C,//             mov         dword ptr [ebx+3Ch],eax  
  //   115:         DIK_shift[0] |= 0x80;
  /*00E611FD*/ 0x8B, 0x45, 0x08,//             mov         eax,dword ptr [DIK_shift]  
  /*00E61200*/ 0x80, 0x08, 0x80,//             or          byte ptr [eax],80h  
  //   116:         ++  dataSeg->scanPassTotal;
  /*00E61203*/ 0xFF, 0x43, 0x50,//             inc         dword ptr [ebx+50h]  
  //   117:         dataSeg->scanPassNoroundTicks = 0;
  /*00E61206*/ 0x83, 0x63, 0x4C, 0x00,//          and         dword ptr [ebx+4Ch],0  
  //   118:       } else {
  /*00E6120A*/ 0xEB, 0x23,//                jmp         __crack_main_+22Fh (0E6122Fh)  
  //   119:         if (mn == dataSeg->scanNextFrameSwitchVPos) {
  /*00E6120C*/ 0x3B, 0x53, 0x44,//             cmp         edx,dword ptr [ebx+44h]  
  /*00E6120F*/ 0x75, 0x0D,//                jne         __crack_main_+21Eh (0E6121Eh)  
  //   120:           // 分界轴点
  //   121:           dataSeg->uCount = -1;
  //   122:           dataSeg->vCount = dataSeg->scanPerWaveWidth - 1;
  /*00E61211*/ 0x8B, 0x43, 0x48,//             mov         eax,dword ptr [ebx+48h]  
  /*00E61214*/ 0x83, 0x4B, 0x38, 0xFF,//          or          dword ptr [ebx+38h],0FFFFFFFFh  
  /*00E61218*/ 0x48,//                   dec         eax  
  /*00E61219*/ 0x89, 0x43, 0x3C,//             mov         dword ptr [ebx+3Ch],eax  
  //   123:           DIK_shift[0] |= 0x80;
  /*00E6121C*/ 0xEB, 0x0B,//                jmp         __crack_main_+229h (0E61229h)  
  //   124:         } else if (dataSeg->uCount % dataSeg->vCount == (dataSeg->vCount - 1)) {
  /*00E6121E*/ 0x8B, 0x4B, 0x3C,//             mov         ecx,dword ptr [ebx+3Ch]  
  /*00E61221*/ 0x99,//                   cdq  
  /*00E61222*/ 0xF7, 0xF9,//                idiv        eax,ecx  
  /*00E61224*/ 0x49,//                   dec         ecx  
  /*00E61225*/ 0x3B, 0xD1,//                cmp         edx,ecx  
  /*00E61227*/ 0x75, 0x06,//                jne         __crack_main_+22Fh (0E6122Fh)  
  //   125:           // 切换触发 
  //   126:           DIK_shift[0] |= 0x80;
  /*00E61229*/ 0x8B, 0x45, 0x08,//             mov         eax,dword ptr [DIK_shift]  
  /*00E6122C*/ 0x80, 0x08, 0x80,//             or          byte ptr [eax],80h  
  //   127:         }
  //   128:       }
  //   129:     }
  //   130: 
  //   131:     if (dataSeg->holdWindow ) {
  /*00E6122F*/ 0x33, 0xFF,//                xor         edi,edi  
  /*00E61231*/ 0x39, 0x7B, 0x60,//             cmp         dword ptr [ebx+60h],edi  
  /*00E61234*/ 0x74, 0x6F,//                je          __crack_main_+2A5h (0E612A5h)  
  //   132:       CHAR sppc[200];
  //   133:       dataSeg->__sprintf (sppc, rdataSegp->statusOutput, 
  //   134:                        dataSeg->nSet, 
  //   135:                        dataSeg->kSet,
  //   136:                        dataSeg->nReal, 
  //   137:                        dataSeg->kReal, dataSeg->ticksTotal, dataSeg->scanPassTotal, id);
  /*00E61236*/ 0xFF, 0x75, 0x0C,//             push        dword ptr [curF1set]  
  /*00E61239*/ 0x8B, 0x45, 0x14,//             mov         eax,dword ptr [rdataSegp]  
  /*00E6123C*/ 0xFF, 0x73, 0x50,//             push        dword ptr [ebx+50h]  
  /*00E6123F*/ 0xFF, 0x73, 0x34,//             push        dword ptr [ebx+34h]  
  /*00E61242*/ 0xFF, 0x73, 0x2C,//             push        dword ptr [ebx+2Ch]  
  /*00E61245*/ 0xFF, 0x73, 0x30,//             push        dword ptr [ebx+30h]  
  /*00E61248*/ 0xFF, 0x73, 0x20,//             push        dword ptr [ebx+20h]  
  /*00E6124B*/ 0xFF, 0x73, 0x24,//             push        dword ptr [ebx+24h]  
  /*00E6124E*/ 0xFF, 0x70, 0x24,//             push        dword ptr [eax+24h]  
  /*00E61251*/ 0x8D, 0x85, 0x24, 0xFF, 0xFF, 0xFF,//    lea         eax,[sppc]  
  /*00E61257*/ 0x50,//                   push        eax  
  /*00E61258*/ 0xFF, 0x53, 0x0C,//             call        dword ptr [ebx+0Ch]  
  /*00E6125B*/ 0x83, 0xC4, 0x24,//             add         esp,24h  
  //   138:       dataSeg->setWindowTextA (dataSeg->wndStatus, sppc);
  /*00E6125E*/ 0x8D, 0x85, 0x24, 0xFF, 0xFF, 0xFF,//    lea         eax,[sppc]  
  /*00E61264*/ 0x50,//                   push        eax  
  /*00E61265*/ 0xFF, 0x73, 0x54,//             push        dword ptr [ebx+54h]  
  /*00E61268*/ 0xFF, 0x53, 0x14,//             call        dword ptr [ebx+14h]  
  //   141:         if ( evt_DescGroup [id] != NULL) {
  /*00E6126B*/ 0x8B, 0x74, 0xBD, 0xF4,//          mov         esi,dword ptr evt_DescGroup[edi*4]  
  /*00E6126F*/ 0x85, 0xF6,//                test        esi,esi  
  /*00E61271*/ 0x74, 0x2C,//                je          __crack_main_+29Fh (0E6129Fh)  
  //   142:           dataSeg->sendMessageA (dataSeg->wndEvent, EM_SETSEL, -2, -1);
  /*00E61273*/ 0x6A, 0xFF,//                push        0FFFFFFFFh  
  /*00E61275*/ 0x6A, 0xFE,//                push        0FFFFFFFEh  
  /*00E61277*/ 0x68, 0xB1, 0x00, 0x00, 0x00,//       push        0B1h  
  /*00E6127C*/ 0xFF, 0x73, 0x58,//             push        dword ptr [ebx+58h]  
  /*00E6127F*/ 0xFF, 0x53, 0x10,//             call        dword ptr [ebx+10h]  
  //   143:           dataSeg->sendMessageA (dataSeg->wndEvent, EM_REPLACESEL, TRUE, (LPARAM) evt_DescGroup[id]);
  /*00E61282*/ 0x56,//                   push        esi  
  /*00E61283*/ 0x6A, 0x01,//                push        1  
  /*00E61285*/ 0x68, 0xC2, 0x00, 0x00, 0x00,//       push        0C2h  
  /*00E6128A*/ 0xFF, 0x73, 0x58,//             push        dword ptr [ebx+58h]  
  /*00E6128D*/ 0xFF, 0x53, 0x10,//             call        dword ptr [ebx+10h]  
  //   144:           dataSeg->sendMessageA (dataSeg->wndEvent, WM_VSCROLL, SB_BOTTOM, 0);
  /*00E61290*/ 0x6A, 0x00,//                push        0  
  /*00E61292*/ 0x6A, 0x07,//                push        7  
  /*00E61294*/ 0x68, 0x15, 0x01, 0x00, 0x00,//       push        115h  
  /*00E61299*/ 0xFF, 0x73, 0x58,//             push        dword ptr [ebx+58h]  
  /*00E6129C*/ 0xFF, 0x53, 0x10,//             call        dword ptr [ebx+10h]  
  //   139:       // 扔条目进去 ....
  //   140:       for (id = 0; id != 2; id++) {
  /*00E6129F*/ 0x47,//                   inc         edi  
  /*00E612A0*/ 0x83, 0xFF, 0x02,//             cmp         edi,2  
  /*00E612A3*/ 0x75, 0xC6,//                jne         __crack_main_+26Bh (0E6126Bh)  
  /*00E612A5*/ 0x5F,//                   pop         edi  
  /*00E612A6*/ 0x5E,//                   pop         esi  
  /*00E612A7*/ 0x5B,//                   pop         ebx  
  //   145:         }
  //   146:       }
  //   147:     }
  //   148: }
  /*00E612A8*/ 0xC9,//                   leave  
  /*00E612A9*/ 0xC2, 0x10, 0x00//             ret         10h   
};

void __calc_write (HANDLE fHandle) {
  DWORD temps__;
  SetFilePointer (fHandle, MOLIOS_PEPROGRAM_ADDRESS, NULL, FILE_BEGIN);
  WriteFile (fHandle, calc_s0, sizeof (calc_s0), & temps__, NULL);
}