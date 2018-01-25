//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMatrixC.c
//                  Basic functions for complex matrix memory and operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusMatrixC.h"
#include "iusMatrix.h"
#include "iusVector.h"
#include "iusError.h"
#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // malloc..

//=============================================================================
// L O C A L   D E F I N E S
//=============================================================================

//=============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//=============================================================================

//=============================================================================
// E X P O R T E D   F U N C T I O N S
//=============================================================================

//-----------------------------------------------------------------------------
// iusAllocate a matrix
//-----------------------------------------------------------------------------
iusComplexFloatMatrix * iusAllocComplexFloatMatrix
(
    const int n1,
    const int n2
)
{
    iusComplexFloatMatrix *pComplexFloatMatrix =
        (iusComplexFloatMatrix *)malloc( sizeof( iusComplexFloatMatrix ) );

    IUS_ASSERT_MEMORY( pComplexFloatMatrix );

    pComplexFloatMatrix->ppRe = iusAllocFloatMatrix( n1, n2 );
    pComplexFloatMatrix->ppIm = iusAllocFloatMatrix( n1, n2 );

    return pComplexFloatMatrix;
}


//-----------------------------------------------------------------------------
// Free a float matrix
//-----------------------------------------------------------------------------
void iusFreeComplexFloatMatrix
(
    iusComplexFloatMatrix *pComplexFloatMatrix,
    const int              n1
)
{
    IUS_ASSERT( pComplexFloatMatrix );

    iusFreeFloatMatrix( pComplexFloatMatrix->ppRe, n1 );
    iusFreeFloatMatrix( pComplexFloatMatrix->ppIm, n1 );
    free( pComplexFloatMatrix );
}
 
//-----------------------------------------------------------------------------
// Set all complex matrix elements to the same float value
//-----------------------------------------------------------------------------
void iusSetValue2ComplexFloatMatrix
(
    iusComplexFloatMatrix *const pComplexFloatMatrix,
    const int                    n1,
    const int                    n2,
    const iusComplexFloat        value
)
{
	IUS_ASSERT( pComplexFloatMatrix );

    iusSetValue2FloatMatrix( pComplexFloatMatrix->ppRe, n1, n2, value.re );
    iusSetValue2FloatMatrix( pComplexFloatMatrix->ppIm, n1, n2, value.im );
}

//-----------------------------------------------------------------------------
// Set all complex matrix elements to 0
//-----------------------------------------------------------------------------
void iusZeroComplexFloatMatrix
(
    iusComplexFloatMatrix *const pComplexFloatMatrix,
    const int                    n1,
    const int                    n2
)
{
    IUS_ASSERT( pComplexFloatMatrix );

    iusZeroFloatMatrix( pComplexFloatMatrix->ppRe, n1, n2 );
    iusZeroFloatMatrix( pComplexFloatMatrix->ppIm, n1, n2 );
}

//-----------------------------------------------------------------------------
// Elementwise addition of complex float matrices
//-----------------------------------------------------------------------------
void iusAddComplexFloatMatrices
(
    iusComplexFloatMatrix *const       ppDest,
    const iusComplexFloatMatrix *const ppSrc1,
    const iusComplexFloatMatrix *const ppSrc2,
    const int                          n1,
    const int                          n2
)
{
	IUS_ASSERT( ppDest && ppSrc1 && ppSrc2 );

    iusAddFloatMatrices( ppDest->ppRe, (const float *const *const)ppSrc1->ppRe, (const float *const *const)ppSrc2->ppRe, n1, n2 );
    iusAddFloatMatrices( ppDest->ppIm, (const float *const *const)ppSrc1->ppIm, (const float *const *const)ppSrc2->ppIm, n1, n2 );
}

//-----------------------------------------------------------------------------
// Power of complex float matrix
//-----------------------------------------------------------------------------
void iusPowerComplexFloatMatrix
(
    float *const *const                ppDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          n1,
    const int                          n2
)
{
    IUS_ASSERT( ppDest && pSrc );

    int c1, c2;

    for ( c1 = 0; c1 < n1; c1++ )
    {
        for ( c2 = 0; c2 < n2; c2++ )
        {
            const float a_re = pSrc->ppRe[c1][c2];
            const float a_im = pSrc->ppIm[c1][c2];

            // conj( a ) * a
            ppDest[c1][c2] = a_re * a_re + a_im * a_im;
        }
    }
}


//-----------------------------------------------------------------------------
// Complex matrix projection sum along 1st dimension leading to a complex
// vector of length n2
//-----------------------------------------------------------------------------
void iusSumComplexFloatMatrixAlongDim1
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          n1,
    const int                          n2
)
{
	IUS_ASSERT( pSrc && pDest );

    iusSumFloatMatrixAlongDim1( pDest->pRe, (const float *const *const)pSrc->ppRe, n1, n2 );
    iusSumFloatMatrixAlongDim1( pDest->pIm, (const float *const *const)pSrc->ppIm, n1, n2 );
}

//-----------------------------------------------------------------------------
// Complex matrix projection sum along 2nd dimension leading to a complex
// vector of length n1
//-----------------------------------------------------------------------------
void iusSumComplexFloatMatrixAlongDim2
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          n1,
    const int                          n2
)
{
	IUS_ASSERT( pSrc && pDest );

    iusSumFloatMatrixAlongDim2( pDest->pRe, (const float *const *const)pSrc->ppRe, n1, n2 );
    iusSumFloatMatrixAlongDim2( pDest->pIm, (const float *const *const)pSrc->ppIm, n1, n2 );
}

//-----------------------------------------------------------------------------
// Get a row vector from a matrix.
//-----------------------------------------------------------------------------
void iusGetRowFromComplexFloatMatrix
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatMatrix *const pSrc,
    const int                          row,
    const int                          numRows
)
{
    IUS_ASSERT( pSrc && pDest && row >= 0 && row < numRows);

    pDest->pRe = pSrc->ppRe[row];
    pDest->pIm = pSrc->ppIm[row];
}

//-----------------------------------------------------------------------------
// Set a row vector from a matrix.
//-----------------------------------------------------------------------------
void iusSetRowFromComplexFloatMatrix
(
    iusComplexFloatMatrix *const       pDest,
    const iusComplexFloatVector *const pSrc,
    const int                          row,
    const int                          numRows
)
{
    IUS_ASSERT( pSrc && pDest && row >= 0 && row < numRows);

    pDest->ppRe[row] = pSrc->pRe;
    pDest->ppIm[row] = pSrc->pIm;
}

//-----------------------------------------------------------------------------
// Copy a matrix.
//-----------------------------------------------------------------------------
void iusCopyComplexFloatMatrix
(
	iusComplexFloatMatrix* const       pDest,
	const iusComplexFloatMatrix* const pSrc,
	int                                n1,
	int								   n2
)
{
	IUS_ASSERT( pSrc && pDest && n1 > 0 && n2 > 0);

	iusCopyFloatMatrix(pDest->ppRe, (const float *const *const)pSrc->ppRe, n1, n2);
	iusCopyFloatMatrix(pDest->ppIm, (const float *const *const)pSrc->ppIm, n1, n2);

}
//-----------------------------------------------------------------------------
// Rotate the column pointers of a matrix to right (steps>0) left (steps<0).
//-----------------------------------------------------------------------------
void iusRotateColumnsComplexFloatMatrix
(
    iusComplexFloatMatrix *const pIO,
    const int                    n1,
    const int                    steps
)
{
    int i, j;
    float *pTmpIm;
    float *pTmpRe;

	IUS_ASSERT( pIO );

    if (steps >= 0)
    {
        // rotate right "steps" times
        for (j = 0; j < steps; j++)
        {
            // rotate right 1 time
            pTmpIm = pIO->ppIm[n1 - 1];
            pTmpRe = pIO->ppRe[n1 - 1];
            for (i = n1 - 1; i > 0; i--)
            {
                pIO->ppIm[i] = pIO->ppIm[i - 1];
                pIO->ppRe[i] = pIO->ppRe[i - 1];
            }
            pIO->ppIm[0] = pTmpIm;
            pIO->ppRe[0] = pTmpRe;
        }
    }
    else
    {
        // rotate left "-steps" times
        for (j = 0; j < -steps; j++)
        {
            // rotate left 1 time
            pTmpIm = pIO->ppIm[0];
            pTmpRe = pIO->ppRe[0];
            for (i = 0; i < n1 - 1; i++)
            {
                pIO->ppIm[i] = pIO->ppIm[i + 1];
                pIO->ppRe[i] = pIO->ppRe[i + 1];
            }
            pIO->ppIm[n1 - 1] = pTmpIm;
            pIO->ppRe[n1 - 1] = pTmpRe;
        }
    }
}

//------------------------------------------------------------------------------
// Print all elements of a matrix to stdout
//------------------------------------------------------------------------------
void iusPrintComplexFloatMatrix
(
    const iusComplexFloatMatrix *const pComplexFloatMatrix,
    const int                          n1,
    const int                          n2
)
{
	IUS_ASSERT( pComplexFloatMatrix );

    fprintf( stdout, "Real part:\n" );
    iusPrintFloatMatrix( (const float *const *const)pComplexFloatMatrix->ppRe, n1, n2 );
    fprintf( stdout, "Imaginary part:\n" );
    iusPrintFloatMatrix( (const float *const *const)pComplexFloatMatrix->ppIm, n1, n2 );
}
