//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMedianFilter.c
//                  1D median filter
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusMedianFilter.h"
#include "iusVector.h"
#include "iusError.h"
#include <stdlib.h> // for malloc,..
#include <string.h> // for memcpy,..

//=============================================================================
// L O C A L   D E F I N E S
//=============================================================================

//=============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//=============================================================================

//=============================================================================
// E X P O R T E D   F U N C T I O N S
//=============================================================================

//------------------------------------------------------------------------------
// Creates a 1D median filter instance with the specified parameters.
//------------------------------------------------------------------------------
IUSMedianFilterInstance * iusMedianFilterCreate
(
    int inputBufferSize,
    int windowWidth
)
{
    IUSMedianFilterInstance * pInst;

    IUS_ASSERT_VALUE( (inputBufferSize > 0) && (windowWidth > 0) );
    IUS_ASSERT_VALUE( windowWidth % 2 == 1 ); // windowWidth must be odd

    pInst = (IUSMedianFilterInstance *)malloc( sizeof( IUSMedianFilterInstance) );

    pInst->pInputData = iusAllocFloatVector( inputBufferSize * windowWidth );

    // iusAllocate extra byte (+1) to prevent out of buffer arithmetic warning
    pInst->pWorkBuffer = iusAllocFloatVector( windowWidth + 1 );

    pInst->inputBufferSize = inputBufferSize;
    pInst->windowWidth     = windowWidth;

    return( pInst );
}

//------------------------------------------------------------------------------
// Runs the 1D median filter on a vector
//------------------------------------------------------------------------------
void iusMedianFilterRun
(
    IUSMedianFilterInstance * pInst,
    float * const             pDest,
    const float * const       pSrc,
    int                       length
)
{
    int     i, j;
    int     insertIndex = 0;
    int     removeIndex = 0;
    int     removeStartIndex;
    int     insertStartIndex;
    float * pInsertValue;
    float * pRemoveValue;
    float * pInputData      = pInst->pInputData;
    float * pWorkBuffer     = pInst->pWorkBuffer;
    int     windowWidth     = pInst->windowWidth;
    int     mid             = windowWidth / 2;
    int     inputBufferSize = pInst->inputBufferSize;

    IUS_ASSERT( (pDest != NULL) && (pSrc != NULL) );
    IUS_ASSERT_VALUE( length == inputBufferSize );
    IUS_UNUSED(length); // prevent compile warning
    //IUS_ASSERT_VALUE (((int) pInst->inputBufferSize % 16) == 0);

    // pInputData layout (size = inputBufferSize * windowWidth) :
    // 1. walkin data          (size = windowWidth / 2)
    // 2. input buffer data    (size = inputBufferSize)
    // 3. walkout data         (size = windowWidth / 2)

    // 1. store walkin data (input data before pSrc buffer needed. Extend symmetrically)
    for ( i = 0, j = mid - 1; i < mid; i++, j-- )
    {
        pInputData[i] = pSrc[j];
    }

    // 2. store input buffer data (pSrc)
    memcpy( pInputData + mid, pSrc, inputBufferSize * sizeof(float) );

    // 3. store walkout data (input data after pSrc buffer needed. Extend symmetrically)
    for ( i = mid + inputBufferSize, j = inputBufferSize - 1; i < (mid + inputBufferSize + mid); i++, j-- )
    {
        pInputData[i] = pSrc[j];
    }

    // Store sorted median window data in workBuffer
    memcpy( pWorkBuffer, pInputData, windowWidth * sizeof(float) );
    iusQuickSortFloatVectorSegment( pWorkBuffer, windowWidth, 0, windowWidth - 1 );

    // store first median in output buffer (pDest)
    pDest[0] = pWorkBuffer[mid];

    pInsertValue = &pInputData[windowWidth];
    pRemoveValue = &pInputData[0];

    for ( i = 1; i < inputBufferSize; i++ )
    {
        removeStartIndex = (*pRemoveValue > pWorkBuffer[mid - 1]) ? mid : 0;
        insertStartIndex = (*pInsertValue < pWorkBuffer[mid + 1]) ? mid : windowWidth - 1;

        for ( removeIndex = removeStartIndex; removeIndex < windowWidth; removeIndex++ )
        {
            if (pWorkBuffer[removeIndex] == *pRemoveValue)
            {
                break;
            }
        }

        for ( insertIndex = insertStartIndex; insertIndex >= 0; insertIndex-- )
        {
            if ( *pInsertValue >= pWorkBuffer[insertIndex] )
            {
                break;
            }
        }

        if ( insertIndex < removeIndex )
        {
            memmove( &pWorkBuffer[insertIndex + 2], &pWorkBuffer[insertIndex + 1], (removeIndex - insertIndex - 1) * sizeof(float) );
            pWorkBuffer[insertIndex + 1] = *pInsertValue;
        }
        else
        {
            memmove( &pWorkBuffer[removeIndex], &pWorkBuffer[removeIndex + 1], (insertIndex - removeIndex) * sizeof(float) );
            pWorkBuffer[insertIndex] = *pInsertValue;
        }

        pDest[i] = pWorkBuffer[mid];

        pRemoveValue++;
        pInsertValue++;
    }
}

//------------------------------------------------------------------------------
// Destroys the median filter instance
//------------------------------------------------------------------------------
void iusMedianFilterDestroy
(
    IUSMedianFilterInstance * pInst
)
{
    IUS_ASSERT( pInst != NULL );

    // free members
    iusFreeFloatVector( pInst->pInputData );
    iusFreeFloatVector( pInst->pWorkBuffer );

    // Free instance
    free( pInst );
}
