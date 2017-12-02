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

#include "directx_gen.h"

int directx_init (struct directx_gen **gen) {
  
  HRESULT sult = CoInitializeEx (null, COINIT_MULTITHREADED);
  struct directx_gen _gen;
  
  if (gen == null)
    return -1;
  
  ZeroMemory (& _gen, sizeof (struct directx_gen));
  
  if (sult == S_OK)
    {
  if ( (_gen.video_drv = Direct3DCreate9 (D3D_SDK_VERSION)) != null)
    {
  if ( XAudio2Create (& _gen.sound_drv, 0, XAUDIO2_DEFAULT_PROCESSOR) == S_OK)
    {
      if ( DirectInput8Create (GetModuleHandle (null), DIRECTINPUT_VERSION, 
                & IID_IDirectInput8, (void **)& _gen.input_drv, null) == S_OK)
                {
                  if (IXAudio2_CreateMasteringVoice (_gen.sound_drv, & _gen.sound_drv_, 
                                 XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL) == S_OK)
                                 {
                                   struct directx_gen *p = malloc (sizeof (struct directx_gen));
                                   if (p != null)
                                      { 
                                    /* copy root.t */
                                        memcpy (p, & _gen, sizeof (struct directx_gen));
                                        *gen = p;
                                        
                                        return 0;
                                      }
                                      /* ! IXAudio2_CreateMasteringVoice */
                                      IXAudio2MasteringVoice_DestroyVoice (_gen.sound_drv_);
                                   _gen.sound_drv_ = null;
                                 }
                  /* ! DirectInput8Create */              
                         IDirectInput8_Release (_gen.input_drv);
                                   _gen.input_drv = null;             
                }   
                  /* ! XAudio2Create */              
                         IXAudio2_Release (_gen.sound_drv);
                                   _gen.sound_drv = null;           
    }
                  /* ! Direct3DCreate9 */              
                         IDirect3D9_Release (_gen.video_drv);
                                   _gen.video_drv = null; 
    }
                      /* ! CoInitializeEx */ 
    CoUninitialize();
    
    }

    *gen = null;
    return -1;
}

void directx_uninit (struct directx_gen **gen) {
  
  struct directx_gen *_gen ;
  if (gen == null)
    return ;
  
  _gen = *gen;
  *gen = null;
  
  if (_gen->video_drv) IDirect3D9_Release (_gen->video_drv);
  if (_gen->input_drv) IDirectInput8_Release (_gen->input_drv);
  if (_gen->sound_drv_)IXAudio2MasteringVoice_DestroyVoice (_gen->sound_drv_);
  if (_gen->sound_drv) IXAudio2_Release (_gen->sound_drv);
  /* assert ? */ CoUninitialize();
}