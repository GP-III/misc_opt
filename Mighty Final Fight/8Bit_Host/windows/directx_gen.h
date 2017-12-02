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

#ifndef directX_H 
#define directX_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <xaudio2.h>
#include <dinput.h>
#include "../system.h"

struct directx_gen {
  IXAudio2 *sound_drv;
  IXAudio2MasteringVoice *sound_drv_;
  IDirectInput8 *input_drv;
  IDirect3D9 *video_drv;    
};

int directx_init (struct directx_gen **gen);
void directx_uninit (struct directx_gen **gen);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif