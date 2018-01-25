//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : FFT.c
//                  Radix-2 FFT for real-valued signals.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file
 *
 * A radix-2 FFT for real valued signals.
 *
 * Reference    :
 *
 * A fast fourier transform algorithm for real-valued series,
 *   Glenn D. Bergland, Bell Telephone Lab, Whippany, NJ
 *   Communications of the ACM , Volume 11 Issue 10, Oct. 1968
 *
 */

#include "iusFFT.h"
#include "iusVector.h"
#include "iusError.h"
#include <math.h>   // for cos,sin
#include <stdlib.h> // for malloc,free

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// L O C A L   D E F I N E S   A N D   V A R I A B L E S
//==============================================================================

#define LC_PI  3.14159265358979323846

// Memory reservation for the twiddle factors, and the bit-reversal
static iusComplexFloat fftc_twi[IUSFFT_MAXFFTSIZE >> 2];
static size_t LV_N = 0;

//==============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//==============================================================================

static void LF_ftwid
(
    const size_t N
);

static void LF_index
(
    const size_t N,
    size_t * const fbr
);

//==============================================================================
// E X P O R T E D   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// Initialize Fast Fourier Transform
//------------------------------------------------------------------------------
void iusInitFFT
(
    const size_t N
)
{
	IUS_ASSERT_VALUE( N <= IUSFFT_MAXFFTSIZE );
	IUS_ASSERT_VALUE( ( N & ( N - 1 ) ) == 0 ); /* check N is a power of 2 */
    LF_ftwid( N );
    LV_N = N;
}

//------------------------------------------------------------------------------
// Fast Fourier Transform
//------------------------------------------------------------------------------
void iusFFT
(
    float * const io,
    const size_t  N
)
{
    size_t n_bfy;
    size_t c1, c2, c3, c4;

	IUS_ASSERT_VALUE( N <= LV_N );

    //--------------------------------------------------------------------------
    // loop over stages
    //--------------------------------------------------------------------------
    n_bfy = N >> 1;
    while ( n_bfy > 1 )
    {
        const size_t nb2 = n_bfy >> 1;

        //----------------------------------------------------------------------
        // stage ; first group indexes the input differently
        //----------------------------------------------------------------------
        for ( c1 = 0, c2 = n_bfy, c4 = 0; c4 < nb2; c4++, c1++, c2++ )
        {
            const float AR = io[c1];
            const float AI = io[c1 + nb2];
            const float BR = io[c2];
            const float BI = io[c2 + nb2];

            io[c1]       = AR + BR;
            io[c1 + nb2] = AI + BI;
            io[c2]       = AR - BR;
            io[c2 + nb2] = BI - AI; /* conj */
        }
        c1 += nb2 + n_bfy;
        c2 += nb2 + n_bfy;

        //----------------------------------------------------------------------
        // stage ; remaining groups
        //----------------------------------------------------------------------
        for ( c3 = 1; c1 < N; c1 += n_bfy, c2 += n_bfy, c3++ )
        {
            const float WR = fftc_twi[c3].re;
            const float WI = fftc_twi[c3].im;

            for ( c4 = 0; c4 < nb2; c4++, c1++, c2++ )
            {
                const float AR = io[c1];
                const float AI = io[c2];
                const float BR = io[c1 + nb2];
                const float BI = io[c2 + nb2];
                const float TR = WR * BR - WI * BI;
                const float TI = WR * BI + WI * BR;

                io[c1]       = AR + TR;
                io[c1 + nb2] = AI + TI;
                io[c2]       = AR - TR;
                io[c2 + nb2] = TI - AI; /* conj */
            }
            c1 += nb2;
            c2 += nb2;
        }
        n_bfy = nb2;
    }

    {
        float dc = io[0] + io[1];
        float nq = io[0] - io[1];
        io[0] = dc;
        io[1] = nq;
    }
}

//------------------------------------------------------------------------------
// Inverse Fast Fourier Transform
//------------------------------------------------------------------------------
void iusIFFT
(
    float * const io,
    const size_t  N
)
{
    const float  s = 1 / (float)( N >> 1 );
    size_t n_bfy;
    size_t c1, c2, c3, c4;

	IUS_ASSERT_VALUE( N <= LV_N );

    {
        float dc = io[0] + io[1];
        float nq = io[0] - io[1];
        io[0] = dc * (float)0.5;
        io[1] = nq * (float)0.5;
    }

    //--------------------------------------------------------------------------
    // loop over stages
    //--------------------------------------------------------------------------
    n_bfy = (size_t)2;
    while ( n_bfy != N )
    {
        const size_t nb2 = n_bfy >> 1;

        //----------------------------------------------------------------------
        // stage ; first group indexes the output differently
        //----------------------------------------------------------------------
        for ( c1 = 0, c2 = n_bfy, c4 = 0; c4 < nb2; c4++, c1++, c2++ )
        {
            const float AR = io[c1];
            const float AI = io[c1 + nb2];
            const float BR = io[c2];
            const float BI = io[c2 + nb2]; 
            const float XR = AR + BR;
            const float XI = AI - BI; /* conj B */
            const float YR = AR - BR;
            const float YI = AI + BI; /* conj B */

            io[c1]       = XR;
            io[c1 + nb2] = XI;
            io[c2]       = YR;
            io[c2 + nb2] = YI;
        }
        c1 += nb2 + n_bfy;
        c2 += nb2 + n_bfy;

        //----------------------------------------------------------------------
        // stage ; remaining groups
        //----------------------------------------------------------------------
        for ( c3 = 1; c1 < N; c1 += n_bfy, c2 += n_bfy, c3++ )
        {
            const float WR = fftc_twi[c3].re;
            const float WI = fftc_twi[c3].im;

            for ( c4 = 0; c4 < nb2; c4++, c1++, c2++ )
            {
                const float AR = io[c1];
                const float AI = io[c1 + nb2];
                const float BR = io[c2];
                const float BI = io[c2 + nb2]; 
                const float XR = AR + BR;
                const float XI = AI - BI; /* conj B */
                const float YR = AR - BR;
                const float YI = AI + BI; /* conj B */

                const float TR = WR * YR + WI * YI; /* conj W */
                const float TI = WR * YI - WI * YR;

                io[c1]       = XR;
                io[c2]       = XI;
                io[c1 + nb2] = TR;
                io[c2 + nb2] = TI;
            }
            c1 += nb2;
            c2 += nb2;
        }

        n_bfy = n_bfy << 1;
    }

    //--------------------------------------------------------------------------
    // scale
    //--------------------------------------------------------------------------
    for ( c1 = 0; c1 < N; c1++ )
    {
        io[c1] *= s;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusMultiplyFloatFFVectors
(
          float * const __restrict io,
    const float * const __restrict in,
    const size_t                   N
)
{
    size_t c1, c2;

	IUS_ASSERT_VALUE( in != io );

    io[0] = in[0] * io[0];
    io[1] = in[1] * io[1];
    for ( c1 = 2, c2 = 3; c1 < N; c1 += 2, c2 += 2 )
    {
        float t1 = in[c1];
        float t2 = in[c2];
        float t3 = io[c1];
        float t4 = io[c2];
        io[c1] = t1 * t3 - t2 * t4;
        io[c2] = t1 * t4 + t2 * t3;
    }
}

//==============================================================================
// S T A T I C   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
static void LF_index
(
    const size_t N,
    size_t * const fbr
)
{
    size_t L, c1;

    //--------------------------------------------------------------------------
    // init table of length L=1
    //--------------------------------------------------------------------------
    fbr[0] = 0; /* always 0 */
    if ( N == 1 ) return;

    //--------------------------------------------------------------------------
    // init table of length L=2
    //--------------------------------------------------------------------------
    fbr[1] = 1; /* ==L/2 */

    //--------------------------------------------------------------------------
    // recursively increase table size until L == N
    //--------------------------------------------------------------------------
    for ( L = 2; L != N; L <<= 1 )
    {
        //----------------------------------------------------------------------
        // new table even (zero-based) indices is old table
        //----------------------------------------------------------------------
        for ( c1 = L; c1--; )
        {
            fbr[c1 << 1] = fbr[c1];
        }

        //----------------------------------------------------------------------
        // new table odd (zero-based) indices is 2L - old table
        //----------------------------------------------------------------------
        for ( c1 = L; c1--; )
        {
            /* note: fbr[2k+1] = N - fbr[2k] */
            fbr[( c1 << 1 ) + 1] = ( L << 1 ) - fbr[c1 << 1];
        }

        fbr[1] = L;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusFFTsort
(
    const size_t N,
    float * const io
)
{
    const size_t N2 = N >> 1;
    size_t c1;
    float * T1;
    size_t * k;

    //--------------------------------------------------------------------------
    // Note: in-place not possible
    //--------------------------------------------------------------------------
    k = (size_t*)malloc( N2 * sizeof( size_t ) );
	IUS_ASSERT( k != NULL );

    T1 = iusAllocFloatVector( (int)N );

    iusCopyFloatVector( T1, io, (int)N );

    LF_index( N2, k ); /*  N/2 tab  voor complex reordering */

    for ( c1 = 0; c1 < N2; c1++ )
    {
        const size_t br = k[c1];

        io[br*2 + 0] = T1[c1*2 + 0];
        io[br*2 + 1] = T1[c1*2 + 1];
    }

    iusFreeFloatVector( T1 );
    free( k );
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusFFTunsort
(
    const size_t N,
    float * const io
)
{
    const size_t N2 = N >> 1;
    size_t c1;
    float * T1;
    size_t * k;

    //--------------------------------------------------------------------------
    // Note: in-place not possible
    //--------------------------------------------------------------------------
    k = (size_t*)malloc( N2 * sizeof( size_t ) );
	IUS_ASSERT( k != NULL );

    T1 = iusAllocFloatVector( (int)N );

    iusCopyFloatVector( T1, io, (int)N );

    LF_index( N2, k ); /*  N/2 tab  voor complex reordering */

    for ( c1 = 0; c1 < N2; c1++ )
    {
        const size_t br = k[c1];

        io[c1 * 2 + 0] = T1[br * 2 + 0];
        io[c1 * 2 + 1] = T1[br * 2 + 1];
    }

    iusFreeFloatVector( T1 );
    free( k );
}

//------------------------------------------------------------------------------
// Twiddle
//------------------------------------------------------------------------------
static void LF_ftwid
(
    const size_t N
)
{
    const size_t N4 = N >> 2;
    const double f = 2.0 * LC_PI / (double)N;
    double c, s;
    size_t c1;
    size_t * k;

    k = (size_t*)malloc( N4 * sizeof( size_t ) );
	IUS_ASSERT( k != NULL );

    LF_index( N4, k );

    //--------------------------------------------------------------------------
    // twiddle factors for complex fft
    //--------------------------------------------------------------------------
    for ( c1 = 0; c1 < N4; c1++ )
    {
        c = cos( f * (double)( - (int)k[c1] ) );
        s = sin( f * (double)( - (int)k[c1] ) );

        fftc_twi[c1].re = (float)c;
        fftc_twi[c1].im = (float)s;

        /* Note: twiddle at odd index c1 is swapped and negated
           version of preceding twiddle.. (c1>2).
           Could reduce table to N/8 +1 .. */
    }

    free( k );
}

#ifdef __cplusplus
}
#endif
