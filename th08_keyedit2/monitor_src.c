#include "molios.h"


__declspec(naked)
__cdecl WndProc (HWND window, UINT msg, WPARAM param, LPARAM param2) {
  __asm {
    mov eax, WM_DESTROY
    cmp eax, dword ptr[esp+8] 
    jne dosub 
    xor eax, eax
    lea eax, [eax+MOLIOS_PGRAM_ADDRESS+AS_Hold_Window]
    mov dword ptr [eax], 0 
dosub:
    mov eax, IAT_DefWindowProcA
    mov eax, DWORD ptr[eax]
    jmp eax
  }
}

void __stdcall __window_main_ (struct __CrackDataSegment *dataSeg,
                     struct __CrackRdataSegmentPointer   *rdataSegp)
{
  HWND hwnd;
  WNDCLASSA wndclass ;
  HINSTANCE inst = (HINSTANCE) (* (HMODULE (WINAPI **)(LPCSTR)) (IAT_GetModuleHandleA)) (NULL);

  ZeroMemory (& wndclass, sizeof (wndclass));
  wndclass.lpfnWndProc = dataSeg->wndCallback ;
  wndclass.hInstance = inst ;
  wndclass.hCursor = (* (HCURSOR (WINAPI **)(HINSTANCE, LPCSTR)) (IAT_LoadCursorA)) (NULL, MAKEINTRESOURCEA(32512));
  wndclass.hbrBackground = (HBRUSH) (* (HGDIOBJ (WINAPI **)(int)) (IAT_GetStockObject)) (DKGRAY_BRUSH);
  wndclass.lpszClassName = rdataSegp->classNameString ;
  (* (ATOM (WINAPI **)(WNDCLASSA *)) (IAT_RegisterClassA)) (& wndclass);

  // ------------------------------------------------------------------------------------
  hwnd = (* (HWND (WINAPI **)(__in DWORD dwExStyle,
                      __in_opt LPCSTR lpClassName,
                      __in_opt LPCSTR lpWindowName,
                      __in DWORD dwStyle,
                      __in int X,
                      __in int Y,
                      __in int nWidth,
                      __in int nHeight,
                      __in_opt HWND hWndParent,
                      __in_opt HMENU hMenu,
                      __in_opt HINSTANCE hInstance,
                      __in_opt LPVOID lpParam)) 
     (IAT_CreateWindowExA)) ( 0L,
                      rdataSegp->classNameString, 
                      rdataSegp->captionString, 
                      WS_OVERLAPPEDWINDOW&~WS_SIZEBOX, 
                      0, 
                      0, 
                      250, 
                      300, 
                      NULL, 
                      NULL, 
                      inst, 
                      NULL);

  // ------------------------------------------------------------------------------------
  (* (HWND (WINAPI **)(__in DWORD dwExStyle,
                      __in_opt LPCSTR lpClassName,
                      __in_opt LPCSTR lpWindowName,
                      __in DWORD dwStyle,
                      __in int X,
                      __in int Y,
                      __in int nWidth,
                      __in int nHeight,
                      __in_opt HWND hWndParent,
                      __in_opt HMENU hMenu,
                      __in_opt HINSTANCE hInstance,
                      __in_opt LPVOID lpParam)) 
     (IAT_CreateWindowExA)) ( 0L,
                      rdataSegp->buttonString, 
                      rdataSegp->statusString, 
                      WS_CHILD | WS_VISIBLE|BS_GROUPBOX|WS_GROUP, 
                      4, 6, 206, 108,
                      hwnd, 0, inst, NULL);

  // ------------------------------------------------------------------------------------
  dataSeg->wndStatus = (* (HWND (WINAPI **)(__in DWORD dwExStyle,
                      __in_opt LPCSTR lpClassName,
                      __in_opt LPCSTR lpWindowName,
                      __in DWORD dwStyle,
                      __in int X,
                      __in int Y,
                      __in int nWidth,
                      __in int nHeight,
                      __in_opt HWND hWndParent,
                      __in_opt HMENU hMenu,
                      __in_opt HINSTANCE hInstance,
                      __in_opt LPVOID lpParam)) 
     (IAT_CreateWindowExA)) ( 0L,
                      rdataSegp->editClassString, 
                      rdataSegp->emptyString, 
                      WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | ES_READONLY, 
                      10, 22, 194, 90,
                      hwnd, 0, inst, NULL);

  // ------------------------------------------------------------------------------------
  (* (HWND (WINAPI **)(__in DWORD dwExStyle,
                      __in_opt LPCSTR lpClassName,
                      __in_opt LPCSTR lpWindowName,
                      __in DWORD dwStyle,
                      __in int X,
                      __in int Y,
                      __in int nWidth,
                      __in int nHeight,
                      __in_opt HWND hWndParent,
                      __in_opt HMENU hMenu,
                      __in_opt HINSTANCE hInstance,
                      __in_opt LPVOID lpParam)) 
     (IAT_CreateWindowExA)) ( 0L,
                      rdataSegp->buttonString, 
                      rdataSegp->eventString, 
                      WS_CHILD | WS_VISIBLE|BS_GROUPBOX|WS_GROUP, 
                      4, 6+120, 206, 108,
                      hwnd, 0, inst, NULL);

  // ------------------------------------------------------------------------------------
  dataSeg->wndEvent = (* (HWND (WINAPI **)(__in DWORD dwExStyle,
                      __in_opt LPCSTR lpClassName,
                      __in_opt LPCSTR lpWindowName,
                      __in DWORD dwStyle,
                      __in int X,
                      __in int Y,
                      __in int nWidth,
                      __in int nHeight,
                      __in_opt HWND hWndParent,
                      __in_opt HMENU hMenu,
                      __in_opt HINSTANCE hInstance,
                      __in_opt LPVOID lpParam)) 
     (IAT_CreateWindowExA)) ( 0L,
                      rdataSegp->editClassString, 
                      rdataSegp->emptyString, 
                      WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | ES_READONLY, 
                      10, 22+120, 194, 90,
                      hwnd, 0, inst, NULL);

  dataSeg->holdWindow = 1;

  (* (BOOL (WINAPI **)(HWND, int)) 
     (IAT_ShowWindow)) ( hwnd, SW_SHOWNORMAL);
}

#if 0
// 语法上的程序入口点
int WINAPI _tWinMain ( HINSTANCE instance_cur, // 当前实例
  HINSTANCE instance_prev,// 之前实例
  LPTSTR cmd_line, // 命令行
  int cmd_show // 如何显示
  )

{
  __asm call __window_main_
}

# if defined (_DEBUG)
int main (void)
{
  _tWinMain (GetModuleHandle (NULL), NULL, NULL, SW_SHOWNORMAL);
  return 0;
}
# endif 

# endif 