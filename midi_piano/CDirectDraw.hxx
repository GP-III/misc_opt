//  CDirectDraw.hxx 
//  Provide simple graphical rendering and texture reading 
# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CDIRECTDRAW_INCLUED_HEADER_CXX_)
#  define _CDIRECTDRAW_INCLUED_HEADER_CXX_

# include <atldef.h>
# include <directx/include/d3dx9.h>

// extern asm interface.
extern "C" {
  int CAlphaSingle32 (void *  _vptr, int pitch, 
          int dst_x, int dst_y, 
                                          int dst_w, int dst_h, 
          COLORREF col,
              BYTE alpha_dst, BYTE alpha_src);
  int CSurfaceCopy32 (void *  _dvptr, int d_pos_x,  int d_pos_y, int d_pitch,
    void *  _svptr, int s_pos_x,  int s_pos_y, int s_pitch,
                                          int w, int h);
}

# if !defined (_WIN64)
#  define CSurfaceCopy CSurfaceCopy32
#  define CAlphaSingle CAlphaSingle32
# else 
# endif 

class CDirectDraw 
{
private:
  IDirect3DDevice9  *m_Direct3D9_device;   
  IDirect3DSurface9  *m_Direct3D9_surface;

  D3DPRESENT_PARAMETERS m_d3dpp;

  HWND m_AttachWindow;

  void initSettings (void) {
    m_Direct3D9_device = NULL;
    m_Direct3D9_surface = NULL;
  }
  void CreateD3dppSurface (int width, int height)  {

    // reset d3d desc
    ResetD3dpp (m_d3dpp, m_AttachWindow, width, height);
    
    // create d3d device
    HRESULT si = sm_Direct3D9_root->CreateDevice (0,
                        D3DDEVTYPE_HAL, 
                           m_AttachWindow, 
                              D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, & m_Direct3D9_device);
    ATLASSERT (SUCCEEDED (si));
  }
public:
  struct graph_desc {
    DWORD *__vptr;
    DWORD pitch;
    DWORD max_width;
    DWORD max_height;
  };
 
  int lockBuffer (IDirect3DSurface9 *surface, RECT *rcLock, graph_desc &gd)  {
    D3DLOCKED_RECT lock_rect;
    D3DSURFACE_DESC lock_desc;
    HRESULT rets = surface->LockRect (& lock_rect, rcLock, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
    if (FAILED (rets))
      return -1;
    else  ;
    rets = surface->GetDesc (& lock_desc);
    ATLASSERT (SUCCEEDED (rets));
    gd.__vptr = (DWORD *) lock_rect.pBits;
    gd.pitch = lock_rect.Pitch;
    if (rcLock != NULL) {
      gd.max_width = abs (rcLock->right -  rcLock->left);
      gd.max_height = abs (rcLock->bottom -  rcLock->top);
    } else  {
      gd.max_width = lock_desc.Width;
      gd.max_height = lock_desc.Height;
    }
    return 0;
  }

  void resetBackSurface (int width, int height)  {
    if (m_Direct3D9_surface != NULL)  {
      m_Direct3D9_surface->Release ();
      m_Direct3D9_surface = NULL;
    }
    HRESULT rets = m_Direct3D9_device->CreateOffscreenPlainSurface (width, height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM,
          & m_Direct3D9_surface, NULL);
    ATLASSERT (SUCCEEDED (rets));
  }
  
  int lockBuffer (RECT *rcLock, graph_desc &gd)  {
    return lockBuffer (m_Direct3D9_surface, rcLock, gd);
  }
  int unlockBuffer (void)  {
    HRESULT rets =m_Direct3D9_surface->UnlockRect ();
    if (FAILED (rets))
      return -1;
    else  return 0;
  }

  void ResetD3dppSurface (int width, int height)  {

    ResetD3dpp (m_d3dpp, m_AttachWindow, width, height);

    HRESULT rets = m_Direct3D9_device->Reset (& m_d3dpp);
    ATLASSERT (SUCCEEDED (rets));
  }

  // global direct3d COM root. 
  static IDirect3D9 *sm_Direct3D9_root;
public:
  static void InitDirect3D (void) {
    if (sm_Direct3D9_root == NULL) {
      sm_Direct3D9_root = Direct3DCreate9 (D3D_SDK_VERSION);
      ATLASSERT (sm_Direct3D9_root != NULL);
    }
  }
  static void UninitDirect3D (void) {
    release_com<IDirect3D9> (sm_Direct3D9_root);
  }

  void ResetD3dpp (D3DPRESENT_PARAMETERS &d3dpp, HWND window, int width, int height)  {

    ZeroMemory (& d3dpp, sizeof (d3dpp));

    d3dpp.BackBufferFormat      = D3DFMT_X8R8G8B8;
    d3dpp.SwapEffect            = D3DSWAPEFFECT_COPY;
    d3dpp.Flags                 = 0;
    d3dpp.hDeviceWindow         = window;
    d3dpp.Windowed              = TRUE;
    d3dpp.PresentationInterval  = D3DPRESENT_INTERVAL_IMMEDIATE; 
    d3dpp.BackBufferHeight = height;
    d3dpp.BackBufferWidth = width;
  }

  int BlitBtoV (POINT *ptDst, POINT *ptSrc, SIZE *szSize)  {
    IDirect3DSurface9 *t;
    D3DSURFACE_DESC lock_desc;
    int rc = -1;
    HRESULT rets = m_Direct3D9_device->GetBackBuffer (0, 0, D3DBACKBUFFER_TYPE_MONO, & t);
    ATLASSERT (SUCCEEDED (rets));
    POINT ptSrcT = { 0, 0};
    POINT *ptPpc = ptSrc ? ptSrc : & ptSrcT;
    // Automatic fit size.
    rets = m_Direct3D9_surface->GetDesc (& lock_desc);
    ATLASSERT (SUCCEEDED (rets));
    SIZE *szPpc = szSize;
    SIZE szSrcT;
    if (szPpc == NULL)  {
      szPpc = & szSrcT;
      szSrcT.cx = lock_desc.Width - ptPpc->x;
      szSrcT.cy = lock_desc.Height - ptPpc->y;
      if (szSrcT.cx <= 0 || (szSrcT.cy <= 0))
        goto  __cleanup;
    } if (1) {
      RECT ac = { ptPpc->x, ptPpc->y, ptPpc->x + szPpc->cx, ptPpc->y + szPpc->cy};
      rets = m_Direct3D9_device->UpdateSurface (m_Direct3D9_surface, & ac, t, ptDst);
      rc = (SUCCEEDED (rets))  ? 0 : -1;
    }
__cleanup:
    rets =  t->Release ();
    ATLASSERT (SUCCEEDED (rets));
    return rc;
  }

  int postRender (RECT *ptDst, RECT *ptSrc)  {
    HRESULT rets = m_Direct3D9_device->Present (ptSrc, ptDst, NULL, NULL);
    if (SUCCEEDED (rets)) 
      return  0;
    else  return -1;
  }
  int postRender (POINT &ptDst, POINT &ptSrc, SIZE &size_)  {
    RECT rcDst = { ptDst.x, ptDst.y, ptDst.x + size_.cx, ptDst.y + size_.cy};
    RECT rcSrc = { ptSrc.x, ptSrc.y, ptSrc.x + size_.cx, ptSrc.y + size_.cy};
    return postRender (& rcDst, & rcSrc);
  }
  // DTOR
 ~CDirectDraw () {
   release_com<IDirect3DSurface9> (m_Direct3D9_surface);
   release_com<IDirect3DDevice9> (m_Direct3D9_device);
  }
  // CTOR
  CDirectDraw (HWND attach, SIZE &VSize, SIZE &BSize) {
    initSettings ();
    // Set the empty brush to avoid repainting the system and cause flickering.
    // SetClassLongPtr (attach, GCLP_HBRBACKGROUND, (LONG_PTR) GetStockObject (NULL_BRUSH)) ;
    m_AttachWindow = attach;
    CreateD3dppSurface (VSize.cx, VSize.cy);
    resetBackSurface (BSize.cx, BSize.cy);
  }
  static int __POW2 (int nc)  {
    int n;
    for  (n= 1; n < nc; n<<=1) {}
    return n;
  }
  // load resourece
  int LoadImage (TCHAR *fname, int width, int height, graph_desc &gd)   {
   {
      IDirect3DTexture9 *tex;
      D3DXIMAGE_INFO infos;
      
      if (FAILED ( D3DXCreateTextureFromFileEx (m_Direct3D9_device, fname, 
        D3DX_DEFAULT_NONPOW2, 
        D3DX_DEFAULT_NONPOW2, 
        D3DX_FROM_FILE, 
        0, 
        D3DFMT_UNKNOWN, 
        D3DPOOL_MANAGED,
        D3DX_DEFAULT, // D3DX_DEFAULT 
        D3DX_DEFAULT, // D3DX_DEFAULT 
        0, //D3DCOLOR_XRGB (0, 0, 0),  
        & infos, NULL, & tex)) )
        return -1;
      {
        D3DLOCKED_RECT vd;
        HRESULT sig;
        DWORD id2;
        // lock it 
        sig = tex->LockRect (0, & vd, NULL, D3DLOCK_READONLY);
        ATLASSERT (SUCCEEDED (sig));

        gd.max_width = infos.Width;
        gd.max_height = infos.Height;
        gd.pitch = __POW2 (vd.Pitch); // step 4 * RGB-depth
        gd.__vptr = (DWORD *) _aligned_malloc (gd.pitch *gd.max_height, 64);

        for (id2 = 0; id2 != infos.Height; id2++)
          memcpy ( & ( ((char *)gd.__vptr)[id2 * gd.pitch]),
             & ( ((char *)vd.pBits)[id2 * vd.Pitch]) ,
             infos.Width *4);

        // unlock., release.
        tex->UnlockRect (0);
        tex->Release ();
        return 0;
      }
      return -1;
    }
  }
  int LoadImage (int resid, int width, int height, graph_desc &gd)   {
   {
      IDirect3DTexture9 *tex;
      D3DXIMAGE_INFO infos;
      
      if (FAILED ( D3DXCreateTextureFromResourceEx (m_Direct3D9_device, NULL, 
        MAKEINTRESOURCE (resid),
        D3DX_DEFAULT_NONPOW2, 
        D3DX_DEFAULT_NONPOW2, 
        D3DX_FROM_FILE, 
        0, 
        D3DFMT_UNKNOWN, 
        D3DPOOL_MANAGED,
        D3DX_DEFAULT, // D3DX_DEFAULT 
        D3DX_DEFAULT, // D3DX_DEFAULT 
        0, //D3DCOLOR_XRGB (0, 0, 0),  
        & infos, NULL, & tex)) )
        return -1;
      {
        D3DLOCKED_RECT vd;
        HRESULT sig;
        DWORD id2;
        // lock it 
        sig = tex->LockRect (0, & vd, NULL, D3DLOCK_READONLY);
        ATLASSERT (SUCCEEDED (sig));

        gd.max_width = infos.Width;
        gd.max_height = infos.Height;
        gd.pitch = __POW2 (vd.Pitch); // step 4 * RGB-depth
        gd.__vptr = (DWORD *) _aligned_malloc (gd.pitch *gd.max_height, 64);

        for (id2 = 0; id2 != infos.Height; id2++)
          memcpy ( & ( ((char *)gd.__vptr)[id2 * gd.pitch]),
             & ( ((char *)vd.pBits)[id2 * vd.Pitch]) ,
             infos.Width *4);

        // unlock., release.
        tex->UnlockRect (0);
        tex->Release ();
        return 0;
      }
      return -1;
    }
  }
  int FreeImage (graph_desc &gd)  {
    if (gd.__vptr != NULL)  {
      _aligned_free (gd.__vptr);
      gd.__vptr = NULL;
    }
    return 0;
  }
  static void CSurfaceBlit (graph_desc &dst, int d_pos_x,  int d_pos_y,
                           graph_desc &src, int s_pos_x,  int s_pos_y,
                                          int w, int h)
  {
    CSurfaceCopy (dst.__vptr, d_pos_x, d_pos_y, dst.pitch, 
            src.__vptr, s_pos_x, s_pos_y, src.pitch,
            w, h);
  }
  static void CSurfaceBlit (graph_desc &dst, POINT &ptDst,
                           graph_desc &src, POINT &ptSrc, SIZE &size)
  {
    CSurfaceBlit (dst, ptDst.x, ptDst.y, 
         src, ptSrc.x, ptSrc.y, size.cx, size.cy);
  }
  static void CSurfaceSingleAlpha (graph_desc &dst, int x,  int y,
                           int w,  int h, 
                           DWORD col, BYTE dst_alpha, BYTE src_alpha)
  {
    CAlphaSingle (dst.__vptr, dst.pitch, x, y, w, h,
            col, dst_alpha, src_alpha);
  }
  static void CSurfaceSingleAlpha (graph_desc &dst, POINT &pt,
                           SIZE &size,
                           DWORD col, BYTE dst_alpha, BYTE src_alpha)
  {
    CSurfaceSingleAlpha (dst, pt.x, pt.y, 
         size.cx, size.cy, col, dst_alpha, src_alpha);
  }
};

IDirect3D9 *CDirectDraw::sm_Direct3D9_root = nullptr;

# endif // !defined(_CDIRECTDRAW_INCLUED_HEADER_CXX_)