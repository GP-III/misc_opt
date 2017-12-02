// Simple map editor for mr-10, support pixel pattern matching / precise search mapping - moecmks
// -------------------------------------------------------------------------------------------------

#include <windows.h>
#include <d3d9.h>
#include <tchar.h>
#include <assert.h>
#include <d3dx9.h>
#include <stdio.h>
#include "resource.h"
#include "stdint.h"
#include "stdbool_msvc.h"

// -------------------------------------------------
// same as name, tile chunk.
// -------------------------------------------------
static struct tile_chunk {
  uint8_t page; // tile 's page.
  uint8_t land; // tile 's land
// indicates whether to display tile
  bool enable; 
};

// -------------------------------------------------
// nametable buffer. 
// -------------------------------------------------
static struct nametable {
  int ntid; // same as name.
  struct tile_chunk tile_arr[32][32]; // [y pos][x pos]
};
static struct nametable_link {
  struct nametable_link *level;
  struct nametable *ntable;
};

// -------------------------------------------------
// mouse infos.
// -------------------------------------------------
static struct mouse_infos {
	bool ctrl_on[2]; // - 
  bool lbut_on_move[2]; // - 
  bool rbut_on_move[2]; // - 
  bool lbut_on[2]; // - 
  bool rbut_on[2]; // - 
  bool mbut_on[2]; // - 
  bool sft_on[2]; // - 
  bool xbut1_on[2]; // - 
  bool xbut2_on[2]; // - 0:current 1:previous 
  
  // mouse current/previous pos
  int16_t mouse_x[2]; // -
  int16_t mouse_y[2]; // -
};

// -------------------------------------------------
// simple mdi window, but e-n-o-u-g-h ^_^.
// -------------------------------------------------
static struct mdi_window {
  const char *caption;
  int uid; /* unique id **/
  
  BOOL min_button;
  BOOL delete_button;
  
  int Z_pri; /* 0:low ... max:high **/
  int Z_pri2; /* 0:low ... max:high **/
  int width; 
  int height;
  
  int cur_posx;
  int cur_posy;
  int cli_startx;
  int cli_starty;

  int *pvbuf;
  int vpitch;
  
  int scr_startx;
  int scr_starty;
  int scr_size;
};
static struct mdi_window_link {
  struct mdi_window_link *level;
  struct mdi_window *window;
};

// -------------------------------------------------
// MISC>
// -------------------------------------------------
static struct pick_helper {
  bool pick_tile;
  bool pick_tid;
};
static struct reset_helper {
  short sx;
  short sy;
  short ex;
  short ey;
  // range alpha.
  uint8_t line_ralpha;
};

// -------------------------------------------------
// paint[section] - service, header is first drawn
// -------------------------------------------------
static struct paserv_link {
  struct paserv_link *level;
  void (* paserv_call)(void *param);
  void *param;
};

// -------------------------------------------------
// --- STATIC GLOBAL VAL.
// -------------------------------------------------
static struct _STATIC_TAG002 {
  // directx obj.
  IDirect3D9 *d3droot; 
  IDirect3DDevice9 *d3ddevice; 
  IDirect3DSurface9 *d3dsurface_main;
  IDirect3DSurface9 *d3dsurface_offscreen;
  // vbackbufer.r (A0R8G88B8).
  int32_t *video_buffer;
  int32_t video_pitch;
  int32_t client_w;
  int32_t client_h;
  // used to replace the default GDI font, the essence is NES 8 *8 dot matrix font
  int32_t *nes_font; // 
  int32_t nesf_mask; // colro aslk.
  int32_t nesf_pitch;
  int32_t nesf_w;
  int32_t nesf_h; 
  // extend function 
  bool fuzzy_matching; //  for plagiarism !!!!
  bool save_incomplete; 
  // some MISC. 
  bool reset_open;
  // nametable buffer size. 
  int ntview_width;
  int ntview_height;
  // some obkec link .
  struct paserv_link *paint_service; 
  struct mouse_infos mouse_infos0;
  struct mdi_window_link *mwinow_link;
  struct nametable_link *ntv_link;
} SGVAL_namespace;

// ---------------------------------------------------------------
// percent per RGB transparent 
// ---------------------------------------------------------------
static  
int32_t alphamix (const int32_t data, const uint8_t purity) {
  
  int8_t x, y, z;
 
  x = data & 0x000000FF; //
  y = data & 0x0000FF00; //
  z = data & 0x00FF0000; // limit range 
    
  x = ((x * purity) >> 8) & 0x000000FF; //
  y = ((y * purity) >> 8) & 0x0000FF00; //
  z = ((z * purity) >> 8) & 0x00FF0000; // strong reduce | FIXME:div/shift
    
  return (x | y | z);  
}

// ---------------------------------------------------------------
// saturation addition
// ---------------------------------------------------------------
static   
int32_t saddudt (const int32_t d0, const int32_t d2) {
  
  int32_t x, y, z;
  
  x = (d0 & 0x000000FF) + (d2 & 0x000000FF); //
  y = (d0 & 0x0000FF00) + (d2 & 0x0000FF00); //
  z = (d0 & 0x00FF0000) + (d2 & 0x00FF0000); // cross add 
  
  if (x > 0x000000FF) x = 0x000000FF; //
  if (y > 0x0000FF00) y = 0x0000FF00; //
  if (z > 0x00FF0000) z = 0x00FF0000; // limit range 
    
  return (x | y | z);  
}

// ---------------------------------------------------------------
// alpha double
// ---------------------------------------------------------------
static 
uint32_t alphamixd (int32_t ag, int32_t ag2, int8_t ag_s, 
         int8_t ag_s2) 
{
  return saddudt ( alphamix (ag, ag_s), 
                   alphamix (ag2, ag_s2));  
}

// ---------------------------------------------------------------
// draw hori line.
// ---------------------------------------------------------------
static __forceinline
void draw_hline ( int32_t *vbuf,
                  int32_t color, int32_t pitchd,   
           int32_t startx, int32_t starty, 
                 int32_t length) {

  if (length == 0)
    return;

  {
    int32_t * vbufsp = & vbuf[startx + starty * pitchd];
    int32_t c = color & 0xFFFFFF;
    int32_t ii = 0;
    
    for (; ii != length; ii++) 
    {
         vbufsp[ii] = c;     
    }
  }
}

// ---------------------------------------------------------------
// draw vert line.
// ---------------------------------------------------------------
static __forceinline
void draw_vline ( int32_t *vbuf,
                  int32_t color, int32_t pitchd,   
           int32_t startx, int32_t starty, 
                 int32_t length) {

  if (length == 0)
    return;

  {
    int32_t * vbufsp = & vbuf[startx + starty * pitchd];
    int32_t c = color & 0xFFFFFF;
    int32_t ii = 0;
    
    for (; ii != length; ii++) 
    {
         vbufsp[ii*pitchd] = c;     
    }
  }
}

// ---------------------------------------------------------------
// draw 8 * 8 mesh line. 
// ---------------------------------------------------------------
static 
void draw_mesh8 (int32_t *vbuf, int32_t pitchd, int32_t color, 
                       int32_t startx, int32_t starty,
                   int32_t hori_nums, int32_t vert_nums)
{
  int ii;

  if (hori_nums <= 0 || vert_nums <= 0)
    return;
  
  // draw hori line. 
  for (ii = 0; ii != hori_nums + 1; ii++)
    draw_hline (vbuf, color, pitchd, startx, starty+ii*9, vert_nums * 9 + 1);
  
  // draw vert line. 
  for (ii = 0; ii != vert_nums + 1; ii++)
    draw_vline (vbuf, color, pitchd, startx+ii*9, starty, vert_nums * 9 + 1);
}

// ---------------------------------------------------------------
// force show cursor. 
// ---------------------------------------------------------------
static 
  void 
  fshow_cursor (void)
{
  do 
  {
    int numb = ShowCursor (TRUE);

    if (numb < 0 || numb == -1)
      continue;
    else 
      break;

  } while (TRUE);
}

// ---------------------------------------------------------------
// force close cursor. 
// ---------------------------------------------------------------
static 
  void  
  fhiddle_cursor (void)
{
  do 
  {
    int numb = ShowCursor (FALSE);

    if (numb >= 0 || numb == -1)
      continue;
    else 
      break;

  } while (TRUE);
}

// ---------------------------------------------------------------
// append paint-service to head.  
// ---------------------------------------------------------------
static 
void pasev_appendto_head (void *proc, void *param)
{
  struct paserv_link *p = malloc (sizeof (struct paserv_link));
  assert (p != NULL);

  p->paserv_call = proc;
  p->param = param;
  
  if (SGVAL_namespace.paint_service == NULL) {
    SGVAL_namespace.paint_service = p;
    p->level = NULL;
  } else {
    p->level = SGVAL_namespace.paint_service->level;
    SGVAL_namespace.paint_service = p;
  }
}

// ---------------------------------------------------------------
// append paint-service to tail.  
// ---------------------------------------------------------------
static 
void pasev_appendto_tail (void *proc, void *param)
{
  struct paserv_link *p = malloc (sizeof (struct paserv_link));
  assert (p != NULL);

  p->level = NULL;
  p->paserv_call = proc;
  p->param = param;

  if (SGVAL_namespace.paint_service == NULL)
    {
      SGVAL_namespace.paint_service = p;
    }
  else 
    {
      struct paserv_link *r, *v; 
      v = SGVAL_namespace.paint_service;
      r;

      for (; v; v = (r = v)->level)
        ;  r->level = p;
    }

}

// ---------------------------------------------------------------
// free paint-service 
// ---------------------------------------------------------------
static 
bool pasev_free_specify (void *proc)
{
  if (SGVAL_namespace.paint_service != NULL)
    {
      struct paserv_link *r, *v; 
      v = SGVAL_namespace.paint_service;
      r = NULL;

      for (; v; v = (r = v)->level) {
        if (v->paserv_call == proc) 
        {
          // remov e it. 
          if (r == NULL) {
            SGVAL_namespace.paint_service = NULL;
          } else {
            r->level = v->level;
          }
          
          free (v);
          return TRUE;
        }
      }
    }
  return FALSE;
}

// ---------------------------------------------------------------
// draw NES 8*8 dot maritx font
// ---------------------------------------------------------------
static 
void __cdecl ntprintf (int32_t *vbuf, int32_t pitchd, 
                     int32_t x, int32_t y, 
                           int32_t color, const char *format, ...)
{
  char buffer[1024];
  va_list args; 

  /* for stack arg */
  va_start (args, format);
  (void) vsprintf (& buffer[0], format, args);
  va_end (args);

  {
    int32_t x0  = x;
    int32_t y0  = y;
    int32_t *headfont = NULL;
    int32_t *vbufsp = NULL;
    int32_t c = color & 0xFFFFFF;
    int32_t ii, ij;
    char *chrptr = & buffer[0];

    do {

      char ch = *chrptr++;
      ch &= 0x7F; /* no deal ascii2 **/

      if (ch == 0)
        break;
      if (ch == '\n') {
        x0 = x;
        y0+= 8;
        continue;
      }

      /* fecth pixel to draw. **/
      headfont = & SGVAL_namespace.nes_font[(ch & 0x0F) * 8+ ((ch & 0xF0) >> 4) * SGVAL_namespace.nesf_pitch * 8];
      vbufsp = & vbuf[x0 + y0 * pitchd];

      for (ii=0; ii != 8; ii++) 
      {
        for (ij=0; ij != 8; ij++) 
        {
          if ( (headfont[ij+ii*SGVAL_namespace.nesf_pitch] & 0xFFFFFF) != SGVAL_namespace.nesf_mask)
            vbufsp[ij+ii*pitchd] = c;
        }
      }

      x0 += 8;
    } while (TRUE);
  }
}

// ---------------------------------------------------------------
// load texture
// ---------------------------------------------------------------
static 
IDirect3DTexture9 *load_texture (const char *name, int width, int height) 
{
  IDirect3DTexture9 * tex = NULL;

  if (SGVAL_namespace.d3ddevice == NULL)
    return NULL;

  if (S_OK !=  D3DXCreateTextureFromFileExA (SGVAL_namespace.d3ddevice, name,
                                      width,
                                      height,
                                      D3DX_FROM_FILE, /* D3DX_FROM_FILE, */
                                      0,
                                      D3DFMT_UNKNOWN,
                                      D3DPOOL_MANAGED,
                                      D3DX_DEFAULT, /* D3DX_DEFAULT */
                                      D3DX_DEFAULT, /* D3DX_DEFAULT */
                                      D3DCOLOR_XRGB (0, 0, 0), 
                                      NULL, 
                                      NULL,
                                    & tex))
    return NULL;
  else 
    return tex;
}

// ---------------------------------------------------------------
// load texture
// ---------------------------------------------------------------
static 
void *load_texture_realloc_unalign (const char *name, int width, int height) {

  IDirect3DTexture9 *tex = NULL;
  D3DLOCKED_RECT tex_infos;
  HRESULT hr = S_OK;
  DWORD *new_block = NULL;
  DWORD *tmp_block = NULL;
  DWORD ii;
  tex = load_texture (name, width, height);
  assert (tex != NULL);

  // lock 
  hr = IDirect3DTexture9_LockRect (tex, 0, & tex_infos, NULL, D3DLOCK_READONLY);
  assert (hr == S_OK);

  // memcpy new block. copy 
  new_block = malloc (4*width*height);
  assert (new_block != NULL);

  tmp_block = tex_infos.pBits;

  for (ii= 0; ii != height; ii++)
    memcpy (& new_block[ii*height], & tmp_block[ii*(tex_infos.Pitch >> 2)], width << 2);

  // unlock., release.
  IDirect3DTexture9_UnlockRect (tex, 0);
  IDirect3DTexture9_Release (tex);

  return new_block;
}

// paint edit-mesh. 
void paint_service4 (void *param) {
  
  DWORD ix;
  DWORD iy;
  DWORD ix0, iy0;
  DWORD total;
  DWORD *chr;
  const DWORD base = CLIENT_SIZE_PITCH + 1;

  if (SGVAL_namespace.chr_table == NULL)
    return;

  for (iy =0; iy != 32; iy++)
    {
      for (ix =0; ix != 32; ix++)
      {
        struct tile_chunk *ptchunk = & SGVAL_namespace.tile_edit[iy*32+ix];
        
        if (ptchunk->enable == FALSE || SGVAL_namespace.chr_table == NULL)
          continue;
        
        total = base + ix + iy * CLIENT_SIZE_PITCH;
        chr = & SGVAL_namespace.chr_table[(ptchunk->index&0x0F)*8+((ptchunk->index&0xF0)>>4)*128*8];
        
        for (iy0 = 0; iy0 != 8; iy0++)
        {
          for (ix0 = 0; ix0 != 8; ix0++) 
          {
            SGVAL_namespace.vbackbuf[total+CLIENT_SIZE_PITCH*iy*8+ix*8+ix0+CLIENT_SIZE_PITCH*iy0] = chr[128*iy0+ix0];
          }
        }
      }
    }
  // reset tile 84, 458 
  if (SGVAL_namespace.tile_reset.enable == TRUE && SGVAL_namespace.chr_table != NULL)
  {
        chr = & SGVAL_namespace.chr_table[(SGVAL_namespace.tile_reset.index&0x0F)*8+((SGVAL_namespace.tile_reset.index&0xF0)>>4)*128*8];
        
        for (iy0 = 0; iy0 != 8; iy0++)
        {
          for (ix0 = 0; ix0 != 8; ix0++) 
          {
            SGVAL_namespace.vbackbuf[84+ix0+(iy0+458)*CLIENT_SIZE_PITCH] = chr[128*iy0+ix0];
          }
        }
  }
  
};

// paint mesh. 
void paint_service0 (void *param) {
  
  DWORD ii;


  for (ii=0; ii!=33; ii++) 
    draw_lineY (0x00ffde, 9*ii, 0, 289);

  for (ii=0; ii!=33; ii++) 
    draw_lineX (0x00ffde, 0, 9*ii, 289);

  for (ii=0; ii!=33; ii++) 
    draw_lineY (0x00ffde, 300+9*ii, 0, 289);

  for (ii=0; ii!=33; ii++) 
    draw_lineX (0x00ffde, 300, 9*ii, 289);

  for (ii=0; ii!=17; ii++) 
    draw_lineY (0x00ffde, 9*ii, 300, 145);

  for (ii=0; ii!=17; ii++) 
    draw_lineX (0x00ffde, 0, 300+9*ii, 145);

# if 1
  printf__0 (0, 500, -1, "mouse-pos X:%d mouse-pos Y:%d\n\
lbut[0]:%d lbut[1]:%d\n\
rbut[0]:%d rbut[1]:%d\n\
lbut_move[0]:%d lbut_move[1]:%d\n\
rbut_move[0]:%d rbut_move[1]:%d", SGVAL_namespace.mouse_infos0.mouse_x[0], SGVAL_namespace.mouse_infos0.mouse_y[0],
      SGVAL_namespace.mouse_infos0.lbut_on[0], SGVAL_namespace.mouse_infos0.lbut_on[1],
      SGVAL_namespace.mouse_infos0.rbut_on[0], SGVAL_namespace.mouse_infos0.rbut_on[1],
      SGVAL_namespace.mouse_infos0.lbut_on_move[0], SGVAL_namespace.mouse_infos0.lbut_on_move[1],
      SGVAL_namespace.mouse_infos0.rbut_on_move[0], SGVAL_namespace.mouse_infos0.rbut_on_move[1]
      );
# endif
  printf__0 (0, 458, -1, "RESET TILE");
  for (ii=0; ii!=2; ii++) 
    draw_lineX (0x00ffde, 83, ii*9+457, 10);
  for (ii=0; ii!=2; ii++) 
    draw_lineY (0x00ffde, 9*ii+83, 457, 10);
  
  printf__0 (0, 469, -1, "FUZZY MATE");
  for (ii=0; ii!=2; ii++) 
    draw_lineX (0x00ffde, 83, ii*9+468, 10);
  for (ii=0; ii!=2; ii++) 
    draw_lineY (0x00ffde, 9*ii+83, 468, 10);
  
  
  printf__0 (0, 480, -1, "LOST  SAVE");
  for (ii=0; ii!=2; ii++) 
    draw_lineX (0x00ffde, 83, ii*9+479, 10);
  for (ii=0; ii!=2; ii++) 
    draw_lineY (0x00ffde, 9*ii+83, 479, 10);
  
  
};

// paint CHR TABEL. 
void paint_service1 (void *param) {
  
 // draw CHR TABLE . START POS(0.300)
  DWORD ix, iy, ix0, iy0;
  DWORD  *pChr = SGVAL_namespace.chr_table;
  const DWORD base = 301 *CLIENT_SIZE_PITCH + 1;
  DWORD total;

     for (iy = 0; iy != 16; iy++)
     {
        for (ix = 0; ix != 16; ix++)
        {
          total = base + ix + iy * CLIENT_SIZE_PITCH;

            for (iy0 = 0; iy0 != 8; iy0++)
            {
              for (ix0 = 0; ix0 != 8; ix0++) 
              {
                SGVAL_namespace.vbackbuf[total+CLIENT_SIZE_PITCH*iy*8+ix*8+ix0+CLIENT_SIZE_PITCH*iy0] = pChr[128*iy*8+ix*8+ix0+128*iy0];
              }
            }
              
        }
     }

 

  


}
// draw- pick's tile .
void paint_service2 (void *param) {
  
 // draw CHR TABLE . START POS(0.300)
  DWORD x;
  DWORD y;
  DWORD *chr = param;

  for (y = 0; y != 8; y++) {
       for (x = 0; x != 8; x++)
         SGVAL_namespace.vbackbuf[SGVAL_namespace.mouse_infos0.mouse_x[0]+x
           + (SGVAL_namespace.mouse_infos0.mouse_y[0] + y) * CLIENT_SIZE_PITCH] = chr[x+y*128];
    }
}

// Draw reset alpha line
void paint_service2_1 (void *param) {
  
  // DWORD *chr = param;
  DWORD mrgb = alphamixd (0x00ffde, 0xff0000, SGVAL_namespace.reset_line_ralpha, SGVAL_namespace.reset_line_ralpha ^ 0xFF);
  // draw alpha rect. 
  DWORD sx;// = SGVAL_namespace.reset_start_x - SGVAL_namespace.mouse_infos0.mouse_x[0];
  DWORD sy;// = SGVAL_namespace.reset_start_y - SGVAL_namespace.mouse_infos0.mouse_y[0];
  DWORD ex;// = SGVAL_namespace.reset_start_x;
  DWORD ey;// = SGVAL_namespace.reset_start_y;
  DWORD ix = SGVAL_namespace.mouse_infos0.mouse_x[0] > 60000 ? 0 : SGVAL_namespace.mouse_infos0.mouse_x[0];
  DWORD iy = SGVAL_namespace.mouse_infos0.mouse_y[0] > 281 ? 281 : SGVAL_namespace.mouse_infos0.mouse_y[0];
  ix = ix > 281 ? 281 : ix;

  if (SGVAL_namespace.reset_start_x > ix) {
    sx = ix;
    ex = SGVAL_namespace.reset_start_x;
  } else {
    sx = SGVAL_namespace.reset_start_x;
    ex = ix;
  }
  if (SGVAL_namespace.reset_start_y > iy) {
    sy = iy;
    ey = SGVAL_namespace.reset_start_y;
  } else {
    sy = SGVAL_namespace.reset_start_y;
    ey = iy;
  }
  // calc coord. limit. 
  sx = sx - sx % 9;
  sy = sy - sy % 9;
  ex = ex + 9 - ex % 9;
  ey = ey + 9 - ey % 9;

  SGVAL_namespace.reset_sx = sx;
  SGVAL_namespace.reset_sy = sy;
  SGVAL_namespace.reset_ex = ex;
  SGVAL_namespace.reset_ey = ey;

  // draw line hori .
  draw_lineX (mrgb, sx, sy, ex - sx);
  draw_lineX (mrgb, sx, ey, ex - sx + 1);

  // draw line vert . 
  draw_lineY (mrgb, sx, sy, ey - sy);
  draw_lineY (mrgb, ex, sy, ey - sy + 1);

  SGVAL_namespace.reset_line_ralpha += 2;
}

static 
void check_point_inrange0 (void) {

  if (SGVAL_namespace.chr_table != NULL)
    {
 // draw CHR TABLE . START POS(0.300)// PROC:145
      DWORD ii;
      static POINT pt;

      if (SGVAL_namespace.mouse_infos0.mouse_x[0] > 144)
        return;
      else if (SGVAL_namespace.mouse_infos0.mouse_y[0] < 300 || SGVAL_namespace.mouse_infos0.mouse_y[0] > 444)
        return;
      // check hit mesh-line. 
      for (ii = 0; ii != 17; ii++) 
        {
          if (SGVAL_namespace.mouse_infos0.mouse_y[0] == 300 + ii * 9)
            return;
        }
      for (ii = 0; ii != 17; ii++) 
        {
          if (SGVAL_namespace.mouse_infos0.mouse_x[0] == ii * 9)
            return;
        }
      // check select point. X - y
      pt.x = SGVAL_namespace.mouse_infos0.mouse_x[0] / 9;
      pt.y = (SGVAL_namespace.mouse_infos0.mouse_y[0] - 300) / 9;

      // insert - pasev= pri is highset .
      fhiddle_cursor ();
      SGVAL_namespace.pick_tile = TRUE;
      SGVAL_namespace.pick_tid = (BYTE) (pt.x + pt.y * 16);
      pasev_appendto_tail (paint_service2, & SGVAL_namespace.chr_table[128*pt.y*8+pt.x*8]);



    }
}

static 
void check_point_inrange1 (void) {

  if (SGVAL_namespace.chr_table != NULL)
    {
 // draw CHR TABLE . START POS(0.300)// PROC:145
#if 0
      DWORD ii;
      static POINT pt;

      if (SGVAL_namespace.mouse_infos0.mouse_x[0] > 144)
        return;
      else if (SGVAL_namespace.mouse_infos0.mouse_y[0] < 300 || SGVAL_namespace.mouse_infos0.mouse_y[0] > 444)
        return;
      // check hit mesh-line. 
      for (ii = 0; ii != 17; ii++) 
        {
          if (SGVAL_namespace.mouse_infos0.mouse_y[0] == 300 + ii * 9)
            return;
        }
      for (ii = 0; ii != 17; ii++) 
        {
          if (SGVAL_namespace.mouse_infos0.mouse_x[0] == ii * 9)
            return;
        }
      // check select point. X - y
      pt.x = SGVAL_namespace.mouse_infos0.mouse_x[0] / 9;
      pt.y = (SGVAL_namespace.mouse_infos0.mouse_y[0] - 300) / 9;

      // insert - pasev= pri is highset .
      fhiddle_cursor ();
      SGVAL_namespace.pick_tile = TRUE;
      SGVAL_namespace.pick_tid = pt.x + pt.y * 16;
      pasev_appendto_tail (paint_service2, & SGVAL_namespace.chr_table[128*pt.y*8+pt.x*8]);
#else 
      short x = (short) SGVAL_namespace.mouse_infos0.mouse_x[0];
      short y = (short) SGVAL_namespace.mouse_infos0.mouse_y[0];

      if ( x < 0 || y < 0)
        return;

      if ( x < 289 && y < 289)
        {
          fhiddle_cursor ();
          SGVAL_namespace.reset_open = TRUE;
          SGVAL_namespace.reset_start_x = SGVAL_namespace.mouse_infos0.mouse_x[0];
          SGVAL_namespace.reset_start_y = SGVAL_namespace.mouse_infos0.mouse_y[0];
          pasev_appendto_tail (paint_service2_1, NULL);
        }


#endif 
    }
}

static 
void free_point_inrange1 (void) {

  if (SGVAL_namespace.reset_open == TRUE)
  {
    
    BOOL s;
    DWORD ix = SGVAL_namespace.reset_sx / 9;
    DWORD iy = SGVAL_namespace.reset_sy / 9;
    DWORD ex = SGVAL_namespace.reset_ex / 9;
    DWORD ey = SGVAL_namespace.reset_ey / 9;
    DWORD tx = ix;
    // calc range's tile. 
    SGVAL_namespace.reset_open = FALSE;

    if ( (GetAsyncKeyState ('A') & 0x8000) )
      goto __s;
    
    if (SGVAL_namespace.tile_reset.enable == FALSE || (GetAsyncKeyState ('C') & 0x8000) )
      {
        // fill disable .
        // x = 
        for (; iy != ey; iy++)
        {        for (tx = ix; tx != ex; tx++)

        {
          SGVAL_namespace.tile_edit[iy*32+tx].enable = FALSE;
        }
        }
      }
    else 
      {
        // pick tid. fill it.
        // 
        BYTE tid = SGVAL_namespace.tile_reset.index;
        for (; iy != ey; iy++)
        {        for (tx = ix; tx != ex; tx++)

        {
                    SGVAL_namespace.tile_edit[iy*32+tx].enable = TRUE;
          SGVAL_namespace.tile_edit[iy*32+tx].index = tid;
        }
        }


      }

    __s:
    fshow_cursor ();
    while (pasev_free_specify (paint_service2_1) == TRUE)
    {
    }


  }
  
}

static 
void free_point_inrange0 (void) {

  if (SGVAL_namespace.chr_table != NULL)
    {
 // draw CHR TABLE . START POS(0.300)// PROC:145
      DWORD ii;
      static POINT pt;
      // insert - pasev= pri is highset .
      fshow_cursor ();
      pasev_free_specify (paint_service2);


      if (SGVAL_namespace.pick_tile == TRUE)
      {
        if (SGVAL_namespace.mouse_infos0.mouse_x[0] < 289 && SGVAL_namespace.mouse_infos0.mouse_y[0] < 289)
        {
          // Place the corresponding position

                // check hit mesh-line. 
      for (ii = 0; ii != 17; ii++) 
        {
          if (SGVAL_namespace.mouse_infos0.mouse_y[0] == ii * 9)
            goto _s;
        }
      for (ii = 0; ii != 17; ii++) 
        {
          if (SGVAL_namespace.mouse_infos0.mouse_x[0] == ii * 9)
            goto _s;
        }

            // check select point. X - y
      pt.x = SGVAL_namespace.mouse_infos0.mouse_x[0] / 9;
      pt.y = (SGVAL_namespace.mouse_infos0.mouse_y[0]) / 9;

      SGVAL_namespace.tile_edit[pt.x+pt.y*32].index = SGVAL_namespace.pick_tid;
      SGVAL_namespace.tile_edit[pt.x+pt.y*32].enable = TRUE;
        }
        else if (SGVAL_namespace.mouse_infos0.mouse_x[0] >= 84  && SGVAL_namespace.mouse_infos0.mouse_x[0] <= (84+8) 
              && SGVAL_namespace.mouse_infos0.mouse_y[0] >= 458  && SGVAL_namespace.mouse_infos0.mouse_x[0] <= (458+8) )
        {
      SGVAL_namespace.tile_reset.index = SGVAL_namespace.pick_tid;
      SGVAL_namespace.tile_reset.enable = TRUE;
        }
            
      }
      _s:
      SGVAL_namespace.pick_tile = FALSE;
    }
}


static 
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
switch (message)
     {
       case WM_CREATE:
         return 0 ;
   
       case WM_MOUSEMOVE:
         SGVAL_namespace.mouse_infos0.ctrl_on[1] = SGVAL_namespace.mouse_infos0.ctrl_on[0];
         SGVAL_namespace.mouse_infos0.lbut_on_move[1] = SGVAL_namespace.mouse_infos0.lbut_on_move[0];
         SGVAL_namespace.mouse_infos0.rbut_on_move[1] = SGVAL_namespace.mouse_infos0.rbut_on_move[0];
         SGVAL_namespace.mouse_infos0.sft_on[1] = SGVAL_namespace.mouse_infos0.sft_on[0];
         SGVAL_namespace.mouse_infos0.mbut_on[1] = SGVAL_namespace.mouse_infos0.mbut_on[0];
         SGVAL_namespace.mouse_infos0.xbut1_on[1] = SGVAL_namespace.mouse_infos0.xbut1_on[0];
         SGVAL_namespace.mouse_infos0.xbut2_on[1] = SGVAL_namespace.mouse_infos0.xbut2_on[0];
         SGVAL_namespace.mouse_infos0.mouse_x[1] = SGVAL_namespace.mouse_infos0.mouse_x[0];
         SGVAL_namespace.mouse_infos0.mouse_y[1] = SGVAL_namespace.mouse_infos0.mouse_y[0];
		     SGVAL_namespace.mouse_infos0.ctrl_on[0] = wparam & MK_CONTROL;
		     SGVAL_namespace.mouse_infos0.lbut_on_move[0] = wparam & MK_LBUTTON;
		     SGVAL_namespace.mouse_infos0.rbut_on_move[0] = wparam & MK_RBUTTON;
		     SGVAL_namespace.mouse_infos0.mbut_on[0] = wparam & MK_MBUTTON;
		     SGVAL_namespace.mouse_infos0.sft_on[0] = wparam & MK_SHIFT;
		     SGVAL_namespace.mouse_infos0.xbut1_on[0] = wparam & MK_XBUTTON1;
		     SGVAL_namespace.mouse_infos0.xbut2_on[0] = wparam & MK_XBUTTON1;
         SGVAL_namespace.mouse_infos0.mouse_x[0] = lparam & 0xFFFF;
	       SGVAL_namespace.mouse_infos0.mouse_y[0] = (lparam & 0xFFFF0000) >> 16;
         break;

	     case WM_LBUTTONDOWN:
         SGVAL_namespace.mouse_infos0.lbut_on[1] = SGVAL_namespace.mouse_infos0.lbut_on[0];
         SGVAL_namespace.mouse_infos0.lbut_on[0] = TRUE;
         check_point_inrange0 ();
		     break;

	     case WM_LBUTTONUP:
         SGVAL_namespace.mouse_infos0.lbut_on[1] = SGVAL_namespace.mouse_infos0.lbut_on[0];
         SGVAL_namespace.mouse_infos0.lbut_on[0] = FALSE;
         free_point_inrange0 ();
		     break;

	     case WM_RBUTTONDOWN:
         SGVAL_namespace.mouse_infos0.rbut_on[1] = SGVAL_namespace.mouse_infos0.rbut_on[0];
         SGVAL_namespace.mouse_infos0.rbut_on[0] = TRUE;
         check_point_inrange1 ();
		     break;

	     case WM_RBUTTONUP:
         SGVAL_namespace.mouse_infos0.rbut_on[1] = SGVAL_namespace.mouse_infos0.rbut_on[0];
         SGVAL_namespace.mouse_infos0.rbut_on[0] = FALSE;
         free_point_inrange1 ();
		     break;

       case WM_COMMAND:
         if (LOWORD(wparam) == IDMR_LOAD_CHR) {
           OPENFILENAME ofn;  
           static TCHAR file_name[MAX_PATH];
           HRESULT hr = S_OK;
           DWORD ii;
           ZeroMemory (& ofn, sizeof (ofn));
           ZeroMemory (& file_name[0], sizeof (file_name));

           ofn.lStructSize = sizeof(OPENFILENAME);
           ofn.hwndOwner = NULL;
           ofn.lpstrFilter = _T ("png files(*.png)\0*.png\0jpg files(*.jpg)\0*.jpg\0jpeg files(*.jpeg)\0*.jpeg\0bmp files(*.bmp)\0*.bmp\0tga files(*.tga)\0*.tga\0All files(*.*)\0*.*\0");
           ofn.nFilterIndex = 1;
           ofn.lpstrFile = & file_name[0];
           ofn.nMaxFile = sizeof (file_name);
           ofn.lpstrInitialDir = NULL;
           ofn.lpstrTitle = _T ("load chr0 file");
           ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

           if (GetOpenFileName (& ofn) != FALSE) { 
             D3DXIMAGE_INFO img_infos;
             IDirect3DTexture9 *tex = NULL;

             hr = D3DXCreateTextureFromFileEx (SGVAL_namespace.d3ddevice, & file_name[0],
                                      D3DX_DEFAULT_NONPOW2,
                                      D3DX_DEFAULT_NONPOW2,
                                      D3DX_FROM_FILE, /* D3DX_FROM_FILE, */
                                      0,
                                      D3DFMT_UNKNOWN,
                                      D3DPOOL_MANAGED,
                                      D3DX_DEFAULT, /* D3DX_DEFAULT */
                                      D3DX_DEFAULT, /* D3DX_DEFAULT */
                                      D3DCOLOR_XRGB (0, 0, 0), 
                                    & img_infos, 
                                      NULL,
                                    & tex);
             assert (hr == S_OK);

             if (img_infos.Width != 128 || img_infos.Height != 128)
               {
                 MessageBox (NULL, _T ("image size error."), _T ("load chr file"), MB_ICONERROR);
                 IDirect3DTexture9_Release (tex);

                 return 0;
               }
             else 
               {
                 D3DLOCKED_RECT tex_infos;
                 PDWORD ptex = NULL;
                 hr = IDirect3DTexture9_LockRect (tex, 0, & tex_infos, NULL, D3DLOCK_READONLY);
                 assert (hr == S_OK);
                 ptex = tex_infos.pBits;

                 if (SGVAL_namespace.chr_table == NULL)
                   {
                     // allov new . 
                     SGVAL_namespace.chr_table = malloc (128*128*4);
                     assert (SGVAL_namespace.chr_table != NULL);

                     // draw to mesh. 
                     pasev_appendto_tail (paint_service1, NULL);
                   }
                 // memcpy .
                 for (ii= 0; ii != 128; ii++)
                   memcpy (& SGVAL_namespace.chr_table[ii*128], & ptex[tex_infos.Pitch/4*ii], 512);

                 IDirect3DTexture9_UnlockRect (tex, 0);
                 IDirect3DTexture9_Release (tex);
               }




           } 
         }
         return 0;

       case WM_DESTROY:
         PostQuitMessage (0) ;
         return 0 ;
     }
   return DefWindowProc (hwnd, message, wparam, lparam) ;
}

void update_surface (void) {

  HRESULT hr = S_OK;
  D3DLOCKED_RECT surface_rect;
  DWORD *surface_ptr = NULL;
  DWORD surface_pitch32 = 0;
  DWORD ii;

  // lock main surface. 
  hr = IDirect3DSurface9_LockRect (SGVAL_namespace.d3dsurface_main, & surface_rect, NULL, D3DLOCK_DISCARD);
  assert (hr == S_OK);

  surface_ptr = surface_rect.pBits;
  surface_pitch32 = surface_rect.Pitch >> 2;

  // copy to vram. 
  for (ii = 0; ii != CLIENT_SIZE_HEIGHT; ii++)
    memcpy (& surface_ptr[surface_pitch32*ii], & SGVAL_namespace.vbackbuf[CLIENT_SIZE_PITCH*ii], CLIENT_SIZE_WIDTH << 2);

  // unlock main. 
  hr = IDirect3DSurface9_UnlockRect (SGVAL_namespace.d3dsurface_main);
  assert (hr == S_OK);

  hr = IDirect3DDevice9_Present (SGVAL_namespace.d3ddevice, 0, 0, 0, 0);
  assert (hr == S_OK);
}




void mapedit_main (void) {

  struct paserv_list *cur_paint;
  ZeroMemory (& SGVAL_namespace.vbackbuf[0], sizeof (SGVAL_namespace.vbackbuf));
  
  // poll - paint-list exec paint-proc .
  // prev_paint = SGVAL_namespace.paint_service.chain->level;
  cur_paint =  SGVAL_namespace.paint_service.level;

  for (; cur_paint != NULL; cur_paint = cur_paint->level)
    cur_paint->paserv_call (cur_paint->param);

  // over. update vram. 
  update_surface ();
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
    static const TCHAR class_name[] = _T ("simple editer for mr-10");
    HRESULT hr = S_OK;
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory (& wndclass, sizeof (WNDCLASS));
    ZeroMemory (& d3dpp, sizeof (D3DPRESENT_PARAMETERS));
    ZeroMemory (& SGVAL_namespace, sizeof (SGVAL_namespace));

    // register wndclass, create window.
    wndclass.hCursor = LoadCursor (NULL, IDI_APPLICATION);
    wndclass.hInstance = hInstance;
    wndclass.lpfnWndProc  = WndProc;
    wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wndclass.lpszMenuName = MAKEINTRESOURCE (IDRM_MENU);
    wndclass.lpszClassName = class_name ;

    if (! RegisterClass (& wndclass))
         {
            MessageBox ( NULL, _T ("This program requires Windows NT!"), 
                      class_name, MB_ICONERROR) ;
            return 0 ;
         }

    hwnd = CreateWindow( class_name, _T ("simple editer for mr-10"), 
            WS_SYSMENU | WS_MAXIMIZEBOX & ~WS_SIZEBOX | WS_MAXIMIZE, 
            72, 64, 0, 0,
           NULL,  NULL,  hInstance,   NULL) ;     

    /// create directx object/load texture.
    SGVAL_namespace.d3droot = Direct3DCreate9 (D3D_SDK_VERSION);
    assert (SGVAL_namespace.d3droot != NULL);

    d3dpp.BackBufferWidth      = 0;
    d3dpp.BackBufferHeight     = 0;
    d3dpp.BackBufferFormat     = D3DFMT_X8R8G8B8;
    d3dpp.SwapEffect           = D3DSWAPEFFECT_DISCARD;
    d3dpp.Flags                = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    d3dpp.hDeviceWindow        = hwnd;
    d3dpp.Windowed             = TRUE;
    // Vsync open, In fact, there is no need to refresh 60 times per second, the 30 is enough
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; 

    hr = IDirect3D9_CreateDevice (SGVAL_namespace.d3droot, 0, D3DDEVTYPE_HAL, hwnd, 
                           D3DCREATE_SOFTWARE_VERTEXPROCESSING, & d3dpp, 
                        & SGVAL_namespace.d3ddevice);
    assert (hr == S_OK);
    hr = IDirect3DDevice9_GetBackBuffer (SGVAL_namespace.d3ddevice, 0, 0, D3DBACKBUFFER_TYPE_MONO, & SGVAL_namespace.d3dsurface_main);
    assert (hr == S_OK);

    SGVAL_namespace.nesfont = load_texture_realloc_unalign ("nesfont.png", FONT_SIZE_WIDTH, FONT_SIZE_HEIGHT);
    assert (SGVAL_namespace.nesfont != NULL);

    pasev_appendto_head (paint_service0, NULL);
    pasev_appendto_tail (paint_service4, NULL);



    ShowWindow (hwnd, SW_MAXIMIZE);
    UpdateWindow (hwnd) ;
     
  do 
  {
    if (PeekMessage (& msg, NULL, 0, 0, PM_REMOVE) == TRUE)
      {
        if (msg.message == WM_QUIT)
          break;
        TranslateMessage (& msg);
        DispatchMessage (& msg);
      }
    else 
      {
        mapedit_main();
      }
  } while (TRUE);

  return msg.wParam;
}