#include "system.h"
#include "setup.h"

struct video_s {
  void *tex[256];
  struct system_s *sys;
#ifdef __WINDOWS_ 
  D3DLOCKED_RECT info;
  IDirect3DDevice9 *dev; 
  IDirect3DSurface9 *surf;
#else 
#endif
};

int init_video (struct system_s *sys) {
#ifdef __WINDOWS_ 
  /* Windows use DirectX-API */
  D3DPRESENT_PARAMETERS d3dpp = {0};
  /* Base object warps */
  struct video_s *vm = dlmalloc (sizeof(struct video_s));
  if (!vm)
      return -1;      
  
  memset (vm, 0x00, sizeof(struct video_s));

  d3dpp.BackBufferWidth      = 256;
  d3dpp.BackBufferHeight     = 240;
  d3dpp.BackBufferFormat     = D3DFMT_X8R8G8B8;
  d3dpp.SwapEffect           = D3DSWAPEFFECT_DISCARD;
  d3dpp.Flags                = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
  d3dpp.hDeviceWindow        = sys->window;
  d3dpp.Windowed             = TRUE;
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; /* Vsync close */

  if (S_OK == IDirect3D9_CreateDevice (sys->video_drv, 0, D3DDEVTYPE_HAL, sys->window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &vm->dev))
    if (S_OK == IDirect3DDevice9_GetBackBuffer (vm->dev, 0, 0, D3DBACKBUFFER_TYPE_MONO, &vm->surf)) {
      vm->sys = sys; // Set parent
      sys->video_handle = vm; // Set child    
      return 0;
    }
    
  // Try free d3ddev, buf 
  __RELEASE_COM(IDirect3DDevice9,vm->dev);
  // Try release video object
  __FREE_OBJECT(vm);
  
  return -1;
#else 
#endif 
}

__rttype_ int alloc_texure (struct video_s *vm, const unsigned char *name, unsigned char slot) {
#ifdef __WINDOWS_      
  D3DLOCKED_RECT TEXINFO;
  DWORD *TEXBUFFER;
  IDirect3DTexture9* TEX; 
  
  if (S_OK == 
      D3DXCreateTextureFromFileExA (vm->dev, name,
                                    128,
                                    128,
                                   -3, /* D3DX_FROM_FILE, */
                                    0,
                                    D3DFMT_UNKNOWN,
                                    D3DPOOL_MANAGED,
                                   -1, /* D3DX_DEFAULT */
                                   -1, /* D3DX_DEFAULT */
                                    D3DCOLOR_XRGB(0,0,0), 
                                    NULL, 
                                    NULL,
                                   &TEX))
                                   
  if (S_OK == 
      IDirect3DTexture9_LockRect (TEX, 0, &TEXINFO, NULL, D3DLOCK_READONLY))                                  
      {
        TEXBUFFER = dlmalloc (128*128*4);
        if (NULL != TEXBUFFER) {
            DWORD I;
            DWORD PITCH = TEXINFO.Pitch >> 2;
            DWORD *SCBUF = TEXINFO.pBits;
            for (I = 0; I != 128; I++)
                 memcpy (&TEXBUFFER[I*128], &SCBUF[I*PITCH], 512);
                  
            if (S_OK != IDirect3DTexture9_UnlockRect (TEX, 0))
                goto __final;
            if (S_OK != IDirect3DTexture9_Release (TEX))
                goto __final;
            __FREE_OBJECT(vm->tex[slot]);
            vm->tex[slot] = &TEXBUFFER[0];
            return 0;
        }
      }
__final:
  __RELEASE_COM(IDirect3DTexture9, TEX);
  __FREE_OBJECT(TEXBUFFER);      
  return -1;  
#else 
#endif 
}
__rttype_ void free_texure (struct video_s *vm, unsigned char slot) {
#ifdef __WINDOWS_        
            __FREE_OBJECT(vm->tex[slot]);
#else 
#endif 
}
int video_render (struct video_s *vm, unsigned int *vbuf, unsigned int pitch) {
#ifdef __WINDOWS_   
  DWORD I;
  D3DLOCKED_RECT vi;
  DWORD *d, *s, N, B;

  if (S_OK != IDirect3DSurface9_LockRect (vm->surf, &vi, NULL, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE))
      return -1;

  s = vbuf;
  d = vi.pBits;
  N = vi.Pitch >> 2;
  B = pitch >> 2;
  
  for (I = 0; I != 240; I++)
       memcpy (&d[I*N], &s[I*B], 1024);

  if (S_OK != IDirect3DSurface9_UnlockRect (vm->surf))
      return -1;
  
  if (S_OK != IDirect3DDevice9_Present (vm->dev, 0, 0, 0, 0))
      return -1;
  
  return 0;
#else  
#endif  
}
int uninit_video (struct video_s *vm) {
#ifdef __WINDOWS_   
  int i;
  if (vm == NULL)
      return 0;
  if (vm->surf)
      if (S_OK != IDirect3DSurface9_Release (vm->surf))
          return -1;
  if (vm->dev) 
      if (S_OK != IDirect3DDevice9_Release (vm->dev))
          return -1;  
  for (i = 0; i != 256; i++)
       if (vm->tex[i] != NULL)
           dlfree (vm->tex[i]), vm->tex[i] = NULL;
//if (vm)
      dlfree(vm);
  
  return 0;
#else 
#endif  
}
__rttype_ void *detail_texture (struct video_s *vm, unsigned char slot) {
      return  vm->tex[slot];
}

