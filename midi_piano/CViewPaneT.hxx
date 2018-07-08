# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CVIEWPANET_INCLUED_HEADER_CXX_)
#  define _CVIEWPANET_INCLUED_HEADER_CXX_

// Open pane inline.
# define __INL_PANE_ENABLE

# include "stdwtl.h"
# include "CDockingTabChild.hxx"
# include <dockwins/TabbedFrame.h>
# include <WTL/atltheme.h>
# include <WTL/atlcrack.h>
# include <ListCtrl/ListCtrl.h>
# include "CDirectMusic2.hxx"
# include "CFileDialogFilter.hxx"
// # include "CPlayerThread.hxx"
# include "resource.h"

// global setttings.
static LPCTSTR __m_P[] = {
#  define CVIEWDS_EFFECTS_GARGLE_INDEX 0
  _T ("CViewDS_Effects_Gargle"),
#  define CVIEWDS_EFFECTS_CHORUS_INEDX (CVIEWDS_EFFECTS_GARGLE_INDEX+1)
  _T ("CViewDS_Effects_Chorus"),
#  define CVIEWDS_EFFECTS_FLANGER_INEDX (CVIEWDS_EFFECTS_CHORUS_INEDX+1)
  _T ("CViewDS_Effects_Flanger"),
#  define CVIEWDS_EFFECTS_ECHO_INEDX (CVIEWDS_EFFECTS_FLANGER_INEDX+1)
  _T ("CViewDS_Effects_Echo"),
#  define CVIEWDS_EFFECTS_DISTORTION_INEDX (CVIEWDS_EFFECTS_ECHO_INEDX+1)
  _T ("CViewDS_Effects_Distortion"),
#  define CVIEWDS_EFFECTS_COMPRESSOR_INEDX (CVIEWDS_EFFECTS_DISTORTION_INEDX+1)
  _T ("CViewDS_Effects_Compressor"),
#  define CVIEWDS_EFFECTS_PARAMEQ_INEDX (CVIEWDS_EFFECTS_COMPRESSOR_INEDX+1)
  _T ("CViewDS_Effects_ParamEQ"),
#  define CVIEWDS_EFFECTS_WAVEREVERB_INEDX (CVIEWDS_EFFECTS_PARAMEQ_INEDX+1)
  _T ("CViewDS_Effects_WaveReverb"),
#  define CVIEWDS_EFFECTS_WAVEREVERB_LEVEL2_INDEX (CVIEWDS_EFFECTS_WAVEREVERB_INEDX+1)
  _T ("CViewDS_Effects_WaveReverb_Level2"),
#  define CVIEW_INSTRUCTMENTS_INDEX (CVIEWDS_EFFECTS_WAVEREVERB_LEVEL2_INDEX+1)
  _T ("CView_Instructments"),
#  define CVIEW_RECORD_INDEX (CVIEW_INSTRUCTMENTS_INDEX+1)
  _T ("CView_Record")
};

template <class T, class TBase, WORD wEffectsIndex, WORD wID_CommandMenu, WORD wID_ICON = -1>
class CViewPaneT : public dockwins::CDockingTabChild<T>
{
public:
  CMenu m_AttachMenu;
  CWindow *m_AttachClipWindow;
  typedef    dockwins::CDockingTabChild<T> baseClass;
  typedef CViewPaneT  thisClass;

  CSplitterWindow m_splitter;  // as panel
  TBase m_panel;

  BOOL m_InitViewPane;

  CViewPaneT (void) {
    m_InitViewPane = FALSE;
  }

  BOOL isInit (void)  {
    return !! m_InitViewPane;
  }

  void SetInit (BOOL init) {
    m_InitViewPane = init;
  }

  static ATL::CWndClassInfo& GetWndClassInfo() 
  { 
    static ATL::CWndClassInfo wc = 
    { 
      { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, StartWindowProc, 
        0, 0, NULL, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, __m_P[wEffectsIndex], NULL }, 
      NULL, NULL, IDC_ARROW, TRUE, 0, _T("") 
    }; 
    wc.m_wc.hIcon = LoadIcon (_Module.GetResourceInstance (), MAKEINTRESOURCE (wID_ICON));
    wc.m_wc.hIconSm = LoadIcon (_Module.GetResourceInstance (), MAKEINTRESOURCE (wID_ICON));
    return wc; 
  }

  BEGIN_MSG_MAP (thisClass)
    MESSAGE_HANDLER (WM_CLOSE, OnClose)
    MESSAGE_HANDLER (WM_SIZE, OnSize)
    MESSAGE_HANDLER (WM_CREATE, OnCreate)
    CHAIN_MSG_MAP (baseClass)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP ()

  void SetAttach (CDirectMusic2 *cmusic, HMENU menu) {
    m_AttachMenu = menu;
    m_panel.SetAttach (cmusic);
  }

  LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) 
  { 
    m_AttachMenu.EnableMenuItem (wID_CommandMenu, MF_ENABLED | MF_BYCOMMAND );
    ShowWindow (SW_HIDE);
    // post to parent tab container deal, no care it.
    bHandled = FALSE;
    return 0;
  }
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
  {
    RECT rc;
    GetClientRect (& rc);
    ::SetWindowPos(m_splitter.m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,SWP_NOZORDER | SWP_NOACTIVATE);
    return 0;
  }

  LRESULT OnCreate (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
  {
    m_splitter.Create(m_hWnd, rcDefault, NULL,
           WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    m_splitter.ResizeClient (TBase ::width,
           TBase::height);
    this->ResizeClient (TBase ::width,
           TBase::height);
    m_splitter.SetSinglePaneMode (SPLIT_PANE_LEFT);
    m_panel.Create (m_splitter, rcDefault);
    m_splitter.SetSplitterPanes (m_panel, NULL);

    m_panel.ResizeClient (TBase::width, TBase::height, TRUE);
    m_panel.SetScrollSize (TBase::width, TBase::height, FALSE, 0);
    return 0;
  }

  LRESULT OnTestCreate (HWND parent, HMENU menu, CDirectMusic2 *music) {
    if  (this->isInit () != FALSE)  
      this->ShowWindow (SW_SHOWNORMAL);
    else {
      this->SetInit (TRUE);
      this->SetAttach (music, menu);
      this->Create (parent, rcDefault, __m_P[wEffectsIndex], WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
      :: EnableMenuItem (  menu, wID_CommandMenu, MF_GRAYED | MF_BYCOMMAND );
    }
    return  0;
  }
};
# include "CViewDS_EffectsGargle.inl"
# include "CViewDS_EffectsChorus.inl"
# include "CViewDS_EffectsFlanger.inl"
# include "CViewDS_EffectsEcho.inl"
# include "CViewDS_EffectsDistortion.inl"
# include "CViewDS_EffectsCompressor.inl"
# include "CViewDS_EffectsParamEQ.inl"
# include "CViewDS_EffectsReverb.inl"
# include "CViewDS_EffectsReverbLevel2.inl"
# include "CViewIoInstructments.inl"
# include "CViewPlayRecordSettings.inl"

class CViewDS_EffectsReverb : 
   public CViewPaneT
            <  CViewDS_EffectsReverb, 
               CViewDS_EffectsReverbBase,
               CVIEWDS_EFFECTS_WAVEREVERB_INEDX,
               ID_EFFECT_REVERB, IDI_EFFECT_REVERB  > {};
class CViewDS_EffectsReverbLevel2 : 
   public CViewPaneT
            <  CViewDS_EffectsReverbLevel2, 
               CViewDS_EffectsReverbLevel2Base,
               CVIEWDS_EFFECTS_WAVEREVERB_LEVEL2_INDEX,
               ID_EFFECT_I3DL2REVERB, IDI_EFFECT_REVERB     > {};
class CViewDS_EffectsCompressor : 
   public CViewPaneT
            <  CViewDS_EffectsCompressor, 
               CViewDS_EffectsCompressorBase,
               CVIEWDS_EFFECTS_COMPRESSOR_INEDX,
               ID_EFFECT_COMPRESSOR, IDI_EFFECT_COMPRESSOR   > {};
class CViewDS_EffectsParamEQ : 
   public CViewPaneT
            <  CViewDS_EffectsParamEQ, 
               CViewDS_EffectsParamEQBase,
               CVIEWDS_EFFECTS_PARAMEQ_INEDX,
               ID_EFFECT_PARAMEQ, IDI_EFFECT_EQ   > {};
class CViewDS_EffectsDistortion :
   public CViewPaneT
            <  CViewDS_EffectsDistortion, 
               CViewDS_EffectsDistortionBase,
               CVIEWDS_EFFECTS_DISTORTION_INEDX,
               ID_EFFECT_DISTORTION, IDI_EFFECT_DISTORTION   > {};
class CViewDS_EffectsGargle : 
   public CViewPaneT
            <  CViewDS_EffectsGargle, 
               CViewDS_EffectsGargleBase,
               CVIEWDS_EFFECTS_GARGLE_INDEX,
               ID_EFFECT_GARGLE, IDI_EFFECT_GARGLE   > {};
class CViewDS_EffectsFlanger : 
   public CViewPaneT
            <  CViewDS_EffectsFlanger, 
               CViewDS_EffectsFlangerBase,
               CVIEWDS_EFFECTS_FLANGER_INEDX,
               ID_EFFECT_FLANGER, IDI_EFFECT_FLANGER   > {};
class CViewDS_EffectsChorus : 
   public CViewPaneT
            <  CViewDS_EffectsChorus, 
               CViewDS_EffectsChorusBase,
               CVIEWDS_EFFECTS_CHORUS_INEDX,
               ID_EFFECT_CHORUS, IDI_EFFECT_CHORUS   > {};
class CViewDS_EffectsEcho : 
   public CViewPaneT
            <  CViewDS_EffectsEcho, 
               CViewDS_EffectsEchoBase,
               CVIEWDS_EFFECTS_ECHO_INEDX,
               ID_EFFECT_ECHO, IDI_EFFECT_ECHO   > {};
class CViewIoInstructments : 
   public CViewPaneT
            <  CViewIoInstructments, 
               CViewIoInstructmentsBase,
               CVIEW_INSTRUCTMENTS_INDEX,
               ID_IO_INSTRUCTMENTS, IDI_IO_INSTRUCTMENTS   > {};
class CViewPlayRecordSettings : 
   public CViewPaneT
            <  CViewPlayRecordSettings, 
               CViewPlayRecordSettingsBase,
               CVIEW_RECORD_INDEX,
               ID_RECORD_WINDOW, IDI_RECORD   > {};

// Close Inline.
# undef __INL_PANE_ENABLE

# endif // !defined(_CVIEWDS_EFFECTS_INCLUED_HEADER_CXX_