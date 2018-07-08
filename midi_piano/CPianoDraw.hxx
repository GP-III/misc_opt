# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CPIANODRAW_INCLUED_HEADER_CXX_)
#  define _CPIANODRAW_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "resource.h"
# include "CDirectDraw.hxx"
# include <dinput.h>

// draw piano's key¡£
namespace CPianoDraw__  {
# define HALF_MASK 0x80 
  static struct dummt_ {
    TCHAR *harmonica_scale;
    TCHAR *piano_scale;
    // TCHAR *bd_scale;   emmm...
    int octave_step;
    int note;
    int midi_pos;
  } keyboard_mapper[48] = {
    // low
    { _T("(1)"), _T ("(C)"),  0, 1,             0-12 }, // 1
    { _T("(#1)"), _T ("(C#)"), 0, 1 | HALF_MASK, 1-12 }, //#1
    { _T("(2)"), _T ("(D)"),  0, 2,             2-12 }, // 2
    { _T("(#2)"), _T ("(D#)"), 0, 2 | HALF_MASK, 3-12 }, //#2
    { _T("(3)"), _T ("(E)"),  0, 3,             4-12 }, // 3
    { _T("(4)"), _T ("(F)"),  0, 4,             5-12 }, // 4
    { _T("(#4)"), _T ("(F#)"), 0, 4 | HALF_MASK, 6-12 }, //#4
    { _T("(5)"), _T ("(G)"),  0, 5 ,            7-12 }, // 5
    { _T("(#5)"), _T ("(G#)"), 0, 5 | HALF_MASK, 8-12 }, //#5
    { _T("(6)"), _T ("(A)"),  0, 6 ,            9-12 }, // 6
    { _T("(#6)"), _T ("(A#)"), 0, 6 | HALF_MASK,10-12 }, //#6
    { _T("(7)"), _T ("(B)"),  0, 7 ,           11-12 }, // 7
    // mid
    { _T("1"), _T ("C"),   1, 1,             0+12-12 }, // 1
    { _T("#1"), _T ("C#"),  1, 1 | HALF_MASK, 1+12-12 }, // #1
    { _T("2"), _T ("D"),   1, 2,             2+12-12 }, // 2
    { _T("#2"), _T ("D#"),  1, 2 | HALF_MASK, 3+12-12 }, // #2
    { _T("3"), _T ("E"),   1, 3,             4+12-12 }, // 3
    { _T("4"), _T ("F"),   1, 4,             5+12-12 }, // 4
    { _T("#4"), _T ("F#"),  1, 4 | HALF_MASK, 6+12-12 }, // #4
    { _T("5"), _T ("G"),   1, 5 ,            7+12-12 }, // 5
    { _T("#5"), _T ("G#"),  1, 5 | HALF_MASK, 8+12-12 }, // #5
    { _T("6"), _T ("A"),   1, 6 ,            9+12-12 }, //  6
    { _T("#6"), _T ("A#"),  1, 6 | HALF_MASK,10+12-12 }, //  #6
    { _T("7"), _T ("B"),   1, 7 ,           11+12-12 }, // 7
    // high
    { _T("[1]"), _T ("[C]"),   2, 1,             0+24-12 }, // 1
    { _T("[#1]"), _T ("[C#]"),  2, 1 | HALF_MASK, 1+24-12 }, // #1
    { _T("[2]"), _T ("[D]"),   2, 2,             2+24-12 }, // 2
    { _T("[#2]"), _T ("[D#]"),  2, 2 | HALF_MASK, 3+24-12 }, // #2
    { _T("[3]"), _T ("[E]"),   2, 3,             4+24-12 }, // 3
    { _T("[4]"), _T ("[F]"),   2, 4,             5+24-12 }, // 4
    { _T("[#4]"), _T ("[F#]"),  2, 4 | HALF_MASK, 6+24-12 }, // #4
    { _T("[5]"), _T ("[G]"),   2, 5 ,            7+24-12 }, // 5
    { _T("[#5]"), _T ("[G#]"),  2, 5 | HALF_MASK, 8+24-12 }, // #5
    { _T("[6]"), _T ("[A]"),   2, 6 ,            9+24-12 }, //  6
    { _T("[#6]"), _T ("[A#]"),  2, 6 | HALF_MASK,10+24-12 }, //  #6
    { _T("[7]"), _T ("[B]"),   2, 7 ,           11+24-12 }, // 7
    // high level2
    { _T("<1>"), _T ("<C>"),   3, 1,             0+36-12 }, // 1
    { _T("<#1>"), _T ("<C#>"),  3, 1 | HALF_MASK, 1+36-12 }, // #1
    { _T("<2>"), _T ("<D>"),   3, 2,             2+36-12 }, // 2
    { _T("<#2>"), _T ("<D#>"),  3, 2 | HALF_MASK, 3+36-12 }, // #2
    { _T("<3>"), _T ("<E>"),   3, 3,             4+36-12 }, // 3
    { _T("<4>"), _T ("<F>"),   3, 4,             5+36-12 }, // 4
    { _T("<#4>"), _T ("<F#>"),  3, 4 | HALF_MASK, 6+36-12 }, // #4
    { _T("<5>"), _T ("<G>"),   3, 5 ,            7+36-12 }, // 5
    { _T("<#5>"), _T ("<G#>"),  3, 5 | HALF_MASK, 8+36-12 }, // #5
    { _T("<6>"), _T ("<A>"),   3, 6 ,            9+36-12 }, //  6
    { _T("<#6>"), _T ("<A#>"),  3, 6 | HALF_MASK,10+36-12 }, //  #6
    { _T("<7>"), _T ("<B>"),   3, 7 ,           11+36-12 } // 7
  };
  struct dummt_ *dummt_map[256];

  static void LoadDefaultSettings (void)  {
    ZeroMemory (dummt_map, sizeof (dummt_map));

    dummt_map[DIK_Z] = & keyboard_mapper[0];
    dummt_map[DIK_S] = & keyboard_mapper[1];
    dummt_map[DIK_X] = & keyboard_mapper[2];
    dummt_map[DIK_D] = & keyboard_mapper[3];
    dummt_map[DIK_C] = & keyboard_mapper[4];
    dummt_map[DIK_V] = & keyboard_mapper[5];
    dummt_map[DIK_F] = & keyboard_mapper[6];
    dummt_map[DIK_B] = & keyboard_mapper[7];
    dummt_map[DIK_G] = & keyboard_mapper[8];
    dummt_map[DIK_N] = & keyboard_mapper[9];
    dummt_map[DIK_H] = & keyboard_mapper[10];
    dummt_map[DIK_M] = & keyboard_mapper[11];  // low .
    dummt_map[DIK_Q] = & keyboard_mapper[0+12];
    dummt_map[DIK_2] = & keyboard_mapper[1+12];
    dummt_map[DIK_W] = & keyboard_mapper[2+12];
    dummt_map[DIK_3] = & keyboard_mapper[3+12];
    dummt_map[DIK_E] = & keyboard_mapper[4+12];
    dummt_map[DIK_R] = & keyboard_mapper[5+12];
    dummt_map[DIK_4] = & keyboard_mapper[6+12];
    dummt_map[DIK_T] = & keyboard_mapper[7+12];
    dummt_map[DIK_5] = & keyboard_mapper[8+12];
    dummt_map[DIK_Y] = & keyboard_mapper[9+12];
    dummt_map[DIK_6] = & keyboard_mapper[10+12];
    dummt_map[DIK_U] = & keyboard_mapper[11+12]; // middle.
    dummt_map[DIK_I] = & keyboard_mapper[0+24];
    dummt_map[DIK_9] = & keyboard_mapper[1+24];
    dummt_map[DIK_O] = & keyboard_mapper[2+24];
    dummt_map[DIK_0] = & keyboard_mapper[3+24];
    dummt_map[DIK_P] = & keyboard_mapper[4+24];
    dummt_map[DIK_LBRACKET] = & keyboard_mapper[5+24];
    dummt_map[DIK_MINUS] = & keyboard_mapper[6+24];
    dummt_map[DIK_RBRACKET] = & keyboard_mapper[7+24];
    dummt_map[DIK_EQUALS] = & keyboard_mapper[8+24];
    dummt_map[DIK_BACKSLASH] = & keyboard_mapper[9+24];
    dummt_map[DIK_BACK] = & keyboard_mapper[10+24];
    dummt_map[DIK_RETURN] = & keyboard_mapper[11+24]; // higth.
    dummt_map[DIK_NUMPAD1] = & keyboard_mapper[0+36];
    dummt_map[DIK_NUMPAD0] = & keyboard_mapper[1+36];
    dummt_map[DIK_NUMPAD2] = & keyboard_mapper[2+36];
    dummt_map[DIK_DECIMAL] = & keyboard_mapper[3+36];
    dummt_map[DIK_NUMPAD3] = & keyboard_mapper[4+36];
    dummt_map[DIK_NUMPAD4] = & keyboard_mapper[5+36];
    dummt_map[DIK_NUMLOCK] = & keyboard_mapper[6+36];
    dummt_map[DIK_NUMPAD5] = & keyboard_mapper[7+36];
    dummt_map[DIK_DIVIDE] = & keyboard_mapper[8+36];
    dummt_map[DIK_NUMPAD6] = & keyboard_mapper[9+36];
    dummt_map[DIK_MULTIPLY] = & keyboard_mapper[10+36];
    dummt_map[DIK_NUMPAD7] = & keyboard_mapper[11+36]; // higth. 2
  }

  static const DWORD sm_ColBlack = 0xffd200; // Piano black key coloring
  static const DWORD sm_ColWhite = 0x00fffc; // Piano white key coloring
  static const BYTE sm_ShadowBlackAlpha = 32; // Black key Alpha percentage
  static const BYTE sm_ShadowWhiteAlpha = 32; // White key Alpha percentage
  // black key base offset 
  static const int left_remain = 3;
  static const POINT sm_note_half1 = { 11 - left_remain, 1 }; // #1 
  static const POINT sm_note_half2 = { 47 - left_remain, 1 }; // #2
  static const POINT sm_note_half4 = {103 - left_remain, 1 }; // #4
  static const POINT sm_note_half5 = {138 - left_remain, 1 }; // #5
  static const POINT sm_note_half6 = {169 - left_remain, 1 }; // #6 
  // white key base offset 
  static const POINT sm_note_1 = {  3 - left_remain, 1 }; // 1  X0Y54 W20H106 | X0Y0 W7H54
  static const POINT sm_note_2 = { 33 - left_remain, 1 }; // 2  X0Y54 W21H106 | X6Y0 W7H54
  static const POINT sm_note_3 = { 64 - left_remain, 1 }; // 3  X0Y54 W21H106 | X11Y0 W10H54
  static const POINT sm_note_4 = { 94 - left_remain, 1 }; // 4  X0Y54 W22H106 | X0Y0 W8H54
  static const POINT sm_note_5 = {126 - left_remain, 1 }; // 5  X0Y54 W20H106 | X5Y0 W6H154
  static const POINT sm_note_6 = {156 - left_remain, 1 }; // 6  X0Y54 W20H106 | X10Y0 W2H154
  static const POINT sm_note_7 = {186 - left_remain, 1 }; // 7  X0Y54 W20H106 | X11Y0 W9H54
  // 3rd octave offset 
  static const int  octave_0 =   3;
  static const int  octave_1 = 215;
  static const int  octave_2 = 427;
  static const int  octave_set[3] = { octave_0, octave_1, octave_2} ;

  void drawPiano (CDirectDraw ::graph_desc &gd, int octave_step, int note /* start from 1*/)  {
    int octave_step_ = octave_step;
    int note_ = note & 0x7; 
    if (octave_step_ == 3) octave_step_ = 2;
    const int base_x = octave_set [octave_step_];
    if  (FALSE != !! (note & HALF_MASK))    {
      // draw black 
      const POINT *pt;
      switch  (note_)   {
      case  1: pt = & sm_note_half1;  break ;
      case  2: pt = & sm_note_half2;  break ;
      case  4: pt = & sm_note_half4;  break ;
      case  5: pt = & sm_note_half5;  break ;
      case  6: pt = & sm_note_half6;  break ;
      default :   ATLASSERT (0);       break ;
      }
      CDirectDraw ::CSurfaceSingleAlpha (gd, 
                    CPoint ( pt->x + base_x + 1, pt->y + 1), 
                    CSize (27, 53), 
                  sm_ColBlack, 
                     sm_ShadowBlackAlpha, 
                   255 - sm_ShadowBlackAlpha);
    } else  {
      // draw white. 
      int pos_x;
      RECT part2 = { 0, 3, 0, 57};
      POINT *base_pos = NULL;
      const POINT *base_pc = NULL;
      switch  (note_)   {
      case  1: base_pc = & sm_note_1; pos_x = 20; part2.right = 7;  break ;
      case  2: base_pc = & sm_note_2; pos_x = 21; part2.left = 6;  part2.right = 13; break ;
      case  3: base_pc = & sm_note_3; pos_x = 21; part2.left = 11; part2.right = 21; break ;
      case  4: base_pc = & sm_note_4; pos_x = 22; part2.right = 8; break ;
      case  5: base_pc = & sm_note_5; pos_x = 20; part2.left = 5;  part2.right = 11; break ;
      case  6: base_pc = & sm_note_6; pos_x = 20; part2.left = 10; part2.right = 12; break ;
      case  7: base_pc = & sm_note_7; pos_x = 20; part2.left = 11; part2.right = 20; break ;
      default : ATLASSERT (0);
      }
      CDirectDraw ::CSurfaceSingleAlpha (gd, 
                    CPoint ( base_pc->x + base_x + 1, 56), 
                    CSize (pos_x, 180), 
                  sm_ColWhite, 
                     sm_ShadowWhiteAlpha, 
                   255 - sm_ShadowWhiteAlpha);
      CDirectDraw ::CSurfaceSingleAlpha (gd, 
                    CPoint ( base_pc->x + base_x + 1 + part2.left , 2), 
                    CSize (part2.right - part2.left, part2.bottom - part2.top), 
                  sm_ColWhite, 
                     sm_ShadowWhiteAlpha, 
                   255 - sm_ShadowWhiteAlpha);
    }
  }
  void drawPiano2 (CDirectDraw ::graph_desc &gd, CDirectDraw ::graph_desc &gd2, int octave_step, int note /* start from 1*/)  {
    int octave_step_ = octave_step;
    int note_ = note & 0x7; 
    if (octave_step_ == 3) octave_step_ = 2;
    const int base_x = octave_set [octave_step_];
    if  (FALSE != !! (note & HALF_MASK))    {
      // draw black 
      const POINT *pt;
      switch  (note_)   {
      case  1: pt = & sm_note_half1;  break ;
      case  2: pt = & sm_note_half2;  break ;
      case  4: pt = & sm_note_half4;  break ;
      case  5: pt = & sm_note_half5;  break ;
      case  6: pt = & sm_note_half6;  break ;
      default :   ATLASSERT (0);       break ;
      }
      CDirectDraw ::CSurfaceBlit (gd, 
                    CPoint ( pt->x + base_x + 1, pt->y + 1), 
                    gd2,
                    CPoint ( pt->x + base_x + 1, pt->y + 1), 
                    CSize (27, 53));
    } else  {
      // draw white. 
      int pos_x;
      RECT part2 = { 0, 3, 0, 57};
      POINT *base_pos = NULL;
      const POINT *base_pc = NULL;
      switch  (note_)   {
      case  1: base_pc = & sm_note_1; pos_x = 20; part2.right = 7;  break ;
      case  2: base_pc = & sm_note_2; pos_x = 21; part2.left = 6;  part2.right = 13; break ;
      case  3: base_pc = & sm_note_3; pos_x = 21; part2.left = 11; part2.right = 21; break ;
      case  4: base_pc = & sm_note_4; pos_x = 22; part2.right = 8; break ;
      case  5: base_pc = & sm_note_5; pos_x = 20; part2.left = 5;  part2.right = 11; break ;
      case  6: base_pc = & sm_note_6; pos_x = 20; part2.left = 10; part2.right = 12; break ;
      case  7: base_pc = & sm_note_7; pos_x = 20; part2.left = 11; part2.right = 20; break ;
      default : ATLASSERT (0);
      }
      CDirectDraw ::CSurfaceBlit (gd, 
                    CPoint ( base_pc->x + base_x + 1, 56), 
                    gd2,
                    CPoint ( base_pc->x + base_x + 1, 56), 
                    CSize (pos_x, 180));
      CDirectDraw ::CSurfaceBlit (gd, 
                    CPoint ( base_pc->x + base_x + 1 + part2.left , 2), 
                    gd2,
                    CPoint ( base_pc->x + base_x + 1 + part2.left , 2), 
                    CSize (part2.right - part2.left, part2.bottom - part2.top));
    }
  }
};

# endif // !defined(_CPIANODRAW_INCLUED_HEADER_CXX_)