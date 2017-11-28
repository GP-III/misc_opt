#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <assert.h>
#include <Windows.h>

// return type 
typedef    int es_status;

// error code 
#define   ES_OK     0
#define   ES_FAIL   1
#define   ES_BADMEM 2
#define   ES_NOEXIST 3 
#define   ES_BUSY   4 
#define   ES_LOST   5 
#define   ES_PERM   6 
#define   ES_OVERFLOW 7 
#define   ES_NOYET  8 
#define   ES_NOMEM  9
#define   ES_NOTSELF 10
#define   ES_EXIST 11

// pixel type 
typedef   uint32_t pixel_t; // RGB32 
// music type 
typedef   void *music_t;
// for   textout
typedef   int TEXT_OPERATE_SET;

#define   TOS_CENTER 1
#define   TOS_VCENTER 2

//  for getkey 
#define   MOUSE_LBUT  VK_LBUTTON
#define   MOUSE_MBUT  VK_MBUTTON
#define   MOUSE_RBUT  VK_RBUTTON

#ifdef _MSC_VER 
#   define ES_API  __cdecl //VC++
#   define ES_EXPORT  __declspec(dllexport)
#   define __STDCALL___ __stdcall
#   define __CDECL___ __cdecl
#elif defined (__GNUG__) || defined (__GNUC__)
#   define ES_API  __attribute__((__cdecl__)) //GCC/G++
#   define __STDCALL___ __attribute__((__stdcall__))
#   define __CDECL___ __attribute__((__cdecl__))
#endif 

struct _image {
  
  int   width;
  int   height;
  int   pitch;

  pixel_t *pointer;
};

struct _es_lib {

  es_status (ES_API* init)(int width, int height);
  es_status (ES_API* terminate)(void);
  
  void * (ES_API* getattach)(void);
  es_status (ES_API* setattach)(void *attach);

  struct _image *(ES_API* loadimg)(const char *filename);
  void (ES_API* freeimg)(struct _image *img);
  // 从内存中伪造一个_image 对象 
  struct _image *(ES_API* dummyimg)(int width, int height);

  es_status (ES_API* fill)(int x, int y, int width, int height, pixel_t colour);
  es_status (ES_API* fillall)(pixel_t colour);
  es_status (ES_API* point)(int x, int y, pixel_t colour);

  es_status (ES_API* holdctl)(void);
  es_status (ES_API* resumectl)(void);

  es_status (ES_API* blit) (struct _image *img, int x_dest, int y_dest, int x_src, int y_src, int width, int height);
  es_status (ES_API* blit_shield) (struct _image *img, int x_dest, int y_dest, int x_src, int y_src, int width, int height, pixel_t shield_colour);
  es_status (ES_API* blit_alpha) (struct _image *img, int x_dest, int y_dest, uint8_t alpha_dest, 
	                                 int x_src, int y_src, uint8_t alpha_src, int width, int height);
									 
  es_status (ES_API* backtoforge)  (int x, int y, int width, int height);
  es_status (ES_API* backtoforgeall)  (void);
  es_status (ES_API* post)  (int x, int y, int width, int height);
  es_status (ES_API* postall)  (void);

  es_status (ES_API* setfont)  (const char *fontname, int width, int height); 
  es_status (ES_API* textout)  (int x, int y, pixel_t colour, TEXT_OPERATE_SET opset, const char *format, ...);

  // 直接锁定当前渲染线程后备缓冲进行快速绘图 
  // 注意当输出字体的时候需要解锁缓冲, 非法内存访问将不受保护
  struct _image *(ES_API* lockback)(es_status *status);
  es_status (ES_API* unlockback)(void);

  es_status (ES_API* vec_fill) (struct _image *imgDst, int xDst, int yDst, 
                             pixel_t colour,  int width, int height);
// bitblt
  es_status (ES_API* vec_blit) (struct _image *imgDst, int xDst, int yDst, 
                     struct _image *imgSrc, int xSrc, int ySrc,  
                              int width, int height);
  es_status (ES_API* vec_blit1) (struct _image *imgDst, int xDst, int yDst, 
                     struct _image *imgSrc, int xSrc, int ySrc,  
                              int width, int height, pixel_t shield);
  es_status (ES_API* vec_blit2) (struct _image *imgDst, int xDst, int yDst, uint8_t aDst,
                     struct _image *imgSrc, int xSrc, int ySrc,   uint8_t aSrc,
                              int width, int height);
  es_status (ES_API* vec_blit3) (struct _image *imgDst, int xDst, int yDst, uint8_t aDst,
                     struct _image *imgSrc, int xSrc, int ySrc,   uint8_t aSrc,
                              int width, int height, pixel_t shield);

  // 计时器, 就是windows所使用的高精度计时器的参数
  // curtime获得的参数除以用 gettimecount 获得的参数可获得当前流逝的秒速
  es_status (ES_API* gettimecount)  (uint64_t  *freqcount); 
  es_status (ES_API* curtime)  (uint64_t  *curticks);

  // 鼠标/键盘函数
  int (ES_API* getkey)  (char ascii);  // UP:0
  int (ES_API* getcursorx) (void);
  int (ES_API* getcursory) (void);
  
  //  播放音频 支持.wav/.wma/.mp3 以及当前安装的可供使用的解码器
  es_status (ES_API* loadmusic)  (const char *filename, music_t *music);  
  es_status (ES_API* freemusic) (music_t music);
  es_status (ES_API* setpos)   (music_t music, double pos);
  es_status (ES_API* startplay)	(music_t music);
  es_status (ES_API* stopplay)    (music_t music);
  es_status (ES_API* curpos)	(music_t music, double *pos);
  es_status (ES_API* getposmax) (music_t music, double *length);
  es_status (ES_API* curistail) (music_t music);    

  // dtor 
  es_status (ES_API* essesf) (void);    

  void (ES_API* atexit)  (void);
  int (ES_API* setibt)  (void *pjm);

  void *(__STDCALL___ *win32loadlib)  (const char *libname);
  void (__STDCALL___ *win32freelib)  (void *lib);
  void *(__STDCALL___ *win32funcfromlib)  (void *lib, const char *func_name);

  HMODULE _library;
};

static 
void __CDECL___ unload_eslib (void) {

#  ifdef  _MSC_VER 
#     ifdef __cplusplus
#       define TYPE02(T) decltype(T)
#     else 
#       define TYPE02(T) void *
#     endif 
#  elif defined (__GNUG__) || defined (__GNUC__)
#     define TYPE02(T) typeof(T)
#  endif 

  // Sleep (500);
  (  (void(ES_API*)	(void)) 
    GetProcAddress (GetModuleHandleA("eswraps.dll"), "essesf"))();

  FreeLibrary ((  (struct _es_lib *(ES_API*)	(void)) 
    GetProcAddress (GetModuleHandleA("eswraps.dll"), "ESG_getibts"))()->_library);
}


static 
void load_eslib (struct _es_lib **eslib0) {

  static struct _es_lib _es_lib0;
  struct _es_lib *eslib = & _es_lib0;

  eslib->_library = LoadLibraryA ("eswraps.dll");
  assert (eslib->_library != (void *) 0);

  eslib->backtoforge = (TYPE02 (eslib->backtoforge)) GetProcAddress (eslib->_library, "ESG_backtoforge");
  eslib->backtoforgeall = (TYPE02 (eslib->backtoforgeall)) GetProcAddress (eslib->_library, "ESG_backtoforgeall");
  eslib->blit = (TYPE02 (eslib->blit)) GetProcAddress (eslib->_library, "ESG_blit");
  eslib->blit_alpha = (TYPE02 (eslib->blit_alpha)) GetProcAddress (eslib->_library, "ESG_blit_alpha");
  eslib->blit_shield = (TYPE02 (eslib->blit_shield)) GetProcAddress (eslib->_library, "ESG_blit_shield");
  eslib->fill = (TYPE02 (eslib->fill)) GetProcAddress (eslib->_library, "ESG_fill");
  eslib->fillall = (TYPE02 (eslib->fillall)) GetProcAddress (eslib->_library, "ESG_fillall");
  eslib->freeimg = (TYPE02 (eslib->freeimg)) GetProcAddress (eslib->_library, "ESG_freeimg");
  eslib->dummyimg = (TYPE02 (eslib->dummyimg)) GetProcAddress (eslib->_library, "ESG_dummyimg");
  eslib->getattach = (TYPE02 (eslib->getattach)) GetProcAddress (eslib->_library, "ESG_getattach");
  eslib->init = (TYPE02 (eslib->init)) GetProcAddress (eslib->_library, "ESG_init");
  eslib->loadimg = (TYPE02 (eslib->loadimg)) GetProcAddress (eslib->_library, "ESG_loadimg");
  eslib->point = (TYPE02 (eslib->point)) GetProcAddress (eslib->_library, "ESG_point");
  eslib->post = (TYPE02 (eslib->post)) GetProcAddress (eslib->_library, "ESG_post");
  eslib->postall = (TYPE02 (eslib->postall)) GetProcAddress (eslib->_library, "ESG_postall");
  eslib->lockback = (TYPE02 (eslib->lockback)) GetProcAddress (eslib->_library, "ESG_lockback");
  eslib->unlockback = (TYPE02 (eslib->unlockback)) GetProcAddress (eslib->_library, "ESG_unlockback");
  eslib->setattach = (TYPE02 (eslib->setattach)) GetProcAddress (eslib->_library, "ESG_setattach");
  eslib->setfont = (TYPE02 (eslib->setfont)) GetProcAddress (eslib->_library, "ESG_setfont");
  eslib->terminate = (TYPE02 (eslib->terminate)) GetProcAddress (eslib->_library, "ESG_terminate");
  eslib->textout = (TYPE02 (eslib->textout)) GetProcAddress (eslib->_library, "ESG_textout");
  eslib->getkey = (TYPE02 (eslib->getkey)) GetProcAddress (eslib->_library, "ESG_getkey");
  eslib->setibt = (TYPE02 (eslib->setibt)) GetProcAddress (eslib->_library, "ESG_setibt");
  eslib->atexit = (TYPE02 (eslib->atexit)) GetProcAddress (eslib->_library, "ESG_atexit");
  eslib->getcursorx = (TYPE02 (eslib->getcursorx)) GetProcAddress (eslib->_library, "ESG_getcursorx");
  eslib->getcursory = (TYPE02 (eslib->getcursory)) GetProcAddress (eslib->_library, "ESG_getcursory");
  eslib->loadmusic = (TYPE02 (eslib->loadmusic)) GetProcAddress (eslib->_library, "ESG_loadmusic");
  eslib->freemusic = (TYPE02 (eslib->freemusic)) GetProcAddress (eslib->_library, "ESG_freemusic");
  eslib->setpos = (TYPE02 (eslib->setpos)) GetProcAddress (eslib->_library, "ESG_setpos");
  eslib->startplay = (TYPE02 (eslib->startplay)) GetProcAddress (eslib->_library, "ESG_startplay");
  eslib->stopplay = (TYPE02 (eslib->stopplay)) GetProcAddress (eslib->_library, "ESG_stopplay");
  eslib->curpos = (TYPE02 (eslib->curpos)) GetProcAddress (eslib->_library, "ESG_curpos");
  eslib->getposmax = (TYPE02 (eslib->getposmax)) GetProcAddress (eslib->_library, "ESG_getcursory");
  eslib->curistail = (TYPE02 (eslib->curistail)) GetProcAddress (eslib->_library, "ESG_curistail");
  eslib->curtime = (TYPE02 (eslib->curtime)) GetProcAddress (eslib->_library, "ESG_curtime");
  eslib->gettimecount = (TYPE02 (eslib->gettimecount)) GetProcAddress (eslib->_library, "ESG_gettimecount");
  eslib->resumectl = (TYPE02 (eslib->resumectl)) GetProcAddress (eslib->_library, "ESG_resumectl");
  eslib->holdctl = (TYPE02 (eslib->holdctl)) GetProcAddress (eslib->_library, "ESG_holdctl");
  eslib->vec_blit = (TYPE02 (eslib->vec_blit)) GetProcAddress (eslib->_library, "ESG_vec_blit");
  eslib->vec_fill = (TYPE02 (eslib->vec_fill)) GetProcAddress (eslib->_library, "ESG_vec_fill");
  eslib->vec_blit1 = (TYPE02 (eslib->vec_blit1)) GetProcAddress (eslib->_library, "ESG_vec_blit1");
  eslib->vec_blit2 = (TYPE02 (eslib->vec_blit2)) GetProcAddress (eslib->_library, "ESG_vec_blit2");
  eslib->vec_blit3 = (TYPE02 (eslib->vec_blit3)) GetProcAddress (eslib->_library, "ESG_vec_blit3");
  eslib->essesf = (TYPE02 (eslib->essesf)) GetProcAddress (eslib->_library, "essesf");

  eslib->setibt (eslib);
  atexit (unload_eslib);

 * eslib0 = eslib;
}

#ifdef __cplusplus
};
#endif

#endif 
