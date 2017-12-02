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
IDirectInputDevice8 *joypad_device;
struct joypad joypad_mask;
/* SCREEN **/
IDirect3DDevice9 *screen_device; 
IDirect3DSurface9 *screen_surface;
/* DIRECTX DRV/MISC **/
struct directx_gen *directx_gen02;
struct emu8bit_ppu *ppu;
HINSTANCE instance;
HWND window_handle;
/* CALLBACK **/
void (callc *mr10_loop_cb)(struct joypad *pad, uint64_t ticks); 
void (callc *mr10_init_cb)(void);
void (callc *mr10_dtor_cb)(void);
