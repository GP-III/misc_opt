# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CMAINFRAME_INCLUED_HEADER_CXX_)
#  define _CMAINFRAME_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "resource.h"
# include "CAbout.hxx"
# include "CPianoWindow.hxx"
# include "CDirectInput.hxx"
# include "CDirectMusic2.hxx"
# include "CViewPaneT.hxx"
# include "CLockLessDIKMsgQueue.hxx"
# include "CThreadTiming.hxx"
# include "CPlayerThread.hxx"
# include <list>

class CMainFrame : public dockwins::CDockingFrameImpl<CMainFrame>, CIdleHandler
{
  typedef dockwins::CDockingFrameImpl<CMainFrame> baseClass;

  //  some global settings. 
  static  const LONG64 sm_refresh_persecond = 32;

  DWORD m_process_id;
  BOOL m_bTopMostWindow;

  BOOL OnIdle (void)  {

    BOOL bDrawPiano = FALSE;
    CThreadTiming::TSTiming tstiming;

    m_ThreadTiming.GetTimingCurrent (tstiming);
    if (tstiming.elapse > 1000 / sm_refresh_persecond) {
      bDrawPiano = TRUE;
      m_ThreadTiming.SetTimingStart (NULL);

      m_PianoWindow->BeginBatchDraw ();
      if (m_bTopMostWindow != FALSE) {
        DWORD pid;
        ::GetWindowThreadProcessId (:: GetTopWindow ( GetDesktopWindow ()), & pid);
        if (pid != m_process_id)
          SetWindowPos (HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
      }
      if (m_PlayThread.m_cur_event != CPlayerThread ::EVENT_CUR_USER
        && m_PlayThread.m_cur_event != CPlayerThread ::EVENT_CUR_RECORDING) {
        m_PianoWindow->EndBatchDraw ();
        return FALSE;
      }
      // pop message. 
      int count = CDirectInput::sm_buffer_nums;
      CLockLessDIKMsgQueue::DikRaw buf[CDirectInput::sm_buffer_nums];

      m_PlayThread.m_KeyMsgQueue.pop_msg (buf, & count);

      // ATLTRACE (_T (" ------\n "));
      CString str;
      for (int id = 0; id != count; id++) {
        // draw piano 
        CPianoDraw__::dummt_ *p = CPianoDraw__::dummt_map[buf[id].dikMap];

        if (buf[id].bPress != FALSE) {
          m_PianoWindow->drawPianoPress (p->octave_step, p->note);
          m_dumlist.push_back (p);
        } else {
          m_PianoWindow->drawPianoUp (p->octave_step, p->note);
          m_dumlist.remove (p);
        }
        
      }
      str.AppendFormat (_T ("level:%d "), m_PlayThread.m_dwOctaveMiddle);
      for (std::list<CPianoDraw__::dummt_ *>::iterator iter
        = m_dumlist.begin ();
        iter != m_dumlist.end ();
      iter ++)
      {
        str.AppendFormat (_T ("%s "), (*iter)->piano_scale);
      }
      m_StatusBar.SetText (0, str);
    }

   if ( bDrawPiano != FALSE) 
      m_PianoWindow->EndBatchDraw ();
   return FALSE;
  }
public:  
  // member .
  CCommandBarCtrl m_CmdBar;
  CSplitterWindow m_splitter;
  CPianoWindow *m_PianoWindow;
  CViewIoInstructments m_ViewIoInstructments;
  CViewPlayRecordSettings m_ViewPlayRecordSettings;

  CViewDS_EffectsReverb  m_ViewDS_EffectsReverb;
  CViewDS_EffectsReverbLevel2  m_ViewDS_EffectsReverb_Level2;
  CViewDS_EffectsCompressor m_ViewDS_EffectsCompressor;
  CViewDS_EffectsParamEQ m_ViewDS_EffectsParamEQ;
  CViewDS_EffectsDistortion  m_ViewDS_EffectsDistortion;
  CViewDS_EffectsGargle m_ViewDS_EffectsGargle;
  CViewDS_EffectsFlanger  m_ViewDS_EffectsFlanger;
  CViewDS_EffectsChorus m_ViewDS_EffectsChorus;
  CViewDS_EffectsEcho m_ViewDS_EffectsEcho;

  CPlayerThread m_PlayThread;
  CDirectMusic2 *m_DirectMusic;
  CDirectInput *m_DirectInput;
  CStatusBarCtrl m_StatusBar;
  CThreadTiming m_ThreadTiming;

  // member. 
  std::list<CPianoDraw__::dummt_ *> m_dumlist;

  CMainFrame (void) {
    m_PianoWindow = NULL;
    m_DirectMusic = NULL;
    m_bTopMostWindow = FALSE;
  }
public:
  DECLARE_FRAME_WND_CLASS(_T("CMainFrame"), IDR_MAINFRAME)

  BOOL PreTranslateMessage(MSG* pMsg)
  {
    pMsg;
    return FALSE;
  }

  BEGIN_MSG_MAP (CMainFrame)
    MESSAGE_HANDLER (WM_CREATE, OnCreate)
    MESSAGE_HANDLER (WM_DESTROY, OnDestroy)
    COMMAND_ID_HANDLER (ID_PLAY_BACKGROUND, OnResetPlayType)
    COMMAND_ID_HANDLER (ID_TOP_MOST_WINDOW, OnResetTopMostWindow)
    COMMAND_ID_HANDLER (ID_IO_INSTRUCTMENTS, OnViewIoInstructments)
    COMMAND_ID_HANDLER (ID_EFFECT_ECHO, OnViewDS_EffectsEcho)
    COMMAND_ID_HANDLER (ID_EFFECT_REVERB, OnViewDS_EffectsReverb)
    COMMAND_ID_HANDLER (ID_EFFECT_I3DL2REVERB, OnViewDS_EffectsReverb2)
    COMMAND_ID_HANDLER (ID_EFFECT_PARAMEQ, OnViewDS_EffectsParamEQ)
    COMMAND_ID_HANDLER (ID_EFFECT_GARGLE, OnViewDS_EffectsGargle)
    COMMAND_ID_HANDLER (ID_EFFECT_DISTORTION, OnViewDS_EffectsDistortion)
    COMMAND_ID_HANDLER (ID_EFFECT_COMPRESSOR, OnViewDS_EffectsCompressor)
    COMMAND_ID_HANDLER (ID_EFFECT_CHORUS, OnViewDS_EffectsChorus)
    COMMAND_ID_HANDLER (ID_EFFECT_FLANGER, OnViewDS_EffectsFlanger)
    COMMAND_ID_HANDLER (ID_RECORD_WINDOW, OnViewPlayRecordSettings)
    COMMAND_ID_HANDLER (ID_ABOUT_ATL, OnAboutATL)
    COMMAND_ID_HANDLER (ID_ABOUT_WTL, OnAboutWTL)
    CHAIN_MSG_MAP (baseClass)
  END_MSG_MAP()

  LRESULT OnViewPlayRecordSettings (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewPlayRecordSettings.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }
  LRESULT OnViewIoInstructments (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewIoInstructments.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }
  LRESULT OnViewDS_EffectsFlanger (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsFlanger.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  } 
  LRESULT OnViewDS_EffectsChorus (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsChorus.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  } 
  LRESULT OnViewDS_EffectsCompressor (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsCompressor.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }  
  LRESULT OnViewDS_EffectsEcho (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsEcho.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }  
  LRESULT OnViewDS_EffectsGargle (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsGargle.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }
  LRESULT OnViewDS_EffectsReverb (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsReverb.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }
  LRESULT OnViewDS_EffectsReverb2 (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsReverb_Level2.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }
  LRESULT OnViewDS_EffectsDistortion (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) { 
    return m_ViewDS_EffectsDistortion.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }
  LRESULT OnViewDS_EffectsParamEQ (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    return m_ViewDS_EffectsParamEQ.OnTestCreate (*this, m_CmdBar.GetMenu (), m_DirectMusic);
  }

  LRESULT OnResetTopMostWindow (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    if ( MF_CHECKED == m_CmdBar.GetMenu().GetMenuState (ID_TOP_MOST_WINDOW, MF_BYCOMMAND)) {
      m_bTopMostWindow  = FALSE;
      m_CmdBar.GetMenu().CheckMenuItem (ID_TOP_MOST_WINDOW, MF_BYCOMMAND | MF_UNCHECKED);
      SetWindowPos (HWND_NOTOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
    } else {
      m_bTopMostWindow  = TRUE;
      m_CmdBar.GetMenu().CheckMenuItem (ID_TOP_MOST_WINDOW, MF_BYCOMMAND | MF_CHECKED);
      SetWindowPos (HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
    }
    return 0;
  }

  LRESULT OnResetPlayType (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    if ( MF_CHECKED == m_CmdBar.GetMenu().GetMenuState (ID_PLAY_BACKGROUND, MF_BYCOMMAND)) {
      m_PlayThread.SetPlayForeground ();
      m_CmdBar.GetMenu().CheckMenuItem (ID_PLAY_BACKGROUND, MF_BYCOMMAND | MF_UNCHECKED);
    } else {
      m_PlayThread.SetPlayBackground ();
      m_CmdBar.GetMenu().CheckMenuItem (ID_PLAY_BACKGROUND, MF_BYCOMMAND | MF_CHECKED);
    }
    return 0;
  }

  LRESULT OnAboutATL(WORD wNotifyCode, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
  {
    CAboutATL c;
    c.DoModal ();

    return 0;
  }
  LRESULT OnAboutWTL(WORD wNotifyCode, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
  {
    CAboutWTL c;
    c.DoModal ();

    return 0;
  }

  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    // init docksetting.
    InitializeDockingFrame();

    // make single splitter window.
    // for child tab continer 
    ImmDisableIME (-1);

    m_hWndClient = m_splitter.Create(m_hWnd, rcDefault, NULL,
          WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    m_splitter.SetSplitterExtendedStyle(SPLIT_PROPORTIONAL | SPLIT_NONINTERACTIVE);
    m_splitter.SetSinglePaneMode (SPLIT_PANE_LEFT);
    
    HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
    // attach menu
    m_CmdBar.AttachMenu(GetMenu ());
    //SetMenu (NULL);
    SetWindowText (_T ("midi_piano"));
    // load command bar images
    m_CmdBar.LoadImages(IDR_MAINFRAME);
    // remove old menu
    SetMenu(NULL);
    // this->AddSimpleReBarBand
    HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

    CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
    AddSimpleReBarBand(hWndCmdBar);
    //AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

    CreateSimpleStatusBar();
    m_StatusBar = m_hWndStatusBar;

    :: EnableMenuItem (  m_CmdBar.GetMenu (), ID_EFFECT_3DEFFECT, MF_GRAYED | MF_BYCOMMAND );

    CMessageLoop *p = _Module.GetMessageLoop ();
    p->AddIdleHandler (this);

    m_process_id = GetCurrentProcessId ();
    return 0;
  }

  void CreateMidiPiano (void) {
    CViewIoInstructmentsBase:: _CViewIoData *iodata;
    m_ViewIoInstructments.m_panel.GetIoDataRef (iodata);

    m_DirectMusic = new CDirectMusic2;
    m_DirectMusic->AdjustCollectionIndex (0);
    m_DirectMusic->AdjustInstructmentsIndex (0);

    m_DirectInput = new CDirectInput;
    m_DirectInput->InitDeviceKeyboard (*this, CDirectInput ::FOREGROUND);

    m_PianoWindow = new CPianoWindow;
    m_PianoWindow->Create (m_splitter);
    //m_PianoWindow->Create (m_hWnd);
    m_splitter.SetSplitterPanes(m_PianoWindow->m_hWnd, 0);

    // Bind cpu0
    m_ThreadTiming.SetThreadAffinity (1);
    m_ThreadTiming.UseMMSystemTiming (FALSE);

    m_PlayThread.SetAttach (m_DirectInput, m_DirectMusic, iodata, & m_ThreadTiming);
    m_PlayThread.Init ();

    m_ViewPlayRecordSettings.m_panel.SetAttachThread (& m_PlayThread);
  }

  LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
      m_PlayThread.terminate ();

      ::PostQuitMessage(1);
      return 0;
  }
  LRESULT OnPaintBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
      HDC dc = (HDC)wParam;
      HBRUSH hbrush=::CreateSolidBrush(RGB(0,0,0)); 
      RECT rcRect;
      GetClientRect (& rcRect);
      :: FillRect (dc, & rcRect, hbrush);
      :: DeleteObject (hbrush);
      bHandled = TRUE;
      return !0; // https://msdn.microsoft.com/en-us/library/ms648055(VS.85).aspx
  }   
  LRESULT OnPaint (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM , BOOL& /*bHandled*/) 
  {
    CPaintDC dc(m_hWnd);
    
    return 0;
  }
  LRESULT OnPaintBackground2(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
      HDC dc = (HDC)wParam;
      HBRUSH hbrush=::CreateSolidBrush(RGB(0,0,0)); 
      RECT rcRect;
      GetClientRect (& rcRect);
      :: FillRect (dc, & rcRect, hbrush);
      :: DeleteObject (hbrush);
      bHandled = TRUE;
      return !0; // https://msdn.microsoft.com/en-us/library/ms648055(VS.85).aspx
  }  
};

# endif // !defined(_STD_WTL_INCLUED_HEADER_CXX_)