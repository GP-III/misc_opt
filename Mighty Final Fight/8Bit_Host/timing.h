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

#ifndef timing_H 
#define timing_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "system.h"

int timing_init60 (void);
int timing_start60 (void);
int timing_terming60 (void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif