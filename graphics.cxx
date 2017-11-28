#include "d3dx9.h"
#include <Windows.h>
#include <list>
#include <assert.h>
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <process.h>
#include <stdarg.h>
#include "futex.h"


extern "C" {

//  线程私有数据结构 
struct   ESG_thlocal {

  IDirect3DDevice9 *  graphics_desc; // IDirect3DDevice9, 线程绘图渲染对象 
  IDirect3DSurface9 * offscreen_surface; // 离屏表面surface, 非显存, 使用系统内存
  D3DPRESENT_PARAMETERS     d3dpp;  //  设备描述 

  HFONT   cur_font;                // 字体
  HGDIOBJ old_devobj;               // 被选出来的设备对象

  HWND    window_;        //  windows下用户级图形编程最重要的根对象, 窗口句柄
  futex_t   lock;  // 线程私有互斥访问锁 

  VOID **hostid;          //  宿主线程的TID
  VOID **selfid;          //  自身使用的TID 

  HANDLE handle;        //  线程句柄

  HANDLE evt_handle;      //  通知句柄

  int attres;

  BOOL self_quit;   

  BYTE dirtyArray[sizeof(RGNDATAHEADER) + sizeof(RECT)];    //  脏矩形测试使用

  RECT *dirtyRegion;

  VOID  **hostattach;

  struct _image curoff;
};

//  全局共享数据结构
struct   _ESG_shlocal {

  // FILE  *log;
  volatile  LONG attach;

  IDirect3D9 *graphics_basis;   // IDirect3D9对象, 一个应用程序只需要初始化一次 
} _ESG_shlocal  = { 0, nullptr };
//  TLS对象当前链接的渲染对象
__declspec(thread) ESG_thlocal *curthlocal = nullptr;
//  TLS对象当前线程的TID
__declspec(thread) VOID *curthtid = nullptr;
// 
_CRT_ALIGN(32)  int  ess_init =0;
// 
_es_lib *gvlib;


#if defined (_DEBUG)
#  define DEBUGOUT printf 
#  define FT_MALLOC FT_malloc0
#  define FT_RELEASE FT_free0
#else 
#  define DEBUGOUT(const char *format, ...) void 
#  define FT_MALLOC(num, const char *format, ...) malloc (num)
#  define FT_RELEASE(ptr, const char *format, ...) free (ptr)
#endif 

void *__cdecl FT_malloc0 (size_t num, const char *format, ...) {

  char strtemp[1024];
  int slen;

  va_list args; 
  va_start(args, format); // !!!! 
  vsprintf(strtemp, format, args); 
  va_end(args); 
  slen = strlen (strtemp);

  printf (strtemp);
  return _aligned_malloc (num, 32);
}

void __cdecl FT_free0 (void *ptr, const char *format, ...) {

  char strtemp[1024];
  int slen;

  va_list args; 
  va_start(args, format); // !!!! 
  vsprintf(strtemp, format, args); 
  va_end(args); 
  slen = strlen (strtemp);

  printf (strtemp);
  _aligned_free (ptr);
}

// 全局窗口风格 
const DWORD dsstyle = WS_OVERLAPPEDWINDOW;// &~WS_SYSMENU;

LOGFONTA default_font = {
  16, 7, 0, 0, 700, 0, 0, 0, 134, 1, 2, 2, 34,
  {   0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0}
};

void
vemcpy2  
( pixel_t *target, int target_x, int target_y, int target_pitch,
  pixel_t *source, int source_x, int source_y, int source_pitch, int width, int height)
{
  pixel_t *target_ = (pixel_t *) target;
  pixel_t *source_ = (pixel_t *) source;

  int target_pitch_ = target_pitch / 4;
  int source_pitch_ = source_pitch / 4;
  int  x;
  int  y;

  target_ = & target_[target_y * target_pitch_ + target_x];
  source_ = & source_[source_y * source_pitch_ + source_x];


  for (y= 0; y != height; y++) {

    for (x= 0; x != width; x++)  {

      target_[x+y*target_pitch_] = source_[x+y*source_pitch_] & 0xFFFFFF;
    }
  }
}

void
vemcpy3  /* XXX: only for x86-32 or WOW64, so use int */
( pixel_t *target, int target_x, int target_y, int target_pitch,
  pixel_t *source, int source_x, int source_y, int source_pitch, int width, int height, pixel_t mask)
{
  pixel_t *target_ = (pixel_t *) target;
  pixel_t *source_ = (pixel_t *) source;
  int target_pitch_ = target_pitch / 4;
  int source_pitch_ = source_pitch / 4;
  int  x;
  int  y;

  target_ = & target_[target_y * target_pitch_ + target_x];
  source_ = & source_[source_y * source_pitch_ + source_x];


  for (y= 0; y != height; y++) {

    for (x= 0; x != width; x++) {

      pixel_t tmp = source_[x+y*source_pitch_] & 0xFFFFFF;

      if ( tmp != (mask & 0xFFFFFF)) {
        target_[x+y*target_pitch_] = tmp;
      }

    }
  }

}

 
__forceinline 
pixel_t alphamix (const pixel_t data, const uint8_t purity) {
  
  pixel_t x, y, z;
 
  x = data & 0x000000FF; //
  y = data & 0x0000FF00; //
  z = data & 0x00FF0000; // limit range 
    
  x = ((x * purity) >> 7) & 0x000000FF; //
  y = ((y * purity) >> 7) & 0x0000FF00; //
  z = ((z * purity) >> 7) & 0x00FF0000; // strong reduce | FIXME:div/shift
    
  return (x | y | z);  
}

__forceinline  
pixel_t saddudt (const pixel_t d0, const pixel_t d2) {
  
  pixel_t x, y, z;
  
  x = (d0 & 0x000000FF) + (d2 & 0x000000FF); //
  y = (d0 & 0x0000FF00) + (d2 & 0x0000FF00); //
  z = (d0 & 0x00FF0000) + (d2 & 0x00FF0000); // cross add 
  
  if (x > 0x000000FF) x = 0x000000FF; //
  if (y > 0x0000FF00) y = 0x0000FF00; //
  if (z > 0x00FF0000) z = 0x00FF0000; // limit range 
    
  return (x | y | z);  
}

/* 
 * alpha double
 */  
__forceinline 
pixel_t alphamixd (pixel_t ag, pixel_t ag2, uint8_t ag_s, 
         uint8_t ag_s2) 
{
  return saddudt ( alphamix (ag, ag_s), 
                   alphamix (ag2, ag_s2));  
}


void
vemcpy4  
( pixel_t *target, int target_x, int target_y, int target_pitch,
  pixel_t *source, int source_x, int source_y, int source_pitch, int width, int height, uint8_t sap,
                         uint8_t dap )
{
  pixel_t *target_ = (pixel_t *) target;
  pixel_t *source_ = (pixel_t *) source;
  int target_pitch_ = target_pitch / 4;
  int source_pitch_ = source_pitch / 4;
  int  x;
  int  y;

  target_ = & target_[target_y * target_pitch_ + target_x];
  source_ = & source_[source_y * source_pitch_ + source_x];


  for (y= 0; y != height; y++)
  {
      for (x= 0; x != width; x++)
  {
target_[x+y*target_pitch_] =
  alphamixd (source_[x+y*source_pitch_], target_[x+y*target_pitch_],
    sap, dap);
  }
  }

}

int _POW2 (int n) {
  int S; for (S = 1; S < n; S *= 2) {} 
  return S; 
}

void *_ptralign (void *scp, size_t align_) {
  // align address
  char *vmp = (char *)scp;
  size_t ofs = (size_t) vmp;
  size_t ofsi= align_ - ofs;
  // MEM:0xFFFFFFFF, ALIGN:16.ADD:1   R:17
  // MEM:0x00000001, ALIGN:16.ADD:15  R:15 
  return (void *) & vmp [ofsi % align_];
}

ES_EXPORT
int essets (void) {

  if ( _InterlockedCompareExchange ((long *)& ess_init, 1, 0) == 0) {
    _ESG_shlocal.graphics_basis = Direct3DCreate9 (D3D_SDK_VERSION);
   assert (_ESG_shlocal.graphics_basis != nullptr);
   CoInitialize(nullptr);
   return 0;
  }
  return -1;
}

ES_EXPORT
int essesf (void) {

  if ( _InterlockedCompareExchange ((long *)& ess_init, -1, 1) == 1) {
    if ( _ESG_shlocal.graphics_basis != nullptr) {
      while (_ESG_shlocal.attach != 0) ;
      HRESULT hr = _ESG_shlocal.graphics_basis->Release ();
    assert (hr == S_OK);
       CoUninitialize();
    }
   return 0;
  }
  return -1;
}

LRESULT 
CALLBACK win32_aglcallback (HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {

  switch (msg) {

  case WM_DESTROY:

    PostQuitMessage(0);
    
    break;

  default:
    break;
  }

  return DefWindowProc (window, msg, wparam, lparam);
}

static 
int resize_client (HWND window, int width, int height, 
                   DWORD style) { 
  RECT wc;
  int x, y, xs, ys;
  if (!GetClientRect (window, &wc)) 
      return -1;
  
  wc.right = width;
  wc.bottom = height;
  
  if (!AdjustWindowRectEx (&wc, style, FALSE, 0))
	  return -1;
  
  x = wc.right - wc.left;
  y = wc.bottom - wc.top;
  xs = (GetSystemMetrics (SM_CXSCREEN) - x) >> 1;
  ys = (GetSystemMetrics (SM_CYSCREEN) - y) >> 1;

  return !SetWindowPos (window, NULL, xs, ys, x, y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
}


static  
es_status ES_API ESG_backtoforge0 (bool full, int x, int y, int width, int height) {
  
  __try {
    es_status es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }

  { // 填充后备缓冲像素, 先锁定后备缓冲 
    IDirect3DSurface9 *masurface;

    curthlocal->graphics_desc->GetBackBuffer (0, 0, D3DBACKBUFFER_TYPE_MONO, & masurface);
#      if   1
    //  make RECT/POINT 
    POINT pt = { x, y };
    RECT rt = { x, y, x + width, y + height };
    curthlocal->graphics_desc->UpdateSurface (curthlocal->offscreen_surface,
      full ? nullptr : & rt, masurface, full ? nullptr : & pt);
#      else 
    curthlocal->graphics_desc->StretchRect (curthlocal->offscreen_surface,
      nullptr, masurface, nullptr, D3DTEXF_NONE);
#      endif 

    masurface->Release ();

    futex_unlock (curthlocal->lock);
    return ES_OK;
  }
}


ES_EXPORT 
es_status ES_API ESG_backtoforge (int x, int y, int width, int height) {
  
  return ESG_backtoforge0 (false, x, y, width, height);
}

ES_EXPORT 
es_status ES_API ESG_backtoforgeall (void) {
  
  return ESG_backtoforge0 (true, 0, 0, 0, 0);
}

static 
es_status ES_API ESG_post0 (bool full, int x, int y, int width, int height) {
  
  __try {
    es_status es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }

  // XXX:设备丢失
  // 绘制脏矩形

  curthlocal->dirtyRegion->left = x;
  curthlocal->dirtyRegion->top = y;
  curthlocal->dirtyRegion->right = x + width;
  curthlocal->dirtyRegion->bottom = y + height;
       
  //  XXX:  设备丢失
  HRESULT rest = curthlocal->graphics_desc->Present (nullptr, nullptr, nullptr, full ? nullptr :(RGNDATA *) curthlocal->dirtyArray);
  // assert (rest == S_OK);

  futex_unlock (curthlocal->lock);
  return ES_OK;
}

ES_EXPORT 
es_status ES_API ESG_post (int x, int y, int width, int height) {
  
  return ESG_post0 (false, x, y, width, height);
}

ES_EXPORT 
es_status ES_API ESG_postall (void) {
  
  return ESG_post0 (true, 0, 0, 0, 0);
}


ES_EXPORT 
struct _image * ES_API ESG_lockback (es_status *status) {
  
  __try {
    es_status es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) {
      __try {
        if (status != nullptr) {
          *status = es;
        }
      } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
        : EXCEPTION_CONTINUE_SEARCH) {
          futex_unlock (curthlocal->lock);
          return nullptr;
      }
      return nullptr;
    }
    
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return nullptr;
  }

  //  XXX:  
  D3DLOCKED_RECT tp_desc;
  HRESULT rest = curthlocal->offscreen_surface->LockRect ( & tp_desc, nullptr, D3DLOCK_DISCARD | D3DLOCK_DONOTWAIT);
  if  (SUCCEEDED (rest)) {

    curthlocal->curoff.height = curthlocal->d3dpp.BackBufferHeight;
    curthlocal->curoff.width = curthlocal->d3dpp.BackBufferWidth;
    curthlocal->curoff.pitch = tp_desc.Pitch;
    curthlocal->curoff.pointer = (pixel_t *) tp_desc.pBits;

    if (status != nullptr) {
      *status = ES_OK;
    } 
    return & curthlocal->curoff;
  }
  if (status != nullptr) {
    *status = ES_FAIL;
  } 
  futex_unlock (curthlocal->lock);
  return nullptr;
}

ES_EXPORT 
es_status ES_API ESG_unlockback (void) {

  __try {
    HRESULT rest = curthlocal->offscreen_surface->UnlockRect ( );
    if  (SUCCEEDED (rest)) {
      return futex_unlock (curthlocal->lock);    
    } else {
      return ES_FAIL;
    }
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}
BOOL new_window (struct   ESG_thlocal *esg_eh, int width, int height) {

  TCHAR class_prefix[] =_T("lOcKbUfFeR'sDeMo0123456701234567");

  //  XXX:也许获取的时间在不同的线程会相同..
  //  随机窗口类名            
  _stprintf (& class_prefix[16], _T("%016I64X"), __rdtsc ());

  assert (width >50);
  assert (height>50);

  WNDCLASS wndclass;
  ZeroMemory (& wndclass, sizeof (wndclass));
  ZeroMemory (& esg_eh->d3dpp, sizeof (esg_eh->d3dpp));

  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = win32_aglcallback;
  wndclass.hInstance = GetModuleHandle (nullptr);
  wndclass.hIcon = LoadIcon (nullptr, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor (nullptr, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
  wndclass.lpszClassName = & class_prefix[0] ;

  if ( !RegisterClass (& wndclass)) {
    return FALSE;
  }

  // 创建窗口 
  esg_eh->window_ = CreateWindow ( wndclass.lpszClassName,
                                _T ("lOcKbUfFeR'sDeMo"),
                                dsstyle,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT, 
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                nullptr,
                                nullptr, 
                                wndclass.hInstance, 
                                nullptr);

  assert ( esg_eh->window_ != nullptr);
  // 调整客户区域尺寸 
  resize_client (esg_eh->window_, width, height, dsstyle);

  // 创建direct3d 绘图设备 
  esg_eh->d3dpp.BackBufferFormat     = D3DFMT_X8R8G8B8; // RGB24 后备缓冲
  esg_eh->d3dpp.SwapEffect           = D3DSWAPEFFECT_COPY; // 拷贝模式
  esg_eh->d3dpp.hDeviceWindow        = esg_eh->window_;
  esg_eh->d3dpp.Windowed             = TRUE;  // 窗口化
  esg_eh->d3dpp.BackBufferHeight     = height;  
  esg_eh->d3dpp.BackBufferWidth      = width;

  HRESULT hr = _ESG_shlocal.graphics_basis->CreateDevice (0, D3DDEVTYPE_HAL, 
         esg_eh->window_, D3DCREATE_SOFTWARE_VERTEXPROCESSING, & esg_eh->d3dpp, & esg_eh->graphics_desc); 
  assert (hr == S_OK);

  // 创建离屏表面[系统内存] 
  hr = esg_eh->graphics_desc->CreateOffscreenPlainSurface ( 
                          esg_eh->d3dpp.BackBufferWidth,
                          esg_eh->d3dpp.BackBufferHeight, 
                          D3DFMT_X8R8G8B8,
                          D3DPOOL_SYSTEMMEM, 
                          & esg_eh->offscreen_surface, nullptr);
  assert (hr == S_OK);
  return TRUE;
}

unsigned int __stdcall es_aglcallback (void *param ) {

  // 因为windows的消息循环独立于线程
  // 所以创建窗口/图形渲染对象需要在各自的新的线程里建立
  ESG_thlocal *p = (ESG_thlocal *) param;
  p->selfid = & curthtid;

  //创建新的窗口和渲染设备 
  BOOL ret =new_window (p, p->d3dpp.BackBufferWidth, p->d3dpp.BackBufferHeight);
  assert (ret != FALSE);

  p->attres = 0;

  //通知宿主线程
  SetEvent (p->evt_handle);
  //等待直到线程
  futex_lock (p->lock);
  //解锁线程
  futex_unlock (p->lock);

  //进入windows消息循环
  ShowWindow (p->window_, SW_SHOWNORMAL);

  MSG msg;
  ZeroMemory (& msg, sizeof (msg));

  while ( GetMessage (& msg, nullptr, 0, 0)) { 
    TranslateMessage (& msg); 
    DispatchMessage (& msg); 
  } 
  // 销毁当前渲染线程
  if (p->self_quit != FALSE) {
    // 自身要求的退出请求
    int ret =futex_lock (p->lock);
    assert (ret == 0);

    * p->hostattach = nullptr;

    // 摧毁direct3d 设备
    p->offscreen_surface->Release ();
    p->graphics_desc->Release ();

    // 摧毁当前渲染线程句柄
    CloseHandle (p->handle);

    if (p->cur_font != nullptr) {
      // 删除之前设置的字体
      DeleteObject (p->cur_font);
    }

    //摧毁私有线程锁
    futex_destroy (p->lock);

    InterlockedDecrement (& _ESG_shlocal.attach);
    DEBUGOUT ("ESG_terminate<notself> success...%p\n", p);

    

    __try {
      free ( p);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
      return msg.wParam ;
    }
  } 
  
  return msg.wParam ;
}

ES_EXPORT 
es_status ES_API ESG_init (int width, int height) {
  
  assert (width >50);
  assert (height>50);

  __try {
    

    if ( InterlockedCompareExchangePointer ((void **)& curthlocal, nullptr, nullptr) == nullptr) {

      // 初始化当前设备
      ESG_thlocal *p = (ESG_thlocal *) malloc (sizeof ( ESG_thlocal));
      assert (p != nullptr);

      p->hostid = & curthtid;
      p->evt_handle = CreateEvent (NULL, TRUE, FALSE, NULL);
      p->d3dpp.BackBufferHeight = height;
      p->d3dpp.BackBufferWidth = width;
      p->old_devobj = nullptr;
      p->self_quit = TRUE;
      p->hostattach = (void **) & curthlocal;
      p->cur_font = nullptr;

      LPRGNDATA cliplist = (LPRGNDATA) & p->dirtyArray[0];

      static RECT   boundrect = { 0, 0, 0, 0};

      boundrect.right = GetSystemMetrics (SM_CXSCREEN);
      boundrect.bottom= GetSystemMetrics (SM_CYSCREEN);

      cliplist->rdh.dwSize = sizeof(RGNDATAHEADER); 
      cliplist->rdh.iType = RDH_RECTANGLES; 
      cliplist->rdh.nCount = 1;
      cliplist->rdh.nRgnSize = sizeof(RECT); 
      cliplist->rdh.rcBound = boundrect; 

      p-> dirtyRegion = (RECT *) & p->dirtyArray[sizeof (RGNDATAHEADER)];

      memcpy ( (PVOID )& cliplist->Buffer[0], 
               (PVOID )p->dirtyRegion, sizeof (RECT));

      // 初始化渲染线程私有锁
      futex_init (& p->lock);
      futex_lock (  p->lock);

      // 创建新的线程
      p->handle = (HANDLE) _beginthreadex (nullptr, 0, es_aglcallback, p, 0, nullptr);

      // 等待线程初始化信号
      DWORD ret =WaitForSingleObject (p->evt_handle, INFINITE);
      futex_unlock (p->lock);

      switch (ret) {
      case WAIT_OBJECT_0:
        if (p->attres != 0)
      default:
        assert (0);
        else 
          // 关闭事件句柄
          CloseHandle (p->evt_handle);
          break;
      }
      curthlocal = p;

      DEBUGOUT ("ESG_init success objptr:%p...\n", p);
      InterlockedIncrement (& _ESG_shlocal.attach);

      return ES_OK;
    }
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    assert (0);
    return ES_BADMEM;
  }
  return ES_EXIST;
}

ES_EXPORT 
es_status ES_API ESG_terminate (void) {
  
  __try {

    if ( InterlockedCompareExchangePointer ((void **)& curthlocal, nullptr, nullptr) != nullptr) {

      // 只允许宿主线程提出退出请求
      if (curthlocal->hostid == & curthtid) {

        futex_lock (curthlocal->lock);

        curthlocal->self_quit = FALSE;

        PostMessage (curthlocal->window_, WM_DESTROY, 0, 0);

        // 等待线程结束
        WaitForSingleObject (curthlocal->handle, INFINITE);

        // 摧毁direct3d 设备
        curthlocal->offscreen_surface->Release ();
        curthlocal->graphics_desc->Release ();

        // 摧毁当前渲染线程句柄
        CloseHandle (curthlocal->handle);

        if (curthlocal->cur_font != nullptr) {
          // 删除之前设置的字体
          DeleteObject (curthlocal->cur_font);
        }

        //摧毁私有线程锁
        futex_destroy (curthlocal->lock);

        free (curthlocal);

        DEBUGOUT ("ESG_terminate<self> success...%p\n", curthlocal);
        // empty it. 
        curthlocal = nullptr;
        InterlockedDecrement (& _ESG_shlocal.attach);
        return ES_OK;
      } 
    }
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    return ES_BADMEM;
  }
  return ES_NOEXIST;
}




ES_EXPORT 
void *ES_API ESG_getattach (void) {
  
  return curthlocal;
}

ES_EXPORT 
void ES_API ESG_setattach (void *attach) {
  
  curthlocal = (ESG_thlocal *)attach;
}

ES_EXPORT 
struct _image * ES_API ESG_dummyimg (int width, int height) {

  // 不触碰关键数据, 不需要枷锁 
  int pitch = _POW2 (width) *4;
  pixel_t *pi = (pixel_t *)malloc (pitch * height);

  if ( pi == nullptr) {
    return nullptr;
  }

  struct _image *pig = (struct _image *) _aligned_malloc (sizeof (struct _image), 32);
  assert (pig != nullptr);

  pig->width = width;
  pig->height = height;
  pig->pitch = pitch;
  pig->pointer = pi;

  return pig;
}

ES_EXPORT 
es_status ES_API ESG_fill (int x, int y, int width, int height, pixel_t colour) {

  es_status es;

  __try {
    es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  } __try {
    D3DLOCKED_RECT  tp_desc;

    // 填充后备缓冲像素, 先锁定后备缓冲 
    curthlocal->offscreen_surface->LockRect (& tp_desc, nullptr, D3DLOCK_DISCARD | D3DLOCK_DONOTWAIT);
    pixel_t *pt_start = & ((pixel_t *) tp_desc.pBits)[x +y*tp_desc.Pitch/4];

    // 绘图, 此处可能出现非法访问内存
    for  ( int IZ = 0; IZ != height; IZ++) {

      for  ( int IA = 0; IA != width; IA++) {
        //画点
        pt_start[IA+IZ*tp_desc.Pitch/4] = colour & 0x00FFFFFF;
      }
    }
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    es = ES_BADMEM;
  }
  curthlocal->offscreen_surface->UnlockRect ();

  futex_unlock (curthlocal->lock);
  return es;
}

ES_EXPORT 
es_status ES_API ESG_fillall (pixel_t colour) {

  es_status es;

  __try {
    es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  } __try {
    D3DLOCKED_RECT  tp_desc;

    // 填充后备缓冲像素, 先锁定后备缓冲 
    curthlocal->offscreen_surface->LockRect (& tp_desc, nullptr, D3DLOCK_DISCARD | D3DLOCK_DONOTWAIT);
    pixel_t *pt_start = & ((pixel_t *) tp_desc.pBits)[0];

    // 绘图, 此处可能出现非法访问内存
    for  ( int IZ = 0; IZ != curthlocal->d3dpp.BackBufferHeight; IZ++) {

      for  ( int IA = 0; IA != curthlocal->d3dpp.BackBufferWidth; IA++) {
        //画点
        pt_start[IA+IZ*tp_desc.Pitch/4] = colour & 0x00FFFFFF;
      }
    }
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    es = ES_BADMEM;
  }
  curthlocal->offscreen_surface->UnlockRect ();

  futex_unlock (curthlocal->lock);
  return es;
}

ES_EXPORT 
es_status ES_API ESG_point (int x, int y,  pixel_t colour) {
  
  es_status es;

  __try {
    es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  } __try {
    D3DLOCKED_RECT  tp_desc;

    // 填充后备缓冲像素, 先锁定后备缓冲 
    curthlocal->offscreen_surface->LockRect (& tp_desc, nullptr, D3DLOCK_DISCARD | D3DLOCK_DONOTWAIT);
   * & ((pixel_t *) tp_desc.pBits)[x +y*tp_desc.Pitch/4] = colour & 0xFFFFFF;

  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      es = ES_BADMEM;
  }
  curthlocal->offscreen_surface->UnlockRect ();

  futex_unlock (curthlocal->lock);
  return es;
}

// 读取纹理 
ES_EXPORT
struct _image *ES_API ESG_loadimg (const char *filename) {

  __try {
    es_status es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return nullptr;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return nullptr;
  }
  {
    IDirect3DTexture9 *tex;
    D3DXIMAGE_INFO infos;

    if (D3DXCreateTextureFromFileExA (curthlocal->graphics_desc, filename, 
      D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
      D3DX_DEFAULT, // D3DX_DEFAULT 
      D3DX_DEFAULT, // D3DX_DEFAULT 
      0, //D3DCOLOR_XRGB (0, 0, 0),  
      & infos, nullptr, & tex) == S_OK)
    {
      D3DLOCKED_RECT Vdesc;
      HRESULT hr = S_OK;
      _image Gdesc;

      // lock it 
      hr = tex->LockRect (0, & Vdesc, nullptr, D3DLOCK_READONLY);
      assert (hr == S_OK);

      // calc/alloc backbuffer  

      Gdesc.width = infos.Width;
      Gdesc.height = infos.Height;
      Gdesc.pitch = _POW2 (Gdesc.width) * 4; // step 4 * RGB-depth
      Gdesc.pointer = (pixel_t *) _aligned_malloc (Gdesc.pitch *Gdesc.height, 64);

      // 拷贝至新的缓冲
      vemcpy2 (Gdesc.pointer, 0, 0, Gdesc.pitch, 
        (pixel_t *) Vdesc.pBits, 0, 0, Vdesc.Pitch, Gdesc.width , Gdesc.height);

      // unlock., release.
      tex->UnlockRect(0);
      tex->Release();

      _image *chunk = new _image;
      *chunk = Gdesc;

      futex_unlock (curthlocal->lock);
      return chunk;
    }
    else 
    {
      futex_unlock (curthlocal->lock);
      return nullptr;
    }
  }
}

//销毁纹理
ES_EXPORT
void ES_API ESG_freeimg (struct _image *img) {

  __try {
    _aligned_free (img->pointer);
    delete img;
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    return ;
  }
}

ES_EXPORT
es_status ES_API ESG_blit (struct _image *img, int x_dest, int y_dest, int x_src, int y_src, int width, int height) {

  es_status es;

  __try {
    es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  } __try {
    D3DLOCKED_RECT  tp_desc;

    // 填充后备缓冲像素, 先锁定后备缓冲 
    curthlocal->offscreen_surface->LockRect (& tp_desc, nullptr, D3DLOCK_DISCARD | D3DLOCK_DONOTWAIT);

    vemcpy2 ((pixel_t *)tp_desc.pBits, x_dest, y_dest, tp_desc.Pitch, 
      (pixel_t *) img->pointer, x_src, y_src, img->pitch, width , height);

  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      es = ES_BADMEM;
  }
  curthlocal->offscreen_surface->UnlockRect ();

  futex_unlock (curthlocal->lock);
  return es;
}

ES_EXPORT
es_status ES_API ESG_blit_shield (struct _image *img, int x_dest, int y_dest, int x_src, int y_src, int width, int height, pixel_t shiled_colour) {

  es_status es;

  __try {
    es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  } __try {
    D3DLOCKED_RECT  tp_desc;

    // 填充后备缓冲像素, 先锁定后备缓冲 
    curthlocal->offscreen_surface->LockRect (& tp_desc, nullptr, D3DLOCK_DISCARD | D3DLOCK_DONOTWAIT);

    vemcpy3 ((pixel_t *)tp_desc.pBits, x_dest, y_dest, tp_desc.Pitch, 
      (pixel_t *) img->pointer, x_src, y_src, img->pitch, width , height, shiled_colour);

  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      es = ES_BADMEM;
  }
  curthlocal->offscreen_surface->UnlockRect ();

  futex_unlock (curthlocal->lock);
  return es;
}

ES_EXPORT
es_status ES_API ESG_blit_alpha (struct _image *img, int x_dest, int y_dest, uint8_t alpha_dest, 
	                                 int x_src, int y_src, uint8_t alpha_src, 
                                   int width, int height) {

  es_status es;

  __try {
    es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  } __try {
    D3DLOCKED_RECT  tp_desc;

    // 填充后备缓冲像素, 先锁定后备缓冲 
    curthlocal->offscreen_surface->LockRect (& tp_desc, nullptr, D3DLOCK_DISCARD | D3DLOCK_DONOTWAIT);

    vemcpy4 ((pixel_t *)tp_desc.pBits, x_dest, y_dest, tp_desc.Pitch, 
      (pixel_t *) img->pointer, x_src, y_src, img->pitch, width , height, alpha_src,alpha_dest );

  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      es = ES_BADMEM;
  }
  curthlocal->offscreen_surface->UnlockRect ();

  futex_unlock (curthlocal->lock);
  return es;
}

// 字体设置/输出 
// 字体设置/输出 
ES_EXPORT
es_status ES_API ESG_setfont (const char *fontname, int width, int height) {

  es_status es;

  __try {
     es = futex_lock (curthlocal->lock);
     if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
     : EXCEPTION_CONTINUE_SEARCH) {
       return ES_BADMEM;
  } __try {

    ZeroMemory (& default_font.lfFaceName[0], sizeof (default_font.lfFaceName));
    size_t len = strlen (fontname);
    if (len >=32)
      len = 31;

    // 字体 !!!!
    memcpy ( & default_font.lfFaceName[0], fontname, len);

    default_font.lfQuality = PROOF_QUALITY;
    default_font.lfWeight = 400;
    default_font.lfCharSet = DEFAULT_CHARSET;

    if ( width != -1)  default_font.lfWidth = (LONG) width;
    if ( height != -1) default_font.lfHeight = (LONG) height;

    HFONT font = CreateFontIndirectA (& default_font);
    if ( font != nullptr) {
      if (curthlocal->cur_font != nullptr) {
        // 删除之前设置的字体
        DeleteObject (curthlocal->cur_font);
      }
      curthlocal->cur_font = font;
    } else {
      es = ES_FAIL;
    }
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    es = ES_BADMEM;
  }
  futex_unlock (curthlocal->lock);
  return es;
}


ES_EXPORT
int ES_API ESG_getkey (char ascii) {


  __try {
    return ( GetAsyncKeyState ( ascii) & 0x8000) ? 1 : 0;
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    return 0;
  }
}

ES_EXPORT
void ES_API ESG_atexit (void) {

  FreeLibrary (gvlib->_library);
}

ES_EXPORT
_es_lib * ES_API ESG_getibts (void) {

  return gvlib;
}

ES_EXPORT
void ES_API ESG_setibt (void *pjm) {

  gvlib = (_es_lib *)pjm;
}

ES_EXPORT
int ES_API ESG_getcursorx (void) {

  __try {
    POINT point;
    POINT point2;

    GetCursorPos (& point);
    GetCursorPos (& point2);
    ScreenToClient ( curthlocal->window_, & point2);

    // 获取当前客户区在尺寸
    RECT rtcl;
    GetClientRect (curthlocal->window_, & rtcl);

    // 获取客户区在电脑上坐标
    POINT ptcl = { 0, 0 };
    ClientToScreen (curthlocal->window_, & ptcl);

    LONG abst = (LONG) abs(rtcl.right - rtcl.left);
    abst--;

    // 如果点再客户区范围内直接返回 
    // 否则 < 0 则返回0, >客户区宽度, 返回客户区宽度 
    if ( point.x >= ptcl.x && (point.x <= (ptcl.x + abst)))  {
      return (int) point2.x;
    } else if ( point.x < ptcl.x) {
      return 0;
    } else if ( point.x > (ptcl.x + abst)) {
      return (int) abst;
    } else {
      assert (0);
      return 0;
    }
  }

  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    return 0;
  }
}

ES_EXPORT
int ES_API ESG_getcursory (void) {

  __try {
    POINT point;
    POINT point2;

    GetCursorPos (& point);
    GetCursorPos (& point2);
    ScreenToClient ( curthlocal->window_, & point2);

    // 获取当前客户区在尺寸
    RECT rtcl;
    GetClientRect (curthlocal->window_, & rtcl);

    // 获取客户区在电脑上坐标
    POINT ptcl = { 0, 0 };
    ClientToScreen (curthlocal->window_, & ptcl);

    LONG abst = (LONG) abs(rtcl.bottom - rtcl.top);
    abst--;

    // 如果点再客户区范围内直接返回 
    // 否则 < 0 则返回0, >客户区高度, 返回客户区高度 
    if ( point.y >= ptcl.y && (point.y <= (ptcl.y + abst)))  {
      return (int) point2.y;
    } else if ( point.y < ptcl.y) {
      return 0;
    } else if ( point.y > (ptcl.y + abst)) {
      return (int) abst;
    } else {
      assert (0);
      return 0;
    }
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    return 0;
  }
}


ES_EXPORT
int ES_API ESG_textout (int x, int y, pixel_t colour, TEXT_OPERATE_SET opset, const char *format, ...) {

  es_status es;

  __try {
    es = futex_lock (curthlocal->lock);
    if ( es != ES_OK) return es;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  } 
  __try {

      HDC dc;
      RECT rt = { x, y, x, y};
      HGDIOBJ  old;
      UINT tomask = 0;

      char strtemp[1024];
      int slen;

      va_list args; 
      va_start(args, format); // !!!! 
      vsprintf(strtemp, format, args); 
      va_end(args); 
      slen = strlen (strtemp);

      // 获取DC
      curthlocal->offscreen_surface->GetDC (& dc);

      SetTextColor (dc, colour & 0xFFFFFF);
      SetBkMode(dc, TRANSPARENT);

      if ( curthlocal->cur_font != nullptr)
        old = SelectObject (dc, (HGDIOBJ) curthlocal->cur_font);

      // calc rect 
      DrawTextA (dc, strtemp, slen, & rt, DT_CALCRECT);

      if (opset & TOS_CENTER)
        tomask |= DT_CENTER;
      if (opset & TOS_VCENTER)
        tomask |= (DT_VCENTER | DT_SINGLELINE);
      if (opset != 0) {
        rt.left =0;
        rt.top = 0;
        rt.right = curthlocal->d3dpp.BackBufferWidth;
        rt.bottom = curthlocal->d3dpp.BackBufferHeight;
      }

      DrawTextA (dc, strtemp, slen, & rt, tomask);

      if ( curthlocal->cur_font != nullptr)
        SelectObject (dc, (HGDIOBJ) old);

      curthlocal->offscreen_surface->ReleaseDC (dc);
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    es = ES_BADMEM;
  }

  futex_unlock (curthlocal->lock);
  return es;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
    DEBUGOUT ("Library Load...\n");
    essets();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
    break;
	case DLL_PROCESS_DETACH:
    DEBUGOUT ("Library Unload...\n");
    // essesf();
		break;
	}
	return TRUE;
}

// music 
#include <DShow.h>

struct music_ {

  IGraphBuilder* graphBuilder;
  IMediaControl* mediaControl;
  IMediaPosition* mediaPosition;
  IMediaEvent *mediaEvent;
};

ES_EXPORT
void ES_API ESG_freemusic  (music_t music) {

  __try {

    struct music_ *pm = (struct music_ *)music;

    if (pm->mediaControl) pm->mediaControl->Stop ();
    if (pm->mediaControl) pm->mediaControl->Release ();
    if (pm->mediaPosition) pm->mediaPosition->Release ();
    if (pm->mediaEvent) pm->mediaEvent->Release ();
    if (pm->graphBuilder) pm->graphBuilder->Release ();

  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    return;
  }
}

ES_EXPORT
es_status ES_API ESG_loadmusic  (const char *filename, music_t *music) {

  music_ *pm = new music_;

    // 读取音乐 
  __try {
    ZeroMemory (pm, sizeof (*pm));

    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,  IID_IGraphBuilder, (void**)&pm->graphBuilder);
 
    // 创建IMediaControl和IMediaPosition/IMediaEvent对象
    pm->graphBuilder->QueryInterface(IID_IMediaControl, (void**)&pm->mediaControl);
    pm->graphBuilder->QueryInterface(IID_IMediaPosition, (void**)&pm->mediaPosition);
    pm->graphBuilder->QueryInterface(IID_IMediaEvent, (void**)&pm->mediaEvent);

    WCHAR w_filename[MAX_PATH] = {0};
    mbstowcs (w_filename, filename, MAX_PATH);

    HRESULT ret = pm->graphBuilder->RenderFile (w_filename, nullptr);

    if (! SUCCEEDED (ret)) {
      ESG_freemusic ( (music_t) pm);
      return ES_NOEXIST;
    }
    *music = (music_t)pm;
    return ES_OK;

  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    ESG_freemusic ( (music_t) pm);
    return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_setpos	(music_t music, double pos) {

  __try {
    struct music_ *pm = (struct music_ *)music;
    return SUCCEEDED(pm->mediaPosition->put_CurrentPosition( (REFTIME) pos)) ? ES_OK : ES_FAIL; 
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_curpos	(music_t music, double *pos) {

  __try {
    struct music_ *pm = (struct music_ *)music;
    return SUCCEEDED(pm->mediaPosition->get_CurrentPosition( (REFTIME *) pos)) ? ES_OK : ES_FAIL; 
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_getposmax	(music_t music, double *length) {

  __try {
    struct music_ *pm = (struct music_ *)music;
    return SUCCEEDED(pm->mediaPosition->get_Duration( (REFTIME *) length)) ? ES_OK : ES_FAIL; 
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_startplay	(music_t music) {

  __try {
    struct music_ *pm = (struct music_ *)music;
    return SUCCEEDED(pm->mediaControl->Run ()) ? ES_OK : ES_FAIL; 
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
                              : EXCEPTION_CONTINUE_SEARCH) {
    return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_stopplay	(music_t music) {

  __try {
    struct music_ *pm = (struct music_ *)music;
    return SUCCEEDED(pm->mediaControl->Stop ()) ? ES_OK : ES_FAIL; 
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_curistail	(music_t music) {

  __try {

    struct music_ *pm = (struct music_ *)music;
    long event_code, param1, param2;

    if (SUCCEEDED(pm->mediaEvent->GetEvent (& event_code, & param1, & param2, 1))) {

      if (event_code == EC_COMPLETE)
      {
        // frees resources associated with the parameters of an events.
        pm->mediaEvent->FreeEventParams (event_code, param1, param2);

        return ES_OK;
      }           
      else 
      {
        pm->mediaEvent->FreeEventParams (event_code, param1, param2);
      }
    }
    return ES_NOYET;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_curtime	(uint64_t  *curticks) {

  __try {

    BOOL rest = QueryPerformanceCounter ((LARGE_INTEGER *) curticks );

    return rest != FALSE ? ES_OK : ES_FAIL;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_gettimecount	(uint64_t  *freqcount) {

  __try {

    BOOL rest = QueryPerformanceFrequency((LARGE_INTEGER *) freqcount );

    return rest != FALSE ? ES_OK : ES_FAIL;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}

ES_EXPORT 
es_status ES_API ESG_holdctl	(void) {

  __try {

    BOOL ret = EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,MF_BYCOMMAND|MF_GRAYED);
    return (ret == TRUE) ? ES_OK :ES_FAIL;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_FAIL;
  }
}

ES_EXPORT 
es_status ES_API ESG_resumectl	(void) {

  __try {

    BOOL ret = EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE,MF_BYCOMMAND);
    return (ret == TRUE) ? ES_OK :ES_FAIL;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_FAIL;
  }
}


// 
ES_EXPORT
es_status ES_API ESG_vec_fill (struct _image *imgDst, int xDst, int yDst, 
                  pixel_t colour,  int width, int height) {

void __cdecl vsetX86 (pixel_t *des, int dx, int dy, int dpitch, pixel_t colour,
                 int width, int height);
__try {

#if ! defined (_WIN64)
  vsetX86 
#else 
  vemcpy2 
#endif 
    (imgDst->pointer, xDst, yDst, imgDst->pitch, colour,
                 width, height);
    return ES_OK;
} __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
  : EXCEPTION_CONTINUE_SEARCH) {
    return ES_BADMEM;
}

}

// 快速拷贝例程 1:
ES_EXPORT
es_status ES_API ESG_vec_blit (struct _image *imgDst, int xDst, int yDst, 
struct _image *imgSrc, int xSrc, int ySrc,  
  int width, int height) {

void __cdecl vemcpyX86 (pixel_t *des, int dx, int dy, int dpitch,
                 pixel_t *src, int sx, int sy, int spitch, int width, int height);
__try {

#if ! defined (_WIN64)
  vemcpyX86 
#else 
  vemcpy2 
#endif 
    (imgDst->pointer, xDst, yDst, imgDst->pitch, 
    imgSrc->pointer, xSrc, ySrc, imgSrc->pitch, width, height);
    return ES_OK;
} __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
  : EXCEPTION_CONTINUE_SEARCH) {
    return ES_BADMEM;
}

}

// 快速拷贝例程 2 带掩码:
ES_EXPORT
es_status ES_API ESG_vec_blit1 (struct _image *imgDst, int xDst, int yDst, 
struct _image *imgSrc, int xSrc, int ySrc,  
  int width, int height, pixel_t sheild) {
  // 内建指令集包含 
  // -------------------------------------------------------
#include <intrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <wmmintrin.h>

  typedef union {

    pixel_t * mp_pixp;
    char *mp8;
    void *mp_v;
    __m128i * mp128;
    __m64 * mp64;
    uintptr_t mp_intptr;
  } mbv_type;

  // 大致考虑源/目的地址的对齐情况 进行掩码拷贝, 不进行精确的case优化
  // 利用 SIMD 的大块传输和maskmov指令用以提高传输速率和分支影响
  // XXX:过多额外的赋值, 循环优化
  // -------------------------------------------------------

  __try {

    if (! width || ! height) {
      return ES_OK;
    }

    uintptr_t round16 = width&~15;
    uintptr_t remain16 = width &15;
    uintptr_t pitDst = imgDst->pitch/4;
    uintptr_t pitSrc = imgSrc->pitch/4;
    uintptr_t ED = round16;
    uintptr_t ED2 = remain16;
    uintptr_t rvaDst = (imgDst->pitch - width* 4);
    uintptr_t rvaSrc = (imgSrc->pitch - width* 4);
    uintptr_t HI = height;

    mbv_type  pDst;
    mbv_type  pSrc;

    pDst.mp_v = (pixel_t *)& imgDst->pointer[xDst+yDst*pitDst];
    pSrc.mp_v = (pixel_t *)& imgSrc->pointer[xSrc+ySrc*pitSrc];
    pixel_t  pemask = sheild & 0xFFFFFF;

    if (round16 != 0) {

      __m128i maskDQ;
      __m128i aone128;
      __m128i maskLimit;

      // 混洗映射一个DWORD掩码为4个DWORD掩码数组/0x00FFFFFF掩码
      maskDQ.m128i_u32[0] = (unsigned int) pemask;
      maskDQ = _mm_shuffle_epi32 (maskDQ, 0);
      maskLimit.m128i_u32[0] = 0xFFFFFF;
      maskLimit = _mm_shuffle_epi32 (maskLimit, 0);

      // 128bit all->1
      aone128 = _mm_cmpeq_epi8 (maskDQ, maskDQ);

      // SSE 宏 
      // -----------------------------------------------------------------------------------------------
#define SSE_UNWIND(loadCALL, RCC) do {                                        \
        do { do {                                                             \
            __m128i C0 = _mm_and_si128 (loadCALL ((const __m128i *) & pSrc.mp128[0]), maskLimit);       \
            __m128i C1 = _mm_and_si128 (loadCALL ((const __m128i *) & pSrc.mp128[1]), maskLimit);       \
            __m128i C2 = _mm_and_si128 (loadCALL ((const __m128i *) & pSrc.mp128[2]), maskLimit);       \
            __m128i C3 = _mm_and_si128 (loadCALL ((const __m128i *) & pSrc.mp128[3]), maskLimit);       \
                                                                              \
            __m128i M0 = _mm_xor_si128 (_mm_cmpeq_epi32 (C0, maskDQ), aone128); \
            __m128i M1 = _mm_xor_si128 (_mm_cmpeq_epi32 (C1, maskDQ), aone128); \
            __m128i M2 = _mm_xor_si128 (_mm_cmpeq_epi32 (C2, maskDQ), aone128); \
            __m128i M3 = _mm_xor_si128 (_mm_cmpeq_epi32 (C3, maskDQ), aone128); \
                                                                              \
            _mm_maskmoveu_si128 (C0, M0, (char *) & pDst.mp128[0]);          \
            _mm_maskmoveu_si128 (C1, M1, (char *) & pDst.mp128[1]);          \
            _mm_maskmoveu_si128 (C2, M2, (char *) & pDst.mp128[2]);          \
            _mm_maskmoveu_si128 (C3, M3, (char *) & pDst.mp128[3]);          \
                                                                              \
            pDst.mp128  =  &  pDst.mp128[4];                                \
            pSrc.mp128  =  &  pSrc.mp128[4];                                \
                                                                              \
        } while ( (ED-= 16)); ED = round16;                                     \
            /** 补齐**/                                                        \
            RCC;                                                              \
                                                                              \
            pDst.mp8  =  &  pDst.mp8[rvaDst];                           \
            pSrc.mp8  =  &  pSrc.mp8[rvaSrc];                           \
                                                                              \
        } while (--HI);                                                       \
      } while (0)

#define SSE_NODONE 

#define SSE_REMAIN do {                                                      \
        do {                                                                 \
          pixel_t temp = 0xFFFFFF & pSrc.mp_pixp[0];                         \
          if (pemask != temp) {                                              \
             pDst.mp_pixp[0] = temp;                                         \
          }                                                                  \
          pDst.mp_pixp  =  &  pDst.mp_pixp[1];                                \
          pSrc.mp_pixp  =  &  pSrc.mp_pixp[1];                                \
        } while (--ED2);  ED2 = remain16;                                    \
      } while (0)

      //  SSE 剩余字节测试
      if  (remain16 == 0) {
        if (! (((uintptr_t)pSrc.mp_v) & 15)) 
          SSE_UNWIND( _mm_load_si128, SSE_NODONE);
        else 
          SSE_UNWIND( _mm_lddqu_si128, SSE_NODONE);
      } else {
        if (! (((uintptr_t)pSrc.mp_v) & 15)) 
          SSE_UNWIND( _mm_load_si128, SSE_REMAIN);
        else 
          SSE_UNWIND( _mm_lddqu_si128, SSE_REMAIN);
      }
    } else {

        do { do {                                                             \
            pixel_t temp = 0xFFFFFF & pSrc.mp_pixp[0];                         \
            if (pemask != temp) {                                              \
              pDst.mp_pixp[0] = temp;                                         \
            }                                                                  \
            pDst.mp_pixp  =  &  pDst.mp_pixp[1];                                \
            pSrc.mp_pixp  =  &  pSrc.mp_pixp[1];                                \
          \
          } while ( --ED); ED = width;                                     \

          pDst.mp8  =  &  pDst.mp8[rvaDst];                           \
          pSrc.mp8  =  &  pSrc.mp8[rvaSrc];                           \
        \
        } while (--HI);                                                       \
    }
    return ES_OK;
  } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
    : EXCEPTION_CONTINUE_SEARCH) {
      return ES_BADMEM;
  }
}


// 快速拷贝例程 3 Alpha:
ES_EXPORT
es_status ES_API ESG_vec_blit2 (struct _image *imgDst, int xDst, int yDst, uint8_t aDst,
struct _image *imgSrc, int xSrc, int ySrc,  uint8_t aSrc, 
  int width, int height) {
    void __cdecl alphaX86 (pixel_t *des, int dx, int dy, int dpitch, uint8_t dalpha,
      pixel_t *src, int sx, int sy, int spitch, uint8_t salpha, int width, int height);
    __try {

#if ! defined (_WIN64)
      alphaX86 
#else 
      vemcpy2 
#endif 
        (imgDst->pointer, xDst, yDst, imgDst->pitch, aDst, 
        imgSrc->pointer, xSrc, ySrc, imgSrc->pitch, aSrc, width, height);
      return ES_OK;
    } __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER
      : EXCEPTION_CONTINUE_SEARCH) {
        return ES_BADMEM;
    }
}
};

#ifdef ___DEBUG_NOTDLL 
#include <crtdbg.h>

void main() {

  struct _es_lib *ssp;
  load_eslib (& ssp);

  
  ssp->init (1200, 720);
  ssp->setfont ("Courier New", 16, 24);

  while (TRUE) {
#if 1

    // printf ("music current pos:%lf\n", pos);

    if (ssp->getattach () == nullptr)
      break;
    ssp->fillall (-0555);
    // ssp->blit_alpha (pcc, 0, 0, 25, 0, 0, 60, 66666666, 5555);
    ssp->textout (20, 0, 0xCCC45, 0, "mouse_posx:%d mouse_posy:%d", ssp->getcursorx (), ssp->getcursory() );
    ssp->backtoforgeall ();
    ssp->postall ();
#endif 
  }
  // _CrtDumpMemoryLeaks();
  // ssp->freemusic (s);
  // ssp->freeimg (ppc);
}

#endif 







