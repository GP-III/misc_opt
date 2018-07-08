//  CDirectInput.hxx 
//  Get the state of the keyboard 
# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CDIRECTINPUT_INCLUED_HEADER_CXX_)
#  define _CDIRECTINPUT_INCLUED_HEADER_CXX_

# include <atldef.h>
# include <directx/include/dinput.h>
  
class CDirectInput {
public:
  // Global DirectInput8 COM root.
  static IDirectInput8 *sm_DirectInput8_root;
  static const int sm_buffer_nums = 12;
public:
  typedef int JP_MASK;
  static const int JP_MASK_PRESS = 1;
  static const int JP_MASK_LAST = 2; 
  
  // play 
  enum PLAY_TYPE {
    FOREGROUND = 0,
    BACKGROUND
  };

  struct DiKeyBufferChunk {
    BYTE DIK_map;
    BYTE key_sig;
    BYTE align[2];
  };
  struct DiKeyBuffer {
    struct DiKeyBufferChunk queue[sm_buffer_nums];
    int vnums;
  };
  
  // Global IDirectInput8 Root Startup 
  static void InitDInput8 (void) {
    if (sm_DirectInput8_root == NULL) {
      HRESULT  sig = DirectInput8Create (GetModuleHandle (NULL), 
                             DIRECTINPUT_VERSION, 
                           IID_IDirectInput8, 
                              (void **)& sm_DirectInput8_root, NULL);
      ATLASSERT (SUCCEEDED (sig));
    }
  }
  // Global IDirectInput8 Root close 
  static void UninitDInput8 (void) {
    release_com<IDirectInput8>(sm_DirectInput8_root);
  }
  
  void resetPlayType (enum PLAY_TYPE t) {
    ATLASSERT (t == FOREGROUND || (t = BACKGROUND));
    
    destroy_base ();
    
    InitDeviceKeyboard (m_AttachWindow, t);
    // reset key status .
    ZeroMemory (m_keyBoardBuffer, sizeof (m_keyBoardBuffer));
    m_keyBoardBank = 0;
  }
  
  void initSettings (void) {
    m_input = NULL;
    // clear  buf 
    ZeroMemory (m_keyBoardBuffer, sizeof (m_keyBoardBuffer));
    m_keyBoardBank = 0;
  }
  
  void InitDeviceKeyboard (HWND hWindow, enum PLAY_TYPE t = FOREGROUND, BOOL buffed_mode = TRUE, BOOL excelus = FALSE)  {
    ATLASSERT (t == FOREGROUND || (t = BACKGROUND));
    
    HRESULT sig = sm_DirectInput8_root->CreateDevice ( GUID_SysKeyboard, & m_input, NULL);   
    ATLASSERT (SUCCEEDED (sig));
    
    BOOL bOldisVisual  = !! IsWindowVisible (hWindow);
    if  (bOldisVisual == FALSE)  
      ShowWindow (hWindow, SW_SHOWNORMAL);  
    m_AttachWindow = hWindow;
    
    // Set   SetCooperativeLevel 
    sig = m_input->SetCooperativeLevel (hWindow, (excelus ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE) | ((t == FOREGROUND )?  DISCL_FOREGROUND : DISCL_BACKGROUND));  
    ATLASSERT (SUCCEEDED (sig));
    
    // Set SetDataFormat KeyBoard.
    sig = m_input->SetDataFormat ( (LPDIDATAFORMAT) & c_dfDIKeyboard);
    ATLASSERT (SUCCEEDED (sig));
    
    // Set BufferMode Buffer Size 
    if (buffed_mode != FALSE)  {
      DIPROPDWORD dipdw = {
        { sizeof(DIPROPDWORD), sizeof(DIPROPHEADER), 0, DIPH_DEVICE },
        sm_buffer_nums
      };
      sig = m_input->SetProperty (DIPROP_BUFFERSIZE, & dipdw.diph);
      ATLASSERT (SUCCEEDED (sig));
    }
    if  (bOldisVisual == FALSE)  // reset old
      ShowWindow (hWindow, SW_HIDE);
  }

  void PollBuffer (void)  {
    int s = m_keyBoardBank;
    m_keyBoardBank ^= 256;
    
    if ( FAILED( m_input->GetDeviceState (256, & m_keyBoardBuffer[s])) ) 
      if (FAILED (m_input->Acquire ()))                   //  first   try   Acquire device.
        ZeroMemory (& m_keyBoardBuffer[s], 256);
      else 
        if ( FAILED( m_input->GetDeviceState (256, & m_keyBoardBuffer[s])) ) 
          ZeroMemory (& m_keyBoardBuffer[s], 256);        //  nodone, clear buffer.
        else  ;
    else ;
  }
  
  int PollBufferFor_BufferMode (DiKeyBuffer *buf)  { 
    DIDEVICEOBJECTDATA didod[sm_buffer_nums];  // Receives buffered data 
    DWORD              dwElements;
    HRESULT            sig;

    buf->vnums = 0;

L0: dwElements = sm_buffer_nums;
    sig = m_input->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     didod, & dwElements, 0 );
    if (FAILED(sig))  {
        //ATLTRACE ( "CDirectInput GetDeviceData Failed!\n");
        sig = m_input->Acquire();
        while (sig == DIERR_INPUTLOST) {
            //ATLTRACE ( "Retry Acquire Device\n");
            sig = m_input->Acquire();
        }
        if (FAILED(sig)) {
          //ATLTRACE ( "CDirectInput Unknown Errors\n");
          return -1;
        }
        goto L0;
    }
    buf->vnums = dwElements;
    // Fill buffer. 
    //if (dwElements != 0)
    //ATLTRACE ( "CDirectInput Buffer Queue Trace Buffer Nums:%d Current Use:%d----------------------------------------\n", sm_buffer_nums, dwElements);
    for (int id = 0; id != dwElements; id++)  {
      DiKeyBufferChunk &t = buf->queue[id];
      DIDEVICEOBJECTDATA &c = didod[id];

      t.DIK_map = (BYTE) c.dwOfs;
      t.key_sig = (BYTE) (c.dwData & 0x80);
      //ATLTRACE ( "CDirectInput Buffer Queue: KEY:%02X Status: %02X\n", t.DIK_map, t.key_sig);
    }
    return 0;
  }

  JP_MASK  getMask (BYTE DIK_idx)  {
    JP_MASK t = 0;
    int c = DIK_idx + m_keyBoardBank;
    int s = c ^ 256;
    
    if ( ( m_keyBoardBuffer[s] & 0x80) != 0) {           //   cur press.
      t |= JP_MASK_PRESS;
      if ( ( m_keyBoardBuffer[c] & 0x80) != 0)           //   old press.
        t |= JP_MASK_LAST;                             
      else  ;
    } else 
      if ( ( m_keyBoardBuffer[c] & 0x80) == 0)           //    cur not press 
        t |= JP_MASK_LAST;                               
      else  ;
    return t;
  }
  
  BOOL  isPressedFirst (BYTE DIK_idx)  {
    int c = DIK_idx + m_keyBoardBank;
    int s = c ^ 256;
    
    if ( ( m_keyBoardBuffer[s] & 0x80) != 0              //  cur  press 
      &&  (( m_keyBoardBuffer[c] & 0x80) == 0) )         
      return TRUE;                                       //   first press
    else ;
    
    return FALSE;
  }
  
public:
  // construct  
  CDirectInput (void)  {
    initSettings ();
  }
  
  void destroy_base (void) {
    if ( m_input != NULL) {
      HRESULT sig= m_input->Unacquire ();
      ATLASSERT (SUCCEEDED (sig));
      release_com<IDirectInputDevice8>(m_input);
    }
  }
  
  // deconstruct
  ~CDirectInput (void) {
    destroy_base ();
  }; 
private:
  BYTE m_keyBoardBuffer[512]; 
  WORD m_keyBoardBank; 
  IDirectInputDevice8 *m_input;
  HWND m_AttachWindow;
};
IDirectInput8 *CDirectInput::sm_DirectInput8_root = nullptr;

# endif 