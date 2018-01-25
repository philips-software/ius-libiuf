//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusDFT.h
//                  Discrete Fourier Transform for complex-valued signals.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file 
 * Discrete Fourier Transform for complex-valued signals.
 */
#ifndef _IUSDFT_H
#define _IUSDFT_H


#include "iusComplex.h"
#include <stddef.h>        // for size_t


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialization of DFT tables.
 *
 * This function should be called once before using any of the DFT and IDFT functions
 * \post        dft tables initialized.
 * \param[in]   numPoints  : Number of points (DFT size), even.
 */
void iusInitDFT
(
    const size_t numPoints
);

/**
 * \brief Performs the DFT for complex-valued input data.
 * 
 * \pre         The vectors in,out were allocated, and in was initialized (and InitDFT called).
 * \post        The vector out contains the transformed data.
 * \param[in]   in           : Array in[0..numPoints-1] of complex float.
 * \param[out]  out          : Array in[0..numPoints-1] of complex float.
 * \param[in]   numPoints    : Number of points (DFT size), must be even.
 */
void iusDFT
(
    const iusComplexFloat* const in,
          iusComplexFloat* const out,
    const size_t numPoints
);

/**
 * \brief Performs the IDFT for complex-valued input data.
 * 
 *  \pre         The vectors in,out were allocated, and in was initialized (and InitDFT called).
 *  \post        The vector out contains the transformed data.
 *  \param[in]   in           : Array in[0..numPoints-1] of complex float.
 *  \param[out]  out          : Array in[0..numPoints-1] of complex float.
 *  \param[in]   numPoints    : Number of points (DFT size), even.
 */
void iusIDFT
(
    const iusComplexFloat* const in,
          iusComplexFloat* const out,
    const size_t numPoints
);

#ifdef __cplusplus
}
#endif

#endif
