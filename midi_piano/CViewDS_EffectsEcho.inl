# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  XXX: bad code to fix.
# if defined (__INL_PANE_ENABLE)

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewDS_EffectsEchoBaseTraits;

// # include "CDirectDraw.hxx"
class CViewDS_EffectsEchoBase:public CWindowImpl<CViewDS_EffectsEchoBase,CWindow,CViewDS_EffectsEchoBaseTraits>,
    public CScrollImpl<CViewDS_EffectsEchoBase>
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 372;
  static  const  int height = 192;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewDS_EffectsEchoBase,CWindow,CViewDS_EffectsEchoBaseTraits> baseClass;
  typedef    CScrollImpl<CViewDS_EffectsEchoBase> baseClass2;
  typedef CViewDS_EffectsEchoBase  thisClass;
  
  // attach DirectMusic Object.
  CDirectMusic2 *m_DirectMusic;  

  // ctrl .
  CEdit m_GuiET_TryMix;
  CEdit m_GuiET_FeedBack;
  CEdit m_GuiET_LeftDelay;
  CEdit m_GuiET_RightDelay;
  CEdit m_GuiET_PanDelay;
  
  CTrackBarCtrl m_GuiTK_TryMix;
  CTrackBarCtrl m_GuiTK_FeedBack;
  CTrackBarCtrl m_GuiTK_LeftDelay;
  CTrackBarCtrl m_GuiTK_RightDelay;
  CTrackBarCtrl m_GuiTK_PanDelay;

  CButton m_GuiCK_Enable;

  DSFXEcho m_EchoParam;

  CViewDS_EffectsEchoBase (void)  {
    m_EchoParam.fWetDryMix = 50.0;
    m_EchoParam.fFeedback = 50.0;
    m_EchoParam.fLeftDelay = 500;
    m_EchoParam.fRightDelay = 500;
    m_EchoParam.lPanDelay = 0;

    m_DirectMusic = NULL;
  }

  DECLARE_WND_CLASS(_T ("CViewDS_EffectsEchoBase"))

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
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_ECHO, FALSE);
    } else  {
      // set  effects .
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_ECHO, TRUE);
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_ECHO, (CDirectMusic2::DSFX_desc *)& m_EchoParam);
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
      
      if  ( hWindow == m_GuiTK_TryMix)  {
        int pos = m_GuiTK_TryMix.GetPos ();
        float c = ((float)pos) / 1000.0f;
        float b = c * (DSFXECHO_WETDRYMIX_MAX - DSFXECHO_WETDRYMIX_MIN);
        m_EchoParam.fWetDryMix =b+ DSFXECHO_WETDRYMIX_MIN;
        if  ( m_EchoParam.fWetDryMix > DSFXECHO_WETDRYMIX_MAX)
          m_EchoParam.fWetDryMix = DSFXECHO_WETDRYMIX_MAX;
        if  ( m_EchoParam.fWetDryMix < DSFXECHO_WETDRYMIX_MIN)
          m_EchoParam.fWetDryMix = DSFXECHO_WETDRYMIX_MIN;
        s.Format (_T ("%f"), m_EchoParam.fWetDryMix);
        m_GuiET_TryMix.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_FeedBack)  {
        int pos = m_GuiTK_FeedBack.GetPos ();
        float c = ((float)pos) / 1000.0f;
        float b = c * (DSFXECHO_FEEDBACK_MAX - DSFXECHO_FEEDBACK_MIN);
        m_EchoParam.fFeedback =b+ DSFXECHO_FEEDBACK_MIN;
        if  ( m_EchoParam.fFeedback > DSFXECHO_FEEDBACK_MAX)
          m_EchoParam.fFeedback = DSFXECHO_FEEDBACK_MAX;
        if  ( m_EchoParam.fFeedback < DSFXECHO_FEEDBACK_MIN)
          m_EchoParam.fFeedback = DSFXECHO_FEEDBACK_MIN;
        s.Format (_T ("%f"), m_EchoParam.fFeedback);
        m_GuiET_FeedBack.SetWindowText (s);
      } else  if  ( hWindow == m_GuiTK_LeftDelay)  {
        int pos = m_GuiTK_LeftDelay.GetPos ();
        float c = ((float)(pos - 1)) / 1999.0f;
        float b = c * (DSFXECHO_LEFTDELAY_MAX - DSFXECHO_LEFTDELAY_MIN);
        m_EchoParam.fLeftDelay =b+ DSFXECHO_LEFTDELAY_MIN;
        if  ( m_EchoParam.fLeftDelay > DSFXECHO_LEFTDELAY_MAX)
          m_EchoParam.fLeftDelay = DSFXECHO_LEFTDELAY_MAX;
        if  ( m_EchoParam.fLeftDelay < DSFXECHO_LEFTDELAY_MIN)
          m_EchoParam.fLeftDelay = DSFXECHO_LEFTDELAY_MIN;
        s.Format (_T ("%f"), m_EchoParam.fLeftDelay);
        m_GuiET_LeftDelay.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_RightDelay)  {
        int pos = m_GuiTK_RightDelay.GetPos ();
        float c = ((float)(pos - 1)) / 1999.0f;
        float b = c * (DSFXECHO_RIGHTDELAY_MAX - DSFXECHO_RIGHTDELAY_MIN);
        m_EchoParam.fRightDelay =b+ DSFXECHO_RIGHTDELAY_MIN;
        if  ( m_EchoParam.fRightDelay > DSFXECHO_RIGHTDELAY_MAX)
          m_EchoParam.fRightDelay = DSFXECHO_RIGHTDELAY_MAX;
        if  ( m_EchoParam.fRightDelay < DSFXECHO_RIGHTDELAY_MIN)
          m_EchoParam.fRightDelay = DSFXECHO_RIGHTDELAY_MIN;
        s.Format (_T ("%f"), m_EchoParam.fRightDelay);
        m_GuiET_RightDelay.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_PanDelay)  {
        int pos = m_GuiTK_PanDelay.GetPos ();
        m_EchoParam.lPanDelay = pos;
        s.Format (_T ("%d"), m_EchoParam.lPanDelay);
        m_GuiET_PanDelay.SetWindowText (s);
      } else  {
        bHandled = FALSE;
        return 0;
      }
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_ECHO, (CDirectMusic2::DSFX_desc *)& m_EchoParam);
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
    HWND T = CreateWindowEx(0, WC_STATIC, _T ("Wet/Try Mix(%)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 47, 75, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, _T ("IDirectSound8 Effects - Echo Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 8, 5, 350, 180, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("Feedback(%)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 73, 63, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("Left Delay(ms)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 99, 69, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("Right Delay(ms)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 125, 75, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx(0, WC_STATIC, _T ("Pan Delay(bool)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 156, 77, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    m_GuiCK_Enable = CreateWindowEx(0, WC_BUTTON, _T ("Enable Echo Effects"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 18, 24, 122, 13, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (m_GuiCK_Enable, WM_SETFONT, (WPARAM)hFont, FALSE);

    // CEdit Group.
    m_GuiET_TryMix = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 46, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_TryMix.SetFont (hFont, FALSE);
    m_GuiET_FeedBack = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 72, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_FeedBack.SetFont (hFont, FALSE);
    m_GuiET_LeftDelay = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 99, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_LeftDelay.SetFont (hFont, FALSE);
    m_GuiET_RightDelay = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 124, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_RightDelay.SetFont (hFont, FALSE);
    m_GuiET_PanDelay = CreateWindowEx(0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 154, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_PanDelay.SetFont (hFont, FALSE);

    // CSlider Ctrl.
    m_GuiTK_TryMix = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 42, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_TryMix.SetFont (hFont, FALSE);
    m_GuiTK_FeedBack = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 68, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_FeedBack.SetFont (hFont, FALSE);
    m_GuiTK_LeftDelay = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 96, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_LeftDelay.SetFont (hFont, FALSE);
    m_GuiTK_RightDelay = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 122, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_RightDelay.SetFont (hFont, FALSE);
    m_GuiTK_PanDelay = CreateWindowEx(0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 151, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_PanDelay.SetFont (hFont, FALSE);

    CString str[5];
    str[0].Format ( _T ("%f"), m_EchoParam.fWetDryMix);
    str[1].Format ( _T ("%f"), m_EchoParam.fFeedback);
    str[2].Format ( _T ("%f"), m_EchoParam.fLeftDelay);
    str[3].Format ( _T ("%f"), m_EchoParam.fRightDelay);
    str[4].Format ( _T ("%d"), m_EchoParam.lPanDelay);

    m_GuiTK_TryMix.SetRange (0, 1000);
    m_GuiTK_TryMix.SetPos ( (int) (m_EchoParam.fWetDryMix / DSFXECHO_WETDRYMIX_MAX * 1000.0 ));
    m_GuiTK_FeedBack.SetRange (0, 1000);
    m_GuiTK_FeedBack.SetPos ( (int) (m_EchoParam.fFeedback / DSFXECHO_FEEDBACK_MAX * 1000.0 ));
    m_GuiTK_LeftDelay.SetRange (1, 2000);
    m_GuiTK_LeftDelay.SetPos ( (int) (m_EchoParam.fLeftDelay / DSFXECHO_LEFTDELAY_MAX * 2000.0 ));
    m_GuiTK_RightDelay.SetRange (1, 2000);
    m_GuiTK_RightDelay.SetPos ( (int) (m_EchoParam.fRightDelay / DSFXECHO_RIGHTDELAY_MAX * 2000.0 ));
    m_GuiTK_PanDelay.SetRange (0, 1);
    m_GuiTK_PanDelay.SetPos ( (int) (m_EchoParam.lPanDelay / DSFXECHO_FEEDBACK_MAX ));

    m_GuiET_TryMix.SetWindowText (str[0]);
    m_GuiET_FeedBack.SetWindowText (str[1]);
    m_GuiET_LeftDelay.SetWindowText (str[2]);
    m_GuiET_RightDelay.SetWindowText (str[3]);
    m_GuiET_PanDelay.SetWindowText (str[4]);

    return 0;
  }
};

# endif // !defined(_CVIEWDS_EFFECTSECHO_INCLUED_HEADER_CXX_