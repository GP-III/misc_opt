# include "nes_dc.h"
# include <windows.h>
# include <MMSystem.h>
# include <d3d9.h>
# include <XAudio2.h>
# include <dinput.h>
# include <DShow.h>
# include <d3dx9.h>
# include <Shlwapi.h>
# include <intrin.h>
# include <assert.h>

# define NES_SCREEN_WIDTH 256
# define NES_SCREEN_HEIGHT 224
# define NES_MERGING_EDGES 8
# define NES_SCREEN_PITCH_DWORD ( (MERGING_EDGES)*2+NES_SCREEN_WIDTH)

# if defined (_MSC_VER)
#  pragma warning (disable:4995)
#  pragma warning (disable:4996)
# endif 

struct sp_chunk {
  struct sp_chunk_item set[8];
  int chan;
};
struct ns_rendercc {
  uint32_t *pstvbbuf;
  uint8_t *pstvbatr;
  uint8_t sn_x;
  uint8_t sn_y;
  uint8_t sn_page;
};
struct ns_render {
  uint32_t vbbuf[272*256];
  uint8_t vbatr[272*256];
  uint32_t ld_lut[256]; /* alpha-mapper table */
  uint8_t lland_bg, lland_un;
  uint8_t lldp_bg, lldp_un;
  uint8_t sn_page;
  uint8_t ss_x;
  uint8_t ss_y; 
  int32_t       ctl_mask;
  struct bg_chunk     map[4096]; /* map-object */  
  struct sp_chunk       sprite[256]; /* sprite-object */
  struct tile     teb[65536];
  void *memblock; /* for align_malloc's free **/
};

struct dummy_struct {
  IXAudio2 *sound_drv;
  IXAudio2MasteringVoice *sound_drv_;
  IDirectInput8 *input_drv;
  IDirectInputDevice8 *input;
  IDirect3D9 *video_drv;    
  IDirect3DDevice9 *video;
  DWORD *vbbuf; /* (256+8+8)+(224+8+8)*/
  WCHAR szPath[MAX_PATH]; /* with \ */
  struct ns_render *nsr;
  void *params;
  void * (* cbinit) (void); 
  void (* cbloop) (struct nes_joypad *joypad, uint64_t ticks, void *params);
  void (* cbdtor) (void *params);
} globals;

struct ns_texture {
  int width;
  int height;
  int pitch;
  void *pptr;
};
struct sys_media {
  IGraphBuilder* graphBuilder;
  IMediaControl* mediaControl;
  IMediaPosition* mediaPosition;
  IMediaEvent *mediaEvent;
};

/* directx base object init */
static 
int directx_drv_init (void) {

  HRESULT sig = CoInitializeEx (null, COINIT_MULTITHREADED);
  assert (SUCCEEDED (sig));

  globals.video_drv = Direct3DCreate9 (D3D_SDK_VERSION);
  assert (globals.video_drv != NULL);

  sig = XAudio2Create (& globals.sound_drv, 0, XAUDIO2_DEFAULT_PROCESSOR);
  assert (SUCCEEDED (sig));

  sig = DirectInput8Create (GetModuleHandle (NULL), DIRECTINPUT_VERSION, 
    & IID_IDirectInput8, (void **)& globals.input_drv, null);
  assert (SUCCEEDED (sig));

  sig = IXAudio2_CreateMasteringVoice (globals.sound_drv, & globals.sound_drv_, 
    XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
  assert (SUCCEEDED (sig));
  
  return 0;
}

static 
int directx_drv_uninit (void) {

  HRESULT sig = IDirect3D9_Release (globals.video_drv);
  assert (SUCCEEDED (sig));

  sig = IDirectInput8_Release (globals.input_drv);
  assert (SUCCEEDED (sig));

  /*sig =*/ IXAudio2MasteringVoice_DestroyVoice (globals.sound_drv_);
  //assert (SUCCEEDED (sig));

  sig = IXAudio2_Release (globals.sound_drv);
  assert (SUCCEEDED (sig));

  CoUninitialize();
  return 0;
}

static 
BOOL resize_client (HWND window, int width, int height, 
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

  return SetWindowPos (window, NULL, xs, ys, x, y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
}

static 
  LRESULT CALLBACK private_wingui_cb (HWND window, UINT msg, WPARAM w, LPARAM l)
{
  if (msg != WM_DESTROY)
    return DefWindowProc (window, msg, w, l); 
  PostQuitMessage (0);
  return 0;
}

static  HWND 
create_nesSize_window (void) {

  HWND wnd = null;
  HMODULE mod = GetModuleHandle (null);
  WNDCLASS wndclass;
  ZeroMemory (& wndclass, sizeof (wndclass));

  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = private_wingui_cb;
  wndclass.hInstance = mod;
  wndclass.hIcon = LoadIcon (null, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor (null, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
  wndclass.lpszClassName = TEXT ("moecmks0123456789") ;

  RegisterClass (& wndclass);
  
  wnd = CreateWindow ( TEXT ("moecmks0123456789"),
    TEXT ("nes device-context"), 
         WS_OVERLAPPEDWINDOW,
                 CW_USEDEFAULT,
          CW_USEDEFAULT, 
            CW_USEDEFAULT,
           CW_USEDEFAULT,
       NULL, NULL, mod, NULL);

  assert (wnd != NULL);
  resize_client (wnd, NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT, WS_OVERLAPPEDWINDOW);
  return wnd;
}

//<VIDEO>========================================================================================================
static 
int directx_video_init (HWND wnd) {

  HRESULT sig;
  D3DPRESENT_PARAMETERS d3dpp;

  int mmbs = NES_SCREEN_HEIGHT+NES_MERGING_EDGES*2;
  mmbs = mmbs * (NES_SCREEN_WIDTH+NES_MERGING_EDGES*2);
  mmbs *= sizeof (DWORD);

  ZeroMemory (& d3dpp, sizeof (D3DPRESENT_PARAMETERS));
  d3dpp.BackBufferWidth      = NES_SCREEN_WIDTH;
  d3dpp.BackBufferHeight     = NES_SCREEN_HEIGHT;
  d3dpp.BackBufferFormat     = D3DFMT_X8R8G8B8;
  d3dpp.SwapEffect           = D3DSWAPEFFECT_DISCARD;
  d3dpp.Flags                = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
  d3dpp.hDeviceWindow        = wnd;
  d3dpp.Windowed             = TRUE;
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; /* Vsync */

  sig = IDirect3D9_CreateDevice (globals.video_drv, 0, D3DDEVTYPE_HAL, 
       wnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, & d3dpp, & globals.video);
  assert (SUCCEEDED (sig));

  globals.vbbuf = _aligned_malloc ( mmbs, 32);
  assert (globals.vbbuf != NULL);

  memset (globals.vbbuf, 0, mmbs);
  return 0;
}

finline
void __inline_Vmemcpy256_224 (void *to, void *from, INT to_pitch, INT from_pitch) {

  INT off_to =  to_pitch - 1024;
  INT off_from =  from_pitch - 1024;
  INT Q, C;
  __m128i *to128p = to, *from128p = from;

  for (Q = 224; Q != 0; Q--) {
    for (C = 8; C != 0; C--) {
      _mm_store_si128 (& to128p[0], _mm_load_si128 (& from128p[0]));
      _mm_store_si128 (& to128p[1], _mm_load_si128 (& from128p[1]));
      _mm_store_si128 (& to128p[2], _mm_load_si128 (& from128p[2]));
      _mm_store_si128 (& to128p[3], _mm_load_si128 (& from128p[3]));
      _mm_store_si128 (& to128p[4], _mm_load_si128 (& from128p[4]));
      _mm_store_si128 (& to128p[5], _mm_load_si128 (& from128p[5]));
      _mm_store_si128 (& to128p[6], _mm_load_si128 (& from128p[6]));
      _mm_store_si128 (& to128p[7], _mm_load_si128 (& from128p[7]));

      to128p = & to128p[8];
      from128p = & from128p[8];
    }
    to128p = (  __m128i *) & ( ((BYTE *)to128p)[off_to]);
    from128p =(  __m128i *)  & ( ((BYTE *)from128p)[off_from]);
  }
}

static 
int screen_bitblt_256_224oop (void *source, int pitch) {

  IDirect3DSurface9 *surf = NULL;
  D3DLOCKED_RECT vdesc;

  /* flush proc:
   * 1: get/lock surface. Prevention of multi thread competition 
   * 2: write vram.
   * 3: unlock surface, present 
   * Unlock the lock can not work under Windows XP can also work. very strange !!!!
   */
  if (FAILED (IDirect3DDevice9_GetBackBuffer (globals.video, 0, 0, D3DBACKBUFFER_TYPE_MONO, & surf))) 
    return -1;
  else if (SUCCEEDED (IDirect3DSurface9_LockRect (surf, & vdesc, NULL, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE)))
    { 
       __inline_Vmemcpy256_224 (vdesc.pBits, source, vdesc.Pitch, pitch);
       IDirect3DSurface9_UnlockRect (surf);
       IDirect3DSurface9_Release (surf);
       IDirect3DDevice9_Present (globals.video, 0, 0, 0, 0); // FIXME:device lost 
       return 0;
    }

  IDirect3DSurface9_Release (surf);
  return -1;
}

static 
int directx_video_uninit (void) {

  HRESULT sig = IDirect3DDevice9_Release (globals.video);
  assert (SUCCEEDED (sig));
  _aligned_free (globals.vbbuf );
  return 0;
}

//<INPUT>========================================================================================================
static 
int directx_input_init (HWND wnd) {

  HRESULT sig = IDirectInput8_CreateDevice (globals.input_drv, (void *) & GUID_SysKeyboard, & globals.input, NULL);
  assert (SUCCEEDED (sig));

  /* only for IDirectInputDevice8_SetCooperativeLevel, so funny */
  ShowWindow (wnd, SW_SHOWNORMAL);

  sig = IDirectInputDevice8_SetCooperativeLevel (globals.input, wnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
  assert (SUCCEEDED (sig));

  ShowWindow (wnd, SW_HIDE);

  sig = IDirectInputDevice8_SetDataFormat (globals.input, & c_dfDIKeyboard);
  assert (SUCCEEDED (sig));
  return 0;
}

static 
int directx_input_uninit (void) {

  IDirectInputDevice8_Unacquire (globals.input);
  IDirectInputDevice8_Release (globals.input);
  return 0;
}

#define GUESS_MASK(keymapper, name)                                 \
  do {                                                             \
  /* 0:cur 1:old */                                                \
  old = ptbank[(s^256)+keymapper] & 0x80;            \
  cur = ptbank[s+keymapper] & 0x80;                  \
  if (old)                                                         \
    if (cur)                                                       \
      njoypad->name = JOYPAD_MASK_PRESS | JOYPAD_MASK_CONTINUOUSLY;\
    else                                                           \
      njoypad->name = 0;                                           \
  else \
    if (cur)\
      njoypad->name = JOYPAD_MASK_PRESS;\
    else \
      njoypad->name = JOYPAD_MASK_CONTINUOUSLY;\
  } while (0)  

static 
void clrkey (BYTE *dikeybuf) {
  dikeybuf[DIK_W] = 0;
  dikeybuf[DIK_S] = 0;
  dikeybuf[DIK_A] = 0;
  dikeybuf[DIK_D] = 0;
  dikeybuf[DIK_SPACE] = 0;
  dikeybuf[DIK_RETURN] = 0;
  dikeybuf[DIK_J] = 0;
  dikeybuf[DIK_K] = 0;
  dikeybuf[DIK_U] = 0;
  dikeybuf[DIK_I] = 0;
}

static 
void getkey (struct nes_joypad *njoypad) {

  static cvalign(32) 
  BYTE ptbank[512] = { 0} ;

  static 
  int s= 0;

  unsigned char cur;
  unsigned char old;

  if (FAILED(IDirectInputDevice8_GetDeviceState (globals.input, 256, & ptbank[s])))
    if (FAILED (IDirectInputDevice8_Acquire (globals.input))) 
      clrkey (& ptbank[s]);
    else 
      if (FAILED(IDirectInputDevice8_GetDeviceState (globals.input, 256, & ptbank[s])))
        clrkey (& ptbank[s]);
      else {}
  else {}

  GUESS_MASK (DIK_A, left);
  GUESS_MASK (DIK_D, right);
  GUESS_MASK (DIK_W, up);
  GUESS_MASK (DIK_S, down);  
  GUESS_MASK (DIK_J, b);
  GUESS_MASK (DIK_K, a);
  GUESS_MASK (DIK_U, x);
  GUESS_MASK (DIK_I, y); 
  GUESS_MASK (DIK_SPACE, select);
  GUESS_MASK (DIK_RETURN, start); 
  s ^= 256;
}

struct ns_sound {
  double aw_sample;
  IXAudio2SourceVoice *sound;
  union {
    struct pcmwaveformat_tag *ftw_pcm;
    struct tWAVEFORMATEX *ftw_wave_ex;
    struct adpcmwaveformat_tag *ftw_adpcm;
    struct WAVEFORMATEXTENSIBLE *ftw_wave_ext2;
    struct tWAVEFORMATEX *tools; // malloc 
  };
  int wave_headsize; // 
  void *pcm_buf;
  int pcm_size;
  int sample_blockPerSec;
  int tickscc;
};

//<SOUND>==============================================================================================
int ns_sound_open (char *filename, struct ns_sound **nsound, int *maxticks) {

  HMMIO mmioHandle;
  MMCKINFO mmckinfoParent;
  MMCKINFO mmckinfoSubChunk;
  MMRESULT sig;
  HRESULT sig7;
  DWORD nums;

  int sig2;
  CHAR szPath[MAX_PATH];

  struct ns_sound * ns = malloc (sizeof (struct ns_sound));

  GetModuleFileNameA (NULL, szPath, sizeof (szPath));
  PathRemoveFileSpecA (szPath);

  strcat (szPath, "\\");
  strcat (szPath, filename);
  
  mmioHandle = mmioOpenA ((LPSTR) szPath, NULL, MMIO_ALLOCBUF | MMIO_READ);
  assert (mmioHandle != NULL);

  ZeroMemory (& mmckinfoParent, sizeof (MMCKINFO));
  ZeroMemory (& mmckinfoSubChunk, sizeof (MMCKINFO));  

  /* get WAVE header */
  mmckinfoParent.fccType = mmioFOURCC ('W', 'A', 'V', 'E');
  sig = mmioDescend (mmioHandle, (LPMMCKINFO) & mmckinfoParent, NULL, MMIO_FINDRIFF);
  assert (sig == MMSYSERR_NOERROR);
  mmckinfoSubChunk.ckid = mmioFOURCC ('f', 'm', 't', ' ');
  sig =  mmioDescend (mmioHandle, &mmckinfoSubChunk, & mmckinfoParent, MMIO_FINDCHUNK);
  assert (sig == MMSYSERR_NOERROR);
  ns->wave_headsize = mmckinfoSubChunk.cksize;
  ns->tools = malloc (ns->wave_headsize);
  ns->tools->cbSize = 0;
  nums = mmioRead (mmioHandle, (HPSTR)ns->tools, mmckinfoSubChunk.cksize);
  assert (nums == ns->wave_headsize);
  sig = mmioAscend (mmioHandle, & mmckinfoSubChunk, 0);
  assert (sig == MMSYSERR_NOERROR);
  /* get data chunk data */
  mmckinfoSubChunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
  sig = mmioDescend (mmioHandle, & mmckinfoSubChunk, & mmckinfoParent, MMIO_FINDCHUNK);
  assert (sig == MMSYSERR_NOERROR);
  ns->pcm_size = mmckinfoSubChunk.cksize;
  ns->pcm_buf = malloc (mmckinfoSubChunk.cksize + 176400/60);
  memset ( & (( (char *)ns->pcm_buf)[ns->pcm_size]), 0, 176400/60);
  assert (ns->pcm_buf != NULL);
  sig2 = mmioSeek (mmioHandle, mmckinfoSubChunk.dwDataOffset, SEEK_SET);
  assert (sig2 != -1);
  sig2 = mmioRead (mmioHandle, ns->pcm_buf, mmckinfoSubChunk.cksize);
  assert (sig2 != -1);
  mmioClose (mmioHandle, 0);
  /* preiod second  */
  ns->sample_blockPerSec = ns->tools->nSamplesPerSec * ns->tools->nChannels * ns->tools->wBitsPerSample / 8;
  ns->tickscc = (int) (( ((double) ns->pcm_size)/ (double) ns->sample_blockPerSec) * (double) 60.00000000);
  /* create xaudio2 source voice objjcectg */
  sig7 = IXAudio2_CreateSourceVoice ( globals.sound_drv, 
            & ns->sound, (WAVEFORMATEX *) ns->tools, 
               0, 0, NULL, NULL, NULL);
  assert (SUCCEEDED (sig7));
  /* stop it at init */
  sig7 = IXAudio2SourceVoice_Stop (ns->sound, 0, XAUDIO2_COMMIT_NOW);
  assert (SUCCEEDED (sig7));
  * nsound = ns;
  if (maxticks != NULL)
    * maxticks = ns->tickscc; // ADPCMWAVEFORMAT
  return 0; 
}

int ns_sound_post2 (struct ns_sound *nsound, struct ns_sound_post *sm_post) {
  /* loopcount -1, loop forever 
     burning_ticks -1 , all ticks from start */
  struct XAUDIO2_BUFFER xbuf;
  int post_burning;

  xbuf.Flags = 0;
  xbuf.pContext = null;
  xbuf.PlayBegin = 0;
  xbuf.LoopBegin = 0;
  xbuf.LoopLength = 0;
  xbuf.PlayLength = 0;
  xbuf.AudioBytes = nsound->pcm_size;
  xbuf.pAudioData = nsound->pcm_buf;
  xbuf.LoopCount = (sm_post->loop_count == - 1) ? XAUDIO2_LOOP_INFINITE : sm_post->loop_count;
  xbuf.PlayBegin = sm_post->start_ticks*nsound->tools->nSamplesPerSec/60;
  xbuf.LoopBegin = sm_post->loop_start_ticks *nsound->tools->nSamplesPerSec/60;
  if (sm_post->burning_ticks == -1) 
    post_burning = nsound->tickscc - sm_post->start_ticks + 1;
  else post_burning = sm_post->burning_ticks;;
  xbuf.PlayLength = post_burning*nsound->tools->nSamplesPerSec/60;
  if (sm_post->loop_burning_ticks == -1) 
    post_burning = nsound->tickscc - sm_post->start_ticks + 1;
  else post_burning = sm_post->loop_burning_ticks;
  xbuf.LoopLength = post_burning*nsound->tools->nSamplesPerSec/60; //TODO:ADPCM stuff.
 
  if (IXAudio2SourceVoice_SubmitSourceBuffer (nsound->sound, & xbuf, null) == S_OK)
    return 0;
  else 
    return -1;
}

int ns_sound_post (struct ns_sound *nsound, int start_tikcs, int burning_ticks, int loop_count) {
  /* loopcount -1, loop forever 
     burning_ticks -1 , all ticks from start */
  struct XAUDIO2_BUFFER xbuf;

  xbuf.Flags = 0;
  xbuf.pContext = null;
  xbuf.PlayBegin = 0;
  xbuf.LoopBegin = 0;
  xbuf.LoopLength = 0;
  xbuf.PlayLength = 0;
  xbuf.LoopCount = (loop_count == - 1) ? XAUDIO2_LOOP_INFINITE : loop_count;
  if ( (burning_ticks == - 1)
        && (start_tikcs == 0)  ) {
    xbuf.AudioBytes = nsound->pcm_size;
    xbuf.pAudioData = nsound->pcm_buf;
  } else {
    char *p = nsound->pcm_buf;
    int nc = burning_ticks * nsound->sample_blockPerSec / 60;
    int nba = nsound->tools->nBlockAlign;
    xbuf.AudioBytes = (nc / nba) * nba;
    nc = start_tikcs * nsound->sample_blockPerSec / 60;
    nc = (nc / nba) * nba; 
    xbuf.pAudioData = & p[nc];
  }
  if (IXAudio2SourceVoice_SubmitSourceBuffer (nsound->sound, & xbuf, null) == S_OK)
    return 0;
  else 
    return -1;
}

int ns_sound_stop (struct ns_sound *nsound) {

  if (IXAudio2SourceVoice_Stop (nsound->sound, 0, XAUDIO2_COMMIT_NOW) == S_OK)
    return 0;
  else 
    return -1;
}

int ns_sound_start (struct ns_sound *nsound) {

  if (IXAudio2SourceVoice_Start (nsound->sound, 0, XAUDIO2_COMMIT_NOW) == S_OK)
    return 0;
  else 
    return -1;
}

int ns_sound_flushqueue (struct ns_sound *nsound) {

  if (IXAudio2SourceVoice_FlushSourceBuffers (nsound->sound) == S_OK)
    return 0;
  else 
    return -1;
}

void ns_sound_close (struct ns_sound **nsound) {
  if (nsound != NULL) {
    struct ns_sound *p = *nsound;
    *nsound = NULL;
    if (p != NULL) {
      HRESULT sig;
      sig = IXAudio2SourceVoice_Stop (p->sound, 0, XAUDIO2_COMMIT_NOW);
      assert (SUCCEEDED (sig));
      sig = IXAudio2SourceVoice_FlushSourceBuffers (p->sound);
      assert (SUCCEEDED (sig));
      /* sig = */IXAudio2SourceVoice_DestroyVoice (p->sound);
      // assert (SUCCEEDED (sig));
      free (p->tools);
      free (p->pcm_buf);
      free (p);
    }
  }
}
//<TIMING>================================================================================================
static cvalign (32) uint64_t ticks_overflow = 0;
static cvalign (32) uint64_t ticks_Freq = 0;
static cvalign (32) uint64_t ticks_Qf = 0;
static cvalign (32) uint64_t ticks_St = 0;

int timing_init60 (void) {

#if 0
  SetThreadAffinityMask (GetCurrentThread (), 1);
#endif 
  QueryPerformanceFrequency ((LARGE_INTEGER *) & ticks_Freq);
  ticks_St = ticks_Freq / 60;
  return 0;
}

int timing_start60 (void) {

  QueryPerformanceCounter ((LARGE_INTEGER *) & ticks_Qf);
  ticks_Qf -= ticks_overflow; /* XXX:maybe neg. **/
  return 0;
}

int timing_terming60 (void) {

  int64_t difference = 0;
  uint64_t time_start = ticks_Qf;
  uint64_t time_current = 0;
  uint64_t time_requir = ticks_St;
  uint64_t time_elapse = 0; /* XXX:win32 timing is LONGLONG not uint64_t */

  do 
  {
    QueryPerformanceCounter ((LARGE_INTEGER *) & time_current);
    time_elapse = time_current - time_start;
    difference = time_elapse -  time_requir;

    /* negative phase ? */
    if (difference >= 0)
    {
      /* burning out .**/
      // tim_v->ticks_Qf += time_elapse; // ÄÔÄÚ¹Û²â. 120-2 mid 80[60] diff = 40[60.20] . sec-
      ticks_overflow = difference;
      break;
    }
    else 
    {
      Sleep (1);
    }
  } while (1);

  return 0;
}

//<NES_DC>================================================================================================
finline 
uint32_t alphamix (const int32_t data, const uint8_t purity) {
  
  uint32_t x, y, z;
 
  x = data & 0x000000FF; //
  y = data & 0x0000FF00; //
  z = data & 0x00FF0000; // limit range 
    
  x = ((x * purity) >> 8) & 0x000000FF; //
  y = ((y * purity) >> 8) & 0x0000FF00; //
  z = ((z * purity) >> 8) & 0x00FF0000; // strong reduce | FIXME:div/shift
    
  return (x | y | z);  
}

finline  
uint32_t saddudt (const uint32_t d0, const uint32_t d2) {
  
  uint32_t x, y, z;
  
  x = (d0 & 0x000000FF) + (d2 & 0x000000FF); //
  y = (d0 & 0x0000FF00) + (d2 & 0x0000FF00); //
  z = (d0 & 0x00FF0000) + (d2 & 0x00FF0000); // cross add 
  
  if (x > 0x000000FF) x = 0x000000FF; //
  if (y > 0x0000FF00) y = 0x0000FF00; //
  if (z > 0x00FF0000) z = 0x00FF0000; // limit range 
    
  return (x | y | z);  
}

finline 
uint32_t alphamix2 (const int32_t data, const uint8_t purity, int32_t ppix) {
  
  uint32_t x, y, z;
 
  x = data & 0x000000FF; //
  y = data & 0x0000FF00; //
  z = data & 0x00FF0000; // limit range 
    
  x = ((x * purity) >> 8) & 0x000000FF; //
  y = ((y * purity) >> 8) & 0x0000FF00; //
  z = ((z * purity) >> 8) & 0x00FF0000; // strong reduce | FIXME:div/shift
    
  return saddudt ( (x | y | z), ppix);  
}

finline 
uint32_t alphamixd (uint32_t ag, uint32_t ag2, uint8_t ag_s, 
         uint8_t ag_s2) 
{
  return saddudt ( alphamix (ag, ag_s), 
                   alphamix (ag2, ag_s2));  
}
        
finline  void
alpha_u0 (void *to, int32_t shied, uint8_t count) {

  uint32_t ix, iy;
  uint32_t *to_cache = to;
  int32_t shied_mix = alphamix (shied, count ^ 0xFF);
  
  for (iy = 0; iy != 240; iy++) {
    for (ix = 0; ix != 256; ix += 4) 
    {
      // FIXME:SIMD stream.
      to_cache[ix+0] = saddudt ( alphamix (to_cache[ix+0], count), shied_mix);
      to_cache[ix+1] = saddudt ( alphamix (to_cache[ix+1], count), shied_mix);
      to_cache[ix+2] = saddudt ( alphamix (to_cache[ix+2], count), shied_mix);
      to_cache[ix+3] = saddudt ( alphamix (to_cache[ix+3], count), shied_mix);
    }
    // total calc-adjust in per line. 
    to_cache = & to_cache[272];
  }
}   

finline void
alpha_mset_v0 (void *to, int32_t shied) {
  /* use intrin.h? **/
  uint32_t ix, iy;
  uint32_t *to_cache = to;
  uint32_t shied0 = shied & 0x00FFFFFF;

  for (iy = 0; iy != 240; iy++) {
    for (ix = 0; ix != 256; ix += 4) 
    {
      // FIXME:SIMD stream.
      to_cache[ix+0] = shied0;
      to_cache[ix+1] = shied0;
      to_cache[ix+2] = shied0;
      to_cache[ix+3] = shied0;
    }
    // total calc-adjust in per line. 
    to_cache = & to_cache[272];
  }
} 



void ns_render_init (struct ns_render *nsr, struct ns_rendercc *nrcc) {

  nrcc->sn_page = nsr->sn_page & 3;
  nrcc->sn_x = nsr->ss_x & 7;
  nrcc->sn_y = nsr->ss_y & 7;
  nrcc->pstvbbuf = & nsr->vbbuf [8 - nrcc->sn_x];
  nrcc->pstvbbuf = & nrcc->pstvbbuf [ (8 - nrcc->sn_y) * 272];
  nrcc->pstvbatr = & nsr->vbatr [8 - nrcc->sn_x];
  nrcc->pstvbatr = & nrcc->pstvbatr [ (8 - nrcc->sn_y) * 272];
  nrcc->sn_x = nsr->ss_x >> 3;
  nrcc->sn_y = nsr->ss_y >> 3;
}

finline
void ns_bgticks_oneline (struct ns_render *nsr, struct ns_rendercc *nrcc, int y_scanline_nums)
{
  // check y scanline startpos 

  int sry = nrcc->sn_y + y_scanline_nums;
  int srx = nrcc->sn_x;
  int srpage = nsr->sn_page & 3;
  int id;
  struct ns_rendercc  rccs;
  struct ns_rendercc  aq;
  
  if (sry > 29) // switch vert pgae 
    { 
      srpage ^= 2;
      sry = sry - 30;
    }

  // repos vram ptr / hitlost .
  rccs.pstvbbuf = & nrcc->pstvbbuf[y_scanline_nums*272*8];
  rccs.pstvbatr = & nrcc->pstvbatr[y_scanline_nums*272*8];

  // scan 33 tiles .
  for (id = 33; id != 0; id--)
  {
    // repos tile's address 
    struct bg_chunk *bgc = & nsr->map[srx+(sry<<5)+srpage*1024];
    struct tile *te =& nsr->teb[bgc->page*256+bgc->land];
    // dostart ready.
    int x_start = 0;
    int x_goal = 8;
    int x_vector = 1;
    int x_start2;
    int y_start = 0;
    int y_goal = 8;
    int y_vector = 1;

    int pixel_buffer[64];
    int *ppixel_buffer = & te->dattable_org[0];
    char *pattr_buffer = & te->dattable_nat[0];

    if (bgc->mask & TEB_HFLIP)
      {
        x_start = 7;
        x_goal = -1;
        x_vector = -1;
      }

    if (bgc->mask & TEB_VFLIP)
      {
        y_start = 7;
        y_goal = -1;
        y_vector = -1;
      }

    if (bgc->mask & TEB_MAPPER)
      {
        int ids;
        for (ids = 0; ids != 64; ids++) {
          int q = te->maptable_sel[ids];
          if (q != -1)
            pixel_buffer[ids] = te->dattable_sel[q];
        }
        ppixel_buffer = & pixel_buffer[0];
      }

    // render tile (8*8)..
    x_start2 = x_start;
    aq.pstvbbuf = rccs.pstvbbuf;
    aq.pstvbatr = rccs.pstvbatr;

    if (! (bgc->mask & TEB_ALPHA)) 
    {
      for (y_start; y_start != y_goal; y_start += y_vector) {
        for (x_start = x_start2; x_start != x_goal; x_start += x_vector)
        {
          int b = (y_start<<3) + x_start;

          if (pattr_buffer [b])
          {
            // not trans 
            * aq.pstvbbuf = ppixel_buffer[b];
            * aq.pstvbatr = 1;
          }
          else 
          {
            * aq.pstvbbuf = nsr->ld_lut[bgc->ld_land];
            * aq.pstvbatr = 0;
          }
          aq.pstvbbuf = & aq.pstvbbuf[1];
          aq.pstvbatr = & aq.pstvbatr[1];
        }
        aq.pstvbbuf = & aq.pstvbbuf[272-8];
        aq.pstvbatr = & aq.pstvbatr[272-8];
      }
    }
    else 
    {
      for (y_start; y_start != y_goal; y_start += y_vector) {
        for (x_start = x_start2; x_start != x_goal; x_start += x_vector)
        {
          int b = (y_start<<3) + x_start;

          if (pattr_buffer [b])
          {
            // not trans 
            * aq.pstvbbuf = alphamixd ( nsr->ld_lut[bgc->ld_land], 
              ppixel_buffer[b], 
              0xFF ^ bgc->ld_percent, bgc->ld_percent);
            * aq.pstvbatr = 1;
          }
          else 
          {
            * aq.pstvbbuf = nsr->ld_lut[bgc->ld_land];
            * aq.pstvbatr = 0;
          }
          aq.pstvbbuf = & aq.pstvbbuf[1];
          aq.pstvbatr = & aq.pstvbatr[1];
        }
        aq.pstvbbuf = & aq.pstvbbuf[272-8];
        aq.pstvbatr = & aq.pstvbatr[272-8];
      }
    }
    rccs.pstvbbuf = & rccs.pstvbbuf [8];
    rccs.pstvbatr = & rccs.pstvbatr [8];
    
    if (++ srx == 32) 
    {
      srx = 0;
      srpage ^= 1;
    }
  }
}

void ns_bgticks (struct ns_render *nsr)
{
  struct ns_rendercc ed;
  int id;

  ns_render_init (nsr, & ed);

  for (id = 0; id != 30; id++)
    ns_bgticks_oneline (nsr, & ed, id);
}

finline
void ns_spticks_onesprite (struct ns_render *nsr, struct sp_chunk_item *sci)
{
  // check y scanline startpos 

  int id;
  struct ns_rendercc  rccs, aq;
  struct tile *te;

  int x_start ;
  int x_goal ;
  int x_vector ;
  int x_start2;
  int y_start ;
  int y_goal ;
  int y_vector ;
  int pixel_buffer[64];
  int *ppixel_buffer;
  char *pattr_buffer;

  // repos vram ptr / hitlost .
  id = sci->x_coord + sci->y_coord * 272;
  rccs.pstvbbuf = & nsr->vbbuf[8+272*8];
  rccs.pstvbatr = & nsr->vbatr[8+272*8];
  rccs.pstvbbuf = & rccs.pstvbbuf[id];
  rccs.pstvbatr = & rccs.pstvbatr[id];

  // repos tile's address 
  te = & sci->cache;

  // dostart ready.
  x_start = 0;
  x_goal = 8;
  x_vector = 1;
  x_start2;
  y_start = 0;
  y_goal = 8;
  y_vector = 1;
  pixel_buffer[64];
  ppixel_buffer = & te->dattable_org[0];
  pattr_buffer = (char *) & te->dattable_nat[0];

  // check range x..
  if (sci->x_coord > 247)
    x_goal = 8 - (sci->x_coord - 248);
  if (sci->y_coord > 231)
    y_goal = 8 - (sci->y_coord - 232);
  if (y_goal < 0 || (sci->mask & TES_CLOSE)) 
    return  ;

  if (sci->mask & TES_HFLIP)
    {
      x_start = 7;
      x_goal =  7 - x_goal; 
      x_vector = -1;
    }

  if (sci->mask & TES_VFLIP)
    {
      y_start = 7;
      y_goal =  7 - y_goal;
      y_vector = -1;
    }

  if (sci->mask & TES_MAPPER)
    {
      int ids;
      for (ids = 0; ids != 64; ids++) {
        int q = te->maptable_sel[ids];
        if (q != -1)
          pixel_buffer[ids] = te->dattable_sel[q];
      }
      ppixel_buffer = & pixel_buffer[0];
    }

  // render tile (8*8)..
  x_start2 = x_start;

  if (! (sci->mask & TES_ALPHA)) 
  {
    for (y_start; y_start != y_goal; y_start += y_vector) {
      aq.pstvbbuf = rccs.pstvbbuf;
      aq.pstvbatr = rccs.pstvbatr;

      for (x_start = x_start2; x_start != x_goal; x_start += x_vector)
      {
        int b = (y_start<<3) + x_start;

        if (pattr_buffer [b])
        {
          // not trans 
          if (sci->mask & TES_BACK) 
          {
            if (0 == * aq.pstvbatr) 
            {
              * aq.pstvbbuf = ppixel_buffer[b];
              * aq.pstvbatr = 1;
            }

          } // back sprite .
          else 
          {
            * aq.pstvbbuf = ppixel_buffer[b];
            * aq.pstvbatr = 1;
          }
        }
        aq.pstvbbuf = & aq.pstvbbuf[1];
        aq.pstvbatr = & aq.pstvbatr[1];
      }
      rccs.pstvbbuf = & rccs.pstvbbuf[272];
      rccs.pstvbatr = & rccs.pstvbatr[272];
    }
  }
  else 
  {
    for (y_start; y_start != y_goal; y_start += y_vector) {
      aq.pstvbbuf = rccs.pstvbbuf;
      aq.pstvbatr = rccs.pstvbatr;

      for (x_start = x_start2; x_start != x_goal; x_start += x_vector)
      {
        int b = (y_start<<3) + x_start;
        if (pattr_buffer [b])
        {
          // not trans 
          if (sci->mask & TES_BACK) 
          {
            if (0 == * aq.pstvbatr) 
            {
              * aq.pstvbbuf = alphamixd ( * aq.pstvbbuf, 
                ppixel_buffer[b], 
                0xFF ^ sci->ld_percent, sci->ld_percent);
              * aq.pstvbatr = 1;
            }

          } // back sprite .
          else 
          {
            * aq.pstvbbuf = alphamixd ( * aq.pstvbbuf, 
              ppixel_buffer[b], 
              0xFF ^ sci->ld_percent, sci->ld_percent);
            * aq.pstvbatr = 1;
          }
        }
        aq.pstvbbuf = & aq.pstvbbuf[1];
        aq.pstvbatr = & aq.pstvbatr[1];
      }
      rccs.pstvbbuf = & rccs.pstvbbuf[272];
      rccs.pstvbatr = & rccs.pstvbatr[272];
    }
  }
}

void ns_spticks (struct ns_render *nsr)
{
  struct ns_rendercc ed;
  int id;

  ns_render_init (nsr, & ed);

  for (id = 255; id != -1; id--) {
    struct sp_chunk_item *spi = & nsr->sprite[id].set[nsr->sprite->chan & 7];
    ns_spticks_onesprite (nsr, spi);
  }
}

void ns_setscrx (uint8_t x) { globals.nsr->ss_x = x ; }
void ns_setscrx2 (uint8_t x_round, uint8_t x_mini) { globals.nsr->ss_x = x_mini & 7 ; globals.nsr->ss_x |= ( (x_round & 31) << 3) ; }
void ns_setscry (uint8_t y) { globals.nsr->ss_y = y ; }
void ns_setscry2 (uint8_t y_round, uint8_t y_mini) { globals.nsr->ss_y = y_mini & 7 ; globals.nsr->ss_y |= ( (y_round & 31) << 3) ; }
void ns_setscrpage (int page) { globals.nsr->sn_page = page & 3 ; }
void ns_setctl_mask (int ctlmask) { globals.nsr->ctl_mask = ctlmask; }
void ns_setbg_ldpercent (uint8_t percent) { globals.nsr->lldp_bg = percent; }
void ns_setun_ldpercent (uint8_t percent) { globals.nsr->lldp_un = percent; }
void ns_setbg_ld (uint8_t idx) { globals.nsr->lland_bg = idx; }
void ns_setun_ld (uint8_t idx) { globals.nsr->lland_un = idx; }
void ns_setld_lut (uint8_t idx, uint32_t col) { globals.nsr->ld_lut[idx] = col & 0xFFFFFF; }

void ns_load_page (struct ns_texture *ntex,
           uint8_t page, 
           int xstart_nsr, 
           int ystart_nsr, 
           int xstart_ntex, 
           int ystart_ntex, 
           int xnums, int ynums,
           int32_t page_shield)
{
  int32_t shied = page_shield & 0x00FFFFFF;
  int32_t pitchMapperDword = ntex->pitch >> 2;

  struct tile *boff = & globals.nsr->teb[page*256];
  struct tile *poll;          
  
  int32_t x;
  int32_t y;
  int32_t x2;
  int32_t y2;
  int32_t x3;
  int32_t y3;
  int32_t z;
  int32_t g;              
  int32_t *pts;
  
  for (y = ystart_nsr, y3 = ystart_ntex, y2 = 0; y2 < ynums; y++, y3++, y2++) {
    for (x = xstart_nsr, x3 = xstart_ntex, x2 = 0; x2 < xnums; x++, x3++, x2++) { 
     
       /* Set init-coord buffer */
       pts = & ((int32_t *)ntex->pptr)[pitchMapperDword * y3 * 8 + x3 * 8];
       
       /* Set init-coord tile */
       poll = & boff[x + y * 16];
       
       /*
        * Clear tebl's infos, reset all status 
        */
       for (z = 0; z != 64; z++) {  
       
         poll->dattable_org[z] = pts[z / 8 * pitchMapperDword + z % 8] & 0x00FFFFFF;
         poll->dattable_sel[z] = shied;
         poll->dattable_count[z] = 0;
         poll->maptable_sel[z] = -1;
         poll->dattable_nat[z] = 0; /* default trans */
         poll->datcount_type = 0; /* Init Count-type, Compiler optimization, do not want to write a line*/
         poll->datcount_numb = 0; /* Init Count-numb, Compiler optimization, do not want to write a line */
       }       

       /* Collect infos */
       for (z = 0; z != 64; z++) {
           
         if ( (poll->dattable_org[z] & 0xFFFFFF) != shied) {
               
           /* Atom pixel */
           poll->datcount_numb++;      

           /* Set notrans */
           poll->dattable_nat[z] = 1;

           /* Test whether to repeat the color selection list */
           for (g = 0; poll->dattable_sel[g] != shied; g++) {

             if (poll->dattable_sel[g] == poll->dattable_org[z]) {
                 
               /* Atom cur pixel select table 's count */
               poll->dattable_count[g]++;
               
               /* Ignore pixel select table */
               goto repeat_;
             }
           }
               
           /* No repeat:add new pixel in pixel select table .
            * Set init pixel count, Atom type count, and mapper table
            */
           poll->dattable_sel[poll->datcount_type] = poll->dattable_org[z] & 0xFFFFFF;
           poll->dattable_count[poll->datcount_type++] = 1;
           
   repeat_:poll->maptable_sel[z] = g;
     
         }
       }  
    }
  }                  
}

void ppu_do (struct ns_render *nsr) 
{
  if (nsr->ctl_mask & PPU_BG_VISABLE) {     
    ns_bgticks (nsr);     
    if (nsr->ctl_mask & PPU_BG_ALPHA)
      alpha_u0 (& nsr->vbbuf[2184], nsr->ld_lut[nsr->lland_bg], nsr->lldp_bg);
  } else {
    memset  (& nsr->vbatr[0], 0x00, sizeof (nsr->vbatr));
    alpha_mset_v0 (& nsr->vbbuf[2184], nsr->ld_lut[nsr->lland_bg] & 0xFFFFFF);
  }
  if (nsr->ctl_mask & PPU_SP_VISABLE) {
    ns_spticks (nsr); 
  }  
  if (nsr->ctl_mask & PPU_UNION_ALPHA)
    alpha_u0 (& nsr->vbbuf[2184], nsr->ld_lut[nsr->lland_un], nsr->lldp_un);
}

void ns_init__ (struct ns_render **nsr) 
{
  int id;
  char *ns = calloc (sizeof (struct ns_render) + 64, 1);
  struct ns_render *nsp = (struct ns_render *)& ns[((uintptr_t)64) - ((uintptr_t)ns) & 63];
  nsp->memblock = ns;

  /* set OAM disable **/
  for (id = 0; id != 256; id++) {
    nsp->sprite[id].set[0].mask |= TES_CLOSE;
    nsp->sprite[id].set[1].mask |= TES_CLOSE;
    nsp->sprite[id].set[2].mask |= TES_CLOSE;
    nsp->sprite[id].set[3].mask |= TES_CLOSE;
    nsp->sprite[id].set[4].mask |= TES_CLOSE;
    nsp->sprite[id].set[5].mask |= TES_CLOSE;
    nsp->sprite[id].set[6].mask |= TES_CLOSE;
    nsp->sprite[id].set[7].mask |= TES_CLOSE;
  }
  *nsr = nsp;
}

void ns_uninit__ (struct ns_render **nsr) 
{
  if (nsr != NULL) {
    struct ns_render *nsp = *nsr;
    *nsr = NULL;
    if (nsp != NULL)
      free (nsp->memblock);
  }
}

int _POW2 (int n) {
  int S; for (S = 1; S < n; S *= 2) {} 
  return S; 
}

int ns_texture_load (char *filename, struct ns_texture **ntex) 
{
  {
    IDirect3DTexture9 *tex;
    D3DXIMAGE_INFO infos;
    CHAR szPath[MAX_PATH];

    GetModuleFileNameA (NULL, szPath, sizeof (szPath));
    PathRemoveFileSpecA (szPath);

    strcat (szPath, "\\");
    strcat (szPath, filename);

    if (FAILED ( D3DXCreateTextureFromFileExA (globals.video, szPath, 
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
      D3DLOCKED_RECT Vdesc;
      HRESULT sig;
      DWORD id2;
      struct ns_texture *nntb;

      // lock it 
      sig = IDirect3DTexture9_LockRect (tex, 0, & Vdesc, NULL, D3DLOCK_READONLY);
      assert (SUCCEEDED (sig));
      nntb = malloc (sizeof (struct ns_texture));
      assert (nntb != NULL);

      nntb->width = ((infos.Width+7) & 0xFFFFFFF8);
      nntb->height = ((infos.Height+7) & 0xFFFFFFF8);
      nntb->pitch = _POW2 (nntb->width *4); // step 4 * RGB-depth
      nntb->pptr = (void *) _aligned_malloc (nntb->pitch *nntb->height, 64);
      memset (nntb->pptr, 0, nntb->pitch *nntb->height);

      for (id2 = 0; id2 != infos.Height; id2++)
        memcpy ( & ( ((char *)nntb->pptr)[id2 * nntb->pitch]),
           & ( ((char *)Vdesc.pBits)[id2 * Vdesc.Pitch]) ,
           infos.Width *4);

      // unlock., release.
      IDirect3DTexture9_UnlockRect (tex, 0);
      IDirect3DTexture9_Release (tex);

      * ntex = nntb;
      return 0;
    }
    return -1;
  }
}

void ns_texture_unload (struct ns_texture **ntex) {
  if (ntex != null) {
    struct ns_texture *p = *ntex;
    *ntex = null;
    if (p != null) {
      if (p->pptr != null)
        _aligned_free (p->pptr);
      free (p);
    }
  }
}

//<SOUND=>==================================================================
int sys_media_load (char *filename, struct sys_media **snd) {

  CHAR szPath[MAX_PATH];
  WCHAR swzPath[MAX_PATH];
  struct sys_media *ns = malloc (sizeof (struct sys_media));
  HRESULT sig;

  GetModuleFileNameA (NULL, szPath, sizeof (szPath));
  PathRemoveFileSpecA (szPath);

  strcat (szPath, "\\");
  strcat (szPath, filename); 
  mbstowcs (swzPath, szPath, MAX_PATH);

  sig = CoCreateInstance ((void *) & CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,  (void *) &IID_IGraphBuilder, (void **)& ns->graphBuilder);
  assert (SUCCEEDED (sig));

  // create IMediaControl/IMediaPosition/IMediaEvent object
  sig = IGraphBuilder_QueryInterface (ns->graphBuilder, (void *) &IID_IMediaControl, (void**)& ns->mediaControl);
  assert (SUCCEEDED (sig));
  sig = IGraphBuilder_QueryInterface (ns->graphBuilder, (void *) &IID_IMediaPosition, (void**)& ns->mediaPosition);
  assert (SUCCEEDED (sig));
  sig = IGraphBuilder_QueryInterface (ns->graphBuilder, (void *) &IID_IMediaEvent, (void**)& ns->mediaEvent);
  assert (SUCCEEDED (sig));
  sig = IGraphBuilder_RenderFile (ns->graphBuilder, swzPath, NULL);
  assert (SUCCEEDED (sig));

  * snd = ns;
  return 0;
}

void sys_media_unload (struct sys_media **snd) {
  if (snd != null) {
    struct sys_media *p = *snd;
    *snd = null;
    if (p != null) {
      if (p->mediaControl) IMediaControl_Stop (p->mediaControl);
      if (p->mediaControl) IMediaControl_Release (p->mediaControl);
      if (p->mediaPosition) IMediaPosition_Release (p->mediaPosition);
      if (p->mediaEvent) IMediaEvent_Release (p->mediaEvent);
      if (p->graphBuilder) IGraphBuilder_Release (p->graphBuilder);
      free (p);
    }
  }
}

int sys_media_setpos (struct sys_media *snd, double pos) {
  return SUCCEEDED (IMediaPosition_put_CurrentPosition ( snd->mediaPosition, (REFTIME) pos)) ? 0 : -1;
}
int sys_media_startplay (struct sys_media *snd) {
  return SUCCEEDED (IMediaControl_Run ( snd->mediaControl)) ? 0 : -1;
}
int sys_media_stopplay (struct sys_media *snd) {
  return SUCCEEDED (IMediaControl_Stop ( snd->mediaControl)) ? 0 : -1;
}
int sys_media_curpos	(struct sys_media *snd, double *pos) {
  return SUCCEEDED (IMediaPosition_get_CurrentPosition ( snd->mediaPosition, (REFTIME *) pos)) ? 0 : -1;
}
int sys_media_getposmax (struct sys_media *snd, double *length) {
  return SUCCEEDED (IMediaPosition_get_Duration ( snd->mediaPosition, (REFTIME *) length)) ? 0 : -1;
}

int sys_media_curistail (struct sys_media *snd, int *ats) {
  long event_code, param1, param2;

  if (SUCCEEDED (IMediaEvent_GetEvent (snd->mediaEvent, & event_code, & param1, & param2, 0))) {

    if (event_code == EC_COMPLETE)
    {
      // frees resources associated with the parameters of an events.
      IMediaEvent_FreeEventParams (snd->mediaEvent, event_code, param1, param2);
      * ats = 0;
      return 0;
    }           
    else 
    {
      IMediaEvent_FreeEventParams (snd->mediaEvent, event_code, param1, param2);
      * ats = -1;
      return 0;
    }
  }
  * ats = -1;
  return 0;
}  // 0:tail 

int ns_entry (   void * (* cbinit) (void), 
  void (* cbloop) (struct nes_joypad *joypad, uint64_t ticks, void *params),
  void (* cbdtor) (void *params) )
{
  // init all... 
  void *param = NULL;
  struct ns_render *nsr;
  HWND widgets = create_nesSize_window ();
  directx_drv_init ();

  directx_input_init (widgets);
  directx_video_init (widgets);
  timing_init60 ();
  ns_init__ (& nsr);

  globals.nsr = nsr;

  if (cbinit != NULL)
    param = cbinit ();

  ShowWindow (widgets, SW_SHOWNORMAL);

  {
    MSG msg;

    while (TRUE) {
      if (PeekMessage (& msg, NULL, 0, 0, PM_REMOVE))
        if (msg.message == WM_QUIT)
          break;
        else 
          DispatchMessage (& msg);
      else
      {
        static cvalign(32) uint64_t ticks = 0;
        struct nes_joypad njoy;

        // Init-countstamp
        // timing_start60 ();
        // Poll keyboard
        getkey (& njoy);
        // Get keyboard mask 
        // callback times 
        if (cbloop != null) {
          cbloop (& njoy, ticks++, param);
        }
        // PPU times 
        ppu_do (nsr);
        // APU times 
        // apu_render (sysh->apu);  
        // PPU Out 
        screen_bitblt_256_224oop (& nsr->vbbuf[2184+8*272], 272 * 4);
        // APU Out 
        // sound_render 
        // Timing sync 
        // timing_terming60 ();  
      } 
    }
    ns_uninit__ (& nsr);

    directx_input_uninit ();
    directx_video_uninit ();
    directx_drv_uninit ();

    if (cbdtor != NULL)
      cbdtor (param);
    return 0;
  }
}

void ns_writebg ( int page, 
                  int width, 
                  int height, 
                  int write_page, 
                  int write_land, 
                  int mask, uint8_t ldland, uint8_t ldpercent)
{
  struct bg_chunk *bgc = & globals.nsr->map[(page &3)*1024+ ((height & 31) << 5)+(width&31)];
  bgc->page = write_page;
  bgc->land = write_land;
  bgc->ld_land = ldland;
  bgc->ld_percent = ldpercent;
  bgc->mask = mask;

}

struct tile *ns_gettile (uint8_t page, uint8_t lend) {
  return & globals.nsr->teb[page*256+lend];
}

void ns_writesprite (int idx, int channels, struct sp_chunk_item *sci) {
 memcpy ( & globals.nsr->sprite[idx&255].set[channels&7], sci, sizeof (struct sp_chunk_item));
}

void ns_readsprite (int idx, int channels, struct sp_chunk_item *sci) {
 memcpy ( sci, & globals.nsr->sprite[idx&255].set[channels&7], sizeof (struct sp_chunk_item));
}

void ns_setsprite_channels (int idx, int channels) {
  globals.nsr->sprite[idx&255].chan = channels;
}

void ns_copy_sprite_cache (struct sp_chunk_item *sci, struct tile *te) {
  memcpy (& sci->cache, te, sizeof (struct tile));
}

int ns_adjusttile_col (uint8_t page, uint8_t land, int32_t catch_col, int32_t adjust_col) {
  struct tile *ep = & globals.nsr->teb[page*256+land];
  int id;
  int col = catch_col & 0xFFFFFF;
  for (id = 0; id != 64; id++) {
    if ( (ep->dattable_sel[id] & 0xFFFFFF) == catch_col) {
      ep->dattable_sel[id] = adjust_col & 0xFFFFFF;
      return id;
    }
  }
  return -1;
}

int32_t *ns_adjusttile_colptr (uint8_t page, uint8_t land, int32_t catch_col) {
  struct tile *ep = & globals.nsr->teb[page*256+land];
  int id;
  int col = catch_col & 0xFFFFFF;
  for (id = 0; id != 64; id++) {
    if ( (ep->dattable_sel[id] & 0xFFFFFF) == catch_col) {
      return & ep->dattable_sel[id];
    }
  }
  return NULL;
}

int32_t *ns_adjustsprite_colptr (int idx, int channels, int32_t catch_col) {

  struct tile *ep = & globals.nsr->sprite[idx& 255].set[channels & 7].cache;
  int id;
  int col = catch_col & 0xFFFFFF;
  for (id = 0; id != 64; id++) {
    if ( (ep->dattable_sel[id] & 0xFFFFFF) == catch_col) {
      return & ep->dattable_sel[id];
    }
  }
  return NULL;
}

void ns_setsprite_mask (int idx, int channels, int32_t mask) {
  globals.nsr->sprite[idx& 255].set[channels & 7].mask = mask;
}
void ns_orsprite_mask (int idx, int channels, int32_t mask) {
  globals.nsr->sprite[idx& 255].set[channels & 7].mask |= mask;
}

int32_t ns_getsprite_mask (int idx, int channels) {
  return  globals.nsr->sprite[idx& 255].set[channels & 7].mask;
}

void ns_setsprite_ld_percent (int idx, int channels, int32_t ld_percent) {
  globals.nsr->sprite[idx& 255].set[channels & 7].ld_percent = ld_percent;
}
void ns_setsprite_x_coord (int idx, int channels, int32_t x) {
  globals.nsr->sprite[idx& 255].set[channels & 7].x_coord = x;
}
void ns_setsprite_y_coord (int idx, int channels, int32_t y) {
  globals.nsr->sprite[idx& 255].set[channels & 7].y_coord = y;
}
void ns_getmap ( int page,  int x_idx,  int y_idx, struct bg_chunk *bgc) {

  memcpy (bgc, & globals.nsr->map[(page &3)*1024+ ((y_idx & 31) << 5)+(x_idx&31)], sizeof (struct bg_chunk));
}
void ns_setmap ( int page,  int x_idx,  int y_idx, struct bg_chunk *bgc) {

  memcpy (& globals.nsr->map[(page &3)*1024+ ((y_idx & 31) << 5)+(x_idx&31)], bgc, sizeof (struct bg_chunk));
}
void *ns_ctor (void)

{
  struct ns_texture *nn;
  struct tile *e;
  struct sp_chunk_item sci;
  int ix;
  int iy;

  ns_setctl_mask (PPU_SP_VISABLE | PPU_BG_VISABLE);
  ns_texture_load ("u.png", & nn);
  ns_load_page(nn, 0, 0, 0, 0, 0, 2, 1, -1);
  ns_setld_lut (0, 0);

  ns_readsprite (0,0,&sci);
  for (iy = 0; iy != 32; iy++)
      for (ix = 0; ix != 32; ix++)
  ns_writebg (1, ix, iy, 0, 0, TEB_HFLIP | TEB_MAPPER, 0, 0);
  e = ns_gettile (0, 0);
  e->dattable_sel[0] = 0xFFFFFF & -5544;
  
  memcpy (& sci.cache, ns_gettile (0,0), sizeof (sci.cache));
  sci.y_coord = 50;
  ns_writesprite(0,0,& sci);
  ns_setsprite_mask (0, 0, 0);
  return NULL;

}


void ns_loop (struct nes_joypad *joypad, uint64_t ticks, void *param)
{
  static int x;
  if (joypad->right & JOYPAD_MASK_PRESS )
  {
    
    x += 3;
    if (x > 255) x = 255;
    ns_setscrx (x);
  }
  else if (joypad->left & JOYPAD_MASK_PRESS )
  {

    x -= 3;
    if (x < 0) x = 0;
    ns_setscrx (x);
  }
  if (joypad->b & JOYPAD_MASK_PRESS )
  {

    static int y;
    y++;
    ns_gettile (0, 0)->dattable_sel[1] = y;
  }
  if (joypad->a & JOYPAD_MASK_PRESS )
  {

    static int yq;
    yq++;
    
    ns_adjusttile_col(0,0,0xffa100, 5566);
  }
  globals.nsr->sprite[0].set[0].ld_percent = x;
  globals.nsr->sprite[0].set[0].mask |= TES_ALPHA;
}




#  if  1



int main (void)
{

  return ns_entry (ns_ctor, ns_loop, NULL);
}

#  endif 
















