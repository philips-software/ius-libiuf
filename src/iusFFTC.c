//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : FFTC.c
//                  Radix-2 FFT for complex-valued signals.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusFFTC.h"
#include "iusVectorC.h"
#include "iusBasicFunctions.h"
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
static iusComplexFloat  fftc_twi[IUSFFTC_MAXFFTSIZE >> 1];
static size_t           fftc_fbr[IUSFFTC_MAXFFTSIZE];
static size_t LV_N     = 0;
static size_t LV_log2N = 0;

//==============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//==============================================================================

static void LF_ftwid
(
    const size_t N
);

static void LF_fbr
(
    const size_t N
);

//==============================================================================
// E X P O R T E D   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// Initialize Complex Fast Fourier Transform
//------------------------------------------------------------------------------
void iusInitFFTC
(
    const size_t N
)
{
    IUS_ASSERT_VALUE( N <= IUSFFTC_MAXFFTSIZE );
	IUS_ASSERT_VALUE( ( N & ( N - 1 ) ) == 0 ); /* check N is a power of 2 */
    LF_ftwid( IUSFFTC_MAXFFTSIZE );
    LF_fbr( IUSFFTC_MAXFFTSIZE );
    LV_N = N;
    LV_log2N = iusIntLog2( (int)N );
}

//------------------------------------------------------------------------------
// Complex Fast Fourier Transform
//------------------------------------------------------------------------------
void iusFFTC
(
    iusComplexFloatVector* const pio,
    const size_t  N
)
{
    const size_t step = IUSFFTC_MAXFFTSIZE >> LV_log2N;
    size_t n_bfy;
    size_t c1, c2, c3, c4;

	IUS_ASSERT_VALUE( N <= LV_N );

    //--------------------------------------------------------------------------
    // loop over stages
    //--------------------------------------------------------------------------
    n_bfy = N >> 1;
    while ( n_bfy > 0 )
    {
        //----------------------------------------------------------------------
        // stage
        //----------------------------------------------------------------------
        for ( c1 = 0, c2 = n_bfy, c3 = 0; c1 < N; c1 += n_bfy, c2+=n_bfy, c3++ )
        {
            const float WR = fftc_twi[c3].re;
            const float WI = fftc_twi[c3].im;

            for ( c4 = 0; c4 < n_bfy; c4++, c1++, c2++ )
            {
                const float AR = pio->pRe[c1];
                const float AI = pio->pIm[c1];   
                const float BR = pio->pRe[c2];          
                const float BI = pio->pIm[c2];    
                const float TR = WR * BR + WI * BI;
                const float TI = WR * BI - WI * BR;

                pio->pRe[c1] = AR + TR;
                pio->pIm[c1] = AI + TI;
                pio->pRe[c2] = AR - TR;
                pio->pIm[c2] = AI - TI;
            }
        }
        n_bfy = n_bfy >> 1;
    }

    //--------------------------------------------------------------------------
    // bitrev
    //--------------------------------------------------------------------------
    for ( c1 = 0, c2 = 0; c1 < N; c1++, c2 += step )
    {
        const size_t br = fftc_fbr[c2];

        if ( c1 < br )
        {
/*
            iusComplexFloat A = io[br];
            io[br] = io[c1];
            io[c1] = A;
*/
            float ARe = pio->pRe[br];
            float AIm = pio->pIm[br];
            pio->pRe[br] = pio->pRe[c1];
            pio->pIm[br] = pio->pIm[c1];
            pio->pRe[c1] = ARe;
            pio->pIm[c1] = AIm;
        }
    }
}

//------------------------------------------------------------------------------
// Inverse Complex Fast Fourier Transform
//------------------------------------------------------------------------------
void iusIFFTC
(
    iusComplexFloatVector* const pio,
    const size_t  N
)
{
    const size_t step = IUSFFTC_MAXFFTSIZE >> LV_log2N;
    const float  s = 1 / (float)N;
    size_t n_bfy;
    size_t c1, c2, c3, c4;

    //--------------------------------------------------------------------------
    // loop over stages
    //--------------------------------------------------------------------------
    n_bfy = N >> 1;
    while ( n_bfy > 0 )
    {
        //----------------------------------------------------------------------
        // stage
        //----------------------------------------------------------------------
        for ( c1 = 0, c2 = n_bfy, c3 = 0; c1 < N; c1 += n_bfy, c2+=n_bfy, c3++ )
        {
            const float WR =   fftc_twi[c3].re;
            const float WI = - fftc_twi[c3].im;

            for ( c4 = 0; c4 < n_bfy; c4++, c1++, c2++ )
            {
/*
                const float AR = io[c1].re;
                const float AI = io[c1].im;
                const float BR = io[c2].re;
                const float BI = io[c2].im;

*/
                const float AR = pio->pRe[c1];
                const float AI = pio->pIm[c1];   
                const float BR = pio->pRe[c2];          
                const float BI = pio->pIm[c2];    
                const float TR = WR * BR + WI * BI;
                const float TI = WR * BI - WI * BR;

/*
                io[c1].re = AR + TR;
                io[c1].im = AI + TI;
                io[c2].re = AR - TR;
                io[c2].im = AI - TI;
*/
                pio->pRe[c1] = AR + TR;
                pio->pIm[c1] = AI + TI;
                pio->pRe[c2] = AR - TR;
                pio->pIm[c2] = AI - TI;

            }
        }
        n_bfy = n_bfy >> 1;
    }

    //--------------------------------------------------------------------------
    // scale
    //--------------------------------------------------------------------------
/*
    for ( c1 = 0; c1 < N; c1++ )
    {
        io[c1].re *= s;
        io[c1].im *= s;
    }
*/
    iusScaleComplexFloatVector( pio, pio, s, (int)N );

    //--------------------------------------------------------------------------
    // bitrev
    //--------------------------------------------------------------------------
    for ( c1 = 0, c2 = 0; c1 < N; c1++, c2 += step )
    {
        const size_t br = fftc_fbr[c2];

        if ( c1 < br )
        {
/*
            iusComplexFloat A = io[br];
            io[br] = io[c1];
            io[c1] = A;
*/
            float ARe = pio->pRe[br];
            float AIm = pio->pIm[br];
            pio->pRe[br] = pio->pRe[c1];
            pio->pIm[br] = pio->pIm[c1];
            pio->pRe[c1] = ARe;
            pio->pIm[c1] = AIm;
        }
    }
}

//==============================================================================
// S T A T I C   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// twiddle
//------------------------------------------------------------------------------
static void LF_ftwid
(
    const size_t nop
)
{
    const size_t nop2 = nop >> 1;
    const double f = 2.0 * LC_PI / (double)nop;
    double c, s;
    size_t c1, J, K;

    //--------------------------------------------------------------------------
    // twiddle factors for complex fft
    //--------------------------------------------------------------------------
    J = 1;
    for ( c1 = 0; c1 < nop2; c1++ )
    {
        c = cos( f * (double)( J - 1 ) ) ;
        s = sin( f * (double)( J - 1 ) ) ;

        fftc_twi[c1].re = (float)c;
        fftc_twi[c1].im = (float)s;

        if ( J == nop2 ) break;

        K  =  nop >> 2;
        while ( K < J )
        {
            J = J - K;
            K = K / 2;
        }
        J = J + K;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static void LF_fbr
(
    const size_t nop
)
{
    const size_t fac = IUSFFTC_MAXFFTSIZE / nop;     /* /  ( 4 * nop ); */
    size_t length = fac;
    size_t c1 = nop;
    size_t c3;

    fftc_fbr[0] = 0;
    while ( c1 > 1 )
    {
        c1 = c1 >> 1;
        for ( c3 = 0 ; c3 < length ; c3 += fac )
        {
            fftc_fbr[c3 + length] = fftc_fbr[c3] + c1;
        }
        length *= 2;
    }
}

#ifdef __cplusplus
}
#endif
