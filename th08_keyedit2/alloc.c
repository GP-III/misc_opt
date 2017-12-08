#include "molios.h"

char * __pp[]  = {
  // IAT 符号描述 
  "ntdll.dll", //char *DLL_ntdll; // ntdll.dll;
  "user32.dll", // char *DLL_user32; // user32.dll
  "sprintf", // char *CRTAPI_sprintf;
  "SetWindowTextA", // char *WINAPI_SetWindowTextA;
  "SendMessageA", // char *WINAPI_SendMessageA;
  "event ~ mega power open\r\n", // char *evt_MegaOpen;
  "event ~ mega power close\r\n", // char *evt_MegaClose;
  "event ~ autoreset open\r\n", // char *evt_AutoReset_Open;
  "event ~ autoreset close\r\n", // char *evt_AutoReset_Close;
  "user set n/k:%d|%d\r\n"
    "real set n/k:%d|%d\r\n"
    "ticks_total:%d\r\n"
    "pass_total:%d\r\n"
    "pass_noround_ticks:%d", // char *statusOutput;
  "", // char *emptyString;
  "moecmks_posix", // 类名随便填啦, char *classNameString;
  "button", // char *buttonString;
  "th08 monitor", // char *captionString;
  "edit", // char *editClassString;
  "status", // char *statusString;
  "event" //  char *eventString;
};

void b_alloc (HANDLE fHandle) {

  // 分配字符串资源, 并设置 const char *指针 
  char *acsm = malloc (0x1000); 
  int id;
  int lens = 0; // 长度计数 
  struct __CrackRdataSegmentPointer css;
  struct __CrackRdataSegmentPointer *cssp = & css;
  DWORD temp___;
  for (id = 0; id != sizeof (__pp)/ sizeof (__pp[0]); id++) {
    // 起始地址 MOLIOS_RDATA_ADDRESS | 
    // 字串指针 MOLIOS_RDATA_SEGP_ENTRY | MOLIOS_PEPGRAM_ADDRESS
    int idss = strlen (__pp[id]) + 1;
    memcpy (& acsm[lens], __pp[id], idss);
    ((char **)cssp)[id] = ( void *) (MOLIOS_RDATA_ADDRESS + lens);
     lens += idss; // MOLIOS_RDATA_ADDRESS
  }
  // 写入RDATA指针域
  SetFilePointer(fHandle, MOLIOS_PEPGRAM_ADDRESS+MOLIOS_PGRAMOFFSET_RDATAP, NULL, FILE_BEGIN );
  WriteFile (fHandle, cssp, sizeof (*cssp), & temp___, NULL);
  // 写入RDATA缓冲区 
  SetFilePointer(fHandle, MOLIOS_PERDATA_ADDRESS, NULL, FILE_BEGIN );
  WriteFile (fHandle, acsm, lens, & temp___, NULL);
}