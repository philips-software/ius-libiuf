//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusBasicFunctions.c
//                  Basic functions
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusBasicFunctions.h"
#include "iusVector.h"
#include "iusVector.h"
#include "iusError.h"

#include <stdlib.h> // for srand,..
#include <string.h> // for memcpy,..
#include <math.h>   // for floor,..
#include <float.h>  // for FLT_EPSILON

//=============================================================================
// L O C A L   D E F I N E S
//=============================================================================

//=============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//=============================================================================

//=============================================================================
// E X P O R T E D   F U N C T I O N S
//=============================================================================

//-----------------------------------------------------------------------------
// Sets the seed for the random number generator.
//-----------------------------------------------------------------------------
void iusSetRandomSeed
(
    unsigned int seed
)
{
    srand( seed );
}

//-----------------------------------------------------------------------------
// Returns the input if the input is an integer multiple of 32. Returns the
// next larger multiple of 32 otherwise.
//-----------------------------------------------------------------------------
int iusNextMultipleOf32
(
    int v
)
{
    IUS_ASSERT_VALUE( v >= 0 );

    while( ( v % 32 ) != 0 )
    {
        v++;
    }
    
    return v;
}

//-----------------------------------------------------------------------------
// Returns the input if the input is a power of 2. Otherwise it returns
// the next larger number that is a power of two.
//-----------------------------------------------------------------------------
int iusNextPowerOfTwo
(
    int v
)
{
    IUS_ASSERT_VALUE( v >= 0 );
    IUS_ASSERT_VALUE( v <= 65536 ); //?

    if( v == 0 )
    {
        return 1;
    }

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    
    return v;
}

//-----------------------------------------------------------------------------
// Returns true iff v is an integer power of 2.
//-----------------------------------------------------------------------------
int iusIsPowerOfTwo
(
    int v
)
{
    IUS_ASSERT_VALUE( v >= 0 );

    return ( v & ( v - 1 ) ) == 0;
}

//-----------------------------------------------------------------------------
// Returns log2(a), iff a is an integer power of 2.
//-----------------------------------------------------------------------------
int iusIntLog2
(
    int a
)
{
    int i;

    IUS_ASSERT_VALUE( a > 0 );

    i = (int)0;
    while( ( (int)1 << i ) < a )
    {
        i++;
    }
    
    return i;
}

//-----------------------------------------------------------------------------
// Allocates and initializes an IusGrid
//-----------------------------------------------------------------------------
IusGrid * iusCreateGrid
(
    int dim0,
    int dim1,
    int dim2
)
{
    IusGrid * pGrid = (IusGrid *)malloc( sizeof( IusGrid ) );
    IUS_ASSERT_MEMORY( pGrid );

    IUS_ASSERT_VALUE( dim0 >= 0 && dim1 >= 0 && dim2 >= 0 );

    pGrid->numPoints0 = dim0;
    pGrid->numPoints1 = dim1;
    pGrid->numPoints2 = dim2;

    pGrid->pPoints0 = NULL;
    pGrid->pPoints1 = NULL;
    pGrid->pPoints2 = NULL;

    if ( dim0 > 0 )
    {
        pGrid->pPoints0 = iusAllocFloatVector( pGrid->numPoints0  );
    }
    if ( dim1 > 0 )
    {
        pGrid->pPoints1 = iusAllocFloatVector( pGrid->numPoints1  );
    }
    if ( dim2 > 0 )
    {
        pGrid->pPoints2 = iusAllocFloatVector( pGrid->numPoints2  );
    }

    return pGrid;
}

//-----------------------------------------------------------------------------
// Frees memory of an IusGrid
//-----------------------------------------------------------------------------
void iusFreeGrid
(
    IusGrid * pGrid
)
{
    IUS_ASSERT_MEMORY( pGrid );

    if ( pGrid->numPoints2 > 0 )
    {
        iusFreeFloatVector( pGrid->pPoints2 );
    }
    if ( pGrid->numPoints1 > 0 )
    {
        iusFreeFloatVector( pGrid->pPoints1 );
    }
    if ( pGrid->numPoints0 > 0 )
    {
        iusFreeFloatVector( pGrid->pPoints0 );
    }
    free( pGrid );
}

//-----------------------------------------------------------------------------
// Copy all internals from one IusGrid to another
//-----------------------------------------------------------------------------
void iusCopyGrid
(
    IusGrid * const       pGridDest,
    const IusGrid * const pGridSrc
)
{
    IUS_ASSERT_MEMORY( pGridSrc  && pGridDest );
    IUS_ASSERT( pGridSrc != (const IusGrid * const)pGridDest );

    IUS_ASSERT_VALUE( pGridDest->numPoints0 == pGridSrc->numPoints0 );
    IUS_ASSERT_VALUE( pGridDest->numPoints1 == pGridSrc->numPoints1 );
    IUS_ASSERT_VALUE( pGridDest->numPoints2 == pGridSrc->numPoints2 );

    if ( pGridSrc->numPoints0 > 0 )
    {
        iusCopyFloatVector( pGridDest->pPoints0, pGridSrc->pPoints0,
            pGridSrc->numPoints0 );
    }
    if ( pGridSrc->numPoints1 > 0 )
    {
        iusCopyFloatVector( pGridDest->pPoints1, pGridSrc->pPoints1,
            pGridSrc->numPoints1 );
    }
    if ( pGridSrc->numPoints2 > 0 )
    {
        iusCopyFloatVector( pGridDest->pPoints2, pGridSrc->pPoints2,
            pGridSrc->numPoints2 );
    }
}


