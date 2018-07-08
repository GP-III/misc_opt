// 522.cpp : main source file for 522.exe
//

# include "stdwtl.h"
# include "CMainFrame.hxx"
# include "CDirectDraw.hxx"
# include "CMessageIdleFastLoop.hxx"
# include "CLoadingWindow.hxx"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
  // Add ATL MessageLoop map .
  CMessageIdleFastLoop theLoop;
  _Module.AddMessageLoop (& theLoop);

  // init loading window.
  CLoading_desc ld;
  initLoadingWindow (& ld);

  // Create main window
  CMainFrame wndMain;

  HWND hWindow = wndMain.CreateEx (NULL, CRect (0, 0, 48, 32), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW);
  ATLASSERT (hWindow != NULL);

  // Create Direct, misc object 
  CDirectDraw :: InitDirect3D ();   // Direct3D init
  CDirectInput :: InitDInput8 ();   // DInput8 init.
  CDirectMusic2 :: InitDirectMusic (wndMain); // DirectMusic, DirectSound init.

  // Set Process Pri.
  SetPriorityClass (GetCurrentProcess (), HIGH_PRIORITY_CLASS);
  SetProcessPriorityBoost (GetCurrentProcess (), TRUE);
  
  wndMain.CreateMidiPiano ();
  wndMain.CenterWindow (NULL);

  SetEvent (ld.SignalLoadingEvent);
  WaitForSingleObject (ld.ThreadWaitHandle, INFINITE);

  ReleaseCLoading_desc (& ld);

  wndMain.ResizeClient (360, 240);
  
  HWND hForeWnd = NULL; 
  HWND hWnd= wndMain; 
  DWORD dwForeID; 
  DWORD dwCurID; 
 
  hForeWnd =  GetForegroundWindow(); 
  dwCurID =  GetCurrentThreadId(); 
  dwForeID =  GetWindowThreadProcessId (hForeWnd, NULL); 
  AttachThreadInput (dwCurID, dwForeID, TRUE); 
  wndMain.CenterWindow (NULL);
  ShowWindow (hWnd, SW_SHOWNORMAL); 
  SetWindowPos (hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE); 
  SetWindowPos (hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE); 
  SetForegroundWindow (hWnd); 
  AttachThreadInput( dwCurID, dwForeID, FALSE);
  SetFocus (hWnd);
  SetWindowPos (hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE); 
  SetFocus (hWnd);
  SetWindowPos (hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE); 

  int nRet = theLoop.Run();
 
  _Module.RemoveMessageLoop();

  // Uninit Direct3D draw base 
  CDirectDraw :: UninitDirect3D ();
  CDirectMusic2 :: UninitDirectMusic ();
  CDirectInput :: UninitDInput8 ();
  return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
  HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//  HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
  ATLASSERT(SUCCEEDED(hRes));

  // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
  ::DefWindowProc(NULL, 0, 0, 0L);

  AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);  // add flags to support other controls

  hRes = _Module.Init(NULL, hInstance);
  ATLASSERT(SUCCEEDED(hRes));

  int nRet = Run(lpstrCmdLine, nCmdShow);

  _Module.Term();
  ::CoUninitialize();

  return nRet;
}