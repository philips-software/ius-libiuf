#include <stdio.h>
#include <string.h>
#include <hdf5.h>
#include <hdf5_hl.h>

#include <ius.h>


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
static int verbose = IUS_FALSE;

herr_t iusHdf5ReadFloat
(
    hid_t        handle,
    const char * pVariableString,
    float *      pValue
)
{
    herr_t status;

    IUS_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_float( handle, pVariableString, pValue );
    if ( status < 0 )
    {
        fprintf( stderr, "H5LTread_dataset_float %s failed\n", pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %f\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5ReadShort
(
    hid_t        handle,
    const char * pVariableString,
    short *      pValue
)
{
    herr_t status;

    IUS_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_short( handle, pVariableString, pValue );
    if ( status < 0 )
    {
        fprintf( stderr, "H5LTread_dataset_short %s failed\n", pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %d\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5ReadInt
(
    hid_t        handle,
    const char * pVariableString,
    int *        pValue
)
{
    herr_t status;

    IUS_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_int( handle, pVariableString, pValue );
    if ( status < 0 )
    {
        fprintf( stderr, "H5LTread_dataset_int %s failed\n", pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %d\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5ReadLong
(
    hid_t        handle,
    const char * pVariableString,
    long int *   pValue
)
{
    herr_t status;

    IUS_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_long(handle, pVariableString, pValue);
    if ( status < 0 )
    {
        fprintf( stderr, "iusInputFileOpen: H5LTread_dataset_int %s failed\n",
                 pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %ld\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5ReadString
(
    hid_t        handle,
    const char * pVariableString,
    char * ppReturnString
)
{
    herr_t status;
    size_t strLength;

//    IUS_ASSERT_MEMORY( pVariableString && ppReturnString );
    status = H5LTread_dataset_string( handle, pVariableString, ppReturnString );
    if ( status < 0 )
    {
        fprintf( stderr,"iusInputFileOpen: H5LTread_dataset_string %s failed\n",
                 pVariableString );
        return status;
    }
    if ( verbose )
    {
        strLength = strlen(pVariableString);
        fprintf( stdout, "read: %s [%d]: %s\n", pVariableString, (int) strLength,
                 ppReturnString );
    }
    return status;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5ReadGridSize
(
hid_t        handle,
const char * pGridName,
int *        pDim0,
int *        pDim1,
int *        pDim2
)
{
    herr_t status = -1;
    char pVariableNameDim1[128];
    char pVariableNameDim2[128];

    IUS_ASSERT_MEMORY( pGridName && pDim0 && pDim1 && pDim2 );

    if ( strcmp( pGridName, "PolarGrid" ) == 0 )
    {
        sprintf( pVariableNameDim2, "%s/numPointsRadial", pGridName );
        sprintf( pVariableNameDim1, "%s/numPointsTheta", pGridName );
        status  = iusHdf5ReadInt( handle, pVariableNameDim2, pDim2 );
        status |= iusHdf5ReadInt( handle, pVariableNameDim1, pDim1 );
        *pDim0 = 0;  // elevation dimension not yet supported
    }
    else if ( strcmp( pGridName, "CartesianGrid" ) == 0 )
    {
        sprintf( pVariableNameDim2, "%s/numPointsZ", pGridName );
        sprintf( pVariableNameDim1, "%s/numPointsX", pGridName );
        status  = iusHdf5ReadInt( handle, pVariableNameDim2, pDim2 );
        status |= iusHdf5ReadInt( handle, pVariableNameDim1, pDim1 );
        *pDim0 = 0;  // elevation dimension not yet supported
    }
    else
    {
        fprintf( stderr, "Error: Grid type %s, is not supported\n", pGridName );
    }

    return status;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
herr_t iusHdf5ReadGrid
(
    hid_t        handle,
    const char * pGridName,
    IusGrid *    pGrid
)
{
    herr_t status = -1;
    char pVariableNameDim1[128];
    char pVariableNameDim2[128];
    int  dims[3];

    IUS_ASSERT_MEMORY( pGrid && pGridName );

    if (strcmp(pGridName, "PolarGrid") == 0 ||
    strcmp(pGridName, "CartesianGrid") == 0)
    {
        status = iusHdf5ReadGridSize( handle, pGridName, &dims[0], &dims[1],
                                      &dims[2] );
        if ( status < 0 )
        {
            fprintf( stderr,
                     "Error reading dataset: H5Dread returned: %d\n", status );
            return status;
        }

        IUS_ASSERT_VALUE( dims[0] == pGrid->numPoints0 );
        IUS_ASSERT_VALUE( dims[1] == pGrid->numPoints1 );
        IUS_ASSERT_VALUE( dims[2] == pGrid->numPoints2 );

        status  = H5LTread_dataset_float( handle, pVariableNameDim2,
                                          pGrid->pPoints2 );
        status |= H5LTread_dataset_float( handle, pVariableNameDim1,
                                          pGrid->pPoints1 );
        //TODO status = H5LTread_dataset_float( handle, pointsZName,
        //         pGrid.pointsZ );
        if ( status < 0 )
        {
            fprintf( stderr,
                     "Error reading dataset: H5Dread returned: %d\n", status );
            return status;
        }
    }
    else
    {
        fprintf(stderr, "Error: Grid type %s, is not supported\n", pGridName);
        return status;
    }

    return status;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5WriteFloat
(
    hid_t handle,
    const char *  pVariableString,
    float * pValues,
    int numValues
)
{
    herr_t   returnValue;
    hsize_t  dims[1];

    IUS_ASSERT_MEMORY( pVariableString && pValues );

    dims[0] = (hsize_t)numValues;

    returnValue =
    H5LTmake_dataset_float( handle, pVariableString, 1, dims, pValues );

    if (returnValue !=0 )
    {
        fprintf(stderr, "Error: iusHdf5WriteFloat error: %d\n", returnValue);
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5WriteInt
(
    hid_t              handle,
    const char * pVariableString,
    int * pValues,
    int  numValues
)
{
    herr_t returnValue;
    hsize_t  dims[1];

    IUS_ASSERT_MEMORY( pVariableString && pValues );

    dims[0] = (hsize_t)numValues;

    returnValue =
    H5LTmake_dataset_int( handle, pVariableString, 1, dims, pValues );
    if ( returnValue != 0 )
    {
        fprintf( stderr, "Error: iusHdf5WriteInt error: %d\n", returnValue );
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5WriteLong
(
    hid_t handle,
    const char * pVariableString,
    long * pValues,
    int  numValues
)
{
    herr_t returnValue;
    hsize_t dims[1];

    IUS_ASSERT_MEMORY( pVariableString && pValues );

    dims[0] = (hsize_t)numValues;

    returnValue =
    H5LTmake_dataset_long( handle, pVariableString, 1, dims, pValues );
    if ( returnValue != 0 )
    {
        fprintf(stderr, "Error: iusHdf5WriteLong error: %d\n", returnValue );
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5WriteString
(
hid_t              handle,
const char * const pVariableString,
const char * const pString
)
{
    herr_t returnValue;
    IUS_ASSERT_MEMORY( pVariableString && pString );

    returnValue = H5LTmake_dataset_string( handle, pVariableString, pString );
    if ( returnValue != 0 )
    {
        fprintf( stderr, "Error: iusHdf5WriteLong error: %d\n", returnValue );
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iusHdf5WriteGrid
(
    hid_t        handle,
    const char * pGridName,
    IusGrid *    pGrid
)
{
    herr_t returnValue;
    char pVariableNameDim[128];
    hsize_t dims[1];

    IUS_ASSERT_MEMORY( pGrid && pGridName );

    dims[0] = (hsize_t)1;

    if ( strcmp( pGridName, "PolarGrid" ) == 0 )
    {
        sprintf(pVariableNameDim, "%s/numPointsRadial",pGridName );
        returnValue = H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                            &dims[0], &(pGrid->numPoints2) );
        sprintf(pVariableNameDim, "%s/numPointsTheta", pGridName );
        returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                             &dims[0], &(pGrid->numPoints1) );
        //sprintf(pVariableNameDim, "%s/numPointsPhi");
        //returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
        //    &dim[0], &(pGrid->numPoints0) );
    }
    else if ( strcmp( pGridName, "CartesianGrid" ) == 0 )
    {
        sprintf( pVariableNameDim, "%s/numPointsZ", pGridName );
        returnValue = H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                            &dims[0], &(pGrid->numPoints2) );
        sprintf( pVariableNameDim, "%s/numPointsX", pGridName );
        returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                             &dims[0], &(pGrid->numPoints1) );
        //sprintf(pVariableNameDim, "%s/numPointsY");
        //returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
        //                                     &dims[0], &(pGrid->numPoints0) );
    }
    else
    {
        returnValue = -1;
        fprintf( stderr, "Error: iusHdf5WriteGrid, Grid name unknown (%s)\n",
                 pGridName );
    }

    if ( verbose )
    {
        printf( "iusHdf5WriteGrid: writing grid to hdf5\n" );
    }

    if ( returnValue != 0 )
    {
        fprintf( stderr, "Error: iusHdf5WriteGrid error: %d\n", returnValue );
    }

    return returnValue;
}


herr_t iusHdf5DisableMessages()
{
    // Turn off error handling permanently
    return H5Eset_auto (H5E_DEFAULT, NULL, NULL);
}
