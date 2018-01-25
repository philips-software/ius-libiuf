//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusFFTC.h
//                  Fast Fourier Transform for complex-valued signals.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file
 * Fast Fourier Transform for complex-valued signals.
 */

#ifndef _IUSFFTC_H
#define _IUSFFTC_H

#include "iusComplex.h"
#include <stddef.h>        // for size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
* FFTC_MAXFFTSIZE : Maximum size of FFT, power of 2, please choose as small  
* as possible to reduce execution time and memory usage.                     
*/
#ifndef IUSFFTC_MAXFFTSIZE
#define IUSFFTC_MAXFFTSIZE          (8192) 
#endif

/**
 \brief Initializes fft tables.
 
 \post        fft tables initialized.
 \param[in]   numPoints          : Number of points (FFT size), power of 2.
*/
void iusInitFFTC
(
    const size_t numPoints
);

/**
\brief Performs the FFT for complex-valued input data.

 \pre         The vector io was allocated and initialized (and InitFFT called).
 \post        The vector io contains the transformed data.
 \param[io]   io           : Array io[0..numPoints-1] of float.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.
*/
void iusFFTC
(
    iusComplexFloatVector* const pio,
    const size_t  N
);

/**
\brief Performs the IFFT for complex-valued input data.

 \pre         The vector io was allocated and initialized (and InitFFT called).
 \post        The vector io contains the transformed data.
 \param[io]   io           : Array io[0..numPoints-1] of float.
 \param[in]   numPoints    : Number of points (FFT size), power of 2.
 \return      void
*/
void iusIFFTC
(
    iusComplexFloatVector* const pio,
    const size_t  N
);


#ifdef __cplusplus
}
#endif

#endif
