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

#ifndef system_H 
#define system_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "stdint.h"
#include "config0.h"

/* System environment initialization */
cvimpl int system_init (void *init, void *loop, void *dtor);
cvimpl void system_uninit (void);
cvimpl int system_schedule (void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif