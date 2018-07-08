# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  XXX: bad code to fix.
# if defined (__INL_PANE_ENABLE)

# include "CDirectMusic2.hxx"

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewIoInstructmentsBaseTraits;

// # include "CDirectDraw.hxx"
class CViewIoInstructmentsBase:public CWindowImpl<CViewIoInstructmentsBase,CWindow,CViewIoInstructmentsBaseTraits>,
    public CScrollImpl<CViewIoInstructmentsBase>
{
public:
  struct _CViewIoData {
    BYTE FixedVel;
    BYTE OffsetNote;
    INT DlsId;
    TCHAR Dls_desc[MAX_PATH];
    INT InstrId;
    TCHAR Instr_desc[MAX_PATH];
    BOOL OpenFeedback;
    BOOL withTraningTime;
    DWORD TraningTime;
    DWORD toMaxTime;
    BYTE initVel;
    BYTE maxVel;

    _CViewIoData (void)  {
      FixedVel = 0x7F;
      OffsetNote = 0x00;
      DlsId = 0;
      InstrId = 0;
      OpenFeedback = FALSE;
      withTraningTime = FALSE;
      TraningTime = 127; // ms
      toMaxTime = 1274; // 
      initVel = 0x50;
      maxVel = 0x70;
    }
  };

  void GetIoDataRef (_CViewIoData *&p) {
    p = & m_IoData;
  }
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 328;
  static  const  int height = 254;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewIoInstructmentsBase,CWindow,CViewIoInstructmentsBaseTraits> baseClass;
  typedef    CScrollImpl<CViewIoInstructmentsBase> baseClass2;
  typedef CViewIoInstructmentsBase  thisClass;
  
  // attach DirectMusic Object.
  BYTE *m_FixedVel;
  BYTE *m_OffsetNote;
  CDirectMusic2 *m_DirectMusic;  
 _CViewIoData m_IoData;
 
  // ctrl 
  CComboBox m_GuiCB_DlsCollection;
  CComboBox m_GuiCB_Instructments;

 // CEdit m_GuiET_FixedVeloctity;
  CEditEnterStuff<CViewIoInstructmentsBase> m_GuiET_FixedVeloctity;
  CEditEnterStuff<CViewIoInstructmentsBase> m_GuiET_OffsetNote;
  CEditEnterStuff<CViewIoInstructmentsBase> m_GuiET_TraningTime;
  CEditEnterStuff<CViewIoInstructmentsBase> m_GuiET_toMaxTime;
  CEditEnterStuff<CViewIoInstructmentsBase> m_GuiET_initVel;
  CEditEnterStuff<CViewIoInstructmentsBase> m_GuiET_maxVel;

  CButton m_GuiBT_AddDls;
  CButton m_GuiBT_DeleteCurDls;

  CButton m_GuiCK_OpenFeedback;
  CButton m_GuiCK_WithTraningTime;

  DECLARE_WND_CLASS(_T ("CViewIoInstructmentsBase"))

  BEGIN_MSG_MAP(thisClass)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    COMMAND_HANDLER2 (m_GuiBT_AddDls, BN_CLICKED, OnButtonAddDls)
    // COMMAND_HANDLER2 (m_GuiBT_DeleteCurDls, BN_CLICKED, OnButtonDelDls) 
    //COMMAND_HANDLER2 (m_GuiCK_Chorus, BN_CLICKED, OnSetEffectCheck)
    //COMMAND_HANDLER2 (m_GuiCK_Reverb, BN_CLICKED, OnSetEffectCheck)
    //COMMAND_HANDLER2 (m_GuiCK_Delay, BN_CLICKED, OnSetEffectCheck)
    COMMAND_HANDLER2 (m_GuiCB_DlsCollection, CBN_SELCHANGE, OnSelDLSCollect)
    COMMAND_HANDLER2 (m_GuiCB_Instructments, CBN_SELCHANGE, OnSelInstrucetments)
    CHAIN_MSG_MAP(baseClass2)
  END_MSG_MAP()

  CViewIoInstructmentsBase (void) {
     m_FixedVel = NULL;
  }
 
  void OnCommandEditEnter (PMSG pMsg) {
    TCHAR buf[MAX_PATH];
    if (pMsg->hwnd == m_GuiET_FixedVeloctity) {  
      m_GuiET_FixedVeloctity.GetWindowText (buf, sizeof (buf)/sizeof(buf[0]));
      m_IoData.FixedVel = _ttoi ( (buf)) & 0x7F;
      _stprintf ( buf, _T ("%d"), m_IoData.FixedVel);
      m_GuiET_FixedVeloctity.SetWindowText ( buf);
    } else if (pMsg->hwnd == m_GuiET_OffsetNote) {  
      m_GuiET_OffsetNote.GetWindowText (buf, sizeof (buf)/sizeof(buf[0]));
      m_IoData.OffsetNote = _ttoi ( (buf)) & 0xFF;
      _stprintf ( buf, _T ("%d"), m_IoData.OffsetNote);
      m_GuiET_OffsetNote.SetWindowText ( buf);
      m_DirectMusic->ClearNoise ();
    }
  }

  void SetAttach (CDirectMusic2 *cmusic) {
    m_DirectMusic = cmusic;
  }
  void DoPaint (CDCHandle dc) {
    CRect t;
    GetClientRect (t);
    t.OffsetRect ( m_ptOffset);
    dc.FillSolidRect (& t, background_color);
  }

  void FeedBack_enable (BOOL enable) {
    if (enable == FALSE)  {
     m_GuiET_TraningTime.EnableWindow (FALSE);
     m_GuiET_toMaxTime.EnableWindow (FALSE);
     m_GuiET_initVel.EnableWindow (FALSE);
     m_GuiET_maxVel.EnableWindow (FALSE);
     m_GuiCK_WithTraningTime.EnableWindow (FALSE);
    } else  {
     m_GuiET_TraningTime.EnableWindow (TRUE);
     m_GuiET_toMaxTime.EnableWindow (TRUE);
     m_GuiET_initVel.EnableWindow (TRUE);
     m_GuiET_maxVel.EnableWindow (TRUE);
     m_GuiCK_WithTraningTime.EnableWindow (TRUE);
    }
  }

  LRESULT OnButtonAddDls(WORD code, WORD /*wID*/, HWND /*hWindow*/, BOOL& /*bHandled*/) {
    CFileDialogFilter strFilter(_T("dls file (*.dls;)")
                                _T("|             *.dls;||") );
    CFileDialog fd( TRUE, NULL, NULL, 0, strFilter, this->m_hWnd );
    if ( fd.DoModal () == IDOK)  {
      std::vector <CString> *t;
      m_DirectMusic->Get_vecCollectionDLS (t);
      t->push_back (fd.m_szFileName);
      m_GuiCB_DlsCollection.InsertString (-1, fd.m_szFileName);
    }
    return  0;
  }
  LRESULT OnSelDLSCollect(WORD code, WORD /*wID*/, HWND /*hWindow*/, BOOL& /*bHandled*/) {

    CString str;
    int sig = m_DirectMusic->AdjustCollectionIndex ( m_GuiCB_DlsCollection.GetCurSel ());
    if (sig != 0)  {
      AtlMessageBox (NULL, _T ("Current DLS Err0r, The reference to the object will be removed"), _T ("CViewIoInstructments"), MB_ICONERROR);
      int id = m_GuiCB_DlsCollection.GetCurSel ();
      std::vector<CString> *t;
      m_DirectMusic->Get_vecCollectionDLS (t);
      t->erase (t->begin() + id);
      // del current item. 
      m_GuiCB_DlsCollection.DeleteString ( m_GuiCB_DlsCollection.GetCurSel ());
    }
    std::vector<CString> *s;
    m_DirectMusic->Get_vecInstrInDLS (s);
    AppendString (*s, m_GuiCB_Instructments);
    m_GuiCB_Instructments.Clear ();
    m_GuiCB_Instructments.SetCurSel (0);

    return  0;
  }
  LRESULT OnButtonDelDls(WORD code, WORD /*wID*/, HWND /*hWindow*/, BOOL& /*bHandled*/) {
    return  0;
  }
  LRESULT OnSelInstrucetments(WORD code, WORD /*wID*/, HWND /*hWindow*/, BOOL& /*bHandled*/) {
    m_DirectMusic->AdjustInstructmentsIndex ( m_GuiCB_Instructments.GetCurSel ());
    return  0;
  }


  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    
    SetClassLongPtr (m_hWnd, GCLP_HBRBACKGROUND, (LONG_PTR) GetStockObject (NULL_BRUSH)) ;
    HINSTANCE hInst = _Module.GetModuleInstance ();
    HWND hWindow = m_hWnd;

    HFONT hFont = CreateFont(-11, 0, 0, 0, 400, FALSE, FALSE, FALSE, 1, 400, 0, 0, 0, _T ("MS Shell Dlg"));
    
    HWND T = CreateWindowEx (0, WC_STATIC, _T ("Instructment"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 15, 55, 63, 16, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("FixedVelocity"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 15, 83, 63, 16, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("&& 0x7F"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 143, 83, 39, 16, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("DLS collection"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 15, 24, 68, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);  
    T = CreateWindowEx (0, WC_STATIC, _T ("OffsetNote"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 15, 114, 53, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("&& 0xFF"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 143, 114, 39, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("init_vel"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 186, 193, 35, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("ms"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 147, 193, 15, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("ms"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 147, 224, 15, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("max_vel"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 186, 223, 41, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("tomax_time"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 15, 223, 62, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("training_time"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 15, 193, 66, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (WS_EX_TRANSPARENT, WC_BUTTON, _T ("Common Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 6, 5, 315, 244, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    
    // Button Group. 
    m_GuiBT_AddDls = CreateWindowEx (0, WC_BUTTON, _T ("add dls"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 186, 78, 123, 23, hWindow, (HMENU)-1, hInst, 0);
    m_GuiBT_AddDls.SetFont (hFont, FALSE);
    m_GuiBT_DeleteCurDls = CreateWindowEx (0, WC_BUTTON, _T ("delcur dls"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 186, 109, 123, 23, hWindow, (HMENU)-1, hInst, 0);
    m_GuiBT_DeleteCurDls.SetFont (hFont, FALSE);
    
    // Combox Group .
    m_GuiCB_DlsCollection = CreateWindowEx (0, WC_COMBOBOX, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_SORT, 87, 23, 222, 197, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCB_DlsCollection.SetFont (hFont, FALSE);
    m_GuiCB_Instructments = CreateWindowEx (0, WC_COMBOBOX, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_SORT, 87, 52, 222, 197, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCB_Instructments.SetFont (hFont, FALSE);

    // CheckBOx Group. 
    m_GuiCK_OpenFeedback = CreateWindowEx (0, WC_BUTTON, _T ("Open FeedBack Settings"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 15, 143, 143, 13, hWindow, (HMENU)-1, hInst, 0); 
    m_GuiCK_OpenFeedback.SetFont (hFont, FALSE);
    m_GuiCK_WithTraningTime = CreateWindowEx (0, WC_BUTTON, _T ("With Traning time"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 15, 166, 107, 13, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCK_WithTraningTime.SetFont (hFont, FALSE);
    
    // Edit Group .
    m_GuiET_FixedVeloctity.m_hWnd = CreateWindowEx (WS_EX_STATICEDGE, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 87, 80, 53, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_FixedVeloctity.SetFont (hFont, FALSE);
    m_GuiET_OffsetNote.m_hWnd = CreateWindowEx (WS_EX_STATICEDGE, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 87, 111, 53, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_OffsetNote.SetFont (hFont, FALSE);
    m_GuiET_TraningTime.m_hWnd = CreateWindowEx (WS_EX_STATICEDGE, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 254, 189, 53, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_TraningTime.SetFont (hFont, FALSE);
    m_GuiET_toMaxTime.m_hWnd = CreateWindowEx (WS_EX_STATICEDGE, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 254, 219, 53, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_toMaxTime.SetFont (hFont, FALSE);
    m_GuiET_initVel.m_hWnd = CreateWindowEx (WS_EX_STATICEDGE, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 87, 192, 53, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_initVel.SetFont (hFont, FALSE);
    m_GuiET_maxVel.m_hWnd = CreateWindowEx (WS_EX_STATICEDGE, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 87, 221, 53, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_maxVel.SetFont (hFont, FALSE);
    
    // Set init status 
    m_GuiCB_DlsCollection.InsertString (-1, _T ("default dls"));
    m_GuiCB_DlsCollection.SetCurSel (0);

    std::vector <CString> *str;
    std::vector <CString>::iterator iter;
    m_DirectMusic->Get_vecInstrInDLS (str);

    for (iter = str->begin (); iter != str->end (); iter++)
      m_GuiCB_Instructments.InsertString (-1, *iter);
    m_GuiCB_Instructments.SetCurSel (0);

    // Set Hook Edit. 
    m_GuiET_FixedVeloctity.SetAttach (this, & CViewIoInstructmentsBase::OnCommandEditEnter);
    m_GuiET_OffsetNote.SetAttach (this, & CViewIoInstructmentsBase::OnCommandEditEnter);
    m_GuiET_TraningTime.SetAttach (this, & CViewIoInstructmentsBase::OnCommandEditEnter);
    m_GuiET_toMaxTime.SetAttach (this, & CViewIoInstructmentsBase::OnCommandEditEnter);
    m_GuiET_initVel.SetAttach (this, & CViewIoInstructmentsBase::OnCommandEditEnter);
    m_GuiET_maxVel.SetAttach (this, & CViewIoInstructmentsBase::OnCommandEditEnter);

    _Module.GetMessageLoop ()->AddMessageFilter (& m_GuiET_FixedVeloctity);
    _Module.GetMessageLoop ()->AddMessageFilter (& m_GuiET_OffsetNote);
    _Module.GetMessageLoop ()->AddMessageFilter (& m_GuiET_TraningTime);
    _Module.GetMessageLoop ()->AddMessageFilter (& m_GuiET_toMaxTime);
    _Module.GetMessageLoop ()->AddMessageFilter (& m_GuiET_initVel);
    _Module.GetMessageLoop ()->AddMessageFilter (& m_GuiET_maxVel);

    m_GuiET_FixedVeloctity.SetWindowText ( GetIntBuf<BYTE> (m_IoData.FixedVel));
    m_GuiET_OffsetNote.SetWindowText ( GetIntBuf<BYTE> (m_IoData.OffsetNote));
    m_GuiET_TraningTime.SetWindowText ( GetIntBuf<DWORD> (m_IoData.TraningTime));
    m_GuiET_toMaxTime.SetWindowText ( GetIntBuf<DWORD> (m_IoData.toMaxTime));
    m_GuiET_initVel.SetWindowText ( GetIntBuf<DWORD> (m_IoData.initVel));
    m_GuiET_maxVel.SetWindowText ( GetIntBuf<DWORD> (m_IoData.maxVel));

    FeedBack_enable (FALSE);

    m_GuiCK_OpenFeedback.EnableWindow (FALSE);
    return 0;
  }
};

# endif 