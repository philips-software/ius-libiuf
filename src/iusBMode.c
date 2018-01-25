#include "iusBasicFunctions.h"
#include "iusBMode.h"
#include "iusError.h"
#include <stdlib.h>
#include <string.h>


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IusBModeInstance * iusBModeCreate
(
    const IusGrid * const pPixelGrid,
    float                 pixelsPerMeter,
    int                   interpolationMethod
)
{
    IusBModeInstance * pInst = 
        (IusBModeInstance *)malloc( sizeof( IusBModeInstance ) );
    
    IUS_ASSERT_MEMORY( pInst != NULL );
    IUS_ASSERT_MEMORY( pPixelGrid != NULL );

    IUS_ASSERT_VALUE( pixelsPerMeter > 0.0f );
    IUS_ASSERT_VALUE( pPixelGrid->numPoints2 > 0.0f );
    IUS_ASSERT_VALUE( pPixelGrid->numPoints1 > 0.0f );
    IUS_ASSERT_MEMORY( pPixelGrid->pPoints2 );
    IUS_ASSERT_MEMORY( pPixelGrid->pPoints1 );

    pInst->pPixelGrid = iusCreateGrid( pPixelGrid->numPoints0, 
                                       pPixelGrid->numPoints1, 
                                       pPixelGrid->numPoints2 );
    iusCopyGrid( pInst->pPixelGrid, pPixelGrid ); 
    pInst->pixelsPerMeter      = pixelsPerMeter;
    pInst->interpolationMethod = interpolationMethod;

  return pInst;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IusBModeInstance * iusBModeRead
(
    hid_t handle,
    int   verbose
)
{
    int                i, num;
    float              dx, x1;
    herr_t             status;
    IusBModeInstance * pInst;
    
    pInst = (IusBModeInstance *)malloc( sizeof( IusBModeInstance ) );

    IUS_ASSERT_MEMORY( pInst != NULL );

    status = iusHdf5ReadFloat( handle, "pixelsPerMeter",
                               &(pInst->pixelsPerMeter), verbose);
    status |= iusHdf5ReadGrid( handle, "Cartesian", pInst->pPixelGrid, verbose);
    status |= iusHdf5ReadInt( handle,  "interpolationMethod",
                              &(pInst->interpolationMethod),verbose);
    status |= iusHdf5ReadInt( handle,  "IUSVersion", &(pInst->IusVersion),
                              verbose);

    //check status
    IUS_ASSERT_VALUE( status == 0 );

    strcpy( pInst->iusNode.pType, IUS_BMODE_TYPE );
    iusNodeLoadParents( (IusNode *)pInst, handle );

    return pInst;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int iusBModeWrite
(
    hid_t                    handle,
    IusBModeInstance * const pBMode,
    int                      verbose
);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void iusBModeDestroy
(
    IusBModeInstance * pInst
)
{
    IUS_ASSERT_MEMORY( pInst );
    IUS_ASSERT_MEMORY( pInst->pPixelGrid );
    iusFreeGrid( pInst->pPixelGrid );
    free( pInst );
}

