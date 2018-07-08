# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CABOUT_INCLUED_HEADER_CXX_)
#  define _CABOUT_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "resource.h"

//  Help - AboutATL .
class CAboutATL : public CDialogImpl<CAboutATL>
{
public:
  enum { IDD = IDD_ABOUT_ATL };

  BEGIN_MSG_MAP(CAboutATL)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnOkOrCancel)
    COMMAND_ID_HANDLER(IDCANCEL, OnOkOrCancel)
    NOTIFY_HANDLER(IDC_SYSLINK_ATL_HOMEPAGE, NM_CLICK, OnNMClick_)
  END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//  LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//  LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//  LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
  LRESULT OnNMClick_ (int dummy_var, NMHDR *pNMHDR, BOOL& bHandled) 
  {  
    bHandled = TRUE;
    PNMLINK pNMLink = (PNMLINK) pNMHDR;   
    if (pNMLink->hdr.hwndFrom == GetDlgItem (IDC_SYSLINK_ATL_HOMEPAGE))   
      ShellExecute (NULL, _T("open"), _T ("https://msdn.microsoft.com/en-us/library/3ax346b7.aspx"), NULL, NULL, SW_SHOWNORMAL);         
    else  bHandled = FALSE;   // The return value is ignored by the control. see https://msdn.microsoft.com/en-us/library/windows/desktop/bb760714(v=vs.85).aspx
    return 0;
  }  

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    // center the dialog on the screen
    CenterWindow();

    // set icons
    HICON hIcon = AtlLoadIconImage(IDI_ATL, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDI_ATL, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    return TRUE;
  }

  LRESULT OnOkOrCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    // TODO: Add validation code 
    EndDialog(wID);
    return 0;
  }
};
//  Help - AboutWTL .
class CAboutWTL : public CDialogImpl<CAboutWTL>
{
public:
  enum { IDD = IDD_ABOUT_WTL };

  BEGIN_MSG_MAP(CAboutWTL)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnOkOrCancel)
    COMMAND_ID_HANDLER(IDCANCEL, OnOkOrCancel)
    NOTIFY_HANDLER(IDC_SYSLINK_WTL_HOMEPAGE, NM_CLICK, OnNMClick_)
    NOTIFY_HANDLER(IDC_SYSLINK_DOCKWINS_HOMEPAGE, NM_CLICK, OnNMClick_)
    NOTIFY_HANDLER(IDC_SYSLINK_LISTCTRL_HOMEPAGE, NM_CLICK, OnNMClick_)
  END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//  LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//  LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//  LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
  LRESULT OnNMClick_ (int dummy_var, NMHDR *pNMHDR, BOOL& bHandled)  
  {  
    bHandled = TRUE;
    PNMLINK pNMLink = (PNMLINK) pNMHDR;   
    if (pNMLink->hdr.hwndFrom == GetDlgItem (IDC_SYSLINK_WTL_HOMEPAGE))   
      ShellExecute (NULL, _T("open"), _T ("https://sourceforge.net/projects/wtl/"), NULL, NULL, SW_SHOWNORMAL);
    else if (pNMLink->hdr.hwndFrom == GetDlgItem (IDC_SYSLINK_DOCKWINS_HOMEPAGE)) 
      ShellExecute (NULL, _T("open"), _T ("https://bitbucket.org/sergiu/wtl-dockwins"), NULL, NULL, SW_SHOWNORMAL);        
    else if (pNMLink->hdr.hwndFrom == GetDlgItem (IDC_SYSLINK_LISTCTRL_HOMEPAGE)) 
      ShellExecute (NULL, _T("open"), _T ("https://www.codeproject.com/Articles/13383/ListCtrl-A-WTL-list-control-with-Windows-Vista-sty"), NULL, NULL, SW_SHOWNORMAL);        
    else bHandled = FALSE;
    return  0; // The return value is ignored by the control. see https://msdn.microsoft.com/en-us/library/windows/desktop/bb760714(v=vs.85).aspx
  }  

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    // center the dialog on the screen
    CenterWindow();

    // set icons
    HICON hIcon = AtlLoadIconImage(IDI_WTL, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDI_WTL, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    return TRUE;
  }

  LRESULT OnOkOrCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    // TODO: Add validation code 
    EndDialog(wID);
    return 0;
  }
};

# endif // !defined(_CABOUT_INCLUED_HEADER_CXX_)