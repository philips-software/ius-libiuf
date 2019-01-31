//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusBasicFunctions.h
//                  Basic functions and macros.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#ifndef _IUSBASICFUNCTIONS_H
#define _IUSBASICFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusTypes.h"

/** 
 * \file iusBasicFunctions.h
 * 
 * \brief Basic functions and macro definitions of IUS
 *
 */

//=============================================================================
// M A C R O S
//=============================================================================

//! Determine maximum of a and b.
#define IUS_MAX(a, b)	            ( ( (a) > (b) ) ? (a) : (b) )

//! Determine minimum of a and b.
#define IUS_MIN(a, b)	            ( ( (a) < (b) ) ? (a) : (b) )

//! Maximum of a, b, c.
#define IUS_MAX3(a, b, c)           ( IUS_MAX( IUS_MAX((a),(b)), (c) ) )

//! Minimum of a, b, c.
#define IUS_MIN3(a, b, c)           ( IUS_MIN( IUS_MIN((a),(b)), (c) ) )

//! Maximum of a, b, c, d with 3 comparisons.
#define IUS_MAX4(a, b, c, d)        ( IUS_MAX( IUS_MAX((a),(b)), IUS_MAX((c),(d)) ) )

//! Minimum of a, b, c, d with 3 comparisons.
#define IUS_MIN4(a, b, c, d)        ( IUS_MIN( IUS_MIN((a),(b)), IUS_MIN((c),(d)) ) )

//! Clip b between a and c, a < c.  The alternate use where a is clipped between b and c is also valid
#define IUS_CLIP(a, b, c)           ( IUS_MIN( IUS_MAX((a),(b)), (c)) )

//! Determine median of a, b and c.
#define IUS_MEDIAN(a, b, c)         ( IUS_MIN( IUS_MAX( IUS_MIN((a),(b)), (c)), IUS_MAX((a),(b))) )

//! Determine absolute value of a.
#define IUS_ABS(a)	                ( ( (a) < (0) ) ? (-(a)) : (a) )

//! modulo (wrapped) value
#define IUS_MOD(val, mod)           ( ( (val) + (mod) ) % (mod) )

//! Pi as a floating pointer
#define IUS_PI       (3.14159265f)

//! a small flaot value
#define IUS_FLT_EPS  (1.19209290E-07f)

//! the max floating point value
#define IUS_MAXFLOAT (1.0E37f)

//! maximum string value
#define IUS_MAX_STRING_LENGTH (1024)


#ifdef __cplusplus
}
#endif

#endif // _IUSBASICFUNCTIONS_H
