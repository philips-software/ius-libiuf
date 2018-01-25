//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusVectorC.h
//                  Basic functions for complex vector memory and operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/**
 * \file \brief This file contains prototypes for operations on complex vectors.
 *
 * Functions for allocation and freeing of memory for complex vectors
 * \li iusAllocComplexDoubleVector, iusAllocComplexFloatVector, iusAllocComplexIntVector, iusAllocComplexShortVector
 * \li iusFreeComplexDoubleVector, iusFreeComplexFloatVector, iusFreeComplexIntVector, iusFreeComplexShortVector
 *
 * Functions for assigning values
 * \li  iusSetValue2ComplexDoubleVector, iusSetValue2ComplexFloatVector, iusSetValue2ComplexIntVector, iusSetValue2ComplexShortVector, iusSetShiftedPhasorTable2ComplexFloatVector
 *
 * Functions for copying
 * \li     ...
 *
 * Operations on vectors and matrices
 * \li     iusMultiplyComplexFloatVectors
 * \li     iusMultiplyConjugateComplexFloatVectors
 * \li     iusAddComplexFloatVectors
 * \li     iusAccComplexFloatVectors
 * \li     iusSubtractComplexFloatVectors
 *
 * and
 * \li     iusPrintComplexDoubleVector, iusPrintComplexFloatVector
 */

#ifndef _IUSVECTORC_H
#define _IUSVECTORC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusComplex.h"

/**
 * \brief Allocates a complex double vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the complex vector was allocated, elements are zero
 * \param[in]   length :  number of complex elements in the vector
 * \return      pointer to the complex double vector
 */
iusComplexDoubleVector * iusAllocComplexDoubleVector
(
    int length
);

/**
 * \brief Allocates a complex float vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the complex vector was allocated, elements are zero
 * \param[in]   length :  number of complex elements in the vector
 * \return      pointer to the complex float vector
 */
iusComplexFloatVector * iusAllocComplexFloatVector
(
    int length
);

/**
 * \brief Allocates a complex int vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the complex vector was allocated, elements are zero
 * \param[in]   length :  number of complex elements in the vector
 * \return      pointer to the complex int vector
 */
iusComplexIntVector * iusAllocComplexIntVector
(
    int length
);

/**
 * \brief Allocates a complex short vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the complex vector was allocated, elements are zero
 * \param[in]   length :  number of complex elements in the vector
 * \return      pointer to the complex short vector
 */
iusComplexShortVector * iusAllocComplexShortVector
(
    int length
);

/**
 * \brief Frees a complex double vector
 *
 * \pre         The complex vector was allocated
 * \post        Memory for the complex vector was freed, vector reference is NULL
 * \param[in]   pComplexDoubleVector :  pointer to a complex double vector
 */
void iusFreeComplexDoubleVector
(
    iusComplexDoubleVector * pComplexDoubleVector
);

/**
 * \brief Frees a complex float vector
 *
 * \pre         The complex vector was allocated
 * \post        Memory for the complex vector was freed, vector reference is NULL
 * \param[in]   pComplexFloatVector :  pointer to a complex float vector
 */
void iusFreeComplexFloatVector
(
    iusComplexFloatVector * pComplexFloatVector
);

/**
 * \brief Frees a complex int vector
 *
 * \pre         The complex vector was allocated
 * \post        Memory for the complex vector was freed, vector reference is NULL
 * \param[in]   pComplexIntVector :  pointer to complex int vector
 */
void iusFreeComplexIntVector
(
    iusComplexIntVector * pComplexIntVector
);

/**
 * \brief Frees a complex short vector
 *
 * \pre         The complex vector was allocated
 * \post        Memory for the complex vector was freed, vector reference is NULL
 * \param[in]   pComplexShortVector :  pointer to a complex short vector
 */
void iusFreeComplexShortVector
(
    iusComplexShortVector * pComplexShortVector
);

/**
 * \brief Sets all elements of a complex double vector to the same value
 * 
 * \pre         The complex double vector was allocated
 * \post        All elements of the complex vector are set to "value"
 * \param[out]  pComplexDoubleVector :  reference to a complex double vector
 * \param[in]   length               :  number of complex vector elements
 * \param[in]   value                :  the complex value
 */
void iusSetValue2ComplexDoubleVector
(
    iusComplexDoubleVector *const pComplexDoubleVector,
    int                           length,
    iusComplexDouble_t            value
);

/**
 * \brief Sets all elements of a complex float vector to the same value
 *
 * \pre         The complex float vector was allocated
 * \post        All elements of the complex vector are set to "value"
 * \param[out]  pComplexFloatVector :  reference to a complex float vector
 * \param[in]   length              :  number of complex vector elements
 * \param[in]   value               :  the complex value
 */
void iusSetValue2ComplexFloatVector
(
    iusComplexFloatVector *const pComplexFloatVector,
    int                          length,
    iusComplexFloat              value
);

/**
 * \brief Sets all elements of a complex int vector to the same value
 *
 * \pre         The complex int vector was allocated
 * \post        All elements of the complex vector are set to "value"
 * \param[out]  pComplexIntVector :  reference to a complex int vector
 * \param[in]   length            :  number of complex vector elements
 * \param[in]   value             :  the complex value
 */
void iusSetValue2ComplexIntVector
(
    iusComplexIntVector *const pComplexIntVector,
    int                        length,
    iusComplexInt_t            value
);

/**
 * \brief Sets all elements of a complex short vector to the same value
 *
 * \pre         The complex short vector was allocated
 * \post        All elements of the complex vector are set to "value"
 * \param[out]  pComplexShortVector :  reference to a complex short vector
 * \param[in]   length              :  number of complex vector elements
 * \param[in]   value               :  the complex value
 */
void iusSetValue2ComplexShortVector
(
    iusComplexShortVector *const pComplexShortVector,
    int                          length,
    iusComplexShort_t            value
);

/**
* \brief Fills a vector with a complex phasor table at a certain normalized frequency
*
* \pre         The vector was allocated
* \post        pComplexFloatVector[n] = exp( j* normFreq * n ) for n = 0, 1, ..., length-1
* \param[out]  pComplexFloatVector:  reference to a vector
* \param[in]   length             :  length of the vector
* \param[in]   normFreq           :  normalized frequency (= 2 * PI * freqHz / sampleRate)
* \param[in]   phase              :  constant phase offset
*/
void iusSetShiftedPhasorTable2ComplexFloatVector
(
    iusComplexFloatVector *pComplexFloatVector,
    int                    length,
    float                  normFreq,  // normFreq  = 2 * PI * freqHz / sampleRate;
    float                  phase
);

/**
 * \brief Copies all data in a complex vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 * \param[out]  pDest  :  reference to the complex destination vector
 * \param[in]   pSrc   :  reference to the complex source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyComplexFloatVector
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc,
    int                                length
);

/**
 * \brief Element-wise multiplication of two complex float vectors
 *
 * In-place:   no (sources and destination may not share same memory space)
 * \pre        Both source and destination vectors were allocated and filled
 *             with complex values, and all have the same number of elements
 * \post       Destination elements with element index i equal:
 *               pDest->pRe[i] = pSrc1->pRe[i] * pSrc2->pRe[i] - pSrc1->pIm[i] * pSrc2->pIm[i];
 *               pDest->pIm[i] = pSrc1->pIm[i] * pSrc2->pRe[i] + pSrc1->pRe[i] * pSrc2->pIm[i];
 * \param[out] pDest  :  reference to the destination vector
 * \param[in]  pSrc1  :  reference to the 1st source vector
 * \param[in]  pSrc2  :  reference to the 2nd source vector
 * \param[in]  length :  number of vector elements
 */
void iusMultiplyComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
);

/**
 * \brief Element-wise conjugate multiplication of two complex float vectors
 *
 * In-place:   no (sources and destination may not share same memory space)
 * \pre        Both source and destination vectors were allocated and filled
 *             with complex values, and all have the same number of elements
 * \post       Destination elements with element index i equal:
 *               pDest->pRe[i] = pSrc1->pRe[i] * pSrc2->pRe[i] + pSrc1->pIm[i] * pSrc2->pIm[i];
 *               pDest->pIm[i] = pSrc1->pIm[i] * pSrc2->pRe[i] - pSrc1->pRe[i] * pSrc2->pIm[i];
 * \param[out] pDest  :  reference to the destination vector
 * \param[in]  pSrc1  :  reference to the 1st source vector
 * \param[in]  pSrc2  :  reference to the 2nd source vector (to be conjugated)
 * \param[in]  length :  number of vector elements
 */
void iusMultiplyConjugateComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
);








/**
 * \brief Element-wise addition of two complex float vectors
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre        Both source and destination vectors were allocated and filled
 *             with complex values, and all have the same number of elements
 * \post       Destination elements with element index i equal:
 *                 
 *                 
 * \param[out] pDest  :  reference to the destination vector
 * \param[in]  pSrc1  :  reference to the 1st source vector
 * \param[in]  pSrc2  :  reference to the 2nd source vector
 * \param[in]  length :  number of vector elements
 */
void iusAddComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
);

/**
 * \brief Element-wise accumulation of two complex float vectors
 *
 * In-place:   pDest values are increased with pSrc values
 * \pre        Both source and destination vectors were allocated and filled
 *             with complex values, and all have the same number of elements
 * \post       Destination elements with element index i equal:
 *                 
 *                 
 * \param[out] pDest  :  reference to the destination vector
 * \param[in]  pSrc   :  reference to the 1st source vector
 * \param[in]  length :  number of vector elements
 */
void iusAccComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc,
    int                                length
);

/**
 * \brief Element-wise subtraction of two complex float vectors
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre        Both source and destination vectors were allocated and filled
 *             with complex values, and all have the same number of elements
 * \post       Destination elements with element index i equal:
 *                 
 *                 
 * \param[out]  pDest  :  reference to the complex destination vector
 * \param[in]   pSrc1  :  reference to the 1st complex source vector
 * \param[in]   pSrc2  :  reference to the 2nd complex source vector
 * \param[in]   length :  number of vector elements
 */
void iusSubtractComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
);

/**
 * \brief Element-wise power of complex float vector
 *
 * In-place:   no.
 * \pre        Both source and destination vectors were allocated and filled
 *             with values, and all have the same number of elements.
 * \post        out[i] = conj(in[i]) * in[i]
 *
 * \param[out]  pOut :  reference to the real-valued destination vector
 * \param[in]   pIn  :   reference to the complex-valued source vector
 * \param[in]   N    :    number of elements in vector
 */
void iusPowerComplexFloatVector
(
    float*                       pOutRe,
    const iusComplexFloatVector *pIn,
    const int                    N
);

/**
* \brief Multiplies all elements of a vector with the same real scale factor
*
* In-place:   yes (both sources and destination may share same memory space)
* \pre        Both source and destination vectors were allocated and filled
*             with values, and all have the same number of elements
* \post        Destination elements equal: pDest[i] = pSrc[i] * factor
* \param[out]  pDest  :  reference to the destination vector
* \param[in]   pSrc   :  reference to the source vector
* \param[in]   factor :  real scale factor
* \param[in]   length :  number of vector elements
*/
void iusScaleComplexFloatVector
(
    iusComplexFloatVector *      pDest,
    const iusComplexFloatVector *pSrc,
    float                        factor,
    int                          length
);

/**
 * \brief Print all elements of a complex vector to stdout
 *
 * in-place    n.a.
 * \pre        The complex vector has been allocated and all elements of the vector have
 *             been given a complex value.
 * \post       The complex vector elements have been printed to stdout.
 * \param[in]  pComplexXYZVector :  The complex vector of type iusComplexXYZ
 * \param[in]  length            :  Number of complex vector elements
 * \return     void
 */
void iusPrintComplexDoubleVector
(
    const iusComplexDoubleVector *const pComplexDoubleVector,
    int                                 length
);

void iusPrintComplexFloatVector
(
    const iusComplexFloatVector *const pComplexFloatVector,
    int                                length
);

#ifdef __cplusplus
}
#endif

#endif // _IUSVECTORC_H
