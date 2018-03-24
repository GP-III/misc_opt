#  if !defined (__PRECOMPILED_H)
#   define __PRECOMPILED_H

# include <wx/wxprec.h>
# include <wx/dcbuffer.h>
# include <wx/aui/aui.h>
# include <wx/artprov.h>
# include <wx/colordlg.h>
# include <wx/docview.h>
# include <wx/docmdi.h>
# include <wx/listctrl.h>
# include <wx/artprov.h>
# include <wx/infobar.h>
# include <wx/file.h>
# include <wx/hyperlink.h>
# include <wx/spinctrl.h>
# include <wx/spinbutt.h>
# include <wx/dialog.h>
# include <wx/filedlg.h>
# include <wx/event.h>
# include <wx/numdlg.h>
# include <wx/statline.h>
# include <wx/textfile.h>
# include <vector>
# include <list>
# include <stack>
# include "xpmid_file.xpm"
# include "xpmid_view.xpm"
# include "xpmid_about.xpm"
# include "wxWidgets_pp.xpm"

# if defined (_WIN32)
#   if defined _M_IX86  
#     pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#   elif defined _M_IA64  
#     pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#   elif defined _M_X64  
#     pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#   else  
#     pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#   endif
#endif

#  endif 