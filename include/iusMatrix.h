//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMatrix.h
//                  Basic functions for matrix memory and operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/**
\file \brief This file contains prototypes for the Matrix operations

Functions for memory iusAllocation and iusFreeing:
\li iusAllocDoubleMatrix, iusAllocFloatMatrix, iusAllocIntMatrix, iusAllocShortMatrix, iusAllocByteMatrix
\li iusAllocVelocityFloatMatrix
\li iusFreeDoubleMatrix , iusFreeFloatMatrix , iusFreeIntMatrix , iusFreeShortMatrix , iusFreeByteMatrix
\li iusFreeVelocityFloatMatrix

Functions for assigning values
\li  iusSetValue2FloatMatrix, iusZeroFloatMatrix, iusZeroFloatMatrix3D
\li  iusSetGaussianFloatMatrix
\li     iusSetRandomFloatMatrix, iusSetRandomIntMatrix, iusSetRandomShortMatrix

Functions for copying
\li     iusCopyFloatMatrix
\li     iusCopyShortMatrix2FloatMatrix, iusCopyFloatMatrix2ShortMatrix
\li     iusCopyVectorAlongFloatMatrixDim1, iusCopyVectorAlongFloatMatrixDim2

Operations on vectors and matrices
\li     iusAddFloatMatrices
\li     iusMultiplyFloatMatrices
\li     iusScaleFloatMatrix
\li     iusComputeFloatMatrixMean
\li     iusMaxFloatMatrix    , iusMinFloatMatrix
\li     iusMaxIndicesFloatMatrix, iusMinIndicesFloatMatrix
\li     iusMaxVectorAlongFloatMatrixDim1, iusMaxVectorAlongFloatMatrixDim2
\li     iusSumFloatMatrixAlongDim1, iusSumFloatMatrixAlongDim2
\li     iusSumSquaredFloatMatrixAlongDim1, iusSumSquaredFloatMatrixAlongDim2

and
     iusFetchBilinearFloatMatrix
     RotateColumnsFloatMatrix
 Printing to stdout
     PrintFloatMatrix
*/
#ifndef _IUSMATRIX_H
#define _IUSMATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusTypes.h"

//=============================================================================
// M A C R O S
//=============================================================================

//=============================================================================
// D E F I N E S
//=============================================================================
#define iusMatrixRow(matrix,r)  ((matrix)[r])

//=============================================================================
// D A T A   T Y P E S
//=============================================================================

//=============================================================================
// F U N C T I O N   P R O T O T Y P E S
//=============================================================================

/**
\brief Allocates a matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated, elements are zero
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
double ** iusAllocDoubleMatrix
(
    const int n1,
    const int n2
);

/**
\brief Allocates a matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated, elements are zero
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
float ** iusAllocFloatMatrix
(
    const int n1,
    const int n2
);

/**
\brief Allocates a matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated, elements are zero
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
int ** iusAllocIntMatrix
(
    const int n1,
    const int n2
);

/**
\brief Allocates a matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated, elements are zero
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
short ** iusAllocShortMatrix
(
    const int n1,
    const int n2
);

/**
\brief Allocates a matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated, elements are zero
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
unsigned char ** iusAllocByteMatrix
(
    const int n1,
    const int n2
);

/**
\brief Allocates a matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated, elements are zero
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
unsigned char *** iusAllocByteMatrix3D
(
	const int n1,
	const int n2,
	const int n3
);

/**
\brief Allocates a matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated, elements are zero
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
float *** iusAllocFloatMatrix3D
(
    const int n1,
    const int n2,
    const int n3
);

/**
\brief Allocates a velocity matrix and initialize its elements to zero

 \pre         sizes are strictly positive numbers (no assert implemented)
 \post        Memory for the matrix was allocated
 \param[in]   n1: size of first dimension in the matrix
 \param[in]   n2: size of second dimension in the matrix
 \return      pointer-pointer to allocated the matrix data
*/
IusVelocity ** iusAllocVelocityFloatMatrix
(
    const int n1,
    const int n2
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   ppDoubleMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of matrix
*/
void iusFreeDoubleMatrix
(
    double **  ppDoubleMatrix,
    const int  n1
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   ppFloatMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of matrix
*/
void iusFreeFloatMatrix
(
    float **  ppFloatMatrix,
    const int n1
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   ppIntMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of matrix
*/
void iusFreeIntMatrix
(
    int **    ppIntMatrix,
    const int n1
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   ppShortMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of matrix
*/
void iusFreeShortMatrix
(
    short **  ppShortMatrix,
    const int n1
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   ppByteMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of matrix
*/
void iusFreeByteMatrix
(
    unsigned char **ppByteMatrix,
    const int       n1
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   pppByteMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of 3D matrix
 \param[in]   n2 : size of second dimension of 3D matrix
*/
void iusFreeByteMatrix3D
(
	unsigned char ***pppByteMatrix,
	const int        n1,
	const int        n2
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   ppDoubleMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of matrix
 \param[in]   n2 : size of first dimension of matrix
*/
void iusFreeFloatMatrix3D
(
    float *** pppFloatMatrix,
    const int n1,
    const int n2
);

/**
\brief Frees a matrix

 \pre         The matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]   ppFloatMatrix:  pointer to a matrix
 \param[in]   n1 : size of first dimension of matrix
*/
void iusFreeVelocityFloatMatrix
(
    IusVelocity **  ppVelocityFloatMatrix,
    const int n1
);








//-----------------------------------------------------------------------------
// Sets all float matrix elements to the same float value
//
// In-place:   n.a.
// pre         The float matrix was allocated
// post        All elements of the float matrix are set to "value"
// param[io]   ppFloatMatrix:  reference to a float matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// param[in]   value:  the value
// return      void
//-----------------------------------------------------------------------------
void iusSetValue2FloatMatrix
(
    float * const * const ppFloatMatrix,
    const int             n1,
    const int             n2,
    const float           value
);

//-----------------------------------------------------------------------------
// Sets all float matrix elements to zero
//
// In-place:   n.a.
// pre         The float matrix was allocated
// post        All elements of the float matrix are set to "value"
// param[io]   ppFloatMatrix:  reference to a float matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusZeroFloatMatrix
(
    float * const * const ppFloatMatrix,
    const int             n1,
    const int             n2
);

//-----------------------------------------------------------------------------
// Set all 3D float matrix elements to zero
//
// In-place:   n.a.
// pre         The 3D float matrix was allocated
// post        All elements of the 3D float matrix are set to "value"
// param[io]   pppFloatMatrix   reference to a 3D float matrix
// param[in]   n1               size of first dimension in the matrix
// param[in]   n2               size of second dimension in the matrix
// param[in]   n3               size of third dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusZeroFloatMatrix3D
(
    float * const * const * const pppFloatMatrix,
    const int                     n1,
    const int                     n2,
    const int                     n3
);

//-----------------------------------------------------------------------------
// Fills a matrix with random values
//
// In-place:   n.a.
// pre         The matrix was allocated
// post        The elements of the matrix have random values based on rand()
//             from stdlib.
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusSetGaussianFloatMatrix // zero-mean and std=1
(
    float * const * const ppFloatMatrix,
    const int             n1,
    const int             n2
);

//-----------------------------------------------------------------------------
// Fill a matrix with random values
//
// In-place:   n.a.
// pre         The matrix was allocated
// post        The elements of the matrix have random values based on rand()
//             from stdlib.
// param[io]   pXYZMatrix:  reference to a matrix of type XYZ
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// param[in]   seed:  seed value, input for srand()
// return      void
//-----------------------------------------------------------------------------
void iusSetRandomDoubleMatrix
(
	double * const * const ppDoubleMatrix,
	const int              n1,
	const int              n2,
	const int              seed
	);

void iusSetRandomFloatMatrix
(
    float * const * const ppFloatMatrix,
    const int             n1,
    const int             n2,
    const int             seed
);

void iusSetRandomIntMatrix
(
    int * const * const ppIntMatrix,
    const int           n1,
    const int           n2,
    const int           seed
);

void iusSetRandomShortMatrix
(
    short * const * const ppShortMatrix,
    const int             n1,
    const int             n2,
    const int             seed
);

//-----------------------------------------------------------------------------
// Copies all data in a matrix to another matrix with the same dimensions
//
// In-place:   no (source and destination must be different)
// pre         Both source and destination matrices were allocated and have
//             the same matrix dimensions
// post        All elements of the source matrix are copied to the destination
//             (with type casting if applicable)
// param[out]  ppDest:  reference to the destination matrix
// param[in]   ppSrc :  reference to the source matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusCopyFloatMatrix
(
          float * const * const ppDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

void iusCopyShortMatrix2FloatMatrix
(
          float * const * const ppDest,
    const short * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

void iusCopyFloatMatrix2ShortMatrix
(
          short * const * const ppDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

//------------------------------------------------------------------------------
// Copy from a float matrix one float vector walking along the first dimension
// of the matrix (think of copying one matrix column to a vector if the matrix 
// has been defined with the first index as the row index and the second index
// as the column index).
//
//  In-place:   n.a. 
// \pre         Both source matrix and destination vector were allocated
//              and source matrix was filled with values
// \post        Destination vector was filled with the element values from one
//              particular vector taken from the source matrix walking along the
//              first dimension                                      
// \param[out]  pDest  : reference to the output vector, length n1
// \param[in]   ppSrc  : reference to the source matrix, size n1 x n2
// \param[in]   n1     : size of matrix first dimension and length of vector
// \param[in]   n2     : size of matrix second dimension
// \param[in]   c      : index of selected vector to be taken along the first
//                       matrix dimension (0 <= c < n2)
//------------------------------------------------------------------------------
void iusCopyVectorAlongFloatMatrixDim1
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2,
    const int                   c
);

//------------------------------------------------------------------------------
// Copy from a float matrix one float vector walking along the second dimension
// of the matrix (think of copying one matrix row to a vector if the matrix has
// been defined with the first index as the row index and the second index as
// the column index).
//
//  In-place:   n.a. 
// \pre         Both source matrix and destination vector were allocated
//              and source matrix was filled with values
// \post        Destination vector was filled with the element values from one
//              particular vector taken from the source matrix walking along the
//              second dimension                                      
// \param[out]  pDest  : reference to the output vector, length n2
// \param[in]   ppSrc  : reference to the source matrix, size n1 x n2
// \param[in]   n1     : size of matrix first dimension
// \param[in]   n2     : size of matrix second dimension and length of vector
// \param[in]   r      : index of selected vector to be taken along the first
//                       matrix dimension (0 <= r < n1)
//------------------------------------------------------------------------------
void iusCopyVectorAlongFloatMatrixDim2
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2,
    const int                   r
);

//----------------------------------------------------------------------------
// Center float matrix columnwise
// In-place:   yes 
// pre         Both source vector and destination matrix were allocated
//             and source vector filled with values
//             
// post        Destination matrix was filled with columnwise centered matrix
//             i.e., the mean value is subtracted from each column                                      
// param[out]  ppDest : reference to the columnwise centered matrix
// param[in]   ppSrc  : reference to the source vector
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
//----------------------------------------------------------------------------
void iusCenterDim1FloatMatrix
(
	      float * const * const ppDest,
	const float * const * const ppSrc,
	const int                   n1,
	const int                   n2
);

//----------------------------------------------------------------------------
// Set float vector to diagonal values of a float square matrix
// In-place:   no 
// pre         Both source vector and destination matrix were allocated
//             and source vector filled with values
//             
// post        Destination elements equal: ppDest[i][i] = ppSrc[i]
//                                         ppDest[i][j] = 0, for i != j
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc  :  reference to the source vector
// param[in]   n: size of vector and square matrix dimension
//----------------------------------------------------------------------------
void iusSetFloatSquareMatrixDiagonal
(
	float * const * const ppFloatMatrix,
	const float * const   pSrc,
	const int             n
);

//----------------------------------------------------------------------------
// Reshape float vector to float matrix
// In-place:   no 
// pre         Both source vector and destination matrix were allocated
//             and source vector filled with values
//             
// post        Destination elements equal: ppDest[i][j] = ppSrc[i*n1+j]
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc  :  reference to the source vector
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
//----------------------------------------------------------------------------
void iusReshapeFloatVector2FloatMatrix
(
	float * const * const ppFloatMatrix,
	const float * const   pSrc,
	const int             n1,
	const int             n2
);

//----------------------------------------------------------------------------
// Reshape double vector to double matrix
// In-place:   no 
// pre         Both source vector and destination matrix were allocated
//             and source vector filled with values
//             
// post        Destination elements equal: ppDest[i][j] = ppSrc[i*n1+j]
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc  :  reference to the source vector
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
//---------------------------------------------------------------------------
void iusReshapeDoubleVector2DoubleMatrix
(
	double * const * const ppDoubleMatrix,
	const double * const   pSrc,
	const int              n1,
	const int              n2
);

//-----------------------------------------------------------------------------
// Perform matrix transpose 
//
// In-place:   no 
// pre         Both source and destination matrices were allocated and 
//             source matrix filled with values
//             
// post        Destination elements equal: ppDest[j][i] = ppSrc[i][j]
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc  :  reference to the source matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusTransposeFloatMatrix
(
	float * const * const       ppDest,
	const float * const * const ppSrc,
	const int                   n1,
	const int                   n2
);

//-----------------------------------------------------------------------------
// Perform float square diagonal matrix inverse 
//
// In-place:   yes  
// pre         Both source and destination matrices were allocated 
//             and source matrix filled with values
// post        Destination elements equal: ppDest[i][i] = 1/ppSrc[i][i] 
//                                         when ppSrc[i][i] != 0
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc  :  reference to the source matrix
// param[in]   n:  size of square matrix dimension
// return      void
//-----------------------------------------------------------------------------
void iusFloatSquareDiagonalMatrixInverse
(
	      float * const * const ppDest,
	const float * const * const ppSrc,
	const int                   n
);

//-----------------------------------------------------------------------------
// Perform multiplication of two matrices
//
// In-place:   no 
// pre         Both source and destination matrices were allocated and filled
//             with values.
// post        Destination elements equal: ppDest[i][j] = ppSrc1[i][]*pSrc2[][j]
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc1 :  reference to the first source matrix
// param[in]   ppSrc2 :  reference to the second source matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// param[in]   m1: size of first dimension in the matrix
// param[in]   m2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusMultiplyFloatMatrices
(
		  float * const * const ppDest,
	const float * const * const ppSrc1,
	const float * const * const ppSrc2,
	const int                   n1,
	const int                   n2,
	const int                   m1,
	const int                   m2
);

//-----------------------------------------------------------------------------
// Performs elementwise addition of two matrices
//
// In-place:   yes (both sources and destination may share same memory space)
// pre         Both source and destination matrices were allocated and filled
//             with values, and all have the same number of elements
// post        Destination elements equal: ppDest[j][i] = ppSrc1[j][i] + pSrc2[j][i]
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc1 :  reference to the first source matrix
// param[in]   ppSrc2 :  reference to the second source matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusAddFloatMatrices
(
          float * const * const ppDest,
    const float * const * const ppSrc1,
    const float * const * const ppSrc2,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Performs elementwise multiplication of two matrices
//
// In-place:   yes (both sources and destination may share same memory space)
// pre         Both source and destination matrices were allocated and filled
//             with values, and all have the same number of elements
// post        Destination elements equal: ppDest[j][i] = ppSrc1[j][i] * pSrc2[j][i]
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc1 :  reference to the first source matrix
// param[in]   ppSrc2 :  reference to the second source matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusMultiplyFloatMatricesElementByElement
(
          float * const * const ppDest,
    const float * const * const ppSrc1,
    const float * const * const ppSrc2,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Multiplies all elements of a matrix with the same scale factor
//
// In-place:   yes (both sources and destination may share same memory space)
// pre         Both source and destination matrices were allocated and filled
//             with values, and all have the same number of elements
// post        Destination elements equal: ppDest[j][i] = ppSrc[j][i] * factor
// param[out]  ppDest :  reference to the destination matrix
// param[in]   ppSrc  :  reference to the source matrix
// param[in]   factor :  scale factor
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusScaleFloatMatrix
(
          float * const * const ppDest,
    const float * const * const ppSrc,
    const float                 factor,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Computes the mean value across all elements of a matrix
//
// In-place:   n.a.
// pre         Input matrix was allocated and filled with values.
// post        The mean value was computed
//
// param[in]   ppFloatMatrix: reference to the input matrix
// param[in]   n1: size of first dimension in the matrix
// param[in]   n2: size of second dimension in the matrix
// return      the mean value
//-----------------------------------------------------------------------------
float iusComputeFloatMatrixMean
(
    const float * const * const ppFloatMatrix,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Finds the maximum / minimum value of a matrix
//
// in-place   n.a.
// pre        Matrix has been allocated and has values.
// post       The result of the operation is returned.
// param[in]  ppSrc:  Input matrix
// param[in]  n1: size of first dimension in the matrix
// param[in]  n2: size of second dimension in the matrix
// return     maximum / minimum value of a matrix
//-----------------------------------------------------------------------------
float iusMaxFloatMatrix
(
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

float iusMinFloatMatrix
(
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Finds the maximum / minimum value of a matrix and its indices
//
// in-place   n.a.
// pre        Matrix has been allocated and has values.
// post       The minimum value and indices are returned.
// param[in]  ppSrc:  Input matrix
// param[in]  n1: size of first dimension in the matrix
// param[in]  n2: size of second dimension in the matrix
// param[out] index1: the found index along first dimension
// param[out] index2: the found index along second dimension
// return     maximum / minimum value of a matrix
//-----------------------------------------------------------------------------
float iusMaxIndicesFloatMatrix
(
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2,
    int *                       index1,
    int *                       index2
);

float iusMinIndicesFloatMatrix
(
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2,
    int *                       index1,
    int *                       index2
);

//-----------------------------------------------------------------------------
// Computes a vector of length n2 with the largest matrix elements along the
// first matrix dimension
//
// in-place   n.a.
// pre        Input matrix has been allocated and has values.
// post       The vector with maxima returned
// param[out] pDest:  Output vector of length n2
// param[in]  ppSrc:  Input matrix of size [n1,n2]
// param[in]  n1: size of first dimension in the matrix
// param[in]  n2: size of second dimension in the matrix
// return     maximum / minimum value of a matrix
//-----------------------------------------------------------------------------
void iusMaxVectorAlongFloatMatrixDim1
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Computes a vector of length n1 with the largest matrix elements along the
// second matrix dimension
//
// in-place   n.a.
// pre        Input matrix has been allocated and has values.
// post       The vector with maxima returned
// param[out] pDest:  Output vector of length n1
// param[in]  ppSrc:  Input matrix of size [n1,n2]
// param[in]  n1: size of first dimension in the matrix
// param[in]  n2: size of second dimension in the matrix
// return     maximum / minimum value of a matrix
//-----------------------------------------------------------------------------
void iusMaxVectorAlongFloatMatrixDim2
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Matrix projection sum along 1st dimension leading to a vector of length n2
//
// in-place   n.a.
// pre        Input matrix of size [n1,n2] has been allocated and has values,
//            output vector of length n2 has been allocated.
// post       The output vector is filled with sum projection values
// param[out] pDest:  Output vector of length n2
// param[in]  ppSrc:  Input matrix of size [n1,n2]
// param[in]  n1: size of first dimension in the matrix
// param[in]  n2: size of second dimension in the matrix
// return     void
//-----------------------------------------------------------------------------
void iusSumFloatMatrixAlongDim1
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

void iusSumSquaredFloatMatrixAlongDim1
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Matrix projection sum along 2nd dimension leading to a vector of length n1
//
// in-place   n.a.
// pre        Input matrix of size [n1,n2] has been allocated and has values,
//            output vector of length n1 has been allocated.
// post       The output vector is filled with sum projection values
// param[out] pDest:  Output vector of length n1
// param[in]  ppSrc:  Input matrix of size [n1,n2]
// param[in]  n1: size of first dimension in the matrix
// param[in]  n2: size of second dimension in the matrix
// return     void
//-----------------------------------------------------------------------------
void iusSumFloatMatrixAlongDim2
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

void iusSumSquaredFloatMatrixAlongDim2
(
    float * const               pDest,
    const float * const * const ppSrc,
    const int                   n1,
    const int                   n2
);

//-----------------------------------------------------------------------------
// Fetching a value from a float matrix at a position with non-integer indices
// using a bilinear interpolation.
//
// in-place   n.a.
// pre        Input matrix of size [n1,n2] has been allocated and has values.
//            (l1 <= (float)(n1 - 1)) && (l1 >= 0.0f)
//            (l2 <= (float)(n2 - 1)) && (l2 >= 0.0f)
// post       A value is returned
// param[in]  ppIn :  Input matrix of size [n1,n2]
// param[in]  n1   :  size of first dimension in the matrix
// param[in]  n2   :  size of second dimension in the matrix
// param[in]  l1   :  non-int index along first dimension, range [0,n1-2]
// param[in]  l2   :  non-int index along second dimension, range [0,n2-2]
// return     interpolated value
//-----------------------------------------------------------------------------
float iusFetchBilinearFloatMatrix
(
    const float * const * const ppIn,
    int                         n1,
    int                         n2,
    float                       l1,
    float                       l2
);

//-----------------------------------------------------------------------------
// Rotates the column pointers of a matrix to right (steps>0) left (steps<0).
//
// in-place   n.a.
// pre        matrix was allocated
// post       column pointers have been rotated with abs(steps) steps. If
//            steps > 0, rotation is to the right. If steps < 0 rotation is to
//            the left. If steps == 0, no rotation.
// param[io]  ppIO :  allocated matrix
// param[in]  n1   :  size of first dimension in the matrix
// param[in]  steps:  amount of steps to rotate to the right
// return     void
//-----------------------------------------------------------------------------
void iusRotateColumnsFloatMatrix
(
    float * * ppIO,
    const int n1,
    const int steps
);

//------------------------------------------------------------------------------
// iusPrintFloatMatrix
//
// Print all elements of a matrix to stdout
//
// in-place   n.a.
// pre        The int matrix has been allocated,
//            and all elements of the matrix have been assigned values.
// post       The error is printed to stdout.
// param[in]  ppMatrix   :  The matrix
// param[in]  n1         :  First matrix dimension
// param[in]  n2         :  Second matrix dimension
// return     void
//------------------------------------------------------------------------------
void iusPrintFloatMatrix
(
    const float * const * const ppMatrix,
    int                         n1,
    int                         n2
);

void iusPrintDoubleMatrix
(
	const double * const * const ppMatrix,
	int                          n1,
	int                          n2
);

#ifdef __cplusplus
}
#endif

#endif // _IUSMATRIX_H
