# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  XXX: bad code to fix.
# if defined (__INL_PANE_ENABLE)

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewDS_EffectsDistortionBaseTraits;

// # include "CDirectDraw.hxx"
class CViewDS_EffectsDistortionBase:public CWindowImpl<CViewDS_EffectsDistortionBase,CWindow,CViewDS_EffectsDistortionBaseTraits>,
    public CScrollImpl<CViewDS_EffectsDistortionBase>
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 422;
  static  const  int height = 202;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewDS_EffectsDistortionBase,CWindow,CViewDS_EffectsDistortionBaseTraits> baseClass;
  typedef    CScrollImpl<CViewDS_EffectsDistortionBase> baseClass2;
  typedef CViewDS_EffectsDistortionBase  thisClass;
  
  // attach DirectMusic Object.
  CDirectMusic2 *m_DirectMusic;  

  // ctrl .
  CEdit m_GuiET_Gain;
  CEdit m_GuiET_Edge;
  CEdit m_GuiET_EqCenterFreq;
  CEdit m_GuiET_EqBandWidth;
  CEdit m_GuiET_PreLowpassCutoff;
  
  CTrackBarCtrl m_GuiTK_Gain;
  CTrackBarCtrl m_GuiTK_Edge;
  CTrackBarCtrl m_GuiTK_EqCenterFreq;
  CTrackBarCtrl m_GuiTK_EqBandWidth;
  CTrackBarCtrl m_GuiTK_PreLowpassCutoff;

  CButton m_GuiCK_Enable;

  DSFXDistortion m_DistortionParam;

  CViewDS_EffectsDistortionBase (void)  {
    m_DistortionParam.fGain = -18.0;
    m_DistortionParam.fEdge = 15.0;
    m_DistortionParam.fPostEQCenterFrequency = 2400.0;
    m_DistortionParam.fPostEQBandwidth = 2400.0;
    m_DistortionParam.fPreLowpassCutoff = 8000.0;

    m_DirectMusic = NULL;
  }

  DECLARE_WND_CLASS(_T ("CViewDS_EffectsDistortionBase"))

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
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_DISTORTION, FALSE);
    } else  {
      // set  effects .
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_DISTORTION, TRUE);
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_DISTORTION, (CDirectMusic2::DSFX_desc *)& m_DistortionParam);
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
      if  ( hWindow == m_GuiTK_Gain)  {
        int pos = m_GuiTK_Gain.GetPos ();
        float c = ((float)pos) / 600.0f;
        float b = c * (DSFXDISTORTION_GAIN_MAX - DSFXDISTORTION_GAIN_MIN);
        m_DistortionParam.fGain =b+ DSFXDISTORTION_GAIN_MIN;
        if  ( m_DistortionParam.fGain > DSFXDISTORTION_GAIN_MAX)
          m_DistortionParam.fGain = DSFXDISTORTION_GAIN_MAX;
        if  ( m_DistortionParam.fGain < DSFXDISTORTION_GAIN_MIN)
          m_DistortionParam.fGain = DSFXDISTORTION_GAIN_MIN;
        s.Format (_T ("%f"), m_DistortionParam.fGain);
        m_GuiET_Gain.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Edge)  {
        int pos = m_GuiTK_Edge.GetPos ();
        float c = ((float)pos) / 1000.0f;
        float b = c * (DSFXDISTORTION_EDGE_MAX - DSFXDISTORTION_EDGE_MIN);
        m_DistortionParam.fEdge =b+ DSFXDISTORTION_EDGE_MIN;
        if  ( m_DistortionParam.fEdge > DSFXDISTORTION_EDGE_MAX)
          m_DistortionParam.fEdge = DSFXDISTORTION_EDGE_MAX;
        if  ( m_DistortionParam.fEdge < DSFXDISTORTION_EDGE_MIN)
          m_DistortionParam.fEdge = DSFXDISTORTION_EDGE_MIN;
        s.Format (_T ("%f"), m_DistortionParam.fEdge);
        m_GuiET_Edge.SetWindowText (s);
      } else  if  ( hWindow == m_GuiTK_EqCenterFreq)  {
        int pos = m_GuiTK_EqCenterFreq.GetPos ();
        float c = ((float)(pos - 100.0)) / 7900;
        float b = c * (DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX - DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN);
        m_DistortionParam.fPostEQCenterFrequency =b+ DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN;
        if  ( m_DistortionParam.fPostEQCenterFrequency > DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX)
          m_DistortionParam.fPostEQCenterFrequency = DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX;
        if  ( m_DistortionParam.fPostEQCenterFrequency < DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN)
          m_DistortionParam.fPostEQCenterFrequency = DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN;
        s.Format (_T ("%f"), m_DistortionParam.fPostEQCenterFrequency);
        m_GuiET_EqCenterFreq.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_EqBandWidth)  {
        int pos = m_GuiTK_EqBandWidth.GetPos ();
        float c = ((float)(pos - 100.0)) / 7900;
        float b = c * (DSFXDISTORTION_POSTEQBANDWIDTH_MAX - DSFXDISTORTION_POSTEQBANDWIDTH_MIN);
        m_DistortionParam.fPostEQBandwidth =b+ DSFXDISTORTION_POSTEQBANDWIDTH_MIN;
        if  ( m_DistortionParam.fPostEQBandwidth > DSFXDISTORTION_POSTEQBANDWIDTH_MAX)
          m_DistortionParam.fPostEQBandwidth = DSFXDISTORTION_POSTEQBANDWIDTH_MAX;
        if  ( m_DistortionParam.fPostEQBandwidth < DSFXDISTORTION_POSTEQBANDWIDTH_MIN)
          m_DistortionParam.fPostEQBandwidth = DSFXDISTORTION_POSTEQBANDWIDTH_MIN;
        s.Format (_T ("%f"), m_DistortionParam.fPostEQBandwidth);
        m_GuiET_EqBandWidth.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_PreLowpassCutoff)  {
        int pos = m_GuiTK_PreLowpassCutoff.GetPos ();
        float c = ((float)(pos - 100.0)) / 7900;
        float b = c * (DSFXDISTORTION_PRELOWPASSCUTOFF_MAX - DSFXDISTORTION_PRELOWPASSCUTOFF_MIN);
        m_DistortionParam.fPreLowpassCutoff =b+ DSFXDISTORTION_PRELOWPASSCUTOFF_MIN;
        if  ( m_DistortionParam.fPreLowpassCutoff > DSFXDISTORTION_PRELOWPASSCUTOFF_MAX)
          m_DistortionParam.fPreLowpassCutoff = DSFXDISTORTION_PRELOWPASSCUTOFF_MAX;
        if  ( m_DistortionParam.fPreLowpassCutoff < DSFXDISTORTION_PRELOWPASSCUTOFF_MIN)
          m_DistortionParam.fPreLowpassCutoff = DSFXDISTORTION_PRELOWPASSCUTOFF_MIN;
        s.Format (_T ("%f"), m_DistortionParam.fPreLowpassCutoff);
        m_GuiET_PreLowpassCutoff.SetWindowText (s);
      } else  {
        bHandled = FALSE;
        return 0;
      }
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_DISTORTION, (CDirectMusic2::DSFX_desc *)& m_DistortionParam);
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
    HWND T = CreateWindowEx (0, WC_STATIC, _T ("Gain(dB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 54, 42, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (WS_EX_TRANSPARENT, WC_BUTTON, _T ("IDirectSound8 Effects - Distortion Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 8, 5, 405, 187, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Edge(%)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 80, 41, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("PostEQCenterFrequency"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 106, 119, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T =  CreateWindowEx (0, WC_STATIC, _T ("PostEQBandwidth(hz)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 132, 105, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("PreLowpassCutoff(hz)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 163, 105, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    m_GuiCK_Enable = CreateWindowEx (0, WC_BUTTON, _T ("Enable Distortion Effects"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 18, 24, 140, 13, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (m_GuiCK_Enable, WM_SETFONT, (WPARAM)hFont, FALSE);

    // CEdit Group.
    m_GuiET_Gain = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 71, 52, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Gain.SetFont (hFont, FALSE);
    m_GuiET_Edge = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 71, 78, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Edge.SetFont (hFont, FALSE);
    m_GuiET_EqCenterFreq = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 153, 106, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_EqCenterFreq.SetFont (hFont, FALSE);
    m_GuiET_EqBandWidth = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 137, 130, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_EqBandWidth.SetFont (hFont, FALSE);
    m_GuiET_PreLowpassCutoff = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 132, 161, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_PreLowpassCutoff.SetFont (hFont, FALSE);

    // CSlider Ctrl.
    m_GuiTK_Gain = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 147, 50, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Gain.SetFont (hFont, FALSE);
    m_GuiTK_Edge = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 147, 75, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Edge.SetFont (hFont, FALSE);
    m_GuiTK_EqCenterFreq = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 228, 102, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_EqCenterFreq.SetFont (hFont, FALSE);
    m_GuiTK_EqBandWidth = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 212, 128, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_EqBandWidth.SetFont (hFont, FALSE);
    m_GuiTK_PreLowpassCutoff = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 207, 158, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_PreLowpassCutoff.SetFont (hFont, FALSE);

    CString str[5];
    str[0].Format ( _T ("%f"), m_DistortionParam.fGain);
    str[1].Format ( _T ("%f"), m_DistortionParam.fEdge);
    str[2].Format ( _T ("%f"), m_DistortionParam.fPostEQCenterFrequency);
    str[3].Format ( _T ("%f"), m_DistortionParam.fPostEQBandwidth);
    str[4].Format ( _T ("%f"), m_DistortionParam.fPreLowpassCutoff);

    m_GuiTK_Gain.SetRange (0, 600);
    m_GuiTK_Gain.SetPos ( (int)( ( m_DistortionParam.fGain - DSFXDISTORTION_GAIN_MIN) / (DSFXDISTORTION_GAIN_MAX - DSFXDISTORTION_GAIN_MIN) * 600.0 ));
    m_GuiTK_Edge.SetRange (0, 1000);
    m_GuiTK_Edge.SetPos ( (int)( ( m_DistortionParam.fEdge - DSFXDISTORTION_EDGE_MIN) / (DSFXDISTORTION_EDGE_MAX - DSFXDISTORTION_EDGE_MIN) * 1000.0 ));
    m_GuiTK_EqCenterFreq.SetRange (100, 8000);
    m_GuiTK_EqCenterFreq.SetPos ( (int)( ( m_DistortionParam.fPostEQCenterFrequency - DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN) / (DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX - DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN) * 7900.0 ));
    m_GuiTK_EqBandWidth.SetRange (100, 8000);
    m_GuiTK_EqBandWidth.SetPos ( (int)( ( m_DistortionParam.fPostEQBandwidth - DSFXDISTORTION_POSTEQBANDWIDTH_MIN) / (DSFXDISTORTION_POSTEQBANDWIDTH_MAX - DSFXDISTORTION_POSTEQBANDWIDTH_MIN) * 7900.0 ));
    m_GuiTK_PreLowpassCutoff.SetRange (100, 8000);
    m_GuiTK_PreLowpassCutoff.SetPos ( (int)( ( m_DistortionParam.fPreLowpassCutoff - DSFXDISTORTION_PRELOWPASSCUTOFF_MIN) / (DSFXDISTORTION_PRELOWPASSCUTOFF_MAX - DSFXDISTORTION_PRELOWPASSCUTOFF_MIN) * 7900.0 ));

    m_GuiET_Gain.SetWindowText (str[0]);
    m_GuiET_Edge.SetWindowText (str[1]);
    m_GuiET_EqCenterFreq.SetWindowText (str[2]);
    m_GuiET_EqBandWidth.SetWindowText (str[3]);
    m_GuiET_PreLowpassCutoff.SetWindowText (str[4]);

    return 0;
  }
};

# endif // !defined(_CVIEWDS_EFFECTSDISTORTION_INCLUED_HEADER_CXX_