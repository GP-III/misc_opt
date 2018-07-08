# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  XXX: bad code to fix.
# if defined (__INL_PANE_ENABLE)

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewDS_EffectsCompressorBaseTraits;

// # include "CDirectDraw.hxx"
class CViewDS_EffectsCompressorBase:public CWindowImpl<CViewDS_EffectsCompressorBase,CWindow,CViewDS_EffectsCompressorBaseTraits>,
    public CScrollImpl<CViewDS_EffectsCompressorBase>
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 362;
  static  const  int height = 242;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewDS_EffectsCompressorBase,CWindow,CViewDS_EffectsCompressorBaseTraits> baseClass;
  typedef    CScrollImpl<CViewDS_EffectsCompressorBase> baseClass2;
  typedef CViewDS_EffectsCompressorBase  thisClass;
  
  // attach DirectMusic Object.
  CDirectMusic2 *m_DirectMusic;  

  // ctrl .
  CEdit m_GuiET_Gain;
  CEdit m_GuiET_Attack;
  CEdit m_GuiET_Release;
  CEdit m_GuiET_Threshold;
  CEdit m_GuiET_Ratio;
  CEdit m_GuiET_Predelay;

  CTrackBarCtrl m_GuiTK_Gain;
  CTrackBarCtrl m_GuiTK_Attack;
  CTrackBarCtrl m_GuiTK_Release;
  CTrackBarCtrl m_GuiTK_Thresold;
  CTrackBarCtrl m_GuiTK_Ratio;
  CTrackBarCtrl m_GuiTK_Predelay;

  CButton m_GuiCK_Enable;

  DSFXCompressor m_CompressorParam;

  CViewDS_EffectsCompressorBase (void)  {
    m_CompressorParam.fGain = 0.0;
    m_CompressorParam.fAttack = 10.0;
    m_CompressorParam.fRelease = 200.0;
    m_CompressorParam.fThreshold = -20;
    m_CompressorParam.fRatio = 3;
    m_CompressorParam.fPredelay = 4;
  }
  // DSFXCOMPRESSOR_RATEHZ_MAX

  DECLARE_WND_CLASS(_T ("CViewDS_EffectsCompressorBase"))

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
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_COMPRESSOR, FALSE);
    } else  {
      // set  effects .
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_COMPRESSOR, TRUE);
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_COMPRESSOR, (CDirectMusic2::DSFX_desc *)& m_CompressorParam);
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
        float c = ((float)pos) / 1200.0f;
        float b = c * (DSFXCOMPRESSOR_GAIN_MAX - DSFXCOMPRESSOR_GAIN_MIN);
        m_CompressorParam.fGain =b+ DSFXCOMPRESSOR_GAIN_MIN;
        if  ( m_CompressorParam.fGain > DSFXCOMPRESSOR_GAIN_MAX)
          m_CompressorParam.fGain = DSFXCOMPRESSOR_GAIN_MAX;
        if  ( m_CompressorParam.fGain < DSFXCOMPRESSOR_GAIN_MIN)
          m_CompressorParam.fGain = DSFXCOMPRESSOR_GAIN_MIN;
        s.Format (_T ("%f"), m_CompressorParam.fGain);
        m_GuiET_Gain.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Attack)  {
        int pos = m_GuiTK_Attack.GetPos ();
        float c = ((float)pos) / 4999.0f;
        float b = c * (DSFXCOMPRESSOR_ATTACK_MAX - DSFXCOMPRESSOR_ATTACK_MIN);
        m_CompressorParam.fAttack =b+ DSFXCOMPRESSOR_ATTACK_MIN;
        if  ( m_CompressorParam.fAttack > DSFXCOMPRESSOR_ATTACK_MAX)
          m_CompressorParam.fAttack = DSFXCOMPRESSOR_ATTACK_MAX;
        if  ( m_CompressorParam.fAttack < DSFXCOMPRESSOR_ATTACK_MIN)
          m_CompressorParam.fAttack = DSFXCOMPRESSOR_ATTACK_MIN;
        s.Format (_T ("%f"), m_CompressorParam.fAttack);
        m_GuiET_Attack.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Release)  {
        int pos = m_GuiTK_Release.GetPos ();
        float c = ((float)pos) / 2950.0f;
        float b = c * (DSFXCOMPRESSOR_RELEASE_MAX - DSFXCOMPRESSOR_RELEASE_MIN);
        m_CompressorParam.fRelease =b+ DSFXCOMPRESSOR_RELEASE_MIN;
        if  ( m_CompressorParam.fRelease > DSFXCOMPRESSOR_RELEASE_MAX)
          m_CompressorParam.fRelease = DSFXCOMPRESSOR_RELEASE_MAX;
        if  ( m_CompressorParam.fRelease < DSFXCOMPRESSOR_RELEASE_MIN)
          m_CompressorParam.fRelease = DSFXCOMPRESSOR_RELEASE_MIN;
        s.Format (_T ("%f"), m_CompressorParam.fRelease);
        m_GuiET_Release.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Thresold)  {
        int pos = m_GuiTK_Thresold.GetPos ();
        float c = ((float)pos) / 600.0f;
        float b = c * (DSFXCOMPRESSOR_THRESHOLD_MAX - DSFXCOMPRESSOR_THRESHOLD_MIN);
        m_CompressorParam.fThreshold =b+ DSFXCOMPRESSOR_THRESHOLD_MIN;
        if  ( m_CompressorParam.fThreshold > DSFXCOMPRESSOR_THRESHOLD_MAX)
          m_CompressorParam.fThreshold = DSFXCOMPRESSOR_THRESHOLD_MAX;
        if  ( m_CompressorParam.fThreshold < DSFXCOMPRESSOR_THRESHOLD_MIN)
          m_CompressorParam.fThreshold = DSFXCOMPRESSOR_THRESHOLD_MIN;
        s.Format (_T ("%f"), m_CompressorParam.fThreshold);
        m_GuiET_Threshold.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Ratio)  {
        int pos = m_GuiTK_Ratio.GetPos ();
        float c = ((float)pos) / 990.0f;
        float b = c * (DSFXCOMPRESSOR_RATIO_MAX - DSFXCOMPRESSOR_RATIO_MIN);
        m_CompressorParam.fRatio =b+ DSFXCOMPRESSOR_RATIO_MIN;
        if  ( m_CompressorParam.fRatio > DSFXCOMPRESSOR_RATIO_MAX)
          m_CompressorParam.fRatio = DSFXCOMPRESSOR_RATIO_MAX;
        if  ( m_CompressorParam.fRatio < DSFXCOMPRESSOR_RATIO_MIN)
          m_CompressorParam.fRatio = DSFXCOMPRESSOR_RATIO_MIN;
        s.Format (_T ("%f"), m_CompressorParam.fRatio);
        m_GuiET_Ratio.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Predelay)  {
        int pos = m_GuiTK_Predelay.GetPos ();
        float c = ((float)pos) / 40.0f;
        float b = c * (DSFXCOMPRESSOR_PREDELAY_MAX - DSFXCOMPRESSOR_PREDELAY_MIN);
        m_CompressorParam.fPredelay =b+ DSFXCOMPRESSOR_PREDELAY_MIN;
        if  ( m_CompressorParam.fPredelay > DSFXCOMPRESSOR_PREDELAY_MAX)
          m_CompressorParam.fPredelay = DSFXCOMPRESSOR_PREDELAY_MAX;
        if  ( m_CompressorParam.fPredelay < DSFXCOMPRESSOR_PREDELAY_MIN)
          m_CompressorParam.fPredelay = DSFXCOMPRESSOR_PREDELAY_MIN;
        s.Format (_T ("%f"), m_CompressorParam.fPredelay);
        m_GuiET_Predelay.SetWindowText (s);
      }  else  {
        bHandled = FALSE;
        return 0;
      }
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_COMPRESSOR, (CDirectMusic2::DSFX_desc *)& m_CompressorParam);
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
    HWND T = CreateWindowEx (0, WC_STATIC, _T ("Gain(dB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 21, 59, 42, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (WS_EX_TRANSPARENT, WC_BUTTON, _T ("IDirectSound8 Effects - Compressor Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 11, 10, 342, 221, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Attack(ms)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 21, 85, 51, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Release(ms)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 21, 111, 60, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Threshold(dB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 21, 137, 68, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Ratio(x:1)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 21, 167, 47, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Predealy(ms)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 21, 198, 62, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);

    // edit Group.
    m_GuiET_Gain = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 95, 57, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Gain.SetFont (hFont, FALSE);
    m_GuiET_Attack = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 95, 83, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Attack.SetFont (hFont, FALSE);
    m_GuiET_Release = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 95, 109, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Release.SetFont (hFont, FALSE);
    m_GuiET_Threshold = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 95, 135, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Threshold.SetFont (hFont, FALSE);
    m_GuiET_Ratio = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 95, 166, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Ratio.SetFont (hFont, FALSE);
    m_GuiET_Predelay = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 95, 195, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Predelay.SetFont (hFont, FALSE);

    // CSlider Group.
    m_GuiTK_Gain = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 55, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Gain.SetFont (hFont, FALSE);    
    m_GuiTK_Attack = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 80, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Attack.SetFont (hFont, FALSE);
    m_GuiTK_Release = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 106, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Release.SetFont (hFont, FALSE);
    m_GuiTK_Thresold = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 133, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Thresold.SetFont (hFont, FALSE);
    m_GuiTK_Ratio = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 163, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Ratio.SetFont (hFont, FALSE);
    m_GuiTK_Predelay = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 170, 193, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Predelay.SetFont (hFont, FALSE);

    // Check Group.
    m_GuiCK_Enable = CreateWindowEx (0, WC_BUTTON, _T ("Enable Compressor Effects"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 21, 29, 158, 24, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCK_Enable.SetFont (hFont, FALSE);


    CString str[6];
    str[0].Format ( _T ("%f"), m_CompressorParam.fGain);
    str[1].Format ( _T ("%f"), m_CompressorParam.fAttack);
    str[2].Format ( _T ("%f"), m_CompressorParam.fRelease);
    str[3].Format ( _T ("%f"), m_CompressorParam.fThreshold);
    str[4].Format ( _T ("%f"), m_CompressorParam.fRatio);
    str[5].Format ( _T ("%f"), m_CompressorParam.fPredelay);

    m_GuiTK_Gain.SetRange (0, 1200);
    m_GuiTK_Gain.SetPos ( (int) (( m_CompressorParam.fGain - DSFXCOMPRESSOR_GAIN_MIN) / (DSFXCOMPRESSOR_GAIN_MAX - DSFXCOMPRESSOR_GAIN_MIN) * 1200.0 ));
    m_GuiTK_Attack.SetRange (0, 4999);
    m_GuiTK_Attack.SetPos ( (int) (( m_CompressorParam.fAttack - DSFXCOMPRESSOR_ATTACK_MIN) / (DSFXCOMPRESSOR_ATTACK_MAX - DSFXCOMPRESSOR_ATTACK_MIN) * 4990.0 ));
    m_GuiTK_Release.SetRange (0, 2950);
    m_GuiTK_Release.SetPos ( (int) (( m_CompressorParam.fRelease - DSFXCOMPRESSOR_RELEASE_MIN) / (DSFXCOMPRESSOR_RELEASE_MAX - DSFXCOMPRESSOR_RELEASE_MIN) * 2950.0 ));  
    m_GuiTK_Thresold.SetRange (0, 600);
    m_GuiTK_Thresold.SetPos ( (int) (( m_CompressorParam.fThreshold - DSFXCOMPRESSOR_THRESHOLD_MIN) / (DSFXCOMPRESSOR_THRESHOLD_MAX - DSFXCOMPRESSOR_THRESHOLD_MIN) * 600.0 ));
    m_GuiTK_Ratio.SetRange (0, 990);
    m_GuiTK_Ratio.SetPos ( (int) (( m_CompressorParam.fRatio - DSFXCOMPRESSOR_RATIO_MIN) / (DSFXCOMPRESSOR_RATIO_MAX - DSFXCOMPRESSOR_RATIO_MIN) * 990.0 ));
    m_GuiTK_Predelay.SetRange (0, 40);
    m_GuiTK_Predelay.SetPos ( (int) (( m_CompressorParam.fPredelay - DSFXCOMPRESSOR_PREDELAY_MIN) / (DSFXCOMPRESSOR_PREDELAY_MAX - DSFXCOMPRESSOR_PREDELAY_MIN) * 40.0 ));

    //  set text .
    m_GuiET_Gain.SetWindowTextW (str[0]);
    m_GuiET_Attack.SetWindowTextW (str[1]);
    m_GuiET_Release.SetWindowTextW (str[2]);
    m_GuiET_Threshold.SetWindowTextW (str[3]);
    m_GuiET_Ratio.SetWindowTextW (str[4]);
    m_GuiET_Predelay.SetWindowTextW (str[5]);
    return 0;
  }
};

# endif // !defined(_CVIEWDS_EFFECTSCOMPRESSOR_INCLUED_HEADER_CXX_