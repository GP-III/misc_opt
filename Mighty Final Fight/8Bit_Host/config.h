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

#ifndef config_H 
#define config_H 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*
 * !!! Temporarily only support Windows.
 */
#if !defined (_WIN32) && !defined (_WIN64)
#  error unsupported platform
#endif 

#if defined (__ICC) || defined (__INTEL_COMPILER)
#  define __INTEL_ICL_
#elif defined (_MSC_VER)
#  define __MSVC_CL_
#elif defined (__GNUC__) || defined (__GNUG__)
#  define __GNU_C_
#else 
#  error unsupported compiler
#endif 

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif