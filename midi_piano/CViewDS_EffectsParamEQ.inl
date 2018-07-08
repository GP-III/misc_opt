# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if defined (__INL_PANE_ENABLE)

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewDS_EffectsParamEQBaseTraits;

// # include "CDirectDraw.hxx"
class CViewDS_EffectsParamEQBase:public CWindowImpl<CViewDS_EffectsParamEQBase,CWindow,CViewDS_EffectsParamEQBaseTraits>,
    public CScrollImpl<CViewDS_EffectsParamEQBase>
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 360;
  static  const  int height = 133;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewDS_EffectsParamEQBase,CWindow,CViewDS_EffectsParamEQBaseTraits> baseClass;
  typedef    CScrollImpl<CViewDS_EffectsParamEQBase> baseClass2;
  typedef CViewDS_EffectsParamEQBase  thisClass;
  
  // attach DirectMusic Object.
  CDirectMusic2 *m_DirectMusic;  

  // ctrl .
  CEdit m_GuiET_Center;
  CEdit m_GuiET_BandWidth;
  CEdit m_GuiET_Gain;

  CTrackBarCtrl m_GuiTK_Center;
  CTrackBarCtrl m_GuiTK_BandWidth;
  CTrackBarCtrl m_GuiTK_Gain;

  CButton m_GuiCK_Enable;

  DSFXParamEq m_EQParam;

  CViewDS_EffectsParamEQBase (void)  {
    m_EQParam.fCenter = 8000.0;
    m_EQParam.fBandwidth = 12.0;
    m_EQParam.fGain = 0.0;

    m_DirectMusic = NULL;
  }

  DECLARE_WND_CLASS(_T ("CViewDS_EffectsParamEQBase"))

  BEGIN_MSG_MAP(thisClass)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_HSCROLL, OnSlider)
    COMMAND_HANDLER2 (m_GuiCK_Enable, BN_CLICKED, OnSetEnable)
    CHAIN_MSG_MAP(baseClass2)
  END_MSG_MAP()

  LRESULT OnSetEnable (WORD code, WORD /*wID*/, HWND hWindow, BOOL& /*bHandled*/) {
    int status = :: SendMessage (hWindow, BM_GETCHECK, 0, 0);
    if (status == BST_UNCHECKED)   {
      // cancel effects.
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_PARAMEQ, FALSE);
    } else  {
      // set  effects .
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_PARAMEQ, TRUE);
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_PARAMEQ, (CDirectMusic2::DSFX_desc *)& m_EQParam);
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
      if  ( hWindow == m_GuiTK_Center)  {
        int pos = m_GuiTK_Center.GetPos ();
        m_EQParam.fCenter = (float) pos;
        if  ( m_EQParam.fCenter > DSFXPARAMEQ_CENTER_MAX)
          m_EQParam.fCenter = DSFXPARAMEQ_CENTER_MAX;
        if  ( m_EQParam.fCenter < DSFXPARAMEQ_CENTER_MIN)
          m_EQParam.fCenter = DSFXPARAMEQ_CENTER_MIN;
        s.Format (_T ("%f"), m_EQParam.fCenter);
        m_GuiET_Center.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_BandWidth)  {
        int pos = m_GuiTK_BandWidth.GetPos ();
        float c = ( ( ((float) pos) - 10.0f)/ 350.0f);
        float b = c * (DSFXPARAMEQ_BANDWIDTH_MAX - DSFXPARAMEQ_BANDWIDTH_MIN);
        m_EQParam.fBandwidth = b + DSFXPARAMEQ_BANDWIDTH_MIN;
        if  ( m_EQParam.fBandwidth > DSFXPARAMEQ_BANDWIDTH_MAX)
          m_EQParam.fBandwidth = DSFXPARAMEQ_BANDWIDTH_MAX;
        if  ( m_EQParam.fBandwidth < DSFXPARAMEQ_BANDWIDTH_MIN)
          m_EQParam.fBandwidth = DSFXPARAMEQ_BANDWIDTH_MIN;
        s.Format (_T ("%f"), m_EQParam.fBandwidth);
        m_GuiET_BandWidth.SetWindowText (s);
      } else  if  ( hWindow == m_GuiTK_Gain)  {
        int pos = m_GuiTK_Gain.GetPos ();
        float c = pos / 300.0f;
        float b = c * (DSFXPARAMEQ_GAIN_MAX - DSFXPARAMEQ_GAIN_MIN);
        m_EQParam.fGain = b + DSFXPARAMEQ_GAIN_MIN;
        if  ( m_EQParam.fBandwidth > DSFXPARAMEQ_GAIN_MAX)
          m_EQParam.fBandwidth = DSFXPARAMEQ_GAIN_MAX;
        if  ( m_EQParam.fBandwidth < DSFXPARAMEQ_GAIN_MIN)
          m_EQParam.fBandwidth = DSFXPARAMEQ_GAIN_MIN;
        s.Format (_T ("%f"), m_EQParam.fGain);
        m_GuiET_Gain.SetWindowText (s);
      } else  {
        bHandled = FALSE;
        return 0;
      }
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_PARAMEQ, (CDirectMusic2::DSFX_desc *)& m_EQParam);
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
    
    // static  Group.   
    HWND T = CreateWindowEx(0, WC_STATIC, _T ("Center Freq(hz)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 47, 74, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("BandWidth(hz)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 73, 72, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("Gain(db)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 96, 42, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, _T ("IDirectSound8 Effects - ParamEQ Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 8, 5, 345, 120, hWindow, (HMENU)-1, hInst, 0);
    SendMessage(T, WM_SETFONT, (WPARAM)hFont, FALSE);

    m_GuiCK_Enable = CreateWindowEx(0, WC_BUTTON, _T ("Enable ParamEQ Effects"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 18, 24, 141, 13, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (m_GuiCK_Enable, WM_SETFONT, (WPARAM)hFont, FALSE);

    // CEdit Group.
    m_GuiET_Center = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 96, 46, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Center.SetFont (hFont, FALSE);
    m_GuiET_BandWidth = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 96, 72, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_BandWidth.SetFont (hFont, FALSE);
    m_GuiET_Gain = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 96, 94, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Gain.SetFont (hFont, FALSE);

    // CSlider Ctrl.
    m_GuiTK_Center = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 42, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Center.SetFont (hFont, FALSE);
    m_GuiTK_BandWidth = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 68, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_BandWidth.SetFont (hFont, FALSE);
    m_GuiTK_Gain = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 91, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Gain.SetFont (hFont, FALSE);

    CString str[3];
    str[0].Format ( _T ("%f"), m_EQParam.fCenter);
    str[1].Format ( _T ("%f"), m_EQParam.fBandwidth);
    str[2].Format ( _T ("%f"), m_EQParam.fGain);

    m_GuiTK_Center.SetRange (80, 16000);
    m_GuiTK_Center.SetPos ( (int) m_EQParam.fCenter );
    m_GuiTK_BandWidth.SetRange (10, 360);
    m_GuiTK_BandWidth.SetPos ( (int) (m_EQParam.fBandwidth*10.0 ));
    m_GuiTK_Gain.SetRange (0, 300);
    m_GuiTK_Gain.SetPos ( (int)((m_EQParam.fGain - DSFXPARAMEQ_GAIN_MIN)/ (DSFXPARAMEQ_GAIN_MAX - DSFXPARAMEQ_GAIN_MIN) * 300.0 ));

    m_GuiET_Center.SetWindowText (str[0]);
    m_GuiET_BandWidth.SetWindowText (str[1]);
    m_GuiET_Gain.SetWindowText (str[2]);
    return 0;
  }
};

# endif // !defined(_CVIEWDS_EffectsReverb_INCLUED_HEADER_CXX_