//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusScalarC.h
//                  Basic functions for complex operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/**
\file \brief This file contains prototypes for the complex scalar operations

Functions for addition and subtraction
\li     iusAddComplexFloats, iusAddComplexDoubles
\li     iusSubtractComplexFloats, iusSubtractComplexDoubles

Functions for scaling, multiplication, and reciprocal
\li     iusScaleComplexFloat, iusScaleComplexDouble
\li     iusMultiplyComplexFloats, iusMultiplyComplexDoubles
\li     iusReciprocalComplexFloat, iusReciprocalComplexDouble

Functions for power, absolute value, and argument
\li     iusPowerComplexFloat, iusPowerComplexDouble
\li     iusAbsoluteComplexFloat, iusAbsoluteComplexDouble
\li     iusArgumentComplexFloat, iusArgumentComplexDouble

Functions for printing
\li     iusPrintComplexFloat, iusPrintComplexDouble
*/

#ifndef _IUSSCALARC_H
#define _IUSSCALARC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusComplex.h"
#include <math.h>

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
* \brief Adds two complex floats
* 
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src1: complex float
* \param[in]   src2: complex float
* \return      complex float containing src1 + src2
*/
iusComplexFloat iusAddComplexFloats
(
    const iusComplexFloat src1,
    const iusComplexFloat src2
);

/**
* \brief Subtracts two complex floats
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src1: complex float
* \param[in]   src2: complex float
* \return      complex float containing src1 - src2
*/
iusComplexFloat iusSubtractComplexFloats
(
    const iusComplexFloat src1,
    const iusComplexFloat src2
);

/**
* \brief Multiplies two complex floats
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src1: complex float
* \param[in]   src2: complex float
* \return      complex float containing src1 * src2
*/
iusComplexFloat iusMultiplyComplexFloats
(
    const iusComplexFloat src1,
    const iusComplexFloat src2
);

/**
* \brief Multiplies a complex float with a real float
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex float
* \param[in]   scaleFactor: float
* \return      complex float containing src * scaleFactor
*/
iusComplexFloat iusScaleComplexFloat
(
    const iusComplexFloat src,
    const float scaleFactor
);

/**
* \brief Computes power of complex float
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex float
* \return      float containing squared absolute value of src
*/
float iusPowerComplexFloat
(
    const iusComplexFloat src
);

/**
* \brief Computes argument (angle) of complex float
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex float
* \return      float containing argument (angle) of src: if src=a+jb, then argument = atan2(b,a)
*/
float iusArgumentComplexFloat
(
    const iusComplexFloat src
);

/**
* \brief Computes absolute value of complex float
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex float
* \return      float containing absolute value of src
*/
float iusAbsoluteComplexFloat
(
    const iusComplexFloat src
);

/**
* \brief Computes reciprocal value of complex float
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex float
* \return      complex float containing reciprocal of src (1/src)
*/
iusComplexFloat iusReciprocalComplexFloat
(
    const iusComplexFloat src
);

/**
* \brief Computes complex conjugate of complex float
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex float
* \return      complex float containing complex conjugate of src
*/
iusComplexFloat iusConjugateComplexFloat
(
    const iusComplexFloat src
);


/**
* \brief Adds two complex doubles
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src1: complex double
* \param[in]   src2: complex double
* \return      complex double containing src1 + src2
*/
iusComplexDouble iusAddComplexDoubles
(
    const iusComplexDouble src1,
    const iusComplexDouble src2
);

/**
* \brief Subtracts two complex doubles
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src1: complex double
* \param[in]   src2: complex double
* \return      complex double containing src1 - src2
*/
iusComplexDouble iusSubtractComplexDoubles
(
    const iusComplexDouble src1,
    const iusComplexDouble src2
);

/**
* \brief Multiplies two complex doubles
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src1: complex double
* \param[in]   src2: complex double
* \return      complex double containing src1 * src2
*/
iusComplexDouble iusMultiplyComplexDoubles
(
    const iusComplexDouble src1,
    const iusComplexDouble src2
);

/**
* \brief Multiplies a complex double with a real double
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex double
* \param[in]   scaleFactor: double
* \return      complex double containing src * scaleFactor
*/
iusComplexDouble iusScaleComplexDouble
(
    const iusComplexDouble src,
    const double scaleFactor
);

/**
* \brief Computes power of complex double
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex double
* \return      double containing squared absolute value of src
*/
double iusPowerComplexDouble
(
    const iusComplexDouble src
);

/**
* \brief Computes argument (angle) of complex double
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex double
* \return      double containing argument (angle) of src: if src=a+jb, then argument = atan2(b,a)
*/
double iusArgumentComplexDouble
(
    const iusComplexDouble src
);

/**
* \brief Computes absolute value of complex double
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex double
* \return      double containing absolute value of src
*/
double iusAbsoluteComplexDouble
(
    const iusComplexDouble src
);

/**
* \brief Computes reciprocal value of complex double
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex double
* \return      complex double containing reciprocal of src (1/src)
*/
iusComplexDouble iusReciprocalComplexDouble
(
    const iusComplexDouble src
);

/**
* \brief Computes complex conjugate of complex double
*
* \in-place    n.a. (call-by-value as opposed to call-by-reference)
* \param[in]   src: complex double
* \return      complex double containing complex conjugate of src
*/
iusComplexDouble iusConjugateComplexDouble
(
    const iusComplexDouble src
);

//------------------------------------------------------------------------------
// Print all elements of a complex scalar to stdout
//------------------------------------------------------------------------------
void iusPrintComplexDouble
(
    const iusComplexDouble ComplexDouble
);

void iusPrintComplexFloat
(
    const iusComplexFloat ComplexFloat
);



#ifdef __cplusplus
}
#endif

#endif // _IUSSCALARC_H
