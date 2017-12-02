/*
 * Copyright (C) 2017 moecmks, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You should have received a copy of the GNU General Public
 * License along with the tile-mr10; if not, see:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "global.h"
#include <malloc.h>

// XXX:type
struct keyinfo_s {
  int8_t buf[2];
//int8_t key_mask;
};
static struct joypad_v {
  struct keyinfo_s left;
  struct keyinfo_s right;
  struct keyinfo_s up;
  struct keyinfo_s down;
  struct keyinfo_s b;
  struct keyinfo_s a;
  struct keyinfo_s select;
  struct keyinfo_s start;
} joypad_set;

static cvalign (32) int8_t dinput8key_buf[512];
static cvalign (32) int16_t swibank_mask = 0;

void joypad_uninit (void) {
  
  /* release inputdev com */     
  if (joypad_device != null) {
    IDirectInputDevice8_Unacquire (joypad_device);
    IDirectInputDevice8_Release (joypad_device);
    joypad_device = null;
  }
}

int joypad_init (void) {
  
  if (S_OK == IDirectInput8_CreateDevice (directx_gen02->input_drv, & GUID_SysKeyboard, & joypad_device, null)) // XXX:empty pointer
            if (S_OK == IDirectInputDevice8_SetCooperativeLevel (joypad_device, window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))
                if (S_OK == IDirectInputDevice8_SetDataFormat (joypad_device, & c_dfDIKeyboard))
        if (S_OK == IDirectInputDevice8_Acquire (joypad_device)) {    
    return 0;            
  } else {
    return -1;
  }
      
  /* release inputdev com */     
  joypad_uninit ();
  return -1;
}

#define GUESS_MASK(keymapper, name)                        \
do {                                                    \
  /* 0:cur 1:old */                                     \
  joypad_set.name.buf[1] = dinput8key_buf[(swibank_mask^256)+keymapper] & 0x80;  \
  joypad_set.name.buf[0] = dinput8key_buf[swibank_mask+keymapper] & 0x80;      \
  if (joypad_set.name.buf[0])              \
    if (joypad_set.name.buf[1])            \
        joypad_mask.name = JOYPAD_MASK_PRESS;  \
    else                            \
        joypad_mask.name = JOYPAD_MASK_PRESS | JOYPAD_MASK_FIRST;   \
  else                              \
    if (joypad_set.name.buf[1])            \
        joypad_mask.name = JOYPAD_MASK_LOOSE | JOYPAD_MASK_FIRST; \
    else                            \
        joypad_mask.name = JOYPAD_MASK_LOOSE;    \
} while (0)   

int joypad_poll (void) {
  
  if (S_OK != IDirectInputDevice8_GetDeviceState (joypad_device, 256, & dinput8key_buf[swibank_mask]))
    if (S_OK != IDirectInputDevice8_Acquire (joypad_device))      
	    return -1;
	  else if (S_OK != IDirectInputDevice8_GetDeviceState (joypad_device, 256, & dinput8key_buf[swibank_mask]))  
	    return -1;
    else {}
  else {}
  
  GUESS_MASK (DIK_A, left);
  GUESS_MASK (DIK_D, right);
  GUESS_MASK (DIK_W, up);
  GUESS_MASK (DIK_S, down);  
  GUESS_MASK (DIK_J, b);
  GUESS_MASK (DIK_K, a);
  GUESS_MASK (DIK_SPACE, select);
  GUESS_MASK (DIK_RETURN, start); 
  
  /* switch bank */
  swibank_mask ^= 256;
  return 0;
}