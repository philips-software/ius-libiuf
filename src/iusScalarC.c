//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMatrixC.c
//                  Basic functions for complex matrix memory and operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusScalarC.h"
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


iusComplexFloat iusAddComplexFloats
(
    const iusComplexFloat src1,
    const iusComplexFloat src2
)
{
    iusComplexFloat sum;
    sum.re = src1.re + src2.re;
    sum.im = src1.im + src2.im;
    return sum;
}

iusComplexFloat iusSubtractComplexFloats
(
    const iusComplexFloat src1,
    const iusComplexFloat src2
)
{
    iusComplexFloat diff;
    diff.re = src1.re - src2.re;
    diff.im = src1.im - src2.im;
    return diff;
}

iusComplexFloat iusMultiplyComplexFloats
(
    const iusComplexFloat src1,
    const iusComplexFloat src2
)
{
    // (a+jb) * (c+jd) = ac - bd + j(bc + ad)
    iusComplexFloat prod;
    prod.re = src1.re * src2.re - src1.im * src2.im;
    prod.im = src1.re * src2.im + src1.im * src2.re;
    return prod;
}

iusComplexFloat iusScaleComplexFloat
(
    const iusComplexFloat src,
    const float scaleFactor
)
{
    // (a+jb) * (c+jd) = ac - bd + j(bc + ad)
    iusComplexFloat prod;
    prod.re = scaleFactor * src.re;
    prod.im = scaleFactor * src.im;
    return prod;
}

float iusPowerComplexFloat
(
    const iusComplexFloat src
)
{
    // conj( a ) * a
    return src.re * src.re + src.im * src.im;
}

float iusArgumentComplexFloat
(
    const iusComplexFloat src
)
{
    return atan2f( src.im, src.re );
}

float iusAbsoluteComplexFloat
(
    const iusComplexFloat src
)
{
    return sqrtf( iusPowerComplexFloat( src ) );
}

iusComplexFloat iusReciprocalComplexFloat
(
    const iusComplexFloat src
)
{
    // 1/(a+jb) = a / (a^2+b^2) - j*b / (a^2+b^2): scale by 1/power & conjugate
    return iusConjugateComplexFloat( iusScaleComplexFloat( src, 1.0f / iusPowerComplexFloat( src ) ) );
}

iusComplexFloat iusConjugateComplexFloat
(
    const iusComplexFloat src
)
{
    iusComplexFloat conj;
    conj.re = src.re;
    conj.im = -src.im;
    return conj;
}

// Double functions: to be corrected

iusComplexDouble iusAddComplexDoubles
(
    const iusComplexDouble src1,
    const iusComplexDouble src2
)
{
    iusComplexDouble sum;
    sum.re = src1.re + src2.re;
    sum.im = src1.im + src2.im;
    return sum;
}

iusComplexDouble iusSubtractComplexDoubles
(
    const iusComplexDouble src1,
    const iusComplexDouble src2
)
{
    iusComplexDouble diff;
    diff.re = src1.re - src2.re;
    diff.im = src1.im - src2.im;
    return diff;
}

iusComplexDouble iusMultiplyComplexDoubles
(
    const iusComplexDouble src1,
    const iusComplexDouble src2
)
{
    iusComplexDouble prod;
    // (a+jb) * (c+jd) = ac - bd + j(bc + ad)
    prod.re = src1.re * src2.re - src1.im * src2.im;
    prod.im = src1.re * src2.im + src1.im * src2.re;
    return prod;
}

iusComplexDouble iusScaleComplexDouble
(
    const iusComplexDouble src,
    const double scaleFactor
)
{
    // (a+jb) * (c+jd) = ac - bd + j(bc + ad)
    iusComplexDouble prod;
    prod.re = scaleFactor * src.re;
    prod.im = scaleFactor * src.im;
    return prod;
}

double iusPowerComplexDouble
(
    const iusComplexDouble src
)
{
    // conj( a ) * a
    return src.re * src.re + src.im * src.im;
}

double iusArgumentComplexDouble
(
    const iusComplexDouble src
)
{
    return atan2( src.im, src.re );
}

double iusAbsoluteComplexDouble
(
    const iusComplexDouble src
)
{
    return sqrt( iusPowerComplexDouble( src ) );
}


iusComplexDouble iusReciprocalComplexDouble
(
    const iusComplexDouble src
)
{
    // 1/(a+jb) = a / (a^2+b^2) - j*b / (a^2+b^2): scale by 1/power & conjugate
    return iusConjugateComplexDouble( iusScaleComplexDouble( src, 1.0f / iusPowerComplexDouble( src ) ) );
}

iusComplexDouble iusConjugateComplexDouble
(
    const iusComplexDouble src
)
{
    iusComplexDouble conj;
    conj.re = src.re;
    conj.im = -src.im;
    return conj;
}

//------------------------------------------------------------------------------
// Print all elements of a complex scalar to stdout
//------------------------------------------------------------------------------
void iusPrintComplexDouble
(
    const iusComplexDouble ComplexDouble
)
{
    fprintf( stdout, "\t%8.4lf + %8.4lfi\n", ComplexDouble.re, ComplexDouble.im );
}

void iusPrintComplexFloat
(
    const iusComplexFloat ComplexFloat
)
{
    fprintf( stdout, "\t%8.4lf + %8.4lfi\n", ComplexFloat.re, ComplexFloat.im );
}
