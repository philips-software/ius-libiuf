//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusFFT.h
//                  Fast Fourier Transform for real-valued signals.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file 
 * Fast Fourier Transform for real-valued signals.
 */

#ifndef _IUSFFT_H
#define _IUSFFT_H

/*============================================================================*/
/* Included modules.                                                          */
/*============================================================================*/
#include <stddef.h>        // for size_t
#include "iusComplex.h"

/*============================================================================*/
/* C++ protection.                                                            */
/*============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/* Constants and Macros for this module.                                      */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* FFT_MAXFFTSIZE : Maximum size of FFT, power of 2, please choose as small   */
/* as possible to reduce execution time and memory usage.                     */
/*----------------------------------------------------------------------------*/
#ifndef IUSFFT_MAXFFTSIZE
#define IUSFFT_MAXFFTSIZE          (8192)
#endif

/*============================================================================*/
/* Type definitions.                                                          */
/*============================================================================*/

/*============================================================================*/
/* Function prototypes.                                                       */
/*============================================================================*/

/**
\brief Initializes fft tables.
 \post        fft tables initialized.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.
*/
void iusInitFFT
(
    const size_t numPoints
);

/** Performs the FFT for real-valued input data.
 \pre         The vector io was allocated and initialized (and InitFFT called).
 \post        The vector io contains the transformed data.
 \param[io]   io           : Array io[0..numPoints-1] of float.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.

*/
void iusFFT
(
    float* const io,
    const size_t numPoints
);

/** \brief Performs the IFFT for complex conjugate-symmetric input data.

 \pre         The vector io was allocated and initialized (and InitFFT called).
 \post        The vector io contains the transformed data.
 \param[io]   io           : Array io[0..numPoints-1] of float.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.
*/
void iusIFFT
(
    float* const io,
    const size_t numPoints
);

/**
\brief Sorts FFT output data into linear frequency ordered [re,im] bins.

 \pre         The vector io was allocated and initialized.
 \post        The vector io contains the transformed data.
 \param[io]   io           : Array io[0..numPoints-1] of float.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.
*/
void iusFFTsort
(
    const size_t numPoints,
    float* const io
);

/**
\brief Unsorts FFT output data from linear order into internal FFT order.

 \pre         The vector io was allocated and initialized.
 \post        The vector io contains the transformed data.
 \param[io]   io           : Array io[0..numPoints-1] of float.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.
*/
void iusFFTunsort
(
    const size_t numPoints,
    float* const io
);

/**
\brief Performs element-wise complex multiplication of the FFT transformed vectors.

 \pre         The vectors io,in were allocated and initialized with FFT().
 \post        The vector io contains the multiplication result.
 \param[io]   io           : Array io[0..numPoints-1] of float.
 \param[in]   in           : Array in[0..numPoints-1] of float.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.
*/
void iusMultiplyFloatFFVectors
(
          float* const __restrict io,
    const float* const __restrict in,
    const size_t                  numPoints
);

/*============================================================================*/
/* Close when C++ protected.                                                  */
/*============================================================================*/
#ifdef __cplusplus
}
#endif

/*============================================================================*/
/* End of multiple inclusion protection.                                      */
/*============================================================================*/
#endif
