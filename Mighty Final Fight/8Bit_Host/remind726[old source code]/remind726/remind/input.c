#include "system.h"
#include "input.h"
// XXX:type
struct keyinfo_s {
  char buf[2];
//char key_mask;
};
struct joypad_s {
  struct keyinfo_s left;
  struct keyinfo_s right;
  struct keyinfo_s up;
  struct keyinfo_s down;
  struct keyinfo_s b;
  struct keyinfo_s a;
  struct keyinfo_s select;
  struct keyinfo_s start; 
  struct system_s *sys;
  struct rtkey_s kmask;
#ifdef __WINDOWS_
  char dinput8key_buf[512];
  IDirectInputDevice8 *dev;
#else 
#endif
};
// init device
int init_input (struct system_s *sys) {
#ifdef __WINDOWS_ 
/* init inputdev */
  struct joypad_s *im = dlmalloc (sizeof(struct joypad_s));
  if (__null_ != im)  
      if (S_OK == IDirectInput8_CreateDevice (sys->input_drv, &GUID_SysKeyboard, &im->dev, __null_)) // XXX:empty pointer
                    if (S_OK == IDirectInputDevice8_SetCooperativeLevel (im->dev, sys->window, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))
                       if (S_OK == IDirectInputDevice8_SetDataFormat (im->dev, &c_dfDIKeyboard))
               if (S_OK == IDirectInputDevice8_Acquire (im->dev)) {                           
      im->sys = sys; /* Set parent */
      sys->input_handle = im; /* Set child */
      return 0;            
  } 
      
  /* release inputdev com */     
  __RELEASE_COM(IDirect3DDevice9,im->dev);
  /* release inputdev mem */
  dlfree(im);

  return -1;
#else 
#endif 
}
#define GUESS_MASK(keymapper, name)                        \
do {                                                    \
  /* 0:cur 1:old */                                     \
  im->name.buf[1] = im->dinput8key_buf[256+keymapper];  \
  im->name.buf[0] = im->dinput8key_buf[keymapper];      \
  if (im->name.buf[0])              \
    if (im->name.buf[1])            \
        im->kmask.name = JPMASK_DOWN | JPMASK_DOWN_SERIES;  \
    else                            \
        im->kmask.name = JPMASK_DOWN;   \
  else                              \
    if (im->name.buf[1])            \
        im->kmask.name = JPMASK_UP; \
    else                            \
        im->kmask.name = JPMASK_UP | JPMASK_UP_SERIES;      \
} while (0)   
// get key infos, push buffer    
int poll_key (struct joypad_s *im) {
#ifdef __WINDOWS_  
  /* update old infos */
  memcpy (&im->dinput8key_buf[256], &im->dinput8key_buf[0], 256);
    
  if (S_OK != IDirectInputDevice8_GetDeviceState (im->dev, 256, &im->dinput8key_buf[0]))
    if (S_OK != IDirectInputDevice8_Acquire (im->dev))      
	    return -1;
	else if (S_OK != IDirectInputDevice8_GetDeviceState (im->dev, 256, &im->dinput8key_buf[0]))  
	    return -1;
  
  GUESS_MASK(DIK_A, left);
  GUESS_MASK(DIK_D, right);
  GUESS_MASK(DIK_W, up);
  GUESS_MASK(DIK_S, down);  
  GUESS_MASK(DIK_J, b);
  GUESS_MASK(DIK_K, a);
  GUESS_MASK(DIK_I, select);
  GUESS_MASK(DIK_L, start); 
  
  return 0;
#else 
#endif  
}
// XXX:low..
int uninit_input (struct joypad_s *im) {
#ifdef __WINDOWS_  
  if (im == __null_) 
      return 0;
  if (im->dev) // XXX:n/a... 
      if (S_OK != IDirectInputDevice8_Unacquire(im->dev))
          return -1;
  if (im->dev) 
      if (S_OK != IDirectInputDevice8_Release(im->dev))
          return -1;
      
  dlfree(im);
  return 0;
#else 
#endif  
}
// get key infos, pull buffer
void keymask_ret (struct joypad_s *im, struct rtkey_s *R) {
  (void)memcpy(R, &im->kmask, sizeof(struct rtkey_s));
}