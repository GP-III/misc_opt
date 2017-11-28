/*
  Win32 linker
*/

module ezui.win32.linker;

import win32.directx.dinput8;
import win32.directx.d3d9;
import win32.directx.d3dx9;

import win32.uuid;

import dinput_lack;

import core.runtime;

import std.stdio;
import std.string;
import std.c.wcharh;
import std.conv;
import std.exception;

import diob.linker;

__gshared IDirect3D9 _gs_d3droot; 
__gshared IDirect3DDevice9 _gs_d3ddevice; 
__gshared IDirect3DSurface9 _gs_d3dsurface_main;
__gshared IDirect3DSurface9 _gs_d3dsurface_offscreen;
__gshared IDirectInput8 _gs_dideivce_root;
__gshared IDirectInputDevice8 _gs_dikeyboard_device;
__gshared IDirectInputDevice8 _gs_dimouse_device;

__gshared char[2][256] _gs_dikeyboard_buffer;
__gshared char[2][4] _gs_dimouse_button;
__gshared _Point[2] _gs_dimouse_point;

__gshared D3DPRESENT_PARAMETERS _gs_d3dpp;

__gshared HWND _gs_window;

__gshared linker.keyboard_buffer *_gs_keybuffer;

__gshared linker.mouse_infos *_gs_mouse;


// guid dump from dinput8.lib 
// dinput8.lib\dilib5.obj &&  dinput8.lib\dilib2.obj
// not mine, from http://www.asmcommunity.net/forums/topic/?id=11310

__gshared DIOBJECTDATAFORMAT[11] c_rgodfDIMouse2 = [ 
{ &GUID_XAxis, 0, 0x00FFFF03, 0 }, 
{ &GUID_YAxis, 4, 0x00FFFF03, 0 }, 
{ &GUID_ZAxis, 8, 0x80FFFF03, 0 }, 
{ null, 12, 0x00FFFF0C, 0 }, 
{ null, 13, 0x00FFFF0C, 0 }, 
{ null, 14, 0x80FFFF0C, 0 }, 
{ null, 15, 0x80FFFF0C, 0 }, 
{ null, 16, 0x80FFFF0C, 0 }, 
{ null, 17, 0x80FFFF0C, 0 }, 
{ null, 18, 0x80FFFF0C, 0 }, 
{ null, 19, 0x80FFFF0C, 0 } 
];  

__gshared const DIDATAFORMAT c_dfDIMouse2_ = { 24, 16, 0x2, 20, 11, & c_rgodfDIMouse2[0] };


__gshared DIOBJECTDATAFORMAT[256] c_rgodfDIKeyboard = [
{ &GUID_Key, 0, 0x8000000C, 0 }, 
{ &GUID_Key, 1, 0x8000010C, 0 }, 
{ &GUID_Key, 2, 0x8000020C, 0 }, 
{ &GUID_Key, 3, 0x8000030C, 0 }, 
{ &GUID_Key, 4, 0x8000040C, 0 }, 
{ &GUID_Key, 5, 0x8000050C, 0 }, 
{ &GUID_Key, 6, 0x8000060C, 0 }, 
{ &GUID_Key, 7, 0x8000070C, 0 }, 
{ &GUID_Key, 8, 0x8000080C, 0 }, 
{ &GUID_Key, 9, 0x8000090C, 0 }, 
{ &GUID_Key, 10, 0x80000A0C, 0 }, 
{ &GUID_Key, 11, 0x80000B0C, 0 }, 
{ &GUID_Key, 12, 0x80000C0C, 0 }, 
{ &GUID_Key, 13, 0x80000D0C, 0 }, 
{ &GUID_Key, 14, 0x80000E0C, 0 }, 
{ &GUID_Key, 15, 0x80000F0C, 0 }, 
{ &GUID_Key, 16, 0x8000100C, 0 }, 
{ &GUID_Key, 17, 0x8000110C, 0 }, 
{ &GUID_Key, 18, 0x8000120C, 0 }, 
{ &GUID_Key, 19, 0x8000130C, 0 }, 
{ &GUID_Key, 20, 0x8000140C, 0 }, 
{ &GUID_Key, 21, 0x8000150C, 0 }, 
{ &GUID_Key, 22, 0x8000160C, 0 }, 
{ &GUID_Key, 23, 0x8000170C, 0 }, 
{ &GUID_Key, 24, 0x8000180C, 0 }, 
{ &GUID_Key, 25, 0x8000190C, 0 }, 
{ &GUID_Key, 26, 0x80001A0C, 0 }, 
{ &GUID_Key, 27, 0x80001B0C, 0 }, 
{ &GUID_Key, 28, 0x80001C0C, 0 }, 
{ &GUID_Key, 29, 0x80001D0C, 0 }, 
{ &GUID_Key, 30, 0x80001E0C, 0 }, 
{ &GUID_Key, 31, 0x80001F0C, 0 }, 
{ &GUID_Key, 32, 0x8000200C, 0 }, 
{ &GUID_Key, 33, 0x8000210C, 0 }, 
{ &GUID_Key, 34, 0x8000220C, 0 }, 
{ &GUID_Key, 35, 0x8000230C, 0 }, 
{ &GUID_Key, 36, 0x8000240C, 0 }, 
{ &GUID_Key, 37, 0x8000250C, 0 }, 
{ &GUID_Key, 38, 0x8000260C, 0 }, 
{ &GUID_Key, 39, 0x8000270C, 0 }, 
{ &GUID_Key, 40, 0x8000280C, 0 }, 
{ &GUID_Key, 41, 0x8000290C, 0 }, 
{ &GUID_Key, 42, 0x80002A0C, 0 }, 
{ &GUID_Key, 43, 0x80002B0C, 0 }, 
{ &GUID_Key, 44, 0x80002C0C, 0 }, 
{ &GUID_Key, 45, 0x80002D0C, 0 }, 
{ &GUID_Key, 46, 0x80002E0C, 0 }, 
{ &GUID_Key, 47, 0x80002F0C, 0 }, 
{ &GUID_Key, 48, 0x8000300C, 0 }, 
{ &GUID_Key, 49, 0x8000310C, 0 }, 
{ &GUID_Key, 50, 0x8000320C, 0 }, 
{ &GUID_Key, 51, 0x8000330C, 0 }, 
{ &GUID_Key, 52, 0x8000340C, 0 }, 
{ &GUID_Key, 53, 0x8000350C, 0 }, 
{ &GUID_Key, 54, 0x8000360C, 0 }, 
{ &GUID_Key, 55, 0x8000370C, 0 }, 
{ &GUID_Key, 56, 0x8000380C, 0 }, 
{ &GUID_Key, 57, 0x8000390C, 0 }, 
{ &GUID_Key, 58, 0x80003A0C, 0 }, 
{ &GUID_Key, 59, 0x80003B0C, 0 }, 
{ &GUID_Key, 60, 0x80003C0C, 0 }, 
{ &GUID_Key, 61, 0x80003D0C, 0 }, 
{ &GUID_Key, 62, 0x80003E0C, 0 }, 
{ &GUID_Key, 63, 0x80003F0C, 0 }, 
{ &GUID_Key, 64, 0x8000400C, 0 }, 
{ &GUID_Key, 65, 0x8000410C, 0 }, 
{ &GUID_Key, 66, 0x8000420C, 0 }, 
{ &GUID_Key, 67, 0x8000430C, 0 }, 
{ &GUID_Key, 68, 0x8000440C, 0 }, 
{ &GUID_Key, 69, 0x8000450C, 0 }, 
{ &GUID_Key, 70, 0x8000460C, 0 }, 
{ &GUID_Key, 71, 0x8000470C, 0 }, 
{ &GUID_Key, 72, 0x8000480C, 0 }, 
{ &GUID_Key, 73, 0x8000490C, 0 }, 
{ &GUID_Key, 74, 0x80004A0C, 0 }, 
{ &GUID_Key, 75, 0x80004B0C, 0 }, 
{ &GUID_Key, 76, 0x80004C0C, 0 }, 
{ &GUID_Key, 77, 0x80004D0C, 0 }, 
{ &GUID_Key, 78, 0x80004E0C, 0 }, 
{ &GUID_Key, 79, 0x80004F0C, 0 }, 
{ &GUID_Key, 80, 0x8000500C, 0 }, 
{ &GUID_Key, 81, 0x8000510C, 0 }, 
{ &GUID_Key, 82, 0x8000520C, 0 }, 
{ &GUID_Key, 83, 0x8000530C, 0 }, 
{ &GUID_Key, 84, 0x8000540C, 0 }, 
{ &GUID_Key, 85, 0x8000550C, 0 }, 
{ &GUID_Key, 86, 0x8000560C, 0 }, 
{ &GUID_Key, 87, 0x8000570C, 0 }, 
{ &GUID_Key, 88, 0x8000580C, 0 }, 
{ &GUID_Key, 89, 0x8000590C, 0 }, 
{ &GUID_Key, 90, 0x80005A0C, 0 }, 
{ &GUID_Key, 91, 0x80005B0C, 0 }, 
{ &GUID_Key, 92, 0x80005C0C, 0 }, 
{ &GUID_Key, 93, 0x80005D0C, 0 }, 
{ &GUID_Key, 94, 0x80005E0C, 0 }, 
{ &GUID_Key, 95, 0x80005F0C, 0 }, 
{ &GUID_Key, 96, 0x8000600C, 0 }, 
{ &GUID_Key, 97, 0x8000610C, 0 }, 
{ &GUID_Key, 98, 0x8000620C, 0 }, 
{ &GUID_Key, 99, 0x8000630C, 0 }, 
{ &GUID_Key, 100, 0x8000640C, 0 }, 
{ &GUID_Key, 101, 0x8000650C, 0 }, 
{ &GUID_Key, 102, 0x8000660C, 0 }, 
{ &GUID_Key, 103, 0x8000670C, 0 }, 
{ &GUID_Key, 104, 0x8000680C, 0 }, 
{ &GUID_Key, 105, 0x8000690C, 0 }, 
{ &GUID_Key, 106, 0x80006A0C, 0 }, 
{ &GUID_Key, 107, 0x80006B0C, 0 }, 
{ &GUID_Key, 108, 0x80006C0C, 0 }, 
{ &GUID_Key, 109, 0x80006D0C, 0 }, 
{ &GUID_Key, 110, 0x80006E0C, 0 }, 
{ &GUID_Key, 111, 0x80006F0C, 0 }, 
{ &GUID_Key, 112, 0x8000700C, 0 }, 
{ &GUID_Key, 113, 0x8000710C, 0 }, 
{ &GUID_Key, 114, 0x8000720C, 0 }, 
{ &GUID_Key, 115, 0x8000730C, 0 }, 
{ &GUID_Key, 116, 0x8000740C, 0 }, 
{ &GUID_Key, 117, 0x8000750C, 0 }, 
{ &GUID_Key, 118, 0x8000760C, 0 }, 
{ &GUID_Key, 119, 0x8000770C, 0 }, 
{ &GUID_Key, 120, 0x8000780C, 0 }, 
{ &GUID_Key, 121, 0x8000790C, 0 }, 
{ &GUID_Key, 122, 0x80007A0C, 0 }, 
{ &GUID_Key, 123, 0x80007B0C, 0 }, 
{ &GUID_Key, 124, 0x80007C0C, 0 }, 
{ &GUID_Key, 125, 0x80007D0C, 0 }, 
{ &GUID_Key, 126, 0x80007E0C, 0 }, 
{ &GUID_Key, 127, 0x80007F0C, 0 }, 
{ &GUID_Key, 128, 0x8000800C, 0 }, 
{ &GUID_Key, 129, 0x8000810C, 0 }, 
{ &GUID_Key, 130, 0x8000820C, 0 }, 
{ &GUID_Key, 131, 0x8000830C, 0 }, 
{ &GUID_Key, 132, 0x8000840C, 0 }, 
{ &GUID_Key, 133, 0x8000850C, 0 }, 
{ &GUID_Key, 134, 0x8000860C, 0 }, 
{ &GUID_Key, 135, 0x8000870C, 0 }, 
{ &GUID_Key, 136, 0x8000880C, 0 }, 
{ &GUID_Key, 137, 0x8000890C, 0 }, 
{ &GUID_Key, 138, 0x80008A0C, 0 }, 
{ &GUID_Key, 139, 0x80008B0C, 0 }, 
{ &GUID_Key, 140, 0x80008C0C, 0 }, 
{ &GUID_Key, 141, 0x80008D0C, 0 }, 
{ &GUID_Key, 142, 0x80008E0C, 0 }, 
{ &GUID_Key, 143, 0x80008F0C, 0 }, 
{ &GUID_Key, 144, 0x8000900C, 0 }, 
{ &GUID_Key, 145, 0x8000910C, 0 }, 
{ &GUID_Key, 146, 0x8000920C, 0 }, 
{ &GUID_Key, 147, 0x8000930C, 0 }, 
{ &GUID_Key, 148, 0x8000940C, 0 }, 
{ &GUID_Key, 149, 0x8000950C, 0 }, 
{ &GUID_Key, 150, 0x8000960C, 0 }, 
{ &GUID_Key, 151, 0x8000970C, 0 }, 
{ &GUID_Key, 152, 0x8000980C, 0 }, 
{ &GUID_Key, 153, 0x8000990C, 0 }, 
{ &GUID_Key, 154, 0x80009A0C, 0 }, 
{ &GUID_Key, 155, 0x80009B0C, 0 }, 
{ &GUID_Key, 156, 0x80009C0C, 0 }, 
{ &GUID_Key, 157, 0x80009D0C, 0 }, 
{ &GUID_Key, 158, 0x80009E0C, 0 }, 
{ &GUID_Key, 159, 0x80009F0C, 0 }, 
{ &GUID_Key, 160, 0x8000A00C, 0 }, 
{ &GUID_Key, 161, 0x8000A10C, 0 }, 
{ &GUID_Key, 162, 0x8000A20C, 0 }, 
{ &GUID_Key, 163, 0x8000A30C, 0 }, 
{ &GUID_Key, 164, 0x8000A40C, 0 }, 
{ &GUID_Key, 165, 0x8000A50C, 0 }, 
{ &GUID_Key, 166, 0x8000A60C, 0 }, 
{ &GUID_Key, 167, 0x8000A70C, 0 }, 
{ &GUID_Key, 168, 0x8000A80C, 0 }, 
{ &GUID_Key, 169, 0x8000A90C, 0 }, 
{ &GUID_Key, 170, 0x8000AA0C, 0 }, 
{ &GUID_Key, 171, 0x8000AB0C, 0 }, 
{ &GUID_Key, 172, 0x8000AC0C, 0 }, 
{ &GUID_Key, 173, 0x8000AD0C, 0 }, 
{ &GUID_Key, 174, 0x8000AE0C, 0 }, 
{ &GUID_Key, 175, 0x8000AF0C, 0 }, 
{ &GUID_Key, 176, 0x8000B00C, 0 }, 
{ &GUID_Key, 177, 0x8000B10C, 0 }, 
{ &GUID_Key, 178, 0x8000B20C, 0 }, 
{ &GUID_Key, 179, 0x8000B30C, 0 }, 
{ &GUID_Key, 180, 0x8000B40C, 0 }, 
{ &GUID_Key, 181, 0x8000B50C, 0 }, 
{ &GUID_Key, 182, 0x8000B60C, 0 }, 
{ &GUID_Key, 183, 0x8000B70C, 0 }, 
{ &GUID_Key, 184, 0x8000B80C, 0 }, 
{ &GUID_Key, 185, 0x8000B90C, 0 }, 
{ &GUID_Key, 186, 0x8000BA0C, 0 }, 
{ &GUID_Key, 187, 0x8000BB0C, 0 }, 
{ &GUID_Key, 188, 0x8000BC0C, 0 }, 
{ &GUID_Key, 189, 0x8000BD0C, 0 }, 
{ &GUID_Key, 190, 0x8000BE0C, 0 }, 
{ &GUID_Key, 191, 0x8000BF0C, 0 }, 
{ &GUID_Key, 192, 0x8000C00C, 0 }, 
{ &GUID_Key, 193, 0x8000C10C, 0 }, 
{ &GUID_Key, 194, 0x8000C20C, 0 }, 
{ &GUID_Key, 195, 0x8000C30C, 0 }, 
{ &GUID_Key, 196, 0x8000C40C, 0 }, 
{ &GUID_Key, 197, 0x8000C50C, 0 }, 
{ &GUID_Key, 198, 0x8000C60C, 0 }, 
{ &GUID_Key, 199, 0x8000C70C, 0 }, 
{ &GUID_Key, 200, 0x8000C80C, 0 }, 
{ &GUID_Key, 201, 0x8000C90C, 0 }, 
{ &GUID_Key, 202, 0x8000CA0C, 0 }, 
{ &GUID_Key, 203, 0x8000CB0C, 0 }, 
{ &GUID_Key, 204, 0x8000CC0C, 0 }, 
{ &GUID_Key, 205, 0x8000CD0C, 0 }, 
{ &GUID_Key, 206, 0x8000CE0C, 0 }, 
{ &GUID_Key, 207, 0x8000CF0C, 0 }, 
{ &GUID_Key, 208, 0x8000D00C, 0 }, 
{ &GUID_Key, 209, 0x8000D10C, 0 }, 
{ &GUID_Key, 210, 0x8000D20C, 0 }, 
{ &GUID_Key, 211, 0x8000D30C, 0 }, 
{ &GUID_Key, 212, 0x8000D40C, 0 }, 
{ &GUID_Key, 213, 0x8000D50C, 0 }, 
{ &GUID_Key, 214, 0x8000D60C, 0 }, 
{ &GUID_Key, 215, 0x8000D70C, 0 }, 
{ &GUID_Key, 216, 0x8000D80C, 0 }, 
{ &GUID_Key, 217, 0x8000D90C, 0 }, 
{ &GUID_Key, 218, 0x8000DA0C, 0 }, 
{ &GUID_Key, 219, 0x8000DB0C, 0 }, 
{ &GUID_Key, 220, 0x8000DC0C, 0 }, 
{ &GUID_Key, 221, 0x8000DD0C, 0 }, 
{ &GUID_Key, 222, 0x8000DE0C, 0 }, 
{ &GUID_Key, 223, 0x8000DF0C, 0 }, 
{ &GUID_Key, 224, 0x8000E00C, 0 }, 
{ &GUID_Key, 225, 0x8000E10C, 0 }, 
{ &GUID_Key, 226, 0x8000E20C, 0 }, 
{ &GUID_Key, 227, 0x8000E30C, 0 }, 
{ &GUID_Key, 228, 0x8000E40C, 0 }, 
{ &GUID_Key, 229, 0x8000E50C, 0 }, 
{ &GUID_Key, 230, 0x8000E60C, 0 }, 
{ &GUID_Key, 231, 0x8000E70C, 0 }, 
{ &GUID_Key, 232, 0x8000E80C, 0 }, 
{ &GUID_Key, 233, 0x8000E90C, 0 }, 
{ &GUID_Key, 234, 0x8000EA0C, 0 }, 
{ &GUID_Key, 235, 0x8000EB0C, 0 }, 
{ &GUID_Key, 236, 0x8000EC0C, 0 }, 
{ &GUID_Key, 237, 0x8000ED0C, 0 }, 
{ &GUID_Key, 238, 0x8000EE0C, 0 }, 
{ &GUID_Key, 239, 0x8000EF0C, 0 }, 
{ &GUID_Key, 240, 0x8000F00C, 0 }, 
{ &GUID_Key, 241, 0x8000F10C, 0 }, 
{ &GUID_Key, 242, 0x8000F20C, 0 }, 
{ &GUID_Key, 243, 0x8000F30C, 0 }, 
{ &GUID_Key, 244, 0x8000F40C, 0 }, 
{ &GUID_Key, 245, 0x8000F50C, 0 }, 
{ &GUID_Key, 246, 0x8000F60C, 0 }, 
{ &GUID_Key, 247, 0x8000F70C, 0 }, 
{ &GUID_Key, 248, 0x8000F80C, 0 }, 
{ &GUID_Key, 249, 0x8000F90C, 0 }, 
{ &GUID_Key, 250, 0x8000FA0C, 0 }, 
{ &GUID_Key, 251, 0x8000FB0C, 0 }, 
{ &GUID_Key, 252, 0x8000FC0C, 0 }, 
{ &GUID_Key, 253, 0x8000FD0C, 0 }, 
{ &GUID_Key, 254, 0x8000FE0C, 0 }, 
{ &GUID_Key, 255, 0x8000FF0C, 0 } 
];

__gshared  
const DIDATAFORMAT c_dfDIKeyboard_ = { 24, 16, 0x2, 256, 256, & c_rgodfDIKeyboard[0] };

static extern(Windows)
LRESULT WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
      case WM_SIZE:
    case WM_PAINT:
          Win32_PostSysScreen (null);
    default:
        break;
  }

  return DefWindowProcA (hWnd, message, wParam, lParam);
}
static ~this () {
  _gs_d3ddevice.Release ();
  _gs_d3droot.Release ();
  _gs_dikeyboard_device.Release ();
  _gs_dimouse_device.Release ();
  _gs_dideivce_root.Release ();
  _gs_d3dsurface_offscreen.Release ();
  writeln ("Hello World\n");
}
// module construct
static this () {

  //
  _gs_keybuffer = null;
  _gs_mouse = null;
  
  _gs_d3droot = Direct3DCreate9 (D3D_SDK_VERSION);
  enforce (_gs_d3droot !is null);

  // create d3ddevice 
  // 
  HRESULT io_success_;
  WNDCLASSA wndclass;
  const char *ccname = cast (char *) "ezui_moecmks";

  wndclass.style         = 0;
  wndclass.lpfnWndProc   = & WindowProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = GetModuleHandle (null);
  wndclass.hIcon         = LoadIcon (wndclass.hInstance, cast(LPSTR) (101));
  wndclass.hCursor       = LoadCursor (null, IDC_ARROW);
  wndclass.hbrBackground = cast(HBRUSH) GetStockObject(NULL_BRUSH);
  wndclass.lpszMenuName  = null;
  wndclass.lpszClassName = ccname ;

  enforce ( RegisterClass(& wndclass));

  _gs_window = CreateWindowA (ccname,  // window class name
                         cast (char *) "ezui-funny ui for Dlang",    // window caption
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT,        // initial x position
                         CW_USEDEFAULT,        // initial y position
                         600,                  // initial x size
                         400,                  // initial y size
                         null,         // parent window handle
                         null,                 // window menu handle
                         wndclass.hInstance,            // program instance handle
                         null);  

  enforce ( _gs_window !is null);

  (cast(char *)& _gs_d3dpp)[0.._gs_d3dpp.sizeof] = 0;

  _gs_d3dpp.BackBufferFormat     = D3DFMT_X8R8G8B8;
  _gs_d3dpp.SwapEffect           = D3DSWAPEFFECT_COPY;
  //_gs_d3dpp.Flags                = D3DPRESENTFLAG_DEVICECLIP | D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
  _gs_d3dpp.hDeviceWindow        = _gs_window;
  _gs_d3dpp.Windowed             = true;
  _gs_d3dpp.BackBufferHeight     = GetSystemMetrics (SM_CYSCREEN);
  _gs_d3dpp.BackBufferWidth      = GetSystemMetrics (SM_CXSCREEN);

  // create dinput8 device. 
  enforce ( DirectInput8Create( GetModuleHandle (null), 0x0800,
                  cast (GUID *)& IID_IDirectInput8, cast ( VOID ** )& _gs_dideivce_root, null ) == S_OK);

  enforce (_gs_d3droot.CreateDevice (0, D3DDEVTYPE_HAL, _gs_window, 
                                                 D3DCREATE_SOFTWARE_VERTEXPROCESSING, & _gs_d3dpp, 
                                                 & _gs_d3ddevice) == S_OK);

  enforce ( _gs_d3ddevice.CreateOffscreenPlainSurface ( 
                          _POW2 (GetSystemMetrics (SM_CXSCREEN)) * 4,
                          GetSystemMetrics (SM_CYSCREEN) * 3, 
                          D3DFMT_X8R8G8B8,
                          D3DPOOL_SYSTEMMEM, 
                          & _gs_d3dsurface_offscreen,
                          null) == S_OK);

  // c_dfDIMouse2, not mine. see dinput8.lib\dilib5.obj [result from IDA] and fSDK's dilib500000003.c
  // DEFINE_GUID(c_dfDIMouse2_, 0x00000018, 0x0010, 0x0000, 0x02, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00);
  // const GUID c_dfDIMouse2_ = { 0x00000018, 0x0010, 0x0000, [0x02, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00 ] };
  // const GUID c_dfDIKeyboard_ = { 0x00000018, 0x0010, 0x0000, [0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 ] };

  enforce ( _gs_dideivce_root.CreateDevice( cast (GUID *) & GUID_SysMouse, & _gs_dimouse_device, null ) == S_OK); 
  enforce ( _gs_dimouse_device.SetDataFormat (cast (DIDATAFORMAT *) & c_dfDIMouse2_) == S_OK); 
  enforce ( _gs_dimouse_device.SetCooperativeLevel (_gs_window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) == S_OK);
  //enforce ( _gs_dimouse_device.Acquire () == S_OK);
  enforce ( _gs_dideivce_root.CreateDevice( cast (GUID *) & GUID_SysKeyboard, & _gs_dikeyboard_device, null ) == S_OK);.    
  enforce ( _gs_dikeyboard_device.SetDataFormat (cast (DIDATAFORMAT *) & c_dfDIKeyboard_) == S_OK); 
  enforce ( _gs_dikeyboard_device.SetCooperativeLevel (_gs_window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) == S_OK);
  //enforce ( _gs_dikeyboard_device.Acquire () == S_OK);
  
  linker.GetMouseInfos = & Win32_GetMouseInfos;
  linker.GetKeyBoardBuffer = & Win32_GetKeyBoardBuffer;
  linker.GetHostClientSize = & Win32_GetHostClientSize;
  linker.LockSysScreenEmuBuffer = & Win32_LockSysScreenEmuBuffer;
  linker.UnlockSysScreenEmuBuffer = & Win32_UnlockSysScreenEmuBuffer;
  linker.PostSysScreen = & Win32_PostSysScreen;
  linker.AllocTexture = & Win32_AllocTexture;
  linker.ReleaseTexture = & Win32_ReleaseTexture;
  linker.Vid_Blit = & Win32_Vid_Blit;
  linker.Blit = & Win32_Blit;
//linker.construct = & Win32_construct;
//linker.deconstruct = & Win32_deconstruct;
  linker.procedure = & Win32_procedure;
  linker.SetHostVisual = & Win32_SetHostVisual;
  linker.SetHostInvisiual = & Win32_SetHostInvisiual;
  linker.SetHostCursorVisual = & Win32_SetHostCursorVisual;
  linker.SetHostCursorInvisiual = & Win32_SetHostCursorInvisiual;
  linker.SetHostCaption = & Win32_SetHostCaption;
  linker.SetHostClientSize = & Win32_SetHostClientSize;
  linker.isCurrentHostFocus = & Win32_isCurrentHostFocus;
}

pure int _POW2 (int n) {
  int S; for (S = 1; S < n; S *= 2) {} 
  return S; 
}

pure void *_ptralign (void *scp, size_t align_) {
  // align address
  char *vmp = cast (char *)scp;
  size_t ofs = cast (size_t) vmp;
  size_t ofsi= align_ - ofs;
  // MEM:0xFFFFFFFF, ALIGN:16.ADD:1   R:17
  // MEM:0x00000001, ALIGN:16.ADD:15  R:15 
  return cast(void *) & vmp [ofsi % align_];
}

extern(C)
void 
vemcpy  /* XXX: only for x86-32 or WOW64, so use int */
( void *target, int target_x, int target_y, int target_pitch,
  void *source, int source_x, int source_y, int source_pitch, int width, int height);

extern(C)
void 
vemcpy2  /* XXX: only for x86-32 or WOW64, so use int */
( void *target, int target_x, int target_y, int target_pitch,
  void *source, int source_x, int source_y, int source_pitch, int width, int height)
{
  int *target_ = cast (int *) target;
  int *source_ = cast (int *) source;
  int target_pitch_ = target_pitch / 4;
  int source_pitch_ = source_pitch / 4;
  int  x;
  int  y;

target_ = & target_[target_y * target_pitch_ + target_x];
source_ = & source_[source_y * source_pitch_ + source_x];


  for (y= 0; y != height; y++)
  {
      for (x= 0; x != width; x++)
  {
target_[x+y*target_pitch_]
=source_[x+y*source_pitch_];

  }
  }

}

extern (C)
int Win32_AllocTexture (vdesc **recv, string file, short width, short height) {

  assert (width != 0);
  assert (height != 0);

  {
    IDirect3DTexture9 tex;

    if (D3DXCreateTextureFromFileExA (_gs_d3ddevice, cast (char *) file, 
           width, height, -3, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
                                    D3DX_DEFAULT, // D3DX_DEFAULT 
                                    D3DX_DEFAULT, // D3DX_DEFAULT 
                                    0, //D3DCOLOR_XRGB (0, 0, 0),  
                                    null, null, & tex) == S_OK)
    {
      D3DLOCKED_RECT Vdesc;
      HRESULT hr = S_OK;
      vdesc Gdesc;

      // lock it 
      hr = tex.LockRect (0, & Vdesc, null, D3DLOCK_READONLY);
      assert (hr == S_OK);

      // calc/alloc backbuffer  
      Gdesc.pitch = _POW2 (width) * 4; // step 4 * RGB-depth
      Gdesc.width = width;
      Gdesc.height = height;
      Gdesc.RAW32ptr = cast(int *) new char [ Gdesc.pitch * Gdesc.height + 128];
      Gdesc.RGB32ptr = cast(int *) _ptralign (Gdesc.RAW32ptr, 64);

      vemcpy2 (Gdesc.RGB32ptr, 0, 0, Gdesc.pitch, 
              Vdesc.pBits, 0, 0, Vdesc.Pitch, width, height);

      // unlock., release.
      tex.UnlockRect = 0;
      tex.Release;

      vdesc *chunk = new vdesc;
      *chunk = Gdesc;

      *recv = chunk;
      return 0;
    }
    return -1;
  }
}

extern (C)
int Win32_ReleaseTexture (vdesc *recv) {

  delete recv.RAW32ptr;
  delete recv;

  return 0;
}

extern (C) 
int Win32_LockSysScreenEmuBuffer (vdesc *desc) {

  RECT RT;
  D3DLOCKED_RECT DRT;

  RT.top = _gs_d3dpp.BackBufferHeight;
  RT.left = _POW2 (_gs_d3dpp.BackBufferWidth);
  RT.bottom = RT.top + _gs_d3dpp.BackBufferHeight;
  RT.right = RT.left + _POW2 (_gs_d3dpp.BackBufferWidth);

  HRESULT hr = _gs_d3dsurface_offscreen.LockRect (& DRT, null, D3DLOCK_DISCARD);

  if (hr == S_OK)
  {
    GetClientRect (_gs_window, & RT);

    desc.width = RT.right;
    desc.height = RT.bottom;
    desc.pitch = DRT.Pitch;
    desc.RGB32ptr = cast (int *) DRT.pBits;
    desc.RAW32ptr = cast (int *) DRT.pBits;

    return 0;
  }

  return -1;
}

extern (C) 
int Win32_UnlockSysScreenEmuBuffer () {

  HRESULT hr = _gs_d3dsurface_offscreen.UnlockRect;

  if (hr == S_OK)
  {
    // _gs_d3dsurface_offscreen = null;
    return 0;
  }

  return -1;
}

extern (C)
int Win32_isCurrentHostFocus () {

  if (_gs_window == GetFocus ())
  {
    return 0;
  }

  return -1;
}

extern (C)
int Win32_SetHostCaption (string caption) {

  SetWindowTextA (_gs_window, cast (char *)caption);
  return 0;
}

extern (C)
int Win32_SetHostInvisiual () {

  ShowWindow (_gs_window, SW_HIDE);
  return 0;
}

extern (C)
int Win32_SetHostVisual () {

  ShowWindow (_gs_window, SW_SHOWNORMAL);
  UpdateWindow (_gs_window);

  return 0;
}

extern (C)
int Win32_SetHostCursorInvisiual () {

 s:int c = ShowCursor (FALSE);
  if (c<0)return 0;
  else goto s;
  
  return 0;
}

extern (C)
int Win32_SetHostCursorVisual () {

 s:int c = ShowCursor (TRUE);
  if (c>=0)return 0;
  else goto s;
  
  return 0;
}

extern (C)
int Win32_SetHostClientSize (_Vector *_vSize) {

	RECT rect;
	if ( !GetClientRect( _gs_window, &rect ) ) return FALSE;

	rect.right = _vSize.width;
	rect.bottom = _vSize.height;

	if ( !AdjustWindowRectEx( &rect, WS_OVERLAPPEDWINDOW, FALSE, 0 ) ) {
		return FALSE;
  }

	return ! SetWindowPos( _gs_window, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE );
}


// module deconstruct
static ~this () {

  if (_gs_d3droot !is null) {
    _gs_d3droot.Release ();
  }

  if (_gs_dideivce_root !is null) {
    _gs_dideivce_root.Release ();
  }
}
  
extern (C)
int Win32_GetKeyBoardBuffer (linker.keyboard_buffer **_keyinfos) {
  *_keyinfos = _gs_keybuffer;
  return 0;
}

extern (C)
linker.keyboard_buffer *Win32_GetKeyBoardBuffer_ () {

  // http://www.ascii-code.com/
  // http://www.asciitable.com/
  
  string build_keyboard_buffer_init (string val_symbols) {
    string result = "static align(32) __gshared linker.keyboard_buffer "~val_symbols~"={";
    int idx =0;
    foreach (int poll; 0..1+(linker.keyboard_buffer.slash.offsetof/linker.keyboard_key_infos.sizeof)) {
      result ~= "{linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY,"~to!string (idx++)~"},";
    } 
    result ~= "[";
    foreach (int poll; 0..10) {
      result ~= "{linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY,"~to!string (idx++)~"}";
      if (poll != 9) {
        result ~= ",";  
      } 
    }
    result ~= "],";
    foreach (int poll; 0..8) {
      result ~= "{linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY,"~to!string (idx++)~"},";
    }
    result ~= "[";
    foreach (int poll; 0..26) {
      result ~= "{linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY,"~to!string (idx++)~"}";
      if (poll != 25) {
        result ~= ",";  
      }
    }
    result ~= "],";
    foreach (int poll; 0..13) {
      result ~= "{linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY,"~to!string (idx++)~"}";
      if (poll != 12) {
        result ~= ",";  
      }    
    }
    result ~= ", false};";
    return result;
  }
  
  string setkey (string val_symbols, string DIK_POS) {
    string result = "if (dikeybuf[cur+"~DIK_POS~"] & 0x80) {";
       result ~=    "  if (dikeybuf[old+"~DIK_POS~"] & 0x80) {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_CONTINUITY;";
       result ~=    "  } else {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_FIRST;";
       result ~=    "  }";
       result ~=    "} else {";
       result ~=    "  if (dikeybuf[old+"~DIK_POS~"] & 0x80) {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_FIRST;";
       result ~=    "  } else {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY;";
       result ~=    "  }";
       result ~=    "}"; 
    return result;
  }
  
  string setkey2 (string val_symbols, string DIK_POS, string DIK_POS2) {
    string result = "if (dikeybuf[cur+"~DIK_POS~"] & 0x80) {";
       result ~=    "  if (dikeybuf[old+"~DIK_POS~"] & 0x80) {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_CONTINUITY;";
       result ~=    "  } else {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_FIRST;";
       result ~=    "  }";
       result ~=    "} else {";
       result ~=    "  if (dikeybuf[old+"~DIK_POS~"] & 0x80) {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_FIRST;";
       result ~=    "  } else {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY;";
       result ~=    "  }";
       result ~=    "}";
       // 
       result ~=    "if (dikeybuf[cur+"~DIK_POS2~"] & 0x80) {";
       result ~=    "  if (dikeybuf[old+"~DIK_POS2~"] & 0x80) {";
       result ~=    "    key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_CONTINUITY;";
       result ~=    "  } else {";
       result ~=    "    if (key_recv."~val_symbols~".status != linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_CONTINUITY)";
       result ~=    "      key_recv."~val_symbols~".status= linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_FIRST;";
       result ~=    "  }";
       result ~=    "}";
    return result;
  }
  
  string setkey_assert (string assert_, string val_symbols, string DIK_POS) {
    string result = "if (" ~ assert_ ~ ") {";
       result ~=       setkey (val_symbols, DIK_POS);
       result ~=    "}";
       
    return result;
  }

  string setkey_assert2 (string val_symbols, string assert_, string assert2_, string DIK_POS, string DIK_POS2) {
    string result = "if ("~assert_~") {";
       result ~=    "  if ("~assert2_~") {";
       result ~=         setkey2 (val_symbols, DIK_POS, DIK_POS2);
       result ~=    "  } else {";
       result ~=         setkey (val_symbols, DIK_POS);
       result ~=    "  }";
       result ~=    "} else {";
       result ~=    "  if ("~assert2_~") {";
       result ~=         setkey (val_symbols, DIK_POS2);
       result ~=    "  } else {";
       result ~=    "    int ii = 0;";
       result ~=    "  }";
       result ~=    "}";
       
    return result;
  }
  
  static align(32) char[512] dikeybuf =0; // You should know that for the bulk assignment of an array, \
                               // DMD takes the rep stos assembly instruction, \
                               // so alignment is very important
  static int switch_bank = 0;
  
  mixin (build_keyboard_buffer_init("key_recv"));
  
  // poll dinput keyboard. 
  // _gs_dikeyboard_device.GetDeviceState ()
  int scan_count = 0;
rep_scan:
  HRESULT res = _gs_dikeyboard_device.GetDeviceState (256, & dikeybuf[switch_bank]);
  switch (res) {
  case DIERR_NOTACQUIRED:
  case DIERR_INPUTLOST:
    HRESULT res2 = _gs_dikeyboard_device.Acquire ();
    switch (res2) {
      case DIERR_INVALIDPARAM:
      case DIERR_NOTINITIALIZED:
        enforce (false); break;
      case DIERR_OTHERAPPHASPRIO:
        // We deal with this mistake mainly.
        // This error indicates that the current window is not a top-level window, \
        // so the input focus cannot be obtained 
        dikeybuf[] = 0; // clear all buffer
      case E_HANDLE:
      default:
        writeln ("lost focus");
        return null;
      case S_OK:
      case S_FALSE:
        if (++scan_count > 300) {
          enforce (false);
        } else {
          goto rep_scan;     
        }
    }
    break;
  case DIERR_NOTINITIALIZED:
  case E_PENDING:
  case DIERR_INVALIDPARAM:
    enforce (false); break;
  default:
    writeln ("errors unknown", __LINE__);
  case S_OK:
    break;
  }

  switch_bank ^= 256;
  
  int cur = switch_bank ^256;
  int old = switch_bank;
  
  // check ascii. 
  bool caplocks = cast(bool) (GetKeyState(VK_CAPITAL) & 1);
  bool numlocks= cast(bool) (GetKeyState(VK_NUMLOCK) & 1);
  bool shift = cast(bool) ( dikeybuf[cur+DIK_LSHIFT] & 0x80);
       shift = shift || cast(bool) ( dikeybuf[cur+DIK_RSHIFT] & 0x80);
  key_recv.bigletter_hold = !! ((shift || caplocks) && ! (shift && caplocks));
  
  immutable char[26] letter_cov = 
  [ DIK_A, DIK_B, DIK_C, DIK_D, DIK_E, DIK_F, DIK_G, DIK_H,
    DIK_I, DIK_J, DIK_K, DIK_L, DIK_M, DIK_N, DIK_O, DIK_P,
    DIK_Q, DIK_Q, DIK_S, DIK_T, DIK_U, DIK_V, DIK_W, DIK_X, DIK_Y, DIK_Z ];
  
  // scan ascii-letter.
  foreach (int ii; 0..26) {
    if (dikeybuf[cur+letter_cov[ii]] & 0x80) {
      if (dikeybuf[old+letter_cov[ii]] & 0x80) key_recv.letter26[ii].status = linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_CONTINUITY;
      else key_recv.letter26[ii].status = linker.KEYBOARD_KEY_STATUS.KEYBOARD_PRESS_FIRST;        
    } else {
      if (dikeybuf[old+letter_cov[ii]] & 0x80) key_recv.letter26[ii].status = linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_FIRST;
      else key_recv.letter26[ii].status = linker.KEYBOARD_KEY_STATUS.KEYBOARD_UP_CONTINUITY;
    }
  }
  
  // scan ascii-remain 
  mixin (setkey ("backspace", "DIK_BACKSPACE"));
  mixin (setkey2 ("enter", "DIK_RETURN", "DIK_NUMPADENTER")); 
  mixin (setkey ("space", "DIK_SPACE"));
  mixin (setkey ("esc", "DIK_ESCAPE"));
  mixin (setkey ("tab", "DIK_TAB"));
  mixin (setkey_assert ("shift", "parentheses_right", "DIK_0"));
  mixin (setkey_assert ("shift", "exclam", "DIK_1"));
  mixin (setkey_assert ("shift", "at", "DIK_2"));
  mixin (setkey_assert ("shift", "pound", "DIK_3"));
  mixin (setkey_assert ("shift", "dollar", "DIK_4"));  
  mixin (setkey_assert ("shift", "percent","DIK_5"));  
  mixin (setkey_assert ("shift", "caret","DIK_6")); 
  mixin (setkey_assert ("shift", "ref_", "DIK_7"));
  mixin (setkey_assert2("asterisk", "shift", "true", "DIK_8", "DIK_MULTIPLY"));
  mixin (setkey_assert ("shift", "parentheses_left", "DIK_9"));
  mixin (setkey_assert ("shift", "underline", "DIK_MINUS"));
  mixin (setkey_assert2("equal", "shift", "true", "DIK_EQUALS", "DIK_NUMPADPLUS"));
  mixin (setkey_assert2("sub", "!shift", "true", "DIK_MINUS", "DIK_SUBTRACT"));
  mixin (setkey_assert2("add", "!shift", "true", "DIK_EQUALS", "DIK_NUMPADEQUALS"));
  mixin (setkey_assert ("shift", "tilde", "DIK_GRAVE"));
  mixin (setkey_assert ("!shift", "grave_accent", "DIK_GRAVE"));
  mixin (setkey_assert ("shift", "brace_left", "DIK_LBRACKET"));
  mixin (setkey_assert ("!shift", "bracket_left", "DIK_LBRACKET"));
  mixin (setkey_assert ("shift", "brace_right", "DIK_RBRACKET"));
  mixin (setkey_assert ("!shift", "bracket_right", "DIK_RBRACKET"));
  mixin (setkey_assert ("shift", "colon", "DIK_SEMICOLON"));
  mixin (setkey_assert ("!shift", "semi_colon", "DIK_SEMICOLON"));
  mixin (setkey_assert ("shift", "dquot", "DIK_APOSTROPHE"));
  mixin (setkey_assert ("!shift", "squot", "DIK_APOSTROPHE"));
  mixin (setkey_assert ("shift", "less", "DIK_COMMA"));
  mixin (setkey_assert ("!shift", "comma", "DIK_COMMA"));
  mixin (setkey_assert ("shift", "large", "DIK_PERIOD"));
  mixin (setkey_assert2("dot", "!shift", "numlocks", "DIK_PERIOD", "DIK_DECIMAL"));
  mixin (setkey_assert ("shift", "question", "DIK_SLASH"));
  mixin (setkey_assert2("slash", "!shift", "numlocks", "DIK_SLASH", "DIK_DIVIDE"));
 
  // number 
  mixin (setkey_assert2 ("numb_0_9[0]", "!shift", "numlocks", "DIK_0", "DIK_NUMPAD0"));
  mixin (setkey_assert2 ("numb_0_9[1]", "!shift", "numlocks", "DIK_1", "DIK_NUMPAD1"));
  mixin (setkey_assert2 ("numb_0_9[2]", "!shift", "numlocks", "DIK_2", "DIK_NUMPAD2"));
  mixin (setkey_assert2 ("numb_0_9[3]", "!shift", "numlocks", "DIK_3", "DIK_NUMPAD3"));
  mixin (setkey_assert2 ("numb_0_9[4]", "!shift", "numlocks", "DIK_4", "DIK_NUMPAD4"));  
  mixin (setkey_assert2 ("numb_0_9[5]", "!shift", "numlocks", "DIK_5", "DIK_NUMPAD5"));  
  mixin (setkey_assert2 ("numb_0_9[6]", "!shift", "numlocks", "DIK_6", "DIK_NUMPAD6")); 
  mixin (setkey_assert2 ("numb_0_9[7]", "!shift", "numlocks", "DIK_7", "DIK_NUMPAD7"));
  mixin (setkey_assert2 ("numb_0_9[8]", "!shift", "numlocks", "DIK_8", "DIK_NUMPAD8"));
  mixin (setkey_assert2 ("numb_0_9[9]", "!shift", "numlocks", "DIK_9", "DIK_NUMPAD9"));
  // some ctrl 
  mixin (setkey2 ("alt", "DIK_LALT", "DIK_RALT")); 
  mixin (setkey2 ("ctrl", "DIK_LCONTROL", "DIK_RCONTROL")); 
  mixin (setkey2 ("shift", "DIK_LSHIFT", "DIK_RSHIFT"));   
  
  static if (true) {
    COORD pos = { 0, 0 };  
    SetConsoleCursorPosition ( GetStdHandle (STD_OUTPUT_HANDLE), pos); 
    
    writeln ("tab:", key_recv.tab.status);
    writeln ("shift:", key_recv.shift.status);
    writeln ("alt:", key_recv.alt.status);
    writeln ("ctrl:", key_recv.ctrl.status);
    writeln ("Aa:", key_recv.letter26[0].status);
    writeln ("Bb:", key_recv.letter26[1].status);
    writeln ("Cc:", key_recv.letter26[2].status);
    writeln ("Dd:", key_recv.letter26[3].status);
    writeln ("Ee:", key_recv.letter26[4].status);
    writeln ("Ff:", key_recv.letter26[5].status);
    writeln ("Gg:", key_recv.letter26[6].status);
    writeln ("Hh:", key_recv.letter26[7].status);
    writeln ("0:", key_recv.numb_0_9[0].status);
    writeln ("1:", key_recv.numb_0_9[1].status);
    writeln ("2:", key_recv.numb_0_9[2].status);
    writeln ("3:", key_recv.numb_0_9[3].status);
    writeln ("4:", key_recv.numb_0_9[4].status);
    writeln ("5:", key_recv.numb_0_9[5].status);
    writeln ("6:", key_recv.numb_0_9[6].status);
    writeln ("7:", key_recv.numb_0_9[7].status);
    writeln ("@:", key_recv.at.status);
    writeln ("~:", key_recv.tilde.status);
    writeln ("`:", key_recv.grave_accent.status);
    
  }
  return  & key_recv;
}

extern (C) // XXX:simple dirty check.
int Win32_PostSysScreen (_RectRel *radraw) {

  RECT rcRect;
  GetClientRect (_gs_window, & rcRect);
  
  if (radraw is null) {
    _gs_d3ddevice.Present (& rcRect, & rcRect, null, null);
    return 0;
  } else if ((radraw.left == radraw.right) || (radraw.top == radraw.bottom)) {
    return 0;
  } else {
  
    BYTE[RGNDATAHEADER.sizeof + RECT.sizeof * 4] rgnDirty;
    RGNDATA * RGN = cast (RGNDATA *) & rgnDirty[0];
    RECT *rtPtr = cast (RECT *) & RGN.Buffer[0];
    
    RGN.rdh.dwSize = RGNDATAHEADER.sizeof;
    RGN.rdh.iType = RDH_RECTANGLES;
    RGN.rdh.nCount = 0;
    RGN.rdh.nRgnSize = 0;
    RGN.rdh.rcBound.left = 0;
    RGN.rdh.rcBound.right = GetSystemMetrics (SM_CXSCREEN);
    RGN.rdh.rcBound.top = 0;
    RGN.rdh.rcBound.bottom = GetSystemMetrics (SM_CYSCREEN);

    rtPtr[RGN.rdh.nCount].top = radraw.top;
    rtPtr[RGN.rdh.nCount].bottom = radraw.bottom;
    rtPtr[RGN.rdh.nCount].left = radraw.left;
    rtPtr[RGN.rdh.nCount].right = radraw.right;
      
    RGN.rdh.nCount++;
    RGN.rdh.nRgnSize += RECT.sizeof; 
      
    _gs_d3ddevice.Present (& rcRect, & rcRect, null, RGN);
  }
  return 0;
}


extern (C)
int Win32_GetMouseInfos (linker.mouse_infos **recv) {

  *recv = _gs_mouse;
  return 0;
}

extern (C)
linker.mouse_infos *Win32_GetMouseInfos_ () {

  POINT point;
  static __gshared align(32) DIMOUSESTATE2[2] mouse2;
  static __gshared align(32) linker.mouse_infos recv;
  
  GetCursorPos (& point);
  ScreenToClient ( _gs_window, & point);

  recv.pos.x = cast (short) point.x;
  recv.pos.y = cast (short) point.y;

  static int switch_bank = 0;

  // poll dinput mouse. 
  // _gs_dimouse_device.GetDeviceState ()
  int scan_count = 0;
rep_scan:
  HRESULT res = _gs_dimouse_device.Poll ();
  switch (res) {
  case DIERR_NOTACQUIRED:
  case DIERR_INPUTLOST:
  rep_scan2:
    HRESULT res2 = _gs_dimouse_device.Acquire ();
    switch (res2) {
      case DIERR_INVALIDPARAM:
      case DIERR_NOTINITIALIZED:
        enforce (false); break;
      case DIERR_OTHERAPPHASPRIO:
        // We deal with this mistake mainly.
        // This error indicates that the current window is not a top-level window, \
        // so the input focus cannot be obtained 
        mouse2[0].rgbButtons[0] = 0;
        mouse2[0].rgbButtons[1] = 0;
        mouse2[1].rgbButtons[0] = 0;
        mouse2[1].rgbButtons[1] = 0;
      case E_HANDLE:
      default:
        writeln ("lost focus");
        return null;
      case DIERR_INPUTLOST:
        goto rep_scan2;
      case S_OK:
        if (++scan_count > 300) {
          enforce (false);
        } else {
          goto rep_scan;     
        }
    }
    break;
  case DIERR_NOTINITIALIZED:
  case E_PENDING:
  case DIERR_INVALIDPARAM:
    enforce (false); break;
  default:
    writeln ("errors unknown", __LINE__);
  case S_OK:
  //case DI_NOEFFECT:
  case S_FALSE:
    HRESULT res3 = _gs_dimouse_device.GetDeviceState (DIMOUSESTATE2.sizeof, & mouse2[switch_bank]);
    switch (res3) {
    case DIERR_NOTACQUIRED:
    case DIERR_INPUTLOST:
      HRESULT res2 = _gs_dikeyboard_device.Acquire ();
      switch (res2) {
        case DIERR_INVALIDPARAM:
        case DIERR_NOTINITIALIZED:
          enforce (false); break;
        case DIERR_OTHERAPPHASPRIO:
          // We deal with this mistake mainly.
          // This error indicates that the current window is not a top-level window, \
          // so the input focus cannot be obtained 
          mouse2[0].rgbButtons[0] = 0;
          mouse2[0].rgbButtons[1] = 0;
          mouse2[1].rgbButtons[0] = 0;
          mouse2[1].rgbButtons[1] = 0;
        case E_HANDLE:
        default:
          writeln ("lost focus");
          return null;
        case S_FALSE:
        case S_OK:
          if (++scan_count > 300) {
            enforce (false);
          } else {
            goto rep_scan;     
          }
      }
      break;
    case DIERR_NOTINITIALIZED:
    case E_PENDING:
    case DIERR_INVALIDPARAM:
      enforce (false); break;
    default:
      writeln ("errors unknown", __LINE__);
    case S_OK:
    
      switch_bank ^= 1;
      
      int cur = switch_bank ^1;
      int old = switch_bank;
      
      if (mouse2[cur].rgbButtons[0] & 0x80) {
        if (mouse2[old].rgbButtons[0] & 0x80) recv.button_left = linker.MOUSE_BUTTON_STATUS.MOUSE_PRESS_CONTINUITY;
        else recv.button_left = linker.MOUSE_BUTTON_STATUS.MOUSE_UP_FIRST;
      } else {
        if (mouse2[old].rgbButtons[0] & 0x80) recv.button_left = linker.MOUSE_BUTTON_STATUS.MOUSE_PRESS_FIRST;
        else recv.button_left = linker.MOUSE_BUTTON_STATUS.MOUSE_UP_CONTINUITY;
      }

      if (mouse2[cur].rgbButtons[1] & 0x80) {
        if (mouse2[old].rgbButtons[1] & 0x80) recv.button_right = linker.MOUSE_BUTTON_STATUS.MOUSE_PRESS_CONTINUITY;
        else recv.button_right = linker.MOUSE_BUTTON_STATUS.MOUSE_UP_FIRST;
      } else {
        if (mouse2[old].rgbButtons[1] & 0x80) recv.button_right = linker.MOUSE_BUTTON_STATUS.MOUSE_PRESS_FIRST;
        else recv.button_right = linker.MOUSE_BUTTON_STATUS.MOUSE_UP_CONTINUITY;
      }
      break;
    }
    break;
  }
  return & recv;
}

extern (C)
int Win32_GetHostClientSize (_Rect *MaxSize, _Rect *CurrentSize) {

  if (MaxSize !is null) {
    RECT rcRect;

    GetClientRect (_gs_window, & rcRect);

    CurrentSize.rel.left = cast (short) rcRect.left;
    CurrentSize.rel.right = cast (short) rcRect.right;
    CurrentSize.rel.top = cast (short) rcRect.top;
    CurrentSize.rel.bottom = cast (short) rcRect.bottom;
    CurrentSize.covToAbs ();
  }

  if (CurrentSize !is null) {

    MaxSize.abs.x = 0;
    MaxSize.abs.y = 0;
    MaxSize.abs.w = cast (short) GetSystemMetrics (SM_CXSCREEN);
    MaxSize.abs.h = cast (short) GetSystemMetrics (SM_CYSCREEN);

    MaxSize.covToRel ();
  }

  return 0;
}

extern (C)
int Win32_Blit (vdesc *ddesc, vdesc *sdesc,
             short ddx, short ddy, 
           short ssx, short ssy, 
                short w,  short h) {

  vemcpy (ddesc.RGB32ptr, cast (int) ddx, cast (int) ddy, cast (int) ddesc.pitch, 
          sdesc.RGB32ptr, cast (int) ssx, cast (int) ssy, cast (int) sdesc.pitch, 
          cast (int) w, cast (int) h);

  return 0;
}

extern (C)
int Win32_Vid_Blit (short ddx, short ddy, 
           short ssx, short ssy, 
                short w,  short h) {

  HRESULT io_success_ = _gs_d3ddevice.GetBackBuffer (0, 0, D3DBACKBUFFER_TYPE_MONO, & _gs_d3dsurface_main);
  RECT targetRect;
  RECT sourceRect;
  POINT point;

  if (io_success_ == S_OK) {
  
    sourceRect.top = ssy;
    sourceRect.left = ssx;
    sourceRect.bottom = ssy + h;
    sourceRect.right = ssx + w;
    
    point.x = ddx;
    point.y = ddy;

    io_success_ = _gs_d3ddevice.UpdateSurface (_gs_d3dsurface_offscreen , & sourceRect,
                   _gs_d3dsurface_main, & point);

    _gs_d3dsurface_main.Release ();
  } else {
    _gs_d3dsurface_main = null;
  }
  return 0;
}

extern (C)
int Win32_procedure (void *param) {

  MSG msg;

  while (TRUE) { 

    if (PeekMessageA (& msg, null, 0, 0, PM_REMOVE)) { 
      if (msg.message == WM_QUIT) 
        break ; 
      // TranslateMessage (& msg); 
      DispatchMessageA (& msg); 
    } 
    else { 
      // collect host's context 
      _gs_keybuffer = Win32_GetKeyBoardBuffer_ ();
      _gs_mouse = Win32_GetMouseInfos_ ();
      
      // call ezui's scheduler
      ezui_parasitic (null);
    } 
  } 
  return msg.wParam ;
}


int main ()
{
  vdesc *tim;
  vdesc emu;
  Win32_GetKeyBoardBuffer_ ();
  Win32_AllocTexture (& tim, "timg.jpg", 819, 1225);
  Win32_LockSysScreenEmuBuffer (& emu);
  Win32_Blit (& emu, tim, 0, 0, 0, 0, 819, 1225);
  Win32_UnlockSysScreenEmuBuffer ();
  Win32_Vid_Blit (0, 0, 0, 0, 600, 600);
  _Vector s = {255, 244};
  Win32_SetHostClientSize (& s);
  
  Win32_SetHostVisual;
  Win32_procedure (null);
  
  return 0;
}
