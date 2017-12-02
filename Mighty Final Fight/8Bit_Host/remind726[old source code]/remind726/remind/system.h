#ifndef __system_
#define __system_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* About COM */
#define __RELEASE_COM(sym,cmb) \
do { \
   if (cmb) sym##_Release (cmb), cmb = NULL;         \
} while (0)
    
/* About ctor/dtor */
#define __ALLOC_OBJECT(r,size) \
do { \
   r = dlmalloc(size);         \
} while (0)
    
#define __FREE_OBJECT(r) \
do { \
   if (r) dlfree(r), r = NULL;  \
} while (0)

#ifdef __WINDOWS_ 
# include "setup.h"
# include <windows.h>
# include <d3d9.h>
# include <d3dx9.h>
# include <dinput.h>
# include <XAudio2.h>
#endif

struct system_s {
  void *ppu; 
//void *apu; 
  void *video_handle;
//void *sound_handle;
  void *input_handle;
  void *timing_handle;
  void (*mach_callback)(void *, void *, void *, unsigned int, unsigned int); 
  void (*init_callback)(void *, void *, void *, unsigned int, unsigned int); 
#ifdef __WINDOWS_
  int multicom;
  int imask;
  HINSTANCE instance;
  WNDCLASS wndclass;
  HWND window;
  MSG msg;
  IXAudio2 *sound_drv;
  IXAudio2MasteringVoice *sound_drv_;
  IDirectInput8 *input_drv;
  IDirect3D9 *video_drv;
#endif     
};

__rttype_ void *init_system (void *init_callback, void *mach_callback);
__rttype_ void show_window (void *sys);
__rttype_ void hide_window (void *sys);
__rttype_ void system_swap (void *sys);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif

/* machine_callback (machine_core, oamobject, sound_object, key_infos, timing_quad, timing_small)
 // main_rountine init_args 
 // .... my set 
 // run loop:machine_callback
 // destroy ... 
 // OK  void (*mach_callback)(void *pm, void *sob, void *keymask, unsigned int quad, unsigned int small);
*/

