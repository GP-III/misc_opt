# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CMESSAGEIDLEFASTLOOP_INCLUED_HEADER_CXX_)
#  define _CMESSAGEIDLEFASTLOOP_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "resource.h"
# include "CDirectDraw.hxx"
# include <dinput.h>

// ATL native message very slow
class CMessageIdleFastLoop : public CMessageLoop {
public :
  int Run()
  {
    for(;;)
    {
      while (:: PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
      {
        if (m_msg.message == WM_QUIT)
          return TRUE;
        if(!PreTranslateMessage(&m_msg))
        {
          ::TranslateMessage(&m_msg);
          ::DispatchMessage(&m_msg);
        }
      }
      // scan all idle 
      CMessageLoop :: OnIdle (0);
    }
  }
};

# endif // !defined(_CMESSAGEIDLEFASTLOOP_INCLUED_HEADER_CXX_)