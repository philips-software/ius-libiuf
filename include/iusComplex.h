//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusComplex.h
//                  definition of complex-valued float type.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file 
 * Definition of complex-valued float type.
 */

#ifndef _IUSCOMPLEX_H
#define _IUSCOMPLEX_H


#ifdef __cplusplus
extern "C" {
#endif

//! Complex double type has double real and double imaginary part.
typedef struct
{
    double* pRe;
    double* pIm;
} iusComplexDoubleVector;

typedef struct
{
    double re;
    double im;
} iusComplexDouble;

// iusComplexDouble_t to be replaced by iusComplexDouble
typedef struct
{
    double re;
    double im;
} iusComplexDouble_t;

//! Complex float type has float real and float imaginary part.
typedef struct
{
    float* pRe;
    float* pIm;
} iusComplexFloatVector;

typedef struct
{
    float** ppRe;
    float** ppIm;
} iusComplexFloatMatrix;

typedef struct
{
    float re;
    float im;
} iusComplexFloat;

//! Complex int type has int real and int imaginary part.
typedef struct
{
    int* pRe;
    int* pIm;
} iusComplexIntVector;

typedef struct
{
    int re;
    int im;
} iusComplexInt_t;

//! Complex short type has short real and short imaginary part.
typedef struct
{
    short* pRe;
    short* pIm;
} iusComplexShortVector;

typedef struct
{
    short im;
    short re;
} iusComplexShort_t;

/*============================================================================*/
/* Function prototypes.                                                       */
/*============================================================================*/

/*============================================================================*/
/* Close when C++ protected.                                                  */
/*============================================================================*/
#ifdef __cplusplus
}
#endif

/*============================================================================*/
/* End of multiple inclusion protection.                                      */
/*============================================================================*/
#endif
