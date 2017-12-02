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

#ifndef screen_H 
#define screen_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "system.h"

/* Pixel format:A0R8G8B8, client size:256 * 240 */
void screen_uninit_256_240 (void);
int screen_init_256_240 (void);
int screen_bitblt_256_240oop (void *source, int pitch);
/* Width is the texture's pitch */
cvimpl void *callc load_imageSqu128 (const char *name);
cvimpl void callc free_imageSqu128 (void *image);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif