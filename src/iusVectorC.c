//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusVectorC.c
//                  Basic functions for complex vector memory and operations
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#include "iusVector.h"
#include "iusVectorC.h"
#include "iusBasicFunctions.h"
#include "iusError.h" // for assertions
#include <stdio.h>    // for fprintf, stdout, ...
#include <stdlib.h>   // for calloc,..
#include <string.h>   // for memmove,memset,..

//==============================================================================
// L O C A L   D E F I N E S
//==============================================================================

//==============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//==============================================================================

//==============================================================================
// E X P O R T E D   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// Allocate a vector
//------------------------------------------------------------------------------
iusComplexDoubleVector *iusAllocComplexDoubleVector
(
    int length
)
{
    iusComplexDoubleVector *pComplexDoubleVector;

    pComplexDoubleVector =
        (iusComplexDoubleVector *)malloc( sizeof( iusComplexDoubleVector ) );
    IUS_ASSERT_MEMORY( pComplexDoubleVector );

    pComplexDoubleVector->pRe = iusAllocDoubleVector( length );
    pComplexDoubleVector->pIm = iusAllocDoubleVector( length );

    return pComplexDoubleVector;
}

iusComplexFloatVector *iusAllocComplexFloatVector
(
    int length
)
{
    iusComplexFloatVector *pComplexFloatVector;

    pComplexFloatVector =
        (iusComplexFloatVector *)malloc( sizeof( iusComplexFloatVector ) );
    IUS_ASSERT_MEMORY( pComplexFloatVector );

    pComplexFloatVector->pRe = iusAllocFloatVector( length );
    pComplexFloatVector->pIm = iusAllocFloatVector( length );

    return pComplexFloatVector;
}

iusComplexIntVector *iusAllocComplexIntVector
(
    int length
)
{
    iusComplexIntVector *pComplexIntVector;
    
    pComplexIntVector =
        (iusComplexIntVector *)malloc( sizeof( iusComplexIntVector ) );
    IUS_ASSERT_MEMORY( pComplexIntVector );

    pComplexIntVector->pRe = iusAllocIntVector( length );
    pComplexIntVector->pIm = iusAllocIntVector( length );

    return pComplexIntVector;
}

iusComplexShortVector *iusAllocComplexShortVector
(
    int length
)
{
    iusComplexShortVector *pComplexShortVector;
    
    pComplexShortVector = 
        (iusComplexShortVector *)malloc( sizeof( iusComplexShortVector ) );
    IUS_ASSERT_MEMORY( pComplexShortVector );

    pComplexShortVector->pRe = iusAllocShortVector( length );
    pComplexShortVector->pIm = iusAllocShortVector( length );

    return pComplexShortVector;
}

//------------------------------------------------------------------------------
// Set all elements of a complex vector to the same value
//------------------------------------------------------------------------------
void iusSetValue2ComplexDoubleVector
(
    iusComplexDoubleVector *const pComplexDoubleVector,
    int                           length,
    iusComplexDouble_t            value
)
{
    IUS_ASSERT( pComplexDoubleVector );

    iusSetValue2DoubleVector( pComplexDoubleVector->pRe, length, value.re );
    iusSetValue2DoubleVector( pComplexDoubleVector->pIm, length, value.im );
}

void iusSetValue2ComplexFloatVector
(
    iusComplexFloatVector *const pComplexFloatVector,
    int                          length,
    iusComplexFloat              value
)
{
    IUS_ASSERT( pComplexFloatVector );

    iusSetValue2FloatVector( pComplexFloatVector->pRe, length, value.re );
    iusSetValue2FloatVector( pComplexFloatVector->pIm, length, value.im );
}

void iusSetValue2ComplexIntVector
(
    iusComplexIntVector *const pComplexIntVector,
    int                        length,
    iusComplexInt_t            value
)
{
    IUS_ASSERT( pComplexIntVector );

    iusSetValue2IntVector( pComplexIntVector->pRe, length, value.re );
    iusSetValue2IntVector( pComplexIntVector->pIm, length, value.im );
}

void iusSetValue2ComplexShortVector
(
    iusComplexShortVector *const pComplexShortVector,
    int                          length,
    iusComplexShort_t            value
)
{
    IUS_ASSERT( pComplexShortVector );

    iusSetValue2ShortVector( pComplexShortVector->pRe, length, value.re );
    iusSetValue2ShortVector( pComplexShortVector->pIm, length, value.im );
}

//------------------------------------------------------------------------------
// Fill a vector with a exp(j*theta) table at a certain normalized frequency and
// with a certain phase offset
//------------------------------------------------------------------------------
void iusSetShiftedPhasorTable2ComplexFloatVector
(
    iusComplexFloatVector *pComplexFloatVector,
    int                    length,
    float                  normFreq,  // normFreq = 2 * PI * freqHz / sampleRate
    float                  phase
)
{
    IUS_ASSERT( pComplexFloatVector );

    iusSetShiftedCosineTable2FloatVector( pComplexFloatVector->pRe, length,
        normFreq, phase );
    iusSetShiftedSineTable2FloatVector( pComplexFloatVector->pIm, length,
        normFreq, phase );
}

//------------------------------------------------------------------------------
// Copy all data in a complex vector to another vector with the same dimensions
//------------------------------------------------------------------------------
void iusCopyComplexFloatVector
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc,
    int                                length
)
{
    IUS_ASSERT( pSrc && pDest && (pSrc != (const iusComplexFloatVector *)pDest) );

    memcpy( pDest->pRe, pSrc->pRe, length * sizeof( float ) );
    memcpy( pDest->pIm, pSrc->pIm, length * sizeof( float ) );
}

//------------------------------------------------------------------------------
// Free a complex vector
//------------------------------------------------------------------------------
void iusFreeComplexDoubleVector
(
    iusComplexDoubleVector *pComplexDoubleVector
)
{
    IUS_ASSERT( pComplexDoubleVector );
    iusFreeDoubleVector( pComplexDoubleVector->pRe );
    iusFreeDoubleVector( pComplexDoubleVector->pIm );
    free( pComplexDoubleVector );
}

void iusFreeComplexFloatVector
(
    iusComplexFloatVector *pComplexFloatVector
)
{
    IUS_ASSERT( pComplexFloatVector );
    iusFreeFloatVector( pComplexFloatVector->pRe );
    iusFreeFloatVector( pComplexFloatVector->pIm );
    free( pComplexFloatVector );
}

void iusFreeComplexIntVector
(
    iusComplexIntVector *pComplexIntVector
)
{
    IUS_ASSERT( pComplexIntVector );
    iusFreeIntVector( pComplexIntVector->pRe );
    iusFreeIntVector( pComplexIntVector->pIm );
    free( pComplexIntVector );
}

void iusFreeComplexShortVector
(
    iusComplexShortVector *pComplexShortVector
)
{
    IUS_ASSERT( pComplexShortVector );
    iusFreeShortVector( pComplexShortVector->pRe );
    iusFreeShortVector( pComplexShortVector->pIm );
    free( pComplexShortVector );
}


//------------------------------------------------------------------------------
// Element-wise multiplication / addition / subtraction of two complex vectors
//------------------------------------------------------------------------------
void iusMultiplyComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc1 && pSrc2 );
    IUS_ASSERT( (pDest != pSrc1) && (pDest != pSrc2) );
    IUS_ASSERT( pDest->pRe && pSrc1->pRe && pSrc2->pRe );
    IUS_ASSERT( pDest->pIm && pSrc1->pIm && pSrc2->pIm );

    for ( i = 0; i < length; i++ )
    {
        pDest->pRe[i] = pSrc1->pRe[i] * pSrc2->pRe[i] - pSrc1->pIm[i] * pSrc2->pIm[i];
        pDest->pIm[i] = pSrc1->pIm[i] * pSrc2->pRe[i] + pSrc1->pRe[i] * pSrc2->pIm[i];
    }
}

void iusMultiplyConjugateComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc1 && pSrc2 );
    IUS_ASSERT( (pDest != pSrc1) && (pDest != pSrc2) );
    IUS_ASSERT( pDest->pRe && pSrc1->pRe && pSrc2->pRe );
    IUS_ASSERT( pDest->pIm && pSrc1->pIm && pSrc2->pIm );

    for ( i = 0; i < length; i++ )
    {
        pDest->pRe[i] = pSrc1->pRe[i] * pSrc2->pRe[i] + pSrc1->pIm[i] * pSrc2->pIm[i];
        pDest->pIm[i] = pSrc1->pIm[i] * pSrc2->pRe[i] - pSrc1->pRe[i] * pSrc2->pIm[i];
    }
}

void iusAddComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
)
{
    IUS_ASSERT( pDest && pSrc1 && pSrc2 );

    iusAddFloatVectors( pDest->pRe, pSrc1->pRe, pSrc2->pRe, length );
    iusAddFloatVectors( pDest->pIm, pSrc1->pIm, pSrc2->pIm, length );
}

void iusAccComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc,
    int                                length
)
{
    IUS_ASSERT( pDest && pSrc );

    iusAccFloatVectors( pDest->pRe, pSrc->pRe, length );
    iusAccFloatVectors( pDest->pIm, pSrc->pIm, length );
}

void iusSubtractComplexFloatVectors
(
    iusComplexFloatVector *const       pDest,
    const iusComplexFloatVector *const pSrc1,
    const iusComplexFloatVector *const pSrc2,
    int                                length
)
{
    IUS_ASSERT( pDest && pSrc1 && pSrc2 );

    iusSubtractFloatVectors( pDest->pRe, pSrc1->pRe, pSrc2->pRe, length );
    iusSubtractFloatVectors( pDest->pIm, pSrc1->pIm, pSrc2->pIm, length );
}

//------------------------------------------------------------------------------
// Take power of complex input vector.
//------------------------------------------------------------------------------
void iusPowerComplexFloatVector
(
    float*                       pDest,
    const iusComplexFloatVector *pSrc,
    const int                    N
)
{
    int c1;

    IUS_ASSERT( pDest && pSrc );

    for ( c1 = 0; c1 < N; c1++ )
    {
        const float a_re = pSrc->pRe[c1];
        const float a_im = pSrc->pIm[c1];

        // conj( a ) * a
        pDest[c1] = a_re * a_re + a_im * a_im;
    }
}

//------------------------------------------------------------------------------
// Scaling of a complex vector (by a real value)
//------------------------------------------------------------------------------
void iusScaleComplexFloatVector
(
    iusComplexFloatVector *      pDest,
    const iusComplexFloatVector *pSrc,
    float                        factor,
    int                          length
)
{
    IUS_ASSERT( pDest && pSrc );

    iusScaleFloatVector( pDest->pRe, pSrc->pRe, factor, length );
    iusScaleFloatVector( pDest->pIm, pSrc->pIm, factor, length );

}

//------------------------------------------------------------------------------
// Print all elements of a complex vector to stdout
//------------------------------------------------------------------------------
void iusPrintComplexDoubleVector
(
    const iusComplexDoubleVector *const pComplexDoubleVector,
    int                                 length
)
{
    int i;

    IUS_ASSERT( pComplexDoubleVector );

    for ( i = 0; i < length; i++ )
    {
        fprintf( stdout, "\t%8.4lf + %8.4lfi\n", pComplexDoubleVector->pRe[i],
            pComplexDoubleVector->pIm[i] );
    }
    fprintf( stdout, "\n" );
}

void iusPrintComplexFloatVector
(
    const iusComplexFloatVector *const pComplexFloatVector,
    int                                length
)
{
    int i;

	IUS_ASSERT( pComplexFloatVector );

    for ( i = 0; i < length; i++ )
    {
        fprintf( stdout, "\t%8.4f + %8.4fi\n", pComplexFloatVector->pRe[i],
            pComplexFloatVector->pIm[i] );
    }
    fprintf( stdout, "\n" );
}

