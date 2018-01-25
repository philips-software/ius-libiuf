//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusLUT.c
//                  Look-up tables
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#include "iusLUT.h"
#include "iusVector.h"
#include "iusError.h"
#include <stdlib.h>           // for malloc, free

//==============================================================================
// L O C A L   D E F I N E S   A N D   V A R I A B L E S
//==============================================================================

//==============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//==============================================================================

//==============================================================================
// E X P O R T E D   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// Allocate the memory for a FloatIntervalLUT look-up table
//------------------------------------------------------------------------------
iusFloatIntervalLUT * iusAllocFloatIntervalLUT
(
    int length
)
{
    iusFloatIntervalLUT * pLUT = NULL;

    IUS_ASSERT_VALUE( length > 1 );

    if ((pLUT = malloc(sizeof(iusFloatIntervalLUT))) != NULL)
    {
        if ((pLUT->pX = iusAllocFloatVector(length)) == NULL)
        {
            free(pLUT);
            pLUT = NULL;
        }
        else if ((pLUT->pY = iusAllocFloatVector(length)) == NULL)
        {
            iusFreeFloatVector(pLUT->pX);
            free(pLUT);
            pLUT = NULL;
        }
    }

    pLUT->length = length;

    return pLUT;
}

//------------------------------------------------------------------------------
// Fill the elements of a FloatIntervalLUT look-up table
//------------------------------------------------------------------------------
void iusFillFloatIntervalLUT
(
    iusFloatIntervalLUT * const pLUT,
    const         float * const pX,
    const         float * const pY,
    int                         length
)
{
    int i;
	IUS_ASSERT( pLUT != NULL );
	IUS_ASSERT_VALUE( length == pLUT->length );

    // verify that pX is monotonically increasing
    for (i = 1; i < length - 1; i++)
    {
		IUS_ASSERT( pX[i] > pX[i - 1] );
    }

    iusCopyFloatVector(pLUT->pX, pX, length - 1);
    iusCopyFloatVector(pLUT->pY, pY, length);
}

//------------------------------------------------------------------------------
// Perform a look-up from a FloatIntervalLUT look-up table
//------------------------------------------------------------------------------
float iusLookUpFloatIntervalLUT
(
    const iusFloatIntervalLUT * const pLUT,
    float x
)
{
    int i;
    float y;

	IUS_ASSERT( pLUT != NULL );

    i = pLUT->length - 1;  // index of last LUT entry
    y = pLUT->pY[i];
    while (--i >= 0)
    {
        if (x < pLUT->pX[i])
        {
            y = pLUT->pY[i];
        }
    }
    return y;
}

//------------------------------------------------------------------------------
// Free the memory of a FloatIntervalLUT look-up table
//------------------------------------------------------------------------------
void iusFreeFloatIntervalLUT
(
    iusFloatIntervalLUT * pLUT
)
{
    iusFreeFloatVector(pLUT->pY);
    iusFreeFloatVector(pLUT->pX);
    free(pLUT);
    pLUT = NULL;
}

