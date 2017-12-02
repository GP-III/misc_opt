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

#ifndef global0_H 
#define global0_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <xaudio2.h>
#include <dinput.h>
#include "../system.h"
#include "../input.h"
#include "../emu_NESppu.h"
#include "directx_gen.h"

/* JOYAD-DRV **/
extern IDirectInputDevice8 *joypad_device;
extern struct joypad joypad_mask;
/* SCREEN **/
extern IDirect3DDevice9 *screen_device; 
extern IDirect3DSurface9 *screen_surface;
/* DIRECTX DRV/MISC **/
extern struct directx_gen *directx_gen02;
extern struct emu8bit_ppu *ppu;
extern HINSTANCE instance;
extern HWND window_handle;
/* CALLBACK **/
extern void (callc *mr10_loop_cb)(struct emu8bit_ppu *ppu, struct joypad *pad, uint64_t ticks); 
extern void (callc *mr10_init_cb)(struct emu8bit_ppu *ppu);
extern void (callc *mr10_dtor_cb)(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif