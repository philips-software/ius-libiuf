//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusVector.h
//                  Basic functions for vector memory and operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file
 * \brief This file contains prototypes for operations on vectors. 
 */
#ifndef _IUSVECTOR_H
#define _IUSVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Allocates a double vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the vector was allocated, elements are zero
 * \param[in]   length :  number of elements in the vector
 * \return      pointer to the vector data
 */
double * iusAllocDoubleVector
(
    int length
);

/**
 * \brief Allocates a float vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the vector was allocated, elements are zero
 * \param[in]   length :  number of elements in the vector
 * \return      pointer to the vector data
 */
float * iusAllocFloatVector
(
    int length
);

/**
 * \brief Allocates an int vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the vector was allocated, elements are zero
 * \param[in]   length :  number of elements in the vector
 * \return      pointer to the vector data
 */
int * iusAllocIntVector
(
    int length
);

/**
 * \brief Allocates a short vector and initializes its elements to zero
 *
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the vector was allocated, elements are zero
 * \param[in]   length :  number of elements in the vector
 * \return      pointer to the vector data
 */
short * iusAllocShortVector
(
    int length
);

/**
 * \brief Allocates an ushort vector and initializes its elements to zero
 * 
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the vector was allocated, elements are zero
 * \param[in]   length :  number of elements in the vector
 * \return      pointer to the vector data
 */
unsigned short * iusAllocUnsignedShortVector
(
    int length
);

/**
 * \brief Allocates an byte vector and initializes its elements to zero
 * 
 * \pre         length is a strictly positive number (no assert implemented)
 * \post        Memory for the vector was allocated, elements are zero
 * \param[in]   length :  number of elements in the vector
 * \return      pointer to the vector data
 */
unsigned char * iusAllocByteVector
(
    int length
);

/**
 * \brief Frees a vector
 *
 * \pre         The vector was allocated
 * \post        Memory for the vector was freed, vector reference is NULL
 * \param[in]   pDoubleVector :  pointer to a vector of type double
 */
void iusFreeDoubleVector
(
    double *pDoubleVector
);

/**
 * \brief Frees a vector
 *
 * \pre         The vector was allocated
 * \post        Memory for the vector was freed, vector reference is NULL
 * \param[in]   pFloatVector :  pointer to a vector of type float
 */
void iusFreeFloatVector
(
    float *pFloatVector
);

/**
 * \brief Frees a vector
 *
 * \pre         The vector was allocated
 * \post        Memory for the vector was freed, vector reference is NULL
 * \param[in]   pIntVector :  pointer to a vector of type int
 */
void iusFreeIntVector
(
    int *pIntVector
);

/**
 * \brief Frees a vector
 *
 * \pre         The vector was allocated
 * \post        Memory for the vector was freed, vector reference is NULL
 * \param[in]   pShortVector :  pointer to a vector of type short
 */
void iusFreeShortVector
(
    short *pShortVector
);

/**
 * \brief Frees a vector
 *
 * \pre         The vector was allocated
 * \post        Memory for the vector was freed, vector reference is NULL
 * \param[in]   pUnsignedShortVector :  pointer to a vector of type ushort
 */
void iusFreeUnsignedShortVector
(
    unsigned short *pUnsignedShortVector
);

/**
 * \brief Frees a vector
 * 
 * \pre         The vector was allocated
 * \post        Memory for the vector was freed, vector reference is NULL
 * \param[in]   pByteVector :  pointer to a vector of type uchar
 */
void iusFreeByteVector
(
    unsigned char *pByteVector
);

/**
 * \brief Sets all elements of a vector to the same value
 * 
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to "value"
 * \param[out]  pDoubleVector :  reference to a vector of type double
 * \param[in]   length        :  number of vector elements
 * \param[in]   value         :  the value
 */
void iusSetValue2DoubleVector
(
    double *pDoubleVector,
    int     length,
    double  value
);

/**
 * \brief Sets all elements of a vector to the same value
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to "value"
 * \param[out]  pFloatVector :  reference to a vector of type float
 * \param[in]   length       :  number of vector elements
 * \param[in]   value        :  the value
 */
void iusSetValue2FloatVector
(
    float *pFloatVector,
    int    length,
    float  value
);

/**
 * \brief Sets all elements of a vector to the same value
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to "value"
 * \param[out]  pIntVector :  reference to a vector of type int
 * \param[in]   length     :  number of vector elements
 * \param[in]   value      :  the value
 */
void iusSetValue2IntVector
(
    int *  pIntVector,
    int    length,
    int    value
);

/**
 * \brief Sets all elements of a vector to the same value
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to "value"
 * \param[out]  pShortVector :  reference to a vector of type short
 * \param[in]   length       :  number of vector elements
 * \param[in]   value        :  the value
 */
void iusSetValue2ShortVector
(
    short *pShortVector,
    int    length,
    short   value
);

/**
 * \brief Sets all elements of a vector to the same value
 * 
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to "value"
 * \param[out]  pByteVector :  reference to a vector of type uchar
 * \param[in]   length      :  number of vector elements
 * \param[in]   value       :  the value
 */
void iusSetValue2ByteVector
(
    unsigned char *pByteVector,
    int            length,
    unsigned char  value
);

/**
 * \brief Sets all elements of a vector to zero
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to zero
 * \param[out]  pDoubleVector :  reference to a vector of type double
 * \param[in]   length        :  number of vector elements
 */
void iusZeroDoubleVector
(
    double *pDoubleVector,
    int     length
);

/**
 * \brief Sets all elements of a vector to zero
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to zero
 * \param[out]  pFloatVector :  reference to a vector of type float
 * \param[in]   length       :  number of vector elements
 */
void iusZeroFloatVector
(
    float *pFloatVector,
    int    length
);

/**
 * \brief Sets all elements of a vector to zero
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to zero
 * \param[out]  pIntVector :  reference to a vector of type int
 * \param[in]   length     :  number of vector elements
 */
void iusZeroIntVector
(
    int *  pIntVector,
    int    length
);

/**
 * \brief Sets all elements of a vector to zero
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to zero
 * \param[out]  pShortVector :  reference to a vector of type short
 * \param[in]   length       :  number of vector elements
 */
void iusZeroShortVector
(
    short *pShortVector,
    int    length
);

/**
 * \brief Sets all elements of a vector to zero
 *
 * \pre         The vector was allocated
 * \post        All elements of the vector are set to zero
 * \param[out]  pByteVector :  reference to a vector of type uchar
 * \param[in]   length      :  number of vector elements
 */
void iusZeroByteVector
(
    unsigned char *pByteVector,
    int            length
);

/**
 * \brief Fills a vector with a sine table at a certain normalized frequency
 *
 * \pre         The vector was allocated
 * \post        pFloatVector[n] = sin( normFreq * n ) for n = 0, 1, ..., length-1
 * \param[out]  pFloatVector :  reference to a vector
 * \param[in]   length       :  length of the vector
 * \param[in]   normFreq     :  normalized frequency (= 2 * PI * freqHz / sampleRate)
 */
void iusSetSineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq
);

/**
 * \brief Fills a vector with a sine table at a certain normalized frequency and
 * with a certain constant phase offset
 *
 * \pre         The vector was allocated
 * \post        pFloatVector[n] = sin( normFreq * n + phase ) for n = 0, 1, ..., length-1
 * \param[out]  pFloatVector :  reference to the output vector
 * \param[in]   length       :  length of the vector
 * \param[in]   normFreq     :  normalized frequency (= 2 * PI * freqHz / sampleRate)
 * \param[in]   phase        :  constant phase offset
 */
void iusSetShiftedSineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq,
    float  phase
);

/**
 * \brief Fills a vector with a cosine table at a certain normalized frequency
 *
 * \pre         The vector was allocated
 * \post        pFloatVector[n] = cos( normFreq * n ) for n = 0, 1, ..., length-1
 * \param[out]   pFloatVector :  reference to the output vector
 * \param[in]   length        :  length of the vector
 * \param[in]   normFreq      :  normalized frequency (= 2 * PI * freqHz / sampleRate)
 */
void iusSetCosineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq
);

/**
* \brief Fills a vector with a cosine table at a certain normalized frequency and
* with a certain constant phase offset
*
* \pre         The vector was allocated
* \post        pFloatVector[n] = cos( normFreq * n + phase ) for n = 0, 1, ..., length-1
* \param[out]  pFloatVector:  reference to the output vector
* \param[in]   length      :  length of the vector
* \param[in]   normFreq    :  normalized frequency (= 2 * PI * freqHz / sampleRate)
* \param[in]   phase       :  constant phase offset
*/
void iusSetShiftedCosineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq,
    float  phase
);

/**
 * \brief Sets a sigmoid function in a float vector
 *
 * \pre         The vector was allocated
 * \post        The elements of the vector follow the sigmoid at specified scale
 * \param[out]  pDest  :  reference to the destinationation vector
 * \param[in]   length :  length of the vector
 * \param[in]   scale  :  scale of the sigmoid, e.g. 2.0f
 */
void iusSetSigmoid2FloatVector
(
    float *pDest,
    int    length,
    float  scale
);

/**
 * \brief Like "polyval" in MatLab, returns an output vector with at each element the polynomial value
 * evaluated at the corresponding element of an input vector pX
 * 
 * Input vector pX, for the i-th vector element let x = pX[i] and y = pDest[i], then
 *   y = pA[n-1] + pA[n-2] * x + pA[n-3] * x^2 + ... pA[0]*x^(n-1)
 *
 * In-place:   yes (pX and pDest may occupy same memory space)
 * \pre         The vectors were allocated, pX and pA contain values
 * \post        Output pDest contains the polynomial values
 * \param[out]  pDest  :  reference to the destimation vector
 * \param[in]   pX     :  reference to the input vector
 * \param[in]   length :  length of input and destination vectors
 * \param[in]   pA     :  vector containing polynomial coefficients
 * \param[in]   n      :  number of polynomial coefficients. Note that the order
 *                        of the polynomial is n-1(!), not n.
 */
void iusPolyValFloatVector
(
    float *      pDest,
    const float *pX,
    int          length,
    const float *pA,
    int          n
);

/**
 * \brief Fills a vector with random values from a Gaussian distribution
 *
 * \pre         The vector was allocated
 * \post        The elements of the vector have random values with a Gaussian distribution
 * \param[out]  pFloatVector :  reference to a vector of type float
 * \param[in]   length       :  length of the vector
 * \param[in]   seed         :  seed value, input for srand()
 */
void iusSetGaussianFloatVector // zero-mean and std=1
(
    float *pFloatVector,
    int    length,
    int    seed
);

/**
 * \brief Fills a vector with random values from a uniform distribution
 *
 * \pre         The vector was allocated
 * \post        The elements of the vector have random values with a uniform distribution
 * \param[out]  pFloatVector :  reference to a vector of type float
 * \param[in]   length       :  length of the vector
 * \param[in]   seed         :  seed value, input for srand()
 */
void iusSetRandomFloatVector
(
    float *pFloatVector,
    int    length,
    int    seed
);

/**
 * \brief Fills a vector with random values
 *
 * \pre         The vector was allocated
 * \post        The elements of the vector have random values based on rand() from stdlib.
 * \param[out]  pIntVector :  reference to a vector of type int
 * \param[in]   length     :  length of the vector
 * \param[in]   seed       :  seed value, input for srand()
 */
void iusSetRandomIntVector
(
    int *  pIntVector,
    int    length,
    int    seed
);

/**
 * \brief Fills a vector with random values
 *
 * \pre         The vector was allocated
 * \post        The elements of the vector have random values based on rand() from stdlib.
 * \param[out]  pShortVector :  reference to a vector of type short
 * \param[in]   length       :  length of the vector
 * \param[in]   seed         :  seed value, input for srand()
 */
void iusSetRandomShortVector
(
    short *pShortVector,
    int    length,
    int    seed
);

/**
 * \brief Fills a vector with random values
 *
 * \pre         The vector was allocated
 * \post        The elements of the vector have random values bein either 0 or 1.
 * \param[out]  pShortVector :  reference to a vector of type short
 * \param[in]   length       :  length of the vector
 * \param[in]   seed         :  seed value, input for srand()
 */
void iusSetRandomBinaryShortVector
(
    short *pShortVector,
    int    length,
    int    seed
);

/**
 * \brief Fill a vector with equally spaced values between a start value and an end value.
 *
 * \pre         The vector was allocated
 * \post        The first vector element is set to startValue, and subsequent
 *              elements of the vector have incremented values where the
 *              positive or non-positive increment is given by
 *              (endValue-startValue) / (length-1).
 * \param[out]  pFloatVector  : reference to a float vector
 * \param[in]   length        : length of the vector
 * \param[in]   startValue    : first value
 * \param[in]   endValue      : last value
 */
void iusSetLinSpaceFloatVector
(
    float *const pFloatVector,
    int          length,
    float        startValue,
    float        endValue
);

/**
 * \brief Fill a vector with an N-points Hamming window. Equivalent to
 * MatLab's hamming(N);
 *
 * \pre         The vector was allocated
 * \post        The vector contains the N-points Hamming window
 * \param[out]  pFloatVector  : reference to a float vector
 * \param[in]   N             : length of the vector
 */
void iusSetHammingFloatVector
(
    float *const pFloatVector,
    int          N
);

/**
 * \brief Fill a vector with an N-points Hamming/Hanning window. Equivalent to
 * MatLab's hanning(N);
 *
 * \pre         The vector was allocated
 * \post        The vector contains the N-points Hanning window
 * \param[out]  pFloatVector  : reference to a float vector
 * \param[in]   N             : length of the vector
 */
void iusSetHanningFloatVector
(
    float *const pFloatVector,
    int          N
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyIntVector
(
    int *        pDest,
    const int *  pSrc,
    int          length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyShortVector
(
    short *      pDest,
    const short *pSrc,
    int          length
);

/**
* \brief Copies all data in a vector to another vector with the same dimensions
*
* In-place:    no (source and destination must be different)
* \pre         Both source and destination vectors were initialized and have
*              the same number of elements
* \post        All elements of the source vector are copied to the destination
*              (with type casting if applicable)
* \param[out]  pDest  :  reference to the destination vector
* \param[in]   pSrc   :  reference to the source vector
* \param[in]   length :  number of vector elements
*/
void iusCopyShortVector2IntVector
(
    int *         pDest,
    const short * pSrc,
    int           length
);

/**
* \brief Copies all data in a vector to another vector with the same dimensions
*
* In-place:    no (source and destination must be different)
* \pre         Both source and destination vectors were initialized and have
*              the same number of elements
* \post        All elements of the source vector are copied to the destination
*              (with type casting if applicable)
* \param[out]  pDest  :  reference to the destination vector
* \param[in]   pSrc   :  reference to the source vector
* \param[in]   length :  number of vector elements
*/
void iusCopyShortVector2FloatVector
(
    float *      pDest,
    const short *pSrc,
    int          length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyByteVector2DoubleVector
(
    double *             pDest,
    const unsigned char *pSrc,
    int                  length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyShortVector2DoubleVector
(
    double *     pDest,
    const short *pSrc,
    int          length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyDoubleVector2FloatVector
(
    float *       pDest,
    const double *pSrc,
    int           length
);

/**
* \brief Copies all data in a vector to another vector with the same dimensions
*
* In-place:    no (source and destination must be different)
* \pre         Both source and destination vectors were initialized and have
*              the same number of elements
* \post        All elements of the source vector are copied to the destination
*              (with type casting if applicable)
* \param[out]  pDest  :  reference to the destination vector
* \param[in]   pSrc   :  reference to the source vector
* \param[in]   length :  number of vector elements
*/
void iusCopyDoubleVector2IntVector
(
    int *         pDest,
    const double *pSrc,
    int           length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 * the same number of elements
 * \post        All elements of the source vector are copied to the destination
 * (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyDoubleVector2ShortVector
(
	short *        pDest,
	const double * pSrc,
	int            length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyDoubleVector2ByteVector
(
    unsigned char *pDest,
    const double * pSrc,
    int            length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyFloatVector2DoubleVector
(
    double *     pDest,
    const float *pSrc,
    int          length
);

/**
 * \brief Copies all data in a vector to another vector with the same dimensions
 *
 * In-place:    no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *              the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *              (with type casting if applicable)
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 */
void iusCopyFloatVector2ShortVector
(
    short *      pDest,
    const float *pSrc,
    int          length
);

/**
 * \brief Flips all data in a vector to another vector with the same dimensions
 *
 * In-place:   no (source and destination must be different)
 * \pre         Both source and destination vectors were initialized and have
 *             the same number of elements
 * \post        All elements of the source vector are copied to the destination
 *             in reversed order.
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   length :  number of vector elements
 * return      void
 */
void iusFlipFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length
);

/**
 * \brief Element-wise multiplication of two float vectors
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre         Both source and destination vectors were allocated and filled
 *             with values, and all have the same number of elements
 * \post        Destination elements equal:
 *                 multiplication:  pDest[i] = pSrc1[i] * pSrc2[i]
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc1  :  reference to the 1st source vector
 * \param[in]   pSrc2  :  reference to the 2nd source vector
 * \param[in]   length :  number of vector elements
 */
void iusMultiplyFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    int          length
);

/**
 * \brief Element-wise addition of two vectors
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre         Both source and destination vectors were allocated and filled
 *             with values, and all have the same number of elements
 * \post        Destination elements equal:
 *                 addition      :  pDest[i] = pSrc1[i] + pSrc2[i]
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc1  :  reference to the 1st source vector
 * \param[in]   pSrc2  :  reference to the 2nd source vector
 * \param[in]   length :  number of vector elements
 */
void iusAddFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    int          length
);

/**
 * \brief Element-wise accumulation of two vectors
 *
 * In-place:    pDest values are increased with pSrc values 
 * \pre         Both source and destination vectors were allocated and filled
 *              with values, and all have the same number of elements
 * \post        Destination elements equal:
 *                 pDest[i] += pSrc[i]
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the 1st source vector
 * \param[in]   length :  number of vector elements
 */
void iusAccFloatVectors
(
    float *      pDest,
    const float *pSrc,
    int          length
);

/**
 * \brief Element-wise subtraction of two vectors
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre         Both source and destination vectors were allocated and filled
 *             with values, and all have the same number of elements
 * \post        Destination elements equal:
 *                 addition      :  pDest[i] = pSrc1[i] - pSrc2[i]
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc1  :  reference to the 1st source vector
 * \param[in]   pSrc2  :  reference to the 2nd source vector
 * \param[in]   length :  number of vector elements
 */
void iusSubtractFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    int          length
);

/**
 * \brief Inner product of two vectors
 *
 * \pre         Both source vectors were allocated and filled with values, and
 *              have the same number of elements
 * \post        inner product computed
 * \param[out]  pInnerProductValue:  inner product
 * \param[in]   pSrc1  :  reference to the 1st source vector
 * \param[in]   pSrc2  :  reference to the 2nd source vector
 * \param[in]   length :  number of vector elements
 */
void iusInnerProductFloatVectors
(
    float *      pInnerProductValue,
    const float *pSrc1,
    const float *pSrc2,
    int          length
);

/**
 * \brief Multiplies all elements of a vector with the same scale factor
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre        Both source and destination vectors were allocated and filled
 *             with values, and all have the same number of elements
 * \post        Destination elements equal: pDest[i] = pSrc[i] * factor
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   factor :  scale factor
 * \param[in]   length :  number of vector elements
 */
void iusScaleFloatVector
(
    float *      pDest,
    const float *pSrc,
    float        factor,
    int          length
);

/**
 * \brief Dividies all elements of a vector by the same factor
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre         Both source and destination vectors were allocated and filled
 *              with values, and all have the same number of elements. The factor
 *              must be non-zero.
 * \post        Destination elements equal: pDest[i] = pSrc[i] / factor
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc   :  reference to the source vector
 * \param[in]   factor :  scale factor
 * \param[in]   length :  number of vector elements
 */
void iusDivideFloatVector
(
    float *      pDest,
    const float *pSrc,
    float        factor,
    int          length
);

/**
 * \brief Fades between two input vectors
 *
 * In-place:   yes (both sources and destination may share same memory space)
 * \pre         Both source and destination vectors were allocated and filled
 *              with values, and all have the same number of elements.
 * \post        Destination elements equal:
 *              pDest[i] = factor * pSrc1[i] + (1.0f - factor) * pSrc2[i];
 * \param[out]  pDest  :  reference to the destination vector
 * \param[in]   pSrc1  :  reference to the first source vector
 * \param[in]   pSrc2  :  reference to the second source vector
 * \param[in]   factor :  scale factor
 * \param[in]   length :  number of vector elements
 */
void iusFadeFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    float        factor,
    int          length
);

/**
 * \brief Computes the mean value of the vector elements
 *
 * \pre         Input vector was allocated and filled with values.
 * \post        The mean value was computed
 *
 *  \param[in]   pFloatVector : reference to the input vector
 * \param[in]   length        : number of vector elements
 * \return      the mean value
 */
float iusComputeFloatVectorMean
(
    const float *pFloatVector,
    int          length
);

/**
 * \brief Computes the variance of the vector elements
 *
 * \pre         Input vector was allocated and filled with values.
 * \post        The variance was computed
 *
 * \param[in]   pFloatVector : reference to the input vector
 * \param[in]   length       : number of vector elements
 * \return      the variance value
 */
float iusComputeFloatVectorVariance
(
    const float *pFloatVector,
    int          length
);

#if 0
//-----------------------------------------------------------------------------
// Print all data in a vector to a stream (like stdout or a file)
//
// In-place:   n.a.
// pre         The vector was allocated and its elements have values
// post        All elements of the vector were printed to the stream
// param[in]   fp:  reference to the stream (like stdout, or a file pointer)
// param[in]   pXYZVector :  the vector of type XYZ to be printed
// param[in]   length:  number of vector elements
// return      void
//-----------------------------------------------------------------------------
void iusFPrintFFloatVector
(
    FILE *       fp,
    const float *pFloatVector,
    int          length
);

void iusFPrintFIntVector
(
    FILE *        fp,
    const int *  pIntVector,
    int           length
);

void iusFPrintFShortVector
(
    FILE *       fp,
    const short *pShortVector,
    int          length
);

#endif

/**
 *  \brief Takes the absolute value for each input vector element.
 *
 *  In-place:    yes (pSrc and pDest may be equal)
 *  \pre         The source and destination float vectors have been allocated,
 *               have equal numbers of elements, and all elements of the source
 *               vector were assigned values.
 *  \post        the result of the ABS( . ) operation is stored in the output.
 *  \param[out]  pDest      :  Output float vector
 *  \param[in]   pSrc       :  Input float vector
 *  \param[in]   length     :  number of vector elements
 *  \return      void
 */
void iusAbsFloatVector
(
	float *       pDest,
	const float * pSrc,
	int           length
);

void iusAbsShortVector
(
	short *       pDest,
	const short * pSrc,
	int           length
);

/**
 * \brief Takes the absolute values after applying a [1 -1] filter operation.
 *
 * in-place:    no (source and destination may NOT occupy the same memory space)
 * \pre         The source and destination float vectors have been allocated,
 *              have equal numbers of elements, and all elements of the source
 *              vector were assigned values.
 * \post        the result of the ABS( diff ) filter operation is stored in the
 *              destination
 * \param[out]  pDest      :  Output float vector
 * \param[in]   pSrc       :  Input float vector
 * \param[in]   length     :  number of vector elements
 * \return      void
 */
void iusAbsDiffFloatVector
(
    float *       pDest,
    const float * pSrc,
    int           length
);

/**
 * \brief Median filter operation on a float vector.
 *
 * In-place:    yes (source and destination may occupy the same memory space)
 * \pre         The source and destination float vectors have been allocated,
 *              have equal numbers of elements, and all elements of the source
 *              vector were assigned values.
 *              windowWidth must be odd-valued.
 * \post        the result of the median filter operation is stored in the
 *              destination 
 * \param[out]  pDest       :  Output float vector
 * \param[in]   pSrc        :  Input float vector
 * \param[in]   length      :  number of vector elements
 * \param[in]   windowWidth :  span of the median operation
 * \return      void
 */
void iusMedianFilterFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    int          windowWidth
);

/**
 * \brief Clips the elements of a vector to a lower and an upper bound
 *
 * in-place    yes (source and destination may occupy the same memory space)
 * \pre        Vectors have been allocated, elements of pSrc have values,
 *             and maxVal <= minVal.
 * \post       The result of the clip operation is stored in pDest.
 * \param[out] pDest  :  Output vector
 * \param[in]  pSrc   :  Input vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \param[in]  minVal :  minimum clipping value
 * \param[in]  maxVal :  maximum clipping value
 * \return     void
 */
void iusClipLoHiFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    float        minVal,
    float        maxVal
);

/**
 * \brief Clips the elements of a vector to a lower and an upper bound, then cast to short
 *
 * in-place   no
 * \pre        Vectors have been allocated, elements of pSrc have values,
 *            and maxVal > minVal.
 * \post       The result of the clip & quantize operation is stored in pDest.
 * \param[out] pDest  :  Output vector
 * \param[in]  pSrc   :  Input vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \param[in]  minVal :  minimum clipping value
 * \param[in]  maxVal :  maximum clipping value
 * \return     void
 */
void iusQuantizeFloatVector2ShortVector
(
    short *      pDest,
    const float *pSrc,
    int          length,
    float        minVal,
    float        maxVal
);

/**
 * \brief Finds the maximum / minimum value of all vector elements
 *
 * in-place    n.a.
 * \pre        Vector has been allocated and has values.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc   :  Input vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \return     maximum / minimum value of a vector
 */
float iusMaxFloatVector
(
    const float *pSrc,
    int          length
);

int iusMaxIntVector
(
    const int *pSrc,
    int        length
);

short iusMaxShortVector
(
	const short *pSrc,
	int           length
);

float iusMinFloatVector
(
	const float *pSrc,
	int         length
);

int iusMinIntVector
(
	const int *pSrc,
	int        length
);

short iusMinShortVector
(
	const short *pSrc,
	int          length
);


//------------------------------------------------------------------------------
// Element-by-element maximum value of two input vectors
//------------------------------------------------------------------------------
/**
 * \brief Element-by-element maximum value of two input vectors
 *
 * in-place    yes
 * \pre        Input arrays has been allocated and filled with values.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc1   :  Input vector
 * \param[in]  pSrc2   :  Input vector
 * \param[in]  length  :  number of elements of pDest, pSrc, and pSrc2
 * \param[out] pDest   :  pDest[i] = max( pSrc1[i], pSrc2[i] )
 * \return     void
 */
void iusElementByElementMaxFloatVectors
(
    float *const       pDest,
    const float *const pSrc1,
    const float *const pSrc2,
    int                length
);


/**
 * \brief Finds the maximum / minimum value of all vector elements plus index
 *
 * in-place    n.a.
 * \pre        Vector has been allocated and has values.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc    :  Input vector
 * \param[in]  length  :  number of vector elements of pSrc
 * \param[out] *pIndex :  the found index for the maximum/minimum
 * \return     maximum / minimum value of a vector
 */
float iusMaxIndexFloatVector
(
    const float *pSrc,
    const int    length,
    int *        pIndex
);

float iusMinIndexFloatVector
(
    const float *pSrc,
    const int    length,
    int *        pIndex
);

/**
 * \brief Counts the number of unique values of (int) vector elements
 *
 * in-place    n.a.
 * \pre        Vector has been allocated and has values.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc   :  Input vector
 * \param[in]  length :  number of vector elements of pSrc
 * \return     the number of unique values in the  vector
 */
int iusNumUniqueValuesIntVector
(
	const int *pSrc,
	const int  length
);

/**
 * \brief Finds the first index counting upwards from 0 for which holds that the
 * corresponding vector element exceeds a given threshold value
 *
 * in-place    n.a.
 * \pre        Vector has been allocated and has values.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc    :  Input vector
 * \param[in]  length  :  number of vector elements of pDest and pSrc
 * \param[out] *pIndex :  the found index for the maximum/minimum
 * \return     the value of the *pIndex-th vector element
 */
float iusFirstIndexAboveThresholdFloatVector
(
    const float *pSrc,
    const int    length,
    const float  threshold,
    int *        pIndex
);

int iusFirstPeakAboveThresholdFloatVector
(
    const float *pSrc,
    const int    length,
    const float  threshold,
    int *        pIndex,
    float *      pPeakValue
);

/**
 * \brief Sum the values of the elements of a vector
 *
 * in-place    n.a.
 * \pre        Vector has been allocated and has values, length > 0.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc   :  Input vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \return     sum of values
 */
float iusSumElementsFloatVector
(
    const float *pSrc,
    int          length
);

/**
 * \brief Sum the absolute values of the elements of a vector
 *
 * in-place    n.a.
 * \pre        Vector has been allocated and has values, length > 0.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc   :  Input vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \return     sum of absolute values
 */
float iusSumAbsElementsFloatVector
(
    const float *pSrc,
    int          length
);

/**
 * \brief Sum the squared values of the elements of a vector
 *
 * in-place    n.a.
 * \pre        Vector has been allocated and has values, length > 0.
 * \post       The result of the operation is returned.
 * \param[in]  pSrc   :  Input vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \return     sum of squared values
 */
float iusSumSquaredElementsFloatVector
(
    const float *pSrc,
    int          length
);

/**
 * \brief From an input vector pass on the values belonging to the positive peaks,
 * characterized by pSrc[i] being larger than a threshold value and being larger
 * than its two neighbors pSrc[i-1] and pSrc[i+1].
 *
 * in-place    no (pDest may not be equal to pSrc)
 * \pre        Input vector has been allocated and has values.
 * \post       The result of the operation is returned.
 * \param[out] pDest     :  Output result vector
 * \param[in]  pSrc      :  Input vector
 * \param[in]  length    : length of input and output vectors
 * \param[in]  threshold : absolute threshold value
 * \return     void
 */
void iusPassPeaksFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    float        threshold
);

/**
 * \brief Sorts the elements of a vector segment between two indices.
 *
 * in-place         yes (only in-place)
 * \pre             The pIO vector has been allocated and initialized, and the
 *                  first and second indices indicate an element within length.
 * \post            The elements of the segment starting from firstIndex to
 *                  lastIndex have been sorted. 
 * \param[in/out]   pIO        :     input/output vector
 * \param[in]       length     :     number of input/output vector elements
 * \param[in]       firstIndex :     first index for input vector segment
 * \param[in]       lastIndex  :     last index for input vector segment
 * \return          void
 */
void iusQuickSortFloatVectorSegment
(
    float *pIO,
    int    length,
    int    firstIndex,
    int    lastIndex
);

/**
 * \brief Direct form II transposed zero phase FIR filter
 * (based from Ofi's basic_fcs.c, rev. 3002)
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        The source and destination float vectors have been allocated,
 *            have equal numbers of elements, and all elements of the source
 *            vector were assigned values.
 * \post       The result of the filter operation is stored in pDest.
 * \param[out] pDest  :  Output float vector
 * \param[in]  pSrc   :  Input float vector
 * \param[in]  length :  Number of vector elements
 * \param[in]  pCoefs :  FIR filter coefficients vector (must be symmetrical!)
 * \param[in]  nTaps  :   Number of filter coefficients (must be odd!)
 * \return     void
 */
void iusZeroPhaseFirFilterFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    const float *pCoefs,
    int          nTaps
);

/**
 * /brief Basic direct form II transposed zero phase FIR filter with output decimation
 * (based from Ofi's basic_fcs.c, rev. 3002)
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * /pre        The source and destination float vectors have been allocated,
 *            and all elements of the source vector were assigned values.
 *            For the length of the destination vector it must hold that
 *            lengthDest == (lengthSrc + decimationStep - 1) / decimationStep.
 * /post       The result of the filter operation is stored in pDest.
 * /param[out] pDest          :  Output float vector
 * /param[in]  lengthDest     :  number of vector elements of pDest
 * /param[in]  pSrc           :  Input float vector
 * /param[in]  lengthDest     :  number of vector elements of pSrc
 * /param[in]  pCoefs         :  FIR filter coefficients vector (must be symmetrical!)
 * /param[in]  nTaps          :  number of FIR filter coeffcients (must be odd!)
 * /param[in]  decimationStep :  step-size skipping output samples for decimation
 * /return     void
 */
void iusZeroPhaseFirFilterAndDecimateFloatVector
(
    float *       pDest,
    int           lengthDest,
    const float * pSrc,
    int           lengthSrc,
    const float * pCoefs,  // must be symmetrical with odd number of elements!
    int           nTaps,
    int           decimationStep
);

/**
 * \brief Half Band FIR filter
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        The source and destination float vectors have been allocated,
 *             have equal numbers of elements, and all elements of the source
 *             vector were assigned values.
 * \post       The result of the filter operation is stored in pDest.
 * \param[out] pDest  :  Output float vector
 * \param[in]  pSrc   :  Input float vector
 * \param[in]  length :  Number of vector elements
 * \param[in]  pCoefs :  filter coefficients; symmetrical and in reversed order.
 * \param[in]  nTaps  :   Number of filter coefficients (must be odd!)
 * \return     void
 */
void iusHalfBandFirFilterFloatVector
(
    float *       pDest,
    const float * pSrc,
    const int     length,
    const float * pCoefs,
    const int     nTaps
);

/**
 * \brief Like MatLab's stdfilt().
 *
 * in-place    yes (pDest may be equal to pSrc, but pDest may not be equal to
 *             pTmp1 nor to pTmp2)
 * \pre        The source and destination float vectors have been allocated,
 *             and all elements of the source vector were assigned values.
 * \post       The result of the operation is stored in pDest.
 * \param[out] pDest   :  Output float vector
 * \param[out] pTmp1   :  scratch float vector
 * \param[out] pTmp2   :  scratch float vector
 * \param[in]  pSrc    :  Input float vector
 * \param[in]  length  :  number of elements in all input/output vectors
 *                        must satisfy: length > (stdSize - 1) / 2
 * \param[in]  stdSize :  kernel size for the std operation, must be odd and > 2
 * \return     void
 */
void iusStdFilterFloatVector
(
    float *      pDest,
    float *      pTmp1,
    float *      pTmp2,
    const float *pSrc,
    int          length,
    int          stdSize
);

/**
 * \brief zero phase filtering by a [-1 -1 ... -1 0 1 ... 1 1] filter having odd 
 * length "kernelSize".
 *
 * in-place    yes (pDest may be equal to pSrc, but pDest may not be equal to
 *             pTmp)
 * \pre        The source and destination float vectors have been allocated,
 *             and all elements of the source vector were assigned values.
 * \post       The result of the operation is stored in pDest.
 * \param[out] pDest      :  Output float vector
 * \param[out] pTmp       :  scratch float vector
 * \param[in]  pSrc       :  Input float vector
 * \param[in]  length     :  number of elements in all input/output vectors
 *                           must satisfy: length > (stdSize - 1) / 2
 * \param[in]  kernelSize :  kernel size for the operation, must be odd and > 2
 * \return     void
 */
void iusEdgeFilterFloatVector
(
    float *      pDest,
    float *      pTmp,
    const float *pSrc,
    int          length,
    int          kernelSize
);

/**
 * \brief Output vector is cumulative sum of input vector (MatLab's cumsum)
 *
 * in-place   yes (pDest may be equal to pSrc)
 * pre        The source and destination float vectors have been allocated,
 *            and all elements of the source vector were assigned values.
 * post       The result of the operation is stored in pDest.
 * param[out] pDestCumSum :  Output float vector
 * param[in]  pSrc        :  Input float vector
 * param[in]  length      :  number of elements in input/output vectors
 * return     void
 */
void iusCumSumFloatVector
(
    float *      pDestCumSum,
    const float *pSrc,
    int          length
);

/**
 * /brief Cumulative sum and cumulative sum of squares
 *
 * in-place    yes (pDestCumSumSquared may be equal to pSrc, but pDestCumSum
 *             may not be equal to pSrc!!)
 * /pre        The source and destination float vectors have been allocated,
 *             and all elements of the source vector were assigned values.
 * /post       The result of the operation is stored in pDest.
 * /param[out] pDestCumSum        :  Output float vector
 * /param[out] pDestCumSumSquared :  Output float vector
 * /param[in]  pSrc               :  Input float vector
 * /param[in]  length             :  number of elements in input/output vectors
 * /return     void
 */
void iusCumSumAndSquaredFloatVector
(
    float *      pDestCumSum,
    float *      pDestCumSumSquared,
    const float *pSrc,
    int          length
);

/**
 * \brief Smooth a float vector with a rectangular sliding window
 * TODO: speed optimise the implementation
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        Vectors have been allocated, are of equal length, and source
 *             vector elements have values. The span is odd-valued.
 * \post       The result of the smoothing operation is stored in pDest.
 * \param[out] pDest  :  Output float vector
 * \param[out] pSrc   :  Input float vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \param[in]  span   :  length in samples for the rectangular sliding window
 * \return     void
 */
void iusRectangularSmoothFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    int          span
);

/**
 * \brief Interpolates linearly between points (x,y), where x is an integer index and y
 * is a float value, to a float vector of length nPoints.
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        Vectors have been allocated, elements of pX, pY have values.
 *             The pX index values must satisfy:
 *                 0 <= pX[0] < pX[1] < ... < pX[nPoints-1] < length.
 * \post       The result of the interpolation is stored in pDest.
 * \param[out] pDest   :  Output float vector
 * \param[in]  length  :  number of vector elements of pDest
 * \param[in]  pX      :  Array of data point x-indices
 * \param[in]  pY      :  Array of data point y-values
 * \param[in]  nPoints :  number of (x,y) points
 * \return     void
 */
void iusLinearInterpolatePoints2FloatVector
(
    float *      pDest,
    int          length,
    const int *  pX,
    const float *pY,
    int          nPoints
);

/**
 * \brief Fetches an interpolated value from a vector at a non-integer index location.
 *
 * in-place    n.a.
 * \pre        Input vector has been allocated and elements have values.
 *             (l1 <= (float)n1 - 1)) && (l1 >= 0.0f)
 * \post       The result of the interpolation is returned
 * \param[in]  pIn :  input vector
 * \param[in]  n1  :  number of vector elements
 * \param[in]  l1  :  non-integer index, range [0, n1-2]
 * \return     void
 */
float iusFetchLinearFloatVector
(
    const float *const pIn,
    int                n1,
    float              l1
);

/**
 * \brief Interpolates a source vector to a destination vector.
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        The source and destination vectors have been allocated,
 *             and all elements of the source vector were assigned values.
 * \post       The result of the upsampling is stored in pDest.
 * \param[out] rr   :  Output vector, of length N*L
 * \param[in]  r    :  Input vector
 * \param[in]  N    :  number of vector elements of r
 * \param[in]  L    :  Integer interpolation factor, 2 or 4 or 8.
 * \return     void
 */
void iusInterpolateFloatVector
(
          float *rr,
    const float *r,
    const int    N,
    const int    L
);

/**
 * \brief Resamples a source vector to a destination vector. Upsampling by a 
 * zero-order hold, downsampling by skipping input values. A copy when source 
 * and destination have equal length.
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        The source and destination vectors have been allocated,
 *             and all elements of the source vector were assigned values.
 * \post       The result of the resampling is stored in pDest.
 * \param[out] pDest         :  Output vector
 * \param[in]  lengthDest    :  number of vector elements of pDest
 * \param[in]  pSrc          :  Input vector
 * \param[in]  lengthDest    :  number of vector elements of pSrc
 * \return     void
 */
void iusReSampleFloatVector
(
    float *      pDest,
    int          lengthDest,
    const float *pSrc,
    int          lengthSrc
);

void iusReSampleIntVector
(
    int *        pDest,
    int          lengthDest,
    const int *  pSrc,
    int          lengthSrc
);

void iusReSampleShortVector
(
    short *      pDest,
    int          lengthDest,
    const short *pSrc,
    int          lengthSrc
);

/**
 * \brief Resamples a source vector to a destination vector. Usampling by a linear
 * interpolation.
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        The source and destination vectors have been allocated,
 *             and all elements of the source vector were assigned values.
 * \post       The result of the inpolated resampling is stored in pDest.
 * \param[out] pDest         :  Output vector
 * \param[in]  lengthDest    :  number of vector elements of pDest
 * \param[in]  pSrc          :  Input vector
 * \param[in]  lengthDest    :  number of vector elements of pSrc
 * \return     void
 */
void iusReSampleLinearInterpFloatVector
(
    float *      pDest,
    int          lengthDest,
    const float *pSrc,
    int          lengthSrc
);

/**
 * \brief Resamples a binary source vector to a binary destination vector. 
 * Upsampling by a zero-order hold. Downsampling by skipping input values 
 * while preventing a 0->1 transition to vanish.
 *
 * in-place    no (source and destination may NOT be the same memory space)
 * \pre        The source and destination vectors have been allocated, and all
 *             elements of the source vector were assigned binary values 0 or 1.
 * \post       The result of the resampling is stored in pDest.
 * \param[out] pDest         :  Output binary-valued vector
 * \param[in]  lengthDest    :  number of vector elements of pDest
 * \param[in]  pSrc          :  Input binary-valued vector
 * \param[in]  lengthDest    :  number of vector elements of pSrc
 * \return     void
 */
void iusReSampleBinaryShortVector
(
    short *      pDest,
    int          lengthDest,
    const short *pSrc,
    int          lengthSrc
);

/**
 * \brief Counts the number of unique elements in an unsorted array. 
 *
 * in-place    n.a.
 * \pre        The source array has been allocated, and all its elements have
 *             been assigned integer values.
 * \post       The number of unique elements is returned.
 * \param[in]  pSrc   :  input array
 * \param[in]  length :  number of input array elements
 * \return     number of unique elements in the input array
 */
int iusCountUniqueElementsIntVector
(
    const int *const pSrc,
    int              length
);

/**
 * \brief Halves the length of pulses in a binary vector (a pulse is a sequence
 * of non-zero values). When the pulse length N is odd, the resulting pulse has
 * length (N+1)/2.
 *
 * in-place    yes (source and destination may occupy the same memory space)
 * \pre        The source and destination vectors have been allocated, and all
 *             elements of the source vector have values 0 or 1.
 * \post       The result of the operation is stored in pDest.
 * \param[out] pDest  :  Output binary-valued vector
 * \param[in]  pSrc   :  Input binary-valued vector
 * \param[in]  length :  number of vector elements of pDest and pSrc
 * \return     void
 */
void iusHalvePulsesBinaryShortVector
(
    short *      pDest,
    const short *pSrc,
    int          length
);

/**
 * \brief A binary vector contains 0 and 1 only. A positive pulse is a sequence
 * of n values of 1. The result of differentiation of the positive pulse is a
 * pulse of the same length of which the first half is 1 and the second half
 * is -1. When the pulse length N is odd, the output pulse has length (N+1)/2.
 *
 * in-place   yes (source and destination may occupy the same memory space)
 * pre        The source and destination vectors have been allocated, and all
 *            elements of the source vector have values 0 or 1.
 * post       The result of the operation is stored in pDest having values 0
 *            or -1 or +1.
 * param[out] pDest  :  Output binary-valued vector
 * param[in]  pSrc   :  Input binary-valued vector
 * param[in]  length :  number of vector elements of pDest and pSrc
 * return     void
 */
void iusDifferentiatePulsesBinaryShortVector
(
    short *      pDest,
    const short *pSrc,
    int          length
);

/**
 * \brief Steepens edges in a vector at places where a pulse goes from 0 to 1 to
 * -1 to 0. First is run through elements of a source vector and a binary mask
 * element vector starting from index 0. Where the mask is "1" the destination 
 * vector is copied from the previous destination element, otherwise from the 
 * source. Next a reverse run is done starting at the end index. Where the mask 
 * is "-1" the destination vector element is overwritten with the value of the
 * destination element from the previous iteration.
 *
 * in-place    yes (source and destination may occupy the same memory space)
 * \pre        The source, destination and mask vectors have been allocated, and
 *             elements of the source and binary mask vectors have values.
 * \post       The result of the edge steepen operation is stored in pDest.
 * \param[out] pDest  :  Output vector
 * \param[in]  pSrc   :  Input vector
 * \param[in]  pMask  :  Binary mask vector
 * \param[in]  length :  number of elements of all vectors
 * \return     void
 */
void iusEdgeSteepenOnMaskFloatVector
(
    float *      pDest,
    const float *pSrc,
    const short *pMask,
    int          length
);

/**
 * \brief Print all elements of a vector to stdout
 *
 * in-place    n.a.
 * \pre        The vector has been allocated and all elements of the vector have
 *             been given a value.
 * \post       The vector elements have been printed to stdout.
 * \param[in]  pVector    :  The vector
 * \param[in]  n1         :  Number of vector elements
 * \return     void
 */
void iusPrintFloatVector
(
    const float *const pVector,
    int                n1
);

void iusPrintDoubleVector
(
	const double *const pVector,
	int                 n1
);

void iusPrint2FileShortVector
(
    const char *       pFileName,
    const short *const pIn,
    int                length
);

void iusPrint2FileFloatVector
(
    const char *       pFileName,
    const float *const pIn,
    int                length
);

#ifdef __cplusplus
}
#endif

#endif // _IUSVECTOR_H
