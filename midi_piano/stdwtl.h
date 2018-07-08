// stdwtl.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_STD_WTL_INCLUED_HEADER_)
#  define _STD_WTL_INCLUED_HEADER_

// Change these values to use different versions
# define WINVER		0x0601
# define _WIN32_WINNT	0x0601
# define _WIN32_IE	0x0700
# define _RICHEDIT_VER	0x0500
# define _WTL_NO_CSTRING 
# define END_MSG_MAP_EX	END_MSG_MAP

# include <atlbase.h>
# include <atlcoll.h>
# include <WTL/atlapp.h>
# include <WTL/atlscrl.h>
# include <WTL/atlmisc.h>
# include <WTL/atlframe.h>
# include <WTL/atlctrls.h>
# include <WTL/atldlgs.h>
# include <WTL/atlctrlw.h>
# include <WTL/atlctrlx.h>
# include <WTL/atlctrls.h>
# include <WTL/atlsplit.h>
# include <dockwins/atlgdix.h>
# include <dockwins/DWAutoHide.h>
# include <dockwins/CustomTabCtrl.h>
# include <dockwins/DockingFrame.h>
# include <dockwins/PlainTextView.h>
# include <dockwins/DotNetTabCtrl.h>
# include <dockwins/FlyingTabs.h>
# include "CDockingTabChild.hxx"
# include "CDockingTabContainer.hxx"

extern CAppModule _Module;

# include <atlwin.h>

// Embedded style list
# if defined (_M_IX86)
  # pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
# elif defined (_M_IA64)
  # pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
# elif defined (_M_X64)
  # pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
# else
  # pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
# endif

template<typename T> void release_com (T*& T_) {
  if (T_ != NULL)  {
    HRESULT sig= T_->Release ();
    ATLASSERT (SUCCEEDED (sig));
    T_ = NULL;
  }
}
template<typename T> void release_object (T*& T_) {
  if (T_ != NULL)  {
    delete T_;
    T_ = NULL;
  }
}
template<typename T> void release_objects (T*& T_) {
  if (T_ != NULL)  {
    delete [] T_;
    T_ = NULL;
  }
}

template<typename T> LPCTSTR GetIntBuf (T value) {
  static TCHAR szIntBuf[48];
  _stprintf (szIntBuf, _T("%d"), value);
  return szIntBuf;
}
template<typename T> LPCTSTR GetUintBuf (T value) {
  static TCHAR szIntBuf[48];
  _stprintf (szIntBuf, _T("%u"), value);
  return szIntBuf;
}

template<class T>
class CEditEnterStuff : public CEdit, 
                        public CMessageFilter {
public:
  BOOL PreTranslateMessage (MSG* pMsg) {
    if ( (pMsg->message == WM_CHAR) 
      && (pMsg->wParam == VK_RETURN)) 
      if ( pMsg->hwnd == *this ) {
        (TThis->*OnCommandEditEnter) (pMsg); 
        return TRUE;
      }
      else ;
    else ;
    return FALSE;
  }
  
  CEditEnterStuff (void) {}
  CEditEnterStuff (T *p, void (T::*__OnCommandEditEnter)(MSG* pMsg)) {
    SetAttach (p, __OnCommandEditEnter);
  }
  void SetAttach (T *p, void (T::*__OnCommandEditEnter)(MSG* pMsg)) {
    TThis = p;
    OnCommandEditEnter = __OnCommandEditEnter;
  }
  // void SetAttachIt ()
private:
  T *TThis;
  void (T::*OnCommandEditEnter)(MSG* pMsg);
};

# define COMMAND_HANDLER2(wnd, code, func)                                    \
                                                                              \
	 if (uMsg == WM_COMMAND && wnd == (HWND)lParam && code == HIWORD(wParam))   \
	 {                                                                          \
		 bHandled = TRUE;                                                         \
		 lResult = func (HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		 if(bHandled)                                                             \
		 	return TRUE;                                                            \
	 }

static 
void AppendString (std::vector<CString> &t, CComboBox &combo, BOOL insert = FALSE)  {
  std::vector<CString>::iterator iter;
  if (insert == FALSE) combo.ResetContent ();
  for (iter = t.begin (); iter != t.end(); iter++) {
    combo.InsertString (-1, (*iter));
  }
}

# endif  // !defined(_STD_WTL_INCLUED_HEADER_)