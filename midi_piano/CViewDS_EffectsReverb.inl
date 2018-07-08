# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  XXX: bad code to fix.
# if defined (__INL_PANE_ENABLE)

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewDS_EffectsReverbBaseTraits;

// # include "CDirectDraw.hxx"
class CViewDS_EffectsReverbBase:public CWindowImpl<CViewDS_EffectsReverbBase,CWindow,CViewDS_EffectsReverbBaseTraits>,
    public CScrollImpl<CViewDS_EffectsReverbBase>
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 398;
  static  const  int height = 160;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewDS_EffectsReverbBase,CWindow,CViewDS_EffectsReverbBaseTraits> baseClass;
  typedef    CScrollImpl<CViewDS_EffectsReverbBase> baseClass2;
  typedef CViewDS_EffectsReverbBase  thisClass;
  
  // attach DirectMusic Object.
  CDirectMusic2 *m_DirectMusic;  

  // ctrl .
  CEdit m_GuiET_InGain;
  CEdit m_GuiET_ReverbMix;
  CEdit m_GuiET_ReverbTime;
  CEdit m_GuiET_HighFreqRTRatio;
  
  CTrackBarCtrl m_GuiTK_InGain;
  CTrackBarCtrl m_GuiTK_ReverbMix;
  CTrackBarCtrl m_GuiTK_ReverbTime;
  CTrackBarCtrl m_GuiTK_HighFreqRTRatio;

  CButton m_GuiCK_Enable;

  DSFXWavesReverb m_ReverbParam;

  CViewDS_EffectsReverbBase (void)  {
    m_ReverbParam.fInGain = DSFX_WAVESREVERB_INGAIN_DEFAULT;
    m_ReverbParam.fReverbMix = DSFX_WAVESREVERB_REVERBMIX_DEFAULT;
    m_ReverbParam.fReverbTime =  DSFX_WAVESREVERB_REVERBTIME_DEFAULT;
    m_ReverbParam.fHighFreqRTRatio = DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;

    m_DirectMusic = NULL;
  }

  DECLARE_WND_CLASS(_T ("CViewDS_EffectsReverbBase"))

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
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB, FALSE);
    } else  {
      // set  effects .
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB, TRUE);
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB, (CDirectMusic2::DSFX_desc *)& m_ReverbParam);
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
      if  ( hWindow == m_GuiTK_InGain)  {
        int pos = m_GuiTK_InGain.GetPos ();
        float c = ((float)(pos)) / 960.0f;
        float b = c * (DSFX_WAVESREVERB_INGAIN_MAX - DSFX_WAVESREVERB_INGAIN_MIN);
        m_ReverbParam.fInGain = b + DSFX_WAVESREVERB_INGAIN_MIN;
        if  ( m_ReverbParam.fInGain > DSFX_WAVESREVERB_INGAIN_MAX)
          m_ReverbParam.fInGain = DSFX_WAVESREVERB_INGAIN_MAX;
        if  ( m_ReverbParam.fInGain < DSFX_WAVESREVERB_INGAIN_MIN)
          m_ReverbParam.fInGain = DSFX_WAVESREVERB_INGAIN_MIN;
        s.Format (_T ("%f"), m_ReverbParam.fInGain);
        m_GuiET_InGain.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_ReverbMix)  {
        int pos = m_GuiTK_ReverbMix.GetPos ();
        float c = ((float)(pos)) / 960.0f;
        float b = c * (DSFX_WAVESREVERB_REVERBMIX_MAX - DSFX_WAVESREVERB_REVERBMIX_MIN);
        m_ReverbParam.fReverbMix = b + DSFX_WAVESREVERB_REVERBMIX_MIN;
        if  ( m_ReverbParam.fReverbMix > DSFX_WAVESREVERB_REVERBMIX_MAX)
          m_ReverbParam.fReverbMix = DSFX_WAVESREVERB_REVERBMIX_MAX;
        if  ( m_ReverbParam.fReverbMix < DSFX_WAVESREVERB_REVERBMIX_MIN)
          m_ReverbParam.fReverbMix = DSFX_WAVESREVERB_REVERBMIX_MIN;
        s.Format (_T ("%f"), m_ReverbParam.fReverbMix);
        m_GuiET_ReverbMix.SetWindowText (s);
      } else  if  ( hWindow == m_GuiTK_ReverbTime)  {
        int pos = m_GuiTK_ReverbTime.GetPos ();
        float c = ((float)(pos - 1)) / 29999.0f;
        float b = c * (DSFX_WAVESREVERB_REVERBTIME_MAX - DSFX_WAVESREVERB_REVERBTIME_MIN);
        m_ReverbParam.fReverbTime = b + DSFX_WAVESREVERB_REVERBTIME_MIN;
        if  ( m_ReverbParam.fReverbTime > DSFX_WAVESREVERB_REVERBTIME_MAX)
          m_ReverbParam.fReverbTime = DSFX_WAVESREVERB_REVERBTIME_MAX;
        if  ( m_ReverbParam.fReverbTime < DSFX_WAVESREVERB_REVERBTIME_MIN)
          m_ReverbParam.fReverbTime = DSFX_WAVESREVERB_REVERBTIME_MIN;
        s.Format (_T ("%f"), m_ReverbParam.fReverbTime);
        m_GuiET_ReverbTime.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_HighFreqRTRatio)  {
        int pos = m_GuiTK_HighFreqRTRatio.GetPos ();
        float c = ((float)(pos - 1)) / 998.0f;
        float b = c * (DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX - DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN);
        m_ReverbParam.fHighFreqRTRatio = b + DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN;
        if  ( m_ReverbParam.fHighFreqRTRatio > DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX)
          m_ReverbParam.fHighFreqRTRatio = DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX;
        if  ( m_ReverbParam.fHighFreqRTRatio < DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN)
          m_ReverbParam.fHighFreqRTRatio = DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN;
        s.Format (_T ("%f"), m_ReverbParam.fHighFreqRTRatio);
        m_GuiET_HighFreqRTRatio.SetWindowText (s);
      } else  {
        bHandled = FALSE;
        return 0;
      }
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB, (CDirectMusic2::DSFX_desc *)& m_ReverbParam);
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
    HWND T = CreateWindowEx(0, WC_STATIC, _T ("In Gain(dB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 47, 54, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("Reverb mix(dB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 73, 74, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("Reverb time(ms)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 96, 78, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("HighFreq RT ratio (x:1)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 120, 108, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, _T ("IDirectSound8 Effects - Reverb Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 8, 5, 381, 148, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    m_GuiCK_Enable = CreateWindowEx(0, WC_BUTTON, _T ("Enable Reverb Effects"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 18, 24, 131, 13, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (m_GuiCK_Enable, WM_SETFONT, (WPARAM)hFont, FALSE);

    // CEdit Group.
    m_GuiET_InGain = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 80, 44, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_InGain.SetFont (hFont, FALSE); 
    m_GuiET_ReverbMix = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 102, 70, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_ReverbMix.SetFont (hFont, FALSE); 
    m_GuiET_ReverbTime = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 105, 94, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_ReverbTime.SetFont (hFont, FALSE); 
    m_GuiET_HighFreqRTRatio = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 134, 119, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_HighFreqRTRatio.SetFont (hFont, FALSE); 

    // CSlider Ctrl.
    m_GuiTK_InGain = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 155, 41, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_InGain.SetFont (hFont, FALSE);
    m_GuiTK_ReverbMix = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 67, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_ReverbMix.SetFont (hFont, FALSE);
    m_GuiTK_ReverbTime = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 183, 93, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_ReverbTime.SetFont (hFont, FALSE);
    m_GuiTK_HighFreqRTRatio = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 206, 115, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_HighFreqRTRatio.SetFont (hFont, FALSE);

    CString str[4];
    str[0].Format ( _T ("%f"), m_ReverbParam.fInGain);
    str[1].Format ( _T ("%f"), m_ReverbParam.fReverbMix);
    str[2].Format ( _T ("%f"), m_ReverbParam.fReverbTime);
    str[3].Format ( _T ("%f"), m_ReverbParam.fHighFreqRTRatio);

    m_GuiTK_InGain.SetRange (0, 960);
    m_GuiTK_InGain.SetPos ( (int)( (m_ReverbParam.fInGain + 96.0f)/ (- DSFX_WAVESREVERB_INGAIN_MIN) * 960.0f ));
    m_GuiTK_ReverbMix.SetRange (0, 960);
    m_GuiTK_ReverbMix.SetPos ( (int)((m_ReverbParam.fReverbMix + 96.0f)/ (- DSFX_WAVESREVERB_REVERBMIX_MIN) * 960.0f ));
    m_GuiTK_ReverbTime.SetRange (1, 30000);
    m_GuiTK_ReverbTime.SetPos ( (int)((m_ReverbParam.fReverbTime)/ (DSFX_WAVESREVERB_REVERBTIME_MAX) * 29999.0f ));
    m_GuiTK_HighFreqRTRatio.SetRange (1, 999);
    m_GuiTK_HighFreqRTRatio.SetPos ( (int)((m_ReverbParam.fHighFreqRTRatio)/ (DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX) * 998.0f ));

    m_GuiET_InGain.SetWindowText (str[0]);
    m_GuiET_ReverbMix.SetWindowText (str[1]);
    m_GuiET_ReverbTime.SetWindowText (str[2]);
    m_GuiET_HighFreqRTRatio.SetWindowText (str[3]);
    return 0;
  }
};

# endif // !defined(_CVIEWDS_EffectsReverb_INCLUED_HEADER_CXX_