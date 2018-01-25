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


/**
 * \brief Sets the seed for the random number generator.
 * 
 * \post        Seed value has been set.
 * \param[in]   seed      :  input for srand()
 */
void iusSetRandomSeed
(
    unsigned int seed
);

/**
 * \brief Returns the input if the input is an integer multiple of 32. Returns the next
 * larger multiple of 32 otherwise.
 * \pre        Input is non-negative
 * \post       The result of the operation is returned.
 * \param[in]  v    :  input scaler
 * \return     result of the operation
 */
int iusNextMultipleOf32
(
    int v
);

/**
 * \brief Returns the input if the input is a power of 2. Otherwise it returns
 * the next larger number that is a power of two.
 * 
 * \pre        Input is non-negative
 * \post       The result of the operation is returned.
 * \param[in]  v    :  input scaler
 * \return     result of the operation
 */
int iusNextPowerOfTwo
(
    int v
);

/**
 * \brief Returns true iff the input is an integer power of 2.
 * 
 * \pre        Input is non-negative
 * \post       The result of the operation is returned.
 * \param[in]  v    :  input scaler
 * \return     result of the operation
 */
int iusIsPowerOfTwo
(
    int v
);


/**
 * \brief Returns log2(a), iff a is an integer power of 2.
 * 
 * \pre        Input is positive.
 * \post       The result of the operation is returned.
 * \param[in]  a    :  input scaler
 * \return     result of the log2 operation
 */
int iusIntLog2
(
    int a
);


/**
 * \brief Allocates and initializes an IusGrid
 * 
 * \pre        Valid values for dim1, dim2, dim3 (0 is allowed)
 * \post       IusGrid allocated and initialized
 * \param[in]  dim0 : first  grid dimension (elevation)
 * \param[in]  dim1 : second grid dimension (lateral)
 * \param[in]  dim2 : third  grid dimension (axial)
 * \return     pointer to an IusGrid
 */
IusGrid * iusCreateGrid
(
    int dim0,
    int dim1,
    int dim2
);


/**
 * \brief Frees memory of an IusGrid
 * 
 * \pre        IusGrid was created by iusCreateGrid()
 * \post       IusGrid freed
 * \param[in]  pGrid : pointer to an IusGrid
 * \return     void
 */
void iusFreeGrid
(
    IusGrid * pGrid
);


/**
 * \brief Copy all internals from one IusGrid to another
 * 
 * \pre        Source and destination grids were created by iusCreateGrid()
 * \post       Internal values of destination grid copied from internal values
 *             of source grid
 * \param[in]  pGridDest : pointer to destination grid
 * \param[in]  pGridSrc  : pointer to source grid
 * \return     void
 */
void iusCopyGrid
(
    IusGrid * const       pGridDest,
    const IusGrid * const pGridSrc
);


#ifdef __cplusplus
}
#endif

#endif // _IUSBASICFUNCTIONS_H
