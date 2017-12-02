// 东方永夜抄的按键小工具 
// -----------------------------------------------------------------------------
# include "resource.h"
# include <windows.h>
# include <assert.h>
# include <tchar.h>
# include <windowsx.h> // combo 控件包含
# include <commctrl.h> // listview/richedit/tree 控件包含
# include <dinput.h> // DIK按键枚举掩码包含

// 分析反汇编代码分析得出实际PE文件的位置
// 游戏为日文原版,汉化版未必能正常使用
// 工具:IDA
// 按键子过程:sub_43D970
// 通过查找导入表得出 窗口模式下 CreateWindowA的Style偏移 0x4213F
// 通过查找.rdata段的错误设置的字面量得到 IDirectInputDevice8设置协调等级时的位掩码偏移 0x46982
// 
// 游戏校验失败导致的伪造违例访问子过程[详见sub_44858D反汇编和反编译]: 0x48442 76->EB
//                                                  0x484B4 C8 FF -> E0 00
// ----------------------------------------------------------------------------
# define PE_sub_43D970_addr 0x3D770 
# define PE_sub_44858D_crack_0xEB_addr 0x48442
# define PE_sub_44858D_crack_0xE0_0x00_addr 0x484B4
# define PE_sub_446A37_setcoope_addr 0x46982
# define PE_sub_446A37_resizeWindow_addr 0x4213F
 
# define TH08_coope_level (DISCL_NOWINKEY | DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) // 0x16 
# define TH08_window_style (WS_VISIBLE | WS_GROUP | WS_SYSMENU) // 0x100A0000

static // 0x43D970
char sub_43D970_stacksave[] =
# define sub_43D970_stacksave_addr 0x43D970
{
  0xC8, 0x08, 0x01, 0x00, // C8 08 01 00          enter 108h,0
  0x53,                   // 53                   push ebx
  0x33, 0xDB,             // 33 DB                xor  ebx,ebx
  0xB3, 0x80              // B3 80                mov   bl,80h
};

static // 0x43E159
char sub_43D970_stackresume[] =
# define sub_43D970_stackresume_addr 0x43E159
{
  0x5B, // 5B                   pop         ebx  
  0xC9, // C9                   leave  
  0xC3, // C3                   ret
  0x90  // 90                   nop 
};

// 按键功能代码 
# define CODE_FUNC_UP 0
# define CODE_FUNC_DOWN 1
# define CODE_FUNC_LEFT 2
# define CODE_FUNC_RIGHT 3
# define CODE_FUNC_SHOOT 4
# define CODE_FUNC_SKILL 5
# define CODE_FUNC_QUIT 6
# define CODE_FUNC_ENTER 7
# define CODE_FUNC_SWITCH 8
# define CODE_FUNC_SWITCH2 9
# define CODE_FUNC_SKIP 10
# define CODE_FUNC_SKIP2 11
# define CODE_FUNC_UNUSED -1

# define EAX_MODRM__ 0
# define ECX_MODRM__ 1
# define EDX_MODRM__ 2

static 
int sub_43D970_joypad_fast[] =
{
  0x0043DD9E, EDX_MODRM__, CODE_FUNC_UP, 10, //0  //0F B6 55 C8                                   movzx   edx, [ebp+var_38] // DIK_UP 需要扩容
  0x0043DDBF, ECX_MODRM__, CODE_FUNC_DOWN, 10, //1  //0F B6 4D D0                                   movzx   ecx, [ebp+var_30] // DIK_DOWN 需要扩容
  0x0043DDE0, EAX_MODRM__, CODE_FUNC_LEFT, 9, //2  //0F B6 45 CB                                   movzx   eax, [ebp+var_35] // DIK_LEFT 需要扩容
  0x0043DE00, EDX_MODRM__, CODE_FUNC_RIGHT, 10, //3  //0F B6 55 CD                                   movzx   edx, [ebp+var_33] // DIK_RIGHT 需要扩容
  0x0043DFB9, EDX_MODRM__, CODE_FUNC_SHOOT, 13,  //5 //0F B6 95 2C FF FF FF                          movzx   edx, [ebp+var_D4] // DIK_Z
  0x0043DFDC, ECX_MODRM__, CODE_FUNC_SKILL, 13,  //4 //0F B6 8D 2D FF FF FF                          movzx   ecx, [ebp+var_D3] // DIK_X
  0x0043E047, ECX_MODRM__, CODE_FUNC_QUIT, 13, //19 //0F B6 8D 01 FF FF FF                          movzx   ecx, [ebp+var_FF] // DIK_ESCAPE
  0x0043E100, EDX_MODRM__, CODE_FUNC_ENTER, 13, //24 //0F B6 95 1C FF FF FF                          movzx   edx, [ebp+var_E4] // DIK_RETURN
  0x0043E000, EAX_MODRM__, CODE_FUNC_SWITCH, 12, //17 //0F B6 85 2A FF FF FF                          movzx   eax, [ebp+var_D6] // DIK_LSHIFT
  0x0043E023, EDX_MODRM__, CODE_FUNC_SWITCH2, 13, //18 //0F B6 95 36 FF FF FF                          movzx   edx, [ebp+var_CA] // DIK_RSHIFT
  0x0043E06B, EAX_MODRM__, CODE_FUNC_SKIP, 12,  //20 //0F B6 85 1D FF FF FF                          movzx   eax, [ebp+var_E3] // DIK_LCONTROL
  0x0043E090, EDX_MODRM__, CODE_FUNC_SKIP2, 10 //21 //0F B6 55 9D                                   movzx   edx, [ebp+var_63] // DIK_RCONTROL 需要扩容
};

static 
int sub_43D970_joypad[] =
{
  0x0043DD9E, EDX_MODRM__, CODE_FUNC_UP, //0  //0F B6 55 C8                                   movzx   edx, [ebp+var_38] // DIK_UP 需要扩容
  0x0043DDBF, ECX_MODRM__, CODE_FUNC_DOWN, //1  //0F B6 4D D0                                   movzx   ecx, [ebp+var_30] // DIK_DOWN 需要扩容
  0x0043DDE0, EAX_MODRM__, CODE_FUNC_LEFT, //2  //0F B6 45 CB                                   movzx   eax, [ebp+var_35] // DIK_LEFT 需要扩容
  0x0043DE00, EDX_MODRM__, CODE_FUNC_RIGHT, //3  //0F B6 55 CD                                   movzx   edx, [ebp+var_33] // DIK_RIGHT 需要扩容
  0x0043DE24, ECX_MODRM__, CODE_FUNC_UNUSED, //4  //0F B6 8D 48 FF FF FF                          movzx   ecx, [ebp+var_B8] // DIK_NUMPAD8
  0x0043DE48, EAX_MODRM__, CODE_FUNC_UNUSED, //5  //0F B6 85 50 FF FF FF                          movzx   eax, [ebp+var_B0] // DIK_NUMPAD2
  0x0043DE6B, EDX_MODRM__, CODE_FUNC_UNUSED, //6  //0F B6 95 4B FF FF FF                          movzx   edx, [ebp+var_B5] // DIK_NUMPAD4
  0x0043DE8F, ECX_MODRM__, CODE_FUNC_UNUSED, //7  //0F B6 8D 4D FF FF FF                          movzx   ecx, [ebp+var_B3] // DIK_NUMPAD6
  0x0043DEB6, EAX_MODRM__, CODE_FUNC_UNUSED, //8  //0F B6 85 47 FF FF FF                          movzx   eax, [ebp+var_B9] // DIK_NUMPAD7
  0x0043DED9, EDX_MODRM__, CODE_FUNC_UNUSED, //9  //0F B6 95 49 FF FF FF                          movzx   edx, [ebp+var_B7] // DIK_NUMPAD9
  0x0043DF00, ECX_MODRM__, CODE_FUNC_UNUSED, //10 //0F B6 8D 4F FF FF FF                          movzx   ecx, [ebp+var_B1] // DIK_NUMPAD1
  0x0043DF24, EAX_MODRM__, CODE_FUNC_UNUSED, //11 //0F B6 85 51 FF FF FF                          movzx   eax, [ebp+var_AF] // DIK_NUMPAD3
  0x0043DF49, EDX_MODRM__, CODE_FUNC_UNUSED, //12 //0F B6 55 C7                                   movzx   edx, [ebp+var_39] // DIK_HOME 需要扩容
  0x0043DF6D, ECX_MODRM__, CODE_FUNC_UNUSED, //13 //0F B6 8D 19 FF FF FF                          movzx   ecx, [ebp+var_E7] // DIK_P
  0x0043DF94, EAX_MODRM__, CODE_FUNC_UNUSED, //14 //0F B6 85 20 FF FF FF                          movzx   eax, [ebp+var_E0] // DIK_D
  0x0043DFB9, EDX_MODRM__, CODE_FUNC_SHOOT, //15 //0F B6 95 2C FF FF FF                          movzx   edx, [ebp+var_D4] // DIK_Z
  0x0043DFDC, ECX_MODRM__, CODE_FUNC_SKILL, //16 //0F B6 8D 2D FF FF FF                          movzx   ecx, [ebp+var_D3] // DIK_X
  0x0043E000, EAX_MODRM__, CODE_FUNC_SWITCH, //17 //0F B6 85 2A FF FF FF                          movzx   eax, [ebp+var_D6] // DIK_LSHIFT
  0x0043E023, EDX_MODRM__, CODE_FUNC_SWITCH2, //18 //0F B6 95 36 FF FF FF                          movzx   edx, [ebp+var_CA] // DIK_RSHIFT
  0x0043E047, ECX_MODRM__, CODE_FUNC_QUIT, //19 //0F B6 8D 01 FF FF FF                          movzx   ecx, [ebp+var_FF] // DIK_ESCAPE
  0x0043E06B, EAX_MODRM__, CODE_FUNC_SKIP, //20 //0F B6 85 1D FF FF FF                          movzx   eax, [ebp+var_E3] // DIK_LCONTROL
  0x0043E090, EDX_MODRM__, CODE_FUNC_SKIP2, //21 //0F B6 55 9D                                   movzx   edx, [ebp+var_63] // DIK_RCONTROL 需要扩容
  0x0043E0B4, ECX_MODRM__, CODE_FUNC_UNUSED, //22 //0F B6 8D 10 FF FF FF                          movzx   ecx, [ebp+var_F0] // DIK_Q
  0x0043E0DB, EAX_MODRM__, CODE_FUNC_UNUSED, //23 //0F B6 85 1F FF FF FF                          movzx   eax, [ebp+var_E1] // DIK_S
  0x0043E100, EDX_MODRM__, CODE_FUNC_ENTER, //24 //0F B6 95 1C FF FF FF                          movzx   edx, [ebp+var_E4] // DIK_RETURN
  0x0043E127, ECX_MODRM__, CODE_FUNC_UNUSED  //25 //0F B6 8D 13 FF FF FF                          movzx   ecx, [ebp+var_ED] // DIK_R
};

__forceinline void
__make_asm_and_ebx_toreg32 (BYTE *opcode, BYTE dreg_coder)
{
  // 23 /r
  // AND r32,r/m32
  // mod 11 R/M 011 
  opcode[0] = 0x23; // AND 
  opcode[1] = 0;  // clear 
  opcode[1] |= 0x03; // make R/M
  opcode[1] |= 0xC0; // make mod
  opcode[1] |= (dreg_coder << 3); // make dreg_coder
}

__forceinline void 
__make_asm_movzx_ebp_offsetmem32_toreg32 (BYTE *buf_appect, 
                                          BYTE DIK_code_set, BYTE dreg_coder, int  total ) 

{ 
  INT id;
  //  0F B6 /r
  //  MOVZX r32,r/m8

  for (id = 0; id != total; id++)
    buf_appect[id] = 0x90;  // 填NOP
  buf_appect[0] = 0x0F;
  buf_appect[1] = 0xB6;
  // 测试偏移-
  if ( (0x100 - DIK_code_set) <= 0x80) {
    // 短偏移
    buf_appect[2] = 0x45;
    buf_appect[3] = DIK_code_set;
  } else {
    // 长偏移
    buf_appect[2] = 0x85;
    buf_appect[3] = DIK_code_set;
    buf_appect[4] = 0xFF;
    buf_appect[5] = 0xFF;
    buf_appect[6] = 0xFF;
  }
  // 构造指令完整ModR/M
  buf_appect[2] |= (dreg_coder << 3); // make dreg_coder
}

__forceinline void 
  th08_crack_keymap (BYTE *buf_appect, 
                     BYTE DIK_code_set, BYTE dreg_coder, int total_ ) 
{
  __make_asm_movzx_ebp_offsetmem32_toreg32 (buf_appect, DIK_code_set, dreg_coder, total_);
  __make_asm_and_ebx_toreg32 (& buf_appect[total_ - 2], dreg_coder);
}

// 读取缓存中的按键信息
__forceinline void 
  th08_loadcrack_keymap (BYTE *buf_appect, 
                     BYTE *DIK_code ) 
{
  // 很简单直接读取第四个字节就可以了, 碰巧就这么方便
  * DIK_code = buf_appect[3];
}

struct mapIDirectInputKey {
  TCHAR *ascii;
  BYTE DIK_code;
};

static const
struct mapIDirectInputKey combox_item [] 
= 
{ { _T ("A"), DIK_A }, // 0
  { _T ("B"), DIK_B }, // 1
  { _T ("C"), DIK_C }, // 2
  { _T ("D"), DIK_D }, // 3
  { _T ("E"), DIK_E }, // 4
  { _T ("F"), DIK_F }, // 5
  { _T ("G"), DIK_G }, // 6
  { _T ("H"), DIK_H }, // 7
  { _T ("L"), DIK_I }, // 8
  { _T ("J"), DIK_J }, // 9
  { _T ("K"), DIK_K }, // 10
  { _T ("L"), DIK_L }, // 11
  { _T ("M"), DIK_M }, // 12
  { _T ("N"), DIK_N }, // 13
  { _T ("O"), DIK_O }, // 14
  { _T ("P"), DIK_P }, // 15
  { _T ("Q"), DIK_Q }, // 16
  { _T ("R"), DIK_R }, // 17
  { _T ("S"), DIK_S }, // 18
  { _T ("T"), DIK_T }, // 19
  { _T ("U"), DIK_U }, // 20
  { _T ("V"), DIK_V }, // 21
  { _T ("W"), DIK_W }, // 22
  { _T ("X"), DIK_X }, // 23
  { _T ("Y"), DIK_Y }, // 24
  { _T ("Z"), DIK_Z }, // 25
  { _T ("space"), DIK_SPACE }, // 26
  { _T ("enter"), DIK_RETURN }, // 27
  { _T ("escape"), DIK_ESCAPE }, // 28
  { _T ("lsft"), DIK_LSHIFT }, // 29
  { _T ("rsft"), DIK_RSHIFT }, // 30
  { _T ("lctl"), DIK_LCONTROL }, // 31
  { _T ("rctl"), DIK_RCONTROL }, // 32
  { _T ("up"), DIK_UP }, // 33
  { _T ("down"), DIK_DOWN }, // 34
  { _T ("left"), DIK_LEFT }, // 35
  { _T ("right"), DIK_RIGHT } // 36
};

static 
struct th08_keyedit_ {
  // 控件/对话框窗口集合
  HWND wndIDD_main;
  HWND wndIDCC_USEDEFAULT;
  HWND wndIDCC_USELOAD;
  HWND wndIDCC_RESIZE;
  HWND wndIDCB_LOADPATH;
  HWND wndIDCC_DISWINKEY;
  HWND wndIDCE_STATUSOUT;
  HWND wndIDCL_KEYMAP;
  HWND wndIDCO_ATTATCH;
  // 程序的路径
  TCHAR path[MAX_PATH];
  //
  BOOL bCatchItem; 
  // 
  HANDLE fHandle; // 文件句柄, 用来操作游戏文件
  // 默认的回调函数
  WNDPROC wndcb_IDD_main;
  // 选中的条目
  int sel_codefunc;
  int sel_combobitem_id;
} gkeyedit;

void th08_init (void) {

  gkeyedit.bCatchItem = FALSE; 
  gkeyedit.fHandle = INVALID_HANDLE_VALUE;
  gkeyedit.sel_codefunc = CODE_FUNC_UNUSED;
  gkeyedit.sel_combobitem_id = -1;
}

TCHAR *
th08_getstring_base (BYTE DIK_code) {
  INT id;
  for ( id= 0; 
        id != sizeof (combox_item) / sizeof (combox_item[0]); 
        id++  )
  {
    if (combox_item[id].DIK_code == DIK_code)
      return combox_item[id].ascii;
    else ;
  }
  assert (0);
}

BYTE 
th08_getDIK_base (TCHAR *ascii) {
  INT id;
  for ( id= 0; 
    id != sizeof (combox_item) / sizeof (combox_item[0]); 
    id++  )
  {
    if (_tcscmp (ascii, combox_item[id].ascii) == 0)
      return combox_item[id].DIK_code;
    else ;
  }
  assert (0);
}
int
th08_getidx (BYTE DIK_code) {
  INT id;
  for ( id= 0; 
        id != sizeof (combox_item) / sizeof (combox_item[0]); 
        id++  )
  {
    if (combox_item[id].DIK_code == DIK_code)
      return id;
    else ;
  }
  return -1;
}

int 
th08_getidx2 (TCHAR *ascii) {
  INT id;
  for ( id= 0; 
    id != sizeof (combox_item) / sizeof (combox_item[0]); 
    id++  )
  {
    if (_tcscmp (ascii, combox_item[id].ascii) == 0)
      return id;
    else ;
  }
  return -1;
}

void 
th08_output (TCHAR *format, ...) {

  if (format != NULL) {
    TCHAR tmpbuf[2048];
    int vnums ;
    va_list args ;
    va_start (args, format) ;
    vnums = wvsprintfW (tmpbuf, format, args);
    va_end(args);
    if (vnums >= 0) {
      SendMessage (gkeyedit.wndIDCE_STATUSOUT, EM_SETSEL, -2, -1);
      SendMessage (gkeyedit.wndIDCE_STATUSOUT, EM_REPLACESEL, TRUE, (LPARAM)tmpbuf);
      /* set scroll tail */
      SendMessage (gkeyedit.wndIDCE_STATUSOUT, WM_VSCROLL, SB_BOTTOM, 0);
    }
  }
}

th08_setv (BYTE code_func, BYTE DIK_code) {
  TCHAR *setKeyMapr =th08_getstring_base (DIK_code);
  switch (code_func) {
  // dir 
  case CODE_FUNC_UP:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 0, 1, setKeyMapr); break;
  case CODE_FUNC_DOWN:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 1, 1, setKeyMapr); break;
  case CODE_FUNC_LEFT:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 2, 1, setKeyMapr); break;
  case CODE_FUNC_RIGHT:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 3, 1, setKeyMapr); break;
  // dmap 
  case CODE_FUNC_SHOOT:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 0, 3, setKeyMapr); break;
  case CODE_FUNC_SKILL:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 1, 3, setKeyMapr); break;
  case CODE_FUNC_QUIT:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 2, 3, setKeyMapr); break;
  case CODE_FUNC_ENTER:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 3, 3, setKeyMapr); break;
  // misc 
  case CODE_FUNC_SWITCH:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 0, 5, setKeyMapr); break;
  case CODE_FUNC_SWITCH2:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 1, 5, setKeyMapr); break;
  case CODE_FUNC_SKIP:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 2, 5, setKeyMapr); break;
  case CODE_FUNC_SKIP2:ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 3, 5, setKeyMapr); break;
  default: assert (0);
  }
  return 0;
}

// 主对话框的钩子回调用来截list控件的消息
// ---------------------------------------------------------------------------------------------
LRESULT 
CALLBACK cb_IDD_main_hook (HWND wnd, UINT msg, WPARAM param, LPARAM param2)
{
  switch (msg)
  {
  case WM_CLOSE:
    SetParent (gkeyedit.wndIDCO_ATTATCH, gkeyedit.wndIDD_main);
    break;
  case WM_NOTIFY:
    // 双击列表项目, 显示隐藏的combox控件, 把他放到我们点击的位置
    // 取消默认的选择效果
    {
      union {
        void *comp; // 兼容指针
        LPARAM comp2;
        NMHDR *base;
        NMLISTVIEW *list;
        NMLVCUSTOMDRAW *draw;
      } nmset;

      nmset.comp2 = param2;

      if (nmset.base->idFrom == IDC_LIST_KEYMAP && nmset.base->hwndFrom == gkeyedit.wndIDCL_KEYMAP) {
        if (nmset.base->code == NM_KILLFOCUS) {
          return 0;

        } else if (nmset.base->code == NM_DBLCLK) {
          // 双击消息, 矫正控件的位置
          RECT nsrt;
          TCHAR szBuffer[32];
          // 0 1 2 3 4 5 
          if (nmset.list->iItem != -1 &&  nmset.list->iSubItem != 0
                               && nmset.list->iSubItem != 2
                               && nmset.list->iSubItem != 4) {
            gkeyedit.bCatchItem = TRUE;
            ZeroMemory (szBuffer, sizeof (szBuffer));
            EnableWindow (gkeyedit.wndIDCO_ATTATCH, TRUE);
            ListView_GetSubItemRect (nmset.list->hdr.hwndFrom, nmset.list->iItem, nmset.list->iSubItem,
              LVIR_LABEL, & nsrt);
            gkeyedit.sel_codefunc = (nmset.list->iSubItem - 1)/ 2 * 4 + nmset.list->iItem; // XXX:枚举宏依赖
            ListView_GetItemText (nmset.list->hdr.hwndFrom, nmset.list->iItem, nmset.list->iSubItem
              , szBuffer, sizeof(szBuffer));
            gkeyedit. sel_combobitem_id = th08_getidx2(szBuffer);
            if (gkeyedit. sel_combobitem_id != -1)
              ComboBox_SetCurSel (gkeyedit.wndIDCO_ATTATCH, gkeyedit. sel_combobitem_id);
            else {
              SetWindowText (gkeyedit.wndIDCO_ATTATCH, _T(""));
              ComboBox_SetCurSel (gkeyedit.wndIDCO_ATTATCH, -1);
            }
            SetWindowPos (gkeyedit.wndIDCO_ATTATCH, HWND_BOTTOM, nsrt.left + 1, nsrt.top + 0
                , nsrt.right - nsrt.left - 1, nsrt.bottom - nsrt.top - 1, SWP_DRAWFRAME | SWP_SHOWWINDOW);
            SetFocus (gkeyedit.wndIDCO_ATTATCH);
            return 0;
          }
        } else if (nmset.base->code == NM_CUSTOMDRAW) {
            if (nmset.draw->nmcd.dwDrawStage == CDDS_PREPAINT)
              return CDRF_NOTIFYITEMDRAW;
            else if (nmset.draw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
              ListView_SetItemState (nmset.draw->nmcd.hdr.hwndFrom, -1, 0, LVIS_DROPHILITED | LVIS_SELECTED);
            return CDRF_DODEFAULT;
        }
      }
    }
  default:
    break;
  }
  // 调用默认的回调
  return CallWindowProc (gkeyedit.wndcb_IDD_main, wnd, msg, param, param2);
}

INT_PTR 
CALLBACK cbIDD_about (HWND wnd, UINT msg, WPARAM param, LPARAM param2)
{
  switch (msg) {
  case WM_COMMAND:
    switch (LOWORD (param)) {
    case IDCANCEL:
      EndDialog (wnd, 0);
      return (INT_PTR) TRUE; 
    default: break;
    }
  }
  return (INT_PTR) FALSE; 
}

INT_PTR 
CALLBACK cbIDD_main (HWND wnd, UINT msg, WPARAM param, LPARAM param2)
{
  WORD idCtrl;
  WORD id;
  WORD command;
  LV_COLUMN LVC;
  LVITEM LVI;
  TCHAR szBuffer[32];

  switch (msg) {
  case WM_COMMAND:
    idCtrl = LOWORD (param);
    command= HIWORD (param);
    switch (idCtrl) {
    case IDC_CHK_USEDEFAULT_STATUS:
    case IDC_CHK_USELOAD_STATUS:
      if (Button_GetCheck ((HWND) param2) == BST_CHECKED && (command == BN_CLICKED)) {
        if (gkeyedit.wndIDCC_USEDEFAULT == (HWND) param2)
          Button_SetCheck (gkeyedit.wndIDCC_USELOAD, BST_UNCHECKED);
        else 
          Button_SetCheck (gkeyedit.wndIDCC_USEDEFAULT, BST_UNCHECKED);
        return (INT_PTR) TRUE; 
      }
      break;
    case IDC_BUT_CONFIRM:
      // 修改确定.. 这段需要SEH异常处理 
      __try 
      {
        BYTE DIK_cache[12] = 
        {  DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT,
           DIK_Z, DIK_X, DIK_ESCAPE, DIK_RETURN, 
           DIK_LSHIFT, DIK_RSHIFT, DIK_LCONTROL , DIK_RCONTROL };
        BYTE coode_ = TH08_coope_level;
        DWORD style_ = TH08_window_style;
        DWORD tempRead;
        BYTE _code_0x48442 = 0xEB;
        BYTE _code_0x484B4[2] = { 0xE0, 0x00 };
        BYTE tenKeyCache[20];
        if (Button_GetCheck ((HWND) gkeyedit.wndIDCC_USELOAD) == BST_CHECKED)
          goto ___exit0;
        if (Button_GetCheck ((HWND) gkeyedit.wndIDCC_RESIZE) == BST_CHECKED)
          style_ |= WS_THICKFRAME;
        if (Button_GetCheck ((HWND) gkeyedit.wndIDCC_DISWINKEY) != BST_CHECKED)
          coode_ &= ~DISCL_NOWINKEY;
        if (gkeyedit.fHandle == INVALID_HANDLE_VALUE) {
          th08_output(_T("no set th08 file path\n") );
          goto ___exit0;
        } else if (Button_GetCheck ((HWND) gkeyedit.wndIDCC_USEDEFAULT) != BST_CHECKED) {
           // 扫描当前Item的文本,转化为DIK_code
          for (id = 0; id != 12; id++) {
            ZeroMemory (szBuffer, sizeof (szBuffer));
            ListView_GetItemText (gkeyedit.wndIDCL_KEYMAP, id % 4, (id >> 2) * 2 + 1, szBuffer, sizeof (szBuffer));
            if (szBuffer[0] == 0) {
              th08_output(_T("have noset's keymap\n") );
              return (INT_PTR) TRUE;
            }
            DIK_cache[id] = th08_getDIK_base(szBuffer);
          }
        }
        // 调包sub_43D970栈操作码
        SetFilePointer (    gkeyedit.fHandle, 
          sub_43D970_stacksave_addr - 0x43D970 + PE_sub_43D970_addr,   
          NULL, FILE_BEGIN ); // 建立栈
        WriteFile (gkeyedit.fHandle, sub_43D970_stacksave, sizeof (sub_43D970_stacksave), & tempRead, NULL);
        SetFilePointer (    gkeyedit.fHandle, 
          sub_43D970_stackresume_addr - 0x43D970 + PE_sub_43D970_addr,   
          NULL, FILE_BEGIN ); // 销毁栈
        WriteFile (gkeyedit.fHandle, sub_43D970_stackresume, sizeof (sub_43D970_stackresume), & tempRead, NULL);
        SetFilePointer (    gkeyedit.fHandle, 
          PE_sub_44858D_crack_0xEB_addr,   
          NULL, FILE_BEGIN ); // 验证JCC转JMP
        WriteFile (gkeyedit.fHandle, & _code_0x48442, sizeof (_code_0x48442), & tempRead, NULL);
        SetFilePointer (    gkeyedit.fHandle, 
          PE_sub_44858D_crack_0xE0_0x00_addr,   
          NULL, FILE_BEGIN ); // 返回值置0
        WriteFile (gkeyedit.fHandle, _code_0x484B4, sizeof (_code_0x484B4), & tempRead, NULL);
        SetFilePointer (    gkeyedit.fHandle, 
          PE_sub_446A37_resizeWindow_addr,     // iwindow拉申
          NULL, FILE_BEGIN );
        WriteFile (gkeyedit.fHandle, & style_, sizeof (style_), & tempRead, NULL);
        SetFilePointer (    gkeyedit.fHandle,  // Dinput协调级设置
          PE_sub_446A37_setcoope_addr,   
          NULL, FILE_BEGIN );
        WriteFile (gkeyedit.fHandle, & coode_, sizeof (coode_), & tempRead, NULL);
        // 扫描按键写入游戏文件 
        for (id = 0; id != 12; id++) {
          unsigned int *p =& sub_43D970_joypad_fast[id* 4]; // IDA offset/ModR/M/code func/total opcode
          th08_crack_keymap (tenKeyCache, DIK_cache[id], p[1], p[3]);
          // 写入文件
          SetFilePointer (    gkeyedit.fHandle, 
            * p - 0x43D970 + PE_sub_43D970_addr,   
            NULL, FILE_BEGIN ); 
          WriteFile (gkeyedit.fHandle, tenKeyCache, p[3], & tempRead, NULL);
        }
        FlushFileBuffers (gkeyedit.fHandle);
        goto ___exit0;
      }
      __except (EXCEPTION_EXECUTE_HANDLER )
      {
        th08_output(_T("WriteFile failed.....\n") );
      }
      return (INT_PTR) TRUE;
    case IDC_BUT_ABOUTPRO:
      //构造模态对话框
      DialogBox (GetModuleHandle (NULL), MAKEINTRESOURCE(IDD_ABOUT), wnd, cbIDD_about);
      return (INT_PTR) TRUE;
    case IDC_COMB_ATTACH:
      if (command == CBN_KILLFOCUS) {
        if (gkeyedit.bCatchItem == TRUE) {
          // 设置当前Item 
          ZeroMemory (szBuffer, sizeof (szBuffer));
          ComboBox_GetLBText (gkeyedit.wndIDCO_ATTATCH, ComboBox_GetCurSel (gkeyedit.wndIDCO_ATTATCH), szBuffer);
          if (szBuffer[0] != 0)
            th08_setv(gkeyedit.sel_codefunc, th08_getDIK_base(szBuffer));
          else ;
          ShowWindow(gkeyedit.wndIDCO_ATTATCH, SW_HIDE);
          return (INT_PTR) TRUE;
        }
      }
      break;
    case IDC_BUT_TH08PATH:
      // 读取东方永夜抄bin文件
      {
        OPENFILENAME ofn; 
        TCHAR file_name[MAX_PATH];
        ZeroMemory (& ofn, sizeof (ofn));
        ZeroMemory (& file_name[0], sizeof (file_name));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = NULL;
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = & file_name[0];
        ofn.nMaxFile = sizeof (file_name);
        ofn.lpstrInitialDir = NULL;
        ofn.lpstrTitle = _T ("please select file");
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetSaveFileName (& ofn) != FALSE) { 
          // 打开文件
          HANDLE t_handle;
          if ((t_handle = CreateFile (file_name,   
            GENERIC_WRITE | GENERIC_READ,
            0, 
            NULL,   
            OPEN_EXISTING,   
            FILE_ATTRIBUTE_NORMAL, 
            NULL))  == INVALID_HANDLE_VALUE) {
             th08_output(_T("open file fail path:%s"),file_name );
          } else {
            CloseHandle (gkeyedit.fHandle);
            gkeyedit.fHandle = t_handle;
            th08_output(_T("open file success path:%s"),file_name );
          } 
        } else MessageBox(NULL, _T ("please select one file"), NULL, MB_ICONERROR); 
        return (INT_PTR)TRUE;
      }
      break;
    case IDCANCEL:
    case IDC_BUT_CANCEL:
___exit0:
      EndDialog (wnd, 0);
      return (INT_PTR) TRUE; 
    default: break;
    }
    break;
  case WM_INITDIALOG:
    // 对话框初始化, 获得控件窗口,设置控件子类化回调
    gkeyedit.wndIDD_main = wnd;
    gkeyedit.wndIDCC_USEDEFAULT = GetDlgItem (wnd, IDC_CHK_USEDEFAULT_STATUS);
    gkeyedit.wndIDCC_USELOAD = GetDlgItem (wnd, IDC_CHK_USELOAD_STATUS);
    gkeyedit.wndIDCC_DISWINKEY = GetDlgItem (wnd, IDC_CHK_DISWINKEY);
    gkeyedit.wndIDCE_STATUSOUT = GetDlgItem (wnd, IDC_EDIT_STATUSOUT);
    gkeyedit.wndIDCL_KEYMAP = GetDlgItem (wnd, IDC_LIST_KEYMAP);
    gkeyedit.wndIDCO_ATTATCH = GetDlgItem (wnd, IDC_COMB_ATTACH);
    gkeyedit.wndIDCC_RESIZE = GetDlgItem (wnd, IDC_CHK_RESIZEWINDOW);
    // 往combox控件扔条目
    for (id = 0; id != sizeof (combox_item) / sizeof (combox_item[0]); id++)
    {
      ComboBox_AddItemData (gkeyedit.wndIDCO_ATTATCH, combox_item[id].ascii);
    }

    // 使Combox控件附着于List控件方便定位坐标
    SetParent (gkeyedit.wndIDCO_ATTATCH, gkeyedit.wndIDCL_KEYMAP);

    // 设置列表控件网格样式
    ListView_SetExtendedListViewStyle (                 
                            gkeyedit.wndIDCL_KEYMAP, 
            ListView_GetExtendedListViewStyle 
                ( gkeyedit.wndIDCL_KEYMAP) | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    // 插入条目 dir mapper dmap mapper misc mapper 
    LVC.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT; 
    LVC.pszText = _T ("dir");        
    LVC.fmt = LVCFMT_LEFT;
    LVC.cx = 36;
    LVC.iSubItem = 0; 
    ListView_InsertColumn (gkeyedit.wndIDCL_KEYMAP, 0, & LVC); 
    LVC.cx = 38;
    LVC.pszText = _T ("map");   
    LVC.iSubItem = 1;
    ListView_InsertColumn (gkeyedit.wndIDCL_KEYMAP, 1, & LVC);
    LVC.cx = 39;
    LVC.pszText = _T ("act");   
    LVC.iSubItem = 2;
    ListView_InsertColumn (gkeyedit.wndIDCL_KEYMAP, 2, & LVC);
    LVC.cx = 40;
    LVC.pszText = _T ("map");   
    LVC.iSubItem = 3;
    ListView_InsertColumn (gkeyedit.wndIDCL_KEYMAP, 3, & LVC); 
    LVC.cx = 50;
    LVC.pszText = _T ("auxi");   
    LVC.iSubItem = 4;
    ListView_InsertColumn (gkeyedit.wndIDCL_KEYMAP, 4, & LVC); 
    LVC.cx = 40;
    LVC.pszText = _T ("map");    
    LVC.iSubItem = 5;
    ListView_InsertColumn (gkeyedit.wndIDCL_KEYMAP, 5, & LVC);
    // 插入四行
    LVI.mask = LVIF_TEXT;
    LVI.iItem = 0;
    LVI.iSubItem = 0;
    LVI.pszText = _T ("right"); 
    ListView_InsertItem (gkeyedit.wndIDCL_KEYMAP, & LVI);
    LVI.pszText = _T ("left");
    ListView_InsertItem (gkeyedit.wndIDCL_KEYMAP, & LVI);
    LVI.pszText = _T ("down");
    ListView_InsertItem (gkeyedit.wndIDCL_KEYMAP, & LVI);
    LVI.pszText = _T ("up");
    ListView_InsertItem (gkeyedit.wndIDCL_KEYMAP, & LVI);
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 0, 2, _T ("shoot"));
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 1, 2, _T ("skill")); 
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 2, 2, _T ("quit")); 
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 3, 2, _T ("ok")); 
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 0, 4, _T ("switch"));
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 1, 4, _T ("switch2"));
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 2, 4, _T ("skip"));
    ListView_SetItemText (gkeyedit.wndIDCL_KEYMAP, 3, 4, _T ("skip2"));
    // 挂个钩子对list控件的WM_NOTIFY消息进行处理 
    gkeyedit.wndcb_IDD_main = (void *)SetWindowLong (gkeyedit.wndIDD_main, GWL_WNDPROC, (LONG)cb_IDD_main_hook);
    return (INT_PTR) TRUE;
  default: break;
  }
  return (INT_PTR) FALSE;
}

// 语法上的程序入口点
int WINAPI _tWinMain ( HINSTANCE instance_cur, // 当前实例
                       HINSTANCE instance_prev,// 之前实例
                       LPTSTR cmd_line, // 命令行
                       int cmd_show // 如何显示
                     )

{
  th08_init ();

  gkeyedit.bCatchItem = FALSE;
  // 构造程序对话框
  DialogBox (instance_cur, MAKEINTRESOURCE (IDD_MAIN), NULL, cbIDD_main);
  // 执行程序的析构工作, 然后退出
  CloseHandle (gkeyedit.fHandle);
  return 0;
}

# if defined (_DEBUG)
 int main (void)
 {
   _tWinMain (GetModuleHandle (NULL), NULL, NULL, SW_SHOWNORMAL);
   return 0;
 }
# endif 

