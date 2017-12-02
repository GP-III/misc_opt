#include "system.h"
#include "input.h"
#include "video.h"
#include "sound.h"
#include "timing.h"
#include "ppu_inline.h"
//
// 
void *init_ppu(void);
void *init_apu(void);
void uninit_ppu(void *ppu);
void ppu_render (void *ppu);
void mach_callback_default(void *s, void *v, void *a, unsigned int g, unsigned int t){}
void init_callback_default(void *s, void *v, void *a, unsigned int g, unsigned int t){}

#ifdef __WINDOWS_
static 
int resize_client (HWND window, INT width, INT height, 
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
LRESULT CALLBACK window_callback (HWND window, UINT msg, WPARAM w, LPARAM l)
{
  if (msg != WM_DESTROY)
      return DefWindowProc (window, msg, w, l); 
  PostQuitMessage (0);
  return 0;
}
static 
void uninit_directx (struct system_s *sysh) {
    
  if (sysh->video_drv)
      IDirect3D9_Release (sysh->video_drv);
  if (sysh->input_drv)
      IDirectInput8_Release (sysh->input_drv);
  if (sysh->sound_drv_)
      IXAudio2MasteringVoice_DestroyVoice (sysh->sound_drv_);
  if (sysh->sound_drv)
      IXAudio2_Release (sysh->sound_drv);
  if (sysh->multicom == 1)  
      CoUninitialize();
}
static 
int init_directx (struct system_s *sysh) {
    
  if (S_OK != CoInitializeEx (NULL, COINIT_MULTITHREADED))
      return -1;
  sysh->multicom = 1; /* set multi-com init flags */
  sysh->video_drv = Direct3DCreate9(D3D_SDK_VERSION);

  // XAudio2Create (&sysh->sound_drv, 0, XAUDIO2_DEFAULT_PROCESSOR); 
  DirectInput8Create (sysh->instance, DIRECTINPUT_VERSION, &IID_IDirectInput8, (void **)&sysh->input_drv, NULL); 
  // IXAudio2_CreateMasteringVoice (sysh->sound_drv, &sysh->sound_drv_, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL); 
  
  if (//!sysh->sound_drv || !sysh->sound_drv_ 
  //||  
	  !sysh->input_drv || !sysh->video_drv) {
      
      uninit_directx (sysh);
      return -1;
  }         
  return 0;
}
#endif

__rttype_ void *init_system(void *init_callback, void *mach_callback) {
#ifdef __WINDOWS_   
  struct system_s *sysh = dlmalloc (sizeof(struct system_s));
  if (!sysh)
      return 0;      
  
  memset (sysh, 0x00, sizeof(struct system_s));

  sysh->mach_callback = mach_callback ? mach_callback : mach_callback_default;
  sysh->init_callback = init_callback ? init_callback : init_callback_default;
  sysh->instance = GetModuleHandle (NULL); 
  if (!sysh->instance)
      goto __final;
  
  sysh->wndclass.style = CS_HREDRAW | CS_VREDRAW;
  sysh->wndclass.lpfnWndProc = window_callback;
  sysh->wndclass.hInstance = sysh->instance;
  sysh->wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  sysh->wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
  sysh->wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
  sysh->wndclass.lpszClassName = TEXT("$~LiAnGZiJuN$~") ;

  if (!RegisterClass (&sysh->wndclass))
      goto __final;
     
  sysh->window = CreateWindow (TEXT("$~LiAnGZiJuN$~"),
                               TEXT("extend_machine"), 
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT, 
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               NULL,
                               NULL, 
                               sysh->instance, 
                               NULL); 
  if (!sysh->window)
      goto __final;
       
  if (0 != init_directx (sysh))
      goto __final;
       
  if (0 != init_timing (sysh)
  ||  0 != init_video (sysh)
  ||  0 != init_input (sysh))
      goto __final;
   
  sysh->ppu = init_ppu();
//sysh->apu = init_apu();  
  
   
  if (0 != resize_client (sysh->window, 256, 240, WS_OVERLAPPEDWINDOW))
      goto __final;
       
  if (0 != set_timing_period (sysh->timing_handle, 60))
      goto __final;
   
  // ShowWindow (sysh->window, SW_SHOWNORMAL);
  // UpdateWindow (sysh->window);
  // callback init call SW_SHOW
  sysh->init_callback (sysh, _GETOAM (sysh->ppu), NULL, 0, 0);
  return sysh;
__final:
  dlfree (sysh);
  return NULL;
#else 
#endif       
}
__rttype_ void show_window (struct system_s *sysh) { 
#ifdef __WINDOWS_   
    ShowWindow (sysh->window, SW_SHOWNORMAL);
  UpdateWindow (sysh->window);
#else 
#endif 
}
__rttype_ void hide_window (struct system_s *sysh) {
#ifdef __WINDOWS_   
    ShowWindow (sysh->window, SW_HIDE);
  UpdateWindow (sysh->window);
#else 
#endif     
}
static _machine_frame (struct system_s *sysh) {
  struct rtkey_s kmask;  
  // Init-countstamp
  start_timing (sysh->timing_handle);
  // Poll keyboard
  poll_key (sysh->input_handle);
  // Get keyboard mask 
  keymask_ret (sysh->input_handle, &kmask);
  // callback times 
  sysh->mach_callback (sysh, _GETOAM (sysh->ppu), &kmask, getquad_timing (sysh->timing_handle), getsmall_timing (sysh->timing_handle));
  // PPU times 
  ppu_render (sysh->ppu);
  // APU times 
  //apu_render (sysh->apu);  
  // PPU Out 
  video_render (sysh->video_handle, _GETPPUBUF (sysh->ppu), 1088);
  // APU Out 
  // sound_render 
  // Timing sync 
  terming_timing (sysh->timing_handle);   
}

__rttype_ void system_swap (struct system_s *sysh) {
#ifdef __WINDOWS_   
  while(TRUE) {
    if (PeekMessage (&sysh->msg, NULL, 0, 0, PM_REMOVE))
      if (sysh->msg.message == WM_QUIT)
          break;
      else 
          DispatchMessage(&sysh->msg);
    else
      _machine_frame (sysh);
  }
  uninit_ppu (sysh->ppu);
//uninit_apu (sysh->apu);
  uninit_timing (sysh->timing_handle);
  uninit_video (sysh->video_handle);
  uninit_input (sysh->input_handle);
//uninit_sound (sysh->sound_handle);
  uninit_directx (sysh);
#else 
#endif     
}
__rttype_ void system_callback_hack_machloop (struct system_s *sysh, void *callback) {
    
    
}
__rttype_ void system_callback_hack_machinit (struct system_s *sysh, void *callback) {
    
    
}
__rttype_ void system_pulse_machinit (struct system_s *sysh, void *callback) {
    
    
}



