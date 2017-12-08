#include "molios.h"

void __stdcall __crack_main_ (BYTE *DIK_shift, BYTE *curF1set, 
                     struct __CrackDataSegment *dataSeg,
                     struct __CrackRdataSegmentPointer   *rdataSegp)
{
    const char *evt_DescGroup[2] = { NULL, NULL };
    BYTE F1_cache[8]; 
    BOOL bRecalcNK = FALSE;
    INT id;
    
    if (dataSeg->Global_Init == 0) 
    {
      // 模拟IAT调用 
      HMODULE ntdll = (* (HMODULE (WINAPI **)(LPCSTR)) (IAT_GetModuleHandleA)) (rdataSegp->DLL_ntdll);
      HMODULE user32 = (* (HMODULE (WINAPI **)(LPCSTR)) (IAT_GetModuleHandleA)) (rdataSegp->DLL_user32);
      dataSeg->__sprintf = (void *) (* (HMODULE (WINAPI **)(HMODULE, LPCSTR)) (IAT_GetProcAddress)) (ntdll, rdataSegp->CRTAPI_sprintf);
      dataSeg->setWindowTextA = (void *) (* (HMODULE (WINAPI **)(HMODULE, LPCSTR)) (IAT_GetProcAddress)) (user32, rdataSegp->WINAPI_SetWindowTextA);
      dataSeg->sendMessageA = (void *) (* (HMODULE (WINAPI **)(HMODULE, LPCSTR)) (IAT_GetProcAddress)) (user32, rdataSegp->WINAPI_SendMessageA);
      dataSeg->Global_Init = 1;
    }

    // 清除杂质,放入按键缓存
    for (id = 0; id != 8; id++)
      F1_cache[id] = curF1set[id] & 0x80;

    if (curF1set[0] && ! dataSeg->F1array[0]) {
      if (dataSeg->megaPower == 0) 
        evt_DescGroup[0] = rdataSegp->evt_MegaOpen;
      else 
        evt_DescGroup[0] = rdataSegp->evt_MegaClose;
      dataSeg->megaPower ^= 1;
    }
    
    if (curF1set[1] && ! dataSeg->F1array[1])
      if (dataSeg->holdWindow  == 0)
        dataSeg->createStatusWindow (dataSeg, rdataSegp);
      else ;
    else ;
  
    // 设置n/k 
    if (curF1set[2] && ! dataSeg->F1array[2]) {
      dataSeg->nSet ++; 
      bRecalcNK = TRUE;
    }
    if (curF1set[3] && ! dataSeg->F1array[3]) {
      -- dataSeg->nSet; 
      bRecalcNK = TRUE;
    }
    if (curF1set[4] && ! dataSeg->F1array[4]) {
      ++ dataSeg->kSet; 
      bRecalcNK = TRUE;
    }
    if (curF1set[5] && ! dataSeg->F1array[5]) {
      -- dataSeg->kSet; 
      bRecalcNK = TRUE;
    }
    // 设置 reset
    if (curF1set[6] && ! dataSeg->F1array[6]) {
      if (dataSeg->autoReset == 0) 
        evt_DescGroup[1] = rdataSegp->evt_AutoReset_Open;
      else 
        evt_DescGroup[1] = rdataSegp->evt_AutoReset_Close;
      dataSeg->autoReset ^= 1;
    }
    for (id = 0; id != 8; id++)
      dataSeg->F1array[id] = F1_cache[id];
  
    if (dataSeg->kSet <= 0) {
      dataSeg->kSet = 1;
      bRecalcNK = TRUE;
    }
    if (dataSeg->nSet <= 0) {
      dataSeg->nSet = 1;
      bRecalcNK = TRUE;
    }

    if ( bRecalcNK) {
      int nk = dataSeg->nSet / dataSeg->kSet; // 8 / 3 
      int nk_mod = dataSeg->nSet % dataSeg->kSet;
      dataSeg->uCount = -1;
      if (nk >= 2) {
        dataSeg->scanOneRingTicks = dataSeg->nSet;
        dataSeg->scanNextFrameSwitchVPos = dataSeg->kSet * nk - 1;
        dataSeg->scanPerWaveWidth = nk_mod ? nk + 1 : nk;
        dataSeg->vCount = dataSeg->scanPerWaveWidth;
        dataSeg->kReal = dataSeg->kSet;
        dataSeg->nReal = dataSeg->nSet;
      } else {
        // 饱和 
        dataSeg->scanOneRingTicks = 2;
        dataSeg->scanNextFrameSwitchVPos = 8;
        dataSeg->scanPerWaveWidth = 2;
        dataSeg->vCount = 2;
        dataSeg->kReal = 1;
        dataSeg->nReal = 2;
      } 
      if (dataSeg->autoReset) {
        dataSeg->ticksTotal = 0;
        dataSeg->scanPassTotal = 0;
        dataSeg->scanPassNoroundTicks = -1;
      }
    }
    // molisi 炮测试
    if (  dataSeg->megaPower )  {
      // 分派至环 
      int mn = dataSeg->ticksTotal % dataSeg->scanOneRingTicks;
      ++ dataSeg->uCount;
      ++ dataSeg->ticksTotal; // 0: 
      id = ++ dataSeg->scanPassNoroundTicks;
      if (mn == (dataSeg->scanOneRingTicks - 1)) {
        // 终点 
        dataSeg->uCount = -1;
        dataSeg->vCount = dataSeg->scanPerWaveWidth;
        DIK_shift[0] |= 0x80;
        ++  dataSeg->scanPassTotal;
        dataSeg->scanPassNoroundTicks = 0;
      } else {
        if (mn == dataSeg->scanNextFrameSwitchVPos) {
          // 分界轴点
          dataSeg->uCount = -1;
          dataSeg->vCount = dataSeg->scanPerWaveWidth - 1;
          DIK_shift[0] |= 0x80;
        } else if (dataSeg->uCount % dataSeg->vCount == (dataSeg->vCount - 1)) {
          // 切换触发 
          DIK_shift[0] |= 0x80;
        }
      }
    }

    if (dataSeg->holdWindow ) {
      CHAR sppc[200];
      dataSeg->__sprintf (sppc, rdataSegp->statusOutput, 
                       dataSeg->nSet, 
                       dataSeg->kSet,
                       dataSeg->nReal, 
                       dataSeg->kReal, dataSeg->ticksTotal, dataSeg->scanPassTotal, id);
      dataSeg->setWindowTextA (dataSeg->wndStatus, sppc);
      // 扔条目进去 ....
      for (id = 0; id != 2; id++) {
        if ( evt_DescGroup [id] != NULL) {
          dataSeg->sendMessageA (dataSeg->wndEvent, EM_SETSEL, -2, -1);
          dataSeg->sendMessageA (dataSeg->wndEvent, EM_REPLACESEL, TRUE, (LPARAM) evt_DescGroup[id]);
          dataSeg->sendMessageA (dataSeg->wndEvent, WM_VSCROLL, SB_BOTTOM, 0);
        }
      }
    }
}

#if 0
// 语法上的程序入口点
int WINAPI _tWinMain ( HINSTANCE instance_cur, // 当前实例
  HINSTANCE instance_prev,// 之前实例
  LPTSTR cmd_line, // 命令行
  int cmd_show // 如何显示
  )

{
  __asm call __crack_main_
}

# if defined (_DEBUG)
int main (void)
{
  _tWinMain (GetModuleHandle (NULL), NULL, NULL, SW_SHOWNORMAL);
  return 0;
}
# endif 

# endif 