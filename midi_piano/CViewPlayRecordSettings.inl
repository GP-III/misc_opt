# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  XXX: bad code to fix.
# if defined (__INL_PANE_ENABLE)

# include "CFile_eps.hxx"
# include "CPlayerThread.hxx"

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewPlayRecordSettingsBaseTraits;

// # include "CDirectDraw.hxx"
class CViewPlayRecordSettingsBase:public CWindowImpl<CViewPlayRecordSettingsBase,CWindow,CViewPlayRecordSettingsBaseTraits>,
    public CScrollImpl<CViewPlayRecordSettingsBase>,
    public CIdleHandler
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 372;
  static  const  int height = 280;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewPlayRecordSettingsBase,CWindow,CViewPlayRecordSettingsBaseTraits> baseClass;
  typedef    CScrollImpl<CViewPlayRecordSettingsBase> baseClass2;
  typedef CViewPlayRecordSettingsBase  thisClass;
  
  // ctrl .
  CEdit m_GuiET_OutMidiMessage;
  CEdit m_GuiET_FilePath;
  CListViewCtrl m_GuiLV_MidiMessage;
  CStatic m_GuiST_Timing;
  CTrackBarCtrl m_GuiTK_Timing;
  CButton m_GuiBT_LoadEps;
  CButton m_GuiBT_PlayEps; 
  CButton m_GuiBT_Record;   
  CButton m_GuiBT_SaveEps; 
  CButton m_GuiBT_ClearMidiMsg; 
  CFile_eps m_eps_play;
  CFile_eps m_eps_record;
  CDirectMusic2 *m_DirectMusic;
  CPlayerThread *m_AttachPlayerThread;

  BOOL m_bDragTrackBar;

  int m_ButtonPlayStatus; // 0:none 1:playing.
  int m_ButtonRecordStatus; // 0:none 1:recording.
  int m_ButtonLoadFile;  // 0:none 1:exist.
  int m_ButtonRecordStatusDone; //0:none 1:exist.

  int m_CurItemNums;

  CViewPlayRecordSettingsBase (void)  {
    m_bDragTrackBar = FALSE;
  }

  void __callsetitem (BYTE msg, BYTE vel) {
    //return  ;
    CViewPlayRecordSettingsBase *t = (CViewPlayRecordSettingsBase *)this;
    TCHAR szBuf[48];

    //  MSG TYPE NOTE-A NOTE-B LEVEL  
    t->m_GuiLV_MidiMessage.InsertItem (t->m_CurItemNums, _T ("44"), -1);
    _stprintf (szBuf, _T ("%02X"), msg & 0x7F);
    t->m_GuiLV_MidiMessage.SetItemText (t->m_CurItemNums, 0, szBuf);
    t->m_GuiLV_MidiMessage.SetItemText (t->m_CurItemNums, 1, (msg & 0x80) ? _T ("ON") : _T ("OFF"));
    static  TCHAR *__ap[12] =  {
      _T ("1"), _T ("#1"), _T ("2"), _T ("#2"),
      _T ("3"), _T ("4"), _T ("#4"), _T ("5"),
      _T ("#5"), _T ("6"), _T ("#6"), _T ("7")
    };
    static  TCHAR *__ap2[12] =  {
      _T ("C"), _T ("C#"), _T ("D"), _T ("D#"),
      _T ("E"), _T ("F"), _T ("F#"), _T ("G"),
      _T ("G#"), _T ("A"), _T ("A#"), _T ("B")
    };
    t->m_GuiLV_MidiMessage.SetItemText (t->m_CurItemNums, 2, __ap[(msg & 0x7F) % 12]);
    t->m_GuiLV_MidiMessage.SetItemText (t->m_CurItemNums, 3, __ap2[(msg & 0x7F) % 12]);
    _stprintf (szBuf, _T ("%d"), (msg & 0x7F) / 12 - 1);
    t->m_GuiLV_MidiMessage.SetItemText (t->m_CurItemNums, 4, szBuf);
    _stprintf (szBuf, _T ("%d"), vel & 0x7F);
    t->m_GuiLV_MidiMessage.SetItemText (t->m_CurItemNums, 5, szBuf);
    t->m_GuiLV_MidiMessage.EnsureVisible (t->m_CurItemNums, FALSE);
    t->m_CurItemNums ++;
  }

  DECLARE_WND_CLASS(_T ("CViewPlayRecordSettingsBase"))

  BEGIN_MSG_MAP (thisClass)
    MESSAGE_HANDLER (WM_CREATE, OnCreate)
    MESSAGE_HANDLER (WM_HSCROLL, OnSlider)
    COMMAND_HANDLER2 (m_GuiBT_Record, BN_CLICKED, OnSetRecord)
    COMMAND_HANDLER2 (m_GuiBT_SaveEps, BN_CLICKED, OnSaveEps)
    COMMAND_HANDLER2 (m_GuiBT_LoadEps, BN_CLICKED, OnLoadEps)
    COMMAND_HANDLER2 (m_GuiBT_PlayEps, BN_CLICKED, OnPlayEps)
    COMMAND_HANDLER2 (m_GuiBT_ClearMidiMsg, BN_CLICKED, OnClearMidiMessage)
    CHAIN_MSG_MAP (baseClass2)
  END_MSG_MAP()

  void DoPaint (CDCHandle dc) {
    CRect t;
    GetClientRect (t);
    t.OffsetRect ( m_ptOffset);
    dc.FillSolidRect (& t, background_color);
  }

  void DisableButtonWhoALL (HWND except_window = NULL) {
    m_GuiBT_LoadEps.EnableWindow (FALSE);
    m_GuiBT_SaveEps.EnableWindow (FALSE);
    m_GuiBT_Record.EnableWindow (FALSE);
    m_GuiBT_PlayEps.EnableWindow (FALSE);
    if ( except_window != NULL)
      :: EnableWindow (except_window, TRUE);
  }
  void EnableButtonALL (void) {
    m_GuiBT_LoadEps.EnableWindow (TRUE);
    m_GuiBT_SaveEps.EnableWindow (TRUE);
    m_GuiBT_Record.EnableWindow (TRUE);
    m_GuiBT_PlayEps.EnableWindow (TRUE);
  }
  void CheckRecordAndPlayEpsBut (void) {
    if (m_ButtonLoadFile != 0) 
      m_GuiBT_PlayEps.EnableWindow (TRUE);
    else 
      m_GuiBT_PlayEps.EnableWindow (FALSE);
    if (m_ButtonRecordStatusDone != 0)
      m_GuiBT_SaveEps.EnableWindow (TRUE);
    else 
      m_GuiBT_SaveEps.EnableWindow (FALSE);
  }
  LRESULT OnClearMidiMessage (UINT /*uMsg*/, WPARAM wParam, HWND hWndCtl, BOOL& bHandled) {
    // save record. 
    m_GuiLV_MidiMessage.DeleteAllItems ();
    m_CurItemNums = 0;
    return 0;
  }
  LRESULT OnSaveEps (UINT /*uMsg*/, WPARAM wParam, HWND hWndCtl, BOOL& bHandled) {
    // save record. 
    CFileDialogFilter strFilter(_T("eps file (*.eps;)")
                                _T("|             *.eps;||") );
    CFileDialog fd( FALSE, _T (".eps"), NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER, strFilter, this->m_hWnd );
    if ( fd.DoModal () == IDOK) 
      m_eps_record.WriteEPS (fd.m_szFileName);
    return 0;
  }
  LRESULT OnPlayEps (UINT /*uMsg*/, WPARAM wParam, HWND hWndCtl, BOOL& bHandled) {
    if  (m_ButtonPlayStatus == 0)  {
      m_AttachPlayerThread->setEps_attach (& m_eps_play);
      m_AttachPlayerThread->SetPlayEps ();
      // m_AttachPlayerThread->SetPlayNoteCallback (__callsetitem, this);
      m_GuiBT_PlayEps.SetWindowText (_T ("Playing..."));
      DisableButtonWhoALL (m_GuiBT_PlayEps);
      m_GuiTK_Timing.EnableWindow (TRUE);
      m_GuiST_Timing.EnableWindow (TRUE);
      m_GuiTK_Timing.SetRange (0, m_eps_play.m_totaltime/1000);
      m_GuiTK_Timing.SetPos (0); 
      m_ButtonPlayStatus = 1;
      m_ButtonRecordStatus = 0;
    }  else   {
      // Stop play. 
      m_AttachPlayerThread->SetPlay ();
      m_AttachPlayerThread->SetPlayNoteCallback (NULL, NULL);
      m_GuiBT_PlayEps.SetWindowText (_T ("Play .eps files"));
      EnableButtonALL ();
      m_GuiTK_Timing.EnableWindow (FALSE);
      m_GuiST_Timing.EnableWindow (FALSE);
      m_GuiST_Timing.SetWindowText (_T ("-:-"));
      m_GuiTK_Timing.SetPos (0); 
      m_ButtonPlayStatus = 0;
      CheckRecordAndPlayEpsBut ();
    }
    return 0;
  }
  LRESULT OnLoadEps (UINT /*uMsg*/, WPARAM wParam, HWND hWndCtl, BOOL& bHandled) {
    // load record. 
    CFileDialogFilter strFilter(_T("eps file (*.eps;)")
                                _T("|             *.eps;||") );
    CFileDialog fd( TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER, strFilter, this->m_hWnd );
    if ( fd.DoModal () == IDOK)  {
      m_ButtonLoadFile = 1;
      m_eps_play.ReadEPS (fd.m_szFileName);
      m_GuiET_FilePath.SetWindowTextW (fd.m_szFileName);
      m_GuiBT_PlayEps.EnableWindow (TRUE);
    }
    return 0;
  }
  LRESULT OnSetRecord (UINT /*uMsg*/, WPARAM wParam, HWND hWndCtl, BOOL& bHandled) {
    if  (m_ButtonRecordStatus == 0)  {
      m_AttachPlayerThread->setEps_attach (& m_eps_record);
      m_AttachPlayerThread->SetRecordStart ();
      m_GuiBT_Record.SetWindowText (_T ("Recording..."));
      DisableButtonWhoALL (m_GuiBT_Record);
      m_GuiTK_Timing.EnableWindow (FALSE);
      m_GuiST_Timing.EnableWindow (FALSE);
      m_ButtonRecordStatus = 1;
      m_ButtonPlayStatus = 0;
    }  else  {
      // compelte .
      m_AttachPlayerThread->SetRecordEnd ();
      m_GuiBT_Record.SetWindowText (_T ("Record"));
      EnableButtonALL ();
      m_ButtonRecordStatus = 0;
      m_ButtonRecordStatusDone = 1;
      CheckRecordAndPlayEpsBut ();
    }
    return 0;
  }

  void SetAttach (CDirectMusic2 *cmusic) {
    m_DirectMusic = cmusic;
  }
  void SetAttachThread (CPlayerThread *thread) {
    m_AttachPlayerThread = thread;
  }

  LRESULT OnSlider(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    WORD code = LOWORD (wParam);
    if (code == TB_ENDTRACK || code == TB_THUMBTRACK)  {
      if  ( (HWND) lParam == m_GuiTK_Timing)  {
        if (code == TB_THUMBTRACK) 
          m_bDragTrackBar = TRUE;
        else {
          m_bDragTrackBar = FALSE;
          m_AttachPlayerThread->suspend ();
          // SetAudio pos. 
          int c =  m_GuiTK_Timing.GetPos ();
          m_eps_play.SetPos (c *1000);
          m_AttachPlayerThread->m_CurTiming = c * 1000;
          m_AttachPlayerThread->resume_ ();
        }
      } 
    }
    bHandled = FALSE;
    return 0;
  }

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    
    SetClassLongPtr (m_hWnd, GCLP_HBRBACKGROUND, (LONG_PTR) GetStockObject (NULL_BRUSH)) ;
    HINSTANCE hInst = _Module.GetModuleInstance ();
    HWND hWindow = m_hWnd;

    HFONT hFont = CreateFont (-11, 0, 0, 0, 400, FALSE, FALSE, FALSE, 1, 400, 0, 0, 0, _T ("MS Shell Dlg"));

    HWND T = CreateWindowEx(0, WC_STATIC, _T (""), WS_VISIBLE | WS_CHILD | SS_ETCHEDHORZ, 11, 231, 345, 2, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);

    // Button Group. 
    m_GuiBT_LoadEps = CreateWindowEx (0, WC_BUTTON, _T ("Load .eps files"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 8, 171, 84, 23, hWindow, (HMENU)-1, hInst, 0);
    m_GuiBT_LoadEps.SetFont (hFont, FALSE);
    m_GuiBT_PlayEps = CreateWindowEx (0, WC_BUTTON, _T ("Play .eps files"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 8, 200, 84, 23, hWindow, (HMENU)-1, hInst, 0);
    m_GuiBT_PlayEps.SetFont (hFont, FALSE);
    m_GuiBT_Record = CreateWindowEx (0, WC_BUTTON, _T ("Record"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 8, 241, 84, 23, hWindow, (HMENU)-1, hInst, 0);
    m_GuiBT_Record.SetFont (hFont, FALSE);
    m_GuiBT_SaveEps = CreateWindowEx (0, WC_BUTTON, _T ("Save record"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 101, 241, 84, 23, hWindow, (HMENU)-1, hInst, 0);
    m_GuiBT_SaveEps.SetFont (hFont, FALSE);

    // CSlider Group.
    m_GuiTK_Timing = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 95, 203, 201, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Timing.SetFont (hFont, FALSE);

    // ListView Group 
    m_GuiLV_MidiMessage = CreateWindowEx(0, WC_LISTVIEW, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | LVS_ALIGNLEFT | LVS_REPORT, 11, 10, 345, 148, hWindow, (HMENU)-1, hInst, 0);
    m_GuiLV_MidiMessage.SetFont (hFont, FALSE);

    m_GuiET_FilePath = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 102, 171, 254, 24, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_FilePath.SetFont (hFont, FALSE);
    
    // Static text 
    m_GuiST_Timing = CreateWindowEx(0, WC_STATIC, _T ("-:-"), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD | WS_GROUP | SS_CENTER, 311, 205, 45, 21, hWindow, (HMENU)-1, hInst, 0);
    m_GuiST_Timing.SetFont (hFont, FALSE);

    m_GuiBT_ClearMidiMsg = CreateWindowEx(0, WC_BUTTON, _T ("Clear MidiMsg"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 195, 241, 84, 23, hWindow, (HMENU)-1, hInst, 0);
    m_GuiBT_ClearMidiMsg.SetFont (hFont, FALSE);

    m_GuiBT_PlayEps.EnableWindow (FALSE);
    m_GuiBT_SaveEps.EnableWindow (FALSE);
    m_GuiTK_Timing.EnableWindow (FALSE);

    //  MSG TYPE NOTE-A NOTE-B LEVEL 
    // m_GuiLV_MidiMessage.C
    m_GuiLV_MidiMessage.InsertColumn (0, _T ("MSG"),      LVCFMT_CENTER, 38 );
    m_GuiLV_MidiMessage.InsertColumn (1, _T ("TYPE"),     LVCFMT_CENTER, 46 );
    m_GuiLV_MidiMessage.InsertColumn (2, _T ("NOTE-A"),   LVCFMT_CENTER, 59 );
    m_GuiLV_MidiMessage.InsertColumn (3, _T ("NOTE-B"),   LVCFMT_CENTER, 59 );
    m_GuiLV_MidiMessage.InsertColumn (4, _T ("LEVEL"),    LVCFMT_CENTER, 52 );
    m_GuiLV_MidiMessage.InsertColumn (5, _T ("VEL"),    LVCFMT_CENTER, 38 );

    DWORD dwStyle = m_GuiLV_MidiMessage.GetExtendedListViewStyle(); 
    dwStyle |= LVS_EX_GRIDLINES; 
    m_GuiLV_MidiMessage.SetExtendedListViewStyle (dwStyle); 

    m_ButtonPlayStatus = 0;
    m_ButtonRecordStatus = 0;
    m_ButtonLoadFile = 0;
    m_ButtonRecordStatusDone = 0;
    m_CurItemNums = 0;

    _Module.GetMessageLoop ()->AddIdleHandler (this);
    return 0;
  }
  BOOL OnIdle (void)  {
    TCHAR szBuf[48];
    if (m_bDragTrackBar != FALSE) {
      DWORD time = m_GuiTK_Timing.GetPos ();
      _stprintf (szBuf, _T ("%d:%d"), time, m_eps_play.m_totaltime/ 1000);
      m_GuiST_Timing.SetWindowText (szBuf);
    } else if (m_ButtonPlayStatus != 0)  {
      DWORD time = m_AttachPlayerThread->m_CurTiming/1000;
      m_GuiTK_Timing.SetPos (time);
      _stprintf (szBuf, _T ("%d:%d"), time, m_eps_play.m_totaltime/ 1000);
      m_GuiST_Timing.SetWindowText (szBuf);
      // Check/set List.
      int count = CDirectInput::sm_buffer_nums;
      CLockLessDIKMsgQueue::DikRaw2 buf[CDirectInput::sm_buffer_nums];
      m_AttachPlayerThread->m_KeyMsgQueue.pop_msg (buf, & count);
      for (int id = 0; id != count; id++) 
         __callsetitem (buf[id].msg, buf[id].vel);
    } 
    return TRUE;
  }
};


# endif // !defined(_CVIEWPLAYRECORDSETTINGS_INCLUED_HEADER_CXX_