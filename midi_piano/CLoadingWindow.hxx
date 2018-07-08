# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CLOADINGWINDOW_INCLUED_HEADER_CXX_)
#  define _CLOADINGWINDOW_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "resource.h"
# include "CDirectDraw.hxx"
# include <dinput.h>
# include <atlutil.h>
# include <atlimage.h>

//  Get IStream pointer for ATL::CImage::Load (IStream *pStream) 
//  Ref > https://blog.csdn.net/Templar1000/article/details/27423699
BOOL loadImage (CImage &img, UINT nResID, LPCTSTR lpType)  {

  img.Destroy();

  HINSTANCE resInstance = _Module.GetResourceInstance ();
  HRSRC hRsrc =::FindResource (resInstance, MAKEINTRESOURCE (nResID), lpType);
  ATLASSERT (hRsrc != NULL);

  // load resource 
  HGLOBAL hImgData = ::LoadResource (resInstance, hRsrc);
  ATLASSERT (hImgData != NULL);

  // lock resourec
  LPVOID lpVoid    = ::LockResource (hImgData);
  ATLASSERT (lpVoid != NULL);

  LPSTREAM pStream =NULL;
  DWORD dwSize     = ::SizeofResource (resInstance, hRsrc);
  HGLOBAL hNew     = ::GlobalAlloc (GHND, dwSize);
  LPBYTE lpByte    = (LPBYTE)::GlobalLock (hNew);
  ::memcpy (lpByte, lpVoid, dwSize);

  // unlock resourec
  ::GlobalUnlock(hNew);

  HRESULT sig = :: CreateStreamOnHGlobal (hNew, TRUE, & pStream);
  ATLASSERT (SUCCEEDED (sig));

  img.Load (pStream);
  ::FreeResource (hImgData);

  return TRUE;
}

//   so bad. 
#   if OLD_EASY_TRY
class CWorkThreadClient :public IWorkerThreadClient {
public:
  CWorkThreadClient (void) {
    m_Param = 0;
    m_result = NULL; // for host return value .
    m_hEvent = NULL;
    m_bQuitMessageLoop = NULL;
    m_ProcExecute = NULL;
    
    m_hEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
    ATLASSERT (m_hEvent != NULL); 
  }
  ~ CWorkThreadClient (void) {}

  HRESULT SetAttach (
       HRESULT *result, 
       PVOID param, 
       BOOL *bQuitMessageLoop,
       HRESULT (*Execute) (PVOID param),
       CWorkerThread<CRTThreadTraits> *AttachThread)
  { 
    m_result = result; 
    m_Param = param;
    m_bQuitMessageLoop = bQuitMessageLoop;
    m_ProcExecute = Execute;
    m_AttachThread = AttachThread;

    * m_bQuitMessageLoop = FALSE;
    * m_result = 0;
    return 0;
  }
  HRESULT Execute(
     DWORD_PTR dwParam,
     HANDLE hObject) { 
       
      if (m_ProcExecute != NULL) 
        *m_result = m_ProcExecute ((PVOID) dwParam);
        * m_bQuitMessageLoop = TRUE;
          // m_AttachThread->RemoveHandle (m_hEvent);

       return *m_result ; 
   }


  HRESULT CloseHandle( HANDLE hHandle) { 
    :: CloseHandle (hHandle); 
    
    return 0;
  }
  
  HANDLE GetEvent (void) {
    return m_hEvent;
  }
private:
  PVOID m_Param;
  HRESULT *m_result; // for host return value .
  HANDLE m_hEvent;
  BOOL *m_bQuitMessageLoop;
  HRESULT (*m_ProcExecute) (PVOID param);
  CWorkerThread<CRTThreadTraits> *m_AttachThread;
};

class CLoadingWindow : public CFrameWindowImpl<CLoadingWindow> 
{
public:
  DECLARE_FRAME_WND_CLASS (_T("CLoadingWindow"), -1) 

  BEGIN_MSG_MAP (CMainFrame)  
    MESSAGE_HANDLER (WM_CREATE, OnCreate)  
    MESSAGE_HANDLER (WM_PAINT, OnPaint)   
    CHAIN_MSG_MAP (CFrameWindowImpl<CLoadingWindow>)  
  END_MSG_MAP ()  


  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    loadImage (m_LoadingImg, IDB_LOADING, _T ("PNG"));
    ResizeClient (480, 320, FALSE);
    CenterWindow (NULL);
    SetClassLongPtr (*this, GCLP_HBRBACKGROUND, (LONG_PTR) GetStockObject (NULL_BRUSH)) ;
    return 0;
  }
  LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    CPaintDC dc (*this);
    dc.TransparentBlt (0, 0, 480, 320, m_LoadingImg.GetDC (), 0, 0, 480, 320, 0x00FFFFFF);
    m_LoadingImg.ReleaseDC ();
    return 0;
  }

  int Run () {
    //  create init thread 
    //  self draw loading image and window . 
    m_WorkThread.Initialize ();
    m_WorkThread.AddHandle (m_WorkClient.GetEvent (), & m_WorkClient, (DWORD_PTR) m_Param);

    // wake up . 
    ::SetEvent ( m_WorkClient.GetEvent ());

    while (m_bQuitMessageLoop == FALSE) {
      MSG msg;
      while (:: PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
        if(!PreTranslateMessage (&msg)) {
          ::TranslateMessage (&msg);
          ::DispatchMessage (&msg);
        }
      }
    }
    m_WorkThread.RemoveHandle (m_WorkClient.GetEvent ());
    m_WorkThread.Shutdown ();
    return m_result;
  }

  HRESULT GetResult (void)  {
    return m_result;
  }
  // void 
  HRESULT SetAttach (
       PVOID param, 
       HRESULT (*Execute) (PVOID param) )
  { 
    m_Param = param;
    m_ProcExecute = Execute;
    
    m_WorkClient.SetAttach (& m_result, param, & m_bQuitMessageLoop, Execute, & m_WorkThread);
    return 0;
  }

private:
  CWorkerThread<CRTThreadTraits> m_WorkThread;
  CWorkThreadClient m_WorkClient;
  HRESULT m_result;
  BOOL m_bQuitMessageLoop;
  HRESULT (*m_ProcExecute) (PVOID param);
  PVOID m_Param;
  CImage m_LoadingImg;
};
#  endif 

class CLoadingWindow : public CFrameWindowImpl<CLoadingWindow> 
{
public:
  DECLARE_FRAME_WND_CLASS (_T("CLoadingWindow"), -1) 

  BEGIN_MSG_MAP (CMainFrame)  
    MESSAGE_HANDLER (WM_CREATE, OnCreate)  
    MESSAGE_HANDLER (WM_PAINT, OnPaint)   
    CHAIN_MSG_MAP (CFrameWindowImpl<CLoadingWindow>)  
  END_MSG_MAP ()  

  CLoadingWindow (void) {
    loadImage (m_LoadingImg, IDB_LOADING, _T ("PNG"));
    
    m_SingalLoadingEvent = CreateEvent (NULL, FALSE, FALSE, NULL);  /* host close  */   
    ATLASSERT (m_SingalLoadingEvent != NULL);
  }
  
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    
    ResizeClient (418, 240, FALSE);
    CenterWindow (NULL);
    ShowWindow (SW_SHOWNORMAL);
    SetClassLongPtr (*this, GCLP_HBRBACKGROUND, (LONG_PTR) GetStockObject (NULL_BRUSH)) ;
    return 0;
  }
  LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    CPaintDC dc (*this);
    dc.TransparentBlt (0, 0, 418, 240, m_LoadingImg.GetDC (), 0, 0, 418, 240, 0x00FFFFFF);
    m_LoadingImg.ReleaseDC ();
    return 0;
  }

  int Run () {
    while (TRUE) {
      MSG msg;
      while (:: PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
        if(!PreTranslateMessage (&msg)) {
          ::TranslateMessage (&msg);
          ::DispatchMessage (&msg);
        }
      }
      //  wait singal. 
      DWORD sig = WaitForSingleObject (m_SingalLoadingEvent, 0);
      if (sig == WAIT_OBJECT_0) {
        ShowWindow (SW_HIDE);
        break  ;
      }    
    }
    return 0;
  }

  // void 
  HRESULT SetAttach (
              HANDLE *pSingalLoadingEvent   /* host close  */ )
  { 
    * pSingalLoadingEvent = m_SingalLoadingEvent;
    
    return 0;
  }

private:
  HANDLE m_SingalLoadingEvent;  /* host close  */
  CImage m_LoadingImg;
};

struct CLoading_desc {
  HANDLE SignalLoadingEvent;
  HANDLE ThreadWaitHandle;
  CLoadingWindow *pWindow;
  volatile   LONG min_spin;
};

static 
unsigned int __stdcall sm_ProcLoading (PVOID param) {

  CLoading_desc *p = (CLoading_desc *)param;
  p->pWindow = new CLoadingWindow;

  p->pWindow->SetAttach (& p->SignalLoadingEvent);

  // signal  host.
  InterlockedExchange (& p->min_spin, 1);

  p->pWindow->CreateEx (0, NULL, WS_POPUP);
  p->pWindow->Run ();

  return 0;
}

static void  initLoadingWindow ( CLoading_desc *p) {

  p->pWindow = NULL;
  p->min_spin = 0;
  p->SignalLoadingEvent = NULL;
  p->ThreadWaitHandle = NULL;

  p->ThreadWaitHandle = (HANDLE) _beginthreadex (NULL, 0,
       sm_ProcLoading, p, 0, NULL);
  
  //  wait event attach compelte .
  while (InterlockedExchange (& p->min_spin, 0) == 0)
    _mm_pause ();

  //  assert thread vail
  ATLASSERT ( p->ThreadWaitHandle != NULL);
}

static void ReleaseCLoading_desc  ( CLoading_desc *p) {
  CloseHandle (p->SignalLoadingEvent);
  CloseHandle (p->ThreadWaitHandle);
  if (p->pWindow != NULL) {
    // p->pWindow->ShowWindow (SW_HIDE);
    //delete  p->pWindow;
    //p->pWindow = NULL;
  }
}

# endif // !defined(_CLOADINGWINDOW_INCLUED_HEADER_CXX_)