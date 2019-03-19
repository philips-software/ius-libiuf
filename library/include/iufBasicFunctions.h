//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iufBasicFunctions.h
//                  Basic functions and macros.
//  iufVersion    : 255.255.255.255
//
//=============================================================================

#ifndef _IUFBASICFUNCTIONS_H
#define _IUFBASICFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iufTypes.h"

/** 
 * \file iufBasicFunctions.h
 * 
 * \brief Basic functions and macro definitions of IUF
 *
 */

//=============================================================================
// M A C R O S
//=============================================================================

//! Determine maximum of a and b.
#define IUF_MAX(a, b)	            ( ( (a) > (b) ) ? (a) : (b) )

//! Determine minimum of a and b.
#define IUF_MIN(a, b)	            ( ( (a) < (b) ) ? (a) : (b) )

//! Maximum of a, b, c.
#define IUF_MAX3(a, b, c)           ( IUF_MAX( IUF_MAX((a),(b)), (c) ) )

//! Minimum of a, b, c.
#define IUF_MIN3(a, b, c)           ( IUF_MIN( IUF_MIN((a),(b)), (c) ) )

//! Maximum of a, b, c, d with 3 comparisons.
#define IUF_MAX4(a, b, c, d)        ( IUF_MAX( IUF_MAX((a),(b)), IUF_MAX((c),(d)) ) )

//! Minimum of a, b, c, d with 3 comparisons.
#define IUF_MIN4(a, b, c, d)        ( IUF_MIN( IUF_MIN((a),(b)), IUF_MIN((c),(d)) ) )

//! Clip b between a and c, a < c.  The alternate use where a is clipped between b and c is also valid
#define IUF_CLIP(a, b, c)           ( IUF_MIN( IUF_MAX((a),(b)), (c)) )

//! Determine median of a, b and c.
#define IUF_MEDIAN(a, b, c)         ( IUF_MIN( IUF_MAX( IUF_MIN((a),(b)), (c)), IUF_MAX((a),(b))) )

//! Determine absolute value of a.
#define IUF_ABS(a)	                ( ( (a) < (0) ) ? (-(a)) : (a) )

//! modulo (wrapped) value
#define IUF_MOD(val, mod)           ( ( (val) + (mod) ) % (mod) )

//! Pi as a floating pointer
#define IUF_PI       (3.14159265f)

//! a small flaot value
#define IUF_FLT_EPS  (1.19209290E-07f)

//! the max floating point value
#define IUF_MAXFLOAT (1.0E37f)

//! maximum string value
#define IUF_MAX_STRING_LENGTH (1024)



#ifdef __cplusplus
}
#endif

#endif // _IUFBASICFUNCTIONS_H
