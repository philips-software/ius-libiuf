//===========================================================================*/
//     COPYRIGHT 2016 PHILIPS RESEARCH
//===========================================================================*/
//
//  Source Name   : iusLUT.h
//                  Look-up tables
//  iusVersion    : 255.255.255.255
//
//==============================================================================

/**
 * \file iusLUT.h
 * \brief File containing look-up table (LUT) routines.
 *
 * \author Harm Belt
*/

#ifndef _IUSLUT_H
#define _IUSLUT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* \brief Data structure for an interval LUT.
*/
typedef struct
{
    float * pX;        /**< array with floating point x-values */
    float * pY;        /**< array with floating point y-values */
    int     length;    /**< length of both above arrays        */
} iusFloatIntervalLUT;

/**
 * \brief Allocate the memory for an interval LUT.
 *
 * in-place     n.a.
 * \pre         More than 1 entry for the LUT (length > 1)
 * \post        Memory for the LUT has been allocated, a valid
 *              LUT pointer has been returned.
 * \param[in]   length : the number of LUT entries
 * \return      A valid pointer to a LUT upon success (NULL otherwise)
 */
iusFloatIntervalLUT * iusAllocFloatIntervalLUT
(
    int length
);

/**
 * \brief Fill up the LUT with LUT entries
 *
 * in-place     n.a.
 * \pre         pLUT is valid and created by iusAllocFloatIntervalLUT(),
 *              the LUT entry values are valid (pX must be monotonically
 *              increasing), number of entries corresponds with call to
 *              iusAllocFloatIntervalLUT();
 * \post        All entries of the LUT have been filled with values
 * \param[in]   pLUT   : pointer created by iusAllocFloatIntervalLUT()
 * \param[in]   pX     : pointer to a float array of "length" elements
 *                       with monotonically increasing values
 * \param[in]   pY     : pointer to a float array of "length" elements
 * \param[in]   length : amount of LUT entries
 * \return      A valid pointer to a LUT upon success (NULL otherwise)
 */
void iusFillFloatIntervalLUT
(
    iusFloatIntervalLUT * const pLUT,
    const         float * const pX,
    const         float * const pY,
    int                         length
);

/**
 * \brief Look up a number from the interval LUT
 *
 * in-place     n.a.
 * \pre         LUT was filled with values by iusFillFloatIntervalLUT()
 * \post        a LUT output value has been returned
 * \param[in]   pLUT : a valid pointer to a LUT which was filled by
 *                     iusFillFloatIntervalLUT()
 * \param[in]      x : LUT input value
 * \return      LUT output value given by pY[i] where for index i holds
 *                 if i > 0:  pX[i-1] <= x < pX[i]
 *                 if i = 0:  x < pX[i]
 */
float iusLookUpFloatIntervalLUT
(
    const iusFloatIntervalLUT * const pLUT,
    float x
);

/**
* \brief Free the memory for an interval LUT.
*
* in-place     n.a.
* \pre         pLUT is a valid pointer created by iusAllocFloatIntervalLUT()
* \post        Memory for the LUT has been freed, pLUT is NULL
* \param[in]   pLUT : pointer to LUT
* \return      void
*/
void iusFreeFloatIntervalLUT
(
    iusFloatIntervalLUT * pLUT
);

#ifdef __cplusplus
}
#endif

#endif  /* _IUSLUT_H */
