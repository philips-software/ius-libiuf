//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusVector.c
//                  Basic functions for vector memory and operations
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#include "iusVector.h"
#include "iusBasicFunctions.h"
//#include "filterdat/FIR_interp_8.h"
#include "iusError.h"
#include <stdio.h>  // for fprintf, stdout, ...
#include <stdlib.h> // for calloc,..
#include <string.h> // for memmove,memset,..
#include <math.h>   // for sqrt,sin,cos,..

//==============================================================================
// L O C A L   D E F I N E S
//==============================================================================
#define SWAP(a, b) { float t = (a); (a) = (b); (b) = t; }

static const float s_FIR_interp_8[] = { -6.496636e-02f, -1.286166e-01f,
    -1.809723e-01f, -2.122066e-01f, -2.138764e-01f, -1.800633e-01f,
    -1.082773e-01f, -2.168965e-17f,  1.392136e-01f,  3.001054e-01f,
     4.705280e-01f,  6.366198e-01f,  7.842133e-01f,  9.003163e-01f,
     9.744954e-01f,  1.000000e+00f,  9.744954e-01f,  9.003163e-01f,
     7.842133e-01f,  6.366198e-01f,  4.705280e-01f,  3.001054e-01f,
     1.392136e-01f, -2.168965e-17f, -1.082773e-01f, -1.800633e-01f,
    -2.138764e-01f, -2.122066e-01f, -1.809723e-01f, -1.286166e-01f,
    -6.496636e-02f };


//==============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//==============================================================================
static float LF_GetMedianFromWindow  // rename to MedianFloat?
(
    float window[],
    int windowWidth
);

//==============================================================================
// E X P O R T E D   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// Allocate a vector
//------------------------------------------------------------------------------
double * iusAllocDoubleVector
(
    int length
)
{
    double *pDoubleVector = (double *)calloc( length, sizeof( double ) );
    IUS_ASSERT_MEMORY( pDoubleVector );

    return pDoubleVector;
}

float * iusAllocFloatVector
(
    int length
)
{
    float *pFloatVector = (float *)calloc( length, sizeof( float ) );
    IUS_ASSERT_MEMORY( pFloatVector );

    return pFloatVector;
}

int * iusAllocIntVector
(
    int length
)
{
    int *pIntVector = (int *)calloc( length, sizeof( int ) );
    IUS_ASSERT_MEMORY( pIntVector );

    return pIntVector;
}

short * iusAllocShortVector
(
    int length
)
{
    short *pShortVector = (short *)calloc( length, sizeof( short ) );
    IUS_ASSERT_MEMORY( pShortVector );

    return pShortVector;
}

unsigned short * iusAllocUnsignedShortVector
(
    int length
)
{
    unsigned short *pUnsignedShortVector = 
        (unsigned short *)calloc( length, sizeof( unsigned short ) );
    IUS_ASSERT_MEMORY( pUnsignedShortVector );

    return pUnsignedShortVector;
}

unsigned char * iusAllocByteVector
(
    int length
)
{
    unsigned char *pByteVector = 
        (unsigned char *)calloc( length, sizeof( unsigned char ) );
    IUS_ASSERT_MEMORY( pByteVector );

    return pByteVector;
}

//------------------------------------------------------------------------------
// Free a vector
//------------------------------------------------------------------------------
void iusFreeDoubleVector
(
    double *pDoubleVector
)
{
    IUS_ASSERT( pDoubleVector );
    free( pDoubleVector );
}

void iusFreeFloatVector
(
    float *pFloatVector
)
{
    IUS_ASSERT( pFloatVector );
    free( pFloatVector );
}

void iusFreeIntVector
(
    int *pIntVector
)
{
    IUS_ASSERT( pIntVector );
    free( pIntVector );
}

void iusFreeShortVector
(
    short *pShortVector
)
{
    IUS_ASSERT( pShortVector );
    free( pShortVector );
}

void iusFreeUnsignedShortVector
(
    unsigned short *pUnsignedShortVector
)
{
    IUS_ASSERT( pUnsignedShortVector );
    free( pUnsignedShortVector );
}

void iusFreeByteVector
(
    unsigned char *pByteVector
)
{
    IUS_ASSERT( pByteVector );
    free( pByteVector );
}

//------------------------------------------------------------------------------
// Set all elements of a vector to the same value
//------------------------------------------------------------------------------
void iusSetValue2DoubleVector
(
    double *pDoubleVector,
    int     length,
    double  value
)
{
    int i;
    IUS_ASSERT( pDoubleVector );

    for ( i = 0; i < length; i++ )
    {
        pDoubleVector[i] = value;
    }
}

void iusSetValue2FloatVector
(
    float *pFloatVector,
    int    length,
    float  value
)
{
    int i;
    IUS_ASSERT( pFloatVector );

    for ( i = 0; i < length; i++ )
    {
        pFloatVector[i] = value;
    }
}

void iusSetValue2IntVector
(
    int *pIntVector,
    int  length,
    int  value
)
{
    int i;
    IUS_ASSERT( pIntVector );

    for ( i = 0; i < length; i++ )
    {
        pIntVector[i] = value;
    }
}

void iusSetValue2ShortVector
(
    short *pShortVector,
    int    length,
    short  value
)
{
    int i;
    IUS_ASSERT( pShortVector );

    for ( i = 0; i < length; i++ )
    {
        pShortVector[i] = value;
    }
}

void iusSetValue2ByteVector
(
    unsigned char *pByteVector,
    int            length,
    unsigned char  value
)
{
    int i;
    IUS_ASSERT( pByteVector );

    for ( i = 0; i < length; i++ )
    {
        pByteVector[i] = value;
    }
}

//------------------------------------------------------------------------------
// Set all elements of a vector to zero
//------------------------------------------------------------------------------
void iusZeroDoubleVector
(
    double *pDoubleVector,
    int     length
)
{
    IUS_ASSERT( pDoubleVector );

    memset( pDoubleVector, 0, length * sizeof( double ) );
}

void iusZeroFloatVector
(
    float *pFloatVector,
    int    length
)
{
    IUS_ASSERT( pFloatVector );

    memset( pFloatVector, 0, length * sizeof( float ) );
}

void iusZeroIntVector
(
    int *pIntVector,
    int  length
)
{
    IUS_ASSERT( pIntVector );

    memset( pIntVector, 0, length * sizeof( int ) );
}

void iusZeroShortVector
(
    short *pShortVector,
    int    length
)
{
    IUS_ASSERT( pShortVector );

    memset( pShortVector, 0, length * sizeof( short ) );
}

void iusZeroByteVector
(
    unsigned char *pByteVector,
    int            length
)
{
    IUS_ASSERT( pByteVector );

    memset( pByteVector, 0, length * sizeof( unsigned char ) );
}

//------------------------------------------------------------------------------
// Fill a vector with a sine table at a certain normalized frequency
//------------------------------------------------------------------------------
void iusSetSineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq  // normFreq  = 2 * PI * freqHz / sampleRate;
)
{
    int i;

    IUS_ASSERT( pFloatVector );

    for ( i = 0; i < length; i++ )
    {
        pFloatVector[i] = sinf( normFreq * i );
    }
}

//------------------------------------------------------------------------------
// Fill a vector with a sine table at a certain normalized frequency and with a
// certain phase offset
//------------------------------------------------------------------------------
void iusSetShiftedSineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq,  // normFreq  = 2 * PI * freqHz / sampleRate;
    float  phase
)
{
    int i;

    IUS_ASSERT( pFloatVector );

    for ( i = 0; i < length; i++ )
    {
        pFloatVector[i] = (float)sin( normFreq * i + phase );
    }
}

//------------------------------------------------------------------------------
// Fill a vector with a cosine table at a certain normalized frequency and with
// a certain phase offset
//------------------------------------------------------------------------------
void iusSetShiftedCosineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq,  // normFreq  = 2 * PI * freqHz / sampleRate;
    float  phase
)
{
    int i;

    IUS_ASSERT( pFloatVector );

    for ( i = 0; i < length; i++ )
    {
        pFloatVector[i] = (float)cos( normFreq * i + phase );
    }
}

//------------------------------------------------------------------------------
// Fill a vector with a cosine table for a certain normalized frequency
//------------------------------------------------------------------------------
void iusSetCosineTable2FloatVector
(
    float *pFloatVector,
    int    length,
    float  normFreq  // normFreq  = 2 * PI * freqHz / sampleRate;
)
{
    int i;

    IUS_ASSERT( pFloatVector );

    for ( i = 0; i < length; i++ )
    {
        pFloatVector[i] = (float)cos( normFreq * i );
    }
}

//------------------------------------------------------------------------------
// Fill a vector with a sigmoid function
//------------------------------------------------------------------------------
void iusSetSigmoid2FloatVector
(
    float *pFloatVector,
    int    length,
    float  scale
)
{
    int i;
    int l2 = length / 2;

    IUS_ASSERT_VALUE( l2*2 == length );

    for ( i = 0; i < length; i++ )
    {
        // (a*(x-(N/2))) ./ sqrt(1 + (a*(x-(N/2))).^2);
        float x  = 2.0f * scale * (i - l2) / length;
        double y = (double)(1 + x * x);

        pFloatVector[i] = x / (float)sqrt( y );
    }
}

//------------------------------------------------------------------------------
// Returns an output vector with at each element the polynomial value evaluated
// at the corresponding element of input vector pX:
// For the i-th vector element let x = pX[i] and y = pFloatVector[i], then
//   y = pA[n-1] + pA[n-2] * x + pA[n-3] * x^2 + ... pA[0]*x^(n-1)
//------------------------------------------------------------------------------
void iusPolyValFloatVector
(
    float *      pFloatVector,
    const float *pX,
    int          length,
    const float *pA,
    int          n
)
{
    int i, j;

    IUS_ASSERT( pFloatVector && pX && pA && 
        ((const float *)pFloatVector != pX) );

    for ( i = 0; i < length; i++ )
    {
        float x  = pX[i];
        float xi = pX[i];

        pFloatVector[i] = pA[n-1];
        for ( j = n-2; j >= 0; j-- )
        {
            pFloatVector[i] += pA[j] * x;
            x               *= xi;
        }
    }
}

//------------------------------------------------------------------------------
// Fill a vector with random values
//------------------------------------------------------------------------------
void iusSetGaussianFloatVector
(
    float *pFloatVector,
    int    length,
    int    seed
)
{
    int i, j;

    IUS_ASSERT( pFloatVector );

    srand( (unsigned int)seed );
    for ( i = 0; i < length; i++ )
    {
        pFloatVector[i] = 0.0f;
        for ( j = 0; j < 12; j++ )
        {
            pFloatVector[i] += (float)rand() / (float)RAND_MAX;
        }
        pFloatVector[i] -= 6.0f;
    }
}

void iusSetRandomFloatVector
(
    float *pFloatVector,
    int    length,
    int    seed
)
{
    int i;

    IUS_ASSERT( pFloatVector );

    srand( (unsigned int)seed );
    for ( i = 0; i < length; i++ )
    {
        pFloatVector[i] = (float)rand();
    }
}

void iusSetRandomIntVector
(
    int *pIntVector,
    int  length,
    int  seed
)
{
    int i;

    IUS_ASSERT( pIntVector );

    srand( (unsigned int)seed );
    for ( i = 0; i < length; i++ )
    {
        pIntVector[i] = rand();
    }
}

void iusSetRandomShortVector
(
    short *pShortVector,
    int    length,
    int    seed
)
{
    int i;

    IUS_ASSERT( pShortVector );

    srand( (unsigned int)seed );
    for ( i = 0; i < length; i++ )
    {
        pShortVector[i] = (short)rand();
    }
}

void iusSetRandomBinaryShortVector
(
    short *pShortVector,
    int    length,
    int    seed
)
{
    int i;
    int T = RAND_MAX / 2;

    IUS_ASSERT( pShortVector );

    srand( (unsigned int)seed );
    for ( i = 0; i < length; i++ )
    {
        pShortVector[i] = (rand() > T) ? 1 : 0;
    }
}

//------------------------------------------------------------------------------
// Fill a vector with equally spaced values between a start value and an end
// value.
//------------------------------------------------------------------------------
void iusSetLinSpaceFloatVector
(
    float *const pFloatVector,
    int          length,
    float        startValue,
    float        endValue
)
{
    int     i;
    float range = endValue - startValue;

    IUS_ASSERT( pFloatVector );
    IUS_ASSERT_VALUE( length > 0 );

    pFloatVector[0] = startValue;
    pFloatVector[length-1] = endValue;
    for ( i = 1; i < length - 1; i++ )
    {
        pFloatVector[i] = startValue + (float)i * range / (float)(length-1);
    }
}

//------------------------------------------------------------------------------
// Fill a vector with an N-points Hamming/Hanning window
//------------------------------------------------------------------------------
void iusSetHammingFloatVector
(
    float *const pFloatVector,
    int          N
)
{
    int i;

    IUS_ASSERT( pFloatVector);
    IUS_ASSERT( N > 0 );

    if ( N == 1 )
    {
        pFloatVector[0] = 1.0f;
    }
    else
    {
        for ( i = 0; i < N; i++ )
        {
            pFloatVector[i] = 0.54f - 0.46f * cosf( 2.0f * IUS_PI * i / (N - 1) );
        }
    }
}

void iusSetHanningFloatVector
(
    float *const pFloatVector,
    int          N
)
{
    int i;

    IUS_ASSERT( pFloatVector );
    IUS_ASSERT( N > 0 );

    for ( i = 0; i < N; i++ )
    {
        pFloatVector[i] = 0.5f - 0.5f * cosf( 2.0f * IUS_PI * (i + 1) / (N + 1) );
    }
}

//------------------------------------------------------------------------------
// Copy all data in a vector to another vector with the same dimensions
//------------------------------------------------------------------------------
void iusCopyFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length
)
{
    IUS_ASSERT( pSrc  && pDest && (pSrc != (const float *)pDest) );

    memcpy( pDest, pSrc, length * sizeof( float ) );
}

void iusCopyIntVector
(
    int *      pDest,
    const int *pSrc,
    int        length
)
{
    IUS_ASSERT( pSrc  && pDest && (pSrc != (const int *)pDest) );

    memcpy( pDest, pSrc, length * sizeof( int ) );
}

void iusCopyShortVector
(
    short *      pDest,
    const short *pSrc,
    int          length
)
{
    IUS_ASSERT( pSrc  && pDest && (pSrc != (const short *)pDest) );

    memcpy( pDest, pSrc, length * sizeof( short ) );
}

void iusCopyShortVector2FloatVector
(
    float *      pDest,
    const short *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pSrc && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (float)pSrc[i];
    }
}

void iusCopyByteVector2DoubleVector
(
    double *             pDest,
    const unsigned char *pSrc,
    int                  length
)
{
    int i;

    IUS_ASSERT( pSrc && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (double)pSrc[i];
    }
}

void iusCopyShortVector2DoubleVector
(
    double *     pDest,
    const short *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pSrc && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (double)pSrc[i];
    }
}

void iusCopyShortVector2IntVector
(
    int *        pDest,
    const short *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pSrc  && pDest  );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (int)pSrc[i];
    }
}

void iusCopyDoubleVector2FloatVector
(
    float *       pDest,
    const double *pSrc,
    int           length
)
{
    int i;

    IUS_ASSERT( pSrc  && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (float)pSrc[i];
    }
}

void iusCopyDoubleVector2IntVector
(
    int *         pDest,
    const double *pSrc,
    int           length
)
{
    int i;

    IUS_ASSERT( pSrc && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (int)pSrc[i];
    }
}

void iusCopyDoubleVector2ShortVector
(
    short *       pDest,
    const double *pSrc,
    int           length
)
{
    int i;

    IUS_ASSERT( pSrc  && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (short)pSrc[i];
    }
}

void iusCopyDoubleVector2ByteVector
(
    unsigned char *pDest,
    const double * pSrc,
    int            length
)
{
    int i;

    IUS_ASSERT( pSrc && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (unsigned char)pSrc[i];
    }
}

void iusCopyFloatVector2DoubleVector
(
    double *     pDest,
    const float *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pSrc && pDest );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (double)pSrc[i];
    }
}

void iusCopyFloatVector2ShortVector
(
    short *      pDest,
    const float *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pSrc && pDest );

    for ( i = 0; i < length; i++ )
    {
        IUS_ASSERT_VALUE( pSrc[i] >= -32768.0f );
        IUS_ASSERT_VALUE( pSrc[i] <=  32767.0f );
        pDest[i] = (short)pSrc[i];
    }
}

//------------------------------------------------------------------------------
// Flip vector
//------------------------------------------------------------------------------
void iusFlipFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length
)
{
    int c1;

    IUS_ASSERT( pSrc && pDest && (pSrc != (const float *)pDest) );

    for ( c1 = 0; c1 < length; c1++ )
    {
        pDest[c1] = pSrc[length - 1 - c1];
    }
}

//------------------------------------------------------------------------------
// Element-wise multiplication / addition / subtraction of two vectors
//------------------------------------------------------------------------------
void iusMultiplyFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc1 && pSrc2 );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = pSrc1[i] * pSrc2[i];
    }
}

void iusAddFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc1 && pSrc2 );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = pSrc1[i] + pSrc2[i];
    }
}

void iusAccFloatVectors
(
          float *pDest,
    const float *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc ) ;

    for ( i = 0; i < length; i++ )
    {
        pDest[i] += pSrc[i];
    }
}

void iusSubtractFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc1 && pSrc2 );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = pSrc1[i] - pSrc2[i];
    }
}

//------------------------------------------------------------------------------
// Inner product of two vectors
//------------------------------------------------------------------------------
void iusInnerProductFloatVectors
(
    float *      pInnerProductValue,
    const float *pSrc1,
    const float *pSrc2,
    int          length
)
{
    int i;

    IUS_ASSERT( pInnerProductValue && pSrc1 && pSrc2 );

    *pInnerProductValue = 0.0f;
    for ( i = 0; i < length; i++ )
    {
        *pInnerProductValue += pSrc1[i] * pSrc2[i];
    }
}

void iusScaleFloatVector
(
    float *      pDest,
    const float *pSrc,
    float        factor,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest&& pSrc );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = pSrc[i] * factor;
    }
}

//------------------------------------------------------------------------------
// Divide all elements of a vector by the same factor
//------------------------------------------------------------------------------
void iusDivideFloatVector
(
    float *      pDest,
    const float *pSrc,
    float        factor,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc );
    IUS_ASSERT( factor != 0.0f );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = pSrc[i] / factor;
    }
}

//------------------------------------------------------------------------------
// Fader between two input vectors
//------------------------------------------------------------------------------
void iusFadeFloatVectors
(
    float *      pDest,
    const float *pSrc1,
    const float *pSrc2,
    float        factor,
    int          length
)
{
    int i;
    float factor2 = 1.0f - factor;

    IUS_ASSERT( pDest && pSrc1 && pSrc2 );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = factor * pSrc1[i] + factor2 * pSrc2[i];
    }
}

//------------------------------------------------------------------------------
// Compute the mean value of the vector elements
//------------------------------------------------------------------------------
float iusComputeFloatVectorMean
(
    const float *pFloatVector,
    int          length
)
{
    int i;
    float mean = 0.0f;

    IUS_ASSERT( pFloatVector );

    for ( i = 0; i < length; i++ )
    {
        mean += pFloatVector[i];
    }
    mean = mean / (float)length;

    return mean;
}

//------------------------------------------------------------------------------
// Compute the variance of the vector elements
//------------------------------------------------------------------------------
float iusComputeFloatVectorVariance
(
    const float *pFloatVector,
    int          length
)
{
    int i;
    float mean;
    float var = 0.0f;

    IUS_ASSERT( pFloatVector );

    mean = iusComputeFloatVectorMean( pFloatVector, length );
    for ( i = 0; i < length; i++ )
    {
        var += (pFloatVector[i] - mean) * (pFloatVector[i] - mean);
    }
    var = var / (float)length;

    return var;
}

#if 0

//------------------------------------------------------------------------------
// Print all data in a vector to a stream (like stdout or an ASCII file)
//------------------------------------------------------------------------------
void iusFPrintFFloatVector
(
    FILE *       fp,
    const float *pFloatVector,
    int          length
)
{
    int i;

    IUS_ASSERT( pFloatVector );

    for ( i = 0; i < length; i++ )
    {
        fprintf( fp, "%9.2f\n", pFloatVector[i] );
    }
}

void iusFPrintFIntVector
(
    FILE *     fp,
    const int *pIntVector,
    int        length
)
{
    int i;

    IUS_ASSERT( pIntVector );

    for ( i = 0; i < length; i++ )
    {
        fprintf( fp, "%10d\n", pIntVector[i] );
    }
}

void iusFPrintFShortVector
(
    FILE *       fp,
    const short *pShortVector,
    int          length
)
{
    int i;

    IUS_ASSERT( pShortVector );

    for ( i = 0; i < length; i++ )
    {
        fprintf( fp, "%5d\n", pShortVector[i] );
    }
}

#endif

//------------------------------------------------------------------------------
// Take the absolute value for each input vector element.
//------------------------------------------------------------------------------
void iusAbsFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = (float)fabsf( pSrc[i] );
    }
}

void iusAbsShortVector
(
    short *      pDest,
    const short *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = pSrc[i] > 0 ? pSrc[i] : -pSrc[i];
    }
}

//------------------------------------------------------------------------------
// Take the absolute values after applying a [1 -1] filter operation.
//------------------------------------------------------------------------------
void iusAbsDiffFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc && ((const float *)pDest != pSrc) );

    pDest[0] = (float)fabs( pSrc[0] );
    for ( i = 1; i < length; i++ )
    {
        pDest[i] = (float)fabs( pSrc[i] - pSrc[i-1] );
    }
}

//------------------------------------------------------------------------------
// Median filter operation on a float vector.
//------------------------------------------------------------------------------
void iusMedianFilterFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    int          windowWidth
)
{
    int i, j;
    float *pWindow;
    float *pMedian;
    float *pDataIn;
    float *pDataOut;
    int bufferSize;

    IUS_ASSERT( pSrc && pDest );
    IUS_ASSERT( windowWidth % 2 == 1 );  // windowWidth must be odd

    bufferSize = length;

    pDataIn  = (float *)pSrc;
    pDataOut = pDest;

    pWindow = (float *)calloc( windowWidth + bufferSize, sizeof( float ) );
    IUS_ASSERT( pWindow );
    pMedian = pWindow + windowWidth;

    for ( i = -(windowWidth / 2); i < 0; i++ )
    {
        // input data before pDataIn buffer needed. Extend symmetrically
        for ( j = 0; j < windowWidth; j++ )
        {
            if ( j < -i )
            {
                pWindow[j] = pDataIn[j];
            }
            else
            {
                pWindow[j] = pDataIn[i + j];
            }
        }

        *pMedian++ = LF_GetMedianFromWindow( pWindow, windowWidth );
    }

    for ( i = 0; i <= bufferSize - windowWidth; i++ )
    {
        // all data available in pDataIn buffer
        for ( j = 0; j < windowWidth; j++ )
        {
            pWindow[j] = pDataIn[i + j];
        }

        *pMedian++ = LF_GetMedianFromWindow( pWindow, windowWidth ); 
    }

    for ( i = bufferSize-windowWidth+1; i < bufferSize - (windowWidth/2); i++ )
    {
        // input data after pDataIn buffer needed. Extend symmetrically
        for ( j = 0; j < windowWidth; j++ )
        {
            if ( j < bufferSize - i )
            {
                pWindow[j] = pDataIn[i + j];
            }
            else
            {
                pWindow[j] = pDataIn[bufferSize - 1 - j];
            }
        }
 
        *pMedian++ = LF_GetMedianFromWindow( pWindow, windowWidth ); 
    }

    // write remaining entries
    pMedian = pWindow + windowWidth;
    for ( i = 0; i < bufferSize; i++ )
    {
        *pDataOut++ = *pMedian++;
    }

    free( pWindow );
}

//------------------------------------------------------------------------------
// Clip the elements of a vector to a lower and an upper bound
//------------------------------------------------------------------------------
void iusClipLoHiFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    float        minVal,
    float        maxVal
)
{
    int i;

    IUS_ASSERT( pDest && pSrc );
    IUS_ASSERT( maxVal >= minVal );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = ( pSrc[i] < minVal ) ? minVal : ( pSrc[i] > maxVal ) ? maxVal : pSrc[i ];
    }
}

//------------------------------------------------------------------------------
// Clip the elements of a vector to a lower and an upper bound, cast to short
//------------------------------------------------------------------------------
void iusQuantizeFloatVector2ShortVector
(
    short *      pDest,
    const float *pSrc,
    int          length,
    float        minVal,
    float        maxVal
)
{
    int i;
    float range, factor;

    IUS_ASSERT( pDest && pSrc );
    IUS_ASSERT( maxVal > minVal );

    //FIXME bugs

    range = maxVal - minVal;
    factor = (float)65535 / range;

    for ( i = 0; i < length; i++ )
    {
        if ( pSrc[i] < minVal )
        {
            pDest[i] = (short)-32768;
        }
        else if ( pSrc[i] > maxVal )
        {
            pDest[i] = (short)32767;
        }
        else
        {
            pDest[i] = (short)(pSrc[i] * factor);
        }
    }
}

//------------------------------------------------------------------------------
// Finds the maximum value of a vector
//------------------------------------------------------------------------------
float iusMaxFloatVector
(
    const float *pSrc,
    int          length
)
{
    float val;
    int i;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    val = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] > val )
        {
            val = pSrc[i];
        }
    }

    return val;
}

int iusMaxIntVector
(
    const int *pSrc,
    int        length
)
{
    int val;
    int i;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    val = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] > val )
        {
            val = pSrc[i];
        }
    }

    return val;
}

short iusMaxShortVector
(
    const short *pSrc,
    int          length
)
{
    short val;
    int i;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    val = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] > val )
        {
            val = pSrc[i];
        }
    }

    return val;
}


//------------------------------------------------------------------------------
// Element-by-element maximum value of two input vectors
//------------------------------------------------------------------------------
void iusElementByElementMaxFloatVectors
(
    float *const       pDest,
    const float *const pSrc1,
    const float *const pSrc2,
    int                length
)
{
    int i;
    IUS_ASSERT( pDest && pSrc1 && pSrc2 );
    IUS_ASSERT_VALUE( length > 0 );

    for ( i = 0; i < length; i++ )
    {
        pDest[i] = IUS_MAX( pSrc1[i], pSrc2[i] );
    }
}


//------------------------------------------------------------------------------
// Finds the minimum value of a vector
//------------------------------------------------------------------------------
float iusMinFloatVector
(
    const float *pSrc,
    int          length
)
{
    float val;
    int i;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    val = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] < val )
        {
            val = pSrc[i];
        }
    }

    return val;
}

int iusMinIntVector
(
    const int *pSrc,
    int        length
)
{
    int val;
    int i;

    IUS_ASSERT( pSrc  );
    IUS_ASSERT( length > 0 );

    val = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] < val )
        {
            val = pSrc[i];
        }
    }

    return val;
}

short iusMinShortVector
(
    const short *pSrc,
    int          length
)
{
    short val;
    int i;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    val = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] < val )
        {
            val = pSrc[i];
        }
    }

    return val;
}

//------------------------------------------------------------------------------
// Finds the maximum value of a vector and its index
//------------------------------------------------------------------------------
float iusMaxIndexFloatVector
(
    const float *pSrc,
    const int    length,
    int *        pIndex
)
{
    float val;
    int x, i;

    IUS_ASSERT( pSrc && pIndex );
    IUS_ASSERT( length > 0 );

    x = 0;
    val = pSrc[x];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] > val )
        {
            x = i;
            val = pSrc[x];
        }
    }

    *pIndex = x;
    return val;
}

//------------------------------------------------------------------------------
// Finds the minimum value of a vector and its index
//------------------------------------------------------------------------------
float iusMinIndexFloatVector
(
    const float *pSrc,
    const int    length,
    int *        pIndex
)
{
    float val;
    int x, i;

    IUS_ASSERT( pSrc && pIndex );
    IUS_ASSERT( length > 0 );

    x = 0;
    val = pSrc[x];
    for ( i = 1; i < length; i++ )
    {
        if ( pSrc[i] < val )
        {
            x   = i;
            val = pSrc[x];
        }
    }

    *pIndex = x;
    return val;
}

//------------------------------------------------------------------------------
// Counts the number of unique values of (int) vector elements
//------------------------------------------------------------------------------
int iusNumUniqueValuesIntVector
(
    const int *pSrc,
    const int  length
)
{
    int outer, inner;
    int unique = 1;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    for ( outer = 1; outer < length; ++outer )
    {
        int is_unique = 1;
        for ( inner = 0; is_unique && inner < outer; ++inner )
        {
            if ( pSrc[inner] == pSrc[outer] )
            {
                is_unique = 0;
            }
        }
        if ( is_unique )
        {
            ++unique;
        }
    }
    return unique;
}

//------------------------------------------------------------------------------
// Finds the first index counting upwards from 0 for which holds that the
// corresponding vector element exceeds a given threshold value
//------------------------------------------------------------------------------
float iusFirstIndexAboveThresholdFloatVector
(
    const float *pSrc,
    const int    length,
    const float  threshold,
    int *        pIndex
)
{
    int i;

    IUS_ASSERT( pSrc  && pIndex );
    IUS_ASSERT( length > 0 );

    i = 0;
    while ( i < length && pSrc[i] <= threshold )
    {
        i++;
    }

    *pIndex = i;
    return pSrc[i];
}

//------------------------------------------------------------------------------
// Finds the first local peak in an array above a fixed threshold value counting
// upwards from array index 0.
//------------------------------------------------------------------------------
int iusFirstPeakAboveThresholdFloatVector
(
    const float *pSrc,
    const int    length,
    const float  threshold,
    int *        pIndex,
    float *      pPeakValue
)
{
    int i;
    int success = 0;

    IUS_ASSERT( pSrc  && pIndex );
    IUS_ASSERT( length > 0 );

    *pPeakValue = 0.0f;
    *pIndex = 0;

    i = 1;
    while ( i < length && pSrc[i] <= threshold )
    {
        i++;
    }
    while ( i < length && pSrc[i] > pSrc[i-1] )
    {
        i++;
    }
    if ( i < length )
    {
        *pPeakValue = pSrc[i-1];
        *pIndex     = i-1;
        success     = 1;
    }

    return success;
}

//------------------------------------------------------------------------------
// Sum the values of the elements of a vector
//------------------------------------------------------------------------------
float iusSumElementsFloatVector
(
    const float *pSrc,
    int          length
)
{
    int i;
    float val;

    IUS_ASSERT( pSrc );
    IUS_ASSERT(length > 0);

    val = 0.0f;
    for ( i = 0; i < length; i++ )
    {
        val += pSrc[i];
    }

    return val;
}

//------------------------------------------------------------------------------
// Sum the absolute values of the elements of a vector
//------------------------------------------------------------------------------
float iusSumAbsElementsFloatVector
(
    const float *pSrc,
    int          length
)
{
    int i;
    float val;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    val = 0.0f;
    for ( i = 0; i < length; i++ )
    {
        val += IUS_ABS( pSrc[i] );
    }

    return val;
}

//------------------------------------------------------------------------------
// Sum the squared values of the elements of a vector
//------------------------------------------------------------------------------
float iusSumSquaredElementsFloatVector
(
    const float *pSrc,
    int          length
)
{
    int i;
    float val;

    IUS_ASSERT( pSrc );
    IUS_ASSERT( length > 0 );

    val = 0.0f;
    for ( i = 0; i < length; i++ )
    {
        val += pSrc[i] * pSrc[i];
    }

    return val;
}

//------------------------------------------------------------------------------
// From an input vector pass on the values belonging to the positive peaks,
// characterized by pSrc[i] being larger than a threshold value and being larger
// than its two neighbors pSrc[i-1] and pSrc[i+1].
//------------------------------------------------------------------------------
#if 0
disabled due to unittest fail
void iusPassPeaksFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    float        threshold
)
{
    int i;

    IUS_ASSERT( pDest && pSrc && ((const float *)pDest != pSrc) );
    IUS_ASSERT( length > 0 );

    pDest[0] = 0.0f;
    for ( i = 1; i < length - 1; i++ )
    {
        pDest[i] = pSrc[i];
        if ( (pDest[i] <= pSrc[i-1]) || (pDest[i] <= pSrc[i+1]) || pDest[i] < threshold )
        {
            pDest[i] = 0.0f;
        }
    }
    pDest[length - 1] = pSrc[length - 1];
}
#endif

//------------------------------------------------------------------------------
// QuickSortFloatVectorSegment
//
// Sorts the elements of a vector segment between two indices.
//------------------------------------------------------------------------------
void iusQuickSortFloatVectorSegment
(
    float *pIO,
    int    length,
    int    firstIndex,
    int    lastIndex
)
{
    int first = firstIndex;
    int last  = lastIndex;
    float middleValue = pIO[(firstIndex + lastIndex) / 2];
 
    IUS_ASSERT( pIO && (firstIndex < length) && (lastIndex < length)
        && (firstIndex <= lastIndex) );

    do 
    {
        while ( pIO[first] < middleValue ) 
        {
            first++;
        }
        while ( middleValue < pIO[last] ) 
        {
            last--;
        }
 
        if ( first <= last ) 
        {
            float temp = pIO[first];
            pIO[first] = pIO[last];
            pIO[last] = temp;
            first++;
            last--;
        }
    } while ( first <= last );
 
    if ( firstIndex < last ) 
    {
        iusQuickSortFloatVectorSegment( pIO, length, firstIndex, last );
    }
    if ( first < lastIndex ) 
    {
        iusQuickSortFloatVectorSegment( pIO, length, first, lastIndex );
    }
}

//------------------------------------------------------------------------------
// Direct form II transposed zero phase FIR filter
// (based from Ofi's basic_fcs.c, rev. 3002)
//------------------------------------------------------------------------------
void iusZeroPhaseFirFilterFloatVector
(
    float *      pDest,
    const float *pSrc,
    int          length,
    const float *pCoefs, // must be symmetrical with odd number of elements!
    int          nTaps
)
{
    int     coefLen2, count, i, j;
    float *pFrstInput; // one past first input, requires pre-decrement in loops
    float *pLastInput;
    float *pCoefStart;
    float *pOut;

    IUS_ASSERT( pDest && pSrc && pCoefs );
    IUS_ASSERT( (const float *)pDest != pSrc ); // in-place not allowed
    IUS_ASSERT( nTaps % 2 == 1 ); // number of filter coefs must be odd-valued

    coefLen2   = (nTaps+1)/2;
    pFrstInput = (float *)pSrc + coefLen2; // one past first input, requires pre-decrement in loops
    pLastInput = (float *)pSrc;
    pCoefStart = (float *)pCoefs;
    pOut       = pDest;
    count      = coefLen2;

    // convolution
    for ( i = 0; i < length - (coefLen2 - 1); i++ )
    {
        const float *pData  = pFrstInput;
        const float *pData2 = pLastInput;
        const float *pCoef  = pCoefStart;

        float accu = 0;

        if ( count <= coefLen2 ) // fade-in
        {
            for ( j = 0; j < count; j++ )
            {
                accu += (*pCoef++) * (*--pData);
            }
        }
        else // optimising
        {
            const int nSgl = nTaps - count;

            for ( j = 0; j < nSgl; j++ )
            {
                accu += (*pCoef++) * (*--pData);
            }
            for ( j = nSgl; j < coefLen2-1; j++ )
            {
                accu += (*pCoef++) * ((*--pData) + (*pData2++));
            }
            accu += (*pCoef++) * (*--pData);
        }
        *pOut++ = accu;

        if ( count < nTaps ) // fade-in
        {
            count++;
        }
        else
        {
            pLastInput++;
        }

        if ( i < length - (coefLen2 - 1) - 1 ) // this check slows down the code but prevents pFrsInput to point out of array bounds after final loop iteration (DevPartner Studio would complain)
        {
            pFrstInput++;
        }
    }
    for ( ; i < length; i++ )
    {
        *pOut++ = 0;
    }
}

//------------------------------------------------------------------------------
// Half Band FIR filter
//------------------------------------------------------------------------------
void iusHalfBandFirFilterFloatVector
(
          float *outbuf,
    const float *inbuf,
    const int    N,
    const float *coef,
    const int    nrtaps
)
{
    const int del = nrtaps >> 1; // acausal delay on input to get 'zerophase'
    int c1, c2, c3;

    IUS_ASSERT( outbuf != inbuf ); // not in-place

    IUS_ASSERT( nrtaps & 1 ); // must be odd
    IUS_ASSERT( fabs( coef[ 0 ] ) > 1e-6f ); // must have nonzero first tap

    //--------------------------------------------------------------------------
    // run-in: assume zeros before first sample.
    //--------------------------------------------------------------------------
    IUS_ASSERT( del <= N - nrtaps );
    for ( c1 = 0; c1 < del; c1++ )
    {
        float out = 0.0f;
        if ( ( del - c1 ) & 1 )
        {
            for ( c3 = del - c1 + 1, c2 = 1; c3 < nrtaps; c2 += 2, c3 += 2 )
            {
                IUS_ASSERT( c2 < N );
                /* forloop: c3 < nrtaps */
                out += inbuf[ c2 ] * coef[ c3 ];
            }
        }
        else
        {
            for ( c3 = del - c1, c2 = 0; c3 < nrtaps; c2 += 2, c3 += 2 )
            {
                IUS_ASSERT( c2 < N );
                /* forloop: c3 < nrtaps */
                out += inbuf[ c2 ] * coef[ c3 ];
            }
        }
        outbuf[c1] = out;
    }

    //--------------------------------------------------------------------------
    // main loop, without edge cases
    //--------------------------------------------------------------------------
    for ( c1 = del; c1 < N - nrtaps + del; c1++ ) // tbd: can run c1 one further..
    {
        float out = 0.0f;
        for ( c3 = 0, c2 = c1 - del; c3 < nrtaps; c2 += 2, c3 += 2 )
        {
            IUS_ASSERT( c2 < N );
            /* forloop: c3 < nrtaps */
            out += inbuf[c2] * coef[c3];
        }
        outbuf[c1] = out;
    }

    //--------------------------------------------------------------------------
    // run-out: assume zeros after last sample.
    //--------------------------------------------------------------------------
    for ( c1 = N - nrtaps + del; c1 < N; c1++ )
    {
        float out = 0.0f;
        for ( c3 = 0, c2 = c1 - del; c2 < N; c2 += 2, c3 += 2 )
        {
            /* forloop: c2 */
            IUS_ASSERT( c3 < nrtaps );
            out += inbuf[c2] * coef[c3];
        }
        outbuf[c1] = out;
    }
}

//------------------------------------------------------------------------------
// Basic direct form II transposed zero phase FIR filter with output decimation
// (based from Ofi's basic_fcs.c, rev. 3002)
//------------------------------------------------------------------------------
void iusZeroPhaseFirFilterAndDecimateFloatVector
(
    float *      pDest,
    int          lengthDest,
    const float *pSrc,
    int          lengthSrc,
    const float *pCoefs, // must be symmetrical with odd number of elements!
    int          nTaps,
    int          decimationStep
)
{
    int     length, coefLen2, count, i, j;
    float *pFrstInput;
    float *pLastInput;
    float *pCoefStart;
    float *pOut;

    IUS_ASSERT( pDest && pSrc && pCoefs );
    IUS_ASSERT( (const float *)pDest != pSrc ); // in-place not allowed
    IUS_ASSERT( lengthDest == ((lengthSrc + decimationStep - 1) / decimationStep) );
    IUS_ASSERT( nTaps % 2 == 1 ); // number of filter coefs must be odd-valued

    length     = lengthSrc;
    coefLen2   = (nTaps + 1) / 2;
    pFrstInput = (float *)pSrc + coefLen2 - 1;
    pLastInput = (float *)pSrc;
    pCoefStart = (float *)pCoefs;
    pOut       = pDest;
    count      = coefLen2;

    // convolution
    for ( i = 0; i < length - (coefLen2 - 1); i++ )
    {
        // Start pData out of iusAllocated buffer. pData is pre-decremented before use.
        // This avoids DevPartner error :
        // As a result of pointer arithmetic, the pointer is out of range; no longer within block, iusAllocated by calloc.
        const float *pData  = pFrstInput + 1;
        const float *pData2 = pLastInput;
        const float *pCoef  = pCoefStart;

        if ( i % decimationStep == 0 )
        {
            float accu = 0;

            if ( count <= coefLen2 ) // fade-in
            {
                for ( j = 0; j < count; j++ )
                {
                    accu += (*pCoef++) * (*--pData);
                }
            }
            else // optimising
            {
                const int nSgl = nTaps - count;

                for ( j = 0; j < nSgl; j++ )
                {
                    accu += (*pCoef++) * (*--pData);
                }
                for ( j = nSgl; j < coefLen2-1; j++ )
                {
                    accu += (*pCoef++) * ((*--pData) + (*pData2++));
                }
                accu += (*pCoef++) * (*--pData);
            }
            *pOut++ = accu;
        }
        if ( count < nTaps ) // fade-in
        {
            count++;
        }
        else
        {
            pLastInput++;
        }
        pFrstInput++;
    }
    for ( i = length - (coefLen2 - 1); i < length; i++ )
    {
        if ( i % decimationStep == 0 )
        {
            *pOut++ = 0;
        }
    }
}

//------------------------------------------------------------------------------
// Cumulative sum
//------------------------------------------------------------------------------
void iusCumSumFloatVector
(
    float *      pDestCumSum,
    const float *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pSrc && pDestCumSum );
    IUS_ASSERT( length > 0 );

    pDestCumSum[0] = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        pDestCumSum[i] = pDestCumSum[i-1] + pSrc[i];
    }
}

//------------------------------------------------------------------------------
// Cumulative sum and cumulative sum of squares
//------------------------------------------------------------------------------
void iusCumSumAndSquaredFloatVector
(
    float *      pDestCumSum,
    float *      pDestCumSumSquared,
    const float *pSrc,
    int          length
)
{
    int i;

    IUS_ASSERT( pSrc && pDestCumSum && pDestCumSumSquared );
    IUS_ASSERT( ((const float *)pDestCumSum != pSrc) );
    IUS_ASSERT( length > 0 );

    pDestCumSum[0]        = pSrc[0];
    pDestCumSumSquared[0] = pSrc[0] * pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        pDestCumSum[i]        = pDestCumSum[i-1] + pSrc[i];
        pDestCumSumSquared[i] = pDestCumSumSquared[i-1] + pSrc[i] * pSrc[i];
    }
}

//------------------------------------------------------------------------------
// Like MatLab's stdfilt().
//------------------------------------------------------------------------------
#if 0
disabled due to unit test fail
void iusStdFilterFloatVector
(
    float *      pDest,
    float *      pTmp1,
    float *      pTmp2,
    const float *pSrc,
    int          length,
    int          stdSize
)
{
    int i, N;
    float s1, s2;
    float fac1, fac2;

    IUS_ASSERT( pSrc && pDest && (pDest != pTmp1) && (pDest != pTmp2) );
    IUS_ASSERT( stdSize % 2 != 0 ); // stdSize must be odd-valued
    IUS_ASSERT( stdSize >= 3 );
    IUS_ASSERT( length > (stdSize - 1) / 2 );

    iusCumSumAndSquaredFloatVector( pTmp1, pTmp2, pSrc, length );

    N    = (stdSize - 1) / 2;
    fac1 = 1.0f / stdSize;
    fac2 = 1.0f / (stdSize - 1);
    for ( i = 0; i < N + 1; i++ )
    {
        // run-in
        s1 = pTmp1[i + N] - pTmp1[0]; // sum of values of pSrc in aperture
        s2 = pTmp2[i + N] - pTmp2[0]; // sum of squared values of pSrc in aperture

        pDest[i] = (float)sqrt( fac2 * (s2 - fac1 * s1 * s1) );
    }
    for ( i = N + 1; i < length - N; i++ )
    {
        s1 = pTmp1[i + N] - pTmp1[i - N - 1]; // sum of values of pSrc in aperture
        s2 = pTmp2[i + N] - pTmp2[i - N - 1]; // sum of squared values of pSrc in aperture

        pDest[i] = (float)sqrt( fac2 * (s2 - fac1 * s1 * s1) );
    }
    for ( i = length - N; i < length; i++ )
    {
        s1 = pTmp1[length - 1] - pTmp1[i - N - 1]; // sum of values of pSrc in aperture
        s2 = pTmp2[length - 1] - pTmp2[i - N - 1]; // sum of squared values of pSrc in aperture

        pDest[i] = (float)sqrt( fac2 * (s2 - fac1 * s1 * s1) );
    }
}
#endif

//------------------------------------------------------------------------------
// zero phase filtering by a [-1 -1 ... -1 0 1 ... 1 1] filter having odd length
// "kernelSize".
//------------------------------------------------------------------------------
void iusEdgeFilterFloatVector
(
    float *      pDest,
    float *      pTmp,
    const float *pSrc,
    int          length,
    int          kernelSize
)
{
    int   i, N;
    float s1, s2;

    IUS_ASSERT( pSrc && pDest && (pDest != pTmp) );
    IUS_ASSERT( kernelSize % 2 != 0 ); // stdSize must be odd-valued
    IUS_ASSERT( kernelSize >= 3 );
    IUS_ASSERT( length > (kernelSize - 1) / 2 );

    iusCumSumFloatVector( pTmp, pSrc, length );

    N = (kernelSize - 1) / 2;
    pDest[0] = pSrc[0];
    for ( i = 1; i < N + 1; i++ )
    {
        // run-in
        s1 = pTmp[  i - 1] - pTmp[0    ];
        s2 = pTmp[2*i - 1] - pTmp[i    ];

        pDest[i] = s1 - s2;
    }
    for ( i = N + 1; i < length - N - 1; i++ )
    {
        s1 = pTmp[i - 1] - pTmp[i - N - 1]; // sum of values of pSrc in aperture
        s2 = pTmp[i + N] - pTmp[i        ]; // sum of values of pSrc in aperture

        pDest[i] = s1 - s2;
    }
    for ( i = length - N - 1; i < length - 1; i++ )
    {
        // run out
        s1 = pTmp[     i - 1] - pTmp[2*i + 1 - length];
        s2 = pTmp[length - 2] - pTmp[i               ];

        pDest[i] = s1 - s2;
    }
    pDest[length-1] = pSrc[length-1];
}

//------------------------------------------------------------------------------
// Interpolate linearly between points (x,y), where x is an integer index and y
// is a float value, to a float vector of length nPoints. The indices x must be
// strictly increasing: pX[0] < pX[1] < ... < pX[nPoints-1].
//------------------------------------------------------------------------------
void iusLinearInterpolatePoints2FloatVector
(
    float *      pDest,
    int          length,
    const int *  pX,
    const float *pY,
    int          nPoints
)
{
    int i, n;
    float *pOut = pDest;

    IUS_ASSERT( pDest && pX  && pY && ((const float *)pDest != pY) );
    IUS_ASSERT( pX[nPoints-1] < length );

    // cover output points with index lower than pX[0]
    for ( i = 0; i < pX[0]; i++ )
    {
        *pOut++ = pY[0];
    }
    // cover output points with index between pX[0] and pX[nPoints-1]
    for ( n = 0; n < nPoints - 1; n++ )
    {
        float slope = (pY[n+1] - pY[n]) / (pX[n+1] - pX[n]);
        for ( ; i <= pX[n+1]; i++ )
        {
            *pOut++ = pY[n] + slope * (i - pX[n]);
        }
    }
    // cover output points with index beyond pX[npoints-1]
    for ( ; i < length; i++ )
    {
        *pOut++ = pY[nPoints - 1];
    }
}

//------------------------------------------------------------------------------
// Fetches an interpolated value from a vector at a non-integer index location.
//------------------------------------------------------------------------------
float iusFetchLinearFloatVector
(
    const float *const pIn,
    int                n1,
    float              l1
)
{
    int   i1, i1p1;
    float f1, n, s, outVal;

    IUS_ASSERT( pIn );
    IUS_ASSERT( (l1 <= (float)(n1 - 1)) && (l1 >= 0.0f) );

    /* Get integer and fractional part of the non-integer idx       */
    i1   = (int)l1;
    i1p1 = (i1 < n1 - 1) ? i1 + 1 : i1;        /* protect array bound */
    f1   = l1 - (float)i1;

    /* Fetch four surrounding values */
    n = pIn[i1];
    s = pIn[i1p1];

    /* Interpolate output value */
    outVal = n * (1.0f - f1) + s * f1;

    return outVal;
}

//------------------------------------------------------------------------------
// Smooth a float vector with a rectangular sliding window
// TODO: speed optimize
//------------------------------------------------------------------------------
void iusRectangularSmoothFloatVector
( 
    float *      pDest,
    const float *pSrc,
    int          length,
    int          span
)
{
    int i, j;
    int w = (span - 1) / 2;
    int low, upp, n;
    float *pOut = pDest;

    IUS_ASSERT( pDest && pSrc && (pSrc != (const float *)pDest) );
    IUS_ASSERT( span % 2 == 1 ); // span must be odd-valued

    for ( i = 0; i < length; i++ )
    {
        float sum = 0;
        if ( i < w )
        {
            low = 0;
            upp = 2 * i + 1;
            n  = (upp - low);
        }
        else if ( i >= length - w )
        {
            low = i - (length - 1 - i);
            upp = length;
            n  = (upp - low);
        }
        else
        {
            low = i - w;
            upp = i + w + 1;
            n   = span;
        }
        for ( j = low; j < upp; j++ )
        {
            sum += pSrc[j];
        }
        *pOut++ = sum / n; 
    }
}

//------------------------------------------------------------------------------
// Interpolates a float vector 
//------------------------------------------------------------------------------
void iusInterpolateFloatVector
(
          float *rr,
    const float *r,
    const int    N,
    const int    L
)
{
    const float *const __restrict b = L == 8 ? s_FIR_interp_8 : NULL;
    int c1, c2, c3;

    IUS_ASSERT( L == 1 || L == 8 );
    /*assert( L == 1 || L == 2 || L == 4 || L == 8 || L == 16 || L == 32 );*/
    IUS_ASSERT( r != rr );

    c2 = 0;
    rr[c2 * L] = r[c2];

    c2 = 1;
    for ( c1 = 1; c1 < L; c1++ )
    {
        float t = 0.0f;
        for ( c3 = 0; c3 < 3; c3++ )
        {
            t += b[c1 - 1 + c3 * L] * r[c2 + 1 - c3];
        }
        rr[c1] = t;
    }
    rr[c2 * L] = r[c2];

    for ( c2 = 2; c2 + 1 < N; c2++ )
    {
        for ( c1 = 1; c1 < L; c1++ )
        {
            float t = 0.0f;
            for ( c3 = 0; c3 < 4; c3++ )
            {
                t += b[c1 - 1 + c3 * L] * r[c2 + 1 - c3];
            }
            rr[L * c2 - L + c1] = t;
        }
        rr[L * c2 ] = r[ c2];
    }

    c2 = N - 1;
    for ( c1 = 1; c1 < L; c1++ )
    {
        float t = 0.0f;
        for ( c3 = 1; c3 < 4; c3++ )
        {
            t += b[c1 - 1 + c3 * L] * r[c2 + 1 - c3];
        }
        rr[L * c2 - L + c1] = t;
    }
    rr[L * c2] = r[c2];

    c2 = N;
    for ( c1 = 1; c1 < L; c1++ )
    {
        float t = 0.0f;
        for ( c3 = 2; c3 < 4; c3++ )
        {
            t += b[c1 - 1 + c3 * L] * r[c2 + 1 - c3];
        }
        rr[L * c2 - L + c1] = t;
    }
}

//------------------------------------------------------------------------------
// Resample a source vector to a destination vector. Usampling by a zero-order
// hold, downsampling by skipping input values.
//------------------------------------------------------------------------------
void iusReSampleFloatVector
(
    float *      pDest,
    int          lengthDest,
    const float *pSrc,
    int          lengthSrc
)
{
    int i, j;
    float ratio;

    IUS_ASSERT( pDest && pSrc && ((const float *)pDest != pSrc) );
    IUS_ASSERT( lengthDest != 0 );

    if ( lengthDest == lengthSrc )
    {
        iusCopyFloatVector( pDest, pSrc, lengthSrc );
    }
    else
    {
        ratio = (float)lengthSrc / lengthDest;
        for ( i = 0; i < lengthDest; i++ )
        {
            j = (int)(ratio * i);
            pDest[i] = pSrc[j];
        }
    }
}

void iusReSampleIntVector
(
    int *        pDest,
    int          lengthDest,
    const int *  pSrc,
    int          lengthSrc
)
{
    int i, j;
    float ratio;

    IUS_ASSERT( pDest && pSrc && ((const int *)pDest != pSrc) );
    IUS_ASSERT( lengthDest != 0 );

    if ( lengthDest == lengthSrc )
    {
        iusCopyIntVector( pDest, pSrc, lengthSrc );
    }
    else
    {
        ratio = (float)lengthSrc / lengthDest;
        for (i = 0; i < lengthDest; i++)
        {
            j = (int)(ratio * i);
            pDest[i] = pSrc[j];
        }
    }
}

void iusReSampleShortVector
(
    short *      pDest,
    int          lengthDest,
    const short *pSrc,
    int          lengthSrc
)
{
    int i, j;
    float ratio;

    IUS_ASSERT( pDest && pSrc && ((const short *)pDest != pSrc) );
    IUS_ASSERT( lengthDest != 0 );

    if ( lengthDest == lengthSrc )
    {
        iusCopyShortVector( pDest, pSrc, lengthSrc );
    }
    else
    {
        ratio = (float)lengthSrc / lengthDest;
        for ( i = 0; i < lengthDest; i++ )
        {
            j = (int)(ratio * i);
            pDest[i] = pSrc[j];
        }
    }
}

//------------------------------------------------------------------------------
// Resample a source vector to a destination vector. Usampling by a linear
// interpolation.
//------------------------------------------------------------------------------
void iusReSampleLinearInterpFloatVector
(
    float *      pDest,
    int          lengthDest,
    const float *pSrc,
    int          lengthSrc
)
{
    int   i, iy;
    float y, fy;
    float ratio;

    IUS_ASSERT( pDest && pSrc && ((const float *)pDest != pSrc) );
    IUS_ASSERT( lengthDest != 0 );

    if (lengthDest == lengthSrc)
    {
        iusCopyFloatVector( pDest, pSrc, lengthSrc );
    }
    else
    {
        ratio = (float)lengthSrc / lengthDest;
        for ( i = 0; i < lengthDest; i++ )
        {
             y = ratio * (float)i; // floating point position in source
            iy = (int)y;           // integer part of floating point position in source
            fy = y - (float)iy;    // fractional part of floating point position in source

            iy = IUS_MIN( iy, lengthSrc - 2 ); // make sure iy+1 never exceeds lengthSrc-1

            pDest[i] = (1.0f - fy) * pSrc[iy] + fy * pSrc[iy+1];
        }
    }
}

//------------------------------------------------------------------------------
// Resample a binary source vector to a binary destination vector. Upsampling
// by a zero-order hold. Downsampling by skipping input values while preventing
// a 0->1 transition to vanish.
//------------------------------------------------------------------------------
void iusReSampleBinaryShortVector
(
    short *      pDest,
    int          lengthDest,
    const short *pSrc,
    int          lengthSrc
)
{
    int i, j;

    IUS_ASSERT( pDest && pSrc && ((const short *)pDest != pSrc) );

    if ( lengthDest > lengthSrc )
    {
        // output longer than input: zero-order hold
        float ratio = (float)lengthSrc / lengthDest;
        for ( i = 0; i < lengthDest; i++ )
        {
            j = (int)(ratio * i);
            pDest[i] = pSrc[j];
        }
    }
    else
    {
        // output shorter than input: drop but keep the binary "1"s
        float ratio = (float)lengthDest / lengthSrc;
        iusZeroShortVector( pDest, lengthDest );
        for ( i = 0; i < lengthSrc; i++ )
        {
            if ( pSrc[i] )
            {
                j = (int)(ratio * i);
                pDest[j] = pSrc[i];
            }
        }
    }
}

//------------------------------------------------------------------------------
// Halve the length of pulses in a binary vector (a pulse is a sequence of
// non-zero values). When the pulse length N is odd, the resulting pulse has
// length (N+1)/2.
//------------------------------------------------------------------------------
void iusHalvePulsesBinaryShortVector
(
    short *      pDest,
    const short *pSrc,
    int          length
)
{
    int i = 0;
    int j, n, n2u, n2d;

    IUS_ASSERT( pDest && pSrc );

    if ( pDest != pSrc )
    {
        iusCopyShortVector( pDest, pSrc, length );
    }
    while ( i < length )
    {
        j = i;
        n = 0;
        while ( (j < length) && pDest[j++] ) // ensured to stop, because pDest[length-1]==0
        {
            n++; // measure the length n of a pulse
        }
        if ( n == 0 )
        {
            i++;
        }
        else
        {
            n2u = (n + 1) / 2;  // integer division by 2 with rounding up, will be 0 when pulse length is 0
            n2d = n / 2;        // integer division by 2 with rounding down, will also be 0 when pulse length is 0
            i += n2u;           // skip first half of the pulse
            for ( j = 0; j < n2d; j++ )
            {
                pDest[i++] = 0; // set to zero second half of the pulse
            }
        }
    }
}


//------------------------------------------------------------------------------
// Counts the number of unique elements in an unsorted array. 
//------------------------------------------------------------------------------
int iusCountUniqueElementsIntVector
(
    const int *const pSrc,
    int              length
)
{
    int i, j;
    int count = 0;

    for ( i = 0; i < length; i++ )
    {
        for ( j = 0; j <= i; j++ )
        {
            if ( pSrc[i] == pSrc[j] )
            {
                break;
            }
        }
        if ( i == j )
        {
            count++;
        }
    }

    return count;
}



//------------------------------------------------------------------------------
// A binary vector contains 0 and 1 only. A positive pulse is a sequence of n
// values of 1. The result of differentiation of the positive pulse is a
// pulse of the same length of which the first half is 1 and the second half
// is -1. When the pulse length N is odd, the output pulse has length (N+1)/2.
//------------------------------------------------------------------------------
void iusDifferentiatePulsesBinaryShortVector
(
    short *      pDest,
    const short *pSrc,
    int          length
)
{
    int i = 0;
    int j, n, n2u, n2d;

    IUS_ASSERT( pDest && pSrc );

    if ( pDest != pSrc )
    {
        iusCopyShortVector( pDest, pSrc, length );
    }
    while ( i < length )
    {
        j = i;
        n = 0;
        while ( (j < length) && (pDest[j++]==1) ) // ensured to stop, because pDest[length-1]==0
        {
            n++; // measure the length n of a pulse consisting of n consecutive 1's.
        }
        if ( n == 0 )
        {
            i++;
        }
        else
        {
            n2u = (n + 1) / 2;  // integer division by 2 with rounding up, will be 0 when pulse length is 0
            n2d = n / 2;        // integer division by 2 with rounding down, will also be 0 when pulse length is 0
            i += n2u;           // skip first half of the pulse
            for ( j = 0; j < n2d; j++ )
            {
                pDest[i++] = -1; // set to zero second half of the pulse
            }
        }
    }
}

//------------------------------------------------------------------------------
// Steepen edges in a vector at places where a pulse goes from 0 to 1 to -1 to 0.
//
// First is run through elements of a source vector and a binary mask vector
// starting from index 0. Where the mask is "1" the destination vector element
// is copied from the previous destination element, otherwise from the source.
// Next a reverse run is done starting at the end index. Where the mask is "-1"
// the destination vector element is overwritten with the value of the
// destination element from the previous iteration.
//------------------------------------------------------------------------------
void iusEdgeSteepenOnMaskFloatVector
(
    float *      pDest,
    const float *pSrc,
    const short *pMask,
    int          length
)
{
    int i;

    IUS_ASSERT( pDest && pSrc && pMask );

    pDest[0] = pSrc[0];
    for ( i = 1; i < length; i++ )
    {
        pDest[i] = (pMask[i] == (short)1) ? pDest[i-1] : pSrc[i];
    }
    for ( i = length - 2; i >= 0; i-- )
    {
        if (pMask[i] == (short)-1)
        {
            pDest[i] = pDest[i+1];
        }
        //pDest[i] = (pMask[i] == (short)-1) ? pDest[i+1] : pDest[i];
    }
}

//------------------------------------------------------------------------------
// Print all elements of a vector to stdout
//------------------------------------------------------------------------------
void iusPrintFloatVector
(
    const float *const pVector,
    int                n1
)
{
    int i;

    for ( i = 0; i < n1; i++ )
    {
        fprintf( stdout, "\t%.4f", pVector[i] );
    }
    fprintf( stdout, "\n" );
}

void iusPrintDoubleVector
(
    const double *const pVector,
    int                 n1
)
{
    int i;

    for ( i = 0; i < n1; i++ )
    {
        fprintf( stdout, "\t%.4lf", pVector[i] );
    }
    fprintf( stdout, "\n" );
}

void iusPrint2FileShortVector
(
    const char *       pFileName,
    const short *const pIn,
    int                length
)
{
    int i;
    FILE *fp = fopen( pFileName, "w" );
    for ( i = 0; i < length; i++ )
    {
        fprintf( fp, "%d\n", pIn[i] );
    }
    fclose(fp);
}

void iusPrint2FileFloatVector
(
    const char *       pFileName,
    const float *const pIn,
    int                length
)
{
    int i;
    FILE *fp = fopen( pFileName, "w" );
    for ( i = 0; i < length; i++ )
    {
        fprintf( fp, "%f\n", pIn[i] );
    }
    fclose(fp);
}

//=============================================================================
// S T A T I C   F U N C T I O N S
//=============================================================================

//------------------------------------------------------------------------------
// The median of a number of float values
//------------------------------------------------------------------------------
static float LF_GetMedianFromWindow
(
    float window[],
    int windowWidth
)
{
    long low, high;
    long median;
    long middle, ll, hh;

    low = 0;
    high = windowWidth - 1;
    median = (low + high) / 2;
    for ( ; ; )
    {
        /* One element only */
        if ( high <= low )
        {
            return window[median];
        }

        /* Two elements only */
        if ( high == low + 1 )
        {
            if ( window[low] > window[high] )
            {
                SWAP(window[low], window[high]);
            }
            return window[median];
        }

        /* Find median of low, middle and high items; swap to low position */
        middle = (low + high) / 2;
        if ( window[middle] > window[high] )
        {
            SWAP( window[middle], window[high] );
        }
        if ( window[low] > window[high] )
        {
            SWAP( window[low], window[high] );
        }
        if ( window[middle] > window[low] )
        {
            SWAP( window[middle], window[low] );
        }

        /* Swap low item (now in position middle) into position (low + 1) */
        SWAP( window[middle], window[low + 1] );

        /* Work from each end towards middle, swapping items when stuck */
        ll = low + 1;
        hh = high;
        for ( ; ; )
        {
            do
            {
                ll++;
            }
            while ( window[low] > window[ll] );
            do
            {
                hh--;
            }
            while ( window[hh] > window[low] );

            if ( hh < ll )
            {
                break;
            }

            SWAP( window[ll], window[hh] );
        }

        /* Swap middle item (in position low) back into correct position */
        SWAP( window[low], window[hh] );

        /* Reset active partition */
        if ( hh <= median )
        {
            low = ll;
        }
        if ( hh >= median )
        {
            high = hh - 1;
        }
    }
}
