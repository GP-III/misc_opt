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

#ifndef input_H 
#define input_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "system.h"

/*
 * Joypad mask
 */
#define JOYPAD_MASK_PRESS 0x01
#define JOYPAD_MASK_LOOSE 0x02
#define JOYPAD_MASK_FIRST 0x80

struct joypad {
  int8_t left;
  int8_t right;
  int8_t up;
  int8_t down;
  int8_t b;
  int8_t a;
  int8_t select;
  int8_t start;   
};

/* - normal keymapper - *\
+        left:A          | 
+        right:D         | 
+        up:W            |
+        down:S          |
|        b:J             +
|        a:K             +
|        select:space    +
|        start:enter     +
\* - normal keymapper - */ 

int joypad_init (void);
void joypad_uninit (void);
int joypad_poll (void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif