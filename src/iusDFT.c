//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : DFT.c
//                  Unoptimized DFT.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/*============================================================================*
 *                                                                            *
 * A straightforward DFT implementation.                                      *
 *                                                                            *
 * Reference    :                                                             *
 *                                                                            *
 *============================================================================*/

#include "iusDFT.h"
#include "iusVector.h"
#include "iusError.h"
#include <math.h>   // for cos,sin
#include <stdlib.h> // malloc

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// L O C A L   D E F I N E S   A N D   V A R I A B L E S
//==============================================================================
#define LC_PI  3.14159265358979323846

#define LD_MPYRE(a,b) ((a).re * (b).re - (a).im * (b).im)
#define LD_MPYIM(a,b) ((a).re * (b).im + (a).im * (b).re)

// Memory reservation for the twiddle factors, and the bit-reversal
static iusComplexFloat* fwd_twi = NULL;
static iusComplexFloat* rev_twi = NULL;
static size_t LV_N = 0;

//==============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//==============================================================================

//------------------------------------------------------------------------------
// twiddle
//------------------------------------------------------------------------------
static void LF_twid
(
    const size_t N
);

//==============================================================================
// E X P O R T E D   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// initialize Discrete Fourier Transform
//------------------------------------------------------------------------------
void iusInitDFT
(
    const size_t N
)
{
    IUS_ASSERT_VALUE( ( N & 1 ) == 0 ); /* check N is even */
    fwd_twi = (iusComplexFloat*)malloc( N / 2 * sizeof( iusComplexFloat ) );
    rev_twi = (iusComplexFloat*)malloc( N / 2 * sizeof( iusComplexFloat ) );
    LF_twid( N );
    LV_N = N;
}

//------------------------------------------------------------------------------
// Discrete Fourier Transform
//------------------------------------------------------------------------------
void iusDFT
(
    const iusComplexFloat* const in,
          iusComplexFloat* const out,
    const size_t N
)
{
    size_t c1, c2, k, N2;

    IUS_ASSERT_VALUE( N == LV_N );
    IUS_ASSERT( in != out );

    /*------------------------------------------------------------------------*/
    /* loop                                                                   */
    /*------------------------------------------------------------------------*/
    N2 = N >> 1;
    for ( c1 = 0; c1 < N; c1++ )
    {
        float acc_re = 0.0f;
        float acc_im = 0.0f;
        const iusComplexFloat* q = in;
        for ( c2 = 0; c2 < N; c2++ )
        {
            k = ( c1 * c2 ) % N;
            if( k >= N2 )
            {
                acc_re -= LD_MPYRE( fwd_twi[k - N2], *q );
                acc_im -= LD_MPYIM( fwd_twi[k - N2], *q );
            }
            else
            {
                acc_re += LD_MPYRE( fwd_twi[k], *q );
                acc_im += LD_MPYIM( fwd_twi[k], *q );
            }
            q++;
        }
        out[c1].re = acc_re;
        out[c1].im = acc_im;
    }
}

//------------------------------------------------------------------------------
// Inverse Discrete Fourier Transform
//------------------------------------------------------------------------------
void iusIDFT
(
    const iusComplexFloat* const in,
          iusComplexFloat* const out,
    const size_t N
)
{
    const float scale = (float)1 / (float)N;
    size_t c1, c2, k, N2;

	IUS_ASSERT_VALUE( N == LV_N );
	IUS_ASSERT( in != out );

    /*------------------------------------------------------------------------*/
    /* loop                                                                   */
    /*------------------------------------------------------------------------*/
    N2 = N >> 1;
    for ( c1 = 0; c1 < N; c1++ )
    {
        float acc_re = 0.0f;
        float acc_im = 0.0f;
        const iusComplexFloat* q = in;
        for ( c2 = 0; c2 < N; c2++ )
        {
            k = ( c1 * c2 ) % N;
            if( k >= N2 )
            {
                acc_re -= LD_MPYRE( rev_twi[k - N2], *q );
                acc_im -= LD_MPYIM( rev_twi[k - N2], *q );
            }
            else
            {
                acc_re += LD_MPYRE( rev_twi[k], *q );
                acc_im += LD_MPYIM( rev_twi[k], *q );
            }
            q++;
        }
        out[c1].re = acc_re;
        out[c1].im = acc_im;
    }

    /*------------------------------------------------------------------------*/
    /* scale output                                                           */
    /*------------------------------------------------------------------------*/
    for ( c1 = 0; c1 < N; c1++ )
    {
        out[c1].re *= scale;
        out[c1].im *= scale;
    }
}

/*============================================================================*/
/* Local function implementations.                                            */
/*============================================================================*/

//------------------------------------------------------------------------------
// twiddle talbe
//------------------------------------------------------------------------------
static void LF_twid
(
    const size_t N
)
{
    const size_t N2 = N >> 1;
    const double f = 2.0 * LC_PI / (double)N;
    double c, s;
    size_t c1;

    /*------------------------------------------------------------------------*/
    /* twiddle factors for complex dft                                        */
    /*------------------------------------------------------------------------*/
    for ( c1 = 0; c1 < N2; c1++ )
    {
        c = cos( f * (double)c1 );
        s = sin( f * (double)c1 );
        fwd_twi[c1].re = (float) c;
        fwd_twi[c1].im = (float)-s; /* conj */
        rev_twi[c1].re = (float) c;
        rev_twi[c1].im = (float) s; /* conj conj */
    }
}

#ifdef __cplusplus
}
#endif
