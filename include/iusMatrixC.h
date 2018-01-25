//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMatrixC.h
//                  Basic functions for complex matrix memory and operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/**
 * \file \brief This file contains prototypes for operations on complex matrices.
 *
 * Functions for allocation and freeing of memory for complex matrices
 * \li iusAllocComplexFloatMatrix
 * \li iusFreeComplexFloatMatrix
 *
 * Functions for assigning values
 * \li  iusSetValue2ComplexFloatMatrix
 * \li  iusZeroComplexFloatMatrix
 *
 * Functions for copying
 * \li  ...
 *
 * Functions for vector analysis
 * \li  iusGetRowFromComplexFloatMatrix
 * \li  iusSetRowToComplexFloatMatrix
 *
 * Operations on complex matrices
 * \li  iusAddComplexFloatMatrices
 * \li  iusSumComplexFloatMatrixAlongDim1
 * \li  iusSumComplexFloatMatrixAlongDim2
 * \li  iusPowerComplexFloatMatrix
 *
 * Various
 * \li  iusPrintComplexFloatMatrix
 */

#ifndef _IUSMATRIXC_H
#define _IUSMATRIXC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusComplex.h"

//=============================================================================
// M A C R O S
//=============================================================================

//=============================================================================
// D E F I N E S
//=============================================================================

//=============================================================================
// D A T A   T Y P E S
//=============================================================================

//=============================================================================
// F U N C T I O N   P R O T O T Y P E S
//=============================================================================

/**
\brief Allocates a complex matrix and initialize its elements to zero

 \pre         Sizes are strictly positive numbers
 \post        Memory for the complex matrix was allocated, elements are zero
 \param[in]   n1 : size of first dimension in the matrix
 \param[in]   n2 : size of second dimension in the matrix
 \return      pointer to a complex matrix
*/
iusComplexFloatMatrix * iusAllocComplexFloatMatrix
(
    const int n1,
    const int n2
);

/**
\brief Frees a matrix

 \pre         The complex matrix was allocated
 \post        Memory for the matrix was freed, matrix reference is NULL
 \param[out]  pComplexFloatMatrix : pointer to a complex matrix
 \param[in]   n1                  : size of first dimension of matrix
*/
void iusFreeComplexFloatMatrix
(
    iusComplexFloatMatrix *pComplexFloatMatrix,
    const int              n1
);

//-----------------------------------------------------------------------------
// Sets all complex matrix elements to the same complex value
//
// In-place:   n.a.
// pre         The complex matrix was allocated
// post        All elements of the complex matrix are set to "value"
// param[io]   pComplexFloatMatrix :  reference to a complex matrix
// param[in]   n1                  : size of first matrix dimension
// param[in]   n2                  : size of second matrix dimension
// param[in]   value               :  the value
// return      void
//-----------------------------------------------------------------------------
void iusSetValue2ComplexFloatMatrix
(
    iusComplexFloatMatrix *const pComplexFloatMatrix,
    const int                    n1,
    const int                    n2,
    const iusComplexFloat        value
);

//-----------------------------------------------------------------------------
// Sets all complex matrix elements to zero
//
// In-place:   n.a.
// pre         The complex matrix was allocated
// post        All elements of the complex matrix are set to zero
// param[io]   pComplexFloatMatrix :  reference to a complex matrix
// param[in]   n1                  : size of first matrix dimension
// param[in]   n2                  : size of second matrix dimension
// return      void
//-----------------------------------------------------------------------------
void iusZeroComplexFloatMatrix
(
    iusComplexFloatMatrix *const pComplexFloatMatrix,
    const int                    n1,
    const int                    n2
);

//-----------------------------------------------------------------------------
// Performs elementwise addition of two complex matrices
//
// In-place:   yes (both sources and destination may share same memory space)
// pre         Both source and destination matrices were allocated and filled
//             with values, and all have the same number of elements
// post        Destination elements equal: ppDest[j][i] = ppSrc1[j][i] + pSrc2[j][i]
// param[out]  ppDest :  reference to the complex destination matrix
// param[in]   ppSrc1 :  reference to the first complex source matrix
// param[in]   ppSrc2 :  reference to the second complex source matrix
// param[in]   n1     : size of first dimension in the matrix
// param[in]   n2     : size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusAddComplexFloatMatrices
(
    iusComplexFloatMatrix *const       pDest,
    const iusComplexFloatMatrix *const pSrc1,
    const iusComplexFloatMatrix *const pSrc2,
    const int                          n1,
    const int                          n2
);

//-----------------------------------------------------------------------------
//  Element-wise power of complex float matrix
// 
// in-place:   no.
// pre         Both source and destination matrices were allocated and filled
//             with values, and all have the same number of elements.
// post        out[i][j] = conj(in[i][j]) * in[i][j]
// 
// param[out]  ppDest : reference to the real-valued destination matrix
// param[in]   pSrc   : reference to the complex-valued source matrix
// param[in]   n1     : size of first dimension in the matrix
// param[in]   n2     : size of second dimension in the matrix
// return      void
//-----------------------------------------------------------------------------
void iusPowerComplexFloatMatrix
(
    float *const *const                ppDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          n1,
    const int                          n2
);

//-----------------------------------------------------------------------------
// Complex matrix projection sum along 1st dimension leading to a complex 
// vector of length n2
//
// in-place   n.a.
// pre        Complex input matrix of size [n1,n2] has been allocated and has
//            values, complex output vector of length n2 has been allocated.
// post       The complex output vector is filled with sum projection values
// param[out] pDest :  complex output vector of length n2
// param[in]  ppSrc :  complex input matrix of size [n1,n2]
// param[in]  n1    : size of first dimension in the matrix
// param[in]  n2    : size of second dimension in the matrix
// return     void
//-----------------------------------------------------------------------------
void iusSumComplexFloatMatrixAlongDim1
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          n1,
    const int                          n2
);

//-----------------------------------------------------------------------------
// Complex matrix projection sum along 2nd dimension leading to a complex
// vector of length n1
//
// in-place   n.a.
// pre        Complex input matrix of size [n1,n2] has been allocated and has
//            values, complex output vector of length n1 has been allocated.
// post       The complex output vector is filled with sum projection values
// param[out] pDest :  complex output vector of length n1
// param[in]  ppSrc :  complex input matrix of size [n1,n2]
// param[in]  n1    : size of first dimension in the matrix
// param[in]  n2    : size of second dimension in the matrix
// return     void
//-----------------------------------------------------------------------------
void iusSumComplexFloatMatrixAlongDim2
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          n1,
    const int                          n2
);

//-----------------------------------------------------------------------------
// Rotates the column pointers of a complex matrix to right (steps>0) left
// (steps<0).
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
void iusRotateColumnsComplexFloatMatrix
(
    iusComplexFloatMatrix *const pIO,
    const int                    n1,
    const int                    steps
);

//-----------------------------------------------------------------------------
// Gets a pointer to a row vector of a complex float matrix
//
// in-place   n.a.
// pre        matrix was allocated
// post       pDest points to requested row vector 
// param[io]  pDest  :  requested row vector
// param[io]  pSrc   :  complex float matrix
// param[in]  row    :  requested row index
// param[in]  numRows:  number of rows in matrix
// return     void
//-----------------------------------------------------------------------------
void iusGetRowFromComplexFloatMatrix
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          row,
    const int                          numRows
);

//-----------------------------------------------------------------------------
// Sets a pointer to a row vector of a complex float matrix
//
// in-place   n.a.
// pre        vector was allocated
// post       pDest[row] points to pSrc float vector 
// param[io]  pDest  :  complex float matrix
// param[io]  pSrc   :  complex float vector
// param[in]  row    :  assigned row index
// param[in]  numRows:  number of rows in matrix
// return     void
//-----------------------------------------------------------------------------
void iusSetRowFromComplexFloatMatrix
(
    iusComplexFloatMatrix *const       pDest,
    const iusComplexFloatVector *const pSrc,
    const int                          row,
    const int                          numRows
);


/**
* \brief Copies all data in a complex matrix to another vector with the same dimensions
*
* In-place:    no (source and destination must be different)
* \pre         Both source and destination matrices were initialized and have
*              the same number of elements
* \post        All elements of the source matrix are copied to the destination
* \param[out]  pDest  :  reference to the complex destination matrix
* \param[in]   pSrc   :  reference to the complex source matrix
* \param[in]   n1     :  number of rows
* \param[in]   n2     :  number of columns
*/
void iusCopyComplexFloatMatrix
(
	iusComplexFloatMatrix* const       pDest,
	const iusComplexFloatMatrix* const pSrc,
	int                                n1,
	int								   n2
);

//------------------------------------------------------------------------------
// iusPrintComplexFloatMatrix
//
// Print all elements of a complex matrix to stdout
//
// in-place   n.a.
// pre        The complex matrix has been allocated,
//            and all elements of the matrix have been assigned values.
// post       The matrix text is printed to stdout.
// param[in]  ppMatrix   :  The matrix
// param[in]  n1         :  First matrix dimension
// param[in]  n2         :  Second matrix dimension
// return     void
//------------------------------------------------------------------------------
void iusPrintComplexFloatMatrix
(
    const iusComplexFloatMatrix *const pComplexFloatMatrix,
    const int                          n1,
    const int                          n2
);

#ifdef __cplusplus
}
#endif

#endif // _IUSMATRIXC_H
