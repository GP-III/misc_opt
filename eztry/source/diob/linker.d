/*
  Platform API compatibility layer 
*/
module ezui.dbio.linker;


struct _Point {

  short x;
  short y;
}

struct _Vector {

  short width;
  short height;
}

struct _RectAbs {

  short x;
  short y;
  short w;
  short h;
}

struct _RectRel {

  short left;
  short right;
  short top;
  short bottom;
}

struct _Rect {

  _RectAbs abs;
  _RectRel rel;

  void covToAbs () {

    abs.x = rel.left;
    abs.y = rel.right;
    abs.w = cast (short) (rel.right - rel.left);
    abs.h = cast (short) (rel.bottom - rel.top);
  }

  void covToRel () {

    rel.left = abs.x;
    rel.right = cast (short) (abs.x + abs.w);
    rel.top = abs.y;
    rel.bottom = cast (short) (abs.y + abs.h);
  }
}

struct vdesc {

  int width;
  int height;
  int pitch;
  int *RGB32ptr;
  int *RAW32ptr;
}

class linker {

  // about mouse 
  enum MOUSE_BUTTON_STATUS {
    MOUSE_UP_FIRST = -1788811,
    MOUSE_UP_CONTINUITY,
    MOUSE_PRESS_FIRST,
    MOUSE_PRESS_CONTINUITY
  }

  struct mouse_infos {
    _Point pos;
    MOUSE_BUTTON_STATUS button_left;
    MOUSE_BUTTON_STATUS button_mid;
    MOUSE_BUTTON_STATUS button_right;
  }

  static 
  extern (C) int function (mouse_infos **recv) GetMouseInfos;

  // about keyboard 
  // keyboard wo only use ASCII key, ctrl, alt, shift
  enum KEYBOARD_KEY_STATUS {
    KEYBOARD_UP_FIRST = -788851,
    KEYBOARD_UP_CONTINUITY,
    KEYBOARD_PRESS_FIRST,
    KEYBOARD_PRESS_CONTINUITY,
    KEYBOARD_OWN,
	  KEYBOARD_CANCEL
  }

  struct keyboard_key_infos {
    KEYBOARD_KEY_STATUS  status;
	  int idx;
  }
  
  struct keyboard_buffer {
  
    /* 0*/ keyboard_key_infos tab; 
    /* 0*/ keyboard_key_infos esc;
    /* 0*/ keyboard_key_infos backspace; //
    /* 0*/ keyboard_key_infos enter; //
    /* 0*/ keyboard_key_infos space; // ' '
    /* 0*/ keyboard_key_infos exclam;// '!' 
    /* 0*/ keyboard_key_infos dquot; // '"'
    /* 0*/ keyboard_key_infos pound; // '#' 
    /* 0*/ keyboard_key_infos dollar;//'$' 
    /* 0*/ keyboard_key_infos percent;//'%'
    /* 0*/ keyboard_key_infos ref_; //'&'
    /* 0*/ keyboard_key_infos squot;//'''
    /* 0*/ keyboard_key_infos parentheses_left;//'('
    /* 0*/ keyboard_key_infos parentheses_right;//')'    
    /* 0*/ keyboard_key_infos asterisk;//'*' 
    /* 0*/ keyboard_key_infos add;//'+'
    /* 0*/ keyboard_key_infos comma;//','
    /* 0*/ keyboard_key_infos sub;//'-'
    /* 0*/ keyboard_key_infos dot;//'.'
    /* 0*/ keyboard_key_infos slash;//'/'
    /* 0*/ keyboard_key_infos[10] numb_0_9;//
    /* 0*/ keyboard_key_infos colon;//':'
    /* 0*/ keyboard_key_infos semi_colon;//';'
    /* 0*/ keyboard_key_infos less;//'<'
    /* 0*/ keyboard_key_infos equal;//'='
    /* 0*/ keyboard_key_infos large;//'>'
    /* 0*/ keyboard_key_infos question;//'?'
    /* 0*/ keyboard_key_infos at;//'@'
    /* 0*/ keyboard_key_infos tilde; // '~'
    /* 0*/ keyboard_key_infos[26] letter26;//
    /* 0*/ keyboard_key_infos bracket_left;//'['
    /* 0*/ keyboard_key_infos back_slant;//'\'
    /* 0*/ keyboard_key_infos bracket_right;//']'
    /* 0*/ keyboard_key_infos caret;//'^'
    /* 0*/ keyboard_key_infos underline;//'_'
    /* 0*/ keyboard_key_infos grave_accent;//’`'
    /* 0*/ keyboard_key_infos brace_left; //'{'
    /* 0*/ keyboard_key_infos vertical_bar;//'|'
    /* 0*/ keyboard_key_infos brace_right; //'}'    
    /* 0*/ keyboard_key_infos del;//
    /* 0*/ keyboard_key_infos ctrl;
    /* 0*/ keyboard_key_infos alt;   
    /* 0*/ keyboard_key_infos shift; 
	  /* 0*/ bool bigletter_hold;
  }

  static 
  extern (C) int function ( keyboard_buffer **keybuf) GetKeyBoardBuffer;

  // about screen 

  static extern (C) { 
    int function (_Rect *MaxSize, _Rect *CurrentSize) GetHostClientSize;
    int function (vdesc *Vdesc) LockSysScreenEmuBuffer;
    int function () UnlockSysScreenEmuBuffer;
    int function (_RectRel *radraw /* for dirty rect **/) PostSysScreen;
    int function (vdesc **recv, string file, short width, short height) AllocTexture;
    int function (vdesc *recv) ReleaseTexture;
    int function (short target_x, short target_y, short source_x, 
                    short source_y, short width, short height  ) Vid_Blit;
    int function (vdesc *target, vdesc *source, short target_x, 
                 short target_y, short source_x, short source_y,
                   short width, short height  ) Blit;
    // main callback 
  //int function (void *param) construct;
  //int function (void *param) deconstruct;
    int function (void *param) procedure;
    // callback to host 

    // some misc 
    int function () SetHostVisual;
    int function () SetHostInvisiual;
    int function () SetHostCursorVisual;
    int function () SetHostCursorInvisiual;
    int function (string caption) SetHostCaption;
    int function (_Vector *_vSize) SetHostClientSize;
    int function () isCurrentHostFocus; // 0:focus
  } 
}

int ezui_parasitic (void *param)  {
  return 0;
} 
    

