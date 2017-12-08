#if !defined (_MOLIOS_H_)
# define _MOLIOS_H_

#include <dinput.h>
#include <tchar.h>

// 0x64 
struct __CrackDataSegment {
# define AS_Global_Init 8 
# define AS_Hold_Window 0x60
  WNDPROC  wndCallback;// XXX:预设置 ! 控件回调
  void (WINAPI * createStatusWindow) (struct __CrackDataSegment *dataSeg, struct __CrackRdataSegmentPointer *rdataSegp); // XXX:预设置 ! 窗口创建函数
  int Global_Init;
  void (CDECL * __sprintf)(char *, const char *, ...);
  void (WINAPI * sendMessageA)(HWND, UINT, WPARAM, LPARAM);
  void (WINAPI * setWindowTextA)(HWND, char *);
  BYTE F1array[8];
  int kSet;
  int nSet;
  int autoReset;
  int kReal;
  int nReal;
  int ticksTotal;
  int uCount;
  int vCount;
  int scanOneRingTicks; // 7 
  int scanNextFrameSwitchVPos; // 5
  int scanPerWaveWidth; // 3 or 2 
  int scanPassNoroundTicks;
  int scanPassTotal; 
  HWND wndStatus;
  HWND wndEvent;
  int megaPower; // 莫里斯炮
  int holdWindow; // 
};
// buff 0x127 
struct __CrackRdataSegmentPointer {
  // IAT 符号描述 
  char *DLL_ntdll; // ntdll.dll;
  char *DLL_user32; // user32.dll
  char *CRTAPI_sprintf;
  char *WINAPI_SetWindowTextA;
  char *WINAPI_SendMessageA;
  char *evt_MegaOpen;
  char *evt_MegaClose;
  char *evt_AutoReset_Open;
  char *evt_AutoReset_Close;
  char *statusOutput;
  char *emptyString;
  char *classNameString;
  char *buttonString;
  char *captionString;
  char *editClassString;
  char *statusString;
  char *eventString;
};

// IAT 符号 
# define IAT_GetModuleHandleA 0x4B40E0 
# define IAT_GetProcAddress 0x4B40D8
# define IAT_DefWindowProcA 0x4B41F8 
# define IAT_LoadCursorA 0x4B41FC
# define IAT_GetStockObject 0x4B4044 
# define IAT_CreateWindowExA 0x4B4204
# define IAT_RegisterClassA 0x4B4200 
# define IAT_ShowWindow 0x4B41C8
# define IAT_VirtualProtect 0x4B4114

// code calc size 0x300 
// code window size 0x200 
// code window_callback 0x100 
// var dataSeg 0x100 
// var rdataSegp 0x50
// rdata chbuffer 0x170 
# define MOLIOS_PROGRAM_ADDRESS 0x4BB250
# define MOLIOS_PEPROGRAM_ADDRESS 0xBAC50 
# define MOLIOS_PROGRAM_SIZE 0x600
# define MOLIOS_PRGOFFSET_CALC 0 
# define MOLIOS_PRGOFFSET_WINDOW 0x300
# define MOLIOS_PRGOFFSET_CALLBACK 0x500

# define MOLIOS_PGRAM_ADDRESS (MOLIOS_PROGRAM_ADDRESS +MOLIOS_PROGRAM_SIZE)  /* with rdata pointer **/
# define MOLIOS_PEPGRAM_ADDRESS (MOLIOS_PEPROGRAM_ADDRESS +MOLIOS_PROGRAM_SIZE) 
# define MOLIOS_PGRAM_SIZE  0x150
# define MOLIOS_PGRAMOFFSET_DATASEG 0
# define MOLIOS_PGRAMOFFSET_RDATAP 0x100

# define MOLIOS_RDATA_ADDRESS (MOLIOS_PGRAM_ADDRESS + MOLIOS_PGRAM_SIZE)
# define MOLIOS_PERDATA_ADDRESS (MOLIOS_PEPGRAM_ADDRESS + MOLIOS_PGRAM_SIZE)
# define MOLIOS_RDATA_SIZE 0x200 
# define MOLIOS_RDATAOFFSET_CHBUFFER 0 
# define MOLIOS_RDATA_CHBUFFER_SIZE 0x200 

// 跳板进入地址
# define MOLIOS_BOARD_ENTRY 0x43dd73 
# define MOLIOS_PEBOARD_ENTRY 0x3DB73
# define MOLIOS_BOARD_SELF 0x4B3B78
# define MOLIOS_PEBOARD_SELF 0xB3978
# define MOLIOS_TEMP_GLOBALUSE 0x4C68DC // 临时使用的地址 for IAT_VirtualProtect
# define MOLIOS_RET 0x43DD9E

#endif 