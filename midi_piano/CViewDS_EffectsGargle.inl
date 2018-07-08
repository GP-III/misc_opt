# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  XXX: bad code to fix.
# if defined (__INL_PANE_ENABLE)

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewDS_EffectsGargleBaseTraits;

// # include "CDirectDraw.hxx"
class CViewDS_EffectsGargleBase:public CWindowImpl<CViewDS_EffectsGargleBase,CWindow,CViewDS_EffectsGargleBaseTraits>,
    public CScrollImpl<CViewDS_EffectsGargleBase>
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 348;
  static  const  int height = 108;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewDS_EffectsGargleBase,CWindow,CViewDS_EffectsGargleBaseTraits> baseClass;
  typedef    CScrollImpl<CViewDS_EffectsGargleBase> baseClass2;
  typedef CViewDS_EffectsGargleBase  thisClass;
  
  // attach DirectMusic Object.
  CDirectMusic2 *m_DirectMusic;  

  // ctrl .
  CEdit m_GuiET_Rate;
  CTrackBarCtrl m_GuiTK_Rate;
  CComboBox m_GuiCB_WaveForm;
  CButton m_GuiCK_Enable;

  DSFXGargle m_GargleParam;

  CViewDS_EffectsGargleBase (void)  {
    m_GargleParam.dwRateHz = 20;
    m_GargleParam.dwWaveShape = DSFXGARGLE_WAVE_TRIANGLE; 

    m_DirectMusic = NULL;
  }
  // DSFXGARGLE_RATEHZ_MAX

  DECLARE_WND_CLASS(_T ("CViewDS_EffectsGargleBase"))

  BEGIN_MSG_MAP(thisClass)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_HSCROLL, OnSlider)
    COMMAND_HANDLER2 (m_GuiCK_Enable, BN_CLICKED, OnSetEnable)
    COMMAND_HANDLER2 (m_GuiCB_WaveForm, CBN_SELCHANGE, OnSelWave)
    CHAIN_MSG_MAP(baseClass2)
  END_MSG_MAP()

  LRESULT OnSelWave (WORD code, WORD /*wID*/, HWND hWindow, BOOL& /*bHandled*/) {
    if ( 0 ==  m_GuiCB_WaveForm.GetCurSel ())
      m_GargleParam.dwWaveShape = DSFXGARGLE_WAVE_TRIANGLE;
    else 
      m_GargleParam.dwWaveShape = DSFXGARGLE_WAVE_SQUARE;
    if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_GARGLE, (CDirectMusic2::DSFX_desc *)& m_GargleParam);
    return 0;
  }

  LRESULT OnSetEnable (WORD code, WORD /*wID*/, HWND hWindow, BOOL& /*bHandled*/) {
    int status = :: SendMessage (hWindow, BM_GETCHECK, 0, 0);
    if (status == BST_UNCHECKED)   {
      // cancel effects.
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_GARGLE, FALSE);
    } else  {
      // set  effects .
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_GARGLE, TRUE);
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_GARGLE, (CDirectMusic2::DSFX_desc *)& m_GargleParam);
    }
    return 0;
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

  LRESULT OnSlider(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    int code = LOWORD (wParam);
    if (code == TB_ENDTRACK || code == TB_THUMBTRACK)  {
      CString s;
      BOOL bSet = TRUE;
      HWND hWindow = (HWND) lParam;
      if  ( hWindow == m_GuiTK_Rate)  {
        m_GargleParam.dwRateHz = m_GuiTK_Rate.GetPos ();
        if  ( m_GargleParam.dwRateHz > DSFXGARGLE_RATEHZ_MAX)
          m_GargleParam.dwRateHz = DSFXGARGLE_RATEHZ_MAX;
        if  ( m_GargleParam.dwRateHz < DSFXGARGLE_RATEHZ_MIN)
          m_GargleParam.dwRateHz = DSFXGARGLE_RATEHZ_MIN;
        s.Format (_T ("%d"), m_GargleParam.dwRateHz);
        m_GuiET_Rate.SetWindowText (s);
      }  else  {
        bHandled = FALSE;
        return 0;
      }
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_GARGLE, (CDirectMusic2::DSFX_desc *)& m_GargleParam);
      return 0;
    }
    bHandled = FALSE;
    return 0;
  }

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    
    SetClassLongPtr (m_hWnd, GCLP_HBRBACKGROUND, (LONG_PTR) GetStockObject (NULL_BRUSH)) ;
    HINSTANCE hInst = _Module.GetModuleInstance ();
    HWND hWindow = m_hWnd;

    HFONT hFont = CreateFont(-11, 0, 0, 0, 400, FALSE, FALSE, FALSE, 1, 400, 0, 0, 0, _T ("MS Shell Dlg"));
    
    //  static Group.
    HWND T = CreateWindowEx (0, WC_STATIC, _T ("Rate(hz)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 47, 42, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("WaveForm"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 73, 54, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (WS_EX_TRANSPARENT, WC_BUTTON, _T ("IDirectSound8 Effects - Gargle Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 8, 5, 327, 98, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);

    // Edit Group 
    m_GuiET_Rate  = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 77, 46, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Rate.SetFont (hFont, FALSE);
    
    // Check Group 
    m_GuiCK_Enable = CreateWindowEx (0, WC_BUTTON, _T ("Enable Gargle Effects"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 18, 24, 128, 13, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCK_Enable.SetFont (hFont, FALSE);

    // CSlider 
    m_GuiTK_Rate = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 147, 42, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Rate.SetFont (hFont, FALSE);

    // Combo Group.
    m_GuiCB_WaveForm = CreateWindowEx (0, WC_COMBOBOX, _T (""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 77, 70, 243, 21, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCB_WaveForm.SetFont (hFont, FALSE);

    // CEdit Group.
    CString str;
    str.Format ( _T ("%d"), m_GargleParam.dwRateHz);

    m_GuiTK_Rate.SetRange (1, 1000);
    m_GuiCB_WaveForm.InsertString ( -1, _T ("Triangular wave"));
    m_GuiCB_WaveForm.InsertString ( -1, _T ("Square wave"));
    m_GuiCB_WaveForm.SetCurSel (0);
    m_GuiET_Rate.SetWindowText (str);
    return 0;
  }
};

# endif // !defined(_CVIEWDS_EFFECTSGARGLE_INCLUED_HEADER_CXX_