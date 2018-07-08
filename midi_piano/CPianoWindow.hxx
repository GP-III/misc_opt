# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CPIANOWINDOW_INCLUED_HEADER_CXX_)
#  define _CPIANOWINDOW_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "resource.h"
# include "CDirectDraw.hxx"
# include "CPianoDraw.hxx"

class CPianoWindow : public CWindowImpl<CPianoWindow, CWindow, CControlWinTraits> 
{ 
public: 
  static  const  int piano_width = 637;
  static  const  int piano_height = 240;
  static  const unsigned __int64 m_refresh_persecond = 32;
  CDirectDraw ::graph_desc m_dst;
  CDirectDraw ::graph_desc m_img; // piano texture.
  CDirectDraw *m_DirectDraw;
  CWindow *m_AttachClipWindow;
  typedef CWindowImpl<CPianoWindow, CWindow, CControlWinTraits> baseClass;
public:
  BEGIN_MSG_MAP(CPianoWindow)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
  END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//  LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//  LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//  LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
  LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
    m_DirectDraw->lockBuffer (NULL, m_dst);
    CDirectDraw :: CSurfaceBlit (m_dst, CPoint (0, 0), m_img, CPoint (0, 0), CSize (piano_width, piano_height));
    m_DirectDraw->unlockBuffer ();
    m_DirectDraw->BlitBtoV (NULL, NULL, NULL);
    m_DirectDraw->postRender (NULL, NULL);
    ValidateRect (NULL);
    return 0;
  }
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
    RECT rc;
    GetClientRect (& rc);
    ::SetWindowPos(*m_AttachClipWindow, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,SWP_NOZORDER | SWP_NOACTIVATE);
    return 0;
  }

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    CPianoDraw__::LoadDefaultSettings ();

    m_DirectDraw = new CDirectDraw (m_hWnd, CSize (piano_width, piano_height), CSize (piano_width, piano_height));
    m_DirectDraw->LoadImage (IDB_MIDI_PIANO, piano_width, piano_height, m_img);

    return TRUE;
  }
  ~ CPianoWindow ()  {
    m_DirectDraw->FreeImage (m_img);
    release_object<CDirectDraw> (m_DirectDraw);
  }
  void BeginBatchDraw (void)  {
    m_DirectDraw->lockBuffer (NULL, m_dst);
  }
  void EndBatchDraw (void)  {
    m_DirectDraw->unlockBuffer ();
    m_DirectDraw->BlitBtoV (NULL, NULL, NULL);
    m_DirectDraw->postRender (NULL, NULL);
  }
  void drawPianoPress (int octave_step, int note /* start from 1*/)  {
    CPianoDraw__ ::drawPiano (m_dst, octave_step, note);
  }
  void drawPianoUp (int octave_step, int note /* start from 1*/)  {
    CPianoDraw__ ::drawPiano2 (m_dst, m_img, octave_step, note);
  }
};

# endif // !defined(_CPIANOWINDOW_INCLUED_HEADER_CXX_)